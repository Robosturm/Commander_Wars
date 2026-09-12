#ifndef AUDIODECODER_H
#define AUDIODECODER_H

#include <QString>
#include <QByteArray>
#include <vector>
#include <QtGlobal>

struct DecodedAudio
{
    std::vector<float> samples; // Interleaved stereo float PCM (L, R, L, R...)
    qint32 sampleRate{44100};
    qint32 channels{2};
    qint64 totalFrames{0};

    bool isValid() const
    {
        return !samples.empty() && totalFrames > 0;
    }
};

namespace AudioDecoder
{
    /**
     * @brief Decodes an audio file (WAV, OGG, or MP3) from raw bytes into stereo float PCM at targetSampleRate.
     * @param data Raw byte array containing the audio file.
     * @param filenameHint Optional filename or extension to prioritize a specific decoder.
     * @param targetSampleRate The sample rate to resample output frames to (default 44100 Hz).
     * @return DecodedAudio struct with interleaved stereo samples.
     */
    DecodedAudio decode(const QByteArray& data, const QString& filenameHint = QString(), qint32 targetSampleRate = 44100);

    /**
     * @brief Decodes WAV format audio.
     */
    DecodedAudio decodeWav(const QByteArray& data, qint32 targetSampleRate = 44100);

    /**
     * @brief Decodes OGG Vorbis format audio.
     */
    DecodedAudio decodeOgg(const QByteArray& data, qint32 targetSampleRate = 44100);

    /**
     * @brief Decodes Ogg Opus format audio.
     */
    DecodedAudio decodeOpus(const QByteArray& data, qint32 targetSampleRate = 44100);

    /**
     * @brief Decodes MP3 format audio.
     */
    DecodedAudio decodeMp3(const QByteArray& data, qint32 targetSampleRate = 44100);
}

#endif // AUDIODECODER_H
