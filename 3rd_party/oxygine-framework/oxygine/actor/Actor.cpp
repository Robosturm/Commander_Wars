#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/math/AffineTransform.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/tween/Tween.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "texture.h"
#include <QtMath>

namespace oxygine
{
#ifndef GRAPHICSUPPORT
    Vector2 Actor::m_dummyVector;
    RectF Actor::m_dummyRectF;
#endif

    Actor::Actor()
        : m_stage(nullptr),
          m_parent(nullptr)
    {
        m_pressedOvered = 0;
    }

    Actor::~Actor()
    {
        removeTweens();
        removeChildren();
    }

    Stage* Actor::__getStage()
    {
        return m_stage;
    }

    void Actor::added2stage(Stage* stage)
    {
        if (m_stage != nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::added2stage trying to add to stage while on a stage");
        }
        m_stage = stage;
        for (auto & child : m_children)
        {
            child->added2stage(stage);
        }
    }

    void Actor::removedFromStage()
    {
        if (m_stage == nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::removedFromStage trying to remove from stage while not on a stage");
            return;
        }
        m_stage->removeEventListeners(this);
        m_stage = nullptr;

        m_pressedOvered = 0;
        for (auto & child : m_children)
        {
            child->removedFromStage();
        }
    }

    void Actor::calcChildrenBounds(RectF& bounds, const AffineTransform& transform) const
    {
#ifdef GRAPHICSUPPORT
        for (auto & child : m_children)
        {
            if (child->getVisible())
            {
                AffineTransform tr = child->getTransform() * transform;
                child->calcBounds2(bounds, tr);
            }
        }
#endif
    }

    void Actor::calcBounds2(RectF& bounds, const AffineTransform& transform) const
    {
#ifdef GRAPHICSUPPORT
        if (!(m_flags & flag_boundsNoChildren))
        {
            calcChildrenBounds(bounds, transform);
        }

        RectF rect;
        if (getBounds(rect))
        {
            bounds.unite(transform.transform(rect.getLeftTop()));
            bounds.unite(transform.transform(rect.getRightTop()));
            bounds.unite(transform.transform(rect.getRightBottom()));
            bounds.unite(transform.transform(rect.getLeftBottom()));
        }
#endif
    }

    RectF Actor::computeBounds(const AffineTransform& transform) const
    {
        RectF bounds = RectF::invalidated();

        calcBounds2(bounds, transform);

        return bounds;
    }

#ifdef GRAPHICSUPPORT
    AffineTransform Actor::computeGlobalTransform(Actor* parent) const
    {
        AffineTransform t;
        const Actor* actor = this;
        while (actor && actor != parent)
        {
            t = t * actor->getTransform();
            actor = actor->getParent();
        }
        return t;
    }
#endif

    pointer_index Actor::getPressed(MouseButton b) const
    {
        return m_pressedButton[b];
    }

    pointer_index Actor::getOvered() const
    {
        return m_overred;
    }

    void Actor::setNotPressed(MouseButton b)
    {
#ifdef GRAPHICSUPPORT
        m_pressedButton[b] = 0;
        if (m_pressedOvered == m_overred)//!_pressed[0] && !_pressed[1] && !_pressed[2])
        {
            Stage* stage = __getStage();
            if (stage)
            {
                stage->removeEventListener(m_onGlobalTouchUpEvent);
            }
        }
#endif
    }

    void Actor::_onGlobalTouchUpEvent(Event* ev)
    {
        TouchEvent* te = safeCast<TouchEvent*>(ev);
        if (te->index != m_pressedButton[te->mouseButton])
        {
            return;
        }
        setNotPressed(te->mouseButton);

        TouchEvent up = *te;
        up.bubbles = false;
        up.localPosition = stage2local(te->localPosition, oxygine::Stage::getStage().get());
        dispatchEvent(&up);
    }

