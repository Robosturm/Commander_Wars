#pragma once
#include <QTransform>
#include <QMutex>

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/tween/Tween.h"
#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"
#include "3rd_party/oxygine-framework/oxygine/TouchEvent.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"
#include "3rd_party/oxygine-framework/oxygine/Property.h"
#include <vector>

namespace oxygine
{

    class TweenOptions final
    {
    public:
        explicit TweenOptions(timeMS duration = timeMS(500))
            : m_duration(duration),
              m_delay(timeMS(0)),
              m_ease(QEasingCurve::Linear),
              m_globalEase(QEasingCurve::Linear),
              m_loops(1),
              m_twoSides(false),
              m_detach(false)
        {
        }
        ~TweenOptions() = default;
        TweenOptions& duration(timeMS duration)
        {
            m_duration = duration;
            return *this;
        }
        TweenOptions& delay(timeMS delay)
        {
            m_delay = delay;
            return *this;
        }
        TweenOptions& loops(qint32 loops)
        {
            m_loops = loops;
            return *this;
        }
        TweenOptions& twoSides(bool enabled = true)
        {
            m_twoSides = enabled;
            return *this;
        }
        TweenOptions& ease(QEasingCurve::Type ease)
        {
            m_ease = ease;
            return *this;
        }
        TweenOptions& detach(bool detach_ = true)
        {
            m_detach = detach_;
            return *this;
        }
        TweenOptions& globalEase(QEasingCurve::Type ease)
        {
            m_globalEase = ease;
            return *this;
        }
        TweenOptions& doneCallback(const EventCallback& cb)
        {
            m_callback = cb;
            return *this;
        }

        EventCallback   m_callback;
        timeMS          m_duration;
        timeMS          m_delay;
        QEasingCurve::Type     m_ease;
        QEasingCurve::Type     m_globalEase;
        qint32          m_loops;
        bool            m_twoSides;
        bool            m_detach;
    };

    class Actor;
    using spActor = std::shared_ptr<Actor>;
    using spWeakActor = std::weak_ptr<Actor>;
    class Actor: public EventDispatcher
    {
    public:
        explicit Actor();
        virtual ~Actor();
        using children = std::vector<spActor>;
        using tweens = std::vector<spTween>;

        children & getChildren()
        {
            OXY_ASSERT(!requiresThreadChange());
            return m_children;
        }
        oxygine::spActor getFirstChild()
        {
            OXY_ASSERT(!requiresThreadChange());
            return m_children.front();
        }
        oxygine::spActor getLastChild()
        {
            OXY_ASSERT(!requiresThreadChange());
            return m_children.back();
        }

        const QPoint& getPosition() const
        {
#ifdef GRAPHICSUPPORT
            return m_pos;
#else
            return m_dummyPoint;
#endif
        }
        qint32 getX() const
        {
#ifdef GRAPHICSUPPORT
            return m_pos.x();
#else
            return 0.0f;
#endif
        }
        qint32 getY() const
        {
#ifdef GRAPHICSUPPORT
            return m_pos.y();
#else
            return 0.0f;
#endif
        }
        const QPointF& getScale() const
        {
#ifdef GRAPHICSUPPORT
            return m_scale;
#else
            return m_dummyPointF;
#endif
        }
        qreal getScaleX() const
        {
#ifdef GRAPHICSUPPORT
            return m_scale.x();
#else
            return 0.0;
#endif
        }
        qreal getScaleY() const
        {
#ifdef GRAPHICSUPPORT
            return m_scale.y();
#else
            return 0.0;
#endif
        }
        /**Returns rotation angle in radians*/
        float getRotation() const
        {
#ifdef GRAPHICSUPPORT
            return m_rotation;
#else
            return 0;
#endif
        }
        /**Returns rotation angle in degrees*/
        float getRotationDegrees() const
        {
#ifdef GRAPHICSUPPORT
            return m_rotation / M_PI * 180.0f;
#else
            return 0.0f;
#endif
        }
        qint32 getPriority() const
        {
#ifdef GRAPHICSUPPORT
            return m_zOrder;
#else
            return 0;
#endif
        }
        virtual bool getVisible() const
        {
#ifdef GRAPHICSUPPORT
            return (m_flags & flag_visible) != 0;
#else
            return false;
#endif
        }
        spWeakEventDispatcher getParent()
        {
            return m_parent;
        }
        spActor getSpParent()
        {
            return oxygine::safeSpCast<oxygine::Actor>(m_parent.lock());
        }

