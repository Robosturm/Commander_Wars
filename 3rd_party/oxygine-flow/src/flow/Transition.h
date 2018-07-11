#pragma once
#include "oxygine/core/Object.h"
#include "oxygine/actor/Stage.h"
#include "Scene.h"
#include "oxygine/actor/ColorRectSprite.h"
#include "oxygine/STDRenderer.h"

namespace oxygine
{
    DECLARE_SMART(MaskedSprite, spMaskedSprite);

    namespace flow
    {
        class Flow;

        DECLARE_SMART(Transition, spTransition);
        class Transition : public Object
        {
        public:

            static void assign(Scene* scene);

            Transition();

            void run(Flow* f, spScene current, spScene next, bool back);

            virtual void _attach(spScene current, spScene next, bool back);
            virtual void _run(spScene current, spScene next, bool back) {}
            virtual void _clear() {}

            void waitTween(spTween);
            TweenOptions getTweenOptions() const { return _tweenOpt; }
            void         setTweenOptions(const TweenOptions& opt) { _tweenOpt = opt; }

            Flow* _flow;
            bool _singleDirection;

        protected:
            TweenOptions _tweenOpt;
        };

        class TransitionSimple : public Transition
        {
        public:
            static void assign(Scene* scene);

        protected:
            void _run(spScene current, spScene next, bool back) override;
        };

        DECLARE_SMART(TransitionMove, spTransitionMove);
        class TransitionMove : public Transition
        {
        public:
            static void assign(Scene* scene);
            /*
            the same instance of TransitionMove should be assigned to transitionIn and transitionOut
            example:
            Scene::_transitionIn = new flow::TransitionMove;
            Scene::_transitionOut = _transitionIn;
            */
            TransitionMove();
            void setSrcPosition(const Vector2& v) { _src = v; }

        protected:
            void _run(spScene current, spScene next, bool back) override;
            spColorRectSprite _fade;
            Vector2 _src;
        };

        class TransitionFade : public Transition
        {
        public:
            static void assign(Scene* scene);

        protected:
            void _run(spScene current, spScene next, bool back) override;
        };


        class TransitionShutters : public Transition
        {
        public:
            TransitionShutters();

            static void assign(Scene* scene);

        protected:
            void _run(spScene current, spScene next, bool back) override;
            void _attach(spScene current, spScene next, bool back) override;
            void _clear() override;

            spColorRectSprite left;
            spColorRectSprite right;
        };

        class TransitionQuads : public Transition
        {
        public:
            TransitionQuads();
            ~TransitionQuads();

            static void assign(Scene* scene);

        private:
            void clicked(Event*);
            Vector2 _center;

            STDRenderer _r;
            spNativeTexture _mask;
            spActor _holder;
            spMaskedSprite _bg;

            void _run(spScene current, spScene next, bool back) override;
            void _clear() override;

            void update(const UpdateState& us);
        };
    }
}