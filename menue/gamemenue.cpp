#include "menue/gamemenue.h"

#include "game/player.h"
#include "game/co.h"

#include "gameinput/humanplayerinput.h"

GameMenue::GameMenue(QString map)
    : InGameMenue(-1, -1, map)
{
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


    GameMap::getInstance()->getspPlayer(0)->setBaseGameInput(new HumanPlayerInput(this));
    startGame(0);
}

void GameMenue::performAction(GameAction* pGameAction)
{
    pGameAction->perform();
    // clean up the action
    delete pGameAction;
}

void GameMenue::startGame(qint32 startPlayer)
{
    GameMap::getInstance()->setCurrentPlayer(startPlayer);
    GameMap::getInstance()->getCurrentPlayer()->earnMoney();
}