        const QSize& getSize() const
        {
#ifdef GRAPHICSUPPORT
            return m_size;
#else
            return m_dummySize;
#endif
        }
        /**Returns Size*Scale*/
        QSize getScaledSize() const
        {
#ifdef GRAPHICSUPPORT
            return QSize(getScaledWidth(), getScaledHeight());
#else
            return m_dummySize;
#endif
        }
        qint32 getWidth() const;
        qint32 getScaledWidth() const
        {
#ifdef GRAPHICSUPPORT
            return m_size.width() * m_scale.x();
#else
            return 0;
#endif
        }
        qint32 getHeight() const;
        qint32 getScaledHeight() const
        {
#ifdef GRAPHICSUPPORT
            return m_size.height() * m_scale.y();
#else
            return 0;
#endif
        }
        unsigned char getAlpha() const;
        const spClock& getClock() const;
        virtual QRect getDestRect() const;

#ifdef GRAPHICSUPPORT
        /**return local actor transformation*/
        const QTransform& getTransform() const;
        const QTransform& getTransformInvert() const;
        /**computes global actor transformation*/
        QTransform computeGlobalTransform(spActor parent = spActor());
#endif

        void setPosition(const QPoint& pos);
        void setPosition(qint32 x, qint32 y);
        void setX(qint32 x);
        void setY(qint32 y);
        void setAnchor(float ax, float ay);

        /**Overwrites transformation matrix. position/scale/rotation would be ignored until you change them*/
        void setTransform(const QTransform& tr);
        /** set z order draw priority, from back (low value) to front (high value). Max value is 32000, Min value -32000*/
        void setPriority(qint32 zorder);
        void setScale(qreal scale);
        void setScale(const QPointF& scale);
        void setScale(qreal scaleX, qreal scaleY);
        void setScaleX(qreal sx);
        void setScaleY(qreal sy);
        /**Sets rotation angle in radians*/
        void setRotation(float angle);
        /**Sets rotation angle in degrees. Converts internally to radians. (use setRotation!)*/
        void setRotationDegrees(float angle);

        /**Sets Size of Actor. Size doesn't scale contents of Actor. Size only affects event handling and rendering if you change Anchor*/
        void setSize(const QSize&);
        void setSize(qint32 w, qint32 h);
        virtual void setWidth(qint32 w);
        virtual void setHeight(qint32 h);

        void setClock(spClock & clock);

        void restartAllTweens();
        void syncAllTweens(oxygine::timeMS syncTime);

        /**Show/Hide actor and children. Invisible Actor doesn't receive Touch events.*/
        virtual void setVisible(bool vis)
        {
#ifdef GRAPHICSUPPORT
            m_flags &= ~flag_visible;
            if (vis)
            {
                m_flags |= flag_visible;
            }
#endif
        }

        /**Sets transparency. if alpha is 0 actor and children are completely invisible. Invisible Actor doesn't receive Touch events.*/
        void setAlpha(unsigned char alpha);

        /**By default Actor doesn't has bounds, this will set it to Actor::getDestRect*/
        void setHasOwnBounds(bool enable = true)
        {
#ifdef GRAPHICSUPPORT
            m_flags &= ~flag_actorHasBounds;
            if (enable)
            {
                m_flags |= flag_actorHasBounds;
            }
#endif
        }
        /**by default actor with Alpha = 0 not clickable*/
        void setClickableWithZeroAlpha(bool enable)
        {
#ifdef GRAPHICSUPPORT
            m_flags &= ~flag_clickableWithZeroAlpha;
            if (enable)
            {
                m_flags |= flag_clickableWithZeroAlpha;
            }
#endif
        }
        virtual bool isOn(const QPoint& localPosition);
        /**Returns true if actor is child or located deeper in current subtree*/
        bool isDescendant(spActor actor) const;
        void addChild(spActor actor);
        /**Remove one child*/
        void removeChild(spActor actor);
        /**Removes all children from Actor*/
        void removeChildren();
        /**detaches actor from parent and returns parent. return NULL If actor doesn't have parent*/
        void detach();
        /**Dispatches an event into the event flow. The event target is the EventDispatcher object upon which the dispatchEvent() method is called.*/
        virtual void dispatchEvent(Event* event) override;

        void addTween(spTween);
        template<class TProperty>
        void addTween(const TProperty& property, timeMS duration, qint32 loops = 1, bool twoSides = false, timeMS delay = timeMS(0), QEasingCurve::Type ease = QEasingCurve::Linear)
        {
            addTween(createTween(property, duration, loops, twoSides, delay, ease));
        }
        /**short syntax version of actor->addEventListener(TouchEvent::CLICK, ...);*/
        void addClickListener(const EventCallback& cb)
        {
            addEventListener(TouchEvent::CLICK, cb);
        }
        /**short syntax version of actor->addEventListener(TouchEvent::TOUCH_DOWN, ...);*/
        void addTouchDownListener(const EventCallback& cb)
        {
            addEventListener(TouchEvent::TOUCH_DOWN, cb);
        }
        /**short syntax version of actor->addEventListener(TouchEvent::TOUCH_UP, ...);*/
        void addTouchUpListener(const EventCallback& cb)
        {
            addEventListener(TouchEvent::TOUCH_UP, cb);
        }

        void removeTween(spTween tween);
        void removeTweens();

        /**Updates this actor, children and all tweens.*/
        virtual void update(const UpdateState& us);
        /**Renders this actor and children.*/
        virtual void render(const RenderState& rs);
        void handleEvent(Event* event);
        virtual void doRender(const RenderState&) {}

