#include "workerthread.h"

#include "oxygine-framework.h"

#include "mainapp.h"

#include "coreengine/console.h"

#include "menue/mainwindow.h"

#include <QDirIterator>

#include "resource_management/terrainmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/movementtablemanager.h"
#include "resource_management/weaponmanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/gamerulemanager.h"

WorkerThread::WorkerThread()
{
    Interpreter::setCppOwnerShip(this);
}

WorkerThread::~WorkerThread()
{
    terminate();
    wait();
}

void WorkerThread::run()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    Console* pConsole = Console::getInstance();
    // create the initial menue no need to store the object
    // it will add itself to the current stage
    oxygine::getStage()->addChild(pConsole);
    m_pInterpreter = new Interpreter();
    pConsole->init();
    GameEnums::registerEnums();
    // load General-Base Scripts
    QString path =  QCoreApplication::applicationDirPath() + "/resources/scripts/general";
    QStringList filter;
    filter << "*.js";
    QDirIterator* dirIter = new QDirIterator(path, filter, QDir::Files, QDirIterator::Subdirectories);
    while (dirIter->hasNext())
    {
        dirIter->next();
        QString file = dirIter->fileInfo().absoluteFilePath();
        m_pInterpreter->openScript(file);
    }
    delete dirIter;
    oxygine::getStage()->addChild(new Mainwindow());

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

    pApp->continueThread();
    started = true;
    while (true)
    {
        exec();
    }
}

bool WorkerThread::getStarted() const
{
    return started;
}
