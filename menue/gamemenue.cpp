#include "menue/gamemenue.h"

#include "game/player.h"
#include "game/co.h"

#include "menue/mainwindow.h"
#include "coreengine/console.h"

#include "gameinput/humanplayerinput.h"

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
    m_pInstance = this;

    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
            {
                emit sigRightClick(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
            }
            else if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
            {
                emit sigLeftClick(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
            }
        }
    });
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
         pMap->getPlayer(i)->getBaseGameInput()->init();
    }
    connect(pMap->getGameRules(), &GameRules::signalVictory, this, &GameMenue::victory, Qt::QueuedConnection);
    connect(pMap, &GameMap::signalExitGame, this, &GameMenue::exitGame, Qt::QueuedConnection);
    connect(pMap, &GameMap::signalSaveGame, this, &GameMenue::saveGame, Qt::QueuedConnection);
    // back to normal code
    m_pPlayerinfo = new PlayerInfo();
    m_pPlayerinfo->updateData();
    this->addChild(m_pPlayerinfo);
}

GameMenue::~GameMenue()
{
    m_pInstance = nullptr;
}

void GameMenue::performAction(GameAction* pGameAction)
{
    QVector<QPoint> path = pGameAction->getMovePath();
    if (path.size() > 0)
    {
        GameMap* pMap = GameMap::getInstance();
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

    pGameAction->perform();
    // clean up the action
    delete pGameAction;
}

void GameMenue::updatePlayerinfo()
{
    m_pPlayerinfo->updateData();
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        pMap->getPlayer(i)->updateVisualCORange();
    }
}

void GameMenue::victory(qint32 team)
{
    Console::print("Leaving Game Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new Mainwindow());
    oxygine::Actor::detach();
}

void GameMenue::saveGame()
{

}

void GameMenue::exitGame()
{
    victory(-1);
}

void GameMenue::startGame(qint32 startPlayer)
{
    GameMap* pMap = GameMap::getInstance();
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
}

