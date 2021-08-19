#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QObject>
#include <QVector>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QIODevice>
#include <QTimer>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

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
    void SignalChangeAudioDevice(const QVariant& value);
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
    /**
     * @brief changeAudioDevice
     * @param value
     */
    void changeAudioDevice(const QVariant& value);
protected slots:
    // stops current Music and launches another one.
    void SlotPlayMusic(qint32 file);
    void SlotSetVolume(qint32 value);
    void SlotAddMusic(QString file, qint64 startPointMs = -1, qint64 endPointMs = -1);
    void SlotClearPlayList();

    /**
     * @brief mediaStatusChanged
     * @param status
     */
    void mediaStatusChanged(QMediaPlayer &player, qint32 playerIndex, QMediaPlayer::MediaStatus status);

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
     * @brief reportReplayError
     * @param error
     * @param errorString
     */
    void reportReplayError(QMediaPlayer::Error error, const QString &errorString);
    void SlotChangeAudioDevice(const QVariant& value);
protected:
    /**
     * @brief loadMusicFolder
     * @param folder
     * @param loadedSounds
     */
    void loadMusicFolder(QString folder, QStringList& loadedSounds);
    /**
     * @brief bufferOtherPlayer
     */
    void bufferOtherPlayer();
    /**
     * @brief initialAudioBuffering
     */
    void initialAudioBuffering();
private:
    // music playback data
    struct PlaylistData
    {
        explicit PlaylistData(QString file)
            : m_file(file)
        {
        }
        explicit PlaylistData(QString file, qint32 startpointMs, qint32 endpointMs)
            : m_startpointMs(startpointMs),
              m_endpointMs(endpointMs),
              m_file(file)
        {
        }
        qint32 m_startpointMs{-1};
        qint32 m_endpointMs{-1};
        QString m_file;
    };
    struct Player
    {
        Player(QObject* parent)
            : m_playerFile(parent),
              m_player(parent)
        {
        }
        QFile m_playerFile;
        QMediaPlayer m_player;
        qint32 m_playerStartPosition{0};
        qint32 m_playListPostiton{-1};
    };
    Player m_player[2];
    QVector<PlaylistData> m_PlayListdata;
    qint32 m_currentPlayer{-1};
    qint32 m_currentMedia{-1};
    QTimer m_positionUpdateTimer;
    // sound playback data
    QVector<QSoundEffect*> m_Sounds;
    QVector<QTimer*> m_SoundTimers;
    // general audio info
    QAudioDevice m_audioDevice;
    QAudioOutput m_audioOutput;
    bool m_loadBaseGameFolders{true};
};

#endif // AUDIOTHREAD_H
