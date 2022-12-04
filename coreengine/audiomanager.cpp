#include "coreengine/audiomanager.h"
#include "coreengine/settings.h"
#include "coreengine/mainapp.h"
#include "coreengine/gameconsole.h"
#include "coreengine/interpreter.h"
#include "coreengine/globalutils.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QList>
#ifdef AUDIOSUPPORT
#include <QAudioDevice>
#endif

AudioManager::AudioManager(bool noAudio)
    :
#ifdef AUDIOSUPPORT
      m_audioOutput(this),
#endif
      m_positionChangedTimer(this),
      m_noAudio(noAudio)

{
#ifdef GRAPHICSUPPORT
    setObjectName("AudioThread");
#endif
    Interpreter::setCppOwnerShip(this);
    if (!m_noAudio)
    {
        connect(this, &AudioManager::sigPlayMusic,         this, &AudioManager::SlotPlayMusic, Qt::QueuedConnection);
        connect(this, &AudioManager::sigSetVolume,         this, &AudioManager::SlotSetVolume, Qt::QueuedConnection);
        connect(this, &AudioManager::sigAddMusic,          this, &AudioManager::SlotAddMusic, Qt::QueuedConnection);
        connect(this, &AudioManager::sigClearPlayList,     this, &AudioManager::SlotClearPlayList, Qt::QueuedConnection);
        connect(this, &AudioManager::sigPlayRandom,        this, &AudioManager::SlotPlayRandom, Qt::QueuedConnection);
        connect(this, &AudioManager::sigLoadFolder,        this, &AudioManager::SlotLoadFolder, Qt::QueuedConnection);
        connect(this, &AudioManager::sigPlaySound,         this, &AudioManager::SlotPlaySound, Qt::QueuedConnection);
        connect(this, &AudioManager::sigStopSound,         this, &AudioManager::SlotStopSound, Qt::QueuedConnection);
        connect(this, &AudioManager::sigStopAllSounds,     this, &AudioManager::SlotStopAllSounds, Qt::QueuedConnection);
        connect(this, &AudioManager::sigChangeAudioDevice, this, &AudioManager::SlotChangeAudioDevice, Qt::QueuedConnection);
        connect(this, &AudioManager::sigLoadNextAudioFile, this, &AudioManager::loadNextAudioFile, Qt::QueuedConnection);

#ifdef AUDIOSUPPORT
        connect(this, &AudioManager::sigDeleteSound,       this, &AudioManager::deleteSound, Qt::QueuedConnection);
        connect(this, &AudioManager::sigPlayDelayedSound,  this, &AudioManager::playDelayedSound, Qt::QueuedConnection);
#endif
    }
}

AudioManager::~AudioManager()
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
                cache->sound[i]->deleteLater();
                cache->sound[i] = nullptr;
                cache->timer[i].reset();
            }
        }
    }
    m_soundCaches.clear();
#endif
}

void AudioManager::initAudio()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        CONSOLE_PRINT("AudioThread::initAudio", GameConsole::eDEBUG);
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

void AudioManager::createSoundCache()
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

void AudioManager::readSoundCacheFromXml(QString folder)
{
    if (!m_noAudio)
    {
        CONSOLE_PRINT("Loading sound cache from " + folder + "res.xml", GameConsole::eDEBUG);
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
            CONSOLE_PRINT("Unable to load: " + folder + "res.xml", GameConsole::eERROR);
            CONSOLE_PRINT("Error: " + error + " at line " + QString::number(line) + " at column " + QString::number(column), GameConsole::eERROR);
        }
    }
}

void AudioManager::createPlayer()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        CONSOLE_PRINT("AudioThread::createPlayer()", GameConsole::eDEBUG);
        m_player.reset(new Player(this));
        m_player->m_player.setAudioOutput(&m_audioOutput);
        connect(&m_player->m_player, &QMediaPlayer::mediaStatusChanged, this, &AudioManager::mediaStatusChanged, Qt::QueuedConnection);
        connect(&m_player->m_player, &QMediaPlayer::playbackStateChanged, this, &AudioManager::mediaPlaybackStateChanged, Qt::QueuedConnection);
        connect(&m_player->m_player, &QMediaPlayer::errorOccurred, this, &AudioManager::reportReplayError, Qt::QueuedConnection);
    }
