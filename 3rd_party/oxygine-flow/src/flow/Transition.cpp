#include "Transition.h"
#include "oxygine/RenderState.h"
#include "oxygine/actor/MaskedSprite.h"
#include "oxygine/core/oxygine.h"
#include "flow.h"
//#include "oxygine/STDMaterial.h"
#include "oxygine/tween/TweenAlphaFade.h"

namespace oxygine
{

    namespace flow
    {

		bool _TRANSITION_ADD_CHILD = false;

        void Transition::assign(Scene* scene)
        {
            spTransition t = new Transition;
            scene->setTransitionIn(t);
            scene->setTransitionOut(t);
        }

        Transition::Transition() : _singleDirection(false), _flow(0), _tweenOpt(500)
        {

        }

        void Transition::run(Flow* f, spScene current, spScene next, bool back)
        {
            _flow = f;
            //_current = current;
            //_next = next;
            //_done = false;

            _attach(current, next, back);
            _run(current, next, back);
        }

        void Transition::waitTween(spTween t)
        {
            t->setDoneCallback([ = ](Event*)
            {
                //_done = true;
                _clear();
                //_current = 0;
                //_next = 0;
                _flow->phaseEnd();
            });
        }

        void Transition::_attach(spScene current, spScene next, bool back)
        {
            if (current->_dialog)
            {
                if (!back)
                    current->_holder->insertSiblingAfter(next->_holder);
            }
            else
            {
                if (back)
                    current->_holder->insertSiblingBefore(next->_holder);
                else
				{ 
					if (_TRANSITION_ADD_CHILD)
						current->_holder->_getStage()->addChild(next->_holder);
					else
						current->_holder->insertSiblingAfter(next->_holder);
				}
                    
            }
        }

        void TransitionSimple::assign(Scene* scene)
        {
            spTransition t = new TransitionSimple;
            scene->setTransitionIn(t);
            scene->setTransitionOut(t);
        }

        void TransitionSimple::_run(spScene current, spScene next, bool back)
        {
            spScene target = back ? current : next;
            spActor holder = target->getHolder();
            spTween tween = holder->addTween(TweenDummy(), 1);
            waitTween(tween);
        }

        void TransitionMove::assign(Scene* scene)
        {
            spTransition t = new TransitionMove();
            scene->setTransitionIn(t);
            scene->setTransitionOut(t);
        }

        TransitionMove::TransitionMove()
        {
            _fade = new ColorRectSprite;
            _fade->setPosition(-10000, -10000);
            _fade->setSize(Vector2(30000, 30000));
            _fade->setColor(Color(0, 0, 0, 128));

            _src = Vector2(0.0f, -getStage()->getHeight());
            _tweenOpt._ease = Tween::ease_inOutBack;
        }

        void TransitionMove::_run(spScene current, spScene next, bool back)
        {
            Vector2 src = _src;
            Vector2 dest = Vector2(0.0f, 0.0f);

            spScene target = back ? current : next;

            spActor holder = target->getHolder();
            if (back)
            {
                std::swap(src, dest);

                spTween t = _fade->addTween(Actor::TweenAlpha(0), _tweenOpt._duration);
                t->detachWhenDone();
            }
            else
            {
                _fade->setAlpha(0);
                _fade->addTween(Actor::TweenAlpha(255), _tweenOpt._duration);
                holder->insertSiblingBefore(_fade);
            }


            holder->setPosition(src);
            spTween tween = holder->addTween(Actor::TweenPosition(dest), _tweenOpt);
            waitTween(tween);
        }

        void TransitionFade::assign(Scene* scene)
        {
            spTransition t = new TransitionFade;
            scene->setTransitionIn(t);
            scene->setTransitionOut(t);
        }

        void TransitionFade::_run(spScene current, spScene next, bool back)
        {
            spScene target = back ? current : next;

            Color cl = Color(0, 0, 0, target->isDialog() ? 0 : 255);
            spTween tween = target->getHolder()->addTween(TweenAlphaFade(!back, PostProcessOptions().fullscreen().clear(cl)), _tweenOpt._duration, 1, false, 0);
            waitTween(tween);
        }

        TransitionShutters::TransitionShutters()
        {
            left = new ColorRectSprite;
            left->setWidth(getStage()->getWidth() / 2);
            left->setHeight(getStage()->getHeight());

            right = new ColorRectSprite;
            right->setWidth(getStage()->getWidth() / 2);
            right->setHeight(getStage()->getHeight());
        }

        void TransitionShutters::_attach(spScene current, spScene next, bool back)
        {

        }

