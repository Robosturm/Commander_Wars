#include "flow.h"
#include "Scene.h"
#include "Transition.h"
#include "oxygine/actor/DebugActor.h"
#include "oxygine/core/oxygine.h"

#define LOGD(...) logs::messageln("flow::" __VA_ARGS__)

namespace oxygine
{
    bool checkQuit();

    namespace flow
    {
        int BLOCK_TOUCH_DURATION = 300;

        spActor _touchBlocker;

        Vector2 _blockedTouchPosition(0, 0);
        bool _wasTouchBlocked;
        bool _wasBackBlocked;
        timeMS _tm = 0;


        Flow Flow::instance;


        spTransition _defaultTransition;


        void init()
        {
            _tm = 0;
            _wasTouchBlocked = false;
            _wasBackBlocked = false;
            _touchBlocker = new Actor;
            _touchBlocker->setName("Scene::_touchBlocker");
            _touchBlocker->setPosition(-10000, -10000);
            _touchBlocker->setSize(20000, 20000);
            _touchBlocker->setName("touchBlocker");

            _defaultTransition = new TransitionFade;

            Flow::instance = Flow();
            Flow::instance.init();
        }

        void free()
        {
            if (_touchBlocker)
                _touchBlocker->detach();
            _touchBlocker = 0;
            _defaultTransition = 0;
            Flow::instance.free();
        }


        Flow::Flow()
        {
            _transition = false;
            _transitionDone = false;
            _back = false;
            _locked = false;
        }

        Flow::~Flow()
        {

        }

        void Flow::init()
        {
        }

        void Flow::free()
        {
            _trans = 0;
            _current = 0;
            _next = 0;
            scenes.clear();
            scenes2show.clear();
        }

        void Flow::show(spScene scene, const resultCallback& cb)
        {
            if (scenes.empty())
            {
                scenes.push_back(scene);
                scene->preEntering();
                scene->preShowing();
                scene->_holder->attachTo(getStage());
                scene->postShowing();
                scene->postEntering();
                scene->_resultCB = cb;
                return;
            }

            auto p = std::find(scenes.begin(), scenes.end(), scene);
            if (p != scenes.end())
            {
                logs::error("you can't show scene '%s', it is already in the list", scene->getName().c_str());
                OX_ASSERT(p == scenes.end());
                return;
            }

            scene->_resultCB = cb;
            scenes2show.push_back(scene);

            checkShow();
        }

        void Flow::blockedTouch(Event* ev)
        {
            if (_locked)
                return;

            if (_tm + BLOCK_TOUCH_DURATION > getTimeMS())
                return;

            TouchEvent* event = safeCast<TouchEvent*>(ev);
            _blockedTouchPosition = event->getPointer()->getPosition();
            _wasTouchBlocked = true;
            _wasBackBlocked = false;
        }

        bool Flow::hasSceneInStack(spScene scene) const
        {
            return std::find(scenes.begin(), scenes.end(), scene) != scenes.end();
        }

        void Flow::removeFromStack(spScene scene)
        {
            //OX_ASSERT(scene->_inloop == false);
            auto it = std::find(scenes.begin(), scenes.end(), scene);
            if (it != scenes.end())
            {
                scenes.erase(it);
            }

            it = std::find(scenes2show.begin(), scenes2show.end(), scene);
            if (it != scenes2show.end())
            {
                scenes2show.erase(it);
            }
        }

        void Flow::lockBlockedTouches()
        {
            _locked = true;
        }

        void Flow::unlockBlockedTouches()
        {
            _locked = false;
        }


        void Flow::finishDialogs()
        {
            for (auto it = scenes.rbegin(); it != scenes.rend(); ++it)
            {
                spScene scene = *it;
                if (scene->isDialog())
                    scene->finishNoResult();
            }
        }

        void Flow::phaseBegin(spScene current, spScene next, bool back)
        {
            _back = back;
            _current = current;
            _next = next;

            _transition = true;


            if (_back)
            {
                if (!current->_dialog)
                    next->preShowing();
            }
            else
            {
                next->preEntering();
                next->preShowing();
            }


            if (!next->_dialog || _back)
            {
                if (_current->_done || _current->_remove)
                    current->preLeaving();
                current->preHiding();
            }


            if (!(back || current->_remove))
                current->sceneShown(next);

            _trans = next->runTransition(this, current, back);

			_touchBlocker->setPriority(10000);
            getStage()->addChild(_touchBlocker);

            _wasTouchBlocked = false;
            _wasBackBlocked = false;
            _tm = getTimeMS();
            getStage()->addEventListener(TouchEvent::CLICK, CLOSURE(this, &Flow::blockedTouch));
        }

