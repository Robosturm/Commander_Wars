#pragma once
#include "test.h"
#include "oxygine-sound.h"

extern SoundPlayer splayer;

class TestCoord: public Test
{
public:

    spSprite orange;
    spSoundInstance snd;

    TestCoord()
    {
        orange = new ColorRectSprite;
        orange->setColor(Color::Orange);
        orange->setSize(getSize() - Vector2(100, 100));
        orange->setPosition(getSize() / 2 - orange->getSize() / 2);

        orange->addEventListener(TouchEvent::OVER, CLOSURE(this, &TestCoord::onEvent));
        orange->addEventListener(TouchEvent::MOVE, CLOSURE(this, &TestCoord::onEvent));
        orange->addEventListener(TouchEvent::OUTX, CLOSURE(this, &TestCoord::onEvent));

        orange->setTouchChildrenEnabled(false);
        orange->attachTo(_content);
    }

    void onEvent(Event* ev)
    {
        TouchEvent* te = safeCast<TouchEvent*>(ev);

        if (ev->type == TouchEvent::OVER)
        {
            orange->setColor(Color::Green);
            snd = splayer.play("amb_bird_2", PlayOptions().loop());
        }
        else if (ev->type == TouchEvent::OUTX)
        {
            orange->setColor(Color::Orange);
            snd->stop();

        }
        else     // TouchEvent::MOVE
        {

            Vector2 const center = orange->getSize() / 2;
            Vector2 const dir    = te->localPosition - center;

            snd->setCoord(dir / 1000);
        }
    }
};
