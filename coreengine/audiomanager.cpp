#include "coreengine/audiomanager.h"
#ifdef AUDIOSUPPORT
    #include "coreengine/audiodecoder.h"
#endif
#include "coreengine/settings.h"
#include "coreengine/mainapp.h"
#include "coreengine/gameconsole.h"
#include "coreengine/interpreter.h"
#include "coreengine/globalutils.h"
#include "coreengine/virtualpaths.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QtGlobal>
#include <QDirIterator>
#include <QList>
#include <QDomDocument>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <limits>

namespace
{
    /** poll timer ticks between two device checks, the timer itself runs at 50 ms for track ends */
    constexpr qint32 DEVICE_CHECK_TICK_INTERVAL = 200;
    constexpr qint32 MAX_FAILED_REOPEN_ATTEMPTS = 5;
    /** if the callback didn't run for this long the stream is considered dead, e.g. after a debugger suspended the process */
    constexpr qint64 STREAM_STALL_TIMEOUT_MS = 500;
    /** short ramp after (re)opening a stream to avoid clicks */
    constexpr qint64 FADE_IN_MS = 20;
    /** lower bound for the output buffer latency, the device "low latency" values are prone to underruns (crackling) */
    constexpr double MIN_OUTPUT_LATENCY_S = 0.08;
    /** samples above this level are compressed smoothly instead of being hard clipped */
    constexpr float SOFT_CLIP_THRESHOLD = 0.8f;

    inline float softClip(float x)
    {
        const float absX = std::fabs(x);
        if (absX <= SOFT_CLIP_THRESHOLD)
        {
            return x;
        }
        constexpr float range = 1.0f - SOFT_CLIP_THRESHOLD;
        const float compressed = SOFT_CLIP_THRESHOLD + range * std::tanh((absX - SOFT_CLIP_THRESHOLD) / range);
        return std::copysign(compressed, x);
    }
}

SoundData::SoundData()
{
#ifdef GRAPHICSUPPORT
    setObjectName("SoundData");
#endif
    Interpreter::setCppOwnerShip(this);
}

AudioManager::AudioManager(bool noAudio, bool useAudioThread)
    : m_noAudio(noAudio)
#ifdef GRAPHICSUPPORT
    , m_pollTimer(this)
#endif
{
#ifdef GRAPHICSUPPORT
    setObjectName("AudioThread");
#endif
    Interpreter::setCppOwnerShip(this);
    if (!m_noAudio)
    {
#ifdef AUDIOSUPPORT
        connect(this, &AudioManager::sigPlayMusic,         this, &AudioManager::SlotPlayMusic, Qt::QueuedConnection);
        connect(this, &AudioManager::sigSetVolume,         this, &AudioManager::SlotSetVolume, Qt::QueuedConnection);
        connect(this, &AudioManager::sigAddMusic,          this, &AudioManager::SlotAddMusic, Qt::QueuedConnection);
        connect(this, &AudioManager::sigClearPlayList,     this, &AudioManager::SlotClearPlayList, Qt::QueuedConnection);
        connect(this, &AudioManager::sigPlayRandom,        this, &AudioManager::SlotPlayRandom, Qt::QueuedConnection);
        connect(this, &AudioManager::sigLoadFolder,        this, &AudioManager::SlotLoadFolder, Qt::QueuedConnection);
        connect(this, &AudioManager::sigPlaySound,         this, &AudioManager::SlotPlaySound, Qt::QueuedConnection);
        connect(this, &AudioManager::sigStopSound,         this, &AudioManager::SlotStopSound, Qt::QueuedConnection);
        connect(this, &AudioManager::sigStopAllSounds,     this, &AudioManager::SlotStopAllSounds, Qt::QueuedConnection);
        connect(this, &AudioManager::sigChangeAudioDevice, this, &AudioManager::SlotChangeAudioDevice, Qt::QueuedConnection);
        connect(this, &AudioManager::sigLoadNextAudioFile, this, &AudioManager::loadNextAudioFile, Qt::QueuedConnection);
        connect(this, &AudioManager::sigSetMuteInternal,   this, &AudioManager::slotSetMuteInternal, Qt::QueuedConnection);
        connect(this, &AudioManager::sigContinueMusic,     this, &AudioManager::SlotContinueMusic, Qt::QueuedConnection);
        connect(this, &AudioManager::sigClearMusicPositions, this, &AudioManager::SlotClearMusicPositions, Qt::QueuedConnection);

        // sync startup and stop signals and slots
        auto connectionType = Qt::AutoConnection;
        if (useAudioThread)
        {
            connectionType = Qt::BlockingQueuedConnection;
        }
        connect(this, &AudioManager::sigInitAudio,         this, &AudioManager::initAudio, connectionType);
        connect(this, &AudioManager::sigStopAudio,         this, &AudioManager::stopAudio, connectionType);
        connect(this, &AudioManager::sigCreateSoundCache,  this, &AudioManager::createSoundCache, connectionType);
#endif
    }
}

void AudioManager::stopAudio()
{
#ifdef AUDIOSUPPORT
    if (Mainapp::getInstance()->isAudioThread())
    {
        CONSOLE_PRINT_MODULE("Stopping audio", GameConsole::eDEBUG, GameConsole::eAudio);
        m_pollTimer.stop();
        m_lastCallbackTimeMs.store(0, std::memory_order_relaxed);
        if (m_paStream)
        {
            Pa_StopStream(m_paStream);
            Pa_CloseStream(m_paStream);
            m_paStream = nullptr;
        }
        Pa_Terminate();
        m_soundCaches.clear();
        disconnect();
    }
    else
    {
        emit sigStopAudio();
    }
#endif
}

