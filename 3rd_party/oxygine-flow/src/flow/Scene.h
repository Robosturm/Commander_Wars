#pragma once
#include "oxygine/EventDispatcher.h"
#include "oxygine/actor/Actor.h"
#include "oxygine/actor/Stage.h"
#include <functional>

namespace oxygine
{
    class Action
    {
    public:
        std::string id;
    };

    namespace flow
    {
        class FlowEvent : public Event
        {
        public:
            FlowEvent(): Event(0) {}
            std::string action;
        };

        class Flow;
        typedef EventCallback resultCallback;

        DECLARE_SMART(Scene, spScene);
        DECLARE_SMART(Transition, spTransition);


        class Scene: public EventDispatcher
        {
        public:

            enum
            {
                //fired before entering to scene
                EVENT_PRE_ENTERING = makefourcc('S', 'P', 'E', 'n'),

                //fired after entering to scene
                EVENT_POST_ENTERING = makefourcc('S', 'P', 'S', 'E'),

                //fired before leaving scene
                EVENT_PRE_LEAVING = makefourcc('S', 'P', 'r', 'L'),

                //fired after leaving scene
                EVENT_POST_LEAVING = makefourcc('S', 'P', 'o', 'L'),

                //current scene will be shown now with transition
                EVENT_PRE_SHOWING =     makefourcc('S', 'P', 'r', 'S'),

                //current scene was shown, transition is done
                EVENT_POST_SHOWING =    makefourcc('S', 'P', 'o', 'S'),

                //current scene will be hidden with transition
                EVENT_PRE_HIDING =      makefourcc('S', 'P', 'r', 'H'),

                //current scene was hidden with transition
                EVENT_POST_HIDING =     makefourcc('S', 'P', 'o', 'H'),

                //fired when other scene was shown on top of current,
                EVENT_SCENE_SHOWN =     makefourcc('S', 'S', 'S', 'h'),

                //fired when other scene was hidden on top of current and current become active
                EVENT_SCENE_HIDDEN =    makefourcc('S', 'S', 'H', 'd'),

                //back button was pressed (android)
                EVENT_BACK =            makefourcc('S', 'B', 'a', 'c'),

                //fired from destructor
                EVENT_DESTROY =         makefourcc('S', 'D', 'e', 's'),

                EVENT_PRESHOWING = EVENT_PRE_SHOWING,
                EVENT_POSTSHOWING = EVENT_POST_SHOWING,
                EVENT_PREHIDING = EVENT_PRE_HIDING,
                EVENT_POSTHIDING = EVENT_POST_HIDING,
            };

            Scene();
            ~Scene();

            /*closes current scene**/
            void finish(Event* ev = 0);
            void finishNoResult();
            void noResult();

            /**
            Automatically removes Scene from stack (flow).
            Can't be used for dialogs.
            resultCallback won't be called for such scene.
            */
            void remove();

            void finishOnClick(spActor);

            /**
            adds TouchEvent::CLICK event listener to actor and calls callback
            if "finish" is true scene will be finished.
            */
            void addClickHandler(spActor, const EventCallback& cb, bool finish = false);

            /**
            adds Back (Android) event listener calls callback
            if "finish" is true scene will be finished.
            */
            void addBackHandler(const EventCallback& cb, bool finish = false);

            /**returns finish method as callback for simplified usage from client code*/
            EventCallback   getFinish();

            void setTransitionIn(spTransition t);
            void setTransitionOut(spTransition t);
            void setTransition(spTransition tin, spTransition tout);
            void setAllowDialogsOnTop(bool allow) { _allowDialogsOnTop = allow; }
            void removeTransitions() { _transitionIn = _transitionOut = 0; }

            spActor         getHolder() const { return _holder; }
            const FlowEvent& getFinishEvent() const { return _finishEvent; }
            bool            isDialog() const { return _dialog; }
            bool            isInStackWide() const { return _instackWide; }
            bool            isVisibleWide() const { return _visibleWide; }
            bool            isFinished() const { return _done; }

            void setPassBlockedTouch(bool v) { _passBlockedTouch = v; }

        protected:

            friend class Flow;
            friend class Transition;

            virtual void _finish(Event*);
            virtual void update() {}

            /**dialog mode*/
            bool _dialog;
            bool _allowDialogsOnTop;
            bool _passBlockedTouch;

            std::function< bool(spScene) > _checkIsThisSceneAllowesDialog;//called as: scene->_checkIsDialogAllowed(dialog)
            std::function< bool(spScene) > _checkIsThisDialogAllowedOnScene;//called  as: dialog->_checkIsDiallogAllowed(scene)


            spTransition runTransition(Flow*, spScene current, bool back);
            virtual spTransition _runTransitionIn(Flow*, spScene current);
            virtual spTransition _runTransitionOut(Flow*, spScene current);

            spActor _holder;
            bool _instack;    //true in [post_entering, pre_leaving]
            bool _instackWide;//true in [pre_entering, post_leaving]
            bool _visible;    //true in [post_showing, pre_hiding]
            bool _visibleWide;//true in [pre_showing, post_hiding]

            void _flowPreLeaving();
            void _flowPostLeaving();

        private:

            spTransition _transitionIn;
            spTransition _transitionOut;

            bool _remove;
            bool _done;


        public:
            void preShowing();
            void postShowing();
            void preHiding();
            void postHiding();
            void preEntering();
            void postEntering();
            void preLeaving();
            void postLeaving();

            void sceneShown(spScene);
            void sceneHidden(spScene);

            FlowEvent _finishEvent;
            resultCallback _resultCB;

        private:

        };
    }

    typedef flow::Scene Frame;
}
