#include "coreengine/audiodecoder.h"
#include "coreengine/gameconsole.h"

#include <cmath>
#include <algorithm>

#ifdef AUDIOSUPPORT
#include <opusfile.h>
#endif

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"

// Include stb_vorbis implementation
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-value"
#endif
#include "stb_vorbis.c"
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

namespace
{
    /**
     * @brief Converts raw channel data to interleaved stereo and resamples to targetSampleRate.
     */
    DecodedAudio convertAndResample(const float* inSamples, qint64 inFrames, qint32 inChannels, qint32 inSampleRate, qint32 targetSampleRate)
    {
        DecodedAudio result;
        result.channels = 2;
        result.sampleRate = targetSampleRate;

        if (!inSamples || inFrames <= 0 || inChannels <= 0 || inSampleRate <= 0 || targetSampleRate <= 0)
        {
            return result;
        }

        // Step 1: Convert to stereo at the input sample rate
        std::vector<float> stereoSamples(inFrames * 2);
        if (inChannels == 1)
        {
            for (qint64 i = 0; i < inFrames; ++i)
            {
                float s = inSamples[i];
                stereoSamples[i * 2 + 0] = s;
                stereoSamples[i * 2 + 1] = s;
            }
        }
        else if (inChannels == 2)
        {
            std::copy(inSamples, inSamples + inFrames * 2, stereoSamples.begin());
        }
        else
        {
            // For > 2 channels, take first two channels
            for (qint64 i = 0; i < inFrames; ++i)
            {
                stereoSamples[i * 2 + 0] = inSamples[i * inChannels + 0];
                stereoSamples[i * 2 + 1] = inSamples[i * inChannels + 1];
            }
        }

        // Step 2: Resample if needed
        if (inSampleRate == targetSampleRate)
        {
            result.samples = std::move(stereoSamples);
            result.totalFrames = inFrames;
        }
        else
        {
            double ratio = static_cast<double>(inSampleRate) / static_cast<double>(targetSampleRate);
            qint64 outFrames = static_cast<qint64>(std::floor(inFrames / ratio));
            result.samples.resize(outFrames * 2);
            result.totalFrames = outFrames;

            for (qint64 i = 0; i < outFrames; ++i)
            {
                double srcPos = i * ratio;
                qint64 srcIdx = static_cast<qint64>(srcPos);
                double frac = srcPos - srcIdx;

                if (srcIdx + 1 < inFrames)
                {
                    result.samples[i * 2 + 0] = static_cast<float>(stereoSamples[srcIdx * 2 + 0] * (1.0 - frac) + stereoSamples[(srcIdx + 1) * 2 + 0] * frac);
                    result.samples[i * 2 + 1] = static_cast<float>(stereoSamples[srcIdx * 2 + 1] * (1.0 - frac) + stereoSamples[(srcIdx + 1) * 2 + 1] * frac);
                }
                else if (srcIdx < inFrames)
                {
                    result.samples[i * 2 + 0] = stereoSamples[srcIdx * 2 + 0];
                    result.samples[i * 2 + 1] = stereoSamples[srcIdx * 2 + 1];
                }
            }
        }

        return result;
    }
}

namespace AudioDecoder
{
    DecodedAudio decodeWav(const QByteArray& data, qint32 targetSampleRate)
    {
        if (data.isEmpty())
        {
            return DecodedAudio();
        }

        drwav wav;
        if (!drwav_init_memory(&wav, data.constData(), static_cast<size_t>(data.size()), nullptr))
        {
            return DecodedAudio();
        }

        std::vector<float> rawSamples(wav.totalPCMFrameCount * wav.channels);
        drwav_uint64 framesRead = drwav_read_pcm_frames_f32(&wav, wav.totalPCMFrameCount, rawSamples.data());
        qint32 channels = static_cast<qint32>(wav.channels);
        qint32 sampleRate = static_cast<qint32>(wav.sampleRate);
        drwav_uninit(&wav);

        if (framesRead == 0)
        {
            return DecodedAudio();
        }

        return convertAndResample(rawSamples.data(), static_cast<qint64>(framesRead), channels, sampleRate, targetSampleRate);
    }

    DecodedAudio decodeOgg(const QByteArray& data, qint32 targetSampleRate)
    {
        if (data.isEmpty())
        {
            return DecodedAudio();
        }

        int channels = 0;
        int sampleRate = 0;
        short* output = nullptr;
        int samplesPerChannel = stb_vorbis_decode_memory(
            reinterpret_cast<const unsigned char*>(data.constData()),
            static_cast<int>(data.size()),
            &channels,
            &sampleRate,
            &output
        );

        if (samplesPerChannel <= 0 || output == nullptr)
        {
            if (output)
            {
                free(output);
            }
            return DecodedAudio();
        }

        qint64 totalSamples = static_cast<qint64>(samplesPerChannel) * channels;
        std::vector<float> rawSamples(totalSamples);
        for (qint64 i = 0; i < totalSamples; ++i)
        {
            rawSamples[i] = static_cast<float>(output[i]) / 32768.0f;
        }
        free(output);

        return convertAndResample(rawSamples.data(), samplesPerChannel, channels, sampleRate, targetSampleRate);
    }

