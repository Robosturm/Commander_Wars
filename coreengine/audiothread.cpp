#include "coreengine/audiothread.h"
#include "coreengine/settings.h"
#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/interpreter.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QList>
#ifdef AUDIOSUPPORT
#include <QAudioDevice>
#endif

AudioThread::AudioThread(bool noAudio)
    :
#ifdef AUDIOSUPPORT
      m_audioOutput(this),
#endif
      m_positionChangedTimer(this),
      m_noAudio(noAudio)

{
    setObjectName("AudioThread");
    Interpreter::setCppOwnerShip(this);
    if (!m_noAudio)
    {
        connect(this, &AudioThread::sigPlayMusic,         this, &AudioThread::SlotPlayMusic, Qt::QueuedConnection);
        connect(this, &AudioThread::sigSetVolume,         this, &AudioThread::SlotSetVolume, Qt::QueuedConnection);
        connect(this, &AudioThread::sigAddMusic,          this, &AudioThread::SlotAddMusic, Qt::QueuedConnection);
        connect(this, &AudioThread::sigClearPlayList,     this, &AudioThread::SlotClearPlayList, Qt::BlockingQueuedConnection);
        connect(this, &AudioThread::sigPlayRandom,        this, &AudioThread::SlotPlayRandom, Qt::QueuedConnection);
        connect(this, &AudioThread::sigLoadFolder,        this, &AudioThread::SlotLoadFolder, Qt::BlockingQueuedConnection);
        connect(this, &AudioThread::sigPlaySound,         this, &AudioThread::SlotPlaySound, Qt::QueuedConnection);
        connect(this, &AudioThread::sigStopSound,         this, &AudioThread::SlotStopSound, Qt::QueuedConnection);
        connect(this, &AudioThread::sigStopAllSounds,     this, &AudioThread::SlotStopAllSounds, Qt::QueuedConnection);
        connect(this, &AudioThread::sigChangeAudioDevice, this, &AudioThread::SlotChangeAudioDevice, Qt::BlockingQueuedConnection);
        connect(this, &AudioThread::sigLoadNextAudioFile, this, &AudioThread::loadNextAudioFile, Qt::QueuedConnection);

#ifdef AUDIOSUPPORT
        connect(this, &AudioThread::sigDeleteSound,       this, &AudioThread::deleteSound, Qt::QueuedConnection);
        connect(this, &AudioThread::sigPlayDelayedSound,  this, &AudioThread::playDelayedSound, Qt::QueuedConnection);
#endif
    }
}

AudioThread::~AudioThread()
{
#ifdef AUDIOSUPPORT
    if (m_player != nullptr)
    {
        m_player->m_player.stop();
    }
    for (auto & cache : m_soundCaches)
    {
        for (qint32 i = 0; i < SoundData::MAX_SAME_SOUNDS; ++i)
        {
            if (cache->sound[i] != nullptr)
            {
                cache->timer[i]->stop();
                cache->sound[i]->stop();
                delete cache->sound[i];
                cache->sound[i] = nullptr;
                cache->timer[i].reset();
            }
        }
    }
    m_soundCaches.clear();
#endif
}

void AudioThread::initAudio()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        CONSOLE_PRINT("AudioThread::initAudio", Console::eDEBUG);
        const auto& value = Settings::getAudioOutput();
        if (value.typeId() == QMetaType::QString &&
            value.toString() == Settings::DEFAULT_AUDIODEVICE)
        {
            const QAudioDevice &defaultDeviceInfo = QMediaDevices::defaultAudioOutput();
            m_audioOutput.setDevice(defaultDeviceInfo);
        }
        else
        {
            m_audioDevice = value.value<QAudioDevice>();
        }
        m_audioOutput.setDevice(m_audioDevice);
        createPlayer();
        SlotSetVolume(static_cast<qint32>(static_cast<float>(Settings::getMusicVolume())));
        m_positionChangedTimer.setInterval(1);
        m_positionChangedTimer.setSingleShot(false);
        connect(&m_positionChangedTimer, &QTimer::timeout, this, [this]()
        {
            SlotCheckMusicEnded(m_player->m_player.position());
        });
        m_positionChangedTimer.start();
    }
