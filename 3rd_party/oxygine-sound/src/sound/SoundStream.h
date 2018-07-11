#pragma once
#include "oxygine/core/file.h"
namespace oxygine
{
    class SoundStream
    {
    public:
        SoundStream(): _channels(0), _rate(0), _duration(0), _pcm(0), _ended(true) {}
        virtual ~SoundStream() {}

        virtual bool init(file::handle, bool close) = 0;
        virtual bool init(const void* data, size_t len) = 0;

        virtual int decodeNextBlock(bool looped, void* data, int bufferSize) = 0;
        virtual void decodeAll(void* data, int bufferSize) = 0;
        virtual void reset() = 0;

        virtual void setPosition(int tm) = 0;

        bool isStreamEnded() const { return _ended; }
        int getRate() const { return _rate; }
        int getNumChannels() const { return _channels; }
        int getDuration() const { return _duration; }
        int getPCM() const { return _pcm; }
        virtual int getPosition() const = 0;


    protected:
        int _channels;
        int _rate;
        int _duration;
        int _pcm;
        bool _ended;
    };
}
