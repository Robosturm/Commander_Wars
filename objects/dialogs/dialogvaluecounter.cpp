#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "objects/dialogs/dialogvaluecounter.h"

DialogValueCounter::DialogValueCounter(qint32 totalPoints, qint32 pointsToAdd)
    : m_totalPoints(totalPoints),
      m_pointsToAdd(pointsToAdd),
      m_updateTimer(this)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DialogValueCounter");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_OkButton->getScaledWidth() / 2,
                            oxygine::Stage::getStage()->getHeight() - 30 - m_OkButton->getScaledHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigFinished();
    });
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont48());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    m_pointsAddedLabel = MemoryManagement::create<Label>(oxygine::Stage::getStage()->getWidth() / 2 + 3 * 50 - 30);
    m_pointsAddedLabel->setStyle(style);
    m_pointsAddedLabel->setHtmlText("Score: " + QString::number(m_pointsToAdd));
    m_pointsAddedLabel->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - 3 * 50, oxygine::Stage::getStage()->getHeight() / 2 - 50);
    pSpriteBox->addChild(m_pointsAddedLabel);
    m_totalPointsLabel = MemoryManagement::create<Label>(oxygine::Stage::getStage()->getWidth() / 2 + 3 * 50 - 30);
    m_totalPointsLabel->setStyle(style);
    m_totalPointsLabel->setHtmlText("Total: " + QString::number(m_totalPoints));
    m_totalPointsLabel->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - 3 * 50, oxygine::Stage::getStage()->getHeight() / 2 + 10);
    pSpriteBox->addChild(m_totalPointsLabel);

    m_updateTimer.setInterval(15);
    m_updateTimer.setSingleShot(false);
    connect(&m_updateTimer, &QTimer::timeout, this, &DialogValueCounter::updateData, Qt::QueuedConnection);
    m_updateTimer.start();

    connect(this, &DialogValueCounter::sigFinished, this, &DialogValueCounter::remove, Qt::QueuedConnection);
    pApp->continueRendering();
}

void DialogValueCounter::remove()
{
    detach();
}

void DialogValueCounter::updateData()
{
    for (qint32 i = 0; i < 3; ++i)
    {
        if (m_pointsToAdd > 0)
        {
            --m_pointsToAdd;
            ++m_totalPoints;
        }
    }
    if (m_pointsToAdd <= 0)
    {
        m_updateTimer.stop();
    }
    m_pointsAddedLabel->setHtmlText("Score: " + QString::number(m_pointsToAdd));
    m_totalPointsLabel->setHtmlText("Total: " + QString::number(m_totalPoints));
}
