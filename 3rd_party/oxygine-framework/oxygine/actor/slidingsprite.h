#ifndef SLIDINGSPRITE_H
#define SLIDINGSPRITE_H

#include "ClipRectActor.h"
#include "Sprite.h"

namespace oxygine
{
    class SlidingSprite;
    typedef oxygine::intrusive_ptr<SlidingSprite> spSlidingSprite;

    class SlidingSprite : public oxygine::ClipRectActor
    {
    public:
        SlidingSprite();
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

#endif // SLIDINGSPRITE_H