    void Actor::_onGlobalTouchMoveEvent(Event* ev)
    {
#ifdef GRAPHICSUPPORT
        TouchEvent* te = safeCast<TouchEvent*>(ev);
        if (m_overred == 0)
        {
            return;
        }
        if (isDescendant(safeSpCast<Actor>(ev->target)))
        {
            return;
        }
        TouchEvent up = *te;
        up.type = TouchEvent::OUTX;
        up.bubbles = false;
        up.localPosition = stage2local(te->localPosition, oxygine::Stage::getStage().get());
        dispatchEvent(&up);
        oxygine::Stage::getStage()->removeEventListener(m_onGlobalTouchMoveEvent);
        m_overred = 0;
        m_onGlobalTouchMoveEvent = -1;
#endif
    }

    void Actor::dispatchEvent(Event* event)
    {
#ifdef GRAPHICSUPPORT
        if (!m_onScreen)
        {
            dispatchToParent(event);
        }
        else
        {
            if (event->type == TouchEvent::MOVE)
            {
                TouchEvent* te = safeCast<TouchEvent*>(event);
                if (m_overred == 0)
                {
                    m_overred = te->index;
                    if (m_overred == 0)
                    {
                        oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::dispatchEvent hover state is 0");
                    }
                    TouchEvent over = *te;
                    over.type = TouchEvent::OVER;
                    over.bubbles = false;
                    dispatchEvent(&over);
                    m_onGlobalTouchMoveEvent = oxygine::Stage::getStage()->addEventListener(TouchEvent::MOVE, EventCallback(this, &Actor::_onGlobalTouchMoveEvent));
                }
            }

            if (event->type == TouchEvent::TOUCH_DOWN)
            {
                TouchEvent* te = safeCast<TouchEvent*>(event);
                if (!m_pressedButton[te->mouseButton])
                {
                    if (m_pressedOvered == m_overred)//!_pressed[0] && !_pressed[1] && !_pressed[2])
                    {
                        m_onGlobalTouchUpEvent = oxygine::Stage::getStage()->addEventListener(TouchEvent::TOUCH_UP, EventCallback(this, &Actor::_onGlobalTouchUpEvent));
                    }
                    m_pressedButton[te->mouseButton] = te->index;
                }
            }

            TouchEvent click(0);

            if (event->type == TouchEvent::TOUCH_UP)
            {
                TouchEvent* te = safeCast<TouchEvent*>(event);
                if (m_pressedButton[te->mouseButton] == te->index && !te->__clickDispatched)
                {
                    te->__clickDispatched = true;
                    click = *te;
                    click.type = TouchEvent::CLICK;
                    click.bubbles = true;
                    setNotPressed(te->mouseButton);
                }
            }


            EventDispatcher::dispatchEvent(event);
            dispatchToParent(event);

            if (click.type)
            {
                //send click event at the end after TOUCH_UP event
                dispatchEvent(&click);
            }
        }
#endif
    }

    void Actor::dispatchToParent(Event* event)
    {
        if (!event->stopsImmediatePropagation && event->bubbles && !event->stopsPropagation)
        {
            if (m_parent)
            {
                if (TouchEvent::isTouchEvent(event->type))
                {
                    TouchEvent* me = safeCast<TouchEvent*>(event);
                    me->localPosition = local2parent(me->localPosition);
                }

                event->phase = Event::phase_bubbling;
                event->currentTarget = nullptr;
                m_parent->dispatchEvent(event);
            }
        }
    }

    void Actor::handleEvent(Event* event)
    {
        bool touchEvent = TouchEvent::isTouchEvent(event->type);
        if (touchEvent)
        {
#ifdef GRAPHICSUPPORT
            if (!(m_flags & flag_visible))
            {
                return;
            }
            if (getAlpha() == 0 && !(m_flags & flag_clickableWithZeroAlpha))
            {
                return;
            }
#endif
        }

        Vector2 originalLocalPos;
        float originalLocalScale;

        if (touchEvent)
        {
#ifdef GRAPHICSUPPORT
            TouchEvent* me = safeCast<TouchEvent*>(event);
            originalLocalPos = me->localPosition;
            originalLocalScale = me->__localScale;
            me->localPosition = parent2local(originalLocalPos);
            me->__localScale *= m_transform.a;
            if (me->__localScale == NAN)
            {
                oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::handleEvent locale scale is NAN");
            }
#endif
        }

        event->phase = Event::phase_capturing;
        auto iter = m_children.end();
        while (iter != m_children.begin())
        {
            iter--;
            iter->get()->handleEvent(event);
        }
        if (touchEvent)
        {
            TouchEvent* me = safeCast<TouchEvent*>(event);
            if (!event->target)
            {
                if (isOn(me->localPosition, me->__localScale))
                {
                    event->phase = Event::phase_target;
                    event->target = this;

                    me->position = me->localPosition;
                    dispatchEvent(event);
                }
            }

            me->localPosition = originalLocalPos;
            me->__localScale = originalLocalScale;
        }
    }

