#include "SoundHandleEmscripten.h"

#include <emscripten.h>
#include "ox/log.hpp"
//#include "Sound.h"
#include "../ResSound.h"
#include "SoundSystemEmscripten.h"
//#include <emscripten/val.h>
//using namespace emscripten;

namespace oxygine
{
    SoundHandleEmscripten::SoundHandleEmscripten(const std::string& path): _handle(0), _path(path)
    {

    }

    SoundHandleEmscripten::~SoundHandleEmscripten()
    {

    }

    void SoundHandleEmscripten::_play()
    {
        if (_handle)
            _stop();

        _handle = EM_ASM_INT(
        {
            return sound.play(Pointer_stringify($0), $1, $2);
        }, _path.c_str(), _volume, _looping);
    }

    void SoundHandleEmscripten::_update()
    {
        int r = EM_ASM_INT(
        {
            return sound.update($0);
        }, _handle);

        if (r == 1)
        {
            _stop();
            _state = ended;
            _handle = 0;
        }
    }

    void SoundHandleEmscripten::_pause()
    {
        EM_ASM_ARGS(
        {
            sound.pause($0);
        }, _handle);
    }

    void SoundHandleEmscripten::_resume()
    {
        if (_handle)
        {
            EM_ASM_ARGS(
            {
                sound.resume($0);
            }, _handle);
        }
        else
        {
            _play();
        }
    }

    void SoundHandleEmscripten::_stop()
    {
        EM_ASM_ARGS(
        {
            sound.stop($0);
            sound.free($0);
        }, _handle);
        _handle = 0;
    }

    void SoundHandleEmscripten::_updateVolume()
    {
        EM_ASM_ARGS(
        {
            sound.setVolume($0, $1);
        }, _handle, _volume);
    }


    void SoundHandleEmscripten::_updatePitch()
    {
    }

    void SoundHandleEmscripten::_updateLoop()
    {
        EM_ASM_ARGS(
        {
            sound.setVolume($0, $1);
        }, _handle, _looping);
    }

    void SoundHandleEmscripten::_init()
    {
    }

    void SoundHandleEmscripten::_setPosition(int tm)
    {
    }

    timeMS SoundHandleEmscripten::_getPosition() const
    {
        return 0;
    }
}