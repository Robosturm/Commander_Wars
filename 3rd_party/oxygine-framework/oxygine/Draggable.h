#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

namespace oxygine
{
    Vector2 convertPosUp(Actor* src, Actor* dest, const Vector2& pos, bool direction);
    Vector2 convertPosDown(Actor* src, Actor* dest, const Vector2& pos, bool direction);

    class Draggable : public IClosureOwner
    {
    public:
        Draggable();
        ~Draggable();

        void init(Actor* actor);
        void start(const PointerState* pointer, Actor* actor, const Vector2& localCenter);

        void destroy();

        Actor*          getDragClient() const {return m_dragClient;}
        const RectF&    getDragBounds() const {return m_bounds;}
        const Vector2&  getClientPos() const {return m_clientPos;}
        bool            getDragEnabled() const { return m_dragEnabled; }
        bool            isDragging() const {return m_pressed;}

        void setDragEnabled(bool en) {m_dragEnabled = en;}
        /**sets bounds position for dragged actor*/
        void setDragBounds(const RectF& bounds);
        /**sets destination drag client. Default value is Actor attached to DragHandler*/
        void setDragClient(Actor* actor);
        void setIgnoreTouchUp(bool ignore) { m_ignoreTouchUp = ignore; }
        void snapClient2Bounds();

    protected:
        Actor* getClient();
        void onEvent(Event* event);
        void startDrag(const Vector2& localPos);

        void onDrag(TouchEvent* es);
        void onMove(const Vector2& position);

    protected:
        RectF m_bounds;
        Vector2 m_dragPos;
        Vector2 m_clientPos;

        Actor* m_dragClient;
        Actor* m_actor;
        timeMS m_startTm;

        bool m_dragEnabled;
        bool m_pressed;
        bool m_singleDrag;
        bool m_ignoreTouchUp;
    };
}
