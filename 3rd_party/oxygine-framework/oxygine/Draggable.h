#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

namespace oxygine
{
    class Draggable : public IClosureOwner
    {
    public:
        enum
        {
            DragMoveEvent = sysEventID('D', 'M', 'E')
        };

        explicit Draggable() = default;
        virtual ~Draggable();

        void init(Actor* actor);

        void destroy();

        Actor* getDragClient() const
        {
            return m_dragClient;
        }
        const QRect& getDragBounds() const
        {
            return m_bounds;
        }
        const QPoint& getClientPos() const
        {
            return m_clientPos;
        }
        bool getDragEnabled() const
        {
            return m_dragEnabled;
        }
        bool isDragging() const
        {
            return m_pressed;
        }
        void setDragEnabled(bool en)
        {
            m_dragEnabled = en;
        }
        /**sets bounds position for dragged actor*/
        void setDragBounds(const QRect& bounds);
        void snapClient2Bounds();
        bool getNoLockForMiddleButton() const;
        void setNoLockForMiddleButton(bool newNoLockForMiddleButton);

    protected:
        Actor* getClient();
        void onEvent(Event* event);
        void startDrag(const QPoint& localPos);

        void onDrag(TouchEvent* es);
        void onMove(const QPoint& position);

        QPoint convertPosUp(Actor* src, Actor* dest, const QPoint& pos, bool direction);
        QPoint convertPosDown(Actor* src, Actor* dest, const QPoint& pos, bool direction);
    protected:
        QRect m_bounds{0, 0, -1, -1};
        QPoint m_dragPos;
        QPoint m_clientPos;
        Actor* m_dragClient{nullptr};
        timeMS m_startTm{0};
        bool m_dragEnabled{true};
        bool m_noLockForMiddleButton{false};
        bool m_middleButton{false};
        bool m_pressed{false};
    };
}
