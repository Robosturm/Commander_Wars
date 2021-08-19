#include "coreengine/audiothread.h"
#include "coreengine/settings.h"
#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/globalutils.h"
#include "coreengine/interpreter.h"

#include <QDir>
#include <QList>
#include <QAudioDevice>

AudioThread::AudioThread()
    : m_player{Player(this), Player(this)},
      m_audioOutput(this),
      m_positionUpdateTimer(this)
{
    setObjectName("AudioThread");
    Interpreter::setCppOwnerShip(this);
    // move signals and slots to Audio Thread
    moveToThread(Mainapp::getAudioWorker());
    connect(this, &AudioThread::SignalPlayMusic,         this, &AudioThread::SlotPlayMusic, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalSetVolume,         this, &AudioThread::SlotSetVolume, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalAddMusic,          this, &AudioThread::SlotAddMusic, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalClearPlayList,     this, &AudioThread::SlotClearPlayList, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalPlayRandom,        this, &AudioThread::SlotPlayRandom, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalLoadFolder,        this, &AudioThread::SlotLoadFolder, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalPlaySound,         this, &AudioThread::SlotPlaySound, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalStopSound,         this, &AudioThread::SlotStopSound, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalStopAllSounds,     this, &AudioThread::SlotStopAllSounds, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::sigInitAudio,            this, &AudioThread::initAudio, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalChangeAudioDevice, this, &AudioThread::SlotChangeAudioDevice, Qt::BlockingQueuedConnection);
    m_audioDevice = Settings::getAudioOutput().value<QAudioDevice>();
    m_audioOutput.setDevice(m_audioDevice);
    m_player[0].m_player.setAudioOutput(&m_audioOutput);
    m_player[0].m_player.setSourceDevice(&m_player[0].m_playerFile);
    m_player[1].m_player.setAudioOutput(&m_audioOutput);
    m_player[1].m_player.setSourceDevice(&m_player[1].m_playerFile);
}

AudioThread::~AudioThread()
{
    m_positionUpdateTimer.stop();
    m_player[0].m_player.stop();
    m_player[1].m_player.stop();
}

void AudioThread::initAudio()
{
    // everything needs to be created in the context of this thread
    SlotSetVolume(static_cast<qint32>(static_cast<float>(Settings::getMusicVolume())));
    connect(&m_player[0].m_player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus newState)
    {
        mediaStatusChanged(m_player[0].m_player, 0, newState);
    });
    connect(&m_player[1].m_player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus newState)
    {
        mediaStatusChanged(m_player[1].m_player, 1, newState);
    });
    connect(&m_player[0].m_player, &QMediaPlayer::errorOccurred, this, &AudioThread::reportReplayError, Qt::QueuedConnection);
    connect(&m_player[1].m_player, &QMediaPlayer::errorOccurred, this, &AudioThread::reportReplayError, Qt::QueuedConnection);

    m_positionUpdateTimer.setInterval(25);
    m_positionUpdateTimer.setSingleShot(false);
    connect(&m_positionUpdateTimer, &QTimer::timeout, this, [=]()
    {
        if (m_currentPlayer >= 0)
        {
            SlotCheckMusicEnded(m_player[m_currentPlayer].m_player.position());
        }
    });
    m_positionUpdateTimer.start();
}

void AudioThread::changeAudioDevice(const QVariant& value)
{
    emit SignalChangeAudioDevice(value);
}

void AudioThread::SlotChangeAudioDevice(const QVariant& value)
{
    m_audioDevice = value.value<QAudioDevice>();
    Console::print("Changing to audio device: " + m_audioDevice.description(), Console::eDEBUG);
    m_audioOutput.setDevice(m_audioDevice);
    m_player[0].m_player.setAudioOutput(&m_audioOutput);
    m_player[0].m_player.setSourceDevice(&m_player[0].m_playerFile);
    m_player[1].m_player.setAudioOutput(&m_audioOutput);
    m_player[1].m_player.setSourceDevice(&m_player[1].m_playerFile);
}

void AudioThread::playMusic(qint32 File)
{
    emit SignalPlayMusic(File);
}

void AudioThread::setVolume(qint32 value)
{
    emit SignalSetVolume(value);
}

qint32 AudioThread::getVolume()
{
    return m_audioOutput.volume();
}

void AudioThread::addMusic(QString File, qint64 startPointMs, qint64 endPointMs)
{
    emit SignalAddMusic(File, startPointMs, endPointMs);
}

void AudioThread::clearPlayList()
{
    emit SignalClearPlayList();
}

void AudioThread::playRandom()
{
    emit SignalPlayRandom();
}

