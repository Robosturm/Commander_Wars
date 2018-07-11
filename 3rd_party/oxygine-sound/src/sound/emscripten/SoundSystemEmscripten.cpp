#include "SoundSystemEmscripten.h"
#include "SoundEmscripten.h"
#include "ox/DebugActor.hpp"
#include "ox/stringUtils.hpp"
#include <emscripten.h>

namespace oxygine
{
    SoundSystem*    SoundSystem::create()
    {
        if (!SoundSystem::instance)
            SoundSystem::instance = new SoundSystemEmscripten;

        return SoundSystem::instance;
    }

    SoundSystemEmscripten::SoundSystemEmscripten()
    {
        EM_ASM_ARGS(
        {
            sound.init();
        }, 0);
    }

    SoundSystemEmscripten::~SoundSystemEmscripten()
    {

    }

    void SoundSystemEmscripten::release()
    {
        //_channels._channels.clear();
    }

    void SoundSystemEmscripten::pause()
    {

    }

    void SoundSystemEmscripten::resume()
    {

    }

    void SoundSystemEmscripten::stop()
    {
        //_channels.stop();
    }


    Sound* SoundSystemEmscripten::createSound(std::vector<unsigned char>& data, bool swap)
    {
        return 0;
    }

    Sound* SoundSystemEmscripten::createSound(const char* file, bool streaming)
    {
        return new SoundEmscripten(file);
    }



    void SoundSystemEmscripten::update()
    {
        //_channels.update();
        if (DebugActor::instance)
        {

            int num = EM_ASM_INT(
            {
                return sound.stats();
            }, 0);

            char str[255];
            safe_sprintf(str, "handles: %d", num);
            DebugActor::instance->addDebugString(str);
        }
    }
}