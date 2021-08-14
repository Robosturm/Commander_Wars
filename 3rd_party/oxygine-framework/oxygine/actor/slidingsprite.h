#pragma once
#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

namespace oxygine
{
    class SlidingSprite;
    typedef oxygine::intrusive_ptr<SlidingSprite> spSlidingSprite;

    class SlidingSprite : public oxygine::ClipRectActor
    {
    public:
        explicit SlidingSprite();
        virtual ~SlidingSprite() = default;
        void setResAnim(ResAnim* pAnim);
        float getSpeed() const;
        void setSpeed(float speed);
        bool getFlipped() const;
        void setFlipped(bool flipped);
        virtual void update(const UpdateState& us) override;
    private:
        oxygine::spSprite m_sprites[2];
        float m_speed{1};
        bool m_flipped{false};
    };
}
