#pragma once
#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

namespace oxygine
{
    class SlidingSprite;
    using spSlidingSprite = std::shared_ptr<SlidingSprite>;
    class SlidingSprite : public oxygine::ClipRectActor
    {
    public:
        explicit SlidingSprite();
        virtual ~SlidingSprite() = default;
        void setResAnim(ResAnim* pAnim);
        qint32 getSpeedX() const;
        void setSpeedX(qint32 speed);
        qint32 getSpeedY() const;
        void setSpeedY(qint32 speed);

        bool getFlippedX() const;
        void setFlippedX(bool flipped);
        virtual void update(const UpdateState& us) override;
        bool getLocked() const;
        void setLocked(bool newLocked);

    private:
        oxygine::spSprite m_sprites[4];
        qint32 m_speedX{1};
        qint32 m_speedY{0};
        bool m_flippedX{false};
        bool m_locked{false};
    };
}
