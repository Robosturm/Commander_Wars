#pragma once
#include "SoundHandleOAL.h"
#include "../oal.h"

namespace oxygine
{
    class StaticSoundHandleOAL: public SoundHandleOAL
    {
    public:
        StaticSoundHandleOAL(ALuint buffer);

    protected:
        void _xplay() override;
        void _xpause() override;
        void _xresume() override;
        void _xupdate() override;
        void _xstop() override;

        void _xsetPosition(int tm) override;

        void _xupdateLoop() override;
        timeMS _getPosition() const override;

        ALuint _buffer;
    };

}
