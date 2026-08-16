#include "objects/base/spriteobject.h"
#include "coreengine/interpreter.h"

SpriteObject::SpriteObject()
{
#ifdef GRAPHICSUPPORT
    setObjectName("SpriteObject");
#endif
    Interpreter::setCppOwnerShip(this);
}

void SpriteObject::loadSprite(const QString & spriteId, qint32 targetHeight)
{
    auto* pAnim = WikiDatabase::getInstance()->getGlobalResAnim(spriteId);
    setResAnim(pAnim);
    if (pAnim)
    {
        setScale(static_cast<float>(targetHeight) / static_cast<float>(pAnim->getHeight()));
    }
}

void SpriteObject::loadFromUnit(Unit* pUnit)
{
    removeChildren();
    if (pUnit)
    {
        spUnit pCopyUnit = MemoryManagement::create<Unit>(pUnit->getUnitID(), pUnit->getOwner(), false, pUnit->getMap());
        pCopyUnit->setHasMoved(pUnit->getHasMoved());
        pCopyUnit->setHp(pUnit->getHp());
        pCopyUnit->setAmmo1(pUnit->getAmmo1());
        pCopyUnit->setAmmo2(pUnit->getAmmo2());
        pCopyUnit->setFuel(pUnit->getFuel());
        pCopyUnit->setUnitRank(pUnit->getUnitRank());
        addChild(pCopyUnit);
    }
}