void AudioManager::initAudio()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        if (Mainapp::getInstance()->isAudioThread())
        {
            CONSOLE_PRINT_MODULE("AudioManager::initAudio", GameConsole::eDEBUG, GameConsole::eAudio);
            PaError err = Pa_Initialize();
            if (err != paNoError)
            {
                CONSOLE_PRINT("Pa_Initialize failed: " + QString::fromUtf8(Pa_GetErrorText(err)), GameConsole::eERROR);
                return;
            }
            else
            {
                CONSOLE_PRINT_MODULE("Pa_Initialize succeeded", GameConsole::eDEBUG, GameConsole::eAudio);
            }

            const auto value = Settings::getInstance()->getAudioOutput();
            QString deviceName = value.toString();
            if (deviceName.isEmpty())
            {
                deviceName = Settings::DEFAULT_AUDIODEVICE;
            }
            CONSOLE_PRINT_MODULE("Open audio stream with device: " + deviceName, GameConsole::eDEBUG, GameConsole::eAudio);
            openStream(deviceName);

            m_pollTimer.setInterval(50);
            connect(&m_pollTimer, &QTimer::timeout, this, [this]()
            {
                if (m_trackEndedFlag.exchange(false))
                {
                    SlotPlayRandom();
                }
                checkStreamStalled();
                ++m_deviceCheckTicks;
                if (m_deviceCheckTicks >= DEVICE_CHECK_TICK_INTERVAL)
                {
                    m_deviceCheckTicks = 0;
                    checkAudioDeviceChanged();
                }
            });
            m_pollTimer.start();

            SlotSetVolume(static_cast<qint32>(static_cast<float>(Settings::getInstance()->getMusicVolume())));
        }
        else
        {
            emit sigInitAudio();
        }
    }
#endif
}

bool AudioManager::openStream(const QString& deviceName)
{
#ifdef AUDIOSUPPORT
    CONSOLE_PRINT_MODULE("AudioManager::openStream for device: " + deviceName, GameConsole::eDEBUG, GameConsole::eAudio); 
    m_currentDeviceName = deviceName;
    if (m_paStream)
    {
        Pa_StopStream(m_paStream);
        Pa_CloseStream(m_paStream);
        m_paStream = nullptr;
    }

    PaDeviceIndex targetDevice = paNoDevice;
    if (deviceName == Settings::DEFAULT_AUDIODEVICE || deviceName.isEmpty())
    {
        targetDevice = Pa_GetDefaultOutputDevice();
    }
    else
    {
        qint32 numDevices = Pa_GetDeviceCount();
        for (qint32 i = 0; i < numDevices; ++i)
        {
            const PaDeviceInfo* devInfo = Pa_GetDeviceInfo(i);
            if (devInfo && devInfo->maxOutputChannels > 0 && QString::fromUtf8(devInfo->name) == deviceName)
            {
                targetDevice = i;
                break;
            }
        }
        if (targetDevice == paNoDevice)
        {
            targetDevice = Pa_GetDefaultOutputDevice();
        }
    }

    if (targetDevice == paNoDevice)
    {
        CONSOLE_PRINT_MODULE("No PortAudio output device found", GameConsole::eERROR, GameConsole::eAudio);
        return false;
    }
    m_lastDefaultDevice = Pa_GetDefaultOutputDevice();

    const PaDeviceInfo* devInfo = Pa_GetDeviceInfo(targetDevice);
    if (devInfo)
    {
        CONSOLE_PRINT_MODULE("Opening audio device: " + QString::fromUtf8(devInfo->name), GameConsole::eDEBUG, GameConsole::eAudio);
        m_sampleRate = (devInfo->defaultSampleRate > 0) ? static_cast<qint32>(devInfo->defaultSampleRate) : 44100;
    }
    else
    {
        m_sampleRate = 44100;
    }

    PaStreamParameters outParams;
    outParams.device = targetDevice;
    outParams.channelCount = 2;
    outParams.sampleFormat = paFloat32;
    outParams.suggestedLatency = devInfo ? std::max(devInfo->defaultHighOutputLatency, MIN_OUTPUT_LATENCY_S) : MIN_OUTPUT_LATENCY_S;
    outParams.hostApiSpecificStreamInfo = nullptr;

    PaError err = Pa_OpenStream(
        &m_paStream,
        nullptr,
        &outParams,
        m_sampleRate,
        paFramesPerBufferUnspecified,
        paPrimeOutputBuffersUsingStreamCallback, // avoids playing uninitialized buffer contents (crackling) on stream start
        &AudioManager::paCallback,
        this
    );

    if (err != paNoError)
    {
        CONSOLE_PRINT_MODULE("Pa_OpenStream failed: " + QString::fromUtf8(Pa_GetErrorText(err)), GameConsole::eERROR, GameConsole::eAudio);
        return false;
    }

    err = Pa_StartStream(m_paStream);
    if (err != paNoError)
    {
        CONSOLE_PRINT_MODULE("Pa_StartStream failed: " + QString::fromUtf8(Pa_GetErrorText(err)), GameConsole::eERROR, GameConsole::eAudio);
        return false;
    }
    m_fadeInFramesTotal = (FADE_IN_MS * m_sampleRate) / 1000;
    m_fadeInFramesRemaining.store(m_fadeInFramesTotal, std::memory_order_relaxed);
    m_lastCallbackTimeMs.store(monotonicMs(), std::memory_order_relaxed);

    return true;
#else
    Q_UNUSED(deviceName);
    return false;
#endif
}

