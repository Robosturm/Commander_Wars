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
}

void DialogModSyncProgress::setProgress(qint32 stagedMods, qint64 receivedBytes)
{
    if (m_totalMods <= 0)
    {
        return;
    }
    if (stagedMods < 0)
    {
        stagedMods = 0;
    }
    if (stagedMods > m_totalMods)
    {
        stagedMods = m_totalMods;
    }
    const float fraction = static_cast<float>(stagedMods) / static_cast<float>(m_totalMods);
    m_BarFill->setWidth(m_barWidth * fraction);
    const qint64 receivedKb = receivedBytes / 1024;
    m_Detail->setHtmlText(tr("%1 / %2 mods (%3 KB)").arg(stagedMods).arg(m_totalMods).arg(receivedKb));
    m_Detail->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_Detail->getTextRect().width() / 2,
                          m_Detail->getY());
}

void DialogModSyncProgress::remove()
{
    detach();
}
