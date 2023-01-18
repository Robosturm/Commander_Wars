#include "coreengine/audiomanager.h"
#include "coreengine/gameconsole.h"
#include "coreengine/globalutils.h"

#ifdef AUDIOSUPPORT
#include <QAudioDevice>
#endif

// code loads and plays sounds on demand slower replay but compatible with os's
// which create a sound pipe during the sound effect creation which only allow
// a limited amount of parallel open sound pipes.

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
    if (soundCache->sound[i] == nullptr &&
        !soundCache->timer[i]->isActive())
    {
        CONSOLE_PRINT_MODULE("Playing sound: " + file + " using cache sound " + QString::number(i), GameConsole::eDEBUG, GameConsole::eAudio);
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
            disconnect(soundCache->timer[i].get(), nullptr, nullptr, nullptr);
            connect(soundCache->timer[i].get(), &QTimer::timeout, soundItem, [this, stopOldestSound, pSoundCache, i, duration]()
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
        connect(soundCache->sound[i], &QSoundEffect::playingChanged, this, [this, pSoundCache, i]()
        {
            emit sigDeleteSound(pSoundCache, i);
        }, Qt::QueuedConnection);
        connect(soundCache->sound[i], &QSoundEffect::statusChanged, this, [pSoundCache, i]()
        {
            if (pSoundCache->sound[i] != nullptr &&
                pSoundCache->sound[i]->status() == QSoundEffect::Error)
            {
                CONSOLE_PRINT_MODULE("Error: Occured when playing sound: " + pSoundCache->cacheUrl.toString(), GameConsole::eDEBUG, GameConsole::eAudio);
            }
        }, Qt::QueuedConnection);
        started = true;
        soundCache->nextSoundToUse = i + 1;
    }
    return started;
}
#endif

#ifdef AUDIOSUPPORT
void AudioManager::stopSoundInternal(SoundData* soundData, qint32 soundIndex)
{
    CONSOLE_PRINT_MODULE("Stopping sound at index " + QString::number(soundIndex), GameConsole::eDEBUG, GameConsole::eAudio);
    if (soundData->sound[soundIndex] != nullptr)
    {
        soundData->timer[soundIndex]->stop();
        soundData->sound[soundIndex]->stop();
        soundData->sound[soundIndex]->deleteLater();
        soundData->sound[soundIndex] = nullptr;
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

void AudioManager::deleteSound(SoundData* soundData, qint32 soundIndex)
{
    if (soundData->sound[soundIndex] != nullptr &&
        !soundData->sound[soundIndex]->isPlaying())
    {
        CONSOLE_PRINT_MODULE("Stopping sound on playing changed.", GameConsole::eDEBUG, GameConsole::eAudio);
        soundData->sound[soundIndex]->stop();
        soundData->sound[soundIndex]->deleteLater();
        soundData->sound[soundIndex] = nullptr;
    }
}
#endif

