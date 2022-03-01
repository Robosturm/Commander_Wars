#include "coreengine/audiothread.h"
#include "coreengine/settings.h"

#ifdef AUDIOSUPPORT
#include <QAudioDevice>
#endif

// code loads and plays sounds on demand slower replay but compatible with os's
// which create a sound pipe during the sound effect creation which only allow
// a limited amount of parallel open sound pipes.

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
    if (soundCache->sound[i].get() == nullptr &&
        !soundCache->timer[i]->isActive())
    {
        CONSOLE_PRINT("Playing sound: " + file + " using cache sound " + QString::number(i), Console::eDEBUG);
        soundCache->sound[i] = std::make_shared<QSoundEffect>(this);
        soundCache->sound[i]->setObjectName(file + QString::number(i));
        soundCache->sound[i]->setAudioDevice(m_audioDevice);
        soundCache->sound[i]->setSource(soundCache->cacheUrl);
        soundCache->sound[i]->setVolume(sound);
        soundCache->sound[i]->setLoopCount(loops);
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
        SoundData* pSoundData = soundCache.get();
        connect(soundCache->sound[i].get(), &QSoundEffect::playingChanged, this, [=]()
        {
            if (pSoundData->sound[i].get() != nullptr &&
                !pSoundData->sound[i]->isPlaying())
            {
                pSoundData->sound[i].reset();
            }
        }, Qt::QueuedConnection);
        started = true;
        soundCache->nextSoundToUse = i + 1;
    }
    return started;
}
#endif

#ifdef AUDIOSUPPORT
void AudioThread::stopSound(SoundData* soundData, qint32 soundIndex)
{
    soundData->sound[soundIndex].reset();
}
#endif