#endif
}

void AudioThread::createSoundCache()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        QStringList searchFolders;
        searchFolders.append("resources/sounds/");
        searchFolders.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/sounds/");
        QStringList mods = Settings::getMods();
        for (const auto & mod : qAsConst(mods))
        {
            searchFolders.append(mod + "/sounds/");
        }
        for (qint32 i = searchFolders.size() - 1; i >= 0; --i)
        {
            QString folder = searchFolders[i];
            if (QFile::exists(folder + "res.xml"))
            {
                readSoundCacheFromXml(folder);
            }
            QStringList filter;
            filter << "*.wav";
            QDirIterator dirIter(folder, filter, QDir::Files, QDirIterator::Subdirectories);
            while (dirIter.hasNext())
            {
                dirIter.next();
                QString file = dirIter.fileName();
                if (!m_soundCaches.contains(file))
                {
                    fillSoundCache(SoundData::DEFAULT_CACHE_SIZE, folder, file);
                }
            }
        }
    }
#endif
}

void AudioThread::readSoundCacheFromXml(QString folder)
{
    if (!m_noAudio)
    {
        CONSOLE_PRINT("Loading sound cache from " + folder + "res.xml", Console::eDEBUG);
        QDomDocument document;
        QFile file(folder + "res.xml");
        file.open(QIODevice::ReadOnly);
        QString error;
        qint32 line;
        qint32 column;
        bool loaded = document.setContent(&file, &error, &line, &column);
        if (loaded)
        {
            auto rootElement = document.documentElement();
            auto node = rootElement.firstChild();
            while (!node.isNull())
            {
                while (node.isComment())
                {
                    node = node.nextSibling();
                }
                if (!node.isNull())
                {
                    if (node.nodeName() == "sound")
                    {
                        auto element = node.toElement();
                        QString file = element.attribute("file");
                        qint32 cacheSize = element.attribute("cachesize").toInt();
                        fillSoundCache(cacheSize, folder, file);
                    }
                }
                node = node.nextSibling();
            }
        }
        else
        {
            CONSOLE_PRINT("Unable to load: " + folder + "res.xml", Console::eERROR);
            CONSOLE_PRINT("Error: " + error + " at line " + QString::number(line) + " at column " + QString::number(column), Console::eERROR);
        }
    }
}

void AudioThread::createPlayer()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        CONSOLE_PRINT("AudioThread::createPlayer()", Console::eDEBUG);
        m_player.reset(new Player(this));
        m_player->m_player.setAudioOutput(&m_audioOutput);
        connect(&m_player->m_player, &QMediaPlayer::mediaStatusChanged, this, &AudioThread::mediaStatusChanged, Qt::QueuedConnection);
        connect(&m_player->m_player, &QMediaPlayer::playbackStateChanged, this, &AudioThread::mediaPlaybackStateChanged, Qt::QueuedConnection);
        connect(&m_player->m_player, &QMediaPlayer::errorOccurred, this, &AudioThread::reportReplayError, Qt::QueuedConnection);
    }
#endif
}

qint32 AudioThread::getSoundsBuffered()
{    
    qint32 count = 0;
#ifdef AUDIOSUPPORT
    for (const auto & cache : qAsConst(m_soundCaches))
    {
        for (const auto & sound : qAsConst(cache->sound))
        {
            if (sound != nullptr)
            {
                ++count;
            }
            else
            {
                break;
            }
        }
    }
#endif
    return count;
}

void AudioThread::changeAudioDevice(const QVariant& value)
{
    if (Mainapp::getInstance()->isMainThread())
    {
        SlotChangeAudioDevice(value);
    }
    else
    {
        emit sigChangeAudioDevice(value);
    }
}

