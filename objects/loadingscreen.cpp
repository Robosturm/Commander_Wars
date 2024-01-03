#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/loadingscreen.h"

#include "coreengine/mainapp.h"
#include "coreengine/gameconsole.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/fontmanager.h"

spLoadingScreen LoadingScreen::m_pLoadingScreen{nullptr};

spLoadingScreen LoadingScreen::getInstance()
{
    if (m_pLoadingScreen.get() == nullptr)
    {
        m_pLoadingScreen = MemoryManagement::create<LoadingScreen>();
    }
    return m_pLoadingScreen;
}

LoadingScreen::LoadingScreen()
{
#ifdef GRAPHICSUPPORT
    setObjectName("LoadingScreen");
#endif
    Interpreter::setCppOwnerShip(this);
    setPriority(static_cast<quint16>(Mainapp::ZOrder::Loadingscreen));
}
LoadingScreen::~LoadingScreen()
{
    CONSOLE_PRINT("LoadingScreen::deleted", GameConsole::eDEBUG);
}

void LoadingScreen::show()
{
    if (oxygine::Stage::getStage().get() != nullptr)
    {
        CONSOLE_PRINT("LoadingScreen::show", GameConsole::eDEBUG);
        oxygine::Stage::getStage()->addChild(m_pLoadingScreen);
        removeChildren();
        oxygine::ResAnim* pBackground;
        BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
        pBackground = pBackgroundManager->getResAnim("loadingscreen");
        // load background
        oxygine::spSprite sprite = MemoryManagement::create<oxygine::Sprite>();
        addChild(sprite);
        if (pBackground != nullptr &&
            pBackground->getWidth() > 0 &&
            pBackground->getHeight() > 0)
        {
            sprite->setResAnim(pBackground);
            // background should be last to draw
            sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
            sprite->setScaleX(static_cast<float>(oxygine::Stage::getStage()->getWidth()) / static_cast<float>(pBackground->getWidth()));
            sprite->setScaleY(static_cast<float>(oxygine::Stage::getStage()->getHeight()) / static_cast<float>(pBackground->getHeight()));
        }
        m_BackgroundBar = MemoryManagement::create<oxygine::ColorRectSprite>();
        m_BackgroundBar->setSize(oxygine::Stage::getStage()->getWidth(), 60);
        m_BackgroundBar->setY(oxygine::Stage::getStage()->getHeight() - 60);
        m_BackgroundBar->setColor(Qt::gray);
        addChild(m_BackgroundBar);

        m_LoadingBar = MemoryManagement::create<oxygine::ColorRectSprite>();
        m_LoadingBar->setSize(oxygine::Stage::getStage()->getWidth(), 60);
        m_LoadingBar->setY(oxygine::Stage::getStage()->getHeight() - 60);
        m_LoadingBar->setColor(Qt::red);
        addChild(m_LoadingBar);

        oxygine::TextStyle style = oxygine::TextStyle(FontManager::getFont("mainBlack24"));
        style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
        style.multiline = true;
        m_workText = MemoryManagement::create<Label>(oxygine::Stage::getStage()->getWidth());
        m_workText->setStyle(style);
        m_workText->setX(0);
        m_workText->setY(oxygine::Stage::getStage()->getHeight() / 2);
        addChild(m_workText);
        m_loadingProgress = MemoryManagement::create<Label>(oxygine::Stage::getStage()->getWidth());
        m_loadingProgress->setStyle(style);
        m_loadingProgress->setPosition(0, oxygine::Stage::getStage()->getHeight() - 50);
        addChild(m_loadingProgress);
        m_workText->setHtmlText("Loading...");
        m_loadingProgress->setHtmlText("0 %");
        m_LoadingBar->setWidth(1);
        setVisible(true);
    }
}

void LoadingScreen::setProgress(QString workText, qint32 value)
{
    if (oxygine::Stage::getStage().get() != nullptr)
    {
        Mainapp* pApp = Mainapp::getInstance();
        CONSOLE_PRINT("LoadingScreen::setProgress " + workText + " " + QString::number(value), GameConsole::eDEBUG);
        m_workText->setHtmlText(workText);
        m_loadingProgress->setHtmlText(QString::number(value) + " %");
        m_LoadingBar->setWidth(value * oxygine::Stage::getStage()->getWidth() / 100);
        if (pApp->isMainThread())
        {
            QCoreApplication::processEvents(QEventLoop::ProcessEventsFlag::AllEvents, 5);
        }
    }
}

void LoadingScreen::setWorktext(QString workText)
{
    m_workText->setHtmlText(workText);
}

void LoadingScreen::hide()
{
    CONSOLE_PRINT("LoadingScreen::hide", GameConsole::eDEBUG);
    m_pLoadingScreen->detach();
    m_pLoadingScreen.reset();
}
