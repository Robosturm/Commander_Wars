#pragma once
#include "SoundHandleOAL.h"
#include "../OggStream.h"
#include "../WavStream.h"

namespace oxygine
{

    const int STREAM_BUFFERS = 3;


    class SoundStream;

    class StreamingSoundHandleOAL : public SoundHandleOAL
    {
    public:
        static void runThread();
        static void stopThread();
        static void clearThreadQueue();

        //control size of buffer for streaming sound
        static void setBufferSize(int);


        StreamingSoundHandleOAL();
        ~StreamingSoundHandleOAL();

        void setStream(SoundStream*);

        void asyncDecode();

    protected:
        void _xplay() override;
        void _xpause() override;
        void _xresume() override;
        void _xupdate() override;
        void _xstop() override;
        timeMS _getPosition() const override;
        void _xsetPosition(int tm) override;

        void decode(ALuint* items, int num);
        size_t stopAsyncDecode();
        void checkNoAsync();

    private:
        SoundStream* _stream;
        ALuint _format;
        ALuint _rate;
        ALuint _buffers[STREAM_BUFFERS];
        bool _reset;
    };


    class SoundOAL;
    class StreamingOggSoundHandleOAL : public StreamingSoundHandleOAL
    {
    public:
        StreamingOggSoundHandleOAL(SoundOAL*);

    protected:
        OggStream _stream;
    };

    class StreamingWavSoundHandleOAL : public StreamingSoundHandleOAL
    {
    public:
        StreamingWavSoundHandleOAL(SoundOAL*);

    protected:
        WavStream _stream;
    };

}
