#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/Draggable.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

namespace oxygine
{
    class SlidingActorNoClipRect;
    using spSlidingActorNoClipRect = std::shared_ptr<SlidingActorNoClipRect>;
    class SlidingActorNoClipRect final : public Actor
    {
        public:
            static void setDefaultTouchThreshold(float val);

            explicit SlidingActorNoClipRect();
           virtual ~SlidingActorNoClipRect();

            spActor getContent() const
            {
                return m_content;
            }
            const QRect& getDragBounds() const
            {
                return m_drag.getDragBounds();
            }
            /**max allowed radius of touch move when content could be clicked*/
            void setTouchThreshold(float rad);
            /**sets actor with scrollable data*/
            void setContent(spActor content);
            /**lock for user scrolling*/
            void setLocked(bool locked);
            /**snaps content*/
            void snap();
            /**stop if sliding*/
            void stop();
            void updateDragBounds();

        protected:
            void destroy();
            virtual void handleEvent(Event* event) override;
            virtual void sizeChanged(const QSize& size) override;
            virtual void doUpdate(const UpdateState& us) override;

            void _newEvent(Event* event);

        protected:
            bool m_sliding{false};
            float m_rad{15.0f};
            bool m_ignoreTouchUp{false};
            QPoint m_downPos;
            QPoint m_speed;
            Draggable m_drag;
            spActor m_content;
            spEventDispatcher m_holded;
            struct  iter
            {
                    QPoint pos;
                    timeMS tm;
            };
            enum {NUM = 11};
            iter m_prev[NUM];
            qint32 m_current{0};
            timeMS m_lastIterTime{0};
            pointer_index m_finger{0};

            qint32 m_touchDownId{-1};
            qint32 m_touchUpId{-1};
            qint32 m_touchMoveId{-1};
            static float m_defaultTouchThreshold;
    };
}
