#include "SoundSystemOAL.h"
#include "oxygine/actor/DebugActor.h"
#include "oxygine/utils/stringUtils.h"
#include "pthread.h"
#include "../oal.h"
#include "../null/SoundSystemNull.h"
#include "StreamingSoundHandleOAL.h"
#include "oxygine/core/oxygine.h"

namespace oxygine
{
    void OAL_CHECK()
    {
#ifdef OX_DEBUG
        int error = alGetError();

        if (error != AL_NO_ERROR)
        {
            logs::messageln("AL error: %d '%s'", error, alGetString(error));

            OX_ASSERT(!"open al error");

        }
#endif
    }


    SoundSystem* SoundSystem::create()
    {
        if (!SoundSystem::instance)
        {

            ALCdevice* device = 0;

#ifdef __ANDROID__
            device = alcOpenDevice("opensles");
#else
            device = alcOpenDevice(0);

            if (!device)
            {
                sleep(100);
                device = alcOpenDevice(0);
            }
#endif

            if (device)
            {
                ALCcontext* context = alcCreateContext(device, 0);
                if (context)
                    SoundSystem::instance = new SoundSystemOAL(device, context);
            }


            if (!SoundSystem::instance)
            {
                OX_ASSERT(0);
                logs::error("can't create SoundSystemOAL");

                SoundSystem::instance = new SoundSystemNull;
            }
        }

        return SoundSystem::instance;
    }

    SoundSystemOAL::SoundSystemOAL(ALCdevice* device, ALCcontext* context): _device(device), _context(context), _volume(1.0)
    {

        logs::messageln("SoundSystemOAL init");

        alcMakeContextCurrent(_context);
        OAL_CHECK();

        /*
        ALCint nummono, numstereo;
        alcGetIntegerv(_device, ALC_MONO_SOURCES, 1, &nummono);
        alcGetIntegerv(_device, ALC_STEREO_SOURCES, 1, &numstereo);
        */

        ALuint sources[6];
        alGenSources(6, sources);

        _freeSources.assign(sources, sources + 6);
        _sources = _freeSources;

        StreamingSoundHandleOAL::runThread();
        OAL_CHECK();

#ifdef __S3E__
//      alcInit();
#endif
    }

    SoundSystemOAL::~SoundSystemOAL()
    {
    }


    void SoundSystemOAL::setContext()
    {
        alcMakeContextCurrent(_context);
        OAL_CHECK();
    }

    void SoundSystemOAL::pause()
    {
        logs::messageln("SoundSystemOAL::pause");

        StreamingOggSoundHandleOAL::clearThreadQueue();

#if defined(__ANDROID__)
        //android needs special workaround
        alcSuspend();
#endif

#if defined(__S3E__)
        alcSuspendContext(_context);
        alcMakeContextCurrent(0);
#endif

#if TARGET_OS_IPHONE
        alcSuspendContext(_context);
        alcMakeContextCurrent(0);
#endif
    }

    void SoundSystemOAL::resume()
    {
        logs::messageln("SoundSystemOAL::resume");

#if defined(__ANDROID__)
        alcResume();
#endif

#if defined(__S3E__)
        alcMakeContextCurrent(_context);
        alcProcessContext(_context);
#endif

#if TARGET_OS_IPHONE
        alcMakeContextCurrent(_context);
        alcProcessContext(_context);
#endif


        /*
        for (channels::iterator i = _channels.begin(); i != _channels.end(); ++i)
        {
            ChannelOAL& channel = *i;
            channel.resume();
        }
        */
    }

    void SoundSystemOAL::stop()
    {
        StreamingSoundHandleOAL::stopThread();
        for (size_t i = 0; i < _sources.size(); ++i)
        {
            alSourceStop(_sources[i]);
        }
        OAL_CHECK();
    }

    void SoundSystemOAL::release()
    {
        stop();

        if (!_sources.empty())
            alDeleteSources(_sources.size(), &_sources[0]);
        _sources.clear();
        _freeSources.clear();
        OAL_CHECK();

        if (!_buffers.empty())
            alDeleteBuffers(_buffers.size(), &_buffers[0]);
        _freeBuffers.clear();
        _buffers.clear();
        OAL_CHECK();


        alcMakeContextCurrent(0);
        alcDestroyContext(_context);
        _context = 0;

        alcCloseDevice(_device);
        _device = 0;
    }


    SoundOAL* SoundSystemOAL::createSound(const char* path, bool streaming)
    {
        if (!_context)
            return 0;

        file::handle fh = file::open(path, "srb", ep_show_warning);
        if (!fh)
            return 0;

        SoundOAL* sound = 0;
        sound = new SoundOAL(streaming ? path : "", fh);

        return sound;
    }

    oxygine::SoundOAL* SoundSystemOAL::createSound(std::vector<unsigned char>& data, bool swap)
    {
        if (!_context)
            return 0;

        return new SoundOAL(data, swap);
    }

    void SoundSystemOAL::update()
    {
        if (!_device)
            return;


        OAL_CHECK();

        if (DebugActor::instance)
        {
            char str[255];
            safe_sprintf(str, "channels: %d/%d", (int)(_sources.size() - _freeSources.size()), int(_sources.size()));
            DebugActor::instance->addDebugString(str);
        }
    }

    void checkSource(ALuint source)
    {

#ifdef OX_DEBUG
        int queued = 0;
        alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
        OX_ASSERT(queued == 0);

        int unqueued = 0;
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &unqueued);
        OX_ASSERT(unqueued == 0);

        int buf;
        alGetSourcei(source, AL_BUFFER, &buf);
        OX_ASSERT(buf == 0);

        int state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        OX_ASSERT(state == AL_STOPPED || state == AL_INITIAL);
#endif
    }

    ALuint SoundSystemOAL::getSource()
    {
        ALuint source;
        if (_freeSources.empty())
        {
            alGenSources(1, &source);
            _sources.push_back(source);
            OAL_CHECK();
            return source;
        }

        source = _freeSources.back();
        _freeSources.pop_back();
        checkSource(source);

        return source;
    }

    void SoundSystemOAL::freeSource(ALuint source)
    {
        alSourcei(source, AL_LOOPING, AL_FALSE);
        alSourcei(source, AL_BUFFER, 0);
        OAL_CHECK();

        _freeSources.push_back(source);

        checkSource(source);
    }


    ALuint SoundSystemOAL::getBuffer()
    {
        ALuint buffer;
        if (_freeBuffers.empty())
        {
            alGenBuffers(1, &buffer);
            _buffers.push_back(buffer);
            OAL_CHECK();
            return buffer;
        }

        buffer = _freeBuffers.back();
        _freeBuffers.pop_back();
        return buffer;
    }

    void   SoundSystemOAL::freeBuffer(ALuint buffer)
    {
        _freeBuffers.push_back(buffer);
    }
}