#endif
}

qint32 AudioManager::getSoundsBuffered()
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

void AudioManager::changeAudioDevice(const QVariant& value)
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

void AudioManager::SlotChangeAudioDevice(const QVariant& value)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        m_audioDevice = value.value<QAudioDevice>();
        CONSOLE_PRINT("Changing to audio device: " + m_audioDevice.description(), GameConsole::eDEBUG);
        m_audioOutput.setDevice(m_audioDevice);
        m_player->m_player.setAudioOutput(&m_audioOutput);
        m_soundCaches.clear();
        createSoundCache();
        m_player->m_player.stop();
        SlotPlayRandom();
    }
#endif
}

void AudioManager::playMusic(qint32 File)
{
    emit sigPlayMusic(File);
}

void AudioManager::setVolume(qint32 value)
{
    emit sigSetVolume(value);
}

qint32 AudioManager::getVolume()
{
#ifdef AUDIOSUPPORT
    return m_audioOutput.volume();
#else
    return 0;
#endif
}

void AudioManager::addMusic(QString File, qint64 startPointMs, qint64 endPointMs)
{
    emit sigAddMusic(File, startPointMs, endPointMs);
}

void AudioManager::clearPlayList()
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

void AudioManager::playRandom()
{
    emit sigPlayRandom();
}

void AudioManager::playSound(QString file, qint32 loops, qint32 delay, float volume, bool stopOldestSound)
{
    emit sigPlaySound(file, loops, delay, volume, stopOldestSound);
}

void AudioManager::stopSound(QString file)
{
    emit sigStopSound(file);
}

void AudioManager::stopAllSounds()
{
    emit sigStopAllSounds();
}

void AudioManager::loadFolder(QString folder)
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

void AudioManager::SlotClearPlayList()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        CONSOLE_PRINT("AudioThread::SlotClearPlayList() start clearing", GameConsole::eDEBUG);
        m_PlayListdata.clear();
        m_player->m_currentMedia = -1;
        m_player->m_nextMedia = -1;
        m_player->m_player.stop();
        m_player->m_player.setSource(QUrl());
        CONSOLE_PRINT("AudioThread::SlotClearPlayList() playlist cleared", GameConsole::eDEBUG);
    }
#endif
}

void AudioManager::SlotPlayMusic(qint32 file)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio && !Settings::getMuted())
    {
        if (file >= 0 && file < m_PlayListdata.size())
        {
            CONSOLE_PRINT("Starting music for player: " + m_PlayListdata[file].m_file, GameConsole::eDEBUG);
            m_player->m_player.stop();
            m_player->m_currentMedia = -1;
            m_player->m_nextMedia = -1;
            m_player->m_currentMedia = file;
            loadMediaForFile(m_PlayListdata[file].m_file);
            m_player->m_player.play();
        }
        else
        {
            CONSOLE_PRINT("AudioThread::SlotPlayMusic Trying to play unknown music index", GameConsole::eERROR);
        }
    }
#endif
}

void AudioManager::loadMediaForFile(QString filePath)
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

