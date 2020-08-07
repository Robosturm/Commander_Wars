#include "loadingscreen.h"

#include "coreengine/mainapp.h"
#include "resource_management/backgroundmanager.h"
#include "resource_management/fontmanager.h"

spLoadingScreen LoadingScreen::m_pLoadingScreen = nullptr;

LoadingScreen* LoadingScreen::getInstance()
{
    if (m_pLoadingScreen.get() == nullptr)
    {
        m_pLoadingScreen = new LoadingScreen();
    }
    return m_pLoadingScreen.get();
}

LoadingScreen::LoadingScreen()
    : QObject()
{
    setPriority(static_cast<quint16>(Mainapp::ZOrder::Loadingscreen));
}

void LoadingScreen::show()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    oxygine::getStage()->addChild(this);
    if (!loaded)
    {
        loaded = true;
        BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
        // load background
        oxygine::spSprite sprite = new oxygine::Sprite();
        addChild(sprite);
        oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("Background+1");
        sprite->setResAnim(pBackground);
        sprite->setPosition(0, 0);
        // background should be last to draw
        sprite->setPriority(static_cast<short>(Mainapp::ZOrder::Background));
        sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
        sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());

        m_BackgroundBar = new oxygine::ColorRectSprite();
        m_BackgroundBar->setSize(Settings::getWidth(), 60);
        m_BackgroundBar->setY(Settings::getHeight() - 60);
        m_BackgroundBar->setColor(Qt::white);
        addChild(m_BackgroundBar);

        m_LoadingBar = new oxygine::ColorRectSprite();
        m_LoadingBar->setSize(Settings::getWidth(), 60);
        m_LoadingBar->setY(Settings::getHeight() - 60);
        m_LoadingBar->setColor(Qt::red);
        addChild(m_LoadingBar);

        oxygine::TextStyle style = FontManager::getMainFont24();
        style.color = FontManager::getFontColor();
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = true;
        m_workText = new oxygine::TextField();
        m_workText->setStyle(style);
        m_workText->setWidth(Settings::getWidth() / 3);
        m_workText->setX(Settings::getWidth() / 3);
        m_workText->setY(Settings::getHeight() / 2);
        addChild(m_workText);
        m_loadingProgress = new oxygine::TextField();
        m_loadingProgress->setStyle(style);
        m_loadingProgress->setPosition(Settings::getWidth() / 2 - 40, Settings::getHeight() - 50);
        addChild(m_loadingProgress);


    }
    m_workText->setText("Loading...");
    m_loadingProgress->setText("0 %");
    m_LoadingBar->setWidth(1);
    setVisible(true);
    triggerUpdate();
    pApp->continueThread();
}

void LoadingScreen::triggerUpdate()
{
    Mainapp* pApp = Mainapp::getInstance();
    qint32 counter = pApp->getLockCounter();
    for (qint32 i = 0; i < counter; i++)
    {
        pApp->continueThread();
    }
    m_semaphore.acquire();
    for (qint32 i = 0; i < counter; i++)
    {
        pApp->suspendThread();
    }
}

void LoadingScreen::setProgress(QString workText, qint32 value)
{
    m_workText->setText(workText);
    m_loadingProgress->setText(QString::number(value) + " %");
    m_LoadingBar->setWidth(value * Settings::getWidth() / 100);
    triggerUpdate();
}

void LoadingScreen::setWorktext(QString workText)
{
    m_workText->setText(workText);
    triggerUpdate();
}

void LoadingScreen::doUpdate(const oxygine::UpdateState& us)
{
    m_semaphore.release();
    oxygine::Actor::doUpdate(us);
}

void LoadingScreen::hide()
{
    m_pLoadingScreen->detach();
}
