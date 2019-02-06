#include "coreengine/audiothread.h"
#include "coreengine/settings.h"
#include "coreengine/mainapp.h"

#include <QSound>
#include <QMediaPlaylist>
#include <QDir>
#include <QList>

AudioThread::AudioThread()
    : m_Player(NULL),
      m_playList(NULL)
{
    // move signals and slots to Audio Thread
    this->moveToThread(this);
    connect(this, SIGNAL(SignalPlayMusic(int)), this, SLOT(SlotPlayMusic(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(SignalSetVolume(int)), this, SLOT(SlotSetVolume(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(SignalAddMusic(QString)), this, SLOT(SlotAddMusic(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(SignalClearPlayList()), this, SLOT(SlotClearPlayList()), Qt::QueuedConnection);
    connect(this, SIGNAL(SignalPlayRandom()), this, SLOT(SlotPlayRandom()), Qt::QueuedConnection);
    connect(this, SIGNAL(SignalLoadFolder(QString)), this, SLOT(SlotLoadFolder(QString)), Qt::QueuedConnection);
}

AudioThread::~AudioThread()
{
    m_Player->stop();
    delete m_Player;
    delete m_playList;
}

void AudioThread::initAudio()
{
    // everything needs to be created in the context of this thread
    m_Player = new QMediaPlayer();
    m_playList = new QMediaPlaylist();
    m_Player->moveToThread(this);
    m_playList->moveToThread(this);
    m_Player->setPlaylist(m_playList);
    SlotSetVolume(Mainapp::getInstance()->getSettings()->getMusicVolume());
    connect(m_Player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(SlotMediaStatusChanged(QMediaPlayer::MediaStatus)));
}

void AudioThread::run()
{
    initAudio();
    while (true)
    {
        exec();
    }
}

void AudioThread::playMusic(qint32 File)
{
    emit SignalPlayMusic(File);
}

void AudioThread::setVolume(qint32 value)
{
    emit SignalSetVolume(value);
}

void AudioThread::SlotPlayMusic(qint32 File)
{
    if (m_Player != NULL)
    {
        m_Player->stop();
        m_playList->setCurrentIndex(File);
        m_Player->play();
    }
}

void AudioThread::SlotSetVolume(qint32 value)
{
    if (m_Player != NULL)
    {
        m_Player->setVolume(value);
        Mainapp::getInstance()->getSettings()->setMusicVolume(value);
    }
}

qint32 AudioThread::getVolume()
{
    return m_Player->volume();
}

void AudioThread::addMusic(const QString& File)
{
    emit SignalAddMusic(File);
}

void AudioThread::SlotAddMusic(QString File)
{
    m_Player->stop();
    m_playList->addMedia(QUrl::fromLocalFile(File));
}

void AudioThread::clearPlayList()
{
    emit SignalClearPlayList();
}

void AudioThread::SlotClearPlayList()
{
    m_Player->stop();
    m_playList->clear();
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

 void AudioThread::playRandom()
 {
     emit SignalPlayRandom();
 }

void AudioThread::SlotPlayRandom()
{
    m_Player->stop();
    qint32 size = m_playList->mediaCount();
    qint32 newMedia = Mainapp::randInt(0, size - 1);
    m_playList->setCurrentIndex(newMedia);
    m_Player->play();
    qint64 dur = m_Player->duration();
    dur--;
}

void AudioThread::loadFolder(const QString& folder)
{
    emit SignalLoadFolder(folder);
}

void AudioThread::SlotLoadFolder(QString folder)
{
    QDir directory(folder);
    QStringList filter("*.mp3");
    QStringList files = directory.entryList(filter);
    for (QStringList::const_iterator it = files.begin(); it != files.end(); it++)
    {
        m_playList->addMedia(QUrl::fromLocalFile(folder + QString("/") + *it));
    }
}

void AudioThread::SlotPlaySound(QString file)
{
    QSound::play(file);
}

