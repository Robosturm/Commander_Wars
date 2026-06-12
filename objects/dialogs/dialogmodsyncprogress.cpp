#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/dialogs/dialogmodsyncprogress.h"

#include "coreengine/interpreter.h"
#include "coreengine/mainapp.h"

#include "objects/base/label.h"

#include "resource_management/objectmanager.h"

#include "ui_reader/uifactory.h"

namespace
{
    constexpr qint32 kBarWidth = 480;
    constexpr qint32 kBarHeight = 24;

    const QColor kBarBackgroundColor{100, 100, 100, 200};
    const QColor kBarFillColor{35, 180, 80, 255};

    constexpr double kBytesPerKib = 1024.0;
    constexpr double kMsPerSecond = 1000.0;
    constexpr qint64 kSecondsPerMinute = 60;
    constexpr qint64 kMinutesPerHour = 60;

    // 50 ms minimum sample window so a hot LAN burst doesn't seed the EMA at multi-GB/s.
    constexpr qint64 kRateMinDtMs = 50;
    constexpr double kRateEmaAlpha = 0.3;

    constexpr qint32 kSizeDecimalPlaces = 1;
    // GB steps are coarse enough that a second decimal carries real information.
    constexpr qint32 kGbDecimalPlaces = 2;
    // Shown while a rate or ETA is not yet computable.
    constexpr const char* const kUnknownPlaceholder = QT_TRANSLATE_NOOP("DialogModSyncProgress", "--");

    // JS global the XML cancel button calls into.
    constexpr const char* const kModSyncProgressItem = "modSyncProgress";
    // Label Id in dialogModSyncProgress.xml.
    const auto kDetailLabelId = QStringLiteral("modSyncDetail");
}

DialogModSyncProgress::DialogModSyncProgress(qint32 totalMods)
    : m_totalMods(totalMods)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DialogModSyncProgress");
#endif
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // The fill bar mutates every progress tick; the XML system cannot hand back a mutable ColoredRect, so both bar rects stay code-built.
    m_barWidth = kBarWidth;
    const qint32 barX = oxygine::Stage::getStage()->getWidth() / 2 - m_barWidth / 2;
    const qint32 barY = oxygine::Stage::getStage()->getHeight() / 2 - kBarHeight / 2;

    m_BarBackground = MemoryManagement::create<oxygine::ColorRectSprite>();
    m_BarBackground->setSize(m_barWidth, kBarHeight);
    m_BarBackground->setColor(kBarBackgroundColor);
    m_BarBackground->setPosition(barX, barY);
    pSpriteBox->addChild(m_BarBackground);

    m_BarFill = MemoryManagement::create<oxygine::ColorRectSprite>();
    m_BarFill->setSize(0, kBarHeight);
    m_BarFill->setColor(kBarFillColor);
    m_BarFill->setPosition(barX, barY);
    pSpriteBox->addChild(m_BarFill);

    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->setGlobal(kModSyncProgressItem, pInterpreter->newQObject(this));
    UiFactory::getInstance().createUi("ui/dialogModSyncProgress.xml", this);
    setDetailText(tr("0 / %1 mods").arg(m_totalMods));

    connect(this, &DialogModSyncProgress::sigCancel, this, &DialogModSyncProgress::remove, Qt::QueuedConnection);
    m_timer.start();
}

DialogModSyncProgress::~DialogModSyncProgress()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (pInterpreter != nullptr)
    {
        pInterpreter->deleteObject(kModSyncProgressItem);
    }
}

void DialogModSyncProgress::cancel()
{
    emit sigCancel();
}

void DialogModSyncProgress::setDetailText(const QString & text)
{
    Label* pDetail = getCastedObject<Label>(kDetailLabelId);
    // createUi failure is non-fatal; progress math keeps running without the label.
    if (pDetail != nullptr)
    {
        pDetail->setHtmlText(text);
    }
}

void DialogModSyncProgress::setExpectedTotalBytes(qint64 expectedUncompressed)
{
    if (expectedUncompressed < 0)
    {
        expectedUncompressed = 0;
    }
    // Manifest is canonical and one-shot in the wire-correct flow; refuse downward revisions so the bar stays monotonic against intentional or accidental shrinkage.
    if (m_expectedTotalBytes > 0 && expectedUncompressed < m_expectedTotalBytes)
    {
        return;
    }
    m_expectedTotalBytes = expectedUncompressed;
}

