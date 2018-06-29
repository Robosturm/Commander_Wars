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

    /**
     * @brief playMusic
     * @param File the music file to be played
     */
    void playMusic(qint32 File);
    /**
     * @brief addMusic
     * @param File adds a file to the playlist
     */
    void addMusic(const QString& File);
    /**
     * @brief loadFolder loads all mp3 to the playlist
     * @param folder  the folder to be loaded
     */
    void loadFolder(const QString& folder);
    /**
     * @brief setVolume changes the volume between 0 and 100
     * @param value
     */
    void setVolume(qint32 value);
    /**
     * @brief getVolume
     * @return the current volume in percent 0 to 100
     */
    qint32 getVolume();
    /**
     * @brief clearPlayList clears the current playlist
     */
    void clearPlayList();
    /**
     * @brief playRandom plays a random mp3 from the playlist
     */
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