#ifdef AUDIOSUPPORT
qint64 AudioManager::monotonicMs()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

bool AudioManager::mixMusic(float* out, qint64 frames, float totalVolume)
{
    if (!m_musicState.isPlaying.load(std::memory_order_relaxed))
    {
        return false;
    }
    std::unique_lock<std::mutex> lock(m_musicMutex, std::try_to_lock);
    if (!lock.owns_lock())
    {
        return false;
    }
    auto & ms = m_musicState;
    if (!ms.isPlaying.load(std::memory_order_relaxed) || ms.samples.empty() || ms.totalFrames <= 0)
    {
        return false;
    }
    const float musicVol = ms.volume * m_musicVolume.load(std::memory_order_relaxed) * totalVolume;
    const float * samples = ms.samples.data();
    const qint64 endFrame = (ms.loopEndFrame > 0 && ms.loopEndFrame < ms.totalFrames) ? ms.loopEndFrame : ms.totalFrames;
    const qint64 startFrame = (ms.loopStartFrame >= 0 && ms.loopStartFrame < endFrame) ? ms.loopStartFrame : 0;
    qint64 written = 0;
    bool mixed = false;
    while (written < frames)
    {
        if (ms.currentFrame >= endFrame || ms.currentFrame < 0)
        {
            const qint32 current = ms.currentMediaIndex.load(std::memory_order_relaxed);
            const qint32 next = ms.nextMediaIndex.load(std::memory_order_relaxed);
            if (current == next || next < 0)
            {
                ms.currentFrame = startFrame;
            }
            else
            {
                ms.isPlaying.store(false, std::memory_order_relaxed);
                m_trackEndedFlag.store(true, std::memory_order_relaxed);
                break;
            }
        }
        const qint64 chunk = std::min(frames - written, endFrame - ms.currentFrame);
        const float * src = samples + ms.currentFrame * 2;
        float * dst = out + written * 2;
        for (qint64 i = 0; i < chunk * 2; ++i)
        {
            dst[i] += src[i] * musicVol;
        }
        ms.currentFrame += chunk;
        written += chunk;
        mixed = true;
    }
    return mixed;
}

bool AudioManager::mixSounds(float* out, qint64 frames, float totalVolume)
{
    const float sfxBaseVol = m_soundVolume.load(std::memory_order_relaxed) * totalVolume;
    bool mixed = false;
    for (qint32 v = 0; v < MAX_PARALLEL_SOUNDS; ++v)
    {
        auto & voice = m_soundVoices[v];
        if (!voice.active.load(std::memory_order_acquire))
        {
            continue;
        }
        // a voice that is currently being modified is skipped instead of stalling the whole mix
        std::unique_lock<std::mutex> lock(voice.mutex, std::try_to_lock);
        if (!lock.owns_lock() || !voice.active.load(std::memory_order_relaxed) || voice.soundData == nullptr)
        {
            continue;
        }
        const SoundData & data = *voice.soundData;
        const qint64 totalFrames = data.m_totalFrames;
        if (totalFrames <= 0 || data.m_samples.empty())
        {
            continue;
        }

        qint64 written = 0;
        if (voice.delayFrames > 0)
        {
            const qint64 skipped = std::min(voice.delayFrames, frames);
            voice.delayFrames -= skipped;
            written = skipped;
        }
        const float voiceVol = voice.volume * sfxBaseVol;
        const float * samples = data.m_samples.data();
        bool finished = false;
        while (written < frames)
        {
            if (voice.currentFrame >= totalFrames)
            {
                if (voice.remainingLoops > 1)
                {
                    --voice.remainingLoops;
                    voice.currentFrame = 0;
                }
                else if (voice.remainingLoops < 0)
                {
                    voice.currentFrame = 0;
                }
                else
                {
                    finished = true;
                    break;
                }
            }
            qint64 chunk = std::min(frames - written, totalFrames - voice.currentFrame);
            if (voice.remainingDurationFrames > 0)
            {
                chunk = std::min(chunk, voice.remainingDurationFrames);
            }
            const float * src = samples + voice.currentFrame * 2;
            float * dst = out + written * 2;
            for (qint64 i = 0; i < chunk * 2; ++i)
            {
                dst[i] += src[i] * voiceVol;
            }
            voice.currentFrame += chunk;
            written += chunk;
            mixed = true;
            if (voice.remainingDurationFrames > 0)
            {
                voice.remainingDurationFrames -= chunk;
                if (voice.remainingDurationFrames <= 0)
                {
                    finished = true;
                    break;
                }
            }
        }
        if (finished)
        {
            voice.active.store(false, std::memory_order_release);
            auto & useCount = voice.soundData->m_currentUseCount;
            qint32 current = useCount.load(std::memory_order_relaxed);
            while (current > 0 && !useCount.compare_exchange_weak(current, current - 1, std::memory_order_relaxed))
            {
            }
        }
    }
    return mixed;
}

