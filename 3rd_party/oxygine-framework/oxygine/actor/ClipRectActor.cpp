#include "ClipRectActor.h"
#include "Stage.h"
#include "../RenderDelegate.h"
#include "../RenderState.h"
#include "../core/oxygine.h"

namespace oxygine
{

    ClipRectActor::ClipRectActor(): _clipping(true)
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
                return;
        }

        Actor::handleEvent(event);
    }

    void ClipRectActor::render(const RenderState& parentRS)
    {
        _rdelegate->render(this, parentRS);
    }
}
