#pragma once
#include "../SoundSystem.h"
#include "../oal.h"
#include "SoundOAL.h"

namespace oxygine
{
    class SoundOAL;
    class SoundSystemOAL : public SoundSystem
    {
    public:
        SoundSystemOAL(ALCdevice* device, ALCcontext* context);
        ~SoundSystemOAL();

        void release() override;

        void pause() override;
        void resume() override;
        void stop() override;

        SoundOAL* createSound(const char* path, bool streaming) override;
        SoundOAL* createSound(std::vector<unsigned char>& data, bool swap) override;


        bool        isAvailable() const {return _context != 0;}
        float       getVolume() const {return _volume;}

        void setContext();

        void update() override;

        ALuint getSource();
        void   freeSource(ALuint);

        ALuint getBuffer();
        void   freeBuffer(ALuint);

    private:
        std::vector<ALuint> _freeSources;
        std::vector<ALuint> _sources;

        std::vector<ALuint> _freeBuffers;
        std::vector<ALuint> _buffers;

        float _volume;

        ALCdevice* _device;
        ALCcontext* _context;
    };
}
