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

SoundData::SoundData()
{
#ifdef GRAPHICSUPPORT
    setObjectName("SoundData");
#endif
    Interpreter::setCppOwnerShip(this);
}

AudioManager::AudioManager(bool noAudio)
    :
#ifdef AUDIOSUPPORT
      m_audioOutput(this),
#endif
      m_noAudio(noAudio)

{
#ifdef GRAPHICSUPPORT
    setObjectName("AudioThread");
#endif
    Interpreter::setCppOwnerShip(this);
    if (!m_noAudio)
    {
#ifdef AUDIOSUPPORT
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
        connect(this, &AudioManager::sigSetMuteInternal,   this, &AudioManager::slotSetMuteInternal, Qt::QueuedConnection);
        connect(this, &AudioManager::sigContinueMusic,   this, &AudioManager::SlotContinueMusic, Qt::QueuedConnection);
        connect(this, &AudioManager::sigClearMusicPositions,   this, &AudioManager::SlotClearMusicPositions, Qt::QueuedConnection);

        // sync startup and stop signals and slots
        connect(this, &AudioManager::sigInitAudio,         this, &AudioManager::initAudio, Qt::BlockingQueuedConnection);
        connect(this, &AudioManager::sigStopAudio,         this, &AudioManager::stopAudio, Qt::BlockingQueuedConnection);
        connect(this, &AudioManager::sigCreateSoundCache,  this, &AudioManager::createSoundCache, Qt::BlockingQueuedConnection);
        for (qint32 i = 0; i < MAX_PARALLEL_SOUNDS; ++i)
        {
            m_soundEffectData[i].timer.setObjectName("SoundEffect" + QString::number(i));
            m_soundEffectData[i].timer.setSingleShot(true);
            m_soundEffectData[i].sound->setObjectName("SoundEffect" + QString::number(i));
            connect(m_soundEffectData[i].sound.get(), &QSoundEffect::statusChanged, this, [this, i]()
            {
                if (m_soundEffectData[i].sound->status() == QSoundEffect::Error)
                {
                    CONSOLE_PRINT_MODULE("Error: Occured when playing sound: " + m_soundEffectData[i].sound->source().toString(), GameConsole::eDEBUG, GameConsole::eAudio);
                }
            }, Qt::QueuedConnection);
        }
#endif
    }
}

void AudioManager::stopAudio()
{
#ifdef AUDIOSUPPORT
    if (Mainapp::getInstance()->isAudioThread())
    {
        if (m_player != nullptr)
        {
            m_player->m_player.disconnect();
            m_player->m_player.stop();
        }
        m_soundCaches.clear();
        disconnect();
    }
    else
    {
        emit sigStopAudio();
    }
#endif
}

void AudioManager::initAudio()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        if (Mainapp::getInstance()->isAudioThread())
        {
            CONSOLE_PRINT_MODULE("AudioThread::initAudio", GameConsole::eDEBUG, GameConsole::eAudio);
            const auto value = Settings::getInstance()->getAudioOutput();
            if (value.typeId() == QMetaType::QString &&
                value.toString() == Settings::getInstance()->DEFAULT_AUDIODEVICE)
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
            SlotSetVolume(static_cast<qint32>(static_cast<float>(Settings::getInstance()->getMusicVolume())));
            connect(&m_player->m_player, &QMediaPlayer::positionChanged, this, &AudioManager::SlotCheckMusicEnded);
        }
        else
        {
            emit sigInitAudio();

        }
    }
#endif
}

void AudioManager::createSoundCache()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        if (Mainapp::getInstance()->isAudioThread())
        {
            QStringList searchFolders;
            searchFolders.append(Settings::getInstance()->getUserPath() + "resources/sounds/");
            searchFolders.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/sounds/");
            QStringList mods = Settings::getInstance()->getMods();
            for (const auto & mod : std::as_const(mods))
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
        else
        {
            emit sigCreateSoundCache();

        }
    }
