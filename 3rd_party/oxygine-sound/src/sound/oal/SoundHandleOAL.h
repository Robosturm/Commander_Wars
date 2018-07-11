#pragma once
#include "../SoundHandle.h"
#include "../oal.h"

namespace oxygine
{
    class Sound;

    class SoundHandleOAL : public SoundHandle
    {
    public:
        static SoundHandleOAL* create(Sound*);

        SoundHandleOAL();
        bool isPlaying() const { return _alSource != 0; }

    protected:
        friend class StaticStreamOAL;
        friend class OggStreamOAL;

        class SoundSystemOAL* ss();

        void _init() override;
        void _play() override;
        void _pause() override;
        void _resume() override;
        void _stop() override;

        void _update() override;
        void _updateVolume() override;
        void _updatePitch() override;
        void _updateLoop() override;
        void _setPosition(int tm) override;

        void _ended();
        void _restore();

        virtual void _xplay() {}
        virtual void _xpause() {}
        virtual void _xupdate() {}
        virtual void _xresume() {}
        virtual void _xstop() {}
        virtual void _xupdateLoop() {}
        virtual void _xsetPosition(int tm) {}

        ALuint _alSource;
        ALint _pos;
    };
}