void AudioThread::SlotChangeAudioDevice(const QVariant& value)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        m_audioDevice = value.value<QAudioDevice>();
        CONSOLE_PRINT("Changing to audio device: " + m_audioDevice.description(), Console::eDEBUG);
        m_audioOutput.setDevice(m_audioDevice);
        m_player->m_player.setAudioOutput(&m_audioOutput);
        m_soundCaches.clear();
        createSoundCache();
    }
#endif
}

void AudioThread::playMusic(qint32 File)
{
    emit sigPlayMusic(File);
}

void AudioThread::setVolume(qint32 value)
{
    emit sigSetVolume(value);
}

qint32 AudioThread::getVolume()
{
#ifdef AUDIOSUPPORT
    return m_audioOutput.volume();
#else
    return 0;
#endif
}

void AudioThread::addMusic(QString File, qint64 startPointMs, qint64 endPointMs)
{
    emit sigAddMusic(File, startPointMs, endPointMs);
}

void AudioThread::clearPlayList()
{
    if (Mainapp::getInstance()->isMainThread())
    {
        SlotClearPlayList();
    }
    else
    {
        emit sigClearPlayList();
    }
}

void AudioThread::playRandom()
{
    emit sigPlayRandom();
}

void AudioThread::playSound(QString file, qint32 loops, qint32 delay, float volume, bool stopOldestSound)
{
    emit sigPlaySound(file, loops, delay, volume, stopOldestSound);
}

void AudioThread::stopSound(QString file)
{
    emit sigStopSound(file);
}

void AudioThread::stopAllSounds()
{
    emit sigStopAllSounds();
}

void AudioThread::loadFolder(QString folder)
{
    if (Mainapp::getInstance()->isMainThread())
    {
        SlotLoadFolder(folder);
    }
    else
    {
        emit sigLoadFolder(folder);
    }
}

void AudioThread::SlotClearPlayList()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        CONSOLE_PRINT("AudioThread::SlotClearPlayList() start clearing", Console::eDEBUG);
        m_PlayListdata.clear();
        m_player->m_currentMedia = -1;
        m_player->m_nextMedia = -1;
        m_player->m_player.stop();
        m_player->m_player.setSource(QUrl());
        CONSOLE_PRINT("AudioThread::SlotClearPlayList() playlist cleared", Console::eDEBUG);
    }
#endif
}

void AudioThread::SlotPlayMusic(qint32 file)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        if (file >= 0 && file < m_PlayListdata.size())
        {
            CONSOLE_PRINT("Starting music for player: " + m_PlayListdata[file].m_file, Console::eDEBUG);
            m_player->m_player.stop();
            m_player->m_currentMedia = -1;
            m_player->m_nextMedia = -1;
            m_player->m_currentMedia = file;
            loadMediaForFile(m_PlayListdata[file].m_file);
            m_player->m_player.play();
        }
        else
        {
            CONSOLE_PRINT("AudioThread::SlotPlayMusic Trying to play unknown music index", Console::eERROR);
        }
    }
#endif
}

void AudioThread::loadMediaForFile(QString filePath)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        m_player->m_player.setPosition(0);
        m_player->m_player.setSource(QUrl());
        m_player->m_player.setSource(GlobalUtils::getUrlForFile(filePath));
    }
#endif
}

