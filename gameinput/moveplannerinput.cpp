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

bool MoveplannerInput::getFocus() const
{
    return m_focus;
}

void MoveplannerInput::setFocus(bool newFocus)
{
    m_focus = newFocus;
}

void MoveplannerInput::leftClick(qint32 x, qint32 y)
{
    if (m_focus)
    {
        HumanPlayerInput::leftClick(x, y);
    }
}
