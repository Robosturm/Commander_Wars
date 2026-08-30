#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/loadingscreen.h"

#include "coreengine/mainapp.h"
#include "coreengine/gameconsole.h"

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
        auto* pApp = Mainapp::getInstance();
        pApp->pauseRendering();
        CONSOLE_PRINT("LoadingScreen::show", GameConsole::eDEBUG);
        oxygine::Stage::getStage()->addChild(m_pLoadingScreen);
        removeChildren();

        changeBackground("loadingscreen");

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
        pApp->continueRendering();
    }
}

void LoadingScreen::setProgress(QString workText, qint32 value)
{
    if (oxygine::Stage::getStage().get() != nullptr)
    {
        Mainapp* pApp = Mainapp::getInstance();
        if (!pApp->isMainThread())
        {
            pApp->pauseRendering();
        }
        CONSOLE_PRINT("LoadingScreen::setProgress " + workText + " " + QString::number(value), GameConsole::eDEBUG);
        m_workText->setHtmlText(workText);
        m_loadingProgress->setHtmlText(QString::number(value) + " %");
        m_LoadingBar->setWidth(value * oxygine::Stage::getStage()->getWidth() / 100);
        if (pApp->isMainThread())
        {
            QCoreApplication::processEvents(QEventLoop::ProcessEventsFlag::AllEvents, 5);
        }
        else
        {
            pApp->continueRendering();
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
    m_pLoadingScreen->detachAndRemove();
    m_pLoadingScreen.reset();
}