void AudioThread::SlotPlayRandom()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        CONSOLE_PRINT("AudioThread::SlotPlayRandom", Console::eDEBUG);
        qint32 size = m_PlayListdata.size();
        if (size > 0)
        {
            if (m_player->m_nextMedia < 0 || m_player->m_nextMedia > size)
            {
                m_player->m_player.stop();
                m_player->m_currentMedia = GlobalUtils::randIntBase(0, size - 1);
                loadMediaForFile(m_PlayListdata[m_player->m_currentMedia].m_file);
                m_player->m_player.play();
                CONSOLE_PRINT("Buffering music for player: " + m_PlayListdata[m_player->m_currentMedia].m_file, Console::eDEBUG);
            }
            else if (m_player->m_currentMedia == m_player->m_nextMedia)
            {
                qint32 loopPos = m_PlayListdata[m_player->m_currentMedia].m_startpointMs;
                if (loopPos < 0)
                {
                    loopPos = 0;
                }
                if (m_player->m_player.playbackState() != QMediaPlayer::PlayingState)
                {
                    loadMediaForFile(m_PlayListdata[m_player->m_currentMedia].m_file);
                    m_player->m_player.setPosition(loopPos);
                    m_player->m_player.play();
                }
                else
                {
                    m_player->m_player.setPosition(loopPos);
                }
                CONSOLE_PRINT("Seeking music for player: " + m_PlayListdata[m_player->m_currentMedia].m_file + " to " + QString::number(loopPos), Console::eDEBUG);
            }
            else
            {
                m_player->m_player.stop();
                m_player->m_currentMedia = m_player->m_nextMedia;
                loadMediaForFile(m_PlayListdata[m_player->m_currentMedia].m_file);
                m_player->m_player.play();
                CONSOLE_PRINT("Buffering music for player: " + m_PlayListdata[m_player->m_currentMedia].m_file, Console::eDEBUG);
            }
            m_player->m_nextMedia = GlobalUtils::randIntBase(0, size - 1);
        }
        else
        {
            CONSOLE_PRINT("Playlist is empty", Console::eDEBUG);
        }
    }
#endif
}

void AudioThread::SlotSetVolume(qint32 value)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        qreal sound = (static_cast<qreal>(value) / 100.0 *
                       static_cast<qreal>(Settings::getTotalVolume()) / 100.0);
        qreal volume = QAudio::convertVolume(sound, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
        if (Settings::getMuted())
        {
            volume = 0.0f;
        }
        CONSOLE_PRINT("Setting volume to : " + QString::number(volume), Console::eDEBUG);
        m_audioOutput.setVolume(volume);
    }
#endif
}

void AudioThread::SlotAddMusic(QString file, qint64 startPointMs, qint64 endPointMs)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        QString currentPath = file;
        if (!Settings::getUserPath().isEmpty())
        {
            currentPath = Settings::getUserPath() + "/" + file;
        }
        if (!QFile::exists(currentPath))
        {
            CONSOLE_PRINT("Unable to locate music file: " + currentPath + " using compiled path.", Console::eDEBUG);
            currentPath = oxygine::Resource::RCC_PREFIX_PATH + file;
        }
        if (QFile::exists(currentPath))
        {
            m_player->m_player.stop();
            addMusicToPlaylist(currentPath, startPointMs, endPointMs);
        }
        else
        {
            CONSOLE_PRINT("Unable to locate music file: " + currentPath, Console::eERROR);
        }
    }
#endif
}

#ifdef AUDIOSUPPORT
void AudioThread::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (!m_noAudio)
    {
        CONSOLE_PRINT("Media status changed for player to " + QString::number(status), Console::eDEBUG);
        switch (status)
        {
            case QMediaPlayer::NoMedia:
            {
                break;
            }
            case QMediaPlayer::EndOfMedia:
            {
                m_player->m_currentMedia = -1;
                CONSOLE_PRINT("Music stopped at position " + QString::number(m_player->m_player.position()), Console::eDEBUG);
                // shuffle through loaded media
                SlotPlayRandom();
                break;
            }
            case QMediaPlayer::InvalidMedia:
            {
                CONSOLE_PRINT("Invalid media detected for player", Console::eWARNING);
                if (m_player->m_currentMedia < m_PlayListdata.size())
                {
                    CONSOLE_PRINT("Unable to play: " +  m_PlayListdata[m_player->m_currentMedia].m_file, Console::eWARNING);
                }
            }
            default:
            {
                break;
            }
        }
    }
}
#endif
void AudioThread::loadNextAudioFile()
{
    SlotPlayRandom();
}

