#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"
#include "3rd_party/oxygine-framework/oxygine/core/Texture.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/math/AffineTransform.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"
#include "3rd_party/oxygine-framework/oxygine/tween/Tween.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/math/OBBox.h"

#include <qmath.h>


namespace oxygine
{
    Actor::Actor()
        : m_rdelegate(STDRenderDelegate::instance.get()),
          m_stage(nullptr),
          m_flags(flag_visible | flag_touchEnabled | flag_touchChildrenEnabled | flag_fastTransform),
          m_alpha(255),
          m_extendedIsOn(0),
          m_parent(nullptr),
          m_scale(1, 1),
          m_rotation(0),
          m_zOrder(0)
    {
        m_transform.identity();
        m_transformInvert.identity();
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
        Q_ASSERT(m_stage == nullptr);
        m_stage = stage;

        spActor actor = m_children._first;
        while (actor)
        {
            spActor next = actor->_next;
            actor->added2stage(stage);
            actor = next;
        }

        onAdded2Stage();
    }

    void Actor::removedFromStage()
    {
        Q_ASSERT(m_stage);

        onRemovedFromStage();
        m_stage->removeEventListeners(this);
        m_stage = 0;

        m_pressedOvered = 0;

        spActor actor = m_children._first;
        while (actor)
        {
            spActor next = actor->_next;
            actor->removedFromStage();
            actor = next;
        }
    }

    void Actor::transformUpdated()
    {
    }

    void Actor::calcChildrenBounds(RectF& bounds, const Transform& transform) const
    {
        const Actor* c = getFirstChild().get();
        while (c)
        {
            if (c->getVisible())
            {
                Transform tr = c->getTransform() * transform;
                c->calcBounds2(bounds, tr);
            }
            c = c->getNextSibling().get();
        }
    }

    void Actor::calcBounds2(RectF& bounds, const Transform& transform) const
    {
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
    }

    RectF Actor::computeBounds(const Transform& transform) const
    {
        RectF bounds = RectF::invalidated();

        calcBounds2(bounds, transform);

        return bounds;
    }

    RectF Actor::computeBoundsInParent() const
    {
        return computeBounds(getTransform());
    }

    oxygine::RectF Actor::computeStageBounds() const
    {
        return computeBounds(computeGlobalTransform());
    }

    Transform Actor::computeGlobalTransform(Actor* parent) const
    {
        Transform t;
        t.identity();
        const Actor* actor = this;
        while (actor && actor != parent)
        {
            t = t * actor->getTransform();
            actor = actor->getParent();
        }

        return t;
    }

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
        m_pressedButton[b] = 0;
        if (m_pressedOvered == m_overred)//!_pressed[0] && !_pressed[1] && !_pressed[2])
        {
            Stage* stage = __getStage();
            if (stage)
            {
                stage->removeEventListener(m_onGlobalTouchUpEvent);
            }
        }

