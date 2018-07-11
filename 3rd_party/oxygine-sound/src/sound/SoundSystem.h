#pragma once
#include <vector>

namespace oxygine
{
    class Sound;
    class SoundHandle;

    class SoundSystem
    {
    public:
        static SoundSystem* instance;

    public:
        SoundSystem();
        virtual ~SoundSystem() {}

        static SoundSystem* create();
        static SoundSystem* get();
        static void free();


        void init(int) {}
        virtual void release() = 0;
        virtual void pause() = 0;
        virtual void resume() = 0;
        virtual void stop() = 0;

        virtual Sound* createSound(const char* file, bool streaming) = 0;
        virtual Sound* createSound(std::vector<unsigned char>& data, bool swap) = 0;
        virtual void update() = 0;
    };
}
