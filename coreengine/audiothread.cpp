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
#include <QAudioDevice>
#include <QApplication>

AudioThread::AudioThread()
    :
#ifdef AUDIOSUPPORT
      m_audioOutput(this)
#endif
{
    setObjectName("AudioThread");
    Interpreter::setCppOwnerShip(this);
    // move signals and slots to Audio Thread
    moveToThread(Mainapp::getAudioWorker());
    Mainapp* pApp = Mainapp::getInstance();
    if (!pApp->getNoUi())
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
        connect(this, &AudioThread::sigInitAudio,            this, &AudioThread::initAudio, Qt::BlockingQueuedConnection);
        connect(this, &AudioThread::sigCreateSoundCache,     this, &AudioThread::createSoundCache, Qt::BlockingQueuedConnection);
        connect(this, &AudioThread::sigChangeAudioDevice, this, &AudioThread::SlotChangeAudioDevice, Qt::BlockingQueuedConnection);
        connect(this, &AudioThread::sigLoadNextAudioFile,     this, &AudioThread::loadNextAudioFile, Qt::QueuedConnection);
    }
}

AudioThread::~AudioThread()
{
#ifdef AUDIOSUPPORT
    for (auto & player : m_player)
    {
        player->m_player.stop();
    }
    for (auto & cache : m_soundCaches)
    {
        for (qint32 i = 0; i < SoundData::MAX_SAME_SOUNDS; ++i)
        {
            if (cache->sound[i].get() != nullptr)
            {
                cache->timer[i]->stop();
                cache->sound[i]->stop();
                cache->sound[i].reset();
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
    CONSOLE_PRINT("AudioThread::initAudio", Console::eDEBUG);
    m_audioDevice = Settings::getAudioOutput().value<QAudioDevice>();
    m_audioOutput.setDevice(m_audioDevice);
    createPlayer(0);
    createPlayer(1);
    // everything needs to be created in the context of this thread
    SlotSetVolume(static_cast<qint32>(static_cast<float>(Settings::getMusicVolume())));
#endif
}

void AudioThread::createPlayer(qint32 player)
{
#ifdef AUDIOSUPPORT
    CONSOLE_PRINT("AudioThread::createPlayer() " + QString::number(player), Console::eDEBUG);
    m_player[player].reset(new Player(this));
    m_player[player]->m_player.setAudioOutput(&m_audioOutput);
    // connect media state handling
    connect(&m_player[player]->m_player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus newState)
    {
        mediaStatusChanged(m_player[player]->m_player, player, newState);
    }, Qt::QueuedConnection);
    // connect playback state handling
    connect(&m_player[player]->m_player, &QMediaPlayer::playbackStateChanged, this, [=](QMediaPlayer::PlaybackState newState)
    {
        mediaPlaybackStateChanged(player, newState);
    }, Qt::QueuedConnection);
    // connect error handling
    connect(&m_player[player]->m_player, &QMediaPlayer::errorOccurred, this, [=](QMediaPlayer::Error error, const QString &errorString)
    {
        reportReplayError(player, error, errorString);
    }, Qt::QueuedConnection);
    connect(&m_player[player]->m_player, &QMediaPlayer::positionChanged, this, &AudioThread::SlotCheckMusicEnded);
#endif
}

void AudioThread::createSoundCache()
{
#ifdef AUDIOSUPPORT
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
#endif
}

void AudioThread::readSoundCacheFromXml(QString folder)
{
#ifdef AUDIOSUPPORT
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
#endif
}

void AudioThread::fillSoundCache(qint32 count, QString folder, QString file)
{
#ifdef AUDIOSUPPORT
    if (count > SoundData::MAX_SAME_SOUNDS)
    {
        count = SoundData::MAX_SAME_SOUNDS;
    }
    std::shared_ptr<SoundData> cache = std::make_shared<SoundData>();
    CONSOLE_PRINT("Caching sound " + folder + file + " with amount " + QString::number(count), Console::eDEBUG);
    for (qint32 i = 0; i < count; ++i)
    {
        cache->sound[i] = std::make_shared<QSoundEffect>(this);
        cache->sound[i]->setObjectName(file);
        cache->sound[i]->setAudioDevice(m_audioDevice);
        cache->sound[i]->setSource(GlobalUtils::getUrlForFile(folder + file));
        cache->timer[i] = std::make_shared<QTimer>(this);
        cache->timer[i]->setObjectName("SoundEffectTimer");
        connect(cache->timer[i].get(), &QTimer::timeout, cache->sound[i].get(), &QSoundEffect::play);
        cache->timer[i]->setSingleShot(true);
    }
    m_soundCaches.insert(file, cache);
#endif
}

qint32 AudioThread::getSoundsBuffered()
{
    qint32 count = 0;
    for (const auto & cache : qAsConst(m_soundCaches))
    {
        for (const auto & sound : qAsConst(cache->sound))
        {
            if (sound.get() != nullptr)
            {
                ++count;
            }
            else
            {
                break;
            }
        }
    }
    return count;
}

void AudioThread::changeAudioDevice(const QVariant& value)
{
    emit sigChangeAudioDevice(value);
}

void AudioThread::SlotChangeAudioDevice(const QVariant& value)
{
#ifdef AUDIOSUPPORT
    m_audioDevice = value.value<QAudioDevice>();
    CONSOLE_PRINT("Changing to audio device: " + m_audioDevice.description(), Console::eDEBUG);
    m_audioOutput.setDevice(m_audioDevice);
    m_player[0]->m_player.setAudioOutput(&m_audioOutput);
    m_player[1]->m_player.setAudioOutput(&m_audioOutput);
    for (auto & cache : m_soundCaches)
    {
        for (auto & sound : cache->sound)
        {
            if (sound.get() != nullptr)
            {
                sound->setAudioDevice(m_audioDevice);
            }
        }
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
    emit sigClearPlayList();
}

void AudioThread::playRandom()
{
    emit sigPlayRandom();
}

void AudioThread::playSound(QString file, qint32 loops, qint32 delay, float volume)
{
    emit sigPlaySound(file, loops, delay, volume);
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
    emit sigLoadFolder(folder);
}

void AudioThread::SlotClearPlayList()
{
#ifdef AUDIOSUPPORT
    CONSOLE_PRINT("AudioThread::SlotClearPlayList() start clearing", Console::eDEBUG);
    m_PlayListdata.clear();
    m_currentPlayer = -1;
    m_player[0]->m_playListPostiton = -1;
    m_player[1]->m_playListPostiton = -1;
    m_player[0]->m_player.stop();
    m_player[0]->m_player.setSource(QUrl());
    m_player[1]->m_player.stop();
    m_player[1]->m_player.setSource(QUrl());
    CONSOLE_PRINT("AudioThread::SlotClearPlayList() playlist cleared", Console::eDEBUG);
#endif
}

void AudioThread::SlotPlayMusic(qint32 file)
{
#ifdef AUDIOSUPPORT
    if (file >= 0 && file < m_PlayListdata.size())
    {
        CONSOLE_PRINT("Starting music for player 0: " + m_PlayListdata[file].m_file, Console::eDEBUG);
        m_currentPlayer = 0;
        for (auto & player : m_player)
        {
            player->m_player.stop();
            player->m_playListPostiton = -1;
        }
        m_player[m_currentPlayer]->m_playListPostiton = file;
        loadMediaForFile(m_player[m_currentPlayer].get(), m_PlayListdata[file].m_file);
        m_player[m_currentPlayer]->m_player.play();
        bufferOtherPlayer();
        m_currentMedia = file;
    }
    else
    {
        CONSOLE_PRINT("AudioThread::SlotPlayMusic Trying to play unknown music index", Console::eERROR);
    }
#endif
}

void AudioThread::loadMediaForFile(Player* pPlayer, QString filePath)
{
//    QFileInfo fileInfo(filePath);
//    QString tempPath;
//    if (!fileInfo.exists() ||
//        filePath.startsWith(oxygine::Resource::RCC_PREFIX_PATH))
//    {
//        if (!pPlayer->m_currentTempFile.isEmpty())
//        {
//            QFile::remove(pPlayer->m_currentTempFile);
//        }
//        tempPath = "temp/" + fileInfo.fileName();
//        QFile::copy(filePath, tempPath);
//        pPlayer->m_currentTempFile = tempPath;
//    }
//    auto fullPath = QUrl::fromLocalFile(QFileInfo(tempPath).absoluteFilePath());
//    pPlayer->m_player.setSource(fullPath);
    pPlayer->m_player.setSource(GlobalUtils::getUrlForFile(filePath));
}

void AudioThread::SlotPlayRandom()
{
#ifdef AUDIOSUPPORT
    initialAudioBuffering();
    // start buffered player
    if (m_currentPlayer == 0)
    {
        m_currentPlayer = 1;
    }
    else
    {
        m_currentPlayer = 0;
    }
    m_currentMedia = m_player[m_currentPlayer]->m_playListPostiton;
    CONSOLE_PRINT("Starting player " + QString::number(m_currentPlayer), Console::eDEBUG);
    m_player[m_currentPlayer]->m_player.play();
    bufferOtherPlayer();
#endif
}

void AudioThread::initialAudioBuffering()
{
#ifdef AUDIOSUPPORT
    if (m_currentPlayer < 0)
    {
        CONSOLE_PRINT("AudioThread::initialAudioBuffering", Console::eDEBUG);
        qint32 size = m_PlayListdata.size();
        if (size > 0)
        {
            m_player[0]->m_player.stop();
            m_player[0]->m_playListPostiton = GlobalUtils::randIntBase(0, size - 1);
            loadMediaForFile(m_player[0].get(), m_PlayListdata[m_player[0]->m_playListPostiton].m_file);
            m_player[0]->m_playerStartPosition = 0;
            CONSOLE_PRINT("Buffering music for player 0: " + m_PlayListdata[m_player[0]->m_playListPostiton].m_file + " at position " + QString::number(m_player[0]->m_playerStartPosition), Console::eDEBUG);
        }
        else
        {
            CONSOLE_PRINT("Playlist is empty", Console::eDEBUG);
        }
    }
#endif
}

void AudioThread::bufferOtherPlayer()
{
#ifdef AUDIOSUPPORT
    CONSOLE_PRINT("AudioThread::bufferOtherPlayer()", Console::eDEBUG);
    qint32 size = m_PlayListdata.size();
    if (size > 0 && m_currentPlayer >= 0)
    {
        qint32 bufferPlayer = 0;
        if (m_currentPlayer == 0)
        {
            bufferPlayer = 1;
        }
        m_player[bufferPlayer]->m_player.stop();
        qint32 newMedia = GlobalUtils::randIntBase(0, size - 1);
        m_player[bufferPlayer]->m_playListPostiton = newMedia;
        loadMediaForFile(m_player[bufferPlayer].get(), m_PlayListdata[m_player[bufferPlayer]->m_playListPostiton].m_file);
        if (m_PlayListdata[newMedia].m_startpointMs > 0 && newMedia == m_player[m_currentPlayer]->m_playListPostiton)
        {
            m_player[bufferPlayer]->m_playerStartPosition = m_PlayListdata[newMedia].m_startpointMs;
        }
        else
        {
            m_player[bufferPlayer]->m_playerStartPosition = 0;
        }
        m_player[bufferPlayer]->m_player.setPosition(0);
        CONSOLE_PRINT("Buffering music for player: " + QString::number(bufferPlayer) + ": " + m_PlayListdata[newMedia].m_file + " at position " + QString::number(m_player[bufferPlayer]->m_playerStartPosition), Console::eDEBUG);
    }
    else
    {
        CONSOLE_PRINT("Playlist is empty or player is invalid", Console::eDEBUG);
    }
#endif
}

void AudioThread::SlotSetVolume(qint32 value)
{
#ifdef AUDIOSUPPORT
    qreal sound = (static_cast<qreal>(value) / 100.0 *
                   static_cast<qreal>(Settings::getTotalVolume()) / 100.0);
    qreal volume = QAudio::convertVolume(sound, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);

    CONSOLE_PRINT("Setting volume to : " + QString::number(volume), Console::eDEBUG);
    m_audioOutput.setVolume(volume);
#endif
}

void AudioThread::SlotAddMusic(QString file, qint64 startPointMs, qint64 endPointMs)
{
#ifdef AUDIOSUPPORT
    QString currentPath = file;
    if (!QFile::exists(currentPath))
    {
        currentPath = oxygine::Resource::RCC_PREFIX_PATH + file;
    }
    if (QFile::exists(currentPath))
    {
        for (auto & player : m_player)
        {
            player->m_player.stop();
        }
        addMusicToPlaylist(currentPath, startPointMs, endPointMs);
    }
    else
    {
        CONSOLE_PRINT("Unable to locate music file: " + currentPath, Console::eERROR);
    }
#endif
}

void AudioThread::mediaStatusChanged(QMediaPlayer &player, qint32 playerIndex, QMediaPlayer::MediaStatus status)
{
#ifdef AUDIOSUPPORT
    CONSOLE_PRINT("Media status changed for player " + QString::number(playerIndex) + " to " + QString::number(status), Console::eDEBUG);
    switch (status)
    {
        case QMediaPlayer::NoMedia:
        {
            if (m_PlayListdata.size() > 0)
            {
                emit sigLoadNextAudioFile(playerIndex);
            }
            break;
        }
        case QMediaPlayer::LoadedMedia:
        case QMediaPlayer::BufferedMedia:
        {
            if (playerIndex != m_currentPlayer)
            {
                CONSOLE_PRINT("Seeking to media position " + QString::number(m_player[playerIndex]->m_playerStartPosition) + " and pausing player again", Console::eDEBUG);
                player.setPosition(m_player[playerIndex]->m_playerStartPosition);
                player.pause();
            }
            break;
        }
        case QMediaPlayer::EndOfMedia:
        {
            CONSOLE_PRINT("Music stopped at position " + QString::number(m_player[playerIndex]->m_player.position()), Console::eDEBUG);
            if (m_currentMedia < m_PlayListdata.size() &&
                m_currentMedia >= 0 &&
                m_PlayListdata[m_currentMedia].m_endpointMs < 0 &&
                playerIndex == m_currentPlayer)
            {
                // shuffle through loaded media
                SlotPlayRandom();
            }
            else
            {
                m_player[playerIndex]->m_player.setSource(QUrl());
            }
            break;
        }
        case QMediaPlayer::InvalidMedia:
        {
            CONSOLE_PRINT("Invalid media detected for player " + QString::number(playerIndex), Console::eWARNING);
        }
        default:
        {
            break;
        }
    }
#endif
}

void AudioThread::loadNextAudioFile(qint32 playerIndex)
{
#ifdef AUDIOSUPPORT
    qint32 playListEntry = m_player[playerIndex]->m_playListPostiton;
    if (playListEntry >= 0 && playListEntry < m_PlayListdata.size())
    {
        loadMediaForFile(m_player[playerIndex].get(), m_PlayListdata[playListEntry].m_file);
        m_player[playerIndex]->m_player.setPosition(0);
        CONSOLE_PRINT("Rebuffering music cause it changed to no media for player: " + QString::number(playerIndex) + ": " + m_PlayListdata[playListEntry].m_file + " at position " + QString::number(m_player[playerIndex]->m_playerStartPosition), Console::eDEBUG);
        m_player[playerIndex]->m_player.play();
    }
#endif
}

void AudioThread::mediaPlaybackStateChanged(qint32 playerIndex, QMediaPlayer::PlaybackState newState)
{
#ifdef AUDIOSUPPORT
    CONSOLE_PRINT("Playback state changed to " + QString::number(newState) + " for player " + QString::number(playerIndex), Console::eDEBUG);
    if (newState == QMediaPlayer::PlaybackState::PlayingState &&
        m_currentPlayer != playerIndex)
    {
        m_player[playerIndex]->m_player.pause();
    }
#endif
}

void AudioThread::SlotLoadFolder(QString folder)
{
#ifdef AUDIOSUPPORT
    QStringList loadedSounds;
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        loadMusicFolder(Settings::getMods().at(i) + "/" + folder, loadedSounds);
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
#endif
}

void AudioThread::addMusicToPlaylist(QString file, qint64 startPointMs, qint64 endPointMs)
{
    CONSOLE_PRINT("Adding " + file + " to play list", Console::eDEBUG);
    m_PlayListdata.append(PlaylistData(file, startPointMs, endPointMs));
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
    if (m_currentMedia >= 0 && m_currentMedia < m_PlayListdata.size())
    {
        qint64 loopPos = m_PlayListdata[m_currentMedia].m_endpointMs;
        if ((loopPos > 0) &&
            ((duration >= loopPos) ||
             (m_currentPlayer >= 0 && m_player[m_currentPlayer]->m_player.mediaStatus() == QMediaPlayer::EndOfMedia)))
        {
            CONSOLE_PRINT("Player reached loop end desired position " + QString::number(loopPos) + " actual loop position " + QString::number(duration) + " for player " + QString::number(m_currentPlayer), Console::eDEBUG);
            // shuffle load new media
            SlotPlayRandom();
        }
    }
#endif
}

void AudioThread::SlotPlaySound(QString file, qint32 loops, qint32 delay, float volume)
{
#ifdef AUDIOSUPPORT
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
                if (soundCache->sound[i].get() == nullptr)
                {
                    break;
                }
                else if (tryPlaySoundAtCachePosition(soundCache, i,
                                                     file, loops, delay, sound))
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
                                                    file, loops, delay, sound))
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

bool AudioThread::tryPlaySoundAtCachePosition(std::shared_ptr<SoundData> & soundCache, qint32 i,
                                              QString & file, qint32 loops, qint32 delay, qreal sound)
{
    bool started = false;
#ifdef AUDIOSUPPORT
    if (!soundCache->sound[i]->isPlaying() &&
        !soundCache->timer[i]->isActive())
    {
        CONSOLE_PRINT("Playing sound: " + file + " using cache sound " + QString::number(i), Console::eDEBUG);

        soundCache->sound[i]->setVolume(sound);
        soundCache->sound[i]->setLoopCount(loops);
        soundCache->sound[i]->setMuted(false);
        // start play
        if (delay > 0)
        {
            soundCache->timer[i]->start(delay);
        }
        else
        {
            soundCache->sound[i]->play();
        }
        started = true;
        soundCache->nextSoundToUse = i + 1;
    }
#endif
    return started;
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
            if (soundCache->sound[i].get() != nullptr &&
                soundCache->sound[i]->isPlaying())
            {
                stopSound(soundCache, i);
            }
        }
    }
#endif
}

void AudioThread::stopSound(std::shared_ptr<SoundData> & soundData, qint32 soundIndex)
{
    soundData->sound[soundIndex]->setVolume(0);
    soundData->sound[soundIndex]->setLoopCount(0);
}

void AudioThread::SlotStopAllSounds()
{
#ifdef AUDIOSUPPORT
    for (auto & soundCache : m_soundCaches)
    {
        for (qint32 i = 0; i < SoundData::MAX_SAME_SOUNDS; ++i)
        {
            if (soundCache->sound[i].get() != nullptr &&
                soundCache->sound[i]->isPlaying())
            {
                stopSound(soundCache, i);
            }
        }
    }
#endif
}

void AudioThread::reportReplayError(qint32 player, QMediaPlayer::Error error, const QString &errorString)
{
    CONSOLE_PRINT("Error in player " + QString::number(player) + ": " + errorString, Console::eERROR);
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
