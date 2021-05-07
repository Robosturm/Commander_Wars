#include "rotatingsprite.h"

#include "coreengine/mainapp.h"

RotatingSprite::RotatingSprite()
{
    m_speedTimer.start();

    m_SecondRect = oxygine::spClipRectActor::create();
    addChild(m_SecondRect);
    m_FirstRect = oxygine::spClipRectActor::create();
    addChild(m_FirstRect);
}

void RotatingSprite::setSprite(oxygine::spActor firstSprite, oxygine::spActor secondSprite)
{
    m_FirstSprite = firstSprite;
    m_SecondSprite = secondSprite;
    m_FirstRect->addChild(m_FirstSprite);
    m_SecondRect->addChild(m_SecondSprite);
}

void RotatingSprite::sizeChanged(const oxygine::Vector2& size)
{
    m_SecondRect->setWidth(0);
    m_FirstRect->setWidth(size.x);

    m_SecondRect->setHeight(size.y);
    m_FirstRect->setHeight(size.y);

    m_SecondRect->setX(m_FirstRect->getWidth());
    if (m_SecondSprite.get() != nullptr)
    {
        m_SecondSprite->setX(-m_FirstRect->getWidth());
    }
    oxygine::Actor::sizeChanged(size);
}

void RotatingSprite::doUpdate(const oxygine::UpdateState& us)
{
    if (m_speedTimer.elapsed() > m_speed)
    {
        float width = getWidth();
        float overflow = m_FirstRect->getWidth() + m_direction;
        if (overflow >= width || overflow <= 0.0f)
        {
            if (m_direction > 0)
            {
                m_SecondRect->setWidth(width - (overflow - width));
                m_FirstRect->setWidth((overflow - width));
            }
            else
            {
                m_SecondRect->setWidth(-overflow);
                m_FirstRect->setWidth(width + overflow);
            }
        }
        else
        {
            m_SecondRect->setWidth(m_SecondRect->getWidth() - m_direction);
            m_FirstRect->setWidth(m_FirstRect->getWidth() + m_direction);
        }
        if (overflow == 0.0f)
        {
            m_SecondRect->setX(0);
            m_FirstSprite->setX(0);
        }
        else
        {
            float firstRectWidth = m_FirstRect->getWidth();
            m_SecondRect->setX(firstRectWidth);
            m_FirstSprite->setX(firstRectWidth - m_FirstSprite->getWidth());
        }
        m_speedTimer.start();
    }
    oxygine::Actor::doUpdate(us);
}

qint32 RotatingSprite::getDirection() const
{
    return m_direction;
}

void RotatingSprite::setDirection(const qint32 &value)
{
    m_direction = value;
}

qint32 RotatingSprite::getSpeed() const
{
    return m_speed;
}

void RotatingSprite::setSpeed(const qint32 &value)
{
    m_speed = value;
}
