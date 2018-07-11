#pragma once
#include "../SoundSystem.h"
namespace oxygine
{
    class Sound;

    class SoundSystemNull : public SoundSystem
    {
    public:
        void release() override;
        void pause() override;
        void resume() override;
        void stop() override;


        Sound* createSound(std::vector<unsigned char>& data, bool swap) override;
        Sound* createSound(const char* file, bool streaming) override;

        void update()  override;
    };
}