#pragma once

#include "objects/base/tooltip.h"

class SpriteObject;
using spSpriteObject = std::shared_ptr<SpriteObject>;

class Unit;

class SpriteObject final : public Tooltip
{
    Q_OBJECT
public:
    explicit SpriteObject();
    virtual ~SpriteObject() = default;
    Q_INVOKABLE void loadSprite(const QString & spriteId, qint32 targetHeight);
    Q_INVOKABLE void loadFromUnit(Unit* pUnit);
};

Q_DECLARE_INTERFACE(SpriteObject, "SpriteObject");
