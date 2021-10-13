#pragma once
#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

namespace oxygine
{
    class SlidingSprite;
    using spSlidingSprite = oxygine::intrusive_ptr<SlidingSprite>;
    class SlidingSprite : public oxygine::ClipRectActor
    {
    public:
        explicit SlidingSprite();
        virtual ~SlidingSprite() = default;
        void setResAnim(ResAnim* pAnim);
        float getSpeedX() const;
        void setSpeedX(float speed);
        float getSpeedY() const;
        void setSpeedY(float speed);

        bool getFlippedX() const;
        void setFlippedX(bool flipped);
        virtual void update(const UpdateState& us) override;
    private:
        oxygine::spSprite m_sprites[4];
        float m_speedX{1};
        float m_speedY{0};
        bool m_flippedX{false};
    };
}
