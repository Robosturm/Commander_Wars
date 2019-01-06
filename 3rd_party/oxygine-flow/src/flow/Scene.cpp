#include "Scene.h"
#include "Transition.h"
#include "ox/Clock.hpp"
#include "ox/MaterialCache.hpp"
#include "flow.h"

#ifdef __S3E__
#include "s3eKeyboard.h"
#include "s3eDevice.h"
#else
#include "SDL.h"
#endif

#define LOGD(...) logs::messageln("flow::" __VA_ARGS__)
//#define LOGD(...) {}
namespace oxygine
{
    bool checkQuit()
    {
        bool back = false;
#ifdef __S3E__
        if (s3eDeviceCheckQuitRequest() ||
                (s3eKeyboardGetState(s3eKeyBackspace) & S3E_KEY_STATE_DOWN) ||
                (s3eKeyboardGetState(s3eKeyAbsBSK) & S3E_KEY_STATE_DOWN))
        {
            back = true;
        }
#elif EMSCRIPTEN

#else
        const Uint8* data = SDL_GetKeyboardState(0);

        if (key::wasPressed(SDL_SCANCODE_AC_BACK))
            return true;

        if (key::wasPressed(SDL_SCANCODE_ESCAPE))
            return true;

        if (key::wasPressed(SDL_SCANCODE_BACKSPACE))
            return true;
#endif
        return back;
    }

    namespace flow
    {
        extern spTransition _defaultTransition;

        Scene::Scene(): _done(false), _remove(false), _dialog(false), _instack(false), _instackWide(false), _visible(false), _visibleWide(false), _allowDialogsOnTop(true), _passBlockedTouch(true)
        {
            setName("Scene");
            _holder = new Actor;
            //if (Stage::instance)
            _holder->setSize(Stage::instance->getSize());
            _holder->setName("Scene::_holder");

            _checkIsThisSceneAllowesDialog = [](spScene)
            {
                return true;
            };
            _checkIsThisDialogAllowedOnScene = [](spScene)
            {
                return true;
            };

            _transitionIn = _defaultTransition;
            _transitionOut = _defaultTransition;
        }

        Scene::~Scene()
        {
        }

        void Scene::setTransitionIn(spTransition t)
        {
            _transitionIn = t;
        }

        void Scene::setTransitionOut(spTransition t)
        {
            _transitionOut = t;
        }

        void Scene::setTransition(spTransition tin, spTransition tout)
        {
            setTransitionIn(tin);
            setTransitionOut(tout);
        }

        void Scene::listenForAllSceneEvents(const EventCallback &cb)
        {
            size_t events[] = { 
                EVENT_PRE_ENTERING, EVENT_POST_ENTERING, EVENT_PRE_LEAVING, EVENT_POST_LEAVING, 
                EVENT_PRE_SHOWING, EVENT_POST_SHOWING, EVENT_PRE_HIDING, EVENT_POST_HIDING,
                EVENT_SCENE_SHOWN, EVENT_SCENE_HIDDEN, EVENT_BACK };

            for (size_t i = 0; i < sizeof(events) / sizeof(events[0]); ++i)            
                addEventListener(events[i], cb);
        }

        void Scene::finishOnClick(spActor actor)
        {
            actor->addClickListener(getFinish());
        }

        void Scene::addClickHandler(spActor actor, const EventCallback& cb, bool finish)
        {
            actor->addClickListener(cb);
            if (finish)
                finishOnClick(actor);
        }

        void Scene::addBackHandler(const EventCallback& cb, bool finish)
        {
            addEventListener(EVENT_BACK, cb);
            if (finish)
                addEventListener(EVENT_BACK, getFinish());
        }

        EventCallback   Scene::getFinish()
        {
            return CLOSURE(this, &Scene::finish);
        }

        spTransition Scene::runTransition(Flow* f, spScene current, bool back)
        {
            return back ? _runTransitionOut(f, current) : _runTransitionIn(f, current);
        }

        spTransition Scene::_runTransitionIn(Flow* f, spScene current)
        {
            spTransition t = _transitionIn;
            if (!t)
                return 0;
            t->run(f, current, this, false);
            return t;
        }

