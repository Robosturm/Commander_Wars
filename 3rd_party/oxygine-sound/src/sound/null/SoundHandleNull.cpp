#include "SoundHandleNull.h"

namespace oxygine
{
    SoundHandleNull::SoundHandleNull(int duration): _duration(duration), _lastUpdate(0), _pos(0)
    {

    }

    timeMS SoundHandleNull::_getPosition() const
    {
        return _pos;
    }

    void SoundHandleNull::_update()
    {
        int tm = getTimeMS();
        int dt = tm - _lastUpdate;

        _pos += dt;
        _lastUpdate = tm;

        if (_pos >= _duration && !_looping)
        {
            _state = ended;
        }
    }

    void SoundHandleNull::_init()
    {

    }

    void SoundHandleNull::_play()
    {
        _pos = 0;
        _lastUpdate = getTimeMS();
    }

    void SoundHandleNull::_pause()
    {

    }
    void SoundHandleNull::_resume()
    {

    }

    void SoundHandleNull::_stop()
    {
    }

    void SoundHandleNull::_setPosition(int tm)
    {
        _pos = tm;
    }
}