    void Actor::markTranformDirty()
    {
#ifdef GRAPHICSUPPORT
        m_flags |= flag_transformDirty | flag_transformInvertDirty;
#endif
    }

    void Actor::setAnchor(const Vector2& anchor)
    {
#ifdef GRAPHICSUPPORT
        m_anchor = anchor;
        m_flags &= ~flag_anchorInPixels;
        markTranformDirty();
#endif
    }

    void Actor::setAnchor(float ax, float ay)
    {
        setAnchor(Vector2(ax, ay));
    }

    void Actor::setAnchorInPixels(const Vector2& anchor)
    {
#ifdef GRAPHICSUPPORT
        m_anchor = anchor;
        m_flags |= flag_anchorInPixels;
        markTranformDirty();
#endif
    }

    void Actor::setAnchorInPixels(float x, float y)
    {
        setAnchorInPixels(Vector2(x, y));
    }

    void Actor::setPosition(const Vector2& pos)
    {
#ifdef GRAPHICSUPPORT
        if (m_pos == pos)
        {
            return;
        }
        m_pos.x = static_cast<qint32>(pos.x);
        m_pos.y = static_cast<qint32>(pos.y);
        markTranformDirty();
#endif
    }

    void Actor::setPosition(float x, float y)
    {
        setPosition(Vector2(x, y));
    }

    void Actor::setX(float x)
    {
#ifdef GRAPHICSUPPORT
        if (m_pos.x == x)
        {
            return;
        }
        m_pos.x = static_cast<qint32>(x);
        markTranformDirty();
#endif
    }

    void Actor::setY(float y)
    {
#ifdef GRAPHICSUPPORT
        if (m_pos.y == y)
        {
            return;
        }
        m_pos.y = static_cast<qint32>(y);
        markTranformDirty();
#endif
    }

    void Actor::setAnchorX(float x)
    {
#ifdef GRAPHICSUPPORT
        if (m_anchor.x == x)
        {
            return;
        }
        m_anchor.x = x;
        m_flags &= ~flag_anchorInPixels;
        markTranformDirty();
#endif
    }

    void Actor::setAnchorY(float y)
    {
#ifdef GRAPHICSUPPORT
        if (m_anchor.y == y)
        {
            return;
        }
        m_anchor.y = y;
        m_flags &= ~flag_anchorInPixels;
        markTranformDirty();
#endif
    }

    void Actor::setTransform(const AffineTransform& tr)
    {
#ifdef GRAPHICSUPPORT
        m_transform = tr;
        m_flags &= ~flag_transformDirty;
        m_flags &= ~flag_fastTransform;
        m_flags |= flag_transformInvertDirty;
#endif
    }

    void Actor::setPriority(qint32 zorder)
    {
#ifdef GRAPHICSUPPORT
        if (m_zOrder == zorder) // fixed by Evgeniy Golovin
        {
            return;
        }
        m_zOrder = zorder;
        if (m_parent)
        {
            QMutexLocker lockParent(&m_parent->m_Locked);
            QMutexLocker lock(&m_Locked);
            spActor me = spActor(this);
            auto iter = m_parent->m_children.cbegin();
            while (iter != m_parent->m_children.cend())
            {
                if (iter->get() == me)
                {
                    m_parent->m_children.erase(iter);
                    break;
                }
                ++iter;
            }
            m_parent->insertActor(me);
        }
#endif
    }

    void Actor::setScale(float scale)
    {
        setScale(Vector2(scale, scale));
    }

