#pragma once

#include "objects/base/tooltip.h"

class SpriteObject;
using spSpriteObject = oxygine::intrusive_ptr<SpriteObject>;

class SpriteObject final : public Tooltip
{
    Q_OBJECT
public:
    SpriteObject() = default;
    ~SpriteObject() = default;
};

