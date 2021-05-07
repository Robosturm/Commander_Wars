#include "3rd_party/oxygine-framework/oxygine/Draggable.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/math/AffineTransform.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"

namespace oxygine
{
    Vector2 convertPosUp(Actor* src, Actor* dest, const Vector2& pos, bool direction)
    {
        Vector2 locPos = pos;
        AffineTransform t;
        t.identity();
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

    Vector2 convertPosDown(Actor* src, Actor* dest, const Vector2& pos, bool direction)
    {
        Vector2 locPos = pos;
        AffineTransform t;
        t.identity();
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

    Draggable::Draggable(): m_bounds(0, 0, -1, -1),
        m_dragClient(0),
        m_actor(0),
        m_startTm(0),
        m_dragEnabled(true),
        m_pressed(false),
        m_singleDrag(false),
        m_ignoreTouchUp(false)
    {

    }

    Draggable::~Draggable()
    {
        destroy();
    }

    void Draggable::destroy()
    {
        if (m_actor && !m_singleDrag)
        {
            if (m_actor->__getStage())
            {
                m_actor->__getStage()->removeEventListeners(this);
            }
            m_actor->removeEventListeners(this);
            m_actor = 0;
        }

        m_pressed = false;
    }

    void Draggable::init(Actor* actor)
    {
        destroy();

        m_singleDrag = false;
        m_actor = actor;
        m_dragClient = actor;

        actor->addEventListener(TouchEvent::TOUCH_DOWN, EventCallback(this, &Draggable::onEvent));
        actor->addEventListener(TouchEvent::TOUCH_UP, EventCallback(this, &Draggable::onEvent));
    }

    void Draggable::start(const PointerState* pointer, Actor* actor, const Vector2& localPosition)
    {
        m_singleDrag = true;
        m_actor = actor;
        m_dragClient = actor;

        m_actor->__getStage()->addEventListener(TouchEvent::TOUCH_UP, EventCallback(this, &Draggable::onEvent));

        Vector2 src = pointer->getPosition().cast<Vector2>();
        Vector2 pos = actor->getParent()->stage2local(src);
        actor->setPosition(pos - localPosition);

        startDrag(localPosition);
    }

    void Draggable::startDrag(const Vector2& localCenter)
    {
        m_startTm = Clock::getTimeMS();
        m_pressed = true;
        m_dragPos = localCenter;
        m_clientPos = m_dragClient->getPosition();
        m_actor->__getStage()->addEventListener(TouchEvent::MOVE, EventCallback(this, &Draggable::onEvent));
    }

    void Draggable::onMove(const Vector2& position)
    {
        if (m_pressed && m_dragEnabled)
        {
            Actor* client = m_dragClient;
            Vector2 localPos = client->stage2local(position);
            Vector2 dragOffset = localPos - m_dragPos;
            Vector2 converted = convertPosUp(client, client->getParent(), dragOffset, true);
            Vector2 np;
            bool _clientIsParent = true;
            if (!_clientIsParent)
            {
                np = m_clientPos + dragOffset;
            }
            else
            {
                np = client->getPosition() + converted;
            }
            client->setPosition(np);
            snapClient2Bounds();
        }
    }

    void Draggable::onEvent(Event* event)
    {
        TouchEvent* te = safeCast<TouchEvent*>(event);
        switch (te->type)
        {
            case TouchEvent::TOUCH_DOWN:
            {
                startDrag(te->localPosition);
            }
                break;
            case TouchEvent::TOUCH_UP:
            {
                if (!m_ignoreTouchUp)
                {
                    m_pressed = false;
                    m_actor->__getStage()->removeEventListeners(this);
                    if (Clock::getTimeMS() - m_startTm < timeMS(2))
                    {
                        m_actor->setPosition(m_clientPos);
                    }
                }
            }
                break;

            case TouchEvent::MOVE:
            {
                onMove(te->localPosition);
            }
                break;
        }
    }

    void Draggable::setDragClient(Actor* actor)
    {
        m_dragClient = actor;
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
        Actor* actor = m_dragClient ? m_dragClient : m_actor;
        return actor;
    }

    void Draggable::snapClient2Bounds()
    {
        Actor* client = getClient();
        if (client != nullptr)
        {
            Vector2 np = client->getPosition();
            if (m_bounds.getWidth() != -1 && m_bounds.getHeight() != -1)
            {
                np.x = std::max(np.x, (float)m_bounds.getX());
                np.y = std::max(np.y, (float)m_bounds.getY());

                np.x = std::min(np.x, (float)m_bounds.getRight());
                np.y = std::min(np.y, (float)m_bounds.getBottom());
            }
            client->setPosition(np);
        }
    }

}
