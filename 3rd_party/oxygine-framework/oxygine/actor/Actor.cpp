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
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
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
        if (m_stage != nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::added2stage trying to add to stage while on a stage");
        }
        m_stage = stage;
        for (auto & child : m_children)
        {
            child->added2stage(stage);
        }
        onAdded2Stage();
    }

    void Actor::removedFromStage()
    {
        if (m_stage == nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::removedFromStage trying to remove from stage while not on a stage");
            return;
        }
        onRemovedFromStage();
        m_stage->removeEventListeners(this);
        m_stage = nullptr;

        m_pressedOvered = 0;
        for (auto & child : m_children)
        {
            child->removedFromStage();
        }
    }

    void Actor::transformUpdated()
    {
    }

    void Actor::calcChildrenBounds(RectF& bounds, const Transform& transform) const
    {
        for (auto & child : m_children)
        {
            if (child->getVisible())
            {
                Transform tr = child->getTransform() * transform;
                child->calcBounds2(bounds, tr);
            }
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
        up.localPosition = stage2local(te->localPosition, oxygine::Stage::getStage().get());
        dispatchEvent(&up);
    }

    void Actor::_onGlobalTouchMoveEvent(Event* ev)
    {
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
        updateStateOvered();
        oxygine::Stage::getStage()->removeEventListener(m_onGlobalTouchMoveEvent);
        m_overred = 0;
        m_onGlobalTouchMoveEvent = -1;
    }

    void Actor::dispatchEvent(Event* event)
    {
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
                    updateStateOvered();

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
            dispatchToParent(event);

            if (click.type)
            {
                //send click event at the end after TOUCH_UP event
                dispatchEvent(&click);
            }
        }
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
                oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::handleEvent locale scale is NAN");
            }
        }

        event->phase = Event::phase_capturing;
        if (!touchEvent || (m_flags & flag_touchChildrenEnabled))
        {
            auto iter = m_children.end();
            while (iter != m_children.begin())
            {
                iter--;
                iter->get()->handleEvent(event);
            }
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
        m_pos.x = static_cast<qint32>(pos.x);
        m_pos.y = static_cast<qint32>(pos.y);
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
        m_pos.x = static_cast<qint32>(x);
        markTranformDirty();
    }

    void Actor::setY(float y)
    {
        if (m_pos.y == y)
        {
            return;
        }
        m_pos.y = static_cast<qint32>(y);
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
            QMutexLocker lockParent(&m_parent->m_Locked);
            QMutexLocker lock(&m_Locked);
            spActor me = spActor(this);
            m_parent->m_children.removeOne(me);
            m_parent->insertActor(me);
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

    void Actor::insertActor(spActor actor)
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
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::addChild trying to add actor from wrong thread");
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
        QMutexLocker lock(&m_Locked);
        QMutexLocker lockActor(&(actor->m_Locked));
        insertActor(actor);
        setParent(actor.get(), this);
    }

    void Actor::removeChild(spActor actor)
    {
        QMutexLocker lock(&m_Locked);
        QMutexLocker lockActor(&(actor->m_Locked));
        if (!GameWindow::getWindow()->isWorker())
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
                m_children.removeOne(actor);
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
            spActor child = m_children.last();
            removeChild(child);
        }
    }


    Actor* Actor::detach()
    {
        Actor* parent = getParent();
        if (parent)
        {
            parent->removeChild(spActor(this));
        }
        return parent;
    }

    void Actor::internalUpdate(const UpdateState& us)
    {
        QMutexLocker lock(&m_Locked);
        for (auto & tween : m_tweens)
        {
            tween->update(*this, us);
            if (tween->isDone())
            {
                m_tweens.removeOne(tween);
            }
        }
        doUpdate(us);
        for (auto & child : m_children)
        {
            child->update(us);
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
        float width = oxygine::Stage::getStage()->getScaledWidth();
        float height = oxygine::Stage::getStage()->getScaledHeight();
        auto scaledWidth = m_size.x * rs.transform.a;
        auto scaledHeight = m_size.y * rs.transform.d;
        if (rs.transform.x > width ||
            rs.transform.y > height ||
            rs.transform.x + scaledWidth < 0 ||
            rs.transform.y + scaledHeight < 0)
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
        rs.transform.x = floorf(rs.transform.x);
        rs.transform.y = floorf(rs.transform.y);
        m_onScreen = onScreen(rs);
        if (m_onScreen)
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

        for (auto & child : m_children)
        {
            child->clean();
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
            m_tweens.append(tween);
        }
        tween->start(*this);
        return tween;
    }

    spTween Actor::addTween(spTween tween)
    {
        return __addTween(tween, false);
    }

    void Actor::removeTween(spTween pTween)
    {
        QMutexLocker lock(&m_Locked);
        if (pTween.get() == nullptr)
        {
            return;
        }
        if (m_tweens.contains(pTween))
        {
            pTween->setClient(nullptr);
            m_tweens.removeOne(pTween);
        }
    }

    void Actor::removeTweens(bool callComplete)
    {
        while (m_tweens.size() > 0)
        {
            spTween tween = m_tweens.last();
            if (callComplete)
            {
                tween->complete();
            }
            else
            {
                removeTween(tween);
            }
        }
    }

    Vector2 Actor::convert_global2local_(const Actor* child, const Actor* parent, Vector2 pos)
    {
        if (child->getParent() && child->getParent() != parent)
        {
            pos = convert_global2local_(child->getParent(), parent, pos);
        }
        pos = child->parent2local(pos);
        return pos;
    }

    Vector2 Actor::convert_global2local(spActor child, spActor parent, const Vector2& pos)
    {
        return convert_global2local_(child.get(), parent.get(), pos);
    }

    Vector2 Actor::convert_local2global_(const Actor* child, const Actor* parent, Vector2 pos)
    {
        while (child && child != parent)
        {
            pos = child->local2parent(pos);
            child = child->getParent();
        }

        return pos;
    }

    Vector2 Actor::convert_local2global(spActor child, spActor parent, const Vector2& pos)
    {
        return convert_local2global_(child.get(), parent.get(), pos);
    }

    Vector2 Actor::convert_local2stage(spActor actor, const Vector2& pos, spActor root)
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

    Vector2 Actor::convert_stage2local(spActor actor, const Vector2& pos, spActor root)
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

    RectF Actor::getActorTransformedDestRect(Actor* actor, const Transform& tr)
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
