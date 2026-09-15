#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QString>
#include <QTimer>
#include <QUrl>
#include <vector>
#include <mutex>
#include <atomic>

#ifdef AUDIOSUPPORT
#include <portaudio.h>
#endif

#include "coreengine/memorymanagement.h"

class AudioManager;
using spAudioManager = std::shared_ptr<AudioManager>;

struct SoundData : public QObject
{
    static constexpr qint32 MAX_SAME_SOUNDS = 60;
    static constexpr qint32 DEFAULT_CACHE_SIZE = 10;
    QUrl cacheUrl;
    QString m_filePath;
    qint32 m_maxUseCount{0};
    qint32 m_currentUseCount{0};
    std::vector<float> m_samples; // Interleaved stereo float PCM
    qint64 m_totalFrames{0};
    SoundData();
};
using spSoundData = std::shared_ptr<SoundData>;

Q_DECLARE_INTERFACE(SoundData, "SoundData");

class AudioManager final : public QObject
{
    static constexpr qint32 MAX_PARALLEL_SOUNDS = 200;
    Q_OBJECT

public:
    explicit AudioManager(bool noAudio, bool useAudioThread);
    virtual ~AudioManager() = default;

    /**
     * @brief getLoadBaseGameFolders
     * @return
     */
    Q_INVOKABLE bool getLoadBaseGameFolders() const;

    /**
     * @brief setLoadBaseGameFolders
     * @param loadBaseGameFolders
     */
    Q_INVOKABLE void setLoadBaseGameFolders(bool loadBaseGameFolders);

signals:
    void sigCreateSoundCache();
    void sigInitAudio();
    void sigPlayMusic(qint32 File);
    void sigSetVolume(qint32 value);
    void sigAddMusic(QString File, qint64 startPointMs = -1, qint64 endPointMs = -1);
    void sigClearPlayList();
    void sigPlayRandom();
    void sigLoadFolder(QString folder);
    void sigPlaySound(QString file, qint32 loops, qint32 delay, float volume = 1.0f,
                      bool stopOldestSound = false, qint32 duration = -1);
    void sigStopSound(QString file);
    void sigStopAllSounds();
    void sigChangeAudioDevice(const QVariant value);
    void sigLoadNextAudioFile();
    void sigStopAudio();
    void sigSetMuteInternal(bool value);
    void sigContinueMusic(QString file, qint32 position = -1);
    void sigClearMusicPositions();

public slots:
    /**
     * @brief createSoundCache
     */
    void createSoundCache();

    /**
     * @brief initAudio
     */
    void initAudio();

    /**
     * @brief stopAudio
     */
    void stopAudio();

    /**
     * @brief playMusic
     * @param File the music file to be played
     */
    void playMusic(qint32 File);

    /**
     * @brief continueMusic if file is empty and exactly one music is in the playlist that music is continued else a random music is started from 0.
     * If file isn't empty the game tries to continue the given music.
     * Note: In all cases the music must be in the playlist else the function does nothing.
     * @param file
     * @param position
     */
    void continueMusic(QString file, qint32 position = -1);

    /**
     * @brief addMusic
     * @param File adds a file to the playlist
     * @param startPointMs when this file is played the music will start at this position in ms. Values smaller 0 mean start at 0ms
     * @param endPointMs when this file is played the music will end at this position in ms and the next file will be played. Values smaller 0 mean play till end.
     */
    void addMusic(QString File, qint64 startPointMs = -1, qint64 endPointMs = -1);

    /**
     * @brief loadFolder loads all mp3/ogg/wav to the playlist
     * @param folder the folder to be loaded
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
     * @brief playRandom plays a random track from the playlist
     */
    void playRandom();

    /**
     * @brief playSound
     * @param file
     * @param loops
     */
    void playSound(QString file, qint32 loops = 1, qint32 delay = 0, float volume = 1.0f,
                   bool stopOldestSound = false, qint32 duration = -1);

    /**
     * @brief stopSound
     * @param file
     */
    void stopSound(QString file);