        void Flow::phaseEnd()
        {
            _transition = false;

            spScene current = _current;
            spScene next = _next;

            _current = 0;
            _next = 0;

            if (!next->_dialog || _back)
            {
                current->_holder->detach();
                current->postHiding();
            }

			_touchBlocker->setPriority(next->getHolder()->getPriority());
            next->getHolder()->insertSiblingBefore(_touchBlocker);

            if (_back)
            {
                if (!current->_dialog)
                    next->postShowing();
            }
            else
            {
                next->postShowing();
                next->postEntering();
            }


            getStage()->removeEventListener(TouchEvent::CLICK, CLOSURE(this, &Flow::blockedTouch));

            if (current->_done)
            {
                current->postLeaving();
                if (current->_resultCB)
                {
                    current->_resultCB(&current->_finishEvent);
                    current->_resultCB = resultCallback();
                    current->_finishEvent = FlowEvent();
                }
            }

            if (_back)
                next->sceneHidden(current);


            if (current->_remove)
            {
                current->postLeaving();
                OX_ASSERT(next->_dialog == false);
                std::list<spScene>::iterator i = std::find(scenes.begin(), scenes.end(), current);
                OX_ASSERT(i != scenes.end());
                scenes.erase(i);

                /*
                for (auto it = scenes.begin(); it != scenes.end(); )
                {
                    if ((*it)->_remove)
                        scenes.erase(it);
                    else
                        it++;
                }
                */
            }


            if (_wasTouchBlocked)
            {
                if (scenes2show.empty())
                {
                    if (current->_passBlockedTouch && !_locked)
                    {
                        LOGD("send  blocked touch");
                        TouchEvent click(TouchEvent::CLICK, true, _blockedTouchPosition);
                        getStage()->handleEvent(&click);
                    }
                }
                _wasTouchBlocked = false;
            }
        }

        void Flow::checkDone()
        {
            spScene current = scenes.back();
            if (current->_done)
            {
                scenes.pop_back();
                if (scenes.empty())
                {
                    current->_resultCB = resultCallback();
                    current->_finishEvent = FlowEvent();
                }
                else
                {
                    spScene prev = scenes.back();
                    phaseBegin(current, prev, true);
                }
            }
        }

        void Flow::checkShow()
        {
            if (scenes2show.empty())
                return;

            if (scenes.back()->_done)
                return;

            if (_transition)
                return;

            spScene current = scenes.back();


            std::list<spScene>::iterator it = scenes2show.begin();

            if (!current->_allowDialogsOnTop)
            {
                for (; it != scenes2show.end(); ++it)
                {
                    if ((*it)->_dialog)
                        continue;
                    break;
                }
            }

            for (; it != scenes2show.end(); ++it)
            {
                spScene dialog = *it;
                if (dialog->_dialog && (!current->_checkIsThisSceneAllowesDialog(dialog) || !dialog->_checkIsThisDialogAllowedOnScene(current)))
                    continue;
                break;
            }

            if (it == scenes2show.end())
                return;

            spScene next = *it;


            if (current->_dialog)
            {
                OX_ASSERT(next->_dialog && "you can't show fullscreen scene on top of dialog");
                OX_ASSERT(!current->_remove && "you can't remove dialog from flow");
                if (!next->_dialog)
                    return;
            }

            scenes2show.erase(it);

            scenes.push_back(next);
            phaseBegin(current, next, false);

        }

        void Flow::update()
        {
            if (scenes.empty() && scenes2show.empty())
                return;

            if (DebugActor::instance)
            {
                std::string str;
                for (auto it = scenes.begin(); it != scenes.end(); ++it)
                {
                    str += (*it)->getName();
                    str += "-> ";
                }
                if (!str.empty())
                {
                    str.pop_back();
                    str.pop_back();
                    str.pop_back();
                    DebugActor::addDebugString(str.c_str());
                }
            }

            bool quit = checkQuit();
            static bool quitLast = false;
            if (quit && !quitLast)
            {
                _wasBackBlocked = true;
                _wasTouchBlocked = false;
            }
            quitLast = quit;

            if (_transition)
            {
                //we don't know who controls transition, next, current, or Transition
                //if (_trans && _trans->_done || _next->_transitionDone || _current->_transitionDone)
                if (_transitionDone)
                    phaseEnd();

                return;
            }

            checkDone();
            checkShow();

            if (_transition)
                return;

            if (!scenes.empty())
            {
                spScene current = scenes.back();

                if (_wasBackBlocked)
                {
                    _wasBackBlocked = false;
                    if (!_locked)
                    {
                        LOGD("send  blocked BACK");

                        Event ev(Scene::EVENT_BACK);
                        current->dispatchEvent(&ev);
                        checkDone();
                    }
                }
                else
                {
                    current->update();
                }
            }

            if (scenes.empty())
                core::requestQuit();
        }

        void update()
        {
            Flow::instance.update();
        }

        void show(spScene s, const resultCallback& cb, Flow* f)
        {
            if (!f)
                f = &Flow::instance;

            f->show(s, cb);
        }

        Flow& get()
        {
            return Flow::instance;
        }
    }
}
