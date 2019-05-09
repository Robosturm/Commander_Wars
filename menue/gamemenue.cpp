#include "menue/gamemenue.h"

#include "game/player.h"
#include "game/co.h"

#include "menue/victorymenue.h"
#include "coreengine/console.h"

#include "gameinput/humanplayerinput.h"

#include "game/gameanimationfactory.h"

#include "objects/filedialog.h"

#include "objects/coinfodialog.h"

#include <QFile>

GameMenue* GameMenue::m_pInstance = nullptr;

GameMenue::GameMenue(qint32 startPlayer)
    : InGameMenue()
{
    loadGameMenue();
    startGame(startPlayer);
}

GameMenue::GameMenue(QString map)
    : InGameMenue(-1, -1, map)
{    
    loadGameMenue();
}

void GameMenue::loadGameMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_pInstance = this;
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
         pMap->getPlayer(i)->getBaseGameInput()->init();
    }
    pMap->centerMap(pMap->getMapWidth() / 2, pMap->getMapHeight() / 2);

    // back to normal code
    m_pPlayerinfo = new PlayerInfo();
    m_IngameInfoBar = new IngameInfoBar();
    m_IngameInfoBar->updateMinimap();
    m_pPlayerinfo->updateData();
    addChild(m_IngameInfoBar);
    addChild(m_pPlayerinfo);

    connect(pMap->getGameRules(), &GameRules::signalVictory, this, &GameMenue::victory, Qt::QueuedConnection);
    connect(pMap, &GameMap::signalExitGame, this, &GameMenue::exitGame, Qt::QueuedConnection);
    connect(pMap, &GameMap::signalSaveGame, this, &GameMenue::saveGame, Qt::QueuedConnection);
    connect(pMap, &GameMap::signalShowCOInfo, this, &GameMenue::showCOInfo, Qt::QueuedConnection);
    connect(pMap, &GameMap::sigQueueAction, this, &GameMenue::performAction, Qt::QueuedConnection);
    connect(m_IngameInfoBar->getMinimap(), &Minimap::clicked, pMap, &GameMap::centerMap, Qt::QueuedConnection);
    connect(GameAnimationFactory::getInstance(), &GameAnimationFactory::animationsFinished, this, &GameMenue::actionPerformed, Qt::QueuedConnection);
    connect(m_Cursor.get(), &Cursor::sigCursorMoved, m_IngameInfoBar.get(), &IngameInfoBar::updateCursorInfo, Qt::QueuedConnection);
}

GameMenue::~GameMenue()
{
    m_pInstance = nullptr;
}

void GameMenue::performAction(GameAction* pGameAction)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    QVector<QPoint> path = pGameAction->getMovePath();
    if (path.size() > 0)
    {
        QVector<QPoint> trapPath;
        for (qint32 i = path.size() - 1; i >= 0; i--)
        {
            // check the movepath for a trap
            QPoint point = path[i];
            Unit* pUnit = pMap->getTerrain(point.x(), point.y())->getUnit();
            if ((pUnit != nullptr) &&
                (pUnit->isStealthed(pMap->getCurrentPlayer())))
            {
                GameAction* pTrapAction = new GameAction("ACTION_TRAP");
                pTrapAction->setMovepath(trapPath);
                pTrapAction->writeDataInt32(point.x());
                pTrapAction->writeDataInt32(point.y());
                pTrapAction->setTarget(pGameAction->getTarget());
                delete pGameAction;
                pGameAction = pTrapAction;
                break;
            }
            else
            {
                trapPath.push_front(point);
            }
        }
    }
    Mainapp::seed(pGameAction->getSeed());
    Mainapp::setUseSeed(true);
    pGameAction->perform();
    // clean up the action
    delete pGameAction;

    if (GameAnimationFactory::getAnimationCount() == 0)
    {
        GameAnimationFactory::getInstance()->removeAnimation(nullptr);
    }
    else
    {
        bool skipAnimations = false;
        switch (Settings::getShowAnimations())
        {
            case GameEnums::AnimationMode_None:
            {
                skipAnimations = true;
                break;
            }
            case GameEnums::AnimationMode_All:
            {
                break;
            }
            case GameEnums::AnimationMode_Own:
            {
                if (pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() != BaseGameInputIF::AiTypes::Human)
                {
                    skipAnimations = true;
                }
                break;
            }
            case GameEnums::AnimationMode_Ally:
            {
                Player * pPlayer1 = pMap->getCurrentPlayer();
                Player * pPlayer2 = pMap->getCurrentViewPlayer();
                if (pPlayer2->isEnemy(pPlayer1))
                {
                    skipAnimations = true;
                }
                break;
            }
            case GameEnums::AnimationMode_Enemy:
            {
                if (pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() == BaseGameInputIF::AiTypes::Human)
                {
                    skipAnimations = true;
                }
                break;
            }
        }
        if (skipAnimations)
        {
            while (GameAnimationFactory::getAnimationCount() > 0)
            {
                GameAnimationFactory::finishAllAnimations();
            }
        }
    }
    pApp->continueThread();
}

void GameMenue::actionPerformed()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    Mainapp::setUseSeed(false);
    m_IngameInfoBar->updateTerrainInfo(m_Cursor->getMapPointX(), m_Cursor->getMapPointY(), true);
    m_IngameInfoBar->updateMinimap();
    m_IngameInfoBar->updatePlayerInfo();
    GameMap* pMap = GameMap::getInstance();
    pMap->getGameRules()->checkVictory();
    pMap->getGameRules()->createFogVision();
    emit sigActionPerformed();
    pApp->continueThread();
}

void GameMenue::updatePlayerinfo()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pPlayerinfo->updateData();
    m_IngameInfoBar->updatePlayerInfo();
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        pMap->getPlayer(i)->updateVisualCORange();
    }
    pApp->continueThread();
}

