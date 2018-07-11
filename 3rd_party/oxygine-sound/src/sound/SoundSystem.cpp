#include "SoundSystem.h"
namespace oxygine
{

    SoundSystem::SoundSystem()
    {

    }

    oxygine::SoundSystem* SoundSystem::get()
    {
        return instance;
    }

    void SoundSystem::free()
    {
        if (instance)
            instance->release();
        delete instance;
        instance = 0;
    }
}
