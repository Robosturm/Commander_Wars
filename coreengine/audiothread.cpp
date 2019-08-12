#include "coreengine/audiothread.h"
#include "coreengine/settings.h"
#include "coreengine/mainapp.h"

#include <QSound>
#include <QMediaPlaylist>
#include <QDir>
#include <QList>

AudioThread::AudioThread()
    : m_Player(nullptr),
      m_playList(nullptr)
{
    Interpreter::setCppOwnerShip(this);
    // move signals and slots to Audio Thread
    this->moveToThread(Mainapp::getAudioWorker());
    connect(this, &AudioThread::SignalPlayMusic,        this, &AudioThread::SlotPlayMusic, Qt::QueuedConnection);
    connect(this, &AudioThread::SignalSetVolume,        this, &AudioThread::SlotSetVolume, Qt::QueuedConnection);
    connect(this, &AudioThread::SignalAddMusic,         this, &AudioThread::SlotAddMusic, Qt::QueuedConnection);
    connect(this, &AudioThread::SignalClearPlayList,    this, &AudioThread::SlotClearPlayList, Qt::QueuedConnection);
    connect(this, &AudioThread::SignalPlayRandom,       this, &AudioThread::SlotPlayRandom, Qt::QueuedConnection);
    connect(this, &AudioThread::SignalLoadFolder,       this, &AudioThread::SlotLoadFolder, Qt::QueuedConnection);
    connect(this, &AudioThread::SignalPlaySound,        this, &AudioThread::SlotPlaySound, Qt::QueuedConnection);
    connect(this, &AudioThread::SignalStopSound,        this, &AudioThread::SlotStopSound, Qt::QueuedConnection);
    connect(this, &AudioThread::sigInitAudio,           this, &AudioThread::initAudio, Qt::QueuedConnection);
}

AudioThread::~AudioThread()
{
    m_Player->stop();
    m_Player2->stop();
    doubleBufferTimer->stop();
    delete m_Player;
    delete m_playList;
    delete m_Player2;
    delete m_playList2;
    delete doubleBufferTimer;
}

void AudioThread::initAudio()
{
    // everything needs to be created in the context of this thread
    m_Player = new QMediaPlayer();
    m_playList = new QMediaPlaylist();
    m_Player->moveToThread(Mainapp::getInstance()->getAudioWorker());
    m_playList->moveToThread(Mainapp::getInstance()->getAudioWorker());
    m_Player->setPlaylist(m_playList);
    m_Player2 = new QMediaPlayer();
    m_playList2 = new QMediaPlaylist();
    m_Player2->moveToThread(Mainapp::getInstance()->getAudioWorker());
    m_playList2->moveToThread(Mainapp::getInstance()->getAudioWorker());
    m_Player2->setPlaylist(m_playList2);

    doubleBufferTimer = new QTimer();
    doubleBufferTimer->moveToThread(Mainapp::getInstance()->getAudioWorker());

    m_Player->setNotifyInterval(200);
    m_Player2->setNotifyInterval(200);

    SlotSetVolume(static_cast<qint32>(static_cast<float>(Mainapp::getInstance()->getSettings()->getMusicVolume())));
    connect(m_Player, &QMediaPlayer::mediaStatusChanged, this, &AudioThread::SlotMediaStatusChanged);
    connect(m_Player, &QMediaPlayer::positionChanged, this, &AudioThread::SlotCheckMusicEnded);
    connect(m_Player2, &QMediaPlayer::mediaStatusChanged, this, &AudioThread::SlotMediaStatusChanged);
    connect(m_Player2, &QMediaPlayer::positionChanged, this, &AudioThread::SlotCheckMusicEnded);

    doubleBufferTimer->setSingleShot(false);
    doubleBufferTimer->setInterval(50);
    connect(doubleBufferTimer, &QTimer::timeout, this, &AudioThread::stopSecondPlayer, Qt::QueuedConnection);
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
    return m_Player->volume();
}

void AudioThread::addMusic(const QString& File, qint64 startPointMs, qint64 endPointMs)
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

void AudioThread::playSound(QString file, qint32 loops, QString folder, qint32 delay)
{
    emit SignalPlaySound(file, loops, folder, delay);
}

void AudioThread::stopSound(QString file, QString folder)
{
    emit SignalStopSound(file, folder);
}

void AudioThread::loadFolder(const QString& folder)
{
    emit SignalLoadFolder(folder);
}

void AudioThread::SlotClearPlayList()
{
    m_Player->stop();
    m_playList->clear();
    m_Player2->stop();
    m_playList2->clear();
    m_PlayListdata.clear();
    currentPlayer = -1;
}

void AudioThread::SlotPlayMusic(qint32 File)
{
    if (m_Player != nullptr)
    {
        currentPlayer = -1;
        m_playList->setCurrentIndex(File);
        if (std::get<0>(m_PlayListdata[File]) > 0)
        {
            m_Player->setPosition(std::get<0>(m_PlayListdata[File]));
        }
        m_Player2->stop();
        m_Player->play();
        currentMedia = File;
    }
}


