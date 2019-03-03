#include "menue/gamemenue.h"

#include "game/player.h"
#include "game/co.h"

#include "gameinput/humanplayerinput.h"

GameMenue* GameMenue::m_pInstance = nullptr;

GameMenue::GameMenue(QString map)
    : InGameMenue(-1, -1, map)
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

    // test code

    GameMap::getInstance()->getspPlayer(0)->setCO("CO_EAGLE", 0);
    GameMap::getInstance()->getspPlayer(0)->defineArmy();

    GameMap::getInstance()->getspPlayer(1)->setCO("CO_COLIN", 0);
    GameMap::getInstance()->getspPlayer(1)->setCO("CO_MAX", 1);
    GameMap::getInstance()->getspPlayer(1)->defineArmy();
    GameMap::getInstance()->updateSprites();

    // back to normal code
    m_pPlayerinfo = new PlayerInfo();
    m_pPlayerinfo->updateData();
    this->addChild(m_pPlayerinfo);

    GameMap::getInstance()->getspPlayer(0)->setBaseGameInput(new HumanPlayerInput(this));
    startGame(0);

}

GameMenue::~GameMenue()
{
    m_pInstance = nullptr;
}

void GameMenue::performAction(GameAction* pGameAction)
{
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
    pMap->nextTurn();
}