#endif
}

void AudioManager::readSoundCacheFromXml(QString folder)
{
    if (!m_noAudio)
    {
        CONSOLE_PRINT_MODULE("Loading sound cache from " + folder + "res.xml", GameConsole::eDEBUG, GameConsole::eAudio);
        QDomDocument document;
        QFile file(folder + "res.xml");
        if (file.open(QIODevice::ReadOnly))
        {
            auto result = document.setContent(&file);
            if (result)
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
                CONSOLE_PRINT_MODULE("Unable to load: " + folder + "res.xml", GameConsole::eERROR, GameConsole::eAudio);
                CONSOLE_PRINT_MODULE("Error: " + result.errorMessage + " at line " + QString::number(result.errorLine) + " at column " + QString::number(result.errorColumn), GameConsole::eERROR, GameConsole::eAudio);
            }
        }
    }
}

void AudioManager::createPlayer()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        CONSOLE_PRINT_MODULE("AudioThread::createPlayer()", GameConsole::eDEBUG, GameConsole::eAudio);
        m_player = MemoryManagement::create<Player>(this);
        m_player->m_player.setAudioOutput(&m_audioOutput);
        m_player->m_fileStream.setBuffer(&m_player->m_content);
        connect(&m_player->m_player, &QMediaPlayer::mediaStatusChanged, this, &AudioManager::mediaStatusChanged, Qt::QueuedConnection);
        connect(&m_player->m_player, &QMediaPlayer::playbackStateChanged, this, &AudioManager::mediaPlaybackStateChanged, Qt::QueuedConnection);
        connect(&m_player->m_player, &QMediaPlayer::errorOccurred, this, &AudioManager::reportReplayError, Qt::QueuedConnection);
    }
#endif
}

void AudioManager::changeAudioDevice(const QVariant& value)
{
    if (Mainapp::getInstance()->isAudioThread())
    {
        SlotChangeAudioDevice(value);
    }
    else
    {
        emit sigChangeAudioDevice(value);
    }
}

void AudioManager::SlotChangeAudioDevice(const QVariant value)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        m_audioDevice = value.value<QAudioDevice>();
        CONSOLE_PRINT_MODULE("Changing to audio device: " + m_audioDevice.description(), GameConsole::eDEBUG, GameConsole::eAudio);
        m_audioOutput.setDevice(m_audioDevice);
        m_player->m_player.setAudioOutput(&m_audioOutput);
        m_soundCaches.clear();
        createSoundCache();
        m_player->m_player.stop();
        SlotPlayRandom();
    }
#endif
}

void AudioManager::clearMusicPositions()
{
    emit sigClearMusicPositions();
}

void AudioManager::SlotClearMusicPositions()
{
#ifdef AUDIOSUPPORT
    m_musicPlayPositionCache.clear();
#endif
}

void AudioManager::playMusic(qint32 File)
{
    emit sigPlayMusic(File);
}

void AudioManager::continueMusic(QString file, qint32 position)
{
    emit sigContinueMusic(file, position);
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
    if (Mainapp::getInstance()->isAudioThread())
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

void AudioManager::playSound(QString file, qint32 loops, qint32 delay, float volume, bool stopOldestSound, qint32 duration)
{
    emit sigPlaySound(file, loops, delay, volume, stopOldestSound, duration);
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
    if (Mainapp::getInstance()->isAudioThread())
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
        CONSOLE_PRINT_MODULE("AudioThread::SlotClearPlayList() start clearing", GameConsole::eDEBUG, GameConsole::eAudio);
        if (!m_player->m_currentMediaFile.isEmpty())
        {
            m_musicPlayPositionCache[m_player->m_currentMediaFile] = m_player->m_player.position();
            m_player->m_currentMediaFile = "";
        }
        m_PlayListdata.clear();
        m_player->m_currentMedia = -1;
        m_player->m_nextMedia = -1;
        m_player->m_player.stop();
        m_player->m_fileStream.close();
        CONSOLE_PRINT_MODULE("AudioThread::SlotClearPlayList() playlist cleared", GameConsole::eDEBUG, GameConsole::eAudio);
    }
#endif
}

