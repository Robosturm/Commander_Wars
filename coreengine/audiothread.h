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

#include "coreengine/globalutils.h"
#include "3rd_party/oxygine-framework/oxygine-framework.h"

class AudioThread : public QObject
{
    Q_OBJECT
private:
    struct SoundData
    {
        static constexpr qint32 MAX_SAME_SOUNDS = 10;
        std::shared_ptr<QSoundEffect> sound[MAX_SAME_SOUNDS];
        std::shared_ptr<QTimer> timer[MAX_SAME_SOUNDS];
    };
public:
    explicit AudioThread();
    virtual ~AudioThread();
    qint32 getSoundsBuffered();
    /**
     * @brief createSoundCache
     */
    void createSoundCache();
    // Qt Signals and Slots
signals:
    void sigInitAudio();
    void sigCreateSoundCache();
    void SignalPlayMusic(qint32 File);
    void SignalSetVolume(qint32 value);
    void SignalAddMusic(QString File, qint64 startPointMs = -1, qint64 endPointMs = -1);
    void SignalClearPlayList();
    void SignalPlayRandom();
    void SignalLoadFolder(QString folder);
    void SignalPlaySound(QString file, qint32 loops, qint32 delay, float volume = 1.0f);
    void SignalStopSound(QString file);
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
     */
    void playSound(QString file, qint32 loops = 1, qint32 delay = 0, float volume = 1.0f);
    /**
     * @brief stopSound
     * @param file
     */
    void stopSound(QString file);
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
    void SlotPlayRandom();
    void SlotLoadFolder(QString folder);
    void SlotCheckMusicEnded(qint64 duration);
    // audio stuff
    void SlotPlaySound(QString file, qint32 loops,qint32 delay, float volume = 1.0f);
    void SlotStopSound(QString file);
    void SlotStopAllSounds();
    void initAudio();
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
    /**
     * @brief readSoundCacheFromXml
     * @param folder
     */
    void readSoundCacheFromXml(QString folder);
    /**
     * @brief fillCache
     * @param cache
     * @param count
     * @param file
     */
    void fillSoundCache(qint32 count, QString folder, QString file);
    /**
     * @brief reportReplayError
     * @param error
     * @param errorString
     */
    void reportReplayError(qint32 player, QMediaPlayer::Error error, const QString &errorString);
    /**
     * @brief mediaStatusChanged
     * @param status
     */
    void mediaStatusChanged(QMediaPlayer &player, qint32 playerIndex, QMediaPlayer::MediaStatus status);
    /**
     * @brief mediaPlaybackStateChanged
     * @param playerIndex
     * @param newState
     */
    void mediaPlaybackStateChanged(qint32 playerIndex, QMediaPlayer::PlaybackState newState);
    /**
     * @brief qtWorkaround
     */
    void qtWorkaround();
private:
    // music playback data
    struct PlaylistData
    {
        explicit PlaylistData(QString file, qint32 startpointMs = -1, qint32 endpointMs = -1)
            : m_startpointMs(startpointMs),
              m_endpointMs(endpointMs),
              m_file(file)
        {
        }
        QUrl getUrl()
        {
            return GlobalUtils::getUrlForFile(m_file);
        }
        qint32 m_startpointMs{-1};
        qint32 m_endpointMs{-1};
        QString m_file;
    };
    struct Player
    {
        Player(QObject* parent)
            : m_player(parent),
              m_playerFile(parent)
        {
        }
        QFile m_playerFile;
        QMediaPlayer m_player;
        qint32 m_playerStartPosition{0};
        qint32 m_playListPostiton{-1};
    };
    QScopedPointer<Player> m_player[2];
    QVector<PlaylistData> m_PlayListdata;
    qint32 m_currentPlayer{-1};
    qint32 m_currentMedia{-1};
    QTimer m_positionUpdateTimer;
    // sound playback data
    QMap<QString, std::shared_ptr<SoundData>> m_soundCaches;
    // general audio info
    QAudioDevice m_audioDevice;
    QAudioOutput m_audioOutput;
    bool m_loadBaseGameFolders{true};
};

#endif // AUDIOTHREAD_H