void AudioThread::playSound(QString file, qint32 loops, QString folder, qint32 delay, float volume)
{
    emit SignalPlaySound(file, loops, folder, delay, volume);
}

void AudioThread::stopSound(QString file, QString folder)
{
    emit SignalStopSound(file, folder);
}

void AudioThread::stopAllSounds()
{
    emit SignalStopAllSounds();
}

void AudioThread::loadFolder(QString folder)
{
    emit SignalLoadFolder(folder);
}

void AudioThread::SlotClearPlayList()
{
    Console::print("AudioThread::SlotClearPlayList()", Console::eDEBUG);
    m_player[0].m_player.stop();
    m_player[0].m_playListPostiton = -1;
    m_player[1].m_player.stop();
    m_player[1].m_playListPostiton = -1;
    m_PlayListdata.clear();
    m_currentPlayer = -1;
}

void AudioThread::SlotPlayMusic(qint32 file)
{
    if (file >= 0 && file < m_PlayListdata.size())
    {
        Console::print("Starting music for player 0: " + m_PlayListdata[file].m_file, Console::eDEBUG);
        m_currentPlayer = 0;
        for (auto & player : m_player)
        {
            player.m_player.stop();
        }
        m_player[m_currentPlayer].m_playListPostiton = file;
        m_player[m_currentPlayer].m_playerFile.close();
        m_player[m_currentPlayer].m_playerFile.setFileName(m_PlayListdata[file].m_file);
        m_player[m_currentPlayer].m_playerFile.open(QIODevice::ReadOnly);
        m_player[m_currentPlayer].m_player.play();
        bufferOtherPlayer();
        m_currentMedia = file;
    }
    else
    {
        Console::print("AudioThread::SlotPlayMusic Trying to play unknown music index", Console::eERROR);
    }
}

void AudioThread::SlotPlayRandom()
{
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
    m_currentMedia = m_player[m_currentPlayer].m_playListPostiton;
    Console::print("Starting player " + QString::number(m_currentPlayer), Console::eDEBUG);
    m_player[m_currentPlayer].m_player.play();
    bufferOtherPlayer();
}

void AudioThread::initialAudioBuffering()
{
    if (m_currentPlayer < 0)
    {
        Console::print("AudioThread::initialAudioBuffering", Console::eDEBUG);
        qint32 size = m_PlayListdata.size();
        if (size > 0)
        {
            m_player[0].m_playListPostiton = GlobalUtils::randIntBase(0, size - 1);
            m_player[1].m_playListPostiton = GlobalUtils::randIntBase(0, size - 1);
            for (auto & player : m_player)
            {
                player.m_player.stop();
                player.m_playerFile.close();
                player.m_playerFile.setFileName(m_PlayListdata[player.m_playListPostiton].m_file);
                player.m_playerFile.open(QIODevice::ReadOnly);
                player.m_player.setSourceDevice(&player.m_playerFile, m_PlayListdata[player.m_playListPostiton].m_file);
            }

            m_player[0].m_playerStartPosition = 0;
            if (m_PlayListdata[m_player[1].m_playListPostiton].m_startpointMs > 0  && m_player[0].m_playListPostiton == m_player[1].m_playListPostiton)
            {
                m_player[1].m_playerStartPosition = m_PlayListdata[m_player[1].m_playListPostiton].m_startpointMs;
            }
            else
            {
                m_player[1].m_playerStartPosition = 0;
            }
            Console::print("Buffering music for player 0: " + m_PlayListdata[m_player[0].m_playListPostiton].m_file + " at position " + QString::number(m_player[0].m_playerStartPosition), Console::eDEBUG);
            Console::print("Buffering music for player 1: " + m_PlayListdata[m_player[1].m_playListPostiton].m_file + " at position " + QString::number(m_player[1].m_playerStartPosition), Console::eDEBUG);
        }
    }
}

