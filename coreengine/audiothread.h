#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QObject>
#include <QVector>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>
#include "3rd_party/oxygine-framework/oxygine-framework.h"

class QMediaPlaylist;

class AudioThread : public QObject
{
    Q_OBJECT
public:
    AudioThread();
    virtual ~AudioThread();

    // Qt Signals and Slots
signals:
    void sigInitAudio();
    void SignalPlayMusic(qint32 File);
    void SignalSetVolume(qint32 value);
    void SignalAddMusic(QString File, qint64 startPointMs = -1, qint64 endPointMs = -1);
    void SignalClearPlayList();
    void SignalPlayRandom();
    void SignalLoadFolder(QString folder);
    void SignalPlaySound(QString file, qint32 loops, QString folder, qint32 delay, float volume = 1.0f);
    void SignalStopSound(QString file, QString folder);
    void SignalStopAllSounds();
public slots:
    /**
     * @brief getLoadBaseGameFolders
     * @return
     */
    bool getLoadBaseGameFolders() const;
    /**
     * @brief setLoadBaseGameFolders
     * @param loadBaseGameFolders
     */
    void setLoadBaseGameFolders(bool loadBaseGameFolders);
    /**
     * @brief playMusic
     * @param File the music file to be played
     */
    void playMusic(qint32 File);
    /**
     * @brief addMusic
     * @param File adds a file to the playlist
     * @param startPointMs when this file is played the music will start at this position in ms. Values smaller 0 mean start at 0ms
     * @param endPointMs when this file is played the music will end at this position in ms and the next file will be played. Values smaller 0 mean play till end.
     */
    void addMusic(QString File, qint64 startPointMs = -1, qint64 endPointMs = -1);
    /**
     * @brief loadFolder loads all mp3 to the playlist
     * @param folder  the folder to be loaded
     */
    void loadFolder(QString folder);
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
    /**
     * @brief playSound
     * @param file
     * @param loops
     * @param folder
     */
    void playSound(QString file, qint32 loops = 1, QString folder = "resources/sounds/", qint32 delay = 0, float volume = 1.0f);
    /**
     * @brief stopSound
     * @param file
     * @param folder
     */
    void stopSound(QString file, QString folder = "resources/sounds/");
    /**
     * @brief stopAllSound
     */
    void stopAllSounds();
protected slots:
    // stops current Music and launches another one.
    void SlotPlayMusic(qint32 File);
    void SlotSetVolume(qint32 value);
    void SlotAddMusic(QString File, qint64 startPointMs = -1, qint64 endPointMs = -1);
    void SlotClearPlayList();
    void SlotMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void SlotPlayRandom();
    void SlotLoadFolder(QString folder);
    void SlotCheckMusicEnded(qint64 duration);
    // audio stuff
    void SlotPlaySound(QString file, qint32 loops, QString folder, qint32 delay, float volume = 1.0f);
    void SlotStopSound(QString file, QString folder);
    void SlotStopAllSounds();
    void SlotSoundEnded();
    void SlotSoundStart();
    void initAudio();
    /**
     * @brief stopSecondPlayer
     */
    void stopSecondPlayer();
    void bufferAudio();

    void reportReplayError(QMediaPlayer::Error error);
    void reportPlaylistErros();
protected:
    void loadMusicFolder(QString folder, QStringList& loadedSounds);
private:
    // two players one is buffering the other one is actually playing
    QMediaPlayer m_Player;
    QMediaPlaylist m_playList;
    QMediaPlayer m_Player2;
    QMediaPlaylist m_playList2;
    qint32 m_currentPlayer{-1};
    QVector<std::tuple<qint64, qint64>> m_PlayListdata;
    qint32 m_currentMedia{-1};
    QVector<QSoundEffect*> m_Sounds;
    QVector<QTimer*> m_SoundTimers;
    QTimer m_doubleBufferTimer;
    bool m_loadBaseGameFolders{true};
};

#endif // AUDIOTHREAD_H
