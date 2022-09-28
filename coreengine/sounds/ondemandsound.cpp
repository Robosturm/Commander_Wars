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
    if (soundCache->sound[i] == nullptr &&
        !soundCache->timer[i]->isActive())
    {
        CONSOLE_PRINT("Playing sound: " + file + " using cache sound " + QString::number(i), Console::eDEBUG);
        auto* soundItem = new QSoundEffect(this);
        soundItem->setObjectName(file + QString::number(i));
        soundItem->setAudioDevice(m_audioDevice);
        soundItem->setSource(soundCache->cacheUrl);
        soundItem->setVolume(sound);
        if (loops < 0)
        {
            soundItem->setLoopCount(QSoundEffect::Infinite);
        }
        else
        {
            soundItem->setLoopCount(loops);
        }
        soundItem->setMuted(false);
        soundCache->sound[i] = soundItem;
        auto* pSoundCache = soundCache.get();
        // start play
        if (delay > 0)
        {
            soundCache->timer[i]->start(delay);
            connect(soundCache->timer[i].get(), &QTimer::timeout, soundItem, [this, stopOldestSound, pSoundCache, i]()
            {
                emit sigPlayDelayedSound(pSoundCache, i, stopOldestSound);
            });
        }
        else
        {
            if (stopOldestSound)
            {
                AudioThread::stopOldestSound(pSoundCache);
            }
            soundItem->play();
        }
        connect(soundCache->sound[i], &QSoundEffect::playingChanged, this, [this, pSoundCache, i]()
        {
            emit sigDeleteSound(pSoundCache, i);
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
    CONSOLE_PRINT("Stopping sound at index " + QString::number(soundIndex), Console::eDEBUG);
    if (soundData->sound[soundIndex] != nullptr)
    {
        soundData->sound[soundIndex]->stop();
        soundData->sound[soundIndex]->deleteLater();
        soundData->sound[soundIndex] = nullptr;
    }
}

void AudioThread::playDelayedSound(SoundData* soundData, qint32 soundIndex, bool stopOldestSound)
{
    CONSOLE_PRINT("Starting delayed sound", Console::eDEBUG);
    if (stopOldestSound)
    {
        AudioThread::stopOldestSound(soundData);
    }
    if (soundData->sound[soundIndex] != nullptr)
    {
        soundData->sound[soundIndex]->play();
    }
}

void AudioThread::deleteSound(SoundData* soundData, qint32 soundIndex)
{
    if (soundData->sound[soundIndex] != nullptr &&
        !soundData->sound[soundIndex]->isPlaying())
    {
        CONSOLE_PRINT("Stopping sound on playing changed.", Console::eDEBUG);
        soundData->sound[soundIndex]->stop();
        soundData->sound[soundIndex]->deleteLater();
        soundData->sound[soundIndex] = nullptr;
    }
}
#endif