        void TransitionShutters::_run(spScene current, spScene next, bool back)
        {
            current->getHolder()->insertSiblingAfter(left);
            current->getHolder()->insertSiblingAfter(right);

            int dur = _tweenOpt._duration;
            left->setX(-left->getWidth());
            right->setX(getStage()->getWidth());

            left->addTween(Actor::TweenX(0), dur);
            spTween t = right->addTween(Actor::TweenX(getStage()->getWidth() / 2), dur);
            t->setDoneCallback([ = ](Event*)
            {
                current->getHolder()->insertSiblingAfter(next->getHolder());
                current->getHolder()->detach();

                left->addTween(Actor::TweenX(-left->getWidth()), dur);
                spTween a = right->addTween(Actor::TweenX(getStage()->getWidth()), dur);
                waitTween(a);
            });
        }

        void TransitionShutters::_clear()
        {
            left->detach();
            right->detach();
        }


        void TransitionShutters::assign(Scene* scene)
        {
            spTransition t = new TransitionShutters;
            scene->setTransitionIn(t);
            scene->setTransitionOut(t);
        }



        TransitionQuads::TransitionQuads() : _center(0, 0)
        {
            //_singleDirection = true;
            getStage()->addEventListener(TouchEvent::CLICK, CLOSURE(this, &TransitionQuads::clicked));
        }

        TransitionQuads::~TransitionQuads()
        {
            getStage()->removeEventListeners(this);
        }

        void TransitionQuads::assign(Scene* scene)
        {
            spTransition t = new TransitionQuads;
            scene->setTransitionIn(t);
            scene->setTransitionOut(t);
        }

        void TransitionQuads::clicked(Event* ev)
        {
            TouchEvent* te = (TouchEvent*)ev;
            _center = getStage()->local2parent(te->localPosition);
        }


        void TransitionQuads::update(const UpdateState& us)
        {
            Color b(0, 0, 0, 0);

            Point ds = core::getDisplaySize();
            Rect vp(Point(0, 0), ds);


            IVideoDriver::instance->setRenderTarget(_mask);

            RenderState rs;
#if OXYGINE_RENDERER >= 5
            STDRenderer& r = *STDRenderer::getCurrent();
#else
            Material::setCurrent(0);
            STDRenderer& r = *STDMaterial::instance->getRenderer();
            rs.material = STDMaterial::instance;
#endif

            r.initCoordinateSystem(ds.x, ds.y, true);
            //TweenAlphaFade
            //r.Renderer::begin(0);

            {
                _holder->setPosition(Vector2(0, 0));
                _holder->setVisible(true);
                _holder->render(rs);
                //rs.material->render(_holder.get(), rs);
                _holder->setPosition(getStage()->parent2local(Vector2(0, 0)));
                //_holder->setVisible(false);
                //r.end();
#if OXYGINE_RENDERER < 5
                rs.material->finish();
#endif
            }

            IVideoDriver::instance->setRenderTarget(0);

        }

        void TransitionQuads::_run(spScene current, spScene next, bool back)
        {
            Point ds = core::getDisplaySize();
            spActor holder = new Actor;
            holder->setPosition(getStage()->parent2local(Vector2(0, 0)));
            //holder->setVisible(false);
            holder->setSize(core::getDisplaySize());
            holder->attachTo(getStage());
            holder->setPriority(1000);

            int numX = ds.x / 40;
            int numY = ds.y / 40;

            //logs::messageln("tq1");
            Vector2 quad(holder->getWidth() / numX, holder->getHeight() / numY);
            spTween slowestTween;

            for (int y = 0; y < numY; ++y)
            {
                for (int x = 0; x < numX; ++x)
                {
                    spSprite sp = new ColorRectSprite;
                    Vector2 pos(quad.x * x, quad.y * y);
                    pos += quad / 2;
                    sp->setPosition(pos);
                    sp->setAnchor(Vector2(0.5f, 0.5f));
                    sp->setSize(quad);
                    sp->attachTo(holder);
                    sp->setScale(0);
                    sp->setColor(Color(0xffffffff));
                    Vector2 d = pos - _center;
                    float time = d.length() / holder->getSize().length();
                    /*
                    if (back)
                    time = 1.0f - time;
                    */

                    float mp = 4.0f;
                    //mp = 10;
                    int tm = int(1 + time * 800 * mp);
                    spTween nt = sp->addTween(Actor::TweenScale(1.0f), int(100 * mp), 1, false, tm);
                    if (!slowestTween || (int)slowestTween->getDelay() < tm)
                        slowestTween = nt;
                }
            }
        }

        void TransitionQuads::_clear()
        {
            OX_ASSERT(!"not implemented");
            //_next->getHolder()->attachTo(getStage());

            _holder->detach();
            _bg->detach();
            _bg->setMask(0);

            _mask->release();
            _mask = 0;
        }
    }
}