        spTransition Scene::_runTransitionOut(Flow* f, spScene current)
        {
            spTransition t = current->_transitionOut;
            if (!t)
                return 0;
            t->run(f, current, this, true);
            return t;
        }

        void Scene::_flowPreLeaving()
        {
            preLeaving();
            preHiding();
        }

        void Scene::_flowPostLeaving()
        {
            postHiding();
            postLeaving();
        }

        void Scene::preEntering()
        {
            LOGD("%-20s '%s'", "preEntering", getName().c_str());
            _instackWide = true;
            Event ev2(EVENT_PRE_ENTERING);
            dispatchEvent(&ev2);
        }

        void Scene::postEntering()
        {
            LOGD("%-20s '%s'", "postEntering", getName().c_str());
            _instack = true;
            Event ev2(EVENT_POST_ENTERING);
            dispatchEvent(&ev2);
        }

        void Scene::preLeaving()
        {
            LOGD("%-20s '%s'", "preLeaving", getName().c_str());

            _instack = false;
            Event ev(EVENT_PRE_LEAVING);
            dispatchEvent(&ev);
        }

        void Scene::postLeaving()
        {
            LOGD("%-20s '%s'", "postLeaving", getName().c_str());
            _instackWide = false;
            Event ev2(EVENT_POST_LEAVING);
            dispatchEvent(&ev2);

            mc().removeUnused();
        }

        void Scene::sceneShown(spScene s)
        {
            LOGD("%-20s '%s' - '%s'", "sceneShown on ", getName().c_str(), s->getName().c_str());
            Event ev(EVENT_SCENE_SHOWN);
            dispatchEvent(&ev);
        }

        void Scene::sceneHidden(spScene s)
        {
            LOGD("%-20s '%s' - '%s'", "sceneHidden on ", getName().c_str(), s->getName().c_str());
            Event ev(EVENT_SCENE_HIDDEN);
            dispatchEvent(&ev);
        }

        void Scene::preShowing()
        {
            _done = false;
            _remove = false;
            //_visible = false;
            _visibleWide = true;


            update();

            LOGD("%-20s '%s'", "preShowing", getName().c_str());
            Event ev(EVENT_PRE_SHOWING);
            dispatchEvent(&ev);
        }

        void Scene::postShowing()
        {
            LOGD("%-20s '%s'", "postShowing", getName().c_str());
            _visible = true;
            Event ev(EVENT_POST_SHOWING);
            dispatchEvent(&ev);
        }

        void Scene::preHiding()
        {
            LOGD("%-20s '%s'", "preHiding", getName().c_str());
            _visible = false;
            Event ev(EVENT_PRE_HIDING);
            dispatchEvent(&ev);
        }

        void Scene::postHiding()
        {
            LOGD("%-20s '%s'", "postHiding", getName().c_str());
            _visibleWide = false;
            Event ev(EVENT_POST_HIDING);
            dispatchEvent(&ev);
        }

        void Scene::_finish(Event* ev)
        {
            _done = true;
        }

        void Scene::finish(Event* ev)
        {
            _finish(ev);
            if (_done)
            {
                if (ev)
                {
                    static_cast<Event&>(_finishEvent) = *ev;
                    if (_finishEvent.target.get() == this)
                        _finishEvent.target = getStage();
                    if (_finishEvent.currentTarget.get() == this)
                        _finishEvent.currentTarget = getStage();
                }
                else
                {
                    _finishEvent = FlowEvent();
                    _finishEvent.target = _finishEvent.currentTarget = getStage();
                }
            }

            //Flow::instance.checkDone();
        }

        void Scene::finishNoResult()
        {
            _done = true;
            _resultCB = EventCallback();
        }

        void Scene::noResult()
        {
            _resultCB = EventCallback();
        }

        void Scene::remove()
        {
            LOGD("%-20s '%s'", "remove", getName().c_str());
            _resultCB = resultCallback();
            OX_ASSERT(_dialog == false);
            if (_visibleWide)
            {
                _remove = true;
            }
            else
            {

                //_flowPreLeaving();
                //_flowPostLeaving();
                preLeaving();
                postLeaving();

                flow::get().removeFromStack(this);
            }
        }
    }
}