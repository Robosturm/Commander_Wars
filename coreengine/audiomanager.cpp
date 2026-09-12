#include "coreengine/audiomanager.h"
#include "coreengine/audiodecoder.h"
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
#include <cmath>
#include <limits>

SoundData::SoundData()
{
#ifdef GRAPHICSUPPORT
    setObjectName("SoundData");
#endif
    Interpreter::setCppOwnerShip(this);
}

AudioManager::AudioManager(bool noAudio, bool useAudioThread)
    : m_noAudio(noAudio)
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
        m_pollTimer.stop();
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
            CONSOLE_PRINT_MODULE("AudioThread::initAudio", GameConsole::eDEBUG, GameConsole::eAudio);
            PaError err = Pa_Initialize();
            if (err != paNoError)
            {
                CONSOLE_PRINT_MODULE("Pa_Initialize failed: " + QString::fromUtf8(Pa_GetErrorText(err)), GameConsole::eERROR, GameConsole::eAudio);
                return;
            }

            const auto value = Settings::getInstance()->getAudioOutput();
            QString deviceName = value.toString();
            if (deviceName.isEmpty())
            {
                deviceName = Settings::DEFAULT_AUDIODEVICE;
            }

            openStream(deviceName);

            m_pollTimer.setInterval(50);
            connect(&m_pollTimer, &QTimer::timeout, this, [this]()
            {
                if (m_trackEndedFlag.exchange(false))
                {
                    SlotPlayRandom();
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
    outParams.suggestedLatency = devInfo ? devInfo->defaultLowOutputLatency : 0.050;
    outParams.hostApiSpecificStreamInfo = nullptr;

    PaError err = Pa_OpenStream(
        &m_paStream,
        nullptr,
        &outParams,
        m_sampleRate,
        paFramesPerBufferUnspecified,
        paNoFlag,
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

    return true;
#else
    Q_UNUSED(deviceName);
    return false;
#endif
}

#ifdef AUDIOSUPPORT
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
    if (!out)
    {
        return paContinue;
    }

    std::fill(out, out + framesPerBuffer * 2, 0.0f);

    std::unique_lock<std::mutex> lock(self->m_audioMutex, std::try_to_lock);
    if (!lock.owns_lock())
    {
        return paContinue;
    }

    if (self->m_isMuted || self->m_internalMuted)
    {
        return paContinue;
    }

    float totalVol = self->m_totalVolume;
    if (totalVol <= 0.0f)
    {
        return paContinue;
    }

    // Mix music
    if (self->m_musicState.isPlaying && !self->m_musicState.samples.empty())
    {
        float musicVol = self->m_musicState.volume * self->m_musicVolume * totalVol;
        auto& ms = self->m_musicState;

        for (unsigned long i = 0; i < framesPerBuffer; ++i)
        {
            if (ms.loopEndFrame > 0 && ms.currentFrame >= ms.loopEndFrame)
            {
                if (ms.currentMediaIndex == ms.nextMediaIndex || ms.nextMediaIndex < 0)
                {
                    ms.currentFrame = ms.loopStartFrame;
                }
                else
                {
                    ms.isPlaying = false;
                    self->m_trackEndedFlag.store(true, std::memory_order_relaxed);
                    break;
                }
            }
            else if (ms.currentFrame >= ms.totalFrames)
            {
                if (ms.currentMediaIndex == ms.nextMediaIndex)
                {
                    ms.currentFrame = ms.loopStartFrame;
                }
                else
                {
                    ms.isPlaying = false;
                    self->m_trackEndedFlag.store(true, std::memory_order_relaxed);
                    break;
                }
            }

            if (ms.currentFrame < ms.totalFrames)
            {
                out[i * 2 + 0] += ms.samples[ms.currentFrame * 2 + 0] * musicVol;
                out[i * 2 + 1] += ms.samples[ms.currentFrame * 2 + 1] * musicVol;
                ms.currentFrame++;
            }
        }
    }

    // Mix sound effects
    float sfxBaseVol = self->m_soundVolume * totalVol;
    for (qint32 v = 0; v < MAX_PARALLEL_SOUNDS; ++v)
    {
        auto& voice = self->m_soundVoices[v];
        if (!voice.active || !voice.soundData || voice.soundData->m_samples.empty())
        {
            continue;
        }

        float voiceVol = voice.volume * sfxBaseVol;
        const auto& samples = voice.soundData->m_samples;
        qint64 totalFrames = voice.soundData->m_totalFrames;

        for (unsigned long i = 0; i < framesPerBuffer; ++i)
        {
            if (voice.delayFrames > 0)
            {
                voice.delayFrames--;
                continue;
            }

            if (voice.remainingDurationFrames > 0)
            {
                voice.remainingDurationFrames--;
                if (voice.remainingDurationFrames == 0)
                {
                    voice.active = false;
                    if (voice.soundData->m_currentUseCount > 0)
                    {
                        voice.soundData->m_currentUseCount--;
                    }
                    break;
                }
            }

            if (voice.currentFrame >= totalFrames)
            {
                if (voice.remainingLoops > 1)
                {
                    voice.remainingLoops--;
                    voice.currentFrame = 0;
                }
                else if (voice.remainingLoops == -1)
                {
                    voice.currentFrame = 0;
                }
                else
                {
                    voice.active = false;
                    if (voice.soundData->m_currentUseCount > 0)
                    {
                        voice.soundData->m_currentUseCount--;
                    }
                    break;
                }
            }

            out[i * 2 + 0] += samples[voice.currentFrame * 2 + 0] * voiceVol;
            out[i * 2 + 1] += samples[voice.currentFrame * 2 + 1] * voiceVol;
            voice.currentFrame++;
        }
    }

    // Clamp output buffer to [-1.0f, 1.0f]
    for (unsigned long i = 0; i < framesPerBuffer * 2; ++i)
    {
        out[i] = std::clamp(out[i], -1.0f, 1.0f);
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
        openStream(deviceName);
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
    return static_cast<qint32>(m_musicVolume * 100.0f);
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
        std::lock_guard<std::mutex> lock(m_audioMutex);
        if (!m_musicState.currentFile.isEmpty())
        {
            qint32 currentPosMs = (m_sampleRate > 0) ? static_cast<qint32>((m_musicState.currentFrame * 1000) / m_sampleRate) : 0;
            m_musicPlayPositionCache[m_musicState.currentFile] = currentPosMs;
            m_musicState.currentFile = "";
        }
        m_PlayListdata.clear();
        m_musicState.samples.clear();
        m_musicState.currentFrame = 0;
        m_musicState.totalFrames = 0;
        m_musicState.isPlaying = false;
        m_musicState.currentMediaIndex = -1;
        m_musicState.nextMediaIndex = -1;
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
            if (!m_musicState.currentFile.isEmpty())
            {
                qint32 currentPosMs = (m_sampleRate > 0) ? static_cast<qint32>((m_musicState.currentFrame * 1000) / m_sampleRate) : 0;
                m_musicPlayPositionCache[m_musicState.currentFile] = currentPosMs;
                m_musicState.currentFile = "";
            }
            m_musicState.isPlaying = false;
            m_musicState.currentMediaIndex = file;
            m_musicState.nextMediaIndex = -1;
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
                    if (!m_musicState.currentFile.isEmpty())
                    {
                        qint32 currentPosMs = (m_sampleRate > 0) ? static_cast<qint32>((m_musicState.currentFrame * 1000) / m_sampleRate) : 0;
                        m_musicPlayPositionCache[m_musicState.currentFile] = currentPosMs;
                        m_musicState.currentFile = "";
                    }
                    m_musicState.isPlaying = false;
                    m_musicState.currentMediaIndex = i;
                    m_musicState.nextMediaIndex = -1;
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
                std::lock_guard<std::mutex> lock(m_audioMutex);
                m_musicState.samples = std::move(decoded.samples);
                m_musicState.totalFrames = decoded.totalFrames;
                m_musicState.currentFile = filePath;

                qint32 curIdx = m_musicState.currentMediaIndex;
                if (curIdx >= 0 && curIdx < m_PlayListdata.size())
                {
                    qint64 startMs = m_PlayListdata[curIdx].m_startpointMs;
                    qint64 endMs = m_PlayListdata[curIdx].m_endpointMs;
                    m_musicState.loopStartFrame = (startMs > 0) ? (startMs * m_sampleRate) / 1000 : 0;
                    m_musicState.loopEndFrame = (endMs > 0) ? (endMs * m_sampleRate) / 1000 : decoded.totalFrames;
                }
                else
                {
                    m_musicState.loopStartFrame = 0;
                    m_musicState.loopEndFrame = decoded.totalFrames;
                }

                m_musicState.currentFrame = (position > 0) ? (static_cast<qint64>(position) * m_sampleRate) / 1000 : 0;
                m_musicState.isPlaying = true;
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
        qint32 size = m_PlayListdata.size();
        if (size > 0)
        {
            if (m_musicState.nextMediaIndex < 0 || m_musicState.nextMediaIndex >= size)
            {
                m_musicState.isPlaying = false;
                m_musicState.currentMediaIndex = GlobalUtils::randIntBase(0, size - 1);
                loadMediaForFile(m_PlayListdata[m_musicState.currentMediaIndex].m_file);
                CONSOLE_PRINT_MODULE("Buffering music for player: " + m_PlayListdata[m_musicState.currentMediaIndex].m_file, GameConsole::eDEBUG, GameConsole::eAudio);
            }
            else if (m_musicState.currentMediaIndex == m_musicState.nextMediaIndex)
            {
                qint32 loopPos = m_PlayListdata[m_musicState.currentMediaIndex].m_startpointMs;
                if (loopPos < 0)
                {
                    loopPos = 0;
                }
                if (!m_musicState.isPlaying || m_musicState.samples.empty())
                {
                    loadMediaForFile(m_PlayListdata[m_musicState.currentMediaIndex].m_file, loopPos);
                }
                else
                {
                    std::lock_guard<std::mutex> lock(m_audioMutex);
                    m_musicState.currentFrame = (static_cast<qint64>(loopPos) * m_sampleRate) / 1000;
                    m_musicState.isPlaying = true;
                }
                CONSOLE_PRINT_MODULE("Seeking music for player: " + m_PlayListdata[m_musicState.currentMediaIndex].m_file + " to " + QString::number(loopPos), GameConsole::eDEBUG, GameConsole::eAudio);
            }
            else
            {
                m_musicState.isPlaying = false;
                m_musicState.currentMediaIndex = m_musicState.nextMediaIndex;
                loadMediaForFile(m_PlayListdata[m_musicState.currentMediaIndex].m_file);
                CONSOLE_PRINT_MODULE("Buffering music for player: " + m_PlayListdata[m_musicState.currentMediaIndex].m_file, GameConsole::eDEBUG, GameConsole::eAudio);
            }
            m_musicState.nextMediaIndex = GlobalUtils::randIntBase(0, size - 1);
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
        std::lock_guard<std::mutex> lock(m_audioMutex);
        m_musicVolume = static_cast<float>(value) / 100.0f;
        m_totalVolume = static_cast<float>(Settings::getInstance()->getTotalVolume()) / 100.0f;
        m_soundVolume = static_cast<float>(Settings::getInstance()->getSoundVolume()) / 100.0f;
        m_isMuted = Settings::getInstance()->getMuted();

        CONSOLE_PRINT_MODULE("Setting volume to : music=" + QString::number(m_musicVolume) + " total=" + QString::number(m_totalVolume), GameConsole::eDEBUG, GameConsole::eAudio);
    }
#endif
}

void AudioManager::slotSetMuteInternal(bool value)
{
    if (Settings::getInstance()->getMuteOnFcousedLost())
    {
#ifdef AUDIOSUPPORT
        std::lock_guard<std::mutex> lock(m_audioMutex);
        m_internalMuted = value;
#endif
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
    if (Settings::getInstance()->getMuted() || m_noAudio || m_internalMuted)
    {
        return;
    }
    if (m_soundCaches.contains(file))
    {
        auto & soundCache = m_soundCaches[file];
        if (soundCache->m_samples.empty())
        {
            return;
        }

        std::lock_guard<std::mutex> lock(m_audioMutex);
        if (soundCache->m_currentUseCount >= soundCache->m_maxUseCount)
        {
            if (stopOldestSound)
            {
                qint32 oldestIdx = -1;
                qint64 oldestAge = std::numeric_limits<qint64>::max();
                for (qint32 i = 0; i < MAX_PARALLEL_SOUNDS; ++i)
                {
                    if (m_soundVoices[i].active && m_soundVoices[i].soundData == soundCache)
                    {
                        if (m_soundVoices[i].age < oldestAge)
                        {
                            oldestAge = m_soundVoices[i].age;
                            oldestIdx = i;
                        }
                    }
                }
                if (oldestIdx >= 0)
                {
                    m_soundVoices[oldestIdx].active = false;
                    soundCache->m_currentUseCount--;
                }
            }
            else
            {
                return;
            }
        }

        qint32 slot = -1;
        for (qint32 i = 0; i < MAX_PARALLEL_SOUNDS; ++i)
        {
            if (!m_soundVoices[i].active)
            {
                slot = i;
                break;
            }
        }

        if (slot >= 0)
        {
            auto& voice = m_soundVoices[slot];
            voice.soundData = soundCache;
            voice.currentFrame = 0;
            voice.remainingLoops = loops;
            voice.delayFrames = (delay > 0) ? static_cast<qint32>((static_cast<qint64>(delay) * m_sampleRate) / 1000) : 0;
            voice.remainingDurationFrames = (duration > 0) ? static_cast<qint32>((static_cast<qint64>(duration) * m_sampleRate) / 1000) : -1;
            voice.volume = volume;
            voice.age = ++m_voiceCounter;
            voice.active = true;
            soundCache->m_currentUseCount++;
        }
    }
    else
    {
        CONSOLE_PRINT("Unable to locate sound: " + file, GameConsole::eDEBUG);
    }
#endif
}

void AudioManager::SlotStopAllSounds()
{
#ifdef AUDIOSUPPORT
    CONSOLE_PRINT_MODULE("Stopping all sounds", GameConsole::eDEBUG, GameConsole::eAudio);
    std::lock_guard<std::mutex> lock(m_audioMutex);
    for (qint32 i = 0; i < MAX_PARALLEL_SOUNDS; ++i)
    {
        m_soundVoices[i].active = false;
    }
    for (auto & soundCache : m_soundCaches)
    {
        soundCache->m_currentUseCount = 0;
    }
#endif
}

void AudioManager::SlotStopSound(QString file)
{
#ifdef AUDIOSUPPORT
    if (m_soundCaches.contains(file))
    {
        CONSOLE_PRINT_MODULE("Stopping sound " + file, GameConsole::eDEBUG, GameConsole::eAudio);
        std::lock_guard<std::mutex> lock(m_audioMutex);
        auto & soundCache = m_soundCaches[file];
        for (qint32 i = 0; i < MAX_PARALLEL_SOUNDS; ++i)
        {
            if (m_soundVoices[i].active && m_soundVoices[i].soundData == soundCache)
            {
                m_soundVoices[i].active = false;
            }
        }
        soundCache->m_currentUseCount = 0;
    }
#endif
}
