#include "rotatingsprite.h"

RotatingSprite::RotatingSprite()
{
    speedTimer.start();

    m_SecondRect = new oxygine::ClipRectActor();
    addChild(m_SecondRect);
    m_FirstRect = new oxygine::ClipRectActor();
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
    if (speedTimer.elapsed() > speed)
    {
        float width = getWidth();
        float overflow = m_FirstRect->getWidth() + direction;
        if (overflow >= width || overflow <= 0.0f)
        {
            if (direction > 0)
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
            m_SecondRect->setWidth(m_SecondRect->getWidth() - direction);
            m_FirstRect->setWidth(m_FirstRect->getWidth() + direction);
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
        speedTimer.start();
    }
    oxygine::Actor::doUpdate(us);
}

qint32 RotatingSprite::getDirection() const
{
    return direction;
}

void RotatingSprite::setDirection(const qint32 &value)
{
    direction = value;
}

qint32 RotatingSprite::getSpeed() const
{
    return speed;
}

void RotatingSprite::setSpeed(const qint32 &value)
{
    speed = value;
}
