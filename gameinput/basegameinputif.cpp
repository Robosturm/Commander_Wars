#include "basegameinputif.h"

#include "coreengine/interpreter.h"

BaseGameInputIF::BaseGameInputIF()
{
    Interpreter::setCppOwnerShip(this);
}

void BaseGameInputIF::setPlayer(Player* pPlayer)
{
    m_pPlayer = pPlayer;
}
