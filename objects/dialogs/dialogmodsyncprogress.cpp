#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/dialogs/dialogmodsyncprogress.h"

#include "coreengine/interpreter.h"
#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

DialogModSyncProgress::DialogModSyncProgress(qint32 totalMods)
    : QObject(),
      m_totalMods(totalMods)
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

    oxygine::TextStyle headerStyle = oxygine::TextStyle(FontManager::getMainFont24());
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    headerStyle.multiline = false;

    m_Header = MemoryManagement::create<oxygine::TextField>();
    m_Header->setStyle(headerStyle);
    m_Header->setHtmlText(tr("Downloading host's mod set"));
    m_Header->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_Header->getTextRect().width() / 2,
                          oxygine::Stage::getStage()->getHeight() / 2 - 80);
    pSpriteBox->addChild(m_Header);

    m_barWidth = 480;
    const qint32 barHeight = 24;
    const qint32 barX = oxygine::Stage::getStage()->getWidth() / 2 - m_barWidth / 2;
    const qint32 barY = oxygine::Stage::getStage()->getHeight() / 2 - barHeight / 2;

    m_BarBackground = MemoryManagement::create<oxygine::ColorRectSprite>();
    m_BarBackground->setSize(m_barWidth, barHeight);
    m_BarBackground->setColor(QColor(100, 100, 100, 200));
    m_BarBackground->setPosition(barX, barY);
    pSpriteBox->addChild(m_BarBackground);

    m_BarFill = MemoryManagement::create<oxygine::ColorRectSprite>();
    m_BarFill->setSize(0, barHeight);
    m_BarFill->setColor(QColor(35, 180, 80, 255));
    m_BarFill->setPosition(barX, barY);
    pSpriteBox->addChild(m_BarFill);

    oxygine::TextStyle detailStyle = oxygine::TextStyle(FontManager::getMainFont24());
    detailStyle.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    detailStyle.multiline = false;

    m_Detail = MemoryManagement::create<oxygine::TextField>();
    m_Detail->setStyle(detailStyle);
    m_Detail->setHtmlText(tr("0 / %1 mods").arg(m_totalMods));
    m_Detail->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_Detail->getTextRect().width() / 2,
                          barY + barHeight + 10);
    pSpriteBox->addChild(m_Detail);

    m_CancelButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_CancelButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_CancelButton->getScaledWidth() / 2,
                                barY + barHeight + 60);
    pSpriteBox->addChild(m_CancelButton);
    m_CancelButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigCancel();
    });
    connect(this, &DialogModSyncProgress::sigCancel, this, &DialogModSyncProgress::remove, Qt::QueuedConnection);
    m_timer.start();
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

    // 50 ms minimum sample window so a hot LAN burst doesn't seed the EMA at multi-GB/s; lastSampleMs starts at 0 (constructor t0) so the first real sample is always rate-eligible.
    constexpr qint64 RATE_MIN_DT_MS = 50;
    constexpr double alpha = 0.3;
    const qint64 nowMs = m_timer.isValid() ? m_timer.elapsed() : 0;
    const qint64 dtMs = nowMs - m_lastSampleMs;
    const qint64 dCompressed = receivedCompressed - m_lastReceivedCompressed;
    const qint64 dUncompressed = receivedUncompressed - m_lastReceivedUncompressed;
    if (dtMs >= RATE_MIN_DT_MS && dCompressed >= 0 && dUncompressed >= 0)
    {
        const double dtSec = static_cast<double>(dtMs) / 1000.0;
        const double instantCompressed = static_cast<double>(dCompressed) / dtSec;
        const double instantUncompressed = static_cast<double>(dUncompressed) / dtSec;
        m_smoothedRateBytesPerSec = alpha * instantCompressed + (1.0 - alpha) * m_smoothedRateBytesPerSec;
        m_smoothedUncompressedRateBytesPerSec = alpha * instantUncompressed + (1.0 - alpha) * m_smoothedUncompressedRateBytesPerSec;
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
    m_Detail->setHtmlText(detail);
    m_Detail->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_Detail->getTextRect().width() / 2,
                          m_Detail->getY());
}

void DialogModSyncProgress::remove()
{
    detach();
}

QString DialogModSyncProgress::formatBytes(qint64 bytes)
{
    if (bytes < 1024)
    {
        return tr("%1 B").arg(bytes);
    }
    const double kb = static_cast<double>(bytes) / 1024.0;
    if (kb < 1024.0)
    {
        return tr("%1 KB").arg(QString::number(kb, 'f', 1));
    }
    const double mb = kb / 1024.0;
    if (mb < 1024.0)
    {
        return tr("%1 MB").arg(QString::number(mb, 'f', 1));
    }
    const double gb = mb / 1024.0;
    return tr("%1 GB").arg(QString::number(gb, 'f', 2));
}

QString DialogModSyncProgress::formatRate(qint64 bytesPerSecond)
{
    if (bytesPerSecond <= 0)
    {
        return tr("--");
    }
    return tr("%1/s").arg(formatBytes(bytesPerSecond));
}

QString DialogModSyncProgress::formatEta(qint64 seconds)
{
    if (seconds < 0)
    {
        return tr("--");
    }
    if (seconds < 60)
    {
        return tr("%1s").arg(seconds);
    }
    const qint64 minutes = seconds / 60;
    const qint64 remSec = seconds % 60;
    if (minutes < 60)
    {
        return tr("%1m %2s").arg(minutes).arg(remSec);
    }
    const qint64 hours = minutes / 60;
    const qint64 remMin = minutes % 60;
    return tr("%1h %2m").arg(hours).arg(remMin);
}
