#include "StaticSoundHandleOAL.h"

namespace oxygine
{
    void OAL_CHECK();

    StaticSoundHandleOAL::StaticSoundHandleOAL(ALuint buffer): _buffer(buffer)
    {
    }

    void StaticSoundHandleOAL::_xplay()
    {
        alSourcei(_alSource, AL_BUFFER, _buffer);
        alSourcei(_alSource, AL_LOOPING, _looping ? AL_TRUE : AL_FALSE);
        OAL_CHECK();
    }

    void StaticSoundHandleOAL::_xpause()
    {
        alSourceStop(_alSource);
        OAL_CHECK();
    }


    void StaticSoundHandleOAL::_xresume()
    {
        _xplay();
    }

    timeMS StaticSoundHandleOAL::_getPosition() const
    {
        if (_alSource)
        {
            float tm;
            alGetSourcef(_alSource, AL_SEC_OFFSET, &tm);

            return (timeMS)(tm * 1000);
        }

        return 0;
        return _pos;
    }

    void StaticSoundHandleOAL::_xupdate()
    {
        ALint state = 0;
        alGetSourcei(_alSource, AL_SOURCE_STATE, &state);
        OAL_CHECK();


        if (state == AL_STOPPED)
        {
            _ended();
        }
    }

    void StaticSoundHandleOAL::_xstop()
    {
        alSourceStop(_alSource);
        OAL_CHECK();
    }

    void StaticSoundHandleOAL::_xsetPosition(int tm)
    {
        if (_alSource)
        {
            alSourcef(_alSource, AL_SEC_OFFSET, tm / 1000.0f);
            OAL_CHECK();
        }
        else
        {
            ALint fr;
            alGetBufferi(_buffer, AL_FREQUENCY, &fr);

            ALint ch;
            alGetBufferi(_buffer, AL_CHANNELS, &ch);

            //tm = bytes / Channels / fr;
            //bytes =
            _pos = (tm * ch * fr) / 1000;
            int q = 0;

        }
    }

    void StaticSoundHandleOAL::_xupdateLoop()
    {
        alSourcei(_alSource, AL_LOOPING, _looping ? 1 : 0);
        OAL_CHECK();
    }
}