#ifdef AUDIOSUPPORT
void AudioThread::mediaPlaybackStateChanged(QMediaPlayer::PlaybackState newState)
{
    CONSOLE_PRINT("Playback state changed to " + QString::number(newState) + " for player", Console::eDEBUG);
}
#endif

void AudioThread::SlotLoadFolder(QString folder)
{
#ifdef AUDIOSUPPORT
    QStringList loadedSounds;
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        loadMusicFolder(Settings::getUserPath() + "/" + Settings::getMods().at(i) + "/" + folder, loadedSounds);
        loadMusicFolder(QString(oxygine::Resource::RCC_PREFIX_PATH) +  "/" + Settings::getMods().at(i) + "/" + folder, loadedSounds);
    }
    if (m_loadBaseGameFolders)
    {
        loadMusicFolder(folder, loadedSounds);
        loadMusicFolder(oxygine::Resource::RCC_PREFIX_PATH + folder, loadedSounds);
    }
#endif
}

void AudioThread::loadMusicFolder(QString folder, QStringList& loadedSounds)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        QDir directory(folder);
        if (directory.exists())
        {
            QStringList filter("*.mp3");
            QStringList files = directory.entryList(filter);
            for (const auto& file : files)
            {
                if (!loadedSounds.contains(file) && QFile::exists(folder + '/' + file))
                {
                    addMusicToPlaylist(folder + '/' + file);
                    loadedSounds.append(file);
                }
            }
        }
        else
        {
            CONSOLE_PRINT("Unable to locate music folder: " + folder, Console::eDEBUG);
        }
    }
#endif
}

void AudioThread::addMusicToPlaylist(QString file, qint64 startPointMs, qint64 endPointMs)
{
    CONSOLE_PRINT("Adding " + file + " to play list", Console::eDEBUG);
#ifdef AUDIOSUPPORT
    m_PlayListdata.append(PlaylistData(file, startPointMs, endPointMs));
#endif
}

bool AudioThread::getLoadBaseGameFolders() const
{
    return m_loadBaseGameFolders;
}

void AudioThread::setLoadBaseGameFolders(bool loadBaseGameFolders)
{
    m_loadBaseGameFolders = loadBaseGameFolders;
}

void AudioThread::SlotCheckMusicEnded(qint64 duration)
{
#ifdef AUDIOSUPPORT
    if (m_player->m_currentMedia >= 0 && m_player->m_currentMedia < m_PlayListdata.size())
    {
        qint64 loopPos = m_PlayListdata[m_player->m_currentMedia].m_endpointMs;
        if (loopPos > 0 && duration >= loopPos && m_player->m_currentMedia == m_player->m_nextMedia)
        {
            CONSOLE_PRINT("Player reached loop end desired position " + QString::number(loopPos) + " actual loop position " + QString::number(duration) + " for player", Console::eDEBUG);
            // shuffle load new media
            SlotPlayRandom();
        }
    }
#endif
}

#ifdef AUDIOSUPPORT
void AudioThread::reportReplayError(QMediaPlayer::Error error, const QString &errorString)
{
    CONSOLE_PRINT("Error in player: " + errorString, Console::eERROR);
    switch (error)
    {
        case QMediaPlayer::Error::FormatError:
        {
            CONSOLE_PRINT("Audio playback error: Wrong audio format provided.", Console::eERROR);
            break;
        }
        case QMediaPlayer::Error::NetworkError:
        {
            CONSOLE_PRINT("Audio playback error: Network access error to audio file.", Console::eERROR);
            break;
        }
        case QMediaPlayer::Error::ResourceError:
        {
            CONSOLE_PRINT("Audio playback error: Media ressouce file could not be resolved.", Console::eERROR);
            break;
        }
        case QMediaPlayer::Error::AccessDeniedError:
        {
            CONSOLE_PRINT("Audio playback error: Access denied.", Console::eERROR);
            break;
        }
        case QMediaPlayer::Error::NoError:
        {
            break;
        }
        default:
        {
            CONSOLE_PRINT("Audio playback error: Service for replaying the requested audio format is missing.", Console::eERROR);
            break;
        }
    }
}
#endif