    /**
     * @brief stopAllSounds
     */
    void stopAllSounds();

    /**
     * @brief changeAudioDevice
     * @param value
     */
    void changeAudioDevice(const QVariant &value);

    /**
     * @brief SlotClearMusicPositions
     */
    void clearMusicPositions();

protected slots:
    // stops current Music and launches another one.
    void SlotContinueMusic(QString file, qint32 position = -1);
    void SlotPlayMusic(qint32 file);
    void SlotSetVolume(qint32 value);
    void SlotAddMusic(QString file, qint64 startPointMs = -1, qint64 endPointMs = -1);
    void SlotClearPlayList();
    void SlotPlayRandom();
    void SlotLoadFolder(QString folder);
    void SlotCheckMusicEnded(qint64 duration);
    // audio stuff
    void SlotPlaySound(QString file, qint32 loops, qint32 delay, float volume = 1.0f,
                       bool stopOldestSound = false, qint32 duration = -1);
    void SlotStopSound(QString file);
    void SlotStopAllSounds();
    void SlotChangeAudioDevice(const QVariant value);
    void slotSetMuteInternal(bool value);

    /**
     * @brief SlotClearMusicPositions
     */
    void SlotClearMusicPositions();

    /**
     * @brief loadNextAudioFile
     */
    void loadNextAudioFile();

protected:
    void addMusicToPlaylist(const QString & file, qint64 startPointMs = -1, qint64 endPointMs = -1);
    void clearTempFolder();
    void loadMusicFolder(const QString & folder, QStringList &loadedSounds);
    void readSoundCacheFromXml(QString folder);
    void fillSoundCache(qint32 count, QString folder, QString file);
    void loadMediaForFile(QString filePath, qint32 position = 0);
    bool openStream(const QString& deviceName);

#ifdef AUDIOSUPPORT
    static int paCallback(const void* inputBuffer, void* outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void* userData);
#endif

private:
    struct PlaylistData
    {
        explicit PlaylistData(const QString & file, qint32 startpointMs = -1, qint32 endpointMs = -1)
            : m_startpointMs(startpointMs),
              m_endpointMs(endpointMs),
              m_file(file)
        {
        }
        qint32 m_startpointMs{-1};
        qint32 m_endpointMs{-1};
        QString m_file;
    };
    bool tryAddMusic(QString file, qint64 startPointMs, qint64 endPointMs);

#ifdef AUDIOSUPPORT
    struct SoundVoice
    {
        spSoundData soundData;
        qint64 currentFrame{0};
        qint32 remainingLoops{1};
        qint32 delayFrames{0};
        qint32 remainingDurationFrames{-1};
        float volume{1.0f};
        bool active{false};
        qint64 age{0};
    };

    struct MusicState
    {
        std::vector<float> samples;
        qint64 currentFrame{0};
        qint64 totalFrames{0};
        qint64 loopStartFrame{0};
        qint64 loopEndFrame{0};
        bool isPlaying{false};
        float volume{1.0f};
        QString currentFile;
        qint32 currentMediaIndex{-1};
        qint32 nextMediaIndex{-1};
    };

    PaStream* m_paStream{nullptr};
    qint32 m_sampleRate{44100};
    std::mutex m_audioMutex;

    SoundVoice m_soundVoices[MAX_PARALLEL_SOUNDS];
    qint64 m_voiceCounter{0};

    MusicState m_musicState;
    std::atomic<bool> m_trackEndedFlag{false};
    QTimer m_pollTimer;

    float m_musicVolume{1.0f};
    float m_soundVolume{1.0f};
    float m_totalVolume{1.0f};
    bool m_isMuted{false};

    QVector<PlaylistData> m_PlayListdata;
    QMap<QString, spSoundData> m_soundCaches;
    QMap<QString, qint32> m_musicPlayPositionCache;
#endif

    bool m_loadBaseGameFolders{true};
    bool m_noAudio{false};
    bool m_internalMuted{false};
};

Q_DECLARE_INTERFACE(AudioManager, "AudioManager");

#endif // AUDIOTHREAD_H
