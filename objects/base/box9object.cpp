#include "objects/base/box9object.h"

Box9Object::Box9Object()
{
    Interpreter::setCppOwnerShip(this);
}

void Box9Object::setColorTableFromPlayer(Player* pPlayer, bool matrix)
{
    setColorTable(pPlayer->getColorTableAnim(), matrix);
}

void Box9Object::setIsVisible(bool vis)
{
    setVisible(vis);
}

void Box9Object::setPosX(qint32 x)
{
    setX(x);
}

void Box9Object::setPosY(qint32 y)
{
    setY(y);
}