int AudioManager::paCallback(const void* inputBuffer, void* outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void* userData)
{
    Q_UNUSED(inputBuffer);
    Q_UNUSED(timeInfo);
    Q_UNUSED(statusFlags);
    auto* self = static_cast<AudioManager*>(userData);
    float* out = static_cast<float*>(outputBuffer);
    if (out == nullptr)
    {
        return paContinue;
    }
    const qint64 frames = static_cast<qint64>(framesPerBuffer);
    std::fill(out, out + frames * 2, 0.0f);
    self->m_lastCallbackTimeMs.store(monotonicMs(), std::memory_order_relaxed);

    if (self->m_isMuted.load(std::memory_order_relaxed) || self->m_internalMuted.load(std::memory_order_relaxed))
    {
        return paContinue;
    }
    const float totalVol = self->m_totalVolume.load(std::memory_order_relaxed);
    if (totalVol <= 0.0f)
    {
        return paContinue;
    }

    bool mixed = self->mixMusic(out, frames, totalVol);
    mixed = self->mixSounds(out, frames, totalVol) || mixed;
    if (!mixed)
    {
        return paContinue;
    }

    qint64 fadeIn = self->m_fadeInFramesRemaining.load(std::memory_order_relaxed);
    if (fadeIn > 0)
    {
        const float fadeTotal = static_cast<float>(self->m_fadeInFramesTotal);
        const qint64 fadeFrames = std::min(fadeIn, frames);
        for (qint64 i = 0; i < fadeFrames; ++i)
        {
            const float gain = 1.0f - static_cast<float>(fadeIn - i) / fadeTotal;
            out[i * 2 + 0] *= gain;
            out[i * 2 + 1] *= gain;
        }
        self->m_fadeInFramesRemaining.store(fadeIn - fadeFrames, std::memory_order_relaxed);
    }

    for (qint64 i = 0; i < frames * 2; ++i)
    {
        out[i] = softClip(out[i]);
    }

    return paContinue;
}
#endif

void AudioManager::createSoundCache()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        if (Mainapp::getInstance()->isAudioThread())
        {
            auto searchPath = VirtualPaths::createSearchPathRev("resources/sounds/");
            for (qint32 i = 0; i < searchPath.length(); ++i)
            {
                QString folder = searchPath[i];
                if (QFile::exists(folder + "res.xml"))
                {
                    readSoundCacheFromXml(folder);
                }
                QStringList filter;
                filter << "*.wav";
                QDirIterator dirIter(folder, filter, QDir::Files, QDirIterator::Subdirectories);
                while (dirIter.hasNext())
                {
                    dirIter.next();
                    QString file = dirIter.fileName();
                    if (!m_soundCaches.contains(file))
                    {
                        fillSoundCache(SoundData::DEFAULT_CACHE_SIZE, folder, file);
                    }
                }
            }
        }
        else
        {
            emit sigCreateSoundCache();
        }
    }
#endif
}

void AudioManager::readSoundCacheFromXml(QString folder)
{
    if (!m_noAudio)
    {
        CONSOLE_PRINT_MODULE("Loading sound cache from " + folder + "res.xml", GameConsole::eDEBUG, GameConsole::eAudio);
        QDomDocument document;
        QFile file(folder + "res.xml");
        if (file.open(QIODevice::ReadOnly))
        {
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
            auto result = document.setContent(&file);
            bool loaded = static_cast<bool>(result);
            QString errorMessage = result.errorMessage;
            qsizetype errorLine = result.errorLine;
            qsizetype errorColumn = result.errorColumn;
#else
            QString errorMessage;
            int errorLine = 0;
            int errorColumn = 0;
            bool loaded = document.setContent(&file, &errorMessage, &errorLine, &errorColumn);
#endif
            if (loaded)
            {
                auto rootElement = document.documentElement();
                auto node = rootElement.firstChild();
                while (!node.isNull())
                {
                    while (node.isComment())
                    {
                        node = node.nextSibling();
                    }
                    if (!node.isNull())
                    {
                        if (node.nodeName() == "sound")
                        {
                            auto element = node.toElement();
                            QString soundFile = element.attribute("file");
                            qint32 cacheSize = element.attribute("cachesize").toInt();
                            fillSoundCache(cacheSize, folder, soundFile);
                        }
                    }
                    node = node.nextSibling();
                }
            }
            else
            {
                CONSOLE_PRINT_MODULE("Unable to load: " + folder + "res.xml", GameConsole::eERROR, GameConsole::eAudio);
                CONSOLE_PRINT_MODULE("Error: " + errorMessage + " at line " + QString::number(errorLine) + " at column " + QString::number(errorColumn), GameConsole::eERROR, GameConsole::eAudio);
            }
        }
    }
}

void AudioManager::fillSoundCache(qint32 count, QString folder, QString file)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        if (count > SoundData::MAX_SAME_SOUNDS)
        {
            count = SoundData::MAX_SAME_SOUNDS;
        }
        QString filePath = folder + file;
        QFile soundFile(filePath);
        if (soundFile.open(QIODevice::ReadOnly))
        {
            QByteArray data = soundFile.readAll();
            DecodedAudio decoded = AudioDecoder::decode(data, filePath, m_sampleRate);
            if (decoded.isValid())
            {
                spSoundData cache = MemoryManagement::create<SoundData>();
                cache->m_filePath = filePath;
                cache->cacheUrl = GlobalUtils::getUrlForFile(filePath);
                cache->m_maxUseCount = count;
                cache->m_samples = std::move(decoded.samples);
                cache->m_totalFrames = decoded.totalFrames;
                CONSOLE_PRINT_MODULE("Caching sound " + filePath + " with amount " + QString::number(count), GameConsole::eDEBUG, GameConsole::eAudio);
                m_soundCaches.insert(file, cache);
            }
            else
            {
                CONSOLE_PRINT_MODULE("Failed to decode sound " + filePath, GameConsole::eERROR, GameConsole::eResources);
            }
        }
        else
        {
            CONSOLE_PRINT_MODULE("Unable to find sound " + filePath + " with amount " + QString::number(count), GameConsole::eERROR, GameConsole::eResources);
        }
    }
