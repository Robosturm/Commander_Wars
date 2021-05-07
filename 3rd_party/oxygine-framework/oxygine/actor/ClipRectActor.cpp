#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"

namespace oxygine
{

    ClipRectActor::ClipRectActor(): m_clipping(true)
    {

    }

    ClipRectActor::~ClipRectActor()
    {

    }

    void ClipRectActor::handleEvent(Event* event)
    {
        if (TouchEvent::isTouchEvent(event->type))
        {
            TouchEvent* te = safeCast<TouchEvent*>(event);
            Vector2 localPosition = parent2local(te->localPosition);
            if (!isOn(localPosition))
            {
                return;
            }
        }

        Actor::handleEvent(event);
    }

    void ClipRectActor::render(const RenderState& parentRS)
    {
        m_rdelegate->render(this, parentRS);
    }
}
