#include "coreengine/mainapp.h"
#include "coreengine/settings.h"

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
    moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() / 2 - m_OkButton->getWidth() / 2,
                            Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigFinished();
    });
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont48());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    m_pointsAddedLabel = spLabel::create(Settings::getWidth() / 2 + 3 * 50 - 30);
    m_pointsAddedLabel->setStyle(style);
    m_pointsAddedLabel->setHtmlText("Score: " + QString::number(m_pointsToAdd));
    m_pointsAddedLabel->setPosition(Settings::getWidth() / 2 - 3 * 50, Settings::getHeight() / 2 - 50);
    pSpriteBox->addChild(m_pointsAddedLabel);
    m_totalPointsLabel = spLabel::create(Settings::getWidth() / 2 + 3 * 50 - 30);
    m_totalPointsLabel->setStyle(style);
    m_totalPointsLabel->setHtmlText("Total: " + QString::number(m_totalPoints));
    m_totalPointsLabel->setPosition(Settings::getWidth() / 2 - 3 * 50, Settings::getHeight() / 2 + 10);
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
