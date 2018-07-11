#include "StreamingSoundHandleOAL.h"
#include "oxygine/core/ThreadDispatcher.h"
#include "SoundSystemOAL.h"
#include "pthread.h"
#include <stdlib.h>

namespace oxygine
{

#ifdef CHANNEL_DEBUG
#define  LOG2DN(...)  logs::messageln(__VA_ARGS__)
#define  LOG2D(...)  logs::message(__VA_ARGS__)

#else
#define  LOG2DN(...) ((void)0)
#define  LOG2D(...)  ((void)0)
#endif

#ifdef CHANNEL_DEBUG
#define  LOGDN(...)  logs::messageln(__VA_ARGS__)
#define  LOGD(...)  logs::message(__VA_ARGS__)

#else
#define  LOGDN(...)  ((void)0)
#define  LOGD(...)  ((void)0)
#endif



    void OAL_CHECK();

    pthread_t _thread = pthread_self();
    ThreadDispatcher _messages;
    pthread_key_t _tls;

    bool _synchronized = true;
    int BUFF_SIZE = 20000;
    void* localMem = 0;


    const int evnt_exit = 123;
    const int evnt_pause = 124;

    void StreamingSoundHandleOAL::setBufferSize(int v)
    {
        BUFF_SIZE = v;
    }

    void* _staticThreadFunc(void* t)
    {
        void* mem = malloc(BUFF_SIZE);
        pthread_setspecific(_tls, mem);

        while (true)
        {
            ThreadMessages::message msg;
            _messages.get(msg);
            if (msg.msgid == evnt_exit)
            {
                LOGDN("evnt_exit");
                break;
            }

        }
        free(mem);
        return 0;
    }

    void* getSoundStreamTempBuffer(int& size)
    {
        void* data = pthread_getspecific(_tls);

        size = BUFF_SIZE;
        return data;
    }

    void StreamingSoundHandleOAL::runThread()
    {
        pthread_key_create(&_tls, 0);
        localMem = malloc(BUFF_SIZE);
        pthread_setspecific(_tls, localMem);

        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&_thread, &attr, _staticThreadFunc, 0);