void AudioManager::SlotPlayRandom()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio && !Settings::getMuted())
    {
        CONSOLE_PRINT("AudioThread::SlotPlayRandom", GameConsole::eDEBUG);
        qint32 size = m_PlayListdata.size();
        if (size > 0)
        {
            if (m_player->m_nextMedia < 0 || m_player->m_nextMedia > size)
            {
                m_player->m_player.stop();
                m_player->m_currentMedia = GlobalUtils::randIntBase(0, size - 1);
                loadMediaForFile(m_PlayListdata[m_player->m_currentMedia].m_file);
                m_player->m_player.play();
                CONSOLE_PRINT("Buffering music for player: " + m_PlayListdata[m_player->m_currentMedia].m_file, GameConsole::eDEBUG);
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
                CONSOLE_PRINT("Seeking music for player: " + m_PlayListdata[m_player->m_currentMedia].m_file + " to " + QString::number(loopPos), GameConsole::eDEBUG);
            }
            else
            {
                m_player->m_player.stop();
                m_player->m_currentMedia = m_player->m_nextMedia;
                loadMediaForFile(m_PlayListdata[m_player->m_currentMedia].m_file);
                m_player->m_player.play();
                CONSOLE_PRINT("Buffering music for player: " + m_PlayListdata[m_player->m_currentMedia].m_file, GameConsole::eDEBUG);
            }
            m_player->m_nextMedia = GlobalUtils::randIntBase(0, size - 1);
        }
        else
        {
            CONSOLE_PRINT("Playlist is empty", GameConsole::eDEBUG);
        }
    }
#endif
}

void AudioManager::SlotSetVolume(qint32 value)
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
            m_player->m_player.stop();
        }
        else
        {
            if (m_player->m_player.playbackState() != QMediaPlayer::PlayingState)
            {
                SlotPlayRandom();
            }
        }
        CONSOLE_PRINT("Setting volume to : " + QString::number(volume), GameConsole::eDEBUG);
        m_audioOutput.setVolume(volume);
    }
#endif
}

void AudioManager::SlotAddMusic(QString file, qint64 startPointMs, qint64 endPointMs)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        QString currentPath = file;
        currentPath = file;
        if (!QFile::exists(currentPath))
        {
            currentPath = Settings::getUserPath() + file;
            if (!QFile::exists(currentPath))
            {
                CONSOLE_PRINT("Unable to locate music file: " + currentPath + " using compiled path.", GameConsole::eDEBUG);
                currentPath = oxygine::Resource::RCC_PREFIX_PATH + file;
            }
        }
        if (QFile::exists(currentPath))
        {
            m_player->m_player.stop();
            addMusicToPlaylist(currentPath, startPointMs, endPointMs);
        }
        else
        {
            CONSOLE_PRINT("Unable to locate music file: " + currentPath, GameConsole::eERROR);
        }
    }
#endif
}