void AudioThread::SlotPlaySound(QString file, qint32 loops, qint32 delay, float volume, bool stopOldestSound)
{
#ifdef AUDIOSUPPORT
    if (Settings::getMuted() || m_noAudio)
    {
        return;
    }
    qreal sound = (static_cast<qreal>(Settings::getSoundVolume()) / 100.0 *
                   static_cast<qreal>(Settings::getTotalVolume()) / 100.0) * volume;
    sound = QAudio::convertVolume(sound, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
    if (sound > 0)
    {
        if (m_soundCaches.contains(file))
        {
            auto & soundCache = m_soundCaches[file];
            bool started = false;
            for (qint32 i = soundCache->nextSoundToUse; i < SoundData::MAX_SAME_SOUNDS; ++i)
            {
                if (soundCache->timer[i].get() == nullptr)
                {
                    break;
                }
                else if (tryPlaySoundAtCachePosition(soundCache, i,
                                                     file, loops, delay, sound,
                                                     stopOldestSound))
                {
                    started = true;
                    break;
                }
            }
            if (!started)
            {
                for (qint32 i = 0; i < soundCache->nextSoundToUse; ++i)
                {
                    if (tryPlaySoundAtCachePosition(soundCache, i,
                                                    file, loops, delay, sound,
                                                    stopOldestSound))
                    {
                        started = true;
                        break;
                    }
                }
                if (!started)
                {
                    CONSOLE_PRINT("no free cached sound found.", Console::eDEBUG);
                }
            }
        }
        else
        {
            CONSOLE_PRINT("Unable to locate sound: " + file, Console::eERROR);
        }
    }
#endif
}

void AudioThread::SlotStopAllSounds()
{
#ifdef AUDIOSUPPORT
    CONSOLE_PRINT("Stopping all sounds", Console::eDEBUG);
    for (auto & soundCache : m_soundCaches)
    {
        for (qint32 i = 0; i < SoundData::MAX_SAME_SOUNDS; ++i)
        {
            stopSoundAtIndex(soundCache.get(), i);
        }
    }
#endif
}

void AudioThread::SlotStopSound(QString file)
{
#ifdef AUDIOSUPPORT
    if (m_soundCaches.contains(file))
    {
        CONSOLE_PRINT("Stopping sound " + file, Console::eDEBUG);
        auto & soundCache = m_soundCaches[file];
        for (qint32 i = 0; i < SoundData::MAX_SAME_SOUNDS; ++i)
        {
            stopSoundAtIndex(soundCache.get(), i);
        }
    }
#endif
}

#ifdef AUDIOSUPPORT
bool AudioThread::stopSoundAtIndex(SoundData* soundData, qint32 index)
{
    bool stopped = false;
    if (soundData->sound[index] != nullptr &&
        soundData->sound[index]->isPlaying())
    {
        stopSound(soundData, index);
        stopped = true;
    }
    if (soundData->timer[index] != nullptr &&
        soundData->timer[index]->isActive())
    {
        soundData->timer[index]->stop();
        stopped = true;
    }
    return stopped;
}

void AudioThread::stopOldestSound(SoundData* soundData)
{
    bool stopped = false;
    for (qint32 i = soundData->nextSoundToUse; i < SoundData::MAX_SAME_SOUNDS; ++i)
    {
        stopped = stopSoundAtIndex(soundData, i);
        if (stopped)
        {
            break;
        }
    }
    if (!stopped)
    {
        for (qint32 i = 0; i < soundData->nextSoundToUse; ++i)
        {
            stopped = stopSoundAtIndex(soundData, i);
            if (stopped)
            {
                break;
            }
        }
    }
}
#endif