void GameMenue::victory(qint32 team)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    // create victory
    if (team >= 0)
    {
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = pMap->getPlayer(i);
            if (pPlayer->getTeam() != team)
            {
                pPlayer->defeatPlayer(nullptr);
            }
        }
    }
    Console::print("Leaving Game Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new VictoryMenue());
    oxygine::Actor::detach();
    pApp->continueThread();
}

void GameMenue::showCOInfo()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();

    GameMap* pMap = GameMap::getInstance();
    addChild(new COInfoDialog(pMap->getCurrentPlayer()->getspCO(0), pMap->getspPlayer(pMap->getCurrentPlayer()->getPlayerID()), [=](spCO& pCurrentCO, spPlayer& pPlayer, qint32 direction)
    {
        if (direction > 0)
        {
            if (pCurrentCO.get() == pPlayer->getCO(1) ||
                pPlayer->getCO(1) == nullptr)
            {
                // roll over case
                if (pPlayer->getPlayerID() == pMap->getPlayerCount() - 1)
                {
                    pPlayer = pMap->getspPlayer(0);
                    pCurrentCO = pPlayer->getspCO(0);
                }
                else
                {
                    pPlayer = pMap->getspPlayer(pPlayer->getPlayerID() + 1);
                    pCurrentCO = pPlayer->getspCO(0);
                }
            }
            else
            {
                pCurrentCO = pPlayer->getspCO(1);
            }
        }
        else
        {
            if (pCurrentCO.get() == pPlayer->getCO(0) ||
                pPlayer->getCO(0) == nullptr)
            {
                // select player
                if (pPlayer->getPlayerID() == 0)
                {
                    pPlayer = pMap->getspPlayer(pMap->getPlayerCount() - 1);
                }
                else
                {
                    pPlayer = pMap->getspPlayer(pPlayer->getPlayerID() - 1);
                }
                // select co
                if ( pPlayer->getCO(1) != nullptr)
                {
                    pCurrentCO = pPlayer->getspCO(1);
                }
                else
                {
                    pCurrentCO = pPlayer->getspCO(0);
                }
            }
            else
            {
                pCurrentCO = pPlayer->getspCO(0);
            }
        }
    }, true));
    pApp->continueThread();
}

void GameMenue::saveGame()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    QVector<QString> wildcards;
    wildcards.append("*.sav");
    QString path = QCoreApplication::applicationDirPath() + "/savegames";
    spFileDialog saveDialog = new FileDialog(path, wildcards, GameMap::getInstance()->getMapName());
    this->addChild(saveDialog);
    connect(saveDialog.get(), &FileDialog::sigFileSelected, this, &GameMenue::saveMap, Qt::QueuedConnection);
    pApp->continueThread();
}

void GameMenue::saveMap(QString filename)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (filename.endsWith(".sav"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        GameMap* pMap = GameMap::getInstance();
        pMap->serializeObject(stream);
        file.close();
    }
    pApp->continueThread();
}

void GameMenue::exitGame()
{
    victory(-1);
}

void GameMenue::startGame(qint32 startPlayer)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameAnimationFactory::clearAllAnimations();
    GameMap* pMap = GameMap::getInstance();
    pMap->startGame(startPlayer);
    if (startPlayer == 0)
    {
        pMap->setCurrentPlayer(GameMap::getInstance()->getPlayerCount() - 1);
    }
    else
    {
        pMap->setCurrentPlayer(startPlayer - 1);
    }
    GameRules* pRules = pMap->getGameRules();
    pRules->changeWeather(pRules->getWeather(pRules->getStartWeather())->getWeatherId(), pMap->getPlayerCount() + 1);
    pMap->nextTurn();
    updatePlayerinfo();
    pApp->continueThread();
}

void GameMenue::keyInput(SDL_Event event)
{
    InGameMenue::keyInput(event);
    if (m_Focused)
    {
        // for debugging
        SDL_Keycode cur = event.key.keysym.sym;
        if (cur == Settings::getKey_quicksave1())
        {
            saveMap("savegames/quicksave1.sav");
        }
        else if (cur == Settings::getKey_quicksave2())
        {
            saveMap("savegames/quicksave2.sav");
        }
        else if (cur == Settings::getKey_quickload1())
        {
            Mainapp* pApp = Mainapp::getInstance();
            pApp->suspendThread();
            Console::print("Leaving Game Menue", Console::eDEBUG);
            oxygine::Actor::detach();

            oxygine::getStage()->addChild(new GameMenue("savegames/quicksave1.sav"));
            pApp->getAudioThread()->clearPlayList();
            GameMap* pMap = GameMap::getInstance();
            pMap->getCurrentPlayer()->loadCOMusic();
            pMap->updateUnitIcons();
            pMap->getGameRules()->createFogVision();
            pApp->getAudioThread()->playRandom();
            GameMenue::getInstance()->updatePlayerinfo();
            pApp->continueThread();
        }
        else if (cur == Settings::getKey_quickload2())
        {

            Mainapp* pApp = Mainapp::getInstance();
            pApp->suspendThread();
            Console::print("Leaving Game Menue", Console::eDEBUG);
            oxygine::Actor::detach();
            oxygine::getStage()->addChild(new GameMenue("savegames/quicksave2.sav"));
            pApp->getAudioThread()->clearPlayList();
            GameMap* pMap = GameMap::getInstance();
            pMap->getCurrentPlayer()->loadCOMusic();
            pMap->updateUnitIcons();
            pMap->getGameRules()->createFogVision();
            pApp->getAudioThread()->playRandom();
            GameMenue::getInstance()->updatePlayerinfo();
            pApp->continueThread();
        }
    }
}
