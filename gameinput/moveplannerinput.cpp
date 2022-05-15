#include "gameinput/moveplannerinput.h"

#include "coreengine/mainapp.h"

#include "menue/movementplanner.h"

MoveplannerInput::MoveplannerInput(GameMap* pMap)
    : HumanPlayerInput(pMap)
{
    m_AiType = GameEnums::AiTypes_MovePlanner;
    setObjectName("MoveplannerInput");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

Player* MoveplannerInput::getPerformingPlayer(Player* pPlayer) const
{
    if (pPlayer)
    {
        return pPlayer;
    }
    else
    {
        return HumanPlayerInput::getPerformingPlayer(pPlayer);
    }
}

QStringList MoveplannerInput::getEmptyActionList()
{
    return HumanPlayerInput::getViewplayerActionList();
}
