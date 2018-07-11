#pragma once
#include "../Sound.h"
namespace oxygine
{
    class SoundEmscripten : public Sound
    {
    public:
        SoundEmscripten(const char* file);
        ~SoundEmscripten();

        int getDuration() const  override;
        SoundHandle* createSH() override;

    protected:
        std::string _path;
    };
}