void AudioThread::SlotPlayRandom()
{
    bufferAudio();
    if (currentPlayer == 0)
    {
        // start buffered player
        currentMedia = m_playList2->currentIndex();
        m_Player2->play();
        currentPlayer = 1;
    }
    else
    {
        // start buffered player
        currentMedia = m_playList->currentIndex();
        m_Player->play();
        currentPlayer = 0;
    }
    doubleBufferTimer->start();
}

void AudioThread::bufferAudio()
{
    if (currentPlayer < 0)
    {
        qint32 size = m_playList->mediaCount();
        if (size > 0)
        {
            qint32 newMedia = Mainapp::randInt(0, size - 1);
            // load buffer on second player
            qint32 newMedia2 = Mainapp::randInt(0, size - 1);
            m_playList2->setCurrentIndex(newMedia2);
            if (std::get<0>(m_PlayListdata[newMedia2]) > 0)
            {
                m_Player2->setPosition(std::get<0>(m_PlayListdata[newMedia2]));
            }
            m_Player2->stop();
            m_playList->setCurrentIndex(newMedia);
            if (std::get<0>(m_PlayListdata[newMedia]) > 0)
            {
                m_Player->setPosition(std::get<0>(m_PlayListdata[newMedia]));
            }
        }
    }
}

void AudioThread::stopSecondPlayer()
{
    qint32 size = m_playList->mediaCount();
    if (size > 0)
    {
        qint32 newMedia = Mainapp::randInt(0, size - 1);
        if (currentPlayer == 0)
        {
            // load buffe on current player
            m_Player2->stop();
            m_playList2->setCurrentIndex(newMedia);
            if (std::get<0>(m_PlayListdata[newMedia]) > 0)
            {
                m_Player2->setPosition(std::get<0>(m_PlayListdata[newMedia]));
            }
        }
        else
        {
            // load buffer on current player
            m_Player->stop();
            m_playList->setCurrentIndex(newMedia);
            if (std::get<0>(m_PlayListdata[newMedia]) > 0)
            {
                m_Player->setPosition(std::get<0>(m_PlayListdata[newMedia]));
            }
        }
    }
}

void AudioThread::SlotSetVolume(qint32 value)
{
    if (m_Player != nullptr)
    {
        qreal sound = (static_cast<qreal>(value) / 100.0 *
                       static_cast<qreal>(Mainapp::getInstance()->getSettings()->getTotalVolume()) / 100.0);
        qreal volume = QAudio::convertVolume(sound, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
        m_Player->setVolume(qRound(volume * 100));
        m_Player2->setVolume(qRound(volume * 100));
    }
}

void AudioThread::SlotAddMusic(QString File, qint64 startPointMs, qint64 endPointMs)
{
    m_Player->stop();
    m_playList->addMedia(QUrl::fromLocalFile(File));
    m_Player2->stop();
    m_playList2->addMedia(QUrl::fromLocalFile(File));
    m_PlayListdata.append(std::tuple<qint64, qint64>(startPointMs, endPointMs));
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
    QDir directory(folder);
    QStringList filter("*.mp3");
    QStringList files = directory.entryList(filter);
    for (QStringList::const_iterator it = files.begin(); it != files.end(); it++)
    {
        m_playList->addMedia(QUrl::fromLocalFile(folder + QString("/") + *it));
        m_playList2->addMedia(QUrl::fromLocalFile(folder + QString("/") + *it));
        m_PlayListdata.append(std::tuple<qint64, qint64>(-1, -1));
    }
}

void AudioThread::SlotCheckMusicEnded(qint64 duration)
{
    if (currentMedia >= 0 && currentMedia < m_PlayListdata.size())
    {
        qint64 loopPos = std::get<1>(m_PlayListdata[currentMedia]);
        if ((loopPos <= duration) &&
            (loopPos > 0))
        {
            // shuffle load new media
            SlotPlayRandom();
        }
    }
}

void AudioThread::SlotPlaySound(QString file, qint32 loops, QString folder, qint32 delay)
{
    qreal sound = (static_cast<qreal>(Mainapp::getInstance()->getSettings()->getSoundVolume()) / 100.0 *
                   static_cast<qreal>(Mainapp::getInstance()->getSettings()->getTotalVolume()) / 100.0);
    QUrl url = QUrl::fromLocalFile(folder + file);
    if (url.isValid())
    {
        QSoundEffect* pSoundEffect = new QSoundEffect();
        qreal value = QAudio::convertVolume(sound,
                                            QAudio::LogarithmicVolumeScale,
                                            QAudio::LinearVolumeScale);
        QTimer* pTimer = new QTimer();
        pTimer->moveToThread(Mainapp::getInstance()->getAudioWorker());
        pSoundEffect->setVolume(value);
        pSoundEffect->setSource(url);
        pSoundEffect->setLoopCount(loops);
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
