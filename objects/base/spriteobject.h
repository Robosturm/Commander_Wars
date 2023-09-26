#pragma once

#include "objects/base/tooltip.h"

class SpriteObject;
using spSpriteObject = std::shared_ptr<SpriteObject>;

class SpriteObject final : public Tooltip
{
    Q_OBJECT
public:
    explicit SpriteObject();
    virtual ~SpriteObject() = default;
};