        //stopThread();
    }

    void StreamingSoundHandleOAL::stopThread()
    {
        _messages.clear();
        _messages.post(evnt_exit, 0, 0);
        if (pthread_equal(_thread, pthread_self()))
            return;
        void* ptr = 0;
        pthread_join(_thread, &ptr);
        free(localMem);
        localMem = 0;
    }


    void StreamingSoundHandleOAL::clearThreadQueue()
    {
        _messages.clear();
        _messages.send(evnt_pause, 0, 0);
    }

    void threadDecode(const ThreadMessages::message& msg)
    {
        StreamingSoundHandleOAL* s = (StreamingSoundHandleOAL*)msg.arg1;
        s->asyncDecode();
    }

    void threadStopProcessing(const ThreadMessages::message& msg)
    {
        std::vector<ThreadDispatcher::message>& messages = _messages.lockMessages();
        size_t size = messages.size();
        size_t num = 0;
        for (size_t i = 0; i < size; ++i)
        {
            ThreadDispatcher::message& msg2 = messages[i];
            if (msg2.arg1 == msg.arg1)
            {
                messages.erase(messages.begin() + i);
                size--;
                i--;
                num++;
            }
        }
        _messages.unlockMessages();
        _messages.reply((void*)num);
    }


    void StreamingSoundHandleOAL::asyncDecode()
    {
        ALint nump;
        alGetSourcei(_alSource, AL_BUFFERS_PROCESSED, &nump);

        ALuint buffers[STREAM_BUFFERS];

        alSourceUnqueueBuffers(_alSource, nump, buffers);
        OAL_CHECK();
        decode(buffers, nump);


        ALint state = 0;
        alGetSourcei(_alSource, AL_SOURCE_STATE, &state);
        if (state == AL_STOPPED)
            alSourcePlay(_alSource);
        OAL_CHECK();
    }

    void StreamingSoundHandleOAL::decode(ALuint* buffers, int num)
    {
        OAL_CHECK();

        int size;
        void* data = getSoundStreamTempBuffer(size);

        for (int i = 0; i < num; ++i)
        {
            //logs::messageln("decode %d", getTimeMS());
            ALuint buffer = buffers[i];
            size = _stream->decodeNextBlock(_looping, data, size);
            if (!size)
                break;
            alBufferData(buffer, _format, data, size, _rate);
            OAL_CHECK();

            if (_alSource)
                alSourceQueueBuffers(_alSource, 1, &buffer);

            OAL_CHECK();
        }
    }



    StreamingSoundHandleOAL::StreamingSoundHandleOAL(): _stream(0), _reset(false)
    {
        for (int i = 0; i < STREAM_BUFFERS; ++i)
            _buffers[i] = ss()->getBuffer();

        OAL_CHECK();
    }

    StreamingSoundHandleOAL::~StreamingSoundHandleOAL()
    {
        if (ss())
        {
            for (int i = 0; i < STREAM_BUFFERS; ++i)
                ss()->freeBuffer(_buffers[i]);
        }
    }

    void StreamingSoundHandleOAL::setStream(SoundStream* s)
    {
        _stream = s;

        int channels = s->getNumChannels();
        if (channels == 1)
            _format = AL_FORMAT_MONO16;
        if (channels == 2)
            _format = AL_FORMAT_STEREO16;

        _rate = s->getRate();
    }

    void StreamingSoundHandleOAL::checkNoAsync()
    {
#ifdef OX_DEBUG
        size_t tasks = stopAsyncDecode();
        OX_ASSERT(tasks == 0);
#endif
    }

    size_t StreamingSoundHandleOAL::stopAsyncDecode()
    {
        size_t res = (size_t)_messages.sendCallback(this, 0, threadStopProcessing, 0, true);
        OAL_CHECK();
        return res;
    }

    void StreamingSoundHandleOAL::_xplay()
    {
        checkNoAsync();

        alSourcei(_alSource, AL_LOOPING, AL_FALSE);
        _stream->reset();
        decode(_buffers, STREAM_BUFFERS);
    }

    void StreamingSoundHandleOAL::_xpause()
    {
        stopAsyncDecode();

        alSourceStop(_alSource);
        OAL_CHECK();
    }

    void StreamingSoundHandleOAL::_xresume()
    {
        checkNoAsync();
        if (_reset)
        {
            _reset = false;
            _stream->reset();
        }
        alSourcei(_alSource, AL_LOOPING, AL_FALSE);
        decode(_buffers, STREAM_BUFFERS);
    }

    void StreamingSoundHandleOAL::_xupdate()
    {
        if (_stream->isStreamEnded())
        {
            ALint state = 0;
            alGetSourcei(_alSource, AL_SOURCE_STATE, &state);
            OAL_CHECK();

            if (state == AL_STOPPED)
            {
                stopAsyncDecode();
                alGetSourcei(_alSource, AL_SOURCE_STATE, &state);
                if (state == AL_STOPPED)
                {
                    _ended();
                }
            }

            return;
        }

        ALint nump;
        alGetSourcei(_alSource, AL_BUFFERS_PROCESSED, &nump);

        if (nump)
            _messages.postCallback(this, 0, threadDecode, 0);
    }

    timeMS StreamingSoundHandleOAL::_getPosition() const
    {
        return _stream->getPosition();
    }


    void StreamingSoundHandleOAL::_xsetPosition(int tm)
    {
        if (_alSource)
        {
            stopAsyncDecode();

            _stream->setPosition(tm);

            alSourceStop(_alSource);
            alSourcei(_alSource, AL_BUFFER, 0);
            OAL_CHECK();

            decode(_buffers, STREAM_BUFFERS);
            alSourcePlay(_alSource);

        }
        else
        {
            checkNoAsync();
            _stream->setPosition(tm);
            decode(_buffers, STREAM_BUFFERS);
        }
    }

    void StreamingSoundHandleOAL::_xstop()
    {
        stopAsyncDecode();
        _reset = true;
        //_stream->reset();

        alSourceStop(_alSource);

        OAL_CHECK();
    }

    StreamingOggSoundHandleOAL::StreamingOggSoundHandleOAL(SoundOAL* snd)
    {
        snd->initStream(_stream);
        setStream(&_stream);
    }

    StreamingWavSoundHandleOAL::StreamingWavSoundHandleOAL(SoundOAL* snd)
    {
        snd->initStream(_stream);
        setStream(&_stream);
    }
}
