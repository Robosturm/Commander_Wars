#pragma once
#include "../SoundSystem.h"


namespace oxygine
{
    class SoundEmscripten;


    class SoundSystemEmscripten : public SoundSystem
    {
    public:
        SoundSystemEmscripten();
        ~SoundSystemEmscripten();

        void release() override;
        void pause() override;
        void resume() override;
        void stop() override;


        Sound* createSound(const char* file, bool streaming) override;
        Sound* createSound(std::vector<unsigned char>& data, bool swap) override;

        void update() override;

    private:

    };
}