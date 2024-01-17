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
        if (QFile::exists(folder + file))
        {
            spSoundData cache = MemoryManagement::create<SoundData>();
            cache->cacheUrl = cacheUrl;
            cache->m_maxUseCount = count;
            CONSOLE_PRINT_MODULE("Caching sound " + folder + file + " with amount " + QString::number(count), GameConsole::eDEBUG, GameConsole::eAudio);
            m_soundCaches.insert(file, cache);
        }
        else
        {
            CONSOLE_PRINT_MODULE("Unable to find sound " + folder + file + " with amount " + QString::number(count), GameConsole::eERROR, GameConsole::eResources);
        }
    }
#endif
}

#ifdef AUDIOSUPPORT
bool AudioManager::tryPlaySoundAtCachePosition(spSoundData &soundCache, qint32 i,
                                               QString &file, qint32 loops, qint32 delay, qreal sound, bool stopOldestSound, qint32 duration)
{
    bool started = false;
    if (!m_soundEffectData[i].timer.isActive() &&
        !m_soundEffectData[i].sound->isPlaying())
    {
        CONSOLE_PRINT_MODULE("Playing sound: " + file + " using cache sound " + QString::number(i), GameConsole::eDEBUG, GameConsole::eAudio);
        disconnect(&m_soundEffectData[i].timer, nullptr, nullptr, nullptr);
        disconnect(m_soundEffectData[i].sound.get(), nullptr, nullptr, nullptr);
        auto &soundItem = m_soundEffectData[i].sound;
        soundItem->setObjectName(file + QString::number(i));
        soundItem->setAudioDevice(m_audioDevice);
        soundItem->setSource(soundCache->cacheUrl);
        soundItem->setVolume(sound);
        soundItem->setMuted(false);
        if (loops < 0)
        {
            soundItem->setLoopCount(QSoundEffect::Infinite);
        }
        else
        {
            soundItem->setLoopCount(loops);
        }
        soundItem->setMuted(false);

        auto *pSoundCache = soundCache.get();
        // start play
        if (delay > 0)
        {
            connect(&m_soundEffectData[i].timer, &QTimer::timeout, soundItem.get(), [this, stopOldestSound, pSoundCache, i, duration]()
                    { playDelayedSound(pSoundCache, i, stopOldestSound, duration); });
            m_soundEffectData[i].timer.start(delay);
        }
        else
        {
            if (stopOldestSound)
            {
                AudioManager::stopOldestSound(pSoundCache);
            }
            playSoundInternal(pSoundCache, i, duration);
        }
        connect(
            soundItem.get(), &QSoundEffect::playingChanged, this, [this, pSoundCache, i]()
            {
            if (!m_soundEffectData[i].sound->isPlaying())
            {
                pSoundCache->m_usedSounds.removeAll(i);
                stopSoundInternal(i);
            } },
            Qt::QueuedConnection);

        started = true;
        soundCache->m_usedSounds.append(i);
    }
    return started;
}
#endif

#ifdef AUDIOSUPPORT
void AudioManager::stopSoundInternal(qint32 soundIndex)
{
    CONSOLE_PRINT_MODULE("Stopping sound at index " + QString::number(soundIndex), GameConsole::eDEBUG, GameConsole::eAudio);
    m_soundEffectData[soundIndex].timer.stop();
    //m_soundEffectData[soundIndex].sound->stop();
    m_soundEffectData[soundIndex].sound->setVolume(0);
    m_soundEffectData[soundIndex].sound->setMuted(true);
    m_toDeleteSounds.push_back(m_soundEffectData[soundIndex].sound);
    m_soundEffectData[soundIndex].sound = MemoryManagement::createNamedQObject<QSoundEffect>("QSoundEffect", this);
    m_soundEffectData[soundIndex].sound->setObjectName("SoundEffect" + QString::number(soundIndex));
    connect(m_soundEffectData[soundIndex].sound.get(), &QSoundEffect::statusChanged, this, [this, soundIndex]()
    {
        if (m_soundEffectData[soundIndex].sound->status() == QSoundEffect::Error)
        {
            CONSOLE_PRINT_MODULE("Error: Occured when playing sound: " + m_soundEffectData[soundIndex].sound->source().toString(), GameConsole::eDEBUG, GameConsole::eAudio);
        }
    },
    Qt::QueuedConnection);

    if (m_toDeleteSounds.size() > 20)
    {
        cleanUpSounds();
    }
}

void AudioManager::cleanUpSounds()
{
    qint32 i = 0;
    while (i < m_toDeleteSounds.size())
    {
        if (!m_toDeleteSounds[i]->isPlaying())
        {
            m_toDeleteSounds.erase(m_toDeleteSounds.cbegin() + i);
        }
        else
        {
            ++i;
        }
    }
}

void AudioManager::playDelayedSound(SoundData *soundData, qint32 soundIndex, bool stopOldestSound, qint32 duration)
{
    CONSOLE_PRINT_MODULE("Starting delayed sound", GameConsole::eDEBUG, GameConsole::eAudio);
    if (stopOldestSound)
    {
        AudioManager::stopOldestSound(soundData);
    }
    playSoundInternal(soundData, soundIndex, duration);
}

void AudioManager::playSoundInternal(SoundData *soundData, qint32 soundIndex, qint32 duration)
{
    if (soundIndex < MAX_PARALLEL_SOUNDS)
    {
        if (duration > 0)
        {
            connect(
                &m_soundEffectData[soundIndex].timer, &QTimer::timeout, m_soundEffectData[soundIndex].sound.get(), [this, soundIndex, soundData]()
                {
                soundData->m_usedSounds.removeAll(soundIndex);
                stopSoundInternal(soundIndex); },
                Qt::QueuedConnection);
            m_soundEffectData[soundIndex].timer.start(duration);
        }
        m_soundEffectData[soundIndex].sound->play();
    }
}

#endif