        //converts position in parent space to local space
        QPoint parent2local(const QPoint& pos) const;
        //converts local position to parent space
        QPoint local2parent(const QPoint& pos = QPoint()) const;
        //converts local position to Stage
        QPoint local2stage(const QPoint& pos = QPoint(), spActor stage = spActor());
        //converts global position (position in Stage space) to local space
        QPoint stage2local(const QPoint& pos = QPoint(), spActor stage = spActor());

        /**Returns Stage where Actor attached to. Used for multi stage (window) mode*/
        spWeakStage getStage() const;

        void setNotPressed(MouseButton b);

        bool internalRender(RenderState& rs, const RenderState& parentRS);
        virtual bool getBounds(QRect&) const;
        static QPoint convert_local2stage(spActor & child, const QPoint& pos, spActor root = spActor());
        static QPoint convert_stage2local(spActor & child, const QPoint& pos, spActor root = spActor());
        static QRect getActorTransformedDestRect(Actor* actor, const QTransform& tr);
        /*****************************************************************************************/
        // properties for tweens
        /*****************************************************************************************/
        using TweenPosition = Property2Args<qint32, QPoint, const QPoint&, Actor, &Actor::getPosition, &Actor::setPosition>;
        using TweenX = Property<qint32, qint32, Actor, &Actor::getX, &Actor::setX>;
        using TweenY = Property<qint32, qint32, Actor, &Actor::getY, &Actor::setY>;
        using TweenWidth = Property<qint32, qint32, Actor, &Actor::getWidth, &Actor::setWidth>;
        using TweenHeight = Property<qint32, qint32, Actor, &Actor::getHeight, &Actor::setHeight>;
        using TweenSize = Property2Args2<qint32, QSize, const QSize&, const QSize&, Actor, &Actor::getSize, &Actor::setSize>;
        using TweenRotation = Property<float, float, Actor, &Actor::getRotation, &Actor::setRotation>;
        using TweenRotationDegrees = Property<float, float, Actor, &Actor::getRotationDegrees, &Actor::setRotationDegrees>;
        using TweenScale = Property2Args1Arg<float, QPointF, const QPointF&, Actor, &Actor::getScale, &Actor::setScale>;
        using TweenScaleX = Property<qreal, qreal, Actor, &Actor::getScaleX, &Actor::setScaleX>;
        using TweenScaleY = Property<qreal, qreal, Actor, &Actor::getScaleY, &Actor::setScaleY>;
        using TweenAlpha = Property<unsigned char, unsigned char, Actor, &Actor::getAlpha, &Actor::setAlpha>;

    protected:
        virtual void handleEventImpl(Event* event);
        void added2stage(spStage);
        virtual void onAdded2Stage(){}
        void removedFromStage();
        static void setParent(spActor actor, spActor parent);
        void _onGlobalTouchUpEvent(Event*);
        void _onGlobalTouchMoveEvent(Event*);
        void __setSize(const QSize&);
        virtual void sizeChanged(const QSize& size);
        bool prepareRender(RenderState& rs, const RenderState& parentRS);
        void markTranformDirty();
        void updateTransform() const;
        void internalUpdate(const UpdateState& us);
        /**doUpdate is virtual method for overloading in inherited classes. UpdateState struct has local time of Actor (relative to Clock) and delta time.*/
        virtual void doUpdate(const UpdateState& us);
        void dispatchToParent(Event* event);
        void insertActor(spActor & actor);
        static QPoint convert_global2local(spActor & child, spActor & parent, QPoint pos);
        static QPoint convert_local2global(spActor & child, spActor & parent, QPoint pos);
    protected:
#ifdef GRAPHICSUPPORT
        enum flags
        {
            flag_visible                = 1 << 1,
            flag_transformDirty         = 1 << 2,
            flag_transformInvertDirty   = 1 << 3,
            flag_fastTransform          = 1 << 5,
            flag_actorHasBounds         = 1 << 7,
            flag_clickableWithZeroAlpha = 1 << 8,
            flag_reserved               = 1 << 9,
            flag_last                   = flag_reserved
        };
        mutable quint32 m_flags{flag_visible | flag_fastTransform};
        mutable QTransform m_transform;
        mutable QTransform m_transformInvert;
        tweens m_tweens;
#else
        static QPoint m_dummyPoint;
        static QSize m_dummySize;
        static QPointF m_dummyPointF;
#endif
        spWeakStage m_stage;
        spClock m_clock;
        spWeakEventDispatcher m_parent;
        children m_children;
        union
        {
            struct
            {
                pointer_index m_overred;
                pointer_index m_pressedButton[MouseButton_Num];
            };
            int32_t m_pressedOvered;
        };
    private:
#ifdef GRAPHICSUPPORT
        unsigned char   m_alpha{255};
        QPoint m_pos;
        QPointF m_scale{1.0f, 1.0f};
        QSize m_size;
        QPointF m_anchor;
        float  m_rotation{0};
        qint32 m_zOrder{0};
        qint32 m_onGlobalTouchUpEvent{-1};
        qint32 m_onGlobalTouchMoveEvent{-1};
#endif
    };

}
