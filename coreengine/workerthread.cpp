#include <QDirIterator>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "coreengine/mainapp.h"
#include "coreengine/workerthread.h"
#include "coreengine/console.h"

#include "menue/mainwindow.h"

#include "multiplayer/multiplayermenu.h"
#include "network/localserver.h"

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
#include "wiki/wikidatabase.h"


WorkerThread::WorkerThread()
{
    Interpreter::setCppOwnerShip(this);
    moveToThread(Mainapp::getWorkerthread());
    connect(this, &WorkerThread::sigStart, this, &WorkerThread::start, Qt::QueuedConnection);
    connect(this, &WorkerThread::sigShowMainwindow, this, &WorkerThread::showMainwindow, Qt::QueuedConnection);
    connect(this, &WorkerThread::sigStartSlaveGame, this, &WorkerThread::startSlaveGame, Qt::QueuedConnection);
}

WorkerThread::~WorkerThread()
{
}

void WorkerThread::start()
{
    Mainapp* pApp = Mainapp::getInstance();
    Console* pConsole = Console::getInstance();
    // create the initial menue no need to store the object
    // it will add itself to the current stage
    oxygine::getStage()->addChild(pConsole);
    Interpreter* pInterpreter = Interpreter::createInstance();
    pConsole->init();
    // load General-Base Scripts
    QStringList searchPaths;
    searchPaths.append("/resources/scripts/general");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        searchPaths.append("/" + Settings::getMods().at(i) + "/scripts/general");
    }
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString path = QCoreApplication::applicationDirPath() + searchPaths[i];
        QStringList filter;
        filter << "*.js";
        QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter.hasNext())
        {
            dirIter.next();
            QString file = dirIter.fileInfo().absoluteFilePath();
            pInterpreter->openScript(file, true);
        }
    }
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    pBuildingSpriteManager->loadAll();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    pCOSpriteManager->loadAll();
    GameManager* pGameManager = GameManager::getInstance();
    pGameManager->loadAll();
    GameRuleManager* pGameRuleManager = GameRuleManager::getInstance();
    pGameRuleManager->loadAll();
    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
    pMovementTableManager->loadAll();
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    pTerrainManager->loadAll();
    UnitSpriteManager* pUnitspritemanager = UnitSpriteManager::getInstance();
    pUnitspritemanager->loadAll();
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    pWeaponManager->loadAll();
    BattleAnimationManager* pBattleAnimationManager = BattleAnimationManager::getInstance();
    pBattleAnimationManager->loadAll();
    COPerkManager* pCOPerkManager = COPerkManager::getInstance();
    pCOPerkManager->loadAll();
    WikiDatabase::getInstance()->load();
    ShopLoader* pShopLoader = ShopLoader::getInstance();
    pShopLoader->loadAll();
    // achievements should be loaded last
    AchievementManager* pAchievementManager = AchievementManager::getInstance();
    pAchievementManager->loadAll();

    if (QFile::exists("init.js"))
    {
        pInterpreter->openScript("init.js", true);
    }

    connect(pApp, &Mainapp::sigMousePressEvent, this, &WorkerThread::mousePressEvent, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigMouseReleaseEvent, this, &WorkerThread::mouseReleaseEvent, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigWheelEvent, this, &WorkerThread::wheelEvent, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigMouseMoveEvent, this, &WorkerThread::mouseMoveEvent, Qt::QueuedConnection);
    started = true;
    
}

void WorkerThread::mousePressEvent(oxygine::MouseButton button, qint32 x, qint32 y)
{
    oxygine::Input* input = &oxygine::Input::instance;
    input->sendPointerButtonEvent(oxygine::getStage(), button, x, y, 1.0f,
                                  oxygine::TouchEvent::TOUCH_DOWN, &input->_pointerMouse);
}

void WorkerThread::mouseReleaseEvent(oxygine::MouseButton button, qint32 x, qint32 y)
{
    oxygine::Input* input = &oxygine::Input::instance;
    input->sendPointerButtonEvent(oxygine::getStage(), button, x, y, 1.0f,
                                  oxygine::TouchEvent::TOUCH_UP, &input->_pointerMouse);
}

void WorkerThread::wheelEvent(qint32 x, qint32 y)
{
    oxygine::Input* input = &oxygine::Input::instance;
    input->sendPointerWheelEvent(oxygine::getStage(), oxygine::Vector2(x, y), &input->_pointerMouse);
}

void WorkerThread::mouseMoveEvent(qint32 x, qint32 y)
{
    oxygine::Input* input = &oxygine::Input::instance;
    input->sendPointerMotionEvent(oxygine::getStage(), x, y, 1.0f, &input->_pointerMouse);
}

void WorkerThread::showMainwindow()
{
    oxygine::getStage()->addChild(new Mainwindow());
}

bool WorkerThread::getStarted() const
{
    return started;
}

void WorkerThread::startSlaveGame()
{
    LocalServer* pServer = new LocalServer();
    Multiplayermenu* pMenu = new Multiplayermenu(pServer, "", true);
    pMenu->connectNetworkSlots();
    oxygine::getStage()->addChild(pMenu);
    pServer->sig_connect(Settings::getSlaveServerName(), 0);
}
