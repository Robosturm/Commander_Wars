#include "3rd_party/oxygine-framework/oxygine/Draggable.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/math/AffineTransform.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"

namespace oxygine
{
    Draggable::~Draggable()
    {
        destroy();
    }

    void Draggable::destroy()
    {
        if (m_dragClient != nullptr)
        {
            oxygine::Stage::getStage()->removeEventListeners(this);
            m_dragClient->removeEventListeners(this);
            m_dragClient->detach();
            m_dragClient = nullptr;
        }

        m_pressed = false;
    }

    void Draggable::init(Actor* actor)
    {
        destroy();
        m_dragClient = actor;
        actor->addEventListener(TouchEvent::TOUCH_DOWN, EventCallback(this, &Draggable::onEvent));
        actor->addEventListener(TouchEvent::TOUCH_UP, EventCallback(this, &Draggable::onEvent));
    }

    void Draggable::startDrag(const Vector2& localCenter)
    {
        m_startTm = Clock::getTimeMS();
        m_pressed = true;
        m_dragPos = localCenter;
        m_clientPos = m_dragClient->getPosition();
        oxygine::Stage::getStage()->addEventListener(TouchEvent::MOVE, EventCallback(this, &Draggable::onEvent));
    }

    void Draggable::onMove(const Vector2& position)
    {
        if (m_pressed && (m_dragEnabled || m_middleButton) &&
            m_dragClient != nullptr)
        {
            Vector2 localPos = m_dragClient->stage2local(position);
            Vector2 dragOffset = localPos - m_dragPos;
            Vector2 converted = convertPosUp(m_dragClient, m_dragClient->getParent(), dragOffset, true);
            Vector2 np;
            bool _clientIsParent = true;
            if (!_clientIsParent)
            {
                np = m_clientPos + dragOffset;
            }
            else
            {
                np = m_dragClient->getPosition() + converted;
            }
            auto startPos = m_dragClient->getPosition();
            m_dragClient->setPosition(np);
            snapClient2Bounds();
            if (startPos != m_dragClient->getPosition())
            {
                oxygine::Event pEvent(DragMoveEvent);
                m_dragClient->dispatchEvent(&pEvent);
            }
        }
    }

    bool Draggable::getNoLockForMiddleButton() const
    {
        return m_noLockForMiddleButton;
    }

    void Draggable::setNoLockForMiddleButton(bool newNoLockForMiddleButton)
    {
        m_noLockForMiddleButton = newNoLockForMiddleButton;
    }

    void Draggable::onEvent(Event* event)
    {
        TouchEvent* te = safeCast<TouchEvent*>(event);
        switch (te->type)
        {
            case TouchEvent::TOUCH_DOWN:
            {
                if (te->mouseButton == MouseButton_Middle && m_noLockForMiddleButton)
                {
                    m_middleButton = true;
                }
                startDrag(te->localPosition);
                break;
            }
            case TouchEvent::TOUCH_UP:
            {
                if (!m_ignoreTouchUp &&
                    m_dragClient != nullptr)
                {
                    m_middleButton = false;
                    m_pressed = false;
                    oxygine::Stage::getStage()->removeEventListeners(this);
                    if (Clock::getTimeMS() - m_startTm < timeMS(2))
                    {
                        m_dragClient->setPosition(m_clientPos);
                    }
                }
                break;
            }

            case TouchEvent::MOVE:
            {
                onMove(te->localPosition);
                break;
            }
        }
    }

    void Draggable::setDragBounds(const RectF& r)
    {
        m_bounds = r;
    }

    bool isParent(Actor* child, Actor* isItParent)
    {
        while (child)
        {
            if (child == isItParent)
            {
                return true;
            }
            child = child->getParent();
        }
        return false;
    }

    Actor* Draggable::getClient()
    {
        return m_dragClient;
    }

    void Draggable::snapClient2Bounds()
    {
        if (m_dragClient != nullptr)
        {
            Vector2 np = m_dragClient->getPosition();
            if (m_bounds.getWidth() != -1 && m_bounds.getHeight() != -1)
            {
                np.x = std::max(np.x, (float)m_bounds.getX());
                np.y = std::max(np.y, (float)m_bounds.getY());

                np.x = std::min(np.x, (float)m_bounds.getRight());
                np.y = std::min(np.y, (float)m_bounds.getBottom());
            }
            m_dragClient->setPosition(np);
        }
    }

    Vector2 Draggable::convertPosUp(Actor* src, Actor* dest, const Vector2& pos, bool direction)
    {
        Vector2 locPos = pos;
        AffineTransform t;
        while (src != dest && src)
        {
            t = src->getTransform() * t;
            src = src->getParent();
        }
        if (direction)
        {
            t.x = 0;
            t.y = 0;
        }

        locPos = t.transform(locPos);
        return locPos;
    }

    Vector2 Draggable::convertPosDown(Actor* src, Actor* dest, const Vector2& pos, bool direction)
    {
        Vector2 locPos = pos;
        AffineTransform t;
        t = src->getTransform();
        while (src != dest && src)
        {
            t =  t * src->getTransform();
            src = src->getParent();
        }
        if (direction)
        {
            t.x = 0;
            t.y = 0;
        }
        locPos = t.transform(locPos);
        return locPos;
    }

}
