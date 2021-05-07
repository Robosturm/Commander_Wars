#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"
#include "3rd_party/oxygine-framework/oxygine/Event.h"
#include "3rd_party/oxygine-framework/oxygine/math/Vector2.h"

/**main oxygine namespace*/
namespace oxygine
{
    namespace core
    {
        /**destroy and reset any GPU allocated memory and handles. Call it to free memory if app was minimized (lost focus)*/
        void reset();
    }
}
