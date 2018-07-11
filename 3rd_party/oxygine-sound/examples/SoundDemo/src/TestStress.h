#pragma once
#include "test.h"
#include "oxygine-sound.h"


class TestStress: public Test
{
public:
    TestStress()
    {
        addButton("run", "run");
    }

    void clicked(string id)
    {
        if (id == "run")
        {
            splayer.stop();
        }
    }

    void doUpdate(const UpdateState& us)
    {
        //oxygine::sleep(rand() % 80);

        int num = splayer.getSoundsNum();
        if (num)
        {
            spSoundInstance s = splayer.getSoundByIndex(rand() % num);
            if ((rand() % 150) == 0)
                s->stop();
        }

        //if ((rand() % 150) == 0)
        //  sleep(rand() % 1000);

        if (num >= 15)
            return;

        const char* sounds[] = { "lose_round", "win_round", "collect_rare"};

        string id = sounds[rand() % 3];
        splayer.play(id, PlayOptions().fade(rand() % 500, rand() % 500));


    }
};