#endif
}

void AudioManager::changeAudioDevice(const QVariant& value)
{
    if (Mainapp::getInstance()->isAudioThread())
    {
        SlotChangeAudioDevice(value);
    }
    else
    {
        emit sigChangeAudioDevice(value);
    }
}

void AudioManager::SlotChangeAudioDevice(const QVariant value)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        QString deviceName = value.toString();
        CONSOLE_PRINT_MODULE("Changing to audio device: " + deviceName, GameConsole::eDEBUG, GameConsole::eAudio);
        m_failedReopenAttempts = 0;
        openStream(deviceName);
    }
#endif
}

void AudioManager::checkAudioDeviceChanged()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio && Mainapp::getInstance()->isAudioThread())
    {
        if (m_failedReopenAttempts >= MAX_FAILED_REOPEN_ATTEMPTS)
        {
            return;
        }
        bool streamBroken = (m_paStream == nullptr) || (Pa_IsStreamStopped(m_paStream) == 1);
        bool usingDefaultDevice = (m_currentDeviceName == Settings::DEFAULT_AUDIODEVICE || m_currentDeviceName.isEmpty());
        PaDeviceIndex currentDefault = Pa_GetDefaultOutputDevice();
        bool defaultChanged = usingDefaultDevice && (currentDefault != paNoDevice) && (currentDefault != m_lastDefaultDevice);
        if (streamBroken || defaultChanged)
        {
            CONSOLE_PRINT_MODULE("Audio device changed or stream stopped unexpectedly, reopening stream", GameConsole::eDEBUG, GameConsole::eAudio);
            if (openStream(m_currentDeviceName))
            {
                m_failedReopenAttempts = 0;
            }
            else
            {
                ++m_failedReopenAttempts;
                m_lastDefaultDevice = currentDefault;
                if (m_failedReopenAttempts >= MAX_FAILED_REOPEN_ATTEMPTS)
                {
                    CONSOLE_PRINT_MODULE("Giving up reopening the audio stream after " + QString::number(m_failedReopenAttempts) +
                                         " attempts. Audio stays disabled until the device is changed.", GameConsole::eWARNING, GameConsole::eAudio);
                }
            }
        }
    }
#endif
}


void AudioManager::checkStreamStalled()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio && m_paStream != nullptr)
    {
        const qint64 lastCallback = m_lastCallbackTimeMs.load(std::memory_order_relaxed);
        if (lastCallback > 0 && monotonicMs() - lastCallback > STREAM_STALL_TIMEOUT_MS)
        {
            CONSOLE_PRINT_MODULE("Audio callback stalled, reopening the stream", GameConsole::eDEBUG, GameConsole::eAudio);
            m_lastCallbackTimeMs.store(monotonicMs(), std::memory_order_relaxed);
            openStream(m_currentDeviceName);
        }
    }
#endif
}

void AudioManager::cacheCurrentMusicPosition()
{
#ifdef AUDIOSUPPORT
    std::lock_guard<std::mutex> lock(m_musicMutex);
    if (!m_musicState.currentFile.isEmpty())
    {
        const qint32 currentPosMs = (m_sampleRate > 0) ? static_cast<qint32>((m_musicState.currentFrame * 1000) / m_sampleRate) : 0;
        m_musicPlayPositionCache[m_musicState.currentFile] = currentPosMs;
        m_musicState.currentFile = "";
    }
#endif
}

void AudioManager::clearMusicPositions()
{
    emit sigClearMusicPositions();
}

void AudioManager::SlotClearMusicPositions()
{
#ifdef AUDIOSUPPORT
    m_musicPlayPositionCache.clear();
#endif
}

void AudioManager::playMusic(qint32 File)
{
    emit sigPlayMusic(File);
}

void AudioManager::continueMusic(QString file, qint32 position)
{
    emit sigContinueMusic(file, position);
}

void AudioManager::setVolume(qint32 value)
{
    emit sigSetVolume(value);
}

qint32 AudioManager::getVolume()
{
#ifdef AUDIOSUPPORT
    return static_cast<qint32>(m_musicVolume.load(std::memory_order_relaxed) * 100.0f);
#else
    return 0;
#endif
}

void AudioManager::addMusic(QString File, qint64 startPointMs, qint64 endPointMs)
{
    emit sigAddMusic(File, startPointMs, endPointMs);
}

void AudioManager::clearPlayList()
{
    if (Mainapp::getInstance()->isAudioThread())
    {
        SlotClearPlayList();
    }
    else
    {
        emit sigClearPlayList();
    }
}

void AudioManager::playRandom()
{
    emit sigPlayRandom();
}

void AudioManager::playSound(QString file, qint32 loops, qint32 delay, float volume, bool stopOldestSound, qint32 duration)
{
    emit sigPlaySound(file, loops, delay, volume, stopOldestSound, duration);
}

void AudioManager::stopSound(QString file)
{
    emit sigStopSound(file);
}

void AudioManager::stopAllSounds()
{
    emit sigStopAllSounds();
}

void AudioManager::loadFolder(QString folder)
{
    if (Mainapp::getInstance()->isAudioThread())
    {
        SlotLoadFolder(folder);
    }
    else
    {
        emit sigLoadFolder(folder);
    }
}