void AudioManager::SlotPlayMusic(qint32 file)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio && !Settings::getInstance()->getMuted())
    {
        if (file >= 0 && file < m_PlayListdata.size())
        {
            CONSOLE_PRINT_MODULE("Starting music for player: " + m_PlayListdata[file].m_file, GameConsole::eDEBUG, GameConsole::eAudio);
            if (!m_player->m_currentMediaFile.isEmpty())
            {
                m_musicPlayPositionCache[m_player->m_currentMediaFile] = m_player->m_player.position();
                m_player->m_currentMediaFile = "";
            }
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

void AudioManager::SlotContinueMusic(QString file, qint32 position)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio && !Settings::getInstance()->getMuted())
    {
        if (file.isEmpty() && m_PlayListdata.size() == 1)
        {
            file = m_PlayListdata[0].m_file;
        }
        if (!file.isEmpty())
        {
            for (qint32 i = 0; i < m_PlayListdata.size(); ++i)
            {
                if (m_PlayListdata[i].m_file.endsWith(file))
                {
                    CONSOLE_PRINT_MODULE("Continue music for player: " + file, GameConsole::eDEBUG, GameConsole::eAudio);
                    if (!m_player->m_currentMediaFile.isEmpty())
                    {
                        m_musicPlayPositionCache[m_player->m_currentMediaFile] = m_player->m_player.position();
                        m_player->m_currentMediaFile = "";
                    }
                    m_player->m_player.stop();
                    m_player->m_currentMedia = -1;
                    m_player->m_nextMedia = -1;
                    if (position <  0)
                    {
                        position = m_musicPlayPositionCache[m_PlayListdata[i].m_file];
                    }
                    m_player->m_currentMedia = i;
                    loadMediaForFile(m_PlayListdata[i].m_file, position);
                    m_player->m_player.play();
                    auto newPosition = m_player->m_player.position();
                    break;
                }
            }
        }
        else
        {
           SlotPlayRandom();
        }
    }
#endif
}

void AudioManager::loadMediaForFile(QString filePath, qint32 position)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        m_player->m_player.setPosition(0);
        m_player->m_fileStream.close();
        QFile file(filePath);
        m_player->m_currentMediaFile = filePath;
        file.open(QIODevice::ReadOnly);
        m_player->m_content = file.readAll();
        m_player->m_fileStream.open(QIODevice::ReadOnly);
        m_player->m_player.setSourceDevice(&(m_player->m_fileStream));
        m_player->m_player.setPosition(position);
    }
#endif
}

void AudioManager::SlotPlayRandom()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio && !Settings::getInstance()->getMuted())
    {
        CONSOLE_PRINT_MODULE("AudioThread::SlotPlayRandom", GameConsole::eDEBUG, GameConsole::eAudio);
        qint32 size = m_PlayListdata.size();
        if (size > 0)
        {
            if (m_player->m_nextMedia < 0 || m_player->m_nextMedia > size)
            {
                m_player->m_player.stop();
                m_player->m_currentMedia = GlobalUtils::randIntBase(0, size - 1);
                loadMediaForFile(m_PlayListdata[m_player->m_currentMedia].m_file);


                m_player->m_player.play();
                CONSOLE_PRINT_MODULE("Buffering music for player: " + m_PlayListdata[m_player->m_currentMedia].m_file, GameConsole::eDEBUG, GameConsole::eAudio);
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
                CONSOLE_PRINT_MODULE("Seeking music for player: " + m_PlayListdata[m_player->m_currentMedia].m_file + " to " + QString::number(loopPos), GameConsole::eDEBUG, GameConsole::eAudio);
            }
            else
            {
                m_player->m_player.stop();
                m_player->m_currentMedia = m_player->m_nextMedia;
                loadMediaForFile(m_PlayListdata[m_player->m_currentMedia].m_file);
                m_player->m_player.play();
                CONSOLE_PRINT_MODULE("Buffering music for player: " + m_PlayListdata[m_player->m_currentMedia].m_file, GameConsole::eDEBUG, GameConsole::eAudio);
            }
            m_player->m_nextMedia = GlobalUtils::randIntBase(0, size - 1);
        }
        else
        {
            CONSOLE_PRINT_MODULE("Playlist is empty", GameConsole::eDEBUG, GameConsole::eAudio);
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
                       static_cast<qreal>(Settings::getInstance()->getTotalVolume()) / 100.0);
        qreal volume = QAudio::convertVolume(sound, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
        if (Settings::getInstance()->getMuted())
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
        CONSOLE_PRINT_MODULE("Setting volume to : " + QString::number(volume), GameConsole::eDEBUG, GameConsole::eAudio);
        m_audioOutput.setVolume(volume);
    }
#endif
}

