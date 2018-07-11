#pragma once
#include "../Sound.h"

namespace oxygine
{
    class SoundNull: public Sound
    {
    public:
        SoundNull(int duration);
        ~SoundNull();

        int getDuration() const override;

        SoundHandle* createSH() override;

    private:
        int _duration;
    };
}
