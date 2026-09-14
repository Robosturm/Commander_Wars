/*
 * Minimal AAudio-backed implementation of the small subset of the PortAudio C API
 * used by AudioManager (coreengine/audiomanager.cpp) and Settings (coreengine/settings.cpp).
 *
 * Upstream PortAudio ships no Android host API: on stock Android there are no ALSA device
 * nodes, no running PulseAudio server and no JACK server, so building the real PortAudio
 * library for Android results in zero usable host APIs. Pa_Initialize() would still
 * "succeed", but Pa_GetDefaultOutputDevice() returns paNoDevice and every Pa_OpenStream()
 * call fails, so no sound is ever produced.
 *
 * This file provides a drop-in replacement of just the functions AudioManager calls,
 * backed by Android's native AAudio API (available since API level 26; this project
 * targets API level 27+), so the rest of the audio engine (mixing, decoding, playlists)
 * stays unchanged across all platforms.
 */

#include "portaudio.h"

#include <aaudio/AAudio.h>
#include "coreengine/gameconsole.h"

#include <algorithm>
#include <cstring>
#include <vector>

namespace
{
    constexpr const char* LOG_TAG = "PortAudioAAudioShim";

    struct PaAndroidStream
    {
        AAudioStream* aaudioStream{nullptr};
        PaStreamCallback* callback{nullptr};
        void* userData{nullptr};
        aaudio_format_t outputFormat{AAUDIO_FORMAT_UNSPECIFIED};
        std::vector<float> floatBuffer;
    };

    aaudio_data_callback_result_t audioDataCallback(AAudioStream* /*stream*/, void* userData,
                                                     void* audioData, int32_t numFrames)
    {
        auto* paStream = static_cast<PaAndroidStream*>(userData);
        if (paStream == nullptr || paStream->callback == nullptr)
        {
            return AAUDIO_CALLBACK_RESULT_STOP;
        }

        PaStreamCallbackTimeInfo timeInfo;
        memset(&timeInfo, 0, sizeof(timeInfo));
        if (paStream->outputFormat == AAUDIO_FORMAT_PCM_FLOAT)
        {
            int result = paStream->callback(nullptr, audioData, static_cast<unsigned long>(numFrames),
                                             &timeInfo, 0, paStream->userData);
            return (result == paContinue) ? AAUDIO_CALLBACK_RESULT_CONTINUE : AAUDIO_CALLBACK_RESULT_STOP;
        }

        if (paStream->outputFormat != AAUDIO_FORMAT_PCM_I16)
        {
            return AAUDIO_CALLBACK_RESULT_STOP;
        }

        const size_t sampleCount = static_cast<size_t>(numFrames) * 2;
        paStream->floatBuffer.resize(sampleCount);
        int result = paStream->callback(nullptr, paStream->floatBuffer.data(),
                                         static_cast<unsigned long>(numFrames), &timeInfo, 0,
                                         paStream->userData);
        auto* output = static_cast<int16_t*>(audioData);
        for (size_t sample = 0; sample < sampleCount; ++sample)
        {
            output[sample] = static_cast<int16_t>(std::clamp(paStream->floatBuffer[sample], -1.0f, 1.0f) * 32767.0f);
        }
        return (result == paContinue) ? AAUDIO_CALLBACK_RESULT_CONTINUE : AAUDIO_CALLBACK_RESULT_STOP;
    }

    void errorCallback(AAudioStream* /*stream*/, void* /*userData*/, aaudio_result_t error)
    {
        CONSOLE_PRINT("AAudio stream error: %s", AAudio_convertResultToText(error), GameConsole::eERROR);
    }
}

