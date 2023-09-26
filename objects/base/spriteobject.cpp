#include "objects/base/spriteobject.h"
#include "coreengine/interpreter.h"

SpriteObject::SpriteObject()
{
#ifdef GRAPHICSUPPORT
    setObjectName("SpriteObject");
#endif
    Interpreter::setCppOwnerShip(this);
}