void AudioManager::SlotClearPlayList()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        CONSOLE_PRINT_MODULE("AudioThread::SlotClearPlayList() start clearing", GameConsole::eDEBUG, GameConsole::eAudio);
        cacheCurrentMusicPosition();
        std::vector<float> oldSamples;
        {
            std::lock_guard<std::mutex> lock(m_musicMutex);
            m_musicState.samples.swap(oldSamples);
            m_musicState.currentFrame = 0;
            m_musicState.totalFrames = 0;
            m_musicState.loopStartFrame = 0;
            m_musicState.loopEndFrame = 0;
            m_musicState.isPlaying.store(false, std::memory_order_relaxed);
            m_musicState.currentMediaIndex.store(-1, std::memory_order_relaxed);
            m_musicState.nextMediaIndex.store(-1, std::memory_order_relaxed);
        }
        m_PlayListdata.clear();
        CONSOLE_PRINT_MODULE("AudioThread::SlotClearPlayList() playlist cleared", GameConsole::eDEBUG, GameConsole::eAudio);
    }
#endif
}

void AudioManager::SlotPlayMusic(qint32 file)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio && !Settings::getInstance()->getMuted())
    {
        if (file >= 0 && file < m_PlayListdata.size())
        {
            CONSOLE_PRINT_MODULE("Starting music for player: " + m_PlayListdata[file].m_file, GameConsole::eDEBUG, GameConsole::eAudio);
            cacheCurrentMusicPosition();
            m_musicState.isPlaying.store(false, std::memory_order_relaxed);
            m_musicState.currentMediaIndex.store(file, std::memory_order_relaxed);
            m_musicState.nextMediaIndex.store(-1, std::memory_order_relaxed);
            loadMediaForFile(m_PlayListdata[file].m_file);
        }
        else
        {
            CONSOLE_PRINT("AudioThread::SlotPlayMusic Trying to play unknown music index", GameConsole::eERROR);
        }
    }
#endif
}

void AudioManager::SlotContinueMusic(QString file, qint32 position)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio && !Settings::getInstance()->getMuted())
    {
        if (file.isEmpty() && m_PlayListdata.size() == 1)
        {
            file = m_PlayListdata[0].m_file;
        }
        if (!file.isEmpty())
        {
            for (qint32 i = 0; i < m_PlayListdata.size(); ++i)
            {
                if (m_PlayListdata[i].m_file.endsWith(file))
                {
                    CONSOLE_PRINT_MODULE("Continue music for player: " + file, GameConsole::eDEBUG, GameConsole::eAudio);
                    cacheCurrentMusicPosition();
                    m_musicState.isPlaying.store(false, std::memory_order_relaxed);
                    m_musicState.currentMediaIndex.store(i, std::memory_order_relaxed);
                    m_musicState.nextMediaIndex.store(-1, std::memory_order_relaxed);
                    if (position < 0)
                    {
                        position = m_musicPlayPositionCache[m_PlayListdata[i].m_file];
                    }
                    loadMediaForFile(m_PlayListdata[i].m_file, position);
                    break;
                }
            }
        }
        else
        {
           SlotPlayRandom();
        }
    }
#endif
}

void AudioManager::loadMediaForFile(QString filePath, qint32 position)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray data = file.readAll();
            DecodedAudio decoded = AudioDecoder::decode(data, filePath, m_sampleRate);
            if (decoded.isValid())
            {
                const qint32 curIdx = m_musicState.currentMediaIndex.load(std::memory_order_relaxed);
                qint64 loopStartFrame = 0;
                qint64 loopEndFrame = decoded.totalFrames;
                if (curIdx >= 0 && curIdx < m_PlayListdata.size())
                {
                    const qint64 startMs = m_PlayListdata[curIdx].m_startpointMs;
                    const qint64 endMs = m_PlayListdata[curIdx].m_endpointMs;
                    loopStartFrame = (startMs > 0) ? (startMs * m_sampleRate) / 1000 : 0;
                    loopEndFrame = (endMs > 0) ? (endMs * m_sampleRate) / 1000 : decoded.totalFrames;
                }
                // the old buffer is released after unlocking to keep the callback lock as short as possible
                std::vector<float> oldSamples;
                {
                    std::lock_guard<std::mutex> lock(m_musicMutex);
                    m_musicState.samples.swap(oldSamples);
                    m_musicState.samples = std::move(decoded.samples);
                    m_musicState.totalFrames = decoded.totalFrames;
                    m_musicState.currentFile = filePath;
                    m_musicState.loopStartFrame = loopStartFrame;
                    m_musicState.loopEndFrame = loopEndFrame;
                    m_musicState.currentFrame = (position > 0) ? (static_cast<qint64>(position) * m_sampleRate) / 1000 : 0;
                    m_musicState.isPlaying.store(true, std::memory_order_relaxed);
                }
            }
            else
            {
                CONSOLE_PRINT("Failed to decode music file: " + filePath, GameConsole::eERROR);
            }
        }
        else
        {
            CONSOLE_PRINT("Failed to open file " + filePath, GameConsole::eERROR);
        }
    }
#endif
}