    DecodedAudio decodeOpus(const QByteArray& data, qint32 targetSampleRate)
    {
#ifdef AUDIOSUPPORT
        if (data.isEmpty())
        {
            return DecodedAudio();
        }

        int error = 0;
        OggOpusFile* of = op_open_memory(
            reinterpret_cast<const unsigned char*>(data.constData()),
            static_cast<size_t>(data.size()),
            &error
        );

        if (!of || error != 0)
        {
            return DecodedAudio();
        }

        ogg_int64_t totalPcm = op_pcm_total(of, -1);
        std::vector<float> stereoSamples;
        if (totalPcm > 0)
        {
            stereoSamples.reserve(static_cast<size_t>(totalPcm * 2));
        }

        float pcmBuf[2048 * 2];
        while (true)
        {
            int readSamples = op_read_float_stereo(of, pcmBuf, sizeof(pcmBuf) / sizeof(float));
            if (readSamples <= 0)
            {
                break;
            }
            stereoSamples.insert(stereoSamples.end(), pcmBuf, pcmBuf + readSamples * 2);
        }

        op_free(of);

        if (stereoSamples.empty())
        {
            return DecodedAudio();
        }

        qint64 decodedFrames = static_cast<qint64>(stereoSamples.size() / 2);
        return convertAndResample(stereoSamples.data(), decodedFrames, 2, 48000, targetSampleRate);
#else
        Q_UNUSED(data);
        Q_UNUSED(targetSampleRate);
        return DecodedAudio();
#endif
    }

    DecodedAudio decodeMp3(const QByteArray& data, qint32 targetSampleRate)
    {
        if (data.isEmpty())
        {
            return DecodedAudio();
        }

        drmp3_config config;
        drmp3_uint64 totalPCMFrameCount = 0;
        float* pSampleData = drmp3_open_memory_and_read_pcm_frames_f32(
            data.constData(), static_cast<size_t>(data.size()), &config, &totalPCMFrameCount, nullptr);

        if (!pSampleData || totalPCMFrameCount == 0)
        {
            if (pSampleData)
            {
                drmp3_free(pSampleData, nullptr);
            }
            return DecodedAudio();
        }

        qint32 channels = static_cast<qint32>(config.channels);
        qint32 sampleRate = static_cast<qint32>(config.sampleRate);
        DecodedAudio result = convertAndResample(pSampleData, static_cast<qint64>(totalPCMFrameCount), channels, sampleRate, targetSampleRate);
        drmp3_free(pSampleData, nullptr);

        return result;
    }

    DecodedAudio decode(const QByteArray& data, const QString& filenameHint, qint32 targetSampleRate)
    {
        if (data.isEmpty())
        {
            return DecodedAudio();
        }

        QString lowerName = filenameHint.toLower();
        if (lowerName.endsWith(".ogg") || lowerName.endsWith(".opus"))
        {
            DecodedAudio res = decodeOpus(data, targetSampleRate);
            if (res.isValid()) return res;
            res = decodeOgg(data, targetSampleRate);
            if (res.isValid()) return res;
        }
        else if (lowerName.endsWith(".wav"))
        {
            DecodedAudio res = decodeWav(data, targetSampleRate);
            if (res.isValid()) return res;
        }
        else if (lowerName.endsWith(".mp3"))
        {
            DecodedAudio res = decodeMp3(data, targetSampleRate);
            if (res.isValid()) return res;
        }

        // Magic header check
        if (data.size() >= 4)
        {
            if (data.startsWith("RIFF"))
            {
                DecodedAudio res = decodeWav(data, targetSampleRate);
                if (res.isValid()) return res;
            }
            else if (data.startsWith("OggS"))
            {
                DecodedAudio res = decodeOpus(data, targetSampleRate);
                if (res.isValid()) return res;
                res = decodeOgg(data, targetSampleRate);
                if (res.isValid()) return res;
            }
            else if (data.startsWith("ID3") || (static_cast<unsigned char>(data[0]) == 0xFF && (static_cast<unsigned char>(data[1]) & 0xE0) == 0xE0))
            {
                DecodedAudio res = decodeMp3(data, targetSampleRate);
                if (res.isValid()) return res;
            }
        }

        // Fallback: try each format
        DecodedAudio res = decodeOpus(data, targetSampleRate);
        if (res.isValid()) return res;
        res = decodeOgg(data, targetSampleRate);
        if (res.isValid()) return res;
        res = decodeWav(data, targetSampleRate);
        if (res.isValid()) return res;
        res = decodeMp3(data, targetSampleRate);
        if (res.isValid()) return res;

        return DecodedAudio();
    }
}
