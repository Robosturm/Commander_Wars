#pragma once
#include "../SoundHandle.h"

namespace oxygine
{
    class Sound;

    class SoundHandleNull : public SoundHandle
    {
    public:
        SoundHandleNull(int duration);

    protected:

        void _update() override;
        void _init() override;
        void _play() override;
        void _pause() override;
        void _resume() override;
        void _stop() override;
        void _setPosition(int tm) override;


        int _duration;
        int _pos;
        int _lastUpdate;


        timeMS _getPosition() const override;
    };
}