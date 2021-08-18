#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/core/vertex.h"
#include "3rd_party/oxygine-framework/oxygine/math/AffineTransform.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"

namespace oxygine
{
    enum blend_mode
    {
        blend_disabled              = 0,
        blend_premultiplied_alpha   = ((VideoDriver::BT_ONE                 << 16) | VideoDriver::BT_ONE_MINUS_SRC_ALPHA),
        blend_alpha                 = ((VideoDriver::BT_SRC_ALPHA           << 16) | VideoDriver::BT_ONE_MINUS_SRC_ALPHA),
        blend_add                   = ((VideoDriver::BT_ONE                 << 16) | VideoDriver::BT_ONE),
        blend_screen                = ((VideoDriver::BT_ONE                 << 16) | VideoDriver::BT_ONE_MINUS_SRC_COLOR),
        blend_multiply              = ((VideoDriver::BT_DST_COLOR           << 16) | VideoDriver::BT_ONE_MINUS_SRC_ALPHA),
        blend_inverse               = ((VideoDriver::BT_ONE_MINUS_DST_COLOR << 16) | VideoDriver::BT_ZERO),
    };
}