void DialogModSyncProgress::setProgress(qint32 stagedMods, qint64 receivedCompressed, qint64 receivedUncompressed)
{
    if (stagedMods < 0)
    {
        stagedMods = 0;
    }
    if (m_totalMods > 0 && stagedMods > m_totalMods)
    {
        stagedMods = m_totalMods;
    }
    if (receivedCompressed < 0)
    {
        receivedCompressed = 0;
    }
    if (receivedUncompressed < 0)
    {
        receivedUncompressed = 0;
    }

    // lastSampleMs starts at 0 (constructor t0) so the first real sample is always rate-eligible.
    const qint64 nowMs = m_timer.isValid() ? m_timer.elapsed() : 0;
    const qint64 dtMs = nowMs - m_lastSampleMs;
    const qint64 dCompressed = receivedCompressed - m_lastReceivedCompressed;
    const qint64 dUncompressed = receivedUncompressed - m_lastReceivedUncompressed;
    if (dtMs >= kRateMinDtMs && dCompressed >= 0 && dUncompressed >= 0)
    {
        const double dtSec = static_cast<double>(dtMs) / kMsPerSecond;
        const double instantCompressed = static_cast<double>(dCompressed) / dtSec;
        const double instantUncompressed = static_cast<double>(dUncompressed) / dtSec;
        m_smoothedRateBytesPerSec = kRateEmaAlpha * instantCompressed + (1.0 - kRateEmaAlpha) * m_smoothedRateBytesPerSec;
        m_smoothedUncompressedRateBytesPerSec = kRateEmaAlpha * instantUncompressed + (1.0 - kRateEmaAlpha) * m_smoothedUncompressedRateBytesPerSec;
        m_lastSampleMs = nowMs;
        m_lastReceivedCompressed = receivedCompressed;
        m_lastReceivedUncompressed = receivedUncompressed;
    }

    float targetFraction = 0.0f;
    if (m_expectedTotalBytes > 0)
    {
        targetFraction = static_cast<float>(static_cast<double>(receivedUncompressed) / static_cast<double>(m_expectedTotalBytes));
    }
    else if (m_totalMods > 0)
    {
        targetFraction = static_cast<float>(stagedMods) / static_cast<float>(m_totalMods);
    }
    if (targetFraction < 0.0f)
    {
        targetFraction = 0.0f;
    }
    if (targetFraction > 1.0f)
    {
        targetFraction = 1.0f;
    }
    if (targetFraction < m_lastDisplayedFraction)
    {
        targetFraction = m_lastDisplayedFraction;
    }
    m_lastDisplayedFraction = targetFraction;
    m_BarFill->setWidth(static_cast<qint32>(m_barWidth * targetFraction));

    QString detail;
    if (m_expectedTotalBytes > 0)
    {
        detail = tr("%1 / %2 mods, %3 / %4, %5, ETA %6")
                 .arg(stagedMods)
                 .arg(m_totalMods)
                 .arg(formatBytes(receivedUncompressed))
                 .arg(formatBytes(m_expectedTotalBytes))
                 .arg(formatRate(static_cast<qint64>(m_smoothedRateBytesPerSec)))
                 .arg(formatEta(m_smoothedUncompressedRateBytesPerSec > 0.0
                                ? static_cast<qint64>(static_cast<double>(m_expectedTotalBytes - receivedUncompressed) / m_smoothedUncompressedRateBytesPerSec)
                                : -1));
    }
    else
    {
        detail = tr("%1 / %2 mods, %3, %4")
                 .arg(stagedMods)
                 .arg(m_totalMods)
                 .arg(formatBytes(receivedUncompressed))
                 .arg(formatRate(static_cast<qint64>(m_smoothedRateBytesPerSec)));
    }
    setDetailText(detail);
}

void DialogModSyncProgress::remove()
{
    detach();
}

QString DialogModSyncProgress::formatBytes(qint64 bytes)
{
    if (bytes < kBytesPerKib)
    {
        return tr("%1 B").arg(bytes);
    }
    const double kb = static_cast<double>(bytes) / kBytesPerKib;
    if (kb < kBytesPerKib)
    {
        return tr("%1 KB").arg(QString::number(kb, 'f', kSizeDecimalPlaces));
    }
    const double mb = kb / kBytesPerKib;
    if (mb < kBytesPerKib)
    {
        return tr("%1 MB").arg(QString::number(mb, 'f', kSizeDecimalPlaces));
    }
    const double gb = mb / kBytesPerKib;
    return tr("%1 GB").arg(QString::number(gb, 'f', kGbDecimalPlaces));
}

QString DialogModSyncProgress::formatRate(qint64 bytesPerSecond)
{
    if (bytesPerSecond <= 0)
    {
        return tr(kUnknownPlaceholder);
    }
    return tr("%1/s").arg(formatBytes(bytesPerSecond));
}

QString DialogModSyncProgress::formatEta(qint64 seconds)
{
    if (seconds < 0)
    {
        return tr(kUnknownPlaceholder);
    }
    if (seconds < kSecondsPerMinute)
    {
        return tr("%1s").arg(seconds);
    }
    const qint64 minutes = seconds / kSecondsPerMinute;
    const qint64 remSec = seconds % kSecondsPerMinute;
    if (minutes < kMinutesPerHour)
    {
        return tr("%1m %2s").arg(minutes).arg(remSec);
    }
    const qint64 hours = minutes / kMinutesPerHour;
    const qint64 remMin = minutes % kMinutesPerHour;
    return tr("%1h %2m").arg(hours).arg(remMin);
}
