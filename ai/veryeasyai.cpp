#include "veryeasyai.h"

#include "game/gameaction.h"

VeryEasyAI::VeryEasyAI()
{

}

void VeryEasyAI::process()
{
    finishTurn();
}

void VeryEasyAI::finishTurn()
{
    GameAction* pAction = new GameAction("ACTION_NEXT_PLAYER");
    emit performAction(pAction);
}

void VeryEasyAI::serialize(QDataStream&)
{
}
void VeryEasyAI::deserialize(QDataStream&)
{
}
