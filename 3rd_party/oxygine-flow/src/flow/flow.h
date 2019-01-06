#pragma once
#include "oxygine/core/Object.h"
#include <functional>
#include "Scene.h"
#include "Transition.h"

namespace oxygine
{
    class Event;

    namespace flow
    {
        DECLARE_SMART(Scene, spScene);
        DECLARE_SMART(Transition, spTransition);
        class Flow;

        inline void doNothingEventCallback(Event*) {}

        /**initializing oxygine-flow subsystem*/
        void init();

        /**releasing oxygine-flow subsystem*/
        void free();

        /**update oxygine-flow. Call it each frame*/
        void update();

        /**shows next scene and call callback when it was finished*/
        void show(spScene, const resultCallback& cb = doNothingEventCallback, Flow* = 0);

        Flow& get();


        class Flow
        {
        public:
            static Flow instance;

            Flow();
            ~Flow();

            void init();
            void free();

            void show(spScene scene, const resultCallback& cb);
            void update();

            void phaseBegin(spScene current, spScene next, bool back);
            void phaseEnd();

            void checkDone();
            void checkShow();

            bool isTransitionPhase() const { return _transition; }

            void blockedTouch(Event*);

            bool hasSceneInStack(spScene) const;
            void removeFromStack(spScene);

            void lockBlockedTouches();
            void unlockBlockedTouches();

            void finishDialogs();

            std::list<spScene> scenes;
            std::list<spScene> scenes2show;

            bool _transition;
            bool _back;
            bool _locked;

            spTransition _trans;
            bool _transitionDone;
            spScene _current;
            spScene _next;


            bool _secondary;

            spActor _touchBlocker;

            Vector2 _blockedTouchPosition;
            bool _wasTouchBlocked;
            bool _wasBackBlocked;
            timeMS _tm;
        };
    }
}