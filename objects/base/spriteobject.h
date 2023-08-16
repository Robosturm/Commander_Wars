#pragma once

#include "objects/base/tooltip.h"

class SpriteObject;
using spSpriteObject = std::shared_ptr<SpriteObject>;

class SpriteObject final : public Tooltip
{
    Q_OBJECT
public:
    SpriteObject() = default;
   virtual ~SpriteObject() = default;
};

