#pragma once
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

namespace oxygine
{
    class Stage;
    using spStage = intrusive_ptr<Stage>;
    class Stage : public Actor
    {
    public:
        enum
        {
            ACTIVATE = sysEventID('S', 'A', 'c'),
            DEACTIVATE = sysEventID('S', 'D', 'c'),
            LOST_CONTEXT = sysEventID('S', 'L', 'C'),
        };
        static spStage instance;

        static Rect calcCenteredViewport(const Point& displaySize, const Point& gameSize);
        explicit Stage();
        virtual ~Stage() = default;

        /**Initializes size, scale and position of Stage
        @param is real display size of device.
        @param is your "virtual" preferred size. You could change it to any size you need
        */
        void init(const Point& displaySize, const Point& gameSize);
        /**Render all actors*/
        void renderStage(const QColor* clearColor, const Rect& viewport, const Matrix& view, const Matrix& proj);
        /**Render all actors, simplified version*/
        void renderStage(const QColor& clearColor, const Rect& viewport);
        /**Updates each children*/
        void updateStage();
        void cleanup();
        virtual RectF getDestRect() const override;
        static spStage& getStage()
        {
            return Stage::instance;
        }
    protected:
        void onDeactivate(Event*);
        virtual bool isOn(const Vector2& localPosition, float localScale) override;

    protected:
        timeMS m_statUpdate;
        Rect   m_viewport;
    };
}
