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
    : m_player(this),
      m_player2(this),
#ifdef EnableMultimedia
      m_audioOutput(this),
#endif
      m_doubleBufferTimer(this)
{

    setObjectName("AudioThread");
    Interpreter::setCppOwnerShip(this);
    // move signals and slots to Audio Thread
    moveToThread(Mainapp::getAudioWorker());
    connect(this, &AudioThread::SignalPlayMusic,        this, &AudioThread::SlotPlayMusic, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalSetVolume,        this, &AudioThread::SlotSetVolume, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalAddMusic,         this, &AudioThread::SlotAddMusic, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalClearPlayList,    this, &AudioThread::SlotClearPlayList, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalPlayRandom,       this, &AudioThread::SlotPlayRandom, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalLoadFolder,       this, &AudioThread::SlotLoadFolder, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalPlaySound,        this, &AudioThread::SlotPlaySound, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalStopSound,        this, &AudioThread::SlotStopSound, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::SignalStopAllSounds,    this, &AudioThread::SlotStopAllSounds, Qt::BlockingQueuedConnection);
    connect(this, &AudioThread::sigInitAudio,           this, &AudioThread::initAudio, Qt::BlockingQueuedConnection);
#ifdef EnableMultimedia
    m_audioDevice = QMediaDevices::defaultAudioOutput();
    m_audioOutput.setDevice(m_audioDevice);
    m_player.setAudioOutput(&m_audioOutput);
    m_player2.setAudioOutput(&m_audioOutput);
#endif
}

AudioThread::~AudioThread()
{
    m_player.stop();
    m_player2.stop();
    m_doubleBufferTimer.stop();
}

void AudioThread::initAudio()
{
    // everything needs to be created in the context of this thread
    SlotSetVolume(static_cast<qint32>(static_cast<float>(Settings::getMusicVolume())));
    connect(&m_player, &QMediaPlayer::mediaStatusChanged, this, &AudioThread::SlotMediaStatusChanged, Qt::QueuedConnection);
    connect(&m_player, &QMediaPlayer::positionChanged, this, &AudioThread::SlotCheckMusicEnded, Qt::QueuedConnection);
    connect(&m_player2, &QMediaPlayer::mediaStatusChanged, this, &AudioThread::SlotMediaStatusChanged, Qt::QueuedConnection);
    connect(&m_player2, &QMediaPlayer::positionChanged, this, &AudioThread::SlotCheckMusicEnded, Qt::QueuedConnection);
    connect(&m_player, &QMediaPlayer::errorOccurred, this, &AudioThread::reportReplayError, Qt::QueuedConnection);
    connect(&m_player2, &QMediaPlayer::errorOccurred, this, &AudioThread::reportReplayError, Qt::QueuedConnection);
    m_doubleBufferTimer.setSingleShot(false);
    m_doubleBufferTimer.setInterval(50);
    connect(&m_doubleBufferTimer, &QTimer::timeout, this, &AudioThread::stopSecondPlayer, Qt::QueuedConnection);
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
#ifdef EnableMultimedia
    return m_audioOutput.volume();
#endif
        return 0;
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
    m_player.stop();
    m_playList.clear();
    m_player2.stop();
    m_playList2.clear();
    m_PlayListdata.clear();
    m_currentPlayer = -1;
    m_playListPostiton = -1;
    m_playListPostiton2 = -1;
}

void AudioThread::SlotPlayMusic(qint32 file)
{
    if (file >= 0 && file < m_playList.size())
    {
        m_currentPlayer = -1;
        m_playListPostiton = file;
        m_playListPostiton2 = -1;
        m_player.setSource(m_playList[m_playListPostiton]);
        m_player2.stop();
        m_player.play();
        m_currentMedia = file;
    }
    else
    {
        Console::print("AudioThread::SlotPlayMusic Trying to play unknown music index", Console::eERROR);
    }
}

void AudioThread::SlotPlayRandom()
{
    bufferAudio();
    if (m_currentPlayer == 0)
    {
        // start buffered player
        m_currentMedia = m_playListPostiton2;
        m_player2.play();
        m_currentPlayer = 1;
    }
    else
    {
        // start buffered player
        m_currentMedia = m_playListPostiton;
        m_player.play();
        m_currentPlayer = 0;
    }
    m_doubleBufferTimer.start();
}

