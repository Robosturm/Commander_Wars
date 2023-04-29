#include <QDirIterator>
#include <QCoreApplication>

#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/Input.h"
#include "3rd_party/oxygine-framework/oxygine/TouchEvent.h"

#include "ai/aiprocesspipe.h"

#include "coreengine/mainapp.h"
#include "coreengine/workerthread.h"
#include "coreengine/gameconsole.h"
#include "coreengine/userdata.h"

#include "menue/mainwindow.h"

#include "game/gameanimation/gameanimationfactory.h"

#include "resource_management/terrainmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/movementtablemanager.h"
#include "resource_management/weaponmanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/gamerulemanager.h"
#include "resource_management/battleanimationmanager.h"
#include "resource_management/coperkmanager.h"
#include "resource_management/achievementmanager.h"
#include "resource_management/shoploader.h"
#include "resource_management/movementplanneraddinmanager.h"
#include "resource_management/uimanager.h"
#include "wiki/wikidatabase.h"

#include "objects/loadingscreen.h"

#include "ui_reader/uifactory.h"

WorkerThread::WorkerThread()
{
#ifdef GRAPHICSUPPORT
    setObjectName("WorkerThread");
#endif
    Interpreter::setCppOwnerShip(this);
    connect(this, &WorkerThread::sigStart, this, &WorkerThread::start, Qt::QueuedConnection);
    connect(this, &WorkerThread::sigShowMainwindow, this, &WorkerThread::showMainwindow, Qt::QueuedConnection);
    connect(this, &WorkerThread::sigStartSlaveGame, this, &WorkerThread::startSlaveGame, Qt::QueuedConnection);
    connect(Mainapp::getWorkerthread(), &QThread::finished, this, &WorkerThread::onQuit);
    moveToThread(Mainapp::getWorkerthread());
}

void WorkerThread::start()
{
    GameConsole::print("Loading worker thread", GameConsole::eDEBUG);
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    Mainapp* pApp = Mainapp::getInstance();
    spConsole pConsole = spConsole(GameConsole::getInstance());
    // create the initial menue no need to store the object
    // it will add itself to the current stage
    oxygine::Stage::getStage()->addChild(pConsole);
    Interpreter* pInterpreter = Interpreter::createInstance();
    Settings::setLanguage(Settings::getLanguage());
    pConsole->init();
    UiFactory::getInstance();
    // load General-Base Scripts
    QStringList searchPaths;
    searchPaths.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/scripts/general");
    searchPaths.append("resources/scripts/general");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        searchPaths.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + Settings::getMods().at(i) + "/scripts/general");
        searchPaths.append(Settings::getUserPath() + Settings::getMods().at(i) + "/scripts/general");
    }
    for (auto & path : searchPaths)
    {
        QStringList filter;
        filter << "*.js";
        QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter.hasNext())
        {
            dirIter.next();
            QString file = dirIter.fileInfo().canonicalFilePath();
            pInterpreter->openScript(file, true);
        }
    }
    pLoadingScreen->setProgress(tr("Loading Buildings..."), Mainapp::SCRIPT_PROCESS);
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    pBuildingSpriteManager->loadAll();
    pLoadingScreen->setProgress(tr("Loading COs..."), Mainapp::SCRIPT_PROCESS + 2);
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    pCOSpriteManager->loadAll();
    pLoadingScreen->setProgress(tr("Loading Gamescripts..."), Mainapp::SCRIPT_PROCESS + 4);
    GameManager* pGameManager = GameManager::getInstance();
    pGameManager->loadAll();
    pLoadingScreen->setProgress(tr("Loading Gamerules..."), Mainapp::SCRIPT_PROCESS + 6);
    GameRuleManager* pGameRuleManager = GameRuleManager::getInstance();
    pGameRuleManager->loadAll();
    pLoadingScreen->setProgress(tr("Loading Movements..."), Mainapp::SCRIPT_PROCESS + 8);
    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
    pMovementTableManager->loadAll();
    pLoadingScreen->setProgress(tr("Loading Terrains..."), Mainapp::SCRIPT_PROCESS + 10);
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    pTerrainManager->loadAll();
    pLoadingScreen->setProgress(tr("Loading Units..."), Mainapp::SCRIPT_PROCESS + 12);
    UnitSpriteManager* pUnitspritemanager = UnitSpriteManager::getInstance();
    pUnitspritemanager->loadAll();
    pLoadingScreen->setProgress(tr("Loading Weapons..."), Mainapp::SCRIPT_PROCESS + 14);
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    pWeaponManager->loadAll();
    pLoadingScreen->setProgress(tr("Loading Battleanimation scripts..."), Mainapp::SCRIPT_PROCESS + 16);
    BattleAnimationManager* pBattleAnimationManager = BattleAnimationManager::getInstance();
    pBattleAnimationManager->loadAll();
    pLoadingScreen->setProgress(tr("Loading CO-Perks..."), Mainapp::SCRIPT_PROCESS + 18);
    COPerkManager* pCOPerkManager = COPerkManager::getInstance();
    pCOPerkManager->loadAll();
    pLoadingScreen->setProgress(tr("Loading Wikiscripts..."), Mainapp::SCRIPT_PROCESS + 20);
    WikiDatabase::getInstance()->load();
    pLoadingScreen->setProgress(tr("Loading Shop items..."), Mainapp::SCRIPT_PROCESS + 22);
    ShopLoader* pShopLoader = ShopLoader::getInstance();
    pShopLoader->loadAll();
    pLoadingScreen->setProgress(tr("Loading Movement Planner addins..."), Mainapp::SCRIPT_PROCESS + 24);
    MovementPlannerAddInManager::getInstance()->loadAll();
    pLoadingScreen->setProgress(tr("Loading Ui scripts..."), Mainapp::SCRIPT_PROCESS + 26);
    UiManager::getInstance()->loadAll();

    Userdata::getInstance()->changeUser();
    pLoadingScreen->setProgress(tr("Loading Achievements..."), Mainapp::SCRIPT_PROCESS + 28);
    // achievements should be loaded last
    AchievementManager* pAchievementManager = AchievementManager::getInstance();
    pAchievementManager->loadAll();
    Player::getNeutralTableAnim();
    pInterpreter->doFunction("Global", "finalizeLoading");
    if(!Settings::getAiSlave())
    {
        if (pApp->getSlave())
        {
            QString script = pApp->getInitScript();
            if (!script.isEmpty())
            {
                CONSOLE_PRINT("Remote script is present and will be loaded", GameConsole::eDEBUG);
                CONSOLE_PRINT("Remote Script=" + script, GameConsole::eDEBUG);
                pInterpreter->evaluate(script, "remoteInit.js");
            }
        }
        else if (QFile::exists("init.js"))
        {
            CONSOLE_PRINT("Init script is present and will be loaded", GameConsole::eDEBUG);
            pInterpreter->openScript("init.js", true);
        }
    }
    connect(pApp, &Mainapp::sigMousePressEvent, this, &WorkerThread::mousePressEvent, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigMouseReleaseEvent, this, &WorkerThread::mouseReleaseEvent, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigWheelEvent, this, &WorkerThread::wheelEvent, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigMouseMoveEvent, this, &WorkerThread::mouseMoveEvent, Qt::QueuedConnection);
    pLoadingScreen->hide();
    m_started = true;
    CONSOLE_PRINT("WorkerThread::start Finalizing", GameConsole::eDEBUG);
    emit pApp->sigNextStartUpStep(Mainapp::StartupPhase::Finalizing);
}