    void Actor::setScale(const Vector2& scale)
    {
#ifdef GRAPHICSUPPORT
        if (m_scale == scale)
        {
            return;
        }
        m_scale = scale;
        markTranformDirty();
        m_flags &= ~flag_fastTransform;
#endif
    }

    void Actor::setScale(float scaleX, float scaleY)
    {
        setScale(Vector2(scaleX, scaleY));
    }

    void Actor::setScaleX(float sx)
    {
#ifdef GRAPHICSUPPORT
        if (m_scale.x == sx)
        {
            return;
        }
        m_scale.x = sx;
        markTranformDirty();
        m_flags &= ~flag_fastTransform;
#endif
    }

    void Actor::setScaleY(float sy)
    {
#ifdef GRAPHICSUPPORT
        if (m_scale.y == sy)
        {
            return;
        }
        m_scale.y = sy;
        markTranformDirty();
        m_flags &= ~flag_fastTransform;
#endif
    }

    void Actor::setRotation(float rotation)
    {
#ifdef GRAPHICSUPPORT
        if (m_rotation == rotation)
        {
            return;
        }
        m_rotation = rotation;
        markTranformDirty();
        m_flags &= ~flag_fastTransform;
#endif
    }

    void Actor::setRotationDegrees(float degr)
    {
        float rad = degr * M_PI / 180.0f;
        setRotation(rad);
    }

    void Actor::sizeChanged(const Vector2&)
    {
    }

    void Actor::__setSize(const Vector2& size)
    {
#ifdef GRAPHICSUPPORT
        if (m_size == size)
        {
            return;
        }
        m_size.x = static_cast<qint32>(size.x);
        m_size.y = static_cast<qint32>(size.y);
        markTranformDirty();
#endif
    }

    void Actor::setSize(const Vector2& size)
    {
        __setSize(size);
        sizeChanged(size);
    }

    void Actor::setSize(float w, float h)
    {
        setSize(Vector2(w, h));
    }

    void Actor::setWidth(float w)
    {
#ifdef GRAPHICSUPPORT
        setSize(Vector2(w, m_size.y));
#endif
    }

    void Actor::setHeight(float h)
    {
#ifdef GRAPHICSUPPORT
        setSize(Vector2(m_size.x, h));
#endif
    }

    void Actor::setClock(spClock & clock)
    {
        m_clock = clock;
    }

    void Actor::setAlpha(unsigned char alpha)
    {
#ifdef GRAPHICSUPPORT
        m_alpha = alpha;
#endif
    }

#ifdef GRAPHICSUPPORT
    const AffineTransform& Actor::getTransform() const
    {
        updateTransform();
        return m_transform;
    }

    const AffineTransform& Actor::getTransformInvert() const
    {
        if (m_flags & flag_transformInvertDirty)
        {
            m_flags &= ~flag_transformInvertDirty;
            m_transformInvert = getTransform();
            m_transformInvert.invert();
        }

        return m_transformInvert;
    }
#endif

    float Actor::getWidth() const
    {
#ifdef GRAPHICSUPPORT
        return m_size.x;
#else
        return 0.0f;
#endif
    }

    float Actor::getHeight() const
    {
#ifdef GRAPHICSUPPORT
        return m_size.y;
#else
        return 0.0f;
#endif
    }

    unsigned char Actor::getAlpha() const
    {
#ifdef GRAPHICSUPPORT
        return m_alpha;
#else
        return 0;
#endif
    }

    const spClock&  Actor::getClock() const
    {
        return m_clock;
    }

    void Actor::updateTransform() const
    {
#ifdef GRAPHICSUPPORT
        if (!(m_flags & flag_transformDirty))
        {
            return;
        }
        AffineTransform tr;

        if (m_flags & flag_fastTransform)
        {
            tr = AffineTransform(1, 0, 0, 1, m_pos.x, m_pos.y);
        }
        else
        {
            float c = 1.0f;
            float s = 0.0f;
            if (m_rotation)
            {
                c = qCos(m_rotation);
                s = qSin(m_rotation);
            }

            tr = AffineTransform(
                     c * m_scale.x, s * m_scale.x,
                     -s * m_scale.y, c * m_scale.y,
                     m_pos.x, m_pos.y);
        }

        Vector2 offset;
        if (m_flags & flag_anchorInPixels)
        {
            offset.x = -m_anchor.x;
            offset.y = -m_anchor.y;
        }
        else
        {
            offset.x = -float(m_size.x * m_anchor.x);
            offset.y = -float(m_size.y * m_anchor.y);
        }

        tr.translate(offset);
        m_transform = tr;
        m_flags &= ~flag_transformDirty;
#endif
    }

