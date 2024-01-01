#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"

namespace oxygine
{

    void ClipRectActor::handleEventImpl(Event* event)
    {
        if (event != nullptr)
        {
            if (TouchEvent::isTouchEvent(event->type))
            {
                TouchEvent* te = safeCast<TouchEvent*>(event);
                QPoint localPosition = parent2local(te->localPosition);
                if (!isOn(localPosition))
                {
                    return;
                }
            }

            Actor::handleEventImpl(event);
        }
    }

    void ClipRectActor::render(const RenderState& parentRS)
    {
        RenderDelegate::instance->render(this, parentRS);
    }
}
