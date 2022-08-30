#pragma once

#include "objects/base/tooltip.h"

class SpriteObject;
using spSpriteObject = oxygine::intrusive_ptr<SpriteObject>;

class SpriteObject : public Tooltip
{
    Q_OBJECT
public:
    SpriteObject() = default;
    virtual ~SpriteObject() = default;
};

