#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/loadingscreen.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/fontmanager.h"

spLoadingScreen LoadingScreen::m_pLoadingScreen;

spLoadingScreen LoadingScreen::getInstance()
{
    if (m_pLoadingScreen.get() == nullptr)
    {
        m_pLoadingScreen = spLoadingScreen::create();
    }
    return m_pLoadingScreen;
}

LoadingScreen::LoadingScreen()
{
    setObjectName("LoadingScreen");
    Interpreter::setCppOwnerShip(this);
    setPriority(static_cast<quint16>(Mainapp::ZOrder::Loadingscreen));
}
LoadingScreen::~LoadingScreen()
{
    CONSOLE_PRINT("LoadingScreen::deleted", Console::eDEBUG);
}

void LoadingScreen::show()
{    
    oxygine::Stage::getStage()->addChild(spLoadingScreen(this));
    removeChildren();
    oxygine::ResAnim* pBackground;
    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    pBackground = pBackgroundManager->getResAnim("loadingscreen");
    // load background
    oxygine::spSprite sprite = oxygine::spSprite::create();
    addChild(sprite);
    if (pBackground != nullptr)
    {
        sprite->setResAnim(pBackground);
        // background should be last to draw
        sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
        sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
        sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());
    }
    m_BackgroundBar = oxygine::spColorRectSprite::create();
    m_BackgroundBar->setSize(Settings::getWidth(), 60);
    m_BackgroundBar->setY(Settings::getHeight() - 60);
    m_BackgroundBar->setColor(Qt::gray);
    addChild(m_BackgroundBar);

    m_LoadingBar = oxygine::spColorRectSprite::create();
    m_LoadingBar->setSize(Settings::getWidth(), 60);
    m_LoadingBar->setY(Settings::getHeight() - 60);
    m_LoadingBar->setColor(Qt::red);
    addChild(m_LoadingBar);

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    style.multiline = true;
    m_workText = oxygine::spTextField::create();
    m_workText->setStyle(style);
    m_workText->setWidth(Settings::getWidth() - 20);
    m_workText->setX(20);
    m_workText->setY(Settings::getHeight() / 2);
    addChild(m_workText);
    m_loadingProgress = oxygine::spTextField::create();
    m_loadingProgress->setStyle(style);
    m_loadingProgress->setPosition(Settings::getWidth() / 2 - 40, Settings::getHeight() - 50);
    addChild(m_loadingProgress);
    m_workText->setHtmlText("Loading...");
    m_loadingProgress->setHtmlText("0 %");
    m_LoadingBar->setWidth(1);
    setVisible(true);    
}

void LoadingScreen::setProgress(QString workText, qint32 value)
{
    CONSOLE_PRINT("LoadingScreen::setProgress " + workText + " " + QString::number(value), Console::eDEBUG);
    m_workText->setHtmlText(workText);
    m_loadingProgress->setHtmlText(QString::number(value) + " %");
    m_LoadingBar->setWidth(value * Settings::getWidth() / 100);
    if (Mainapp::getInstance()->isMainThread())
    {
        QApplication::processEvents();
    }
}

void LoadingScreen::setWorktext(QString workText)
{
    m_workText->setHtmlText(workText);
}

void LoadingScreen::hide()
{
    CONSOLE_PRINT("LoadingScreen::hide", Console::eDEBUG);
    m_pLoadingScreen->detach();
    m_pLoadingScreen = nullptr;
}