void WorkerThread::mousePressEvent(oxygine::MouseButton button, qint32 x, qint32 y)
{
    oxygine::Input* input = &oxygine::Input::getInstance();
    input->sendPointerButtonEvent(oxygine::Stage::getStage(), button, x, y, 1.0f,
                                  oxygine::TouchEvent::TOUCH_DOWN, input->getPointerMouse());
}

void WorkerThread::mouseReleaseEvent(oxygine::MouseButton button, qint32 x, qint32 y)
{
    oxygine::Input* input = &oxygine::Input::getInstance();
    input->sendPointerButtonEvent(oxygine::Stage::getStage(), button, x, y, 1.0f,
                                  oxygine::TouchEvent::TOUCH_UP, input->getPointerMouse());
}

void WorkerThread::wheelEvent(qint32 x, qint32 y)
{
    oxygine::Input* input = &oxygine::Input::getInstance();
    input->sendPointerWheelEvent(oxygine::Stage::getStage(), QPoint(x, y), input->getPointerMouse());
}

void WorkerThread::mouseMoveEvent(qint32 x, qint32 y)
{
    oxygine::Input* input = &oxygine::Input::getInstance();
    input->sendPointerMotionEvent(oxygine::Stage::getStage(), x, y, 1.0f, input->getPointerMouse());
}

void WorkerThread::showMainwindow()
{
    CONSOLE_PRINT("WorkerThread::showMainwindow", GameConsole::eDEBUG);
    QCoreApplication::processEvents();
    QThread::msleep(5);

    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->hide();
    auto window = spMainwindow::create("ui/menu/mainmenu.xml");
    oxygine::Stage::getStage()->addChild(window);
}

bool WorkerThread::getStarted() const
{
    return m_started;
}

void WorkerThread::onQuit()
{
    CONSOLE_PRINT("Shutting down workerthread", GameConsole::eDEBUG);
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->hide();
    UiFactory::shutdown();
    if (oxygine::Stage::getStage())
    {
        oxygine::Stage::getStage()->cleanup();
        for (qint32 i = 0; i < 20; ++i)
        {
            QCoreApplication::processEvents();
        }
    }
    GameAnimationFactory::getInstance()->release();
    COSpriteManager::getInstance()->release();
    Player::releaseStaticData();
    Mainapp::getAiProcessPipe().quit();
    GameConsole::getInstance()->release();
    QCoreApplication::processEvents();
    Interpreter::release();
    for (qint32 i = 0; i < 20; ++i)
    {
        QCoreApplication::processEvents();
    }
}

void WorkerThread::startSlaveGame()
{
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->hide();
    Mainapp::getInstance()->getParser().startSlaveGame();
}

void WorkerThread::executeServerScript()
{
    const char* const SCRIPTFILE = "serverScript.js";
    CONSOLE_PRINT("MainServer::executeScript checking for script " + QString(SCRIPTFILE), GameConsole::eDEBUG);
    if (QFile::exists(SCRIPTFILE))
    {
        CONSOLE_PRINT("Loading server script", GameConsole::eDEBUG);
        Interpreter* pInterpreter = Interpreter::getInstance();
        if (pInterpreter->openScript(SCRIPTFILE, false))
        {
            QFile::remove(SCRIPTFILE);
            CONSOLE_PRINT("Executing server script", GameConsole::eDEBUG);
            pInterpreter->doFunction("serverScript");
        }
    }
}
