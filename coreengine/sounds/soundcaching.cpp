#include "coreengine/audiothread.h"
#include "coreengine/settings.h"

#ifdef AUDIOSUPPORT
#include <QAudioDevice>
#endif

// code precaches sounds for faster replay but may be incompatible with certain os's

void AudioThread::fillSoundCache(qint32 count, QString folder, QString file)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        if (count > SoundData::MAX_SAME_SOUNDS)
        {
            count = SoundData::MAX_SAME_SOUNDS;
        }
        QUrl cacheUrl = GlobalUtils::getUrlForFile(folder + file);
        std::shared_ptr<SoundData> cache = std::make_shared<SoundData>();
        cache->cacheUrl = cacheUrl;
        CONSOLE_PRINT("Caching sound " + folder + file + " with amount " + QString::number(count), Console::eDEBUG);
        for (qint32 i = 0; i < count; ++i)
        {
            cache->sound[i] = new QSoundEffect(this);
            cache->sound[i]->setObjectName(file + QString::number(i));
            cache->sound[i]->setAudioDevice(m_audioDevice);
            cache->sound[i]->setSource(cacheUrl);
            cache->timer[i] = std::make_shared<QTimer>(this);
            cache->timer[i]->setObjectName("SoundEffectTimer");
            cache->timer[i]->setSingleShot(true);
        }
        m_soundCaches.insert(file, cache);
    }
#endif
}

#ifdef AUDIOSUPPORT
bool AudioThread::tryPlaySoundAtCachePosition(std::shared_ptr<SoundData> & soundCache, qint32 i,
                                              QString & file, qint32 loops, qint32 delay, qreal sound, bool stopOldestSound)
{
    bool started = false;

    if (!soundCache->sound[i]->isPlaying() &&
        !soundCache->timer[i]->isActive())
    {
        CONSOLE_PRINT("Playing sound: " + file + " using cache sound " + QString::number(i), Console::eDEBUG);

        soundCache->sound[i]->setVolume(sound);
        if (loops < 0)
        {
            soundCache->sound[i]->setLoopCount(QSoundEffect::Infinite);
        }
        else
        {
            soundCache->sound[i]->setLoopCount(loops);
        }
        soundCache->sound[i]->setMuted(false);
        QObject::disconnect(soundCache->timer[i].get(), &QTimer::timeout, nullptr, nullptr);
        auto* pSoundCache = soundCache.get();
        // start play
        if (delay > 0)
        {
            soundCache->timer[i]->start(delay);
            connect(soundCache->timer[i].get(), &QTimer::timeout, this, [=]()
            {
                if (stopOldestSound)
                {
                    AudioThread::stopOldestSound(pSoundCache);
                }
                pSoundCache->sound[i]->play();
            });
        }
        else
        {
            if (stopOldestSound)
            {
                AudioThread::stopOldestSound(pSoundCache);
            }
            soundCache->sound[i]->play();
        }
        started = true;
        soundCache->nextSoundToUse = i + 1;
    }
    return started;
}
#endif

#ifdef AUDIOSUPPORT
void AudioThread::stopSound(SoundData* soundData, qint32 soundIndex)
{
    if (soundData->sound[soundIndex] != nullptr)
    {
        soundData->sound[soundIndex]->setVolume(0);
        soundData->sound[soundIndex]->setLoopCount(0);
        soundData->sound[soundIndex]->stop();
    }
}

void AudioThread::deleteSound(SoundData* soundData, qint32 soundIndex)
{
}


void AudioThread::playDelayedSound(SoundData* soundData, qint32 soundIndex, bool stopOldestSound)
{
}

#endif