void AudioThread::bufferOtherPlayer()
{
    Console::print("AudioThread::bufferOtherPlayer()", Console::eDEBUG);
    qint32 size = m_PlayListdata.size();
    if (size > 0 && m_currentPlayer >= 0)
    {
        qint32 newMedia = GlobalUtils::randIntBase(0, size - 1);

        qint32 bufferPlayer = 0;
        if (m_currentPlayer == 0)
        {
            bufferPlayer = 1;
        }
        m_player[bufferPlayer].m_player.stop();
        m_player[bufferPlayer].m_playListPostiton = newMedia;
        m_player[bufferPlayer].m_playerFile.close();
        m_player[bufferPlayer].m_playerFile.setFileName(m_PlayListdata[newMedia].m_file);
        m_player[bufferPlayer].m_playerFile.open(QIODevice::ReadOnly);
        m_player[bufferPlayer].m_player.setSourceDevice(&m_player[bufferPlayer].m_playerFile, m_PlayListdata[newMedia].m_file);
        if (m_PlayListdata[newMedia].m_startpointMs > 0 && newMedia == m_player[m_currentPlayer].m_playListPostiton)
        {
            m_player[bufferPlayer].m_playerStartPosition = m_PlayListdata[newMedia].m_startpointMs;
        }
        else
        {
            m_player[bufferPlayer].m_playerStartPosition = 0;
        }
        Console::print("Buffering music for player " + QString::number(bufferPlayer) + ": " + m_PlayListdata[newMedia].m_file + " at position " + QString::number(m_player[bufferPlayer].m_playerStartPosition), Console::eDEBUG);
        m_player[bufferPlayer].m_player.play();
    }
}

void AudioThread::SlotSetVolume(qint32 value)
{
    qreal sound = (static_cast<qreal>(value) / 100.0 *
                   static_cast<qreal>(Settings::getTotalVolume()) / 100.0);
    qreal volume = QAudio::convertVolume(sound, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
    m_audioOutput.setVolume(qRound(volume * 100));
}

void AudioThread::SlotAddMusic(QString file, qint64 startPointMs, qint64 endPointMs)
{
    QString currentPath = file;
    if (!QFile::exists(currentPath))
    {
        currentPath = oxygine::Resource::RCC_PREFIX_PATH + file;
    }
    if (QFile::exists(currentPath))
    {
        Console::print("Adding " + currentPath + " to play list", Console::eDEBUG);
        for (auto & player : m_player)
        {
            player.m_player.stop();
        }
        m_PlayListdata.append(PlaylistData(currentPath, startPointMs, endPointMs));
    }
    else
    {
        Console::print("Unable to locate music file: " + currentPath, Console::eERROR);
    }
}

void AudioThread::mediaStatusChanged(QMediaPlayer &player, qint32 playerIndex, QMediaPlayer::MediaStatus status)
{
    Console::print("Media status changed for player " + QString::number(playerIndex) + " to " + QString::number(status), Console::eDEBUG);
    switch (status)
    {
        case QMediaPlayer::LoadedMedia:
        case QMediaPlayer::BufferedMedia:
        {
            if (playerIndex != m_currentPlayer &&
                m_player[playerIndex].m_playerStartPosition != player.position())
            {
                Console::print("Seeking to media position " + QString::number(m_player[playerIndex].m_playerStartPosition) + " and pausing player again", Console::eDEBUG);
                player.setPosition(m_player[playerIndex].m_playerStartPosition);
                player.pause();
            }
            break;
        }
        case QMediaPlayer::EndOfMedia:
        {
            Console::print("Music stopped at position " + QString::number(m_player[playerIndex].m_player.position()), Console::eDEBUG);
            if (m_PlayListdata[m_currentMedia].m_endpointMs < 0 &&
                playerIndex == m_currentPlayer)
            {
                // shuffle through loaded media
                SlotPlayRandom();
            }
            break;
        }
        default:
        {
            break;
        }
    }
}

void AudioThread::SlotLoadFolder(QString folder)
{
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
}

void AudioThread::loadMusicFolder(QString folder, QStringList& loadedSounds)
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
                Console::print("Adding " + folder + '/' + file + " to play list", Console::eDEBUG);
                m_PlayListdata.append(PlaylistData(folder + '/' + file));
                loadedSounds.append(file);
            }
        }
    }
    else
    {
        Console::print("Unable to locate music folder: " + folder, Console::eDEBUG);
    }
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
    if (m_currentMedia >= 0 && m_currentMedia < m_PlayListdata.size())
    {
        qint64 loopPos = m_PlayListdata[m_currentMedia].m_endpointMs;
        if ((duration >= loopPos) &&
            (loopPos > 0))
        {
            Console::print("Player reached loop end desired position " + QString::number(loopPos) + " actual loop position " + QString::number(duration) + " for player " + QString::number(m_currentPlayer), Console::eDEBUG);
            // shuffle load new media
            SlotPlayRandom();
        }
    }
}

