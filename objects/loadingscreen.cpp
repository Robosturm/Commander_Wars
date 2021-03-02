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
    oxygine::getStage()->addChild(this);
    removeChildren();
    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("loadingscreen");
    sprite->setResAnim(pBackground);
    // background should be last to draw
    sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
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
    m_workText->setText("Loading...");
    m_loadingProgress->setText("0 %");
    m_LoadingBar->setWidth(1);
    setVisible(true);
    
}


void LoadingScreen::setProgress(QString workText, qint32 value)
{
    m_workText->setText(workText);
    m_loadingProgress->setText(QString::number(value) + " %");
    m_LoadingBar->setWidth(value * Settings::getWidth() / 100);

}

void LoadingScreen::setWorktext(QString workText)
{
    m_workText->setText(workText);

}


void LoadingScreen::hide()
{
    m_pLoadingScreen->detach();
}
