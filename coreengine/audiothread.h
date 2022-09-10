#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QObject>
#include <QVector>
#ifdef AUDIOSUPPORT
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioOutput>
#endif
#include <QIODevice>
#include <QTimer>

#include "coreengine/globalutils.h"

class AudioThread final : public QObject
{
    Q_OBJECT
private:
#ifdef AUDIOSUPPORT
    struct SoundData
    {
        static constexpr qint32 MAX_SAME_SOUNDS = 60;
        static constexpr qint32 DEFAULT_CACHE_SIZE = 10;
        QSoundEffect* sound[MAX_SAME_SOUNDS];
        std::shared_ptr<QTimer> timer[MAX_SAME_SOUNDS];
        qint32 nextSoundToUse = 0;
        QUrl cacheUrl;
    };
    struct Player
    {
        Player(QObject* parent)
            : m_player(parent)
        {
        }
        QMediaPlayer m_player;
        qint32 m_currentMedia{-1};
        qint32 m_nextMedia{-1};
    };
#endif
public:
    explicit AudioThread(bool noAudio);
    ~AudioThread();
    qint32 getSoundsBuffered();
    /**
     * @brief createSoundCache
     */
    void createSoundCache();

signals:
    void sigPlayMusic(qint32 File);
    void sigSetVolume(qint32 value);
    void sigAddMusic(QString File, qint64 startPointMs = -1, qint64 endPointMs = -1);
    void sigClearPlayList();
    void sigPlayRandom();
    void sigLoadFolder(QString folder);
    void sigPlaySound(QString file, qint32 loops, qint32 delay, float volume = 1.0f,
                      bool stopOldestSound = false);
    void sigStopSound(QString file);
    void sigStopAllSounds();
    void sigChangeAudioDevice(const QVariant& value);
    void sigLoadNextAudioFile();
#ifdef AUDIOSUPPORT
    void sigDeleteSound(SoundData* soundData, qint32 soundIndex);
    void sigPlayDelayedSound(SoundData* soundData, qint32 soundIndex, bool stopOldestSound);
#endif
public slots:
    /**
     * @brief initAudio
     */
    void initAudio();
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
    void playSound(QString file, qint32 loops = 1, qint32 delay = 0, float volume = 1.0f,
                   bool stopOldestSound = false);
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
    void SlotPlaySound(QString file, qint32 loops,qint32 delay, float volume = 1.0f,
                       bool stopOldestSound = false);
    void SlotStopSound(QString file);
    void SlotStopAllSounds();
    void SlotChangeAudioDevice(const QVariant& value);
    /**
     * @brief loadNextAudioFile
     * @param playerIndex
     */
    void loadNextAudioFile();
protected:
    /**
     * @brief createPlayer
     * @param player
     */
    void createPlayer();
    /**
     * @brief addMusicToTempFolder
     * @param file
     * @param startPointMs
     * @param endPointMs
     */
    void addMusicToPlaylist(QString file, qint64 startPointMs = -1, qint64 endPointMs = -1);
    /**
     * @brief clearTempFolder
     */
    void clearTempFolder();
    /**
     * @brief loadMusicFolder
     * @param folder
     * @param loadedSounds
     */
    void loadMusicFolder(QString folder, QStringList& loadedSounds);
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
#ifdef AUDIOSUPPORT
    /**
     * @brief reportReplayError
     * @param error
     * @param errorString
     */
    void reportReplayError(QMediaPlayer::Error error, const QString &errorString);
    /**
     * @brief mediaStatusChanged
     * @param status
     */
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    /**
     * @brief mediaPlaybackStateChanged
     * @param playerIndex
     * @param newState
     */
    void mediaPlaybackStateChanged(QMediaPlayer::PlaybackState newState);
    /**
     * @brief playSoundAtCachePosition
     * @param soundCache
     * @param i
     * @return
     */
    bool tryPlaySoundAtCachePosition(std::shared_ptr<SoundData> & soundCache, qint32 i,
                                     QString & file, qint32 loops, qint32 delay, qreal sound,
                                     bool stopOldestSound);
    /**
     * @brief stopSound
     * @param soundData
     * @param soundIndex
     */
    void stopSound(SoundData* soundData, qint32 soundIndex);
    /**
     * @brief stopOldestSound
     * @param soundData
     */
    void stopOldestSound(SoundData* soundData);
    /**
     * @brief stopSoundAtIndex
     * @param soundData
     * @param index
     */
    bool stopSoundAtIndex(SoundData* soundData, qint32 index);
    /**
     * @brief deleteSound
     * @param soundData
     * @param soundIndex
     */
    void deleteSound(SoundData* soundData, qint32 soundIndex);
    /**
     * @brief playDelayedSound
     * @param soundData
     * @param soundIndex
     * @param stopOldestSound
     */
    void playDelayedSound(SoundData* soundData, qint32 soundIndex, bool stopOldestSound);
#endif
    void loadMediaForFile(QString filePath);

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
        qint32 m_startpointMs{-1};
        qint32 m_endpointMs{-1};
        QString m_file;
    };
    QTimer m_positionChangedTimer;
#ifdef AUDIOSUPPORT
    QScopedPointer<Player> m_player;
    QVector<PlaylistData> m_PlayListdata;
    // sound playback data
    QMap<QString, std::shared_ptr<SoundData>> m_soundCaches;
    // general audio info
    QAudioDevice m_audioDevice;
    QAudioOutput m_audioOutput;
#endif
    bool m_loadBaseGameFolders{true};
    bool m_noAudio{false};
};

#endif // AUDIOTHREAD_H
