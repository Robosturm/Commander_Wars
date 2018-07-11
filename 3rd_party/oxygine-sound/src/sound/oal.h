#pragma once

#ifdef __S3E__
#include "openalext.h"
#define ALCdevice _ALCdevice
#define ALCcontext _ALCcontext
#define alDeleteBuffers _alDeleteBuffers
#define alGenBuffers _alGenBuffers
#define alBufferData _alBufferData
#define alGetError _alGetError
#define alSourceQueueBuffers _alSourceQueueBuffers
#define alSourceUnqueueBuffers _alSourceUnqueueBuffers
#define alGetSourcei _alGetSourcei
#define alSourcei _alSourcei
#define alSourcef _alSourcef
#define alSourcePlay _alSourcePlay
#define alSourcePause _alSourcePause
#define alSourceStop _alSourceStop

#define alcSuspendContext _alcSuspendContext
#define alcProcessContext _alcProcessContext

#define alcMakeContextCurrent _alcMakeContextCurrent
#define alcCreateContext _alcCreateContext
#define alGenSources _alGenSources
#define alcDestroyContext _alcDestroyContext
#define alcOpenDevice _alcOpenDevice
#define alcCloseDevice _alcCloseDevice

#define ALuint _ALuint
#define ALint _ALint
#define ALCint _ALCint
#define ALsizei _ALsizei
#define ALenum _ALenum
#define ALfloat _ALfloat
#define ALCboolean _ALCboolean
#define ALvoid _ALvoid


#else
#include "AL/al.h"
#include "AL/alc.h"
#endif
