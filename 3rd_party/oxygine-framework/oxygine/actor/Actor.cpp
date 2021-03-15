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
        : _rdelegate(STDRenderDelegate::instance.get()),
          _stage(nullptr),
          _flags(flag_visible | flag_touchEnabled | flag_touchChildrenEnabled | flag_fastTransform),
          _alpha(255),
          _extendedIsOn(0),
          _parent(nullptr),
          _scale(1, 1),
          _rotation(0),
          _zOrder(0)
    {
        _transform.identity();
        _transformInvert.identity();
        _pressedOvered = 0;
    }

    Actor::~Actor()
    {
        removeTweens();
        removeChildren();
    }

    Stage* Actor::_getStage()
    {
        return _stage;
    }

    void Actor::added2stage(Stage* stage)
    {
        Q_ASSERT(_stage == nullptr);
        _stage = stage;

        spActor actor = _children._first;
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
        Q_ASSERT(_stage);

        onRemovedFromStage();
        _stage->removeEventListeners(this);
        _stage = 0;

        _pressedOvered = 0;

        spActor actor = _children._first;
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
        if (!(_flags & flag_boundsNoChildren))
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
        return _pressedButton[b];
    }

    pointer_index Actor::getOvered() const
    {
        return _overred;
    }

    void Actor::setNotPressed(MouseButton b)
    {
        _pressedButton[b] = 0;
        if (_pressedOvered == _overred)//!_pressed[0] && !_pressed[1] && !_pressed[2])
        {
            Stage* stage = _getStage();
            if (stage)
            {
                stage->removeEventListener(TouchEvent::TOUCH_UP, EventCallback(this, &Actor::_onGlobalTouchUpEvent));
            }
        }

        updateStatePressed();
    }

    void Actor::_onGlobalTouchUpEvent(Event* ev)
    {
        TouchEvent* te = safeCast<TouchEvent*>(ev);
        if (te->index != _pressedButton[te->mouseButton])
        {
            return;
        }
        setNotPressed(te->mouseButton);

        TouchEvent up = *te;
        up.bubbles = false;
        up.localPosition = stage2local(te->localPosition, _getStage());
        dispatchEvent(&up);
    }

    void Actor::_onGlobalTouchMoveEvent(Event* ev)
    {
        TouchEvent* te = safeCast<TouchEvent*>(ev);
        if (_overred == 0)
        {
            return;
        }
        if (isDescendant(safeCast<Actor*>(ev->target.get())))
        {
            return;
        }

        _overred = 0;
        _getStage()->removeEventListener(TouchEvent::MOVE, EventCallback(this, &Actor::_onGlobalTouchMoveEvent));

        TouchEvent up = *te;
        up.type = TouchEvent::OUTX;
        up.bubbles = false;
        up.localPosition = stage2local(te->localPosition, _getStage());
        dispatchEvent(&up);

        updateStateOvered();
    }

    void Actor::dispatchEvent(Event* event)
    {
        if (event->type == TouchEvent::MOVE)
        {
            TouchEvent* te = safeCast<TouchEvent*>(event);
            if (!_overred)
            {
                _overred = te->index;
                updateStateOvered();

                TouchEvent over = *te;
                over.type = TouchEvent::OVER;
                over.bubbles = false;
                dispatchEvent(&over);

                _getStage()->addEventListener(TouchEvent::MOVE, EventCallback(this, &Actor::_onGlobalTouchMoveEvent));
            }
        }

        if (event->type == TouchEvent::TOUCH_DOWN)
        {
            TouchEvent* te = safeCast<TouchEvent*>(event);
            if (!_pressedButton[te->mouseButton])
            {
                if (_pressedOvered == _overred)//!_pressed[0] && !_pressed[1] && !_pressed[2])
                    _getStage()->addEventListener(TouchEvent::TOUCH_UP, EventCallback(this, &Actor::_onGlobalTouchUpEvent));

                _pressedButton[te->mouseButton] = te->index;
                updateStatePressed();
            }
        }

        TouchEvent click(0);

        if (event->type == TouchEvent::TOUCH_UP)
        {
            TouchEvent* te = safeCast<TouchEvent*>(event);
            if (_pressedButton[te->mouseButton] == te->index && !te->__clickDispatched)
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
            if (_parent)
            {
                if (TouchEvent::isTouchEvent(event->type))
                {
                    TouchEvent* me = safeCast<TouchEvent*>(event);
                    me->localPosition = local2parent(me->localPosition);
                }

                event->phase = Event::phase_bubbling;
                event->currentTarget = 0;
                _parent->dispatchEvent(event);
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
            if (!(_flags & flag_visible))
            {
                return;
            }
            if (getAlpha() == 0 && !(_flags & flag_clickableWithZeroAlpha))
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
            me->__localScale *= _transform.a;
            if (me->__localScale == NAN)
            {
                Q_ASSERT(0);
            }
        }

        event->phase = Event::phase_capturing;
        spActor actor = _children._last;
        while (actor)
        {
            spActor prev = actor->_prev;
            if (!touchEvent || (_flags & flag_touchChildrenEnabled))
            {
                actor->handleEvent(event);
            }
            //if (event->target)
            //  break;
            actor = prev;
        }

        if (touchEvent)
        {
            TouchEvent* me = safeCast<TouchEvent*>(event);
            if (!event->target)
            {
                if ((_flags & flag_touchEnabled) && isOn(me->localPosition, me->__localScale))
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
        _flags |= flag_transformDirty | flag_transformInvertDirty;
    }

    void Actor::setAnchor(const Vector2& anchor)
    {
        _anchor = anchor;
        _flags &= ~flag_anchorInPixels;
        markTranformDirty();
    }

    void Actor::setAnchor(float ax, float ay)
    {
        setAnchor(Vector2(ax, ay));
    }

    void Actor::setAnchorInPixels(const Vector2& anchor)
    {
        _anchor = anchor;
        _flags |= flag_anchorInPixels;
        markTranformDirty();
    }

    void Actor::setAnchorInPixels(float x, float y)
    {
        setAnchorInPixels(Vector2(x, y));
    }

    void Actor::setPosition(const Vector2& pos)
    {
        if (_pos == pos)
        {
            return;
        }
        _pos = pos;
        markTranformDirty();
    }

    void Actor::setPosition(float x, float y)
    {
        setPosition(Vector2(x, y));
    }

    void Actor::setX(float x)
    {
        if (_pos.x == x)
        {
            return;
        }
        _pos.x = x;
        markTranformDirty();
    }

    void Actor::setY(float y)
    {
        if (_pos.y == y)
        {
            return;
        }
        _pos.y = y;
        markTranformDirty();
    }

    void Actor::setAnchorX(float x)
    {
        if (_anchor.x == x)
        {
            return;
        }
        _anchor.x = x;
        _flags &= ~flag_anchorInPixels;
        markTranformDirty();
    }

    void Actor::setAnchorY(float y)
    {
        if (_anchor.y == y)
        {
            return;
        }
        _anchor.y = y;
        _flags &= ~flag_anchorInPixels;
        markTranformDirty();
    }

    void Actor::setTransform(const AffineTransform& tr)
    {
        _transform = tr;
        _flags &= ~flag_transformDirty;
        _flags &= ~flag_fastTransform;
        _flags |= flag_transformInvertDirty;
    }

    void Actor::setPriority(qint32 zorder)
    {
        if (_zOrder == zorder) // fixed by Evgeniy Golovin
        {
            return;
        }

        _zOrder = zorder;
        if (_parent)
        {
            Actor* parent = _parent;

            spActor me = this;

            parent->_children.remove(me);

            Actor* sibling = parent->_children._last.get();

            //try to insert at the end of list first
            if (sibling && sibling->getPriority() > _zOrder)
            {
                sibling = sibling->intr_list::_prev.get();
                while (sibling)
                {
                    if (sibling->getPriority() <= _zOrder)
                        break;
                    sibling = sibling->intr_list::_prev.get();
                }
            }

            if (sibling)
            {
                spActor s = sibling;
                parent->_children.insert_after(me, s);
            }
            else
            {
                parent->_children.prepend(me);
            }
        }
    }

    void Actor::setScale(float scale)
    {
        setScale(Vector2(scale, scale));
    }

    void Actor::setScale(const Vector2& scale)
    {
        if (_scale == scale)
        {
            return;
        }
        _scale = scale;
        markTranformDirty();
        _flags &= ~flag_fastTransform;
    }

    void Actor::setScale(float scaleX, float scaleY)
    {
        setScale(Vector2(scaleX, scaleY));
    }

    void Actor::setScaleX(float sx)
    {
        if (_scale.x == sx)
            return;
        _scale.x = sx;
        markTranformDirty();
        _flags &= ~flag_fastTransform;
    }

    void Actor::setScaleY(float sy)
    {
        if (_scale.y == sy)
        {
            return;
        }
        _scale.y = sy;
        markTranformDirty();
        _flags &= ~flag_fastTransform;
    }

    void Actor::setRotation(float rotation)
    {
        if (_rotation == rotation)
        {
            return;
        }
        _rotation = rotation;
        markTranformDirty();
        _flags &= ~flag_fastTransform;
    }

    void Actor::setRotationDegrees(float degr)
    {
        float rad = degr * M_PI / 180.0f;
        setRotation(rad);
    }

    void Actor::sizeChanged(const Vector2&)
    {

    }

    void Actor::_setSize(const Vector2& size)
    {
        if (_size == size)
        {
            return;
        }
        _size = size;
        markTranformDirty();
    }

    void Actor::setSize(const Vector2& size)
    {
        _setSize(size);
        sizeChanged(size);
    }

    void Actor::setSize(float w, float h)
    {
        setSize(Vector2(w, h));
    }

    void Actor::setWidth(float w)
    {
        setSize(Vector2(w, _size.y));
    }

    void Actor::setHeight(float h)
    {
        setSize(Vector2(_size.x, h));
    }

    void Actor::setClock(spClock clock)
    {
        _clock = clock;
    }

    void Actor::setAlpha(unsigned char alpha)
    {
        _alpha = alpha;
    }

    void Actor::setRenderDelegate(RenderDelegate* mat)
    {
        _rdelegate = mat;
    }

    const Transform& Actor::getTransform() const
    {
        updateTransform();
        return _transform;
    }

    const Transform& Actor::getTransformInvert() const
    {
        if (_flags & flag_transformInvertDirty)
        {
            _flags &= ~flag_transformInvertDirty;
            _transformInvert = getTransform();
            _transformInvert.invert();
        }

        return _transformInvert;
    }

    float Actor::getWidth() const
    {
        return _size.x;
    }

    float Actor::getHeight() const
    {
        return _size.y;
    }

    unsigned char Actor::getAlpha() const
    {
        return _alpha;
    }

    const spClock&  Actor::getClock() const
    {
        return _clock;
    }

    void Actor::updateTransform() const
    {
        if (!(_flags & flag_transformDirty))
            return;

        AffineTransform tr;

        if (_flags & flag_fastTransform)
        {
            tr = AffineTransform(1, 0, 0, 1, _pos.x, _pos.y);
        }
        else
        {
            float c = 1.0f;
            float s = 0.0f;
            if (_rotation)
            {
                c = qCos(_rotation);
                s = qSin(_rotation);
            }

            tr = AffineTransform(
                     c * _scale.x, s * _scale.x,
                     -s * _scale.y, c * _scale.y,
                     _pos.x, _pos.y);
        }

        Vector2 offset;
        if (_flags & flag_anchorInPixels)
        {
            offset.x = -_anchor.x;
            offset.y = -_anchor.y;
        }
        else
        {
            offset.x = -float(_size.x * _anchor.x);
            offset.y = -float(_size.y * _anchor.y);//todo, what to do? (per pixel quality)
        }

        tr.translate(offset);


        _transform = tr;
        _flags &= ~flag_transformDirty;

        const_cast<Actor*>(this)->transformUpdated();
    }

    bool Actor::isOn(const Vector2& localPosition, float)
    {
        RectF r = getDestRect();
        r.expand(Vector2(_extendedIsOn, _extendedIsOn), Vector2(_extendedIsOn, _extendedIsOn));

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
                return true;
            act = act->getParent();
        }
        return false;
    }

    Actor* Actor::getDescendant(QString name, error_policy ep)
    {
        if (isName(name))
            return this;

        Actor* actor = _getDescendant(name);
        if (!actor)
        {
            handleErrorPolicy(ep, "can't find descendant: " + name);
        }
        return actor;
    }

    Actor* Actor::_getDescendant(QString name)
    {
        Actor* child = _children._first.get();
        while (child)
        {
            if (child->isName(name))
                return child;

            child = child->getNextSibling().get();
        }

        child = _children._first.get();
        while (child)
        {
            Actor* des = child->_getDescendant(name);
            if (des)
                return des;

            child = child->getNextSibling().get();
        }

        return nullptr;
    }

    spActor  Actor::getChild(QString name, error_policy ep) const
    {
        spActor actor = _children._first;
        while (actor)
        {
            if (actor->isName(name))
                return actor;
            actor = actor->_next;
        }

        handleErrorPolicy(ep, "can't find child: " + name);

        return nullptr;
    }

    void Actor::setParent(Actor* actor, Actor* parent)
    {
        actor->_parent = parent;
        if (parent && parent->_getStage())
        {
            actor->added2stage(parent->_getStage());
        }
        else
        {
            if (actor->_getStage())
            {
                actor->removedFromStage();
            }
        }
    }

    void Actor::insertSiblingBefore(spActor actor)
    {
        Q_ASSERT(actor != this);
        Q_ASSERT(actor);
        Q_ASSERT(_parent);
        if (!_parent)
        {
            return;
        }
        actor->detach();
        spActor t = this;
        _parent->_children.insert_before(actor, t);
        setParent(actor.get(), _parent);
    }

    void Actor::insertSiblingAfter(spActor actor)
    {
        Q_ASSERT(actor != this);
        Q_ASSERT(actor);
        Q_ASSERT(_parent);
        if (!_parent)
        {
            return;
        }
        actor->detach();
        spActor t = this;
        _parent->_children.insert_after(actor, t);
        setParent(actor.get(), _parent);
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

        int z = actor->getPriority();

        spActor sibling = _children._last;

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
            _children.prepend(t);
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
            Q_ASSERT(actor->_parent == this);
            if (actor->_parent == this)
            {
                m_Locked.lock();
                setParent(actor.get(), nullptr);
                _children.remove(actor);
                m_Locked.unlock();
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
        m_Locked.lock();
        spTween tween = _tweens._first;
        while (tween)
        {
            spTween tweenNext = tween->getNextSibling();

            if (tween->getParentList())
            {
                tween->update(*this, us);
            }
            if (tween->isDone() && tween->getParentList())
            {
                _tweens.remove(tween);
            }
            tween = tweenNext;
        }
        doUpdate(us);

        spActor actor = _children._first;
        while (actor)
        {
            spActor next = actor->_next;
            if (actor->getParent())
            {
                actor->update(us);
            }
            if (!next)
            {
                //Q_ASSERT(actor == _children._last);
            }
            actor = next;
        }
        m_Locked.unlock();
    }

    void Actor::update(const UpdateState& parentUS)
    {
        UpdateState us = parentUS;
        if (_clock)
        {
            us.iteration = 0;
            _clock->update();

            timeMS dt = _clock->doTick();
            while (dt > timeMS(0))
            {
                us.dt = dt;
                us.time = _clock->getTime();

                internalUpdate(us);

                dt = _clock->doTick();
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
        if (!(_flags & flag_visible))
        {
            return false;
        }
        unsigned char alpha = (parentRS.alpha * _alpha) / 255;
        if (!alpha)
        {
            return false;
        }
        rs = parentRS;
        rs.alpha = alpha;


        const Transform& tr = getTransform();
        if (_flags & flag_fastTransform)
        {
            rs.transform = parentRS.transform;
            rs.transform.translate(Vector2(tr.x, tr.y));
        }
        else
        {
            Transform::multiply(rs.transform, tr, parentRS.transform);
        }

        if (_flags & flag_cull)
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
        if (_flags & flag_actorHasBounds)
        {
            bounds = getDestRect();
            return true;
        }
        return false;
    }

    void Actor::render(const RenderState& parentRS)
    {
        m_Locked.lock();
        _rdelegate->render(this, parentRS);
        m_Locked.unlock();
    }

    RectF Actor::getDestRect() const
    {
        return RectF(Vector2(0, 0), getSize());
    }

    spTween Actor::_addTween(spTween tween, bool)
    {
        Q_ASSERT(tween);
        if (!tween)
        {
            return nullptr;
        }
        tween->start(*this);
        _tweens.append(tween);

        return tween;
    }

    spTween Actor::addTween(spTween tween)
    {
        return _addTween(tween, false);
    }

    spTween Actor::addTween2(spTween tween, const TweenOptions& opt)
    {
        tween->init2(opt);
        return _addTween(tween, false);
    }

    spTween Actor::getTween(QString name, error_policy ep)
    {
        spTween tween = _tweens._first;
        while (tween)
        {
            if (tween->isName(name))
            {
                return tween;
            }
            tween = tween->getNextSibling();
        }
        handleErrorPolicy(ep, "can't find tween: " + name);
        return nullptr;
    }

    void Actor::removeTween(spTween v)
    {
        m_Locked.lock();
        Q_ASSERT(v);
        if (!v)
        {
            return;
        }

        if (v->getParentList() == &_tweens)
        {
            v->setClient(nullptr);
            _tweens.remove(v);
        }
        m_Locked.unlock();
    }

    void Actor::removeTweens(bool callComplete)
    {
        spTween t = _tweens._first;
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

    void Actor::removeTweensByName(QString name)
    {
        spTween t = _tweens._first;
        while (t)
        {
            spTween c = t;
            t = t->getNextSibling();

            if (c->isName(name))
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

    spTween setTimeout(timeMS dur, const EventCallback& cb, Actor* root)
    {
        if (!root)
        {
            root = getStage().get();
        }
        dur = timeMS(std::max((long long) dur.count(), 1ll));
        spTween t = root->addTween(TweenDummy(), dur);
        t->setDoneCallback(cb);
        return t;
    }

    spTween setTimeout(timeMS dur, const EventCallback& cb, spActor root)
    {
        return setTimeout(dur, cb, root.get());
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

    void decompose(const Transform& t, Vector2& pos, float& angle, Vector2& scale)
    {
        scale.x = qSqrt(t.a * t.a + t.c * t.c);
        scale.y = qSqrt(t.b * t.b + t.d * t.d);

        angle = -atan2(t.c, t.a);
        pos.x = t.x;
        pos.y = t.y;
    }

    void setDecomposedTransform(Actor* actor, const Transform& t)
    {
        Vector2 pos;
        Vector2 scale;
        float angle;

        decompose(t, pos, angle, scale);
        actor->setPosition(pos);
        actor->setRotation(angle);
        actor->setScale(scale);
    }

    void    reattachActor(spActor actor, spActor newParent, spActor root)
    {
        Transform t1 = actor->computeGlobalTransform(root.get());
        Transform t2 = newParent->computeGlobalTransform(root.get());
        t2.invert();
        Transform r = t1 * t2;

        Vector2 pos;
        Vector2 scale;
        float angle;

        decompose(r, pos, angle, scale);
        actor->attachTo(newParent);
        actor->setPosition(pos);
        actor->setRotation(angle);
        actor->setScale(scale);
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

    extern int HIT_TEST_DOWNSCALE;

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
            return false;

        /*
        float s1 = objB->getSize().x * objB->getSize().y;
        float s2 = objA->getSize().x * objA->getSize().y;
        bool swapped = false;
        if (s2 < s1)
        {
            swapped = true;
            std::swap(objA, objB);
            std::swap(transA, transB);
            n = transA * transB;
        }

        */


        int w = (int)objA->getWidth();
        int h = (int)objA->getHeight();


        for (int y = 0; y < h; y += HIT_TEST_DOWNSCALE)
        {
            for (int x = 0; x < w; x += HIT_TEST_DOWNSCALE)
            {
                Vector2 posA = Vector2(float(x), float(y));

                if (!objA->isOn(posA))
                    continue;

                Vector2 posB = n.transform(posA);

                if (!objB->isOn(posB))
                    continue;

                if (contact)
                    *contact = swapped ? posB : posA;
                return true;
            }
        }

        return false;
    }
}
