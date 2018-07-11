#include "SoundSystemNull.h"
#include "SoundNull.h"
#include "../OggStream.h"

namespace oxygine
{
    SoundSystem* SoundSystem::instance = 0;


    void SoundSystemNull::release()
    {

    }

    void SoundSystemNull::pause()
    {

    }

    void SoundSystemNull::resume()
    {

    }

    void SoundSystemNull::stop()
    {

    }


    Sound* SoundSystemNull::createSound(std::vector<unsigned char>& data, bool swap)
    {
        OggStream oggStream;
        oggStream.init(&data.front(), data.size());


        return new SoundNull(oggStream.getDuration());
    }

    Sound* SoundSystemNull::createSound(const char* file, bool)
    {
        OggStream oggStream;
        oggStream.init(file);

        return new SoundNull(oggStream.getDuration());
    }



    void SoundSystemNull::update()
    {

    }
}