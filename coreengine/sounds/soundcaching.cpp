#include "coreengine/audiomanager.h"
#include "coreengine/gameconsole.h"
#include "coreengine/globalutils.h"

#ifdef AUDIOSUPPORT
#include <QAudioDevice>
#endif

// code precaches sounds for faster replay but may be incompatible with certain os's

void AudioManager::fillSoundCache(qint32 count, QString folder, QString file)
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
        CONSOLE_PRINT_MODULE("Caching sound " + folder + file + " with amount " + QString::number(count), GameConsole::eDEBUG, GameConsole::eAudio);
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
bool AudioManager::tryPlaySoundAtCachePosition(std::shared_ptr<SoundData> & soundCache, qint32 i,
                                              QString & file, qint32 loops, qint32 delay, qreal sound, bool stopOldestSound, qint32 duration)
{
    bool started = false;

    if (!soundCache->sound[i]->isPlaying() &&
        !soundCache->timer[i]->isActive())
    {
        CONSOLE_PRINT_MODULE("Playing sound: " + file + " using cache sound " + QString::number(i), GameConsole::eDEBUG, GameConsole::eAudio);

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
            disconnect(soundCache->timer[i].get(), nullptr, nullptr, nullptr);
            connect(soundCache->timer[i].get(), &QTimer::timeout, this, [=]()
            {
                emit sigPlayDelayedSound(pSoundCache, i, stopOldestSound, duration);
            });
            soundCache->timer[i]->start(delay);
        }
        else
        {
            if (stopOldestSound)
            {
                AudioManager::stopOldestSound(pSoundCache);
            }
            playSoundInternal(pSoundCache, i, duration);
        }
        started = true;
        soundCache->nextSoundToUse = i + 1;
    }
    return started;
}
#endif

#ifdef AUDIOSUPPORT
void AudioManager::stopSoundInternal(SoundData* soundData, qint32 soundIndex)
{
    if (soundData->sound[soundIndex] != nullptr)
    {
        soundData->sound[soundIndex]->setVolume(0);
        soundData->sound[soundIndex]->setLoopCount(0);
        soundData->sound[soundIndex]->stop();
    }
}

void AudioManager::deleteSound(SoundData* soundData, qint32 soundIndex)
{
}

void AudioManager::playSoundInternal(SoundData* soundData, qint32 soundIndex, qint32 duration)
{
    if (soundData->sound[soundIndex] != nullptr)
    {
        if (duration > 0)
        {
            disconnect(soundData->timer[soundIndex].get(), nullptr, nullptr, nullptr);
            connect(soundData->timer[soundIndex].get(), &QTimer::timeout, soundData->sound[soundIndex], [this, soundData, soundIndex]()
            {
                emit sigStopSoundInternal(soundData, soundIndex);
            });
            soundData->timer[soundIndex]->start(duration);
        }
        soundData->sound[soundIndex]->play();
    }
}

void AudioManager::playDelayedSound(SoundData* soundData, qint32 soundIndex, bool stopOldestSound, qint32 duration)
{
    CONSOLE_PRINT_MODULE("Starting delayed sound", GameConsole::eDEBUG, GameConsole::eAudio);
    if (stopOldestSound)
    {
        AudioManager::stopOldestSound(soundData);
    }
    playSoundInternal(soundData, soundIndex, duration);
}

#endif
