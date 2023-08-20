#pragma once
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include<QMatrix4x4>

namespace oxygine
{
    class Stage;
    using spStage = std::shared_ptr<Stage>;
    class Stage final : public Actor
    {
    public:
        enum
        {
            ACTIVATE = sysEventID('S', 'A', 'c'),
            DEACTIVATE = sysEventID('S', 'D', 'c'),
            LOST_CONTEXT = sysEventID('S', 'L', 'C'),
        };

        static QMatrix4x4 getViewProjectionMatrix(const QRectF& viewport);
        explicit Stage();
        ~Stage() = default;

        /**Initializes size, scale and position of Stage
        @param is real display size of device.
        @param is your "virtual" preferred size. You could change it to any size you need
        */
        void init(const QSize& gameSize, const qreal scaleFactor);
        /**Render all actors*/
        void renderStage(const QColor* clearColor, const QRect& viewport, const QMatrix4x4 & viewProjection);
        /**Render all actors, simplified version*/
        void renderStage(const QColor& clearColor, const QRect& viewport);
        /**Updates each children*/
        void updateStage();
        void cleanup();
        virtual QRect getDestRect() const override;
        static spStage& getStage()
        {
            return Stage::instance;
        }
        static void setStage(spStage stage)
        {
            Stage::instance = stage;
        }
    protected:
        void onDeactivate(Event*);
        virtual bool isOn(const QPoint& localPosition) override;

    protected:
        static spStage instance;
        timeMS m_statUpdate;
    };
}
