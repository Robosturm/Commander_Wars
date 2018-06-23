#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QMediaPlayer>
#include <QThread>
#include <QObject>

class QMediaPlaylist;

class AudioThread : public QThread        
{
    Q_OBJECT
public:
    AudioThread();
    virtual ~AudioThread();


    void playMusic(qint32 File);
    void addMusic(const QString& File);
    void loadFolder(const QString& folder);
    void setVolume(qint32 value);
    qint32 getVolume();
    void clearPlayList();
    void playRandom();
protected:
    virtual void run();
private:
    QMediaPlayer* m_Player;
    QMediaPlaylist* m_playList;
    void initAudio();
    // Qt Signals and Slots
signals:
    void SignalPlayMusic(qint32 File);
    void SignalSetVolume(qint32 value);
    void SignalAddMusic(QString File);
    void SignalClearPlayList();
    void SignalPlayRandom();
    void SignalLoadFolder(QString folder);
public slots:
    // stops current Music and launches new one.
    void SlotPlayMusic(qint32 File);
    void SlotSetVolume(qint32 value);
    void SlotAddMusic(QString File);
    void SlotClearPlayList();
    void SlotMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void SlotPlayRandom();
    void SlotLoadFolder(QString folder);
};

#endif // AUDIOTHREAD_H