void AudioThread::bufferAudio()
{
    if (m_currentPlayer < 0)
    {
        qint32 size = m_playList.size();
        if (size > 0)
        {
            qint32 newMedia = GlobalUtils::randIntBase(0, size - 1);
            // load buffer on second player
            qint32 newMedia2 = GlobalUtils::randIntBase(0, size - 1);
            m_playListPostiton2 = newMedia2;
            m_player2.setSource(m_playList2[m_playListPostiton2]);
            if (std::get<0>(m_PlayListdata[newMedia2]) > 0  && newMedia2 == newMedia)
            {
                m_player2.setPosition(std::get<0>(m_PlayListdata[newMedia2]));
            }
            else
            {
                m_player2.setPosition(0);
            }
            m_player2.stop();
            m_playListPostiton = newMedia;
            m_player.setSource(m_playList[m_playListPostiton]);
            if (std::get<0>(m_PlayListdata[newMedia]) > 0  &&
                m_currentPlayer >= 0 &&
                newMedia2 == newMedia)
            {
                m_player.setPosition(std::get<0>(m_PlayListdata[newMedia]));
            }
            else
            {
                m_player.setPosition(0);
            }
        }
    }
}

void AudioThread::stopSecondPlayer()
{
    qint32 size = m_playList.size();
    if (size > 0)
    {
        qint32 newMedia = GlobalUtils::randIntBase(0, size - 1);
        if (m_currentPlayer == 0)
        {
            // load buffe on current player
            m_player2.stop();
            m_playListPostiton2 = newMedia;
            m_player2.setSource(m_playList2[m_playListPostiton2]);
            if (std::get<0>(m_PlayListdata[newMedia]) > 0 && newMedia == m_playListPostiton)
            {
                m_player2.setPosition(std::get<0>(m_PlayListdata[newMedia]));
            }
        }
        else
        {
            // load buffer on current player
            m_player.stop();
            m_playListPostiton = newMedia;
            m_player.setSource(m_playList[m_playListPostiton]);
            if (std::get<0>(m_PlayListdata[newMedia]) > 0 && newMedia == m_playListPostiton2)
            {
                m_player.setPosition(std::get<0>(m_PlayListdata[newMedia]));
            }
        }
    }
    m_doubleBufferTimer.stop();
}

void AudioThread::SlotSetVolume(qint32 value)
{
    qreal sound = (static_cast<qreal>(value) / 100.0 *
                   static_cast<qreal>(Settings::getTotalVolume()) / 100.0);
    qreal volume = QAudio::convertVolume(sound, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
#ifdef EnableMultimedia
    m_audioOutput.setVolume(qRound(volume * 100));
#endif
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
        m_player.stop();
        m_playList.append(QUrl::fromLocalFile(currentPath));
        m_player2.stop();
        m_playList2.append(QUrl::fromLocalFile(currentPath));
        m_PlayListdata.append(std::tuple<qint64, qint64>(startPointMs, endPointMs));
    }
    else
    {
        Console::print("Unable to locate music file: " + currentPath, Console::eERROR);
    }
}

void AudioThread::SlotMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch (status) {
        case QMediaPlayer::EndOfMedia:
        {
            // shuffle through loaded media
            SlotPlayRandom();
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
    QString currentPath = QFileInfo(folder).absoluteFilePath();
    QDir directory(folder);
    if (directory.exists())
    {
        QStringList filter("*.mp3");
        QStringList files = directory.entryList(filter);
        for (const auto& file : files)
        {
            if (!loadedSounds.contains(file) && QFile::exists(currentPath + '/' + file))
            {
                m_playList.append(QUrl::fromLocalFile(currentPath + '/' + file));
                m_playList2.append(QUrl::fromLocalFile(currentPath + '/' + file));
                m_PlayListdata.append(std::tuple<qint64, qint64>(-1, -1));
                loadedSounds.append(file);
            }
        }
    }
    else
    {
        Console::print("Unable to locate music folder: " + currentPath, Console::eDEBUG);
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
        qint64 loopPos = std::get<1>(m_PlayListdata[m_currentMedia]);
        if ((loopPos <= duration) &&
            (loopPos > 0))
        {
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
    searchFolders.append(folder + file);
    searchFolders.append(oxygine::Resource::RCC_PREFIX_PATH + folder + file);
    for (qint32 i = searchFolders.size() - 1; i >= 0; i--)
    {
        if (QFile::exists(searchFolders[i] + "/sounds/" + file))
        {
            soundfile = searchFolders[i] + "/sounds/" + file;
            break;
        }
    }
    QUrl url = QUrl::fromLocalFile(soundfile);
    if (url.isValid())
    {
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
        // pSoundEffect->setAudioDevice(m_audioDevice);
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