void AudioManager::slotSetMuteInternal(bool value)
{
    if (Settings::getInstance()->getMuteOnFcousedLost())
    {
        m_internalMuted = value;
#ifdef AUDIOSUPPORT
        m_audioOutput.setMuted(m_internalMuted);
        for (auto & soundEffect : m_soundEffectData)
        {
            if (soundEffect.sound.get() != nullptr)
            {
                soundEffect.sound->setMuted(m_internalMuted);
            }
        }
#endif
    }
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
            currentPath = Settings::getInstance()->getUserPath() + file;
            if (!QFile::exists(currentPath))
            {
                CONSOLE_PRINT_MODULE("Unable to locate music file: " + currentPath + " using compiled path.", GameConsole::eDEBUG, GameConsole::eAudio);
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
        CONSOLE_PRINT_MODULE("Media status changed for player to " + QString::number(status), GameConsole::eDEBUG, GameConsole::eAudio);
        switch (status)
        {
            case QMediaPlayer::NoMedia:
            {
                break;
            }
            case QMediaPlayer::EndOfMedia:
            {
                m_player->m_currentMedia = -1;
                CONSOLE_PRINT_MODULE("Music stopped at position " + QString::number(m_player->m_player.position()), GameConsole::eDEBUG, GameConsole::eAudio);
                // shuffle through loaded media
                SlotPlayRandom();
                break;
            }
            case QMediaPlayer::InvalidMedia:
            {
                CONSOLE_PRINT_MODULE("Invalid media detected for player", GameConsole::eWARNING, GameConsole::eAudio);
                if (m_player->m_currentMedia < m_PlayListdata.size())
                {
                    CONSOLE_PRINT_MODULE("Unable to play: " +  m_PlayListdata[m_player->m_currentMedia].m_file, GameConsole::eWARNING, GameConsole::eAudio);
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
    CONSOLE_PRINT_MODULE("Playback state changed to " + QString::number(newState) + " for player", GameConsole::eDEBUG, GameConsole::eAudio);
}
#endif

void AudioManager::SlotLoadFolder(QString folder)
{
#ifdef AUDIOSUPPORT
    QStringList loadedSounds;
    for (qint32 i = 0; i < Settings::getInstance()->getMods().size(); i++)
    {
        loadMusicFolder(Settings::getInstance()->getUserPath() + "/" + Settings::getInstance()->getMods().at(i) + "/" + folder, loadedSounds);
        loadMusicFolder(QString(oxygine::Resource::RCC_PREFIX_PATH) +  "/" + Settings::getInstance()->getMods().at(i) + "/" + folder, loadedSounds);
    }
    if (m_loadBaseGameFolders)
    {
        loadMusicFolder(Settings::getInstance()->getUserPath() + folder, loadedSounds);
        loadMusicFolder(oxygine::Resource::RCC_PREFIX_PATH + folder, loadedSounds);
    }
#endif
}

void AudioManager::loadMusicFolder(const QString & folder, QStringList& loadedSounds)
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
            CONSOLE_PRINT_MODULE("Unable to locate music folder: " + folder, GameConsole::eDEBUG, GameConsole::eAudio);
        }
    }
#endif
}

void AudioManager::addMusicToPlaylist(const QString & file, qint64 startPointMs, qint64 endPointMs)
{
    CONSOLE_PRINT_MODULE("Adding " + file + " to play list", GameConsole::eDEBUG, GameConsole::eAudio);
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
            CONSOLE_PRINT_MODULE("Player reached loop end desired position " + QString::number(loopPos) + " actual loop position " + QString::number(duration) + " for player", GameConsole::eDEBUG, GameConsole::eAudio);
            // shuffle load media
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

void AudioManager::SlotPlaySound(QString file, qint32 loops, qint32 delay, float volume, bool stopOldestSound, qint32 duration)
{
#ifdef AUDIOSUPPORT
    if (Settings::getInstance()->getMuted() || m_noAudio || m_internalMuted)
    {
        return;
    }
    qreal sound = (static_cast<qreal>(Settings::getInstance()->getSoundVolume()) / 100.0 *
                   static_cast<qreal>(Settings::getInstance()->getTotalVolume()) / 100.0) * volume;
    sound = QAudio::convertVolume(sound, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
    if (sound > 0)
    {
        if (m_soundCaches.contains(file))
        {
            auto & soundCache = m_soundCaches[file];
            if (soundCache->m_usedSounds.size() < soundCache->m_maxUseCount)
            {
                bool started = false;
                for (qint32 i = m_lastUsedSoundSlot; i < MAX_PARALLEL_SOUNDS; ++i)
                {
                    if (tryPlaySoundAtCachePosition(soundCache, i,
                                                    file, loops, delay, sound,
                                                    stopOldestSound, duration))
                    {
                        m_lastUsedSoundSlot = i + 1;
                        started = true;
                        break;
                    }
                }
                if (!started)
                {
                    for (qint32 i = 0; i < m_lastUsedSoundSlot; ++i)
                    {
                        if (tryPlaySoundAtCachePosition(soundCache, i,
                                                        file, loops, delay, sound,
                                                        stopOldestSound, duration))
                        {
                            m_lastUsedSoundSlot = 0;
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            CONSOLE_PRINT("Unable to locate sound: " + file, GameConsole::eDEBUG);
        }
    }
#endif
}

void AudioManager::SlotStopAllSounds()
{
#ifdef AUDIOSUPPORT
    CONSOLE_PRINT_MODULE("Stopping all sounds", GameConsole::eDEBUG, GameConsole::eAudio);
    for (auto & soundCache : m_soundCaches)
    {
        for (auto & soundEffect : m_soundEffectData)
        {
            soundEffect.sound->stop();
            soundEffect.timer.stop();
        }
    }
#endif
}

void AudioManager::SlotStopSound(QString file)
{
#ifdef AUDIOSUPPORT
    if (m_soundCaches.contains(file))
    {
        CONSOLE_PRINT_MODULE("Stopping sound " + file, GameConsole::eDEBUG, GameConsole::eAudio);
        auto & soundCache = m_soundCaches[file];
        for (auto & item : soundCache->m_usedSounds)
        {
            stopSoundInternal(item);
        }
        soundCache->m_usedSounds.clear();
    }
#endif
}

#ifdef AUDIOSUPPORT

void AudioManager::stopOldestSound(SoundData* soundData)
{
    CONSOLE_PRINT_MODULE("Stopping oldest sound sound of " + soundData->cacheUrl.toString(), GameConsole::eDEBUG, GameConsole::eAudio);
    if (soundData->m_usedSounds.size() > 0)
    {
        stopSoundInternal(soundData->m_usedSounds[0]);
        soundData->m_usedSounds.removeFirst();
    }
}
#endif
