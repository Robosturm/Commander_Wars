#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/tween/Tween.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include <QtMath>

namespace oxygine
{
#ifndef GRAPHICSUPPORT
    QPoint Actor::m_dummyPoint;
    QSize Actor::m_dummySize;
    QPointF Actor::m_dummyPointF;
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

#ifdef GRAPHICSUPPORT
    QTransform Actor::computeGlobalTransform(Actor* parent) const
    {
        QTransform t;
        const Actor* actor = this;
        while (actor && actor != parent)
        {
            t = t * actor->getTransform();
            actor = actor->getParent();
        }
        return t;
    }
#endif

    void Actor::setAnchor(float ax, float ay)
    {
#ifdef GRAPHICSUPPORT
        m_anchor = QPointF(ax, ay);
        markTranformDirty();
#endif
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
        if (isDescendant(safeSpCast<Actor>(ev->target).get()))
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
                event->currentTarget.reset();
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

        QPoint originalLocalPos;
        float originalLocalScale = 1.0f;

        if (touchEvent)
        {
#ifdef GRAPHICSUPPORT
            TouchEvent* me = safeCast<TouchEvent*>(event);
            originalLocalPos = me->localPosition;
            originalLocalScale = me->__localScale;
            me->localPosition = parent2local(originalLocalPos);
            me->__localScale *= m_transform.m11();
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
                if (isOn(me->localPosition))
                {
                    event->phase = Event::phase_target;
                    event->target = getSharedPtr<EventDispatcher>();

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

    void Actor::setPosition(const QPoint& pos)
    {
#ifdef GRAPHICSUPPORT
        if (m_pos == pos)
        {
            return;
        }
        m_pos = pos;
        markTranformDirty();
#endif
    }

    void Actor::setPosition(qint32 x, qint32 y)
    {
        setPosition(QPoint(x, y));
    }

    void Actor::setX(qint32 x)
    {
#ifdef GRAPHICSUPPORT
        if (m_pos.x() == x)
        {
            return;
        }
        m_pos.setX(x);
        markTranformDirty();
#endif
    }

    void Actor::setY(qint32 y)
    {
#ifdef GRAPHICSUPPORT
        if (m_pos.y() == y)
        {
            return;
        }
        m_pos.setY(y);
        markTranformDirty();
#endif
    }

    void Actor::setTransform(const QTransform& tr)
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
            spActor me = getSharedPtr<Actor>();
            auto iter = m_parent->m_children.cbegin();
            while (iter != m_parent->m_children.cend())
            {
                if (iter->get() == me.get())
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

    void Actor::setScale(qreal scale)
    {
        setScale(QPointF(scale, scale));
    }

    void Actor::setScale(const QPointF& scale)
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

    void Actor::setScale(qreal scaleX, qreal scaleY)
    {
        setScale(QPointF(scaleX, scaleY));
    }

    void Actor::setScaleX(qreal sx)
    {
#ifdef GRAPHICSUPPORT
        if (m_scale.x() == sx)
        {
            return;
        }
        m_scale.setX(sx);
        markTranformDirty();
        m_flags &= ~flag_fastTransform;
#endif
    }

    void Actor::setScaleY(qreal sy)
    {
#ifdef GRAPHICSUPPORT
        if (m_scale.y() == sy)
        {
            return;
        }
        m_scale.setY(sy);
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

    void Actor::sizeChanged(const QSize&)
    {
    }

    void Actor::__setSize(const QSize& size)
    {
#ifdef GRAPHICSUPPORT
        if (m_size == size)
        {
            return;
        }
        m_size = size;
        markTranformDirty();
#endif
    }

    void Actor::setSize(const QSize& size)
    {
        __setSize(size);
        sizeChanged(size);
    }

    void Actor::setSize(qint32 w, qint32 h)
    {
        setSize(QSize(w, h));
    }

    void Actor::setWidth(qint32 w)
    {
#ifdef GRAPHICSUPPORT
        setSize(QSize(w, m_size.height()));
#endif
    }

    void Actor::setHeight(qint32 h)
    {
#ifdef GRAPHICSUPPORT
        setSize(QSize(m_size.width(), h));
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
    const QTransform& Actor::getTransform() const
    {
        updateTransform();
        return m_transform;
    }

    const QTransform& Actor::getTransformInvert() const
    {
        if (m_flags & flag_transformInvertDirty)
        {
            m_flags &= ~flag_transformInvertDirty;
            m_transformInvert = getTransform().inverted();
        }

        return m_transformInvert;
    }
#endif

    qint32 Actor::getWidth() const
    {
#ifdef GRAPHICSUPPORT
        return m_size.width();
#else
        return 0.0f;
#endif
    }

    qint32 Actor::getHeight() const
    {
#ifdef GRAPHICSUPPORT
        return m_size.height();
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
        QTransform tr;

        if (m_flags & flag_fastTransform)
        {
            tr = QTransform(1, 0, 0, 1, m_pos.x(), m_pos.y());
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

            tr = QTransform(
                     c * m_scale.x(), s * m_scale.x(),
                     -s * m_scale.y(), c * m_scale.y(),
                     m_pos.x(), m_pos.y());
        }
        QPoint offset(-m_size.width() * m_anchor.x(),
                      -m_size.height() * m_anchor.y());
        tr.translate(offset.x(), offset.y());
        m_transform = tr;
        m_flags &= ~flag_transformDirty;
#endif
    }

    bool Actor::isOn(const QPoint& localPosition)
    {
        if (getDestRect().contains(localPosition))
        {
             return true;
        }
        return false;
    }

    bool Actor::isDescendant(const Actor* actor) const
    {
        const Actor* act = actor;
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
                    if (iter->get() == actor.get())
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

    QPoint Actor::parent2local(const QPoint& global) const
    {
#ifdef GRAPHICSUPPORT
        const QTransform& t = getTransformInvert();
        return t.map(global);
#else
        return QPoint();
#endif
    }

    QPoint Actor::local2parent(const QPoint& local) const
    {
#ifdef GRAPHICSUPPORT
        const QTransform& t = getTransform();
        return t.map(local);
#else
        return QPoint();
#endif
    }

    QPoint Actor::local2stage(const QPoint& pos, Actor* stage) const
    {
        return convert_local2stage(this, pos, stage);
    }

    QPoint Actor::stage2local(const QPoint& pos, Actor* stage) const
    {
        return convert_stage2local(this, pos, stage);
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


        const QTransform& tr = getTransform();
        if (m_flags & flag_fastTransform)
        {
            rs.transform = parentRS.transform;
            rs.transform.translate(tr.m31(), tr.m32());
        }
        else
        {
            rs.transform = tr * parentRS.transform;
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
        doRender(rs);
#endif
        return true;
    }

    bool Actor::getBounds(QRect& bounds) const
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

    QRect Actor::getDestRect() const
    {
        return QRect(0, 0, getWidth(), getHeight());
    }

    spTween Actor::__addTween(spTween tween, bool)
    {
#ifdef GRAPHICSUPPORT
        if (tween.get() == nullptr)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "Actor::__addTween tween is nullptr");
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
            if (iter->get() == pTween.get())
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

    QPoint Actor::convert_global2local_(const Actor* child, const Actor* parent, QPoint pos)
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

    QPoint Actor::convert_global2local(spActor & child, spActor & parent, const QPoint& pos)
    {
        return convert_global2local_(child.get(), parent.get(), pos);
    }

    QPoint Actor::convert_local2global_(const Actor* child, const Actor* parent, QPoint pos)
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

    QPoint Actor::convert_local2global(spActor & child, spActor & parent, const QPoint& pos)
    {
        return convert_local2global_(child.get(), parent.get(), pos);
    }

#ifdef GRAPHICSUPPORT
    QMutex* Actor::getLocked()
    {
        return &m_Locked;
    }
#endif

    QPoint Actor::convert_local2stage(spActor & actor, const QPoint& pos, spActor root)
    {
        if (!root)
        {
            root = Stage::getStage();
        }
        return convert_local2global(actor, root, pos);
    }

    QPoint Actor::convert_local2stage(const Actor* actor, const QPoint& pos, const Actor* root)
    {
        if (!root)
        {
            root = Stage::getStage().get();
        }
        return convert_local2global_(actor, root, pos);
    }

    QPoint Actor::convert_stage2local(spActor & actor, const QPoint& pos, spActor root)
    {
        if (!root)
        {
            root = Stage::getStage();
        }
        return convert_global2local(actor, root, pos);
    }

    QPoint Actor::convert_stage2local(const Actor* actor, const QPoint& pos, const Actor* root)
    {
        if (!root)
        {
            root = Stage::getStage().get();
        }
        return convert_global2local_(actor, root, pos);
    }

    QRect Actor::getActorTransformedDestRect(Actor* actor, const QTransform& tr)
    {
        QRect rect = actor->getDestRect();
        QPoint tl(rect.x(), rect.y());
        QPoint br(rect.x() + rect.width(), rect.y() + rect.height());

        tl = tr.map(tl);
        br = tr.map(br);
        QSize size(qAbs(br.x() - tl.x()),
                   qAbs(br.y() - tl.y()));

        QPoint ntl(std::min(tl.x(), br.x()),
                   std::min(tl.y(), br.y()));

        return QRect(ntl, size);
    }
}
