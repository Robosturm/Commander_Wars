#pragma once
#include "3rd_party/oxygine-framework/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

namespace oxygine
{
    DECLARE_SMART(ClipRectActor, spClipRectActor);
    /**
    ClipRectActor clips all out of bound children. Rotation is not supported
    */
    class ClipRectActor : public Actor
    {
    public:
        ClipRectActor();
        ~ClipRectActor();

        bool getClipping() const { return m_clipping; }

        /**Enables clipping for children. By default is on*/
        void setClipping(bool enable) {m_clipping = enable;}

        void render(const RenderState& rs) override;


        void handleEvent(Event* event) override;
    protected:
        bool m_clipping;
    };
}