void AudioThread::SlotPlaySound(QString file, qint32 loops, QString folder, qint32 delay, float volume)
{
    qreal sound = (static_cast<qreal>(Settings::getSoundVolume()) / 100.0 *
                   static_cast<qreal>(Settings::getTotalVolume()) / 100.0) * volume;
    QString soundfile = folder + file;
    QStringList searchFolders = Settings::getMods();
    searchFolders.append(folder);
    searchFolders.append(oxygine::Resource::RCC_PREFIX_PATH + folder);
    for (qint32 i = searchFolders.size() - 1; i >= 0; i--)
    {
        if (QFile::exists(searchFolders[i] + file))
        {
            soundfile = searchFolders[i] + file;
            break;
        }
        else if (QFile::exists(searchFolders[i] + "/sounds/" + file))
        {
            soundfile = searchFolders[i] + "/sounds/" + file;
            break;
        }
    }
    QUrl url = QUrl::fromLocalFile(soundfile);
    if (url.isValid())
    {
        Console::print("Playing sound: " + url.toString(), Console::eDEBUG);
        QSoundEffect* pSoundEffect = new QSoundEffect(this);
        pSoundEffect->setObjectName("SoundEffect");
        qreal value = QAudio::convertVolume(sound,
                                            QAudio::LogarithmicVolumeScale,
                                            QAudio::LinearVolumeScale);
        QTimer* pTimer = new QTimer(this);
        pTimer->setObjectName("SoundEffectTimer");
        pSoundEffect->setVolume(value);
        pSoundEffect->setSource(url);
        pSoundEffect->setLoopCount(loops);
        pSoundEffect->setAudioDevice(m_audioDevice);
        connect(pSoundEffect, &QSoundEffect::playingChanged, this, &AudioThread::SlotSoundEnded, Qt::QueuedConnection);
        if (delay > 0)
        {
            connect(pTimer, &QTimer::timeout, this, &AudioThread::SlotSoundStart, Qt::QueuedConnection);
            pTimer->setSingleShot(true);
            pTimer->start(delay);
        }
        else
        {
            pSoundEffect->play();
        }
        m_Sounds.push_back(pSoundEffect);
        m_SoundTimers.push_back(pTimer);
    }
}

void AudioThread::SlotSoundStart()
{
    QTimer* pSender = dynamic_cast<QTimer*>(sender());
    for (qint32 i = 0; i < m_SoundTimers.size(); i++)
    {
        if (pSender == m_SoundTimers[i])
        {
            m_Sounds[i]->play();
        }
    }
}

void AudioThread::SlotSoundEnded()
{
    QSoundEffect* pSender = dynamic_cast<QSoundEffect*>(sender());
    qint32 i = 0;
    while (i < m_Sounds.size())
    {
        if ((m_Sounds[i]->isPlaying() == false) &&
            (m_Sounds[i]->loopsRemaining() == 0) &&
            (m_Sounds[i] == pSender))
        {
            delete m_SoundTimers[i];
            delete m_Sounds[i];
            m_Sounds.removeAt(i);
            m_SoundTimers.removeAt(i);
        }
        else
        {
            i++;
        }
    }
}

void AudioThread::SlotStopSound(QString file, QString folder)
{
    QUrl url = QUrl::fromLocalFile(folder + file);
    for (qint32 i = 0; i < m_Sounds.size(); i++)
    {
        if (m_Sounds[i]->source() == url)
        {
            m_SoundTimers[i]->stop();
            m_Sounds[i]->stop();
            delete m_Sounds[i];
            delete m_SoundTimers[i];
            m_Sounds.removeAt(i);
            m_SoundTimers.removeAt(i);
            break;
        }
    }
}

void AudioThread::SlotStopAllSounds()
{
    for (qint32 i = 0; i < m_Sounds.size(); i++)
    {
        m_SoundTimers[i]->stop();
        m_Sounds[i]->stop();
        delete m_Sounds[i];
        delete m_SoundTimers[i];
    }
    m_Sounds.clear();
    m_SoundTimers.clear();
}

void AudioThread::reportReplayError(QMediaPlayer::Error error, const QString &errorString)
{
    switch (error)
    {
        case QMediaPlayer::Error::FormatError:
        {
            Console::print("Audio playback error: Wrong audio format provided.", Console::eERROR);
            break;
        }
        case QMediaPlayer::Error::NetworkError:
        {
            Console::print("Audio playback error: Network access error to audio file.", Console::eERROR);
            break;
        }
        case QMediaPlayer::Error::ResourceError:
        {
            Console::print("Audio playback error: Media ressouce file could not be resolved.", Console::eERROR);
            break;
        }
        case QMediaPlayer::Error::AccessDeniedError:
        {
            Console::print("Audio playback error: Access denied.", Console::eERROR);
            break;
        }
        case QMediaPlayer::Error::NoError:
        {
            break;
        }
        default:
        {
            Console::print("Audio playback error: Service for replaying the requested audio format is missing.", Console::eERROR);
            break;
        }
    }
    Console::print("Error in player1: " + errorString, Console::eERROR);
    Console::print("Error in player2: " + errorString, Console::eERROR);
}
