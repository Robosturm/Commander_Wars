#include "menue/wikimenu.h"
#include "menue/mainwindow.h"

#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/gameconsole.h"
#include "coreengine/audiomanager.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

Wikimenu::Wikimenu()
    : Basemenu()
{
#ifdef GRAPHICSUPPORT
    setObjectName("Wikimenu");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    Interpreter::setCppOwnerShip(this);
    CONSOLE_PRINT("Entering Wiki Menue", GameConsole::eDEBUG);
    changeBackground("wikimenu");
    pApp->getAudioManager()->clearPlayList();
    pApp->getAudioManager()->loadFolder("resources/music/hauptmenue");
    pApp->getAudioManager()->playRandom();

    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    addChild(pButtonExit);
    pButtonExit->setPosition(oxygine::Stage::getStage()->getWidth()  / 2.0f - pButtonExit->getScaledWidth() / 2.0f,
                             oxygine::Stage::getStage()->getHeight() - pButtonExit->getScaledHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    m_pWikiView = MemoryManagement::create<WikiView>(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(m_pWikiView);
    connect(this, &Wikimenu::sigExitMenue, this, &Wikimenu::exitMenue, Qt::QueuedConnection);
    pApp->continueRendering();
}

void Wikimenu::onEnter()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString object = "Init";
    QString func = "wikiMenu";
    if (pInterpreter->exists(object, func))
    {
        CONSOLE_PRINT("Executing:" + object + "." + func, GameConsole::eDEBUG);
        QJSValueList args({pInterpreter->newQObject(this)});
        pInterpreter->doFunction(object, func, args);
    }
}

void Wikimenu::exitMenue()
{
    CONSOLE_PRINT("Leaving Wiki Menue", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    auto window = MemoryManagement::create<Mainwindow>("ui/menu/mainmenu.xml");
    oxygine::Stage::getStage()->addChild(window);
    detachAndRemove();
}