#ifdef AUDIOSUPPORT
void AudioManager::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (!m_noAudio)
    {
        CONSOLE_PRINT("Media status changed for player to " + QString::number(status), GameConsole::eDEBUG);
        switch (status)
        {
            case QMediaPlayer::NoMedia:
            {
                break;
            }
            case QMediaPlayer::EndOfMedia:
            {
                m_player->m_currentMedia = -1;
                CONSOLE_PRINT("Music stopped at position " + QString::number(m_player->m_player.position()), GameConsole::eDEBUG);
                // shuffle through loaded media
                SlotPlayRandom();
                break;
            }
            case QMediaPlayer::InvalidMedia:
            {
                CONSOLE_PRINT("Invalid media detected for player", GameConsole::eWARNING);
                if (m_player->m_currentMedia < m_PlayListdata.size())
                {
                    CONSOLE_PRINT("Unable to play: " +  m_PlayListdata[m_player->m_currentMedia].m_file, GameConsole::eWARNING);
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
void AudioManager::loadNextAudioFile()
{
    SlotPlayRandom();
}

#ifdef AUDIOSUPPORT
void AudioManager::mediaPlaybackStateChanged(QMediaPlayer::PlaybackState newState)
{
    CONSOLE_PRINT("Playback state changed to " + QString::number(newState) + " for player", GameConsole::eDEBUG);
}
#endif

void AudioManager::SlotLoadFolder(QString folder)
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

void AudioManager::loadMusicFolder(QString folder, QStringList& loadedSounds)
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
            CONSOLE_PRINT("Unable to locate music folder: " + folder, GameConsole::eDEBUG);
        }
    }
#endif
}

void AudioManager::addMusicToPlaylist(QString file, qint64 startPointMs, qint64 endPointMs)
{
    CONSOLE_PRINT("Adding " + file + " to play list", GameConsole::eDEBUG);
#ifdef AUDIOSUPPORT
    m_PlayListdata.append(PlaylistData(file, startPointMs, endPointMs));
#endif
}

bool AudioManager::getLoadBaseGameFolders() const
{
    return m_loadBaseGameFolders;
}

void AudioManager::setLoadBaseGameFolders(bool loadBaseGameFolders)
{
    m_loadBaseGameFolders = loadBaseGameFolders;
}

void AudioManager::SlotCheckMusicEnded(qint64 duration)
{
#ifdef AUDIOSUPPORT
    if (m_player->m_currentMedia >= 0 && m_player->m_currentMedia < m_PlayListdata.size())
    {
        qint64 loopPos = m_PlayListdata[m_player->m_currentMedia].m_endpointMs;
        if (loopPos > 0 && duration >= loopPos && m_player->m_currentMedia == m_player->m_nextMedia)
        {
            CONSOLE_PRINT("Player reached loop end desired position " + QString::number(loopPos) + " actual loop position " + QString::number(duration) + " for player", GameConsole::eDEBUG);
            // shuffle load new media
            SlotPlayRandom();
        }
    }
#endif
}

#ifdef AUDIOSUPPORT
void AudioManager::reportReplayError(QMediaPlayer::Error error, const QString &errorString)
{
    CONSOLE_PRINT("Error in player: " + errorString, GameConsole::eERROR);
    switch (error)
    {
        case QMediaPlayer::Error::FormatError:
        {
            CONSOLE_PRINT("Audio playback error: Wrong audio format provided.", GameConsole::eERROR);
            break;
        }
        case QMediaPlayer::Error::NetworkError:
        {
            CONSOLE_PRINT("Audio playback error: Network access error to audio file.", GameConsole::eERROR);
            break;
        }
        case QMediaPlayer::Error::ResourceError:
        {
            CONSOLE_PRINT("Audio playback error: Media ressouce file could not be resolved.", GameConsole::eERROR);
            break;
        }
        case QMediaPlayer::Error::AccessDeniedError:
        {
            CONSOLE_PRINT("Audio playback error: Access denied.", GameConsole::eERROR);
            break;
        }
        case QMediaPlayer::Error::NoError:
        {
            break;
        }
        default:
        {
            CONSOLE_PRINT("Audio playback error: Service for replaying the requested audio format is missing.", GameConsole::eERROR);
            break;
        }
    }
}
#endif

void AudioManager::SlotPlaySound(QString file, qint32 loops, qint32 delay, float volume, bool stopOldestSound)
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
                    CONSOLE_PRINT("no free cached sound found.", GameConsole::eDEBUG);
                }
            }
        }
        else
        {
            CONSOLE_PRINT("Unable to locate sound: " + file, GameConsole::eERROR);
        }
    }
#endif
}

void AudioManager::SlotStopAllSounds()
{
#ifdef AUDIOSUPPORT
    CONSOLE_PRINT("Stopping all sounds", GameConsole::eDEBUG);
    for (auto & soundCache : m_soundCaches)
    {
        for (qint32 i = 0; i < SoundData::MAX_SAME_SOUNDS; ++i)
        {
            stopSoundAtIndex(soundCache.get(), i);
        }
    }
#endif
}

void AudioManager::SlotStopSound(QString file)
{
#ifdef AUDIOSUPPORT
    if (m_soundCaches.contains(file))
    {
        CONSOLE_PRINT("Stopping sound " + file, GameConsole::eDEBUG);
        auto & soundCache = m_soundCaches[file];
        for (qint32 i = 0; i < SoundData::MAX_SAME_SOUNDS; ++i)
        {
            stopSoundAtIndex(soundCache.get(), i);
        }
    }
#endif
}

#ifdef AUDIOSUPPORT
bool AudioManager::stopSoundAtIndex(SoundData* soundData, qint32 index)
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

void AudioManager::stopOldestSound(SoundData* soundData)
{
    CONSOLE_PRINT("Stopping oldest sound sound of " + soundData->cacheUrl.toString(), GameConsole::eDEBUG);
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