        updateStatePressed();
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
        up.localPosition = stage2local(te->localPosition, __getStage());
        dispatchEvent(&up);
    }

    void Actor::_onGlobalTouchMoveEvent(Event* ev)
    {
        TouchEvent* te = safeCast<TouchEvent*>(ev);
        if (m_overred == 0)
        {
            return;
        }
        if (isDescendant(safeCast<Actor*>(ev->target.get())))
        {
            return;
        }
        TouchEvent up = *te;
        up.type = TouchEvent::OUTX;
        up.bubbles = false;
        up.localPosition = stage2local(te->localPosition, __getStage());
        dispatchEvent(&up);
        updateStateOvered();
        __getStage()->removeEventListener(m_onGlobalTouchMoveEvent);
        m_overred = 0;
        m_onGlobalTouchMoveEvent = -1;
    }

    void Actor::dispatchEvent(Event* event)
    {
        if (event->type == TouchEvent::MOVE)
        {
            TouchEvent* te = safeCast<TouchEvent*>(event);
            if (m_overred == 0)
            {
                m_overred = te->index;
                Q_ASSERT(m_overred > 0);
                updateStateOvered();

                TouchEvent over = *te;
                over.type = TouchEvent::OVER;
                over.bubbles = false;
                dispatchEvent(&over);
                m_onGlobalTouchMoveEvent = __getStage()->addEventListener(TouchEvent::MOVE, EventCallback(this, &Actor::_onGlobalTouchMoveEvent));
            }
        }

        if (event->type == TouchEvent::TOUCH_DOWN)
        {
            TouchEvent* te = safeCast<TouchEvent*>(event);
            if (!m_pressedButton[te->mouseButton])
            {
                if (m_pressedOvered == m_overred)//!_pressed[0] && !_pressed[1] && !_pressed[2])
                {
                    m_onGlobalTouchUpEvent = __getStage()->addEventListener(TouchEvent::TOUCH_UP, EventCallback(this, &Actor::_onGlobalTouchUpEvent));
                }
                m_pressedButton[te->mouseButton] = te->index;
                updateStatePressed();
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
                //will be dispatched later after UP

                setNotPressed(te->mouseButton);
            }
        }


        EventDispatcher::dispatchEvent(event);

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
                event->currentTarget = 0;
                m_parent->dispatchEvent(event);
            }
        }

        if (click.type)
        {
            //send click event at the end after TOUCH_UP event
            dispatchEvent(&click);
        }
    }

    void Actor::handleEvent(Event* event)
    {
        bool touchEvent = TouchEvent::isTouchEvent(event->type);
        if (touchEvent)
        {
            if (!(m_flags & flag_visible))
            {
                return;
            }
            if (getAlpha() == 0 && !(m_flags & flag_clickableWithZeroAlpha))
            {
                return;
            }
        }

        Vector2 originalLocalPos;
        float originalLocalScale;

        if (touchEvent)
        {
            TouchEvent* me = safeCast<TouchEvent*>(event);
            originalLocalPos = me->localPosition;
            originalLocalScale = me->__localScale;
            me->localPosition = parent2local(originalLocalPos);
            me->__localScale *= m_transform.a;
            if (me->__localScale == NAN)
            {
                Q_ASSERT(0);
            }
        }

        event->phase = Event::phase_capturing;
        spActor actor = m_children._last;
        while (actor)
        {
            spActor prev = actor->_prev;
            if (!touchEvent || (m_flags & flag_touchChildrenEnabled))
            {
                actor->handleEvent(event);
            }
            actor = prev;
        }

        if (touchEvent)
        {
            TouchEvent* me = safeCast<TouchEvent*>(event);
            if (!event->target)
            {
                if ((m_flags & flag_touchEnabled) && isOn(me->localPosition, me->__localScale))
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
        m_flags |= flag_transformDirty | flag_transformInvertDirty;
    }

    void Actor::setAnchor(const Vector2& anchor)
    {
        m_anchor = anchor;
        m_flags &= ~flag_anchorInPixels;
        markTranformDirty();
    }

    void Actor::setAnchor(float ax, float ay)
    {
        setAnchor(Vector2(ax, ay));
    }

    void Actor::setAnchorInPixels(const Vector2& anchor)
    {
        m_anchor = anchor;
        m_flags |= flag_anchorInPixels;
        markTranformDirty();
    }

    void Actor::setAnchorInPixels(float x, float y)
    {
        setAnchorInPixels(Vector2(x, y));
    }

    void Actor::setPosition(const Vector2& pos)
    {
        if (m_pos == pos)
        {
            return;
        }
        m_pos = pos;
        markTranformDirty();
    }

    void Actor::setPosition(float x, float y)
    {
        setPosition(Vector2(x, y));
    }

    void Actor::setX(float x)
    {
        if (m_pos.x == x)
        {
            return;
        }
        m_pos.x = x;
        markTranformDirty();
    }

    void Actor::setY(float y)
    {
        if (m_pos.y == y)
        {
            return;
        }
        m_pos.y = y;
        markTranformDirty();
    }

    void Actor::setAnchorX(float x)
    {
        if (m_anchor.x == x)
        {
            return;
        }
        m_anchor.x = x;
        m_flags &= ~flag_anchorInPixels;
        markTranformDirty();
    }

    void Actor::setAnchorY(float y)
    {
        if (m_anchor.y == y)
        {
            return;
        }
        m_anchor.y = y;
        m_flags &= ~flag_anchorInPixels;
        markTranformDirty();
    }

    void Actor::setTransform(const AffineTransform& tr)
    {
        m_transform = tr;
        m_flags &= ~flag_transformDirty;
        m_flags &= ~flag_fastTransform;
        m_flags |= flag_transformInvertDirty;
    }

    void Actor::setPriority(qint32 zorder)
    {
        if (m_zOrder == zorder) // fixed by Evgeniy Golovin
        {
            return;
        }

        m_zOrder = zorder;
        if (m_parent)
        {
            Actor* parent = m_parent;

            spActor me = this;

            parent->m_children.removeItem(me);

            Actor* sibling = parent->m_children._last.get();

            //try to insert at the end of list first
            if (sibling && sibling->getPriority() > m_zOrder)
            {
                sibling = sibling->intr_list::_prev.get();
                while (sibling)
                {
                    if (sibling->getPriority() <= m_zOrder)
                    {
                        break;
                    }
                    sibling = sibling->intr_list::_prev.get();
                }
            }

            if (sibling)
            {
                spActor s = sibling;
                parent->m_children.insert_after(me, s);
            }
            else
            {
                parent->m_children.prepend(me);
            }
        }
    }

    void Actor::setScale(float scale)
    {
        setScale(Vector2(scale, scale));
    }

    void Actor::setScale(const Vector2& scale)
    {
        if (m_scale == scale)
        {
            return;
        }
        m_scale = scale;
        markTranformDirty();
        m_flags &= ~flag_fastTransform;
    }

    void Actor::setScale(float scaleX, float scaleY)
    {
        setScale(Vector2(scaleX, scaleY));
    }

    void Actor::setScaleX(float sx)
    {
        if (m_scale.x == sx)
        {
            return;
        }
        m_scale.x = sx;
        markTranformDirty();
        m_flags &= ~flag_fastTransform;
    }

    void Actor::setScaleY(float sy)
    {
        if (m_scale.y == sy)
        {
            return;
        }
        m_scale.y = sy;
        markTranformDirty();
        m_flags &= ~flag_fastTransform;
    }

    void Actor::setRotation(float rotation)
    {
        if (m_rotation == rotation)
        {
            return;
        }
        m_rotation = rotation;
        markTranformDirty();
        m_flags &= ~flag_fastTransform;
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
        if (m_size == size)
        {
            return;
        }
        m_size = size;
        markTranformDirty();
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
        setSize(Vector2(w, m_size.y));
    }

    void Actor::setHeight(float h)
    {
        setSize(Vector2(m_size.x, h));
    }

    void Actor::setClock(spClock clock)
    {
        m_clock = clock;
    }

    void Actor::setAlpha(unsigned char alpha)
    {
        m_alpha = alpha;
    }

    void Actor::setRenderDelegate(RenderDelegate* mat)
    {
        m_rdelegate = mat;
    }

    const Transform& Actor::getTransform() const
    {
        updateTransform();
        return m_transform;
    }

    const Transform& Actor::getTransformInvert() const
    {
        if (m_flags & flag_transformInvertDirty)
        {
            m_flags &= ~flag_transformInvertDirty;
            m_transformInvert = getTransform();
            m_transformInvert.invert();
        }

        return m_transformInvert;
    }

    float Actor::getWidth() const
    {
        return m_size.x;
    }

    float Actor::getHeight() const
    {
        return m_size.y;
    }

    unsigned char Actor::getAlpha() const
    {
        return m_alpha;
    }

    const spClock&  Actor::getClock() const
    {
        return m_clock;
    }

    void Actor::updateTransform() const
    {
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
            offset.y = -float(m_size.y * m_anchor.y);//todo, what to do? (per pixel quality)
        }

        tr.translate(offset);


        m_transform = tr;
        m_flags &= ~flag_transformDirty;

        const_cast<Actor*>(this)->transformUpdated();
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
        if (parent && parent->__getStage())
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

    void Actor::insertSiblingBefore(spActor actor)
    {
        Q_ASSERT(actor != this);
        Q_ASSERT(actor);
        Q_ASSERT(m_parent);
        if (!m_parent)
        {
            return;
        }
        actor->detach();
        spActor t = this;
        m_parent->m_children.insert_before(actor, t);
        setParent(actor.get(), m_parent);
    }

    void Actor::insertSiblingAfter(spActor actor)
    {
        Q_ASSERT(actor != this);
        Q_ASSERT(actor);
        Q_ASSERT(m_parent);
        if (!m_parent)
        {
            return;
        }
        actor->detach();
        spActor t = this;
        m_parent->m_children.insert_after(actor, t);
        setParent(actor.get(), m_parent);
    }

    void Actor::attachTo(spActor parent)
    {
        Q_ASSERT(parent != this);
        attachTo(parent.get());
    }

    void Actor::attachTo(Actor* parent)
    {
        Q_ASSERT(parent != this);
        Q_ASSERT(parent);
        if (!parent)
        {
            return;
        }
        parent->addChild(this);
    }

    void Actor::addChild(Actor* actor)
    {
        Q_ASSERT(actor);
        if (!actor)
        {
            return;
        }

        Q_ASSERT(actor != this);

        actor->detach();

        qint32 z = actor->getPriority();

        spActor sibling = m_children._last;

        //try to insert at the end of list first
        if (sibling && sibling->getPriority() > z)
        {
            sibling = sibling->getPrevSibling();
            while (sibling)
            {
                if (sibling->getPriority() <= z)
                {
                    break;
                }
                sibling = sibling->getPrevSibling();
            }
        }
        if (sibling)
        {
            sibling->insertSiblingAfter(actor);
        }
        else
        {
            spActor t = actor;
            m_children.prepend(t);
            setParent(actor, this);
        }
    }

    void Actor::prependChild(spActor actor)
    {
        prependChild(actor.get());
    }

    void Actor::prependChild(Actor* actor)
    {
        if (actor == getFirstChild().get())
        {
            return;
        }
        if (getFirstChild())
        {
            getFirstChild()->insertSiblingBefore(actor);
        }
        else
        {
            addChild(actor);
        }
    }

    void Actor::addChild(spActor actor)
    {
        addChild(actor.get());
    }

    void Actor::removeChild(spActor actor)
    {
        Q_ASSERT(actor);
        if (actor)
        {
            Q_ASSERT(actor->m_parent == this);
            if (actor->m_parent == this)
            {
                QMutexLocker lock(&m_Locked);
                setParent(actor.get(), nullptr);
                m_children.removeItem(actor);
            }
        }
    }

    void Actor::removeChildren()
    {
        spActor child = getFirstChild();
        while (child)
        {
            spActor copy = child;
            child = child->getNextSibling();
            removeChild(copy);
        }
    }


    Actor* Actor::detach()
    {
        Actor* parent = getParent();
        if (parent)
        {
            parent->removeChild(this);
        }
        return parent;
    }

    void Actor::internalUpdate(const UpdateState& us)
    {
        QMutexLocker lock(&m_Locked);
        spTween tween = m_tweens._first;
        while (tween)
        {
            spTween tweenNext = tween->getNextSibling();

            if (tween->getParentList())
            {
                tween->update(*this, us);
            }
            if (tween->isDone() && tween->getParentList())
            {
                m_tweens.removeItem(tween);
            }
            tween = tweenNext;
        }
        doUpdate(us);

        spActor actor = m_children._first;
        while (actor)
        {
            spActor next = actor->_next;
            if (actor->getParent())
            {
                actor->update(us);
            }
            actor = next;
        }
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
        const AffineTransform& t = getTransformInvert();
        return t.transform(global);
    }

    Vector2 Actor::local2parent(const Vector2& local) const
    {
        const AffineTransform& t = getTransform();
        return t.transform(local);
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


        const Transform& tr = getTransform();
        if (m_flags & flag_fastTransform)
        {
            rs.transform = parentRS.transform;
            rs.transform.translate(Vector2(tr.x, tr.y));
        }
        else
        {
            Transform::multiply(rs.transform, tr, parentRS.transform);
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

        return true;
    }

    bool Actor::onScreen(RenderState& rs)
    {
        float width = oxygine::getStage()->getWidth();
        float height = oxygine::getStage()->getHeight();
        auto scaledSize = getScaledSize();
        static constexpr float  extension = 50.0f;
        if (rs.transform.x > width + extension ||
            rs.transform.y > height + extension ||
            rs.transform.x + scaledSize.x < -extension ||
            rs.transform.y + scaledSize.y < -extension)
        {
            return false;
        }
        return true;
    }

    void Actor::completeRender(const RenderState&)
    {

    }

    bool Actor::internalRender(RenderState& rs, const RenderState& parentRS)
    {
        if (!prepareRender(rs, parentRS))
        {
            return false;
        }
        if (onScreen(rs))
        {
            doRender(rs);
        }
        completeRender(rs);
        return true;
    }

    void Actor::clean()
    {
        removeTweens();
        removeAllEventListeners();

        spActor child = getFirstChild();
        while (child)
        {
            child->clean();
            child = child->getNextSibling();
        }
    }

    bool Actor::getBounds(RectF& bounds) const
    {
        if (m_flags & flag_actorHasBounds)
        {
            bounds = getDestRect();
            return true;
        }
        return false;
    }

    void Actor::render(const RenderState& parentRS)
    {
        QMutexLocker lock(&m_Locked);
        m_rdelegate->render(this, parentRS);
    }

    RectF Actor::getDestRect() const
    {
        return RectF(Vector2(0, 0), getSize());
    }

    spTween Actor::__addTween(spTween tween, bool)
    {
        Q_ASSERT(tween);
        QMutexLocker lock(&m_Locked);
        if (!tween)
        {
            return nullptr;
        }
        tween->start(*this);
        m_tweens.append(tween);
        return tween;
    }

    spTween Actor::addTween(spTween tween)
    {
        return __addTween(tween, false);
    }

    spTween Actor::addTween2(spTween tween, const TweenOptions& opt)
    {
        tween->init2(opt);
        return __addTween(tween, false);
    }

    void Actor::removeTween(spTween pTween)
    {
        QMutexLocker lock(&m_Locked);
        if (pTween.get() == nullptr)
        {
            return;
        }
        if (pTween->getParentList() == &m_tweens)
        {
            pTween->setClient(nullptr);
            m_tweens.removeItem(pTween);
        }
    }

    void Actor::removeTweens(bool callComplete)
    {
        spTween t = m_tweens._first;
        while (t)
        {
            spTween c = t;
            t = t->getNextSibling();

            if (callComplete)
            {
                c->complete();
            }
            else
            {
                removeTween(c);
            }
        }
    }

    Vector2 convert_global2local_(const Actor* child, const Actor* parent, Vector2 pos)
    {
        if (child->getParent() && child->getParent() != parent)
        {
            pos = convert_global2local_(child->getParent(), parent, pos);
        }

        pos = child->parent2local(pos);
        return pos;
    }

    Vector2 convert_global2local(spActor child, spActor parent, const Vector2& pos)
    {
        return convert_global2local_(child.get(), parent.get(), pos);
    }

    Vector2 convert_local2global_(const Actor* child, const Actor* parent, Vector2 pos)
    {
        while (child && child != parent)
        {
            pos = child->local2parent(pos);
            child = child->getParent();
        }

        return pos;
    }

    Vector2 convert_local2global(spActor child, spActor parent, const Vector2& pos)
    {
        return convert_local2global_(child.get(), parent.get(), pos);
    }

    Vector2 convert_local2stage(spActor actor, const Vector2& pos, spActor root)
    {
        if (!root)
        {
            root = getStage();
        }
        return convert_local2global(actor, root, pos);
    }

    Vector2 convert_local2stage(const Actor* actor, const Vector2& pos, const Actor* root)
    {
        if (!root)
        {
            root = getStage().get();
        }
        return convert_local2global_(actor, root, pos);
    }

    Vector2 convert_stage2local(spActor actor, const Vector2& pos, spActor root)
    {
        if (!root)
        {
            root = getStage();
        }
        return convert_global2local(actor, root, pos);
    }

    Vector2 convert_stage2local(const Actor* actor, const Vector2& pos, const Actor* root)
    {
        if (!root)
        {
            root = getStage().get();
        }
        return convert_global2local_(actor, root, pos);
    }

    Transform getGlobalTransform(spActor child, spActor parent)
    {
        Transform t;
        t.identity();
        while (child && child != parent)
        {
            t = t * child->getTransform();
            child = child->getParent();
        }

        return t;
    }

    Transform getGlobalTransform2(spActor child, Actor* parent)
    {
        Transform t;
        t.identity();
        while (child && (child.get() != parent))
        {
            t = t * child->getTransform();
            child = child->getParent();
        }

        return t;
    }

    void changeParentAndSavePosition(spActor mutualParent, spActor actor, spActor newParent)
    {
        Vector2 pos = actor->getPosition();
        spActor act = actor->getParent();
        while (act && act != mutualParent)
        {
            pos = act->local2parent(pos);
            act = act->getParent();
        }

        if (newParent != mutualParent)
        {
            pos = convert_global2local(newParent, mutualParent, pos);
        }
        actor->setPosition(pos);
        actor->attachTo(newParent);
    }

    RectF getActorTransformedDestRect(Actor* actor, const Transform& tr)
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

    extern qint32 HIT_TEST_DOWNSCALE;

    bool testIntersection(spActor objA, spActor objB, spActor parent, Vector2* contact)
    {
        float s1 = objB->getSize().x * objB->getSize().y;
        float s2 = objA->getSize().x * objA->getSize().y;
        bool swapped = false;
        if (s2 < s1)
        {
            swapped = true;
            std::swap(objA, objB);
        }

        Transform transA = objA->computeGlobalTransform(parent.get());
        Transform transB = objB->computeGlobalTransform(parent.get());
        //Transform transBInv = getGlobalTransform(objB, parent);
        transB.invert();
        Transform n = transA * transB;

        AffineTransform ident;
        ident.identity();

        OBBox a(objB->getDestRect(), ident);
        OBBox b(objA->getDestRect(), n);
        if (!a.overlaps(b))
        {
            return false;
        }
        qint32 w = (int)objA->getWidth();
        qint32 h = (int)objA->getHeight();


        for (qint32 y = 0; y < h; y += HIT_TEST_DOWNSCALE)
        {
            for (qint32 x = 0; x < w; x += HIT_TEST_DOWNSCALE)
            {
                Vector2 posA = Vector2(float(x), float(y));

                if (!objA->isOn(posA))
                {
                    continue;
                }
                Vector2 posB = n.transform(posA);

                if (!objB->isOn(posB))
                {
                    continue;
                }

                if (contact)
                {
                    *contact = swapped ? posB : posA;
                }
                return true;
            }
        }

        return false;
    }
}