    bool Actor::isOn(const Vector2& localPosition, float)
    {
        RectF r = getDestRect();
        r.expand(Vector2(m_extendedIsOn, m_extendedIsOn), Vector2(m_extendedIsOn, m_extendedIsOn));

        if (r.pointIn(localPosition))
        {
             return true;
        }
        return false;
    }

    bool Actor::isDescendant(const spActor& actor) const
    {
        const Actor* act = actor.get();
        while (act)
        {
            if (act == this)
            {
                return true;
            }
            act = act->getParent();
        }
        return false;
    }

    void Actor::setParent(Actor* actor, Actor* parent)
    {
        actor->m_parent = parent;
        if (parent != nullptr &&
            parent->__getStage())
        {
            actor->added2stage(parent->__getStage());
        }
        else
        {
            if (actor->__getStage())
            {
                actor->removedFromStage();
            }
        }
    }

    void Actor::insertActor(spActor & actor)
    {
        qint32 z = actor->getPriority();
        auto iter = m_children.cend();
        auto insertBefore = iter;
        while (iter != m_children.cbegin())
        {
            iter--;
            if (iter->get()->getPriority() <= z)
            {
                break;
            }
            insertBefore = iter;
        }
        m_children.insert(insertBefore, actor);
    }

    void Actor::addChild(spActor actor)
    {
        if (!GameWindow::getWindow()->isWorker())
        {
            QString name = QThread::currentThread()->objectName();
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::addChild trying to add actor from wrong thread. Thread: " + name + " worker running: " + QString::number(GameWindow::getWindow()->isWorkerRunning()));
        }
        else if (actor.get() == nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::addChild trying to add empty actor");
            return;
        }
        else if (actor.get() == this)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::addChild trying to add self");
        }
        actor->detach();
#ifdef GRAPHICSUPPORT
        QMutexLocker lock(&m_Locked);
        QMutexLocker lockActor(&(actor->m_Locked));
#endif
        insertActor(actor);
        setParent(actor.get(), this);
    }

    void Actor::removeChild(spActor & actor)
    {
#ifdef GRAPHICSUPPORT
        QMutexLocker lock(&m_Locked);
        QMutexLocker lockActor(&(actor->m_Locked));
#endif
        if (!GameWindow::getWindow()->isWorker() &&
            dynamic_cast<QObject*>(this) != nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::removeChild trying to remove actor from wrong thread");
        }
        else if (actor)
        {
            if (actor->m_parent != this)
            {
                oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::removeChild wrong parent while removing a child");
            }
            else
            {                
                setParent(actor.get(), nullptr);
                auto iter = m_children.cbegin();
                while (iter != m_children.cend())
                {
                    if (iter->get() == actor)
                    {
                        m_children.erase(iter);
                        break;
                    }
                    ++iter;
                }
            }
        }
        else
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::removeChild trying to remove empty actor");
        }
    }

    void Actor::removeChildren()
    {
        while (m_children.size() > 0)
        {
            spActor child = m_children.back();
            removeChild(child);
        }
    }

    Actor* Actor::detach()
    {
        Actor* parent = getParent();
        if (parent)
        {
            spActor pActor(this);
            parent->removeChild(pActor);
        }
        return parent;
    }

    void Actor::internalUpdate(const UpdateState& us)
    {
#ifdef GRAPHICSUPPORT
        QMutexLocker lock(&m_Locked);
        auto iter = m_tweens.begin();
        while (iter != m_tweens.end())
        {
            auto* tween = iter->get();
            tween->update(*this, us);
            if (tween->isDone())
            {
                iter = m_tweens.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
        doUpdate(us);
        for (auto & child : m_children)
        {
            child->update(us);
        }
#endif
    }

    void Actor::update(const UpdateState& parentUS)
    {
        UpdateState us = parentUS;
        if (m_clock)
        {
            us.iteration = 0;
            m_clock->update();

            timeMS dt = m_clock->doTick();
            while (dt > timeMS(0))
            {
                us.dt = dt;
                us.time = m_clock->getTime();

                internalUpdate(us);

                dt = m_clock->doTick();
                us.iteration += 1;
            }
        }
        else
        {
            internalUpdate(us);
        }

    }

    void Actor::doUpdate(const UpdateState&)
    {

    }

    Vector2 Actor::parent2local(const Vector2& global) const
    {
#ifdef GRAPHICSUPPORT
        const AffineTransform& t = getTransformInvert();
        return t.transform(global);
#else
        return Vector2();
#endif
    }

    Vector2 Actor::local2parent(const Vector2& local) const
    {
#ifdef GRAPHICSUPPORT
        const AffineTransform& t = getTransform();
        return t.transform(local);
#else
        return Vector2();
#endif
    }

    Vector2 Actor::local2stage(const Vector2& pos, Actor* stage) const
    {
        return convert_local2stage(this, pos, stage);
    }

    Vector2 Actor::local2stage(float x, float y, Actor* stage) const
    {
        return convert_local2stage(this, Vector2(x, y), stage);
    }

    Vector2 Actor::stage2local(const Vector2& pos, Actor* stage) const
    {
        return convert_stage2local(this, pos, stage);
    }

    Vector2 Actor::stage2local(float x, float y, Actor* stage) const
    {
        return convert_stage2local(this, Vector2(x, y), stage);
    }

    bool Actor::prepareRender(RenderState& rs, const RenderState& parentRS)
    {
#ifdef GRAPHICSUPPORT
        if (!(m_flags & flag_visible))
        {
            return false;
        }
        unsigned char alpha = (parentRS.alpha * m_alpha) / 255;
        if (!alpha)
        {
            return false;
        }
        rs = parentRS;
        rs.alpha = alpha;


        const AffineTransform& tr = getTransform();
        if (m_flags & flag_fastTransform)
        {
            rs.transform = parentRS.transform;
            rs.transform.translate(Vector2(tr.x, tr.y));
        }
        else
        {
            AffineTransform::multiply(rs.transform, tr, parentRS.transform);
        }

        if (m_flags & flag_cull)
        {
            RectF ss_rect = getActorTransformedDestRect(this, rs.transform);
            RectF intersection = ss_rect;
            intersection.clip(*rs.clip);
            if (intersection.isEmpty())
            {
                return false;
            }
        }
#endif
        return true;
    }

    bool Actor::onScreen(RenderState& rs)
    {
#ifdef GRAPHICSUPPORT
        constexpr float safetyArea = 10;
        float width = oxygine::Stage::getStage()->getScaledWidth() + safetyArea;
        float height = oxygine::Stage::getStage()->getScaledHeight() + safetyArea;
        auto scaledWidth = m_size.x * rs.transform.a;
        auto scaledHeight = m_size.y * rs.transform.d;
        if (rs.transform.x > width ||
            rs.transform.y > height ||
            rs.transform.x + scaledWidth < -safetyArea ||
            rs.transform.y + scaledHeight < -safetyArea)
        {
            return false;
        }
#endif
        return true;
    }

    bool Actor::internalRender(RenderState& rs, const RenderState& parentRS)
    {
#ifdef GRAPHICSUPPORT
        if (!prepareRender(rs, parentRS))
        {
            return false;
        }
        rs.transform.x = static_cast<qint32>(rs.transform.x);
        rs.transform.y = static_cast<qint32>(rs.transform.y);
        m_onScreen = onScreen(rs);
        if (m_onScreen)
        {
            doRender(rs);
        }
#endif
        return true;
    }

    bool Actor::getBounds(RectF& bounds) const
    {
#ifdef GRAPHICSUPPORT
        if (m_flags & flag_actorHasBounds)
        {
            bounds = getDestRect();
            return true;
        }
#endif
        return false;
    }

    void Actor::render(const RenderState& parentRS)
    {
#ifdef GRAPHICSUPPORT
        QMutexLocker lock(&m_Locked);
        RenderDelegate::instance->render(this, parentRS);
#endif
    }

    RectF Actor::getDestRect() const
    {
        return RectF(Point(0.0f, 0.0f), getSize());
    }

    spTween Actor::__addTween(spTween tween, bool)
    {        
#ifdef GRAPHICSUPPORT
        if (tween.get() == nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::__addTween tween is nullptr");
            return spTween();
        }
        else if (getRefCounter() == 0)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::__addTween trying to add tween during actor construction isn't allowed.");
            return spTween();
        }
        {
            QMutexLocker lock(&m_Locked);
            m_tweens.push_back(tween);
        }
        tween->start(*this);
#endif
        return tween;

    }

    spTween Actor::addTween(spTween tween)
    {
        return __addTween(tween, false);
    }

    void Actor::removeTween(spTween pTween)
    {        
#ifdef GRAPHICSUPPORT
        QMutexLocker lock(&m_Locked);
        if (pTween.get() == nullptr)
        {
            return;
        }
        auto iter = m_tweens.begin();
        while (iter != m_tweens.end())
        {
            if (iter->get() == pTween)
            {
                pTween->setClient(nullptr);
                m_tweens.erase(iter);
                break;
            }
            ++iter;
        }
#endif
    }

    void Actor::removeTweens(bool callComplete)
    {        
#ifdef GRAPHICSUPPORT
        while (m_tweens.size() > 0)
        {
            spTween tween = m_tweens.back();
            if (callComplete)
            {
                tween->complete();
            }
            else
            {
                removeTween(tween);
            }
        }
#endif
    }

    Vector2 Actor::convert_global2local_(const Actor* child, const Actor* parent, Vector2 pos)
    {        
#ifdef GRAPHICSUPPORT
        if (child->getParent() && child->getParent() != parent)
        {
            pos = convert_global2local_(child->getParent(), parent, pos);
        }
        pos = child->parent2local(pos);
#endif
        return pos;
    }

    Vector2 Actor::convert_global2local(spActor & child, spActor & parent, const Vector2& pos)
    {
        return convert_global2local_(child.get(), parent.get(), pos);
    }

    Vector2 Actor::convert_local2global_(const Actor* child, const Actor* parent, Vector2 pos)
    {
#ifdef GRAPHICSUPPORT
        while (child && child != parent)
        {
            pos = child->local2parent(pos);
            child = child->getParent();
        }
#endif
        return pos;
    }

    Vector2 Actor::convert_local2global(spActor & child, spActor & parent, const Vector2& pos)
    {
        return convert_local2global_(child.get(), parent.get(), pos);
    }

    Vector2 Actor::convert_local2stage(spActor & actor, const Vector2& pos, spActor root)
    {
        if (!root)
        {
            root = Stage::getStage();
        }
        return convert_local2global(actor, root, pos);
    }

    Vector2 Actor::convert_local2stage(const Actor* actor, const Vector2& pos, const Actor* root)
    {
        if (!root)
        {
            root = Stage::getStage().get();
        }
        return convert_local2global_(actor, root, pos);
    }

    Vector2 Actor::convert_stage2local(spActor & actor, const Vector2& pos, spActor root)
    {
        if (!root)
        {
            root = Stage::getStage();
        }
        return convert_global2local(actor, root, pos);
    }

    Vector2 Actor::convert_stage2local(const Actor* actor, const Vector2& pos, const Actor* root)
    {
        if (!root)
        {
            root = Stage::getStage().get();
        }
        return convert_global2local_(actor, root, pos);
    }

    RectF Actor::getActorTransformedDestRect(Actor* actor, const AffineTransform& tr)
    {
        RectF rect = actor->getDestRect();
        Vector2 tl = rect.pos;
        Vector2 br = rect.pos + rect.size;

        tl = tr.transform(tl);
        br = tr.transform(br);

        Vector2 size = Vector2(
                           qAbs(br.x - tl.x),
                           qAbs(br.y - tl.y));

        Vector2 ntl;
        ntl.x = std::min(tl.x, br.x);
        ntl.y = std::min(tl.y, br.y);

        return RectF(ntl, size);
    }
}
