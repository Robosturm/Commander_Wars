#pragma once
#include "3rd_party/oxygine-framework/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

namespace oxygine
{
    DECLARE_SMART(ProgressBar, spProgressBar);

    class ProgressBar: public Sprite
    {
    public:
        enum { PROGRESS_CHANGED = sysEventID('P', 'C', 'h') };

        enum direction
        {
            dir_0,//moveable right edge
            dir_90,//moveable top edge
            dir_180,//moveable left edge
            dir_270,//moveable bottom edge
            dir_radial_cw,
            __dir_radial_ccw//not implemented!!
        };

        ProgressBar();
        ~ProgressBar();

        float       getProgress() const;
        direction   getDirection() const;

        /** setProgress
        @param value range [0.0f, 1.0f]
        */
        void setProgress(float value);
        void setDirection(direction dir);

        typedef Property<float, float, ProgressBar, &ProgressBar::getProgress, &ProgressBar::setProgress> TweenProgress;

    private:
        void doRender(const RenderState&) override;
        virtual void _update();
        void animFrameChanged(const AnimationFrame& f) override;

    private:
        float m_progress;
        direction m_direction;
        AnimationFrame m_originalFrame;
    };
}
