#include "../null/SoundSystemNull.h"
namespace oxygine
{

    SoundSystem*    SoundSystem::create()
    {
        if (!SoundSystem::instance)
            SoundSystem::instance = new SoundSystemNull;

        return SoundSystem::instance;
    }

}