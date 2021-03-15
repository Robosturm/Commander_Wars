#include "3rd_party/oxygine-framework/oxygine/actor/slidingsprite.h"

namespace oxygine
{

    SlidingSprite::SlidingSprite()
    {
        m_sprites[0] = new Sprite();
        m_sprites[1] = new Sprite();
        addChild(m_sprites[0]);
        addChild(m_sprites[1]);
    }

    void SlidingSprite::setResAnim(ResAnim* pAnim)
    {
        if (pAnim != nullptr)
        {
            m_sprites[0]->setResAnim(pAnim);
            m_sprites[0]->setWidth(pAnim->getWidth());
            m_sprites[1]->setResAnim(pAnim);
            m_sprites[1]->setWidth(pAnim->getWidth());
            m_sprites[0]->setX(0);
            m_sprites[1]->setX(pAnim->getWidth());
        }
    }

    float SlidingSprite::getSpeed() const
    {
        return m_speed;
    }

    void SlidingSprite::setSpeed(float speed)
    {
        m_speed = speed;
    }

    bool SlidingSprite::getFlipped() const
    {
        return m_flipped;
    }

    void SlidingSprite::setFlipped(bool flipped)
    {
        m_flipped = flipped;
        m_sprites[0]->setFlippedX(flipped);
        m_sprites[1]->setFlippedX(flipped);
    }

    void SlidingSprite::update(const UpdateState& us)
    {
        if (m_speed != 0.0f)
        {
            float speed = -m_speed;
            if (m_flipped)
            {
                speed = m_speed;
            }
            m_sprites[0]->setX(m_sprites[0]->getX() + speed);
            m_sprites[1]->setX(m_sprites[1]->getX() + speed);
            if (speed > 0)
            {
                if (m_sprites[0]->getX() >= getWidth())
                {
                    m_sprites[0]->setX(m_sprites[1]->getX() - m_sprites[0]->getWidth());
                }
                if (m_sprites[1]->getX() >= getWidth())
                {
                    m_sprites[1]->setX(m_sprites[0]->getX() - m_sprites[1]->getWidth());
                }
            }
            else
            {
                if (m_sprites[0]->getX() + m_sprites[0]->getWidth() < 0)
                {
                    m_sprites[0]->setX(m_sprites[1]->getX() + m_sprites[1]->getWidth());
                }
                if (m_sprites[1]->getX() + m_sprites[1]->getWidth() < 0)
                {
                    m_sprites[1]->setX(m_sprites[0]->getX() + m_sprites[0]->getWidth());
                }
            }
        }
        ClipRectActor::update(us);
    }
}
