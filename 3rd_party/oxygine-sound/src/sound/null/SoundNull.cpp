#include "SoundNull.h"
#include "SoundHandleNull.h"
namespace oxygine
{
    SoundNull::SoundNull(int duration): _duration(duration)
    {
    }

    SoundNull::~SoundNull()
    {

    }

    int SoundNull::getDuration() const
    {
        return 0;
    }

    SoundHandle* SoundNull::createSH()
    {
        return new SoundHandleNull(_duration);
    }
}