extern "C"
{

PaError Pa_Initialize(void)
{
    return paNoError;
}

PaError Pa_Terminate(void)
{
    return paNoError;
}

const char* Pa_GetErrorText(PaError errorCode)
{
    switch (errorCode)
    {
        case paNoError:
            return "Success";
        case paNotInitialized:
            return "PortAudio not initialized";
        case paInvalidDevice:
            return "Invalid device";
        case paDeviceUnavailable:
            return "Device unavailable";
        case paBadStreamPtr:
            return "Invalid stream pointer";
        default:
            return "PortAudio AAudio shim error";
    }
}

PaDeviceIndex Pa_GetDeviceCount(void)
{
    return 1;
}

const PaDeviceInfo* Pa_GetDeviceInfo(PaDeviceIndex device)
{
    if (device != 0)
    {
        return nullptr;
    }
    static PaDeviceInfo info = []()
    {
        PaDeviceInfo i;
        memset(&i, 0, sizeof(i));
        i.structVersion = 2;
        i.name = "Android Audio";
        i.hostApi = 0;
        i.maxInputChannels = 0;
        i.maxOutputChannels = 2;
        i.defaultLowInputLatency = 0.01;
        i.defaultLowOutputLatency = 0.01;
        i.defaultHighInputLatency = 0.05;
        i.defaultHighOutputLatency = 0.05;
        i.defaultSampleRate = 48000.0;
        return i;
    }();
    return &info;
}

PaDeviceIndex Pa_GetDefaultOutputDevice(void)
{
    return 0;
}

PaError Pa_OpenStream(PaStream** stream,
                       const PaStreamParameters* inputParameters,
                       const PaStreamParameters* outputParameters,
                       double sampleRate,
                       unsigned long framesPerBuffer,
                       PaStreamFlags /*streamFlags*/,
                       PaStreamCallback* streamCallback,
                       void* userData)
{
    (void)inputParameters;

    if (stream == nullptr || outputParameters == nullptr || streamCallback == nullptr)
    {
        return paBadStreamPtr;
    }

    auto* paStream = new PaAndroidStream();
    paStream->callback = streamCallback;
    paStream->userData = userData;

    AAudioStreamBuilder* builder = nullptr;
    aaudio_result_t result = AAudio_createStreamBuilder(&builder);
    if (result != AAUDIO_OK || builder == nullptr)
    {
        delete paStream;
        return paUnanticipatedHostError;
    }

    AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_OUTPUT);
    AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_FLOAT);
    AAudioStreamBuilder_setChannelCount(builder, outputParameters->channelCount);
    AAudioStreamBuilder_setSampleRate(builder, static_cast<int32_t>(sampleRate));
    AAudioStreamBuilder_setPerformanceMode(builder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
    AAudioStreamBuilder_setSharingMode(builder, AAUDIO_SHARING_MODE_SHARED);
    if (framesPerBuffer != paFramesPerBufferUnspecified)
    {
        AAudioStreamBuilder_setFramesPerDataCallback(builder, static_cast<int32_t>(framesPerBuffer));
    }
    AAudioStreamBuilder_setDataCallback(builder, audioDataCallback, paStream);
    AAudioStreamBuilder_setErrorCallback(builder, errorCallback, paStream);

    AAudioStream* aaudioStream = nullptr;
    result = AAudioStreamBuilder_openStream(builder, &aaudioStream);
    AAudioStreamBuilder_delete(builder);

    if (result != AAUDIO_OK || aaudioStream == nullptr)
    {
        CONSOLE_PRINT("Failed to open AAudio stream: %s", AAudio_convertResultToText(result), GameConsole::eERROR);
        delete paStream;
        return paUnanticipatedHostError;
    }

    paStream->aaudioStream = aaudioStream;
    paStream->outputFormat = AAudioStream_getFormat(aaudioStream);
    const int32_t channelCount = AAudioStream_getChannelCount(aaudioStream);
    if ((paStream->outputFormat != AAUDIO_FORMAT_PCM_FLOAT && paStream->outputFormat != AAUDIO_FORMAT_PCM_I16) ||
        channelCount != outputParameters->channelCount)
    {
        CONSOLE_PRINT("Unsupported AAudio output configuration: format=%d channels=%d",
                      paStream->outputFormat, channelCount, GameConsole::eERROR);
        AAudioStream_close(aaudioStream);
        delete paStream;
        return paSampleFormatNotSupported;
    }

    if (paStream->outputFormat == AAUDIO_FORMAT_PCM_I16)
    {
        const int32_t callbackFrames = AAudioStream_getFramesPerDataCallback(aaudioStream);
        if (callbackFrames > 0)
        {
            paStream->floatBuffer.resize(static_cast<size_t>(callbackFrames) * channelCount);
        }
    }
    CONSOLE_PRINT_MODULE("Opened AAudio stream: format=%d channels=%d sampleRate=%d callbackFrames=%d",
                  paStream->outputFormat, channelCount,
                  AAudioStream_getSampleRate(aaudioStream),
                  AAudioStream_getFramesPerDataCallback(aaudioStream), GameConsole::eINFO, GameConsole::eAudio);
    *stream = paStream;
    return paNoError;
}

PaError Pa_StartStream(PaStream* stream)
{
    auto* paStream = static_cast<PaAndroidStream*>(stream);
    if (paStream == nullptr || paStream->aaudioStream == nullptr)
    {
        return paBadStreamPtr;
    }
    aaudio_result_t result = AAudioStream_requestStart(paStream->aaudioStream);
    return (result == AAUDIO_OK) ? paNoError : paUnanticipatedHostError;
}

PaError Pa_StopStream(PaStream* stream)
{
    auto* paStream = static_cast<PaAndroidStream*>(stream);
    if (paStream == nullptr || paStream->aaudioStream == nullptr)
    {
        return paBadStreamPtr;
    }
    aaudio_result_t result = AAudioStream_requestStop(paStream->aaudioStream);
    return (result == AAUDIO_OK) ? paNoError : paUnanticipatedHostError;
}

PaError Pa_CloseStream(PaStream* stream)
{
    auto* paStream = static_cast<PaAndroidStream*>(stream);
    if (paStream == nullptr)
    {
        return paBadStreamPtr;
    }
    if (paStream->aaudioStream != nullptr)
    {
        AAudioStream_close(paStream->aaudioStream);
    }
    delete paStream;
    return paNoError;
}

} // extern "C"