void AudioManager::SlotPlayRandom()
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio && !Settings::getInstance()->getMuted())
    {
        CONSOLE_PRINT_MODULE("AudioThread::SlotPlayRandom", GameConsole::eDEBUG, GameConsole::eAudio);
        const qint32 size = m_PlayListdata.size();
        if (size > 0)
        {
            const qint32 nextIndex = m_musicState.nextMediaIndex.load(std::memory_order_relaxed);
            if (nextIndex < 0 || nextIndex >= size)
            {
                m_musicState.isPlaying.store(false, std::memory_order_relaxed);
                m_musicState.currentMediaIndex.store(GlobalUtils::randIntBase(0, size - 1), std::memory_order_relaxed);
                const QString & musicFile = m_PlayListdata[m_musicState.currentMediaIndex.load(std::memory_order_relaxed)].m_file;
                loadMediaForFile(musicFile);
                CONSOLE_PRINT_MODULE("Buffering music for player: " + musicFile, GameConsole::eDEBUG, GameConsole::eAudio);
            }
            else if (m_musicState.currentMediaIndex.load(std::memory_order_relaxed) == nextIndex)
            {
                const qint32 currentIndex = m_musicState.currentMediaIndex.load(std::memory_order_relaxed);
                qint32 loopPos = m_PlayListdata[currentIndex].m_startpointMs;
                if (loopPos < 0)
                {
                    loopPos = 0;
                }
                bool needsReload = true;
                if (m_musicState.isPlaying.load(std::memory_order_relaxed))
                {
                    std::lock_guard<std::mutex> lock(m_musicMutex);
                    if (!m_musicState.samples.empty())
                    {
                        m_musicState.currentFrame = (static_cast<qint64>(loopPos) * m_sampleRate) / 1000;
                        needsReload = false;
                    }
                }
                if (needsReload)
                {
                    loadMediaForFile(m_PlayListdata[currentIndex].m_file, loopPos);
                }
                CONSOLE_PRINT_MODULE("Seeking music for player: " + m_PlayListdata[currentIndex].m_file + " to " + QString::number(loopPos), GameConsole::eDEBUG, GameConsole::eAudio);
            }
            else
            {
                m_musicState.isPlaying.store(false, std::memory_order_relaxed);
                m_musicState.currentMediaIndex.store(nextIndex, std::memory_order_relaxed);
                const QString & musicFile = m_PlayListdata[nextIndex].m_file;
                loadMediaForFile(musicFile);
                CONSOLE_PRINT_MODULE("Buffering music for player: " + musicFile, GameConsole::eDEBUG, GameConsole::eAudio);
            }
            m_musicState.nextMediaIndex.store(GlobalUtils::randIntBase(0, size - 1), std::memory_order_relaxed);
        }
        else
        {
            CONSOLE_PRINT_MODULE("Playlist is empty", GameConsole::eDEBUG, GameConsole::eAudio);
        }
    }
#endif
}

void AudioManager::SlotSetVolume(qint32 value)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        m_totalVolume.store(qPow(static_cast<float>(Settings::getInstance()->getTotalVolume()) / 100.0f, 2), std::memory_order_relaxed);
        m_musicVolume.store(qPow(static_cast<float>(value) / 100.0f, 2), std::memory_order_relaxed);
        m_soundVolume.store(qPow(static_cast<float>(Settings::getInstance()->getSoundVolume()) / 100.0f, 2), std::memory_order_relaxed);
        m_isMuted.store(Settings::getInstance()->getMuted(), std::memory_order_relaxed);

        CONSOLE_PRINT_MODULE("Setting volume to : music=" + QString::number(m_musicVolume.load(std::memory_order_relaxed)) +
                             " total=" + QString::number(m_totalVolume.load(std::memory_order_relaxed)), GameConsole::eDEBUG, GameConsole::eAudio);
    }
#endif
}

void AudioManager::slotSetMuteInternal(bool value)
{
    if (Settings::getInstance()->getMuteOnFcousedLost())
    {
        m_internalMuted.store(value, std::memory_order_relaxed);
    }
}

bool AudioManager::tryAddMusic(QString file, qint64 startPointMs, qint64 endPointMs)
{
    bool success = false;
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        QString currentPath = VirtualPaths::find(file);
        if (QFile::exists(currentPath))
        {
            addMusicToPlaylist(currentPath, startPointMs, endPointMs);
            success = true;
        }
    }
#endif
    return success;
}

void AudioManager::SlotAddMusic(QString file, qint64 startPointMs, qint64 endPointMs)
{
#ifdef AUDIOSUPPORT
    const QStringList supportedFormats = {".mp3", ".wav", ".ogg"};
    bool success = tryAddMusic(file, startPointMs, endPointMs);
    for (qint32 i = 0; i < supportedFormats.length() && !success; ++i)
    {
        QString filePath = file.first(file.lastIndexOf('.')) + supportedFormats[i];
        success = tryAddMusic(filePath, startPointMs, endPointMs);
    }
    if (!success)
    {
        CONSOLE_PRINT("Unable to locate music file: " + file, GameConsole::eERROR);
    }
#endif
}

void AudioManager::loadNextAudioFile()
{
    SlotPlayRandom();
}

void AudioManager::SlotLoadFolder(QString folder)
{
#ifdef AUDIOSUPPORT
    QStringList loadedSounds;
    QStringList searchPath = VirtualPaths::createSearchPathRev(folder);
    for (const QString & currentFolder : std::as_const(searchPath))
    {
        loadMusicFolder(currentFolder, loadedSounds);
    }
#endif
}

void AudioManager::loadMusicFolder(const QString & folder, QStringList& loadedSounds)
{
#ifdef AUDIOSUPPORT
    if (!m_noAudio)
    {
        QDir directory(folder);
        if (directory.exists())
        {
            QStringList filter;
            filter << "*.mp3" << "*.wav" << "*.ogg";
            QStringList files = directory.entryList(filter);
            for (const auto& file : std::as_const(files))
            {
                if (!loadedSounds.contains(file) && QFile::exists(folder + '/' + file))
                {
                    addMusicToPlaylist(folder + '/' + file);
                    loadedSounds.append(file);
                }
            }
        }
        else
        {
            CONSOLE_PRINT_MODULE("Unable to locate music folder: " + folder, GameConsole::eDEBUG, GameConsole::eAudio);
        }
    }
#endif
}

void AudioManager::addMusicToPlaylist(const QString & file, qint64 startPointMs, qint64 endPointMs)
{
    CONSOLE_PRINT_MODULE("Adding " + file + " to play list", GameConsole::eDEBUG, GameConsole::eAudio);
#ifdef AUDIOSUPPORT
    m_PlayListdata.append(PlaylistData(file, static_cast<qint32>(startPointMs), static_cast<qint32>(endPointMs)));
#endif
}

void AudioManager::clearTempFolder()
{
}

bool AudioManager::getLoadBaseGameFolders() const
{
    return m_loadBaseGameFolders;
}

void AudioManager::setLoadBaseGameFolders(bool loadBaseGameFolders)
{
    m_loadBaseGameFolders = loadBaseGameFolders;
}

void AudioManager::SlotCheckMusicEnded(qint64 duration)
{
    Q_UNUSED(duration);
}

void AudioManager::SlotPlaySound(QString file, qint32 loops, qint32 delay, float volume, bool stopOldestSound, qint32 duration)
{
#ifdef AUDIOSUPPORT
    if (Settings::getInstance()->getMuted() || m_noAudio || m_internalMuted.load(std::memory_order_relaxed))
    {
        return;
    }
    const auto it = m_soundCaches.constFind(file);
    if (it == m_soundCaches.constEnd())
    {
        CONSOLE_PRINT("Unable to locate sound: " + file, GameConsole::eDEBUG);
        return;
    }
    const spSoundData & soundCache = it.value();
    if (soundCache->m_samples.empty())
    {
        return;
    }

    if (soundCache->m_currentUseCount.load(std::memory_order_relaxed) >= soundCache->m_maxUseCount)
    {
        if (!stopOldestSound)
        {
            return;
        }
        qint32 oldestIdx = -1;
        qint64 oldestAge = std::numeric_limits<qint64>::max();
        for (qint32 i = 0; i < MAX_PARALLEL_SOUNDS; ++i)
        {
            auto & voice = m_soundVoices[i];
            std::lock_guard<std::mutex> lock(voice.mutex);
            if (voice.active.load(std::memory_order_relaxed) && voice.soundData == soundCache && voice.age < oldestAge)
            {
                oldestAge = voice.age;
                oldestIdx = i;
            }
        }
        if (oldestIdx < 0)
        {
            return;
        }
        auto & oldestVoice = m_soundVoices[oldestIdx];
        std::lock_guard<std::mutex> lock(oldestVoice.mutex);
        if (oldestVoice.active.exchange(false, std::memory_order_acq_rel))
        {
            soundCache->m_currentUseCount.fetch_sub(1, std::memory_order_relaxed);
        }
    }

    for (qint32 i = 0; i < MAX_PARALLEL_SOUNDS; ++i)
    {
        auto & voice = m_soundVoices[i];
        if (voice.active.load(std::memory_order_acquire))
        {
            continue;
        }
        std::lock_guard<std::mutex> lock(voice.mutex);
        if (voice.active.load(std::memory_order_relaxed))
        {
            continue;
        }
        voice.soundData = soundCache;
        voice.currentFrame = 0;
        voice.remainingLoops = loops;
        voice.delayFrames = (delay > 0) ? (static_cast<qint64>(delay) * m_sampleRate) / 1000 : 0;
        voice.remainingDurationFrames = (duration > 0) ? (static_cast<qint64>(duration) * m_sampleRate) / 1000 : -1;
        voice.volume = volume;
        voice.age = ++m_voiceCounter;
        soundCache->m_currentUseCount.fetch_add(1, std::memory_order_relaxed);
        voice.active.store(true, std::memory_order_release);
        break;
    }
#endif
}

void AudioManager::SlotStopAllSounds()
{
#ifdef AUDIOSUPPORT
    CONSOLE_PRINT_MODULE("Stopping all sounds", GameConsole::eDEBUG, GameConsole::eAudio);
    for (qint32 i = 0; i < MAX_PARALLEL_SOUNDS; ++i)
    {
        auto & voice = m_soundVoices[i];
        std::lock_guard<std::mutex> lock(voice.mutex);
        voice.active.store(false, std::memory_order_release);
    }
    for (auto & soundCache : m_soundCaches)
    {
        soundCache->m_currentUseCount.store(0, std::memory_order_relaxed);
    }
#endif
}

void AudioManager::SlotStopSound(QString file)
{
#ifdef AUDIOSUPPORT
    const auto it = m_soundCaches.constFind(file);
    if (it != m_soundCaches.constEnd())
    {
        CONSOLE_PRINT_MODULE("Stopping sound " + file, GameConsole::eDEBUG, GameConsole::eAudio);
        const spSoundData & soundCache = it.value();
        for (qint32 i = 0; i < MAX_PARALLEL_SOUNDS; ++i)
        {
            auto & voice = m_soundVoices[i];
            std::lock_guard<std::mutex> lock(voice.mutex);
            if (voice.soundData == soundCache)
            {
                voice.active.store(false, std::memory_order_release);
            }
        }
        soundCache->m_currentUseCount.store(0, std::memory_order_relaxed);
    }
#endif
}
