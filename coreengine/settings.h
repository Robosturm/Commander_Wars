#ifndef SETTINGS_H
#define SETTINGS_H

#include <type_traits>

#include <Qt>
#include <QObject>
#include <QTranslator>
#include <QSettings>
#ifdef AUDIOSUPPORT
#include <QMediaDevices>
#include <QAudioDevice>
#endif

#include "coreengine/gameconsole.h"

#include "game/GameEnums.h"

class GLGraphicsView;
class Settings;
using spSettings = std::shared_ptr<Settings>;
using spQTranslator = std::shared_ptr<QTranslator>;

class Settings final : public QObject
{
    Q_OBJECT
public:
    static const char* const DEFAULT_AUDIODEVICE;

private:
    struct ValueBase
    {
        ValueBase(const char* const group, const char* const name)
            : m_group(group),
            m_name(name)
        {
        }
        virtual void readValue(QSettings & settings) = 0;
        virtual void saveValue(QSettings & settings) = 0;
        virtual void resetValue() = 0;
        const char* const getGroup() const
        {
            return m_group;
        }
        const char* const getName() const
        {
            return m_name;
        }
    protected:
        const char* const m_group;
        const char* const m_name;
    };

    using spValueBase = std::shared_ptr<ValueBase>;

    template<typename TType>
    struct Value : public ValueBase
    {
        static constexpr const char* const getTypeName()
        {
            return "Value";
        }
        Value(const char* const group, const char* const name, TType* value, TType defaultValue, TType minValue, TType maxValue, bool excludeFromReset = false)
            : ValueBase(group, name),
            m_defaultValue{defaultValue},
            m_value{value},
            m_minValue{minValue},
            m_maxValue{maxValue},
            m_excludeFromReset{excludeFromReset}
        {
        }
        virtual void readValue(QSettings & settings) override
        {
            settings.beginGroup(m_group);
            if (!settings.contains(m_name))
            {
                CONSOLE_PRINT("Key " + QString(m_name) + " in group " + m_group + " not found using default value", GameConsole::eDEBUG);
            }
            if constexpr(std::is_floating_point<TType>::value)
            {
                bool ok = false;
                *m_value = settings.value(m_name, m_defaultValue).toFloat(&ok);
                if(!ok || *m_value <= m_minValue || *m_value > m_maxValue)
                {
                    QString error = "Error in the Ini File: [" + QString(m_group) + "] Setting: " + QString(m_name);
                    CONSOLE_PRINT(error, GameConsole::eERROR);
                    *m_value = m_defaultValue;
                }
            }
            else if constexpr (std::is_same<TType, bool>::value)
            {
                *m_value = settings.value(m_name, m_defaultValue).toBool();
            }
            else if constexpr (std::is_same<TType, QString>::value)
            {
                *m_value = settings.value(m_name, m_defaultValue).toString();
            }
            else if constexpr (std::is_same<TType, quint32>::value ||
                               std::is_same<TType, quint16>::value ||
                               std::is_same<TType, quint8>::value)
            {
                bool ok = false;
                *m_value = settings.value(m_name, m_defaultValue).toUInt(&ok);
                if(!ok || *m_value < m_minValue || *m_value > m_maxValue)
                {
                    QString error = "Error in the Ini File: [" + QString(m_group) + "] Setting: " + QString(m_name);
                    CONSOLE_PRINT(error, GameConsole::eERROR);
                    *m_value = m_defaultValue;
                }
            }
            else if constexpr (std::is_same<TType, quint64>::value)
            {
                bool ok = false;
                *m_value = settings.value(m_name, m_defaultValue).toULongLong(&ok);
                if(!ok || *m_value < m_minValue || *m_value > m_maxValue)
                {
                    QString error = "Error in the Ini File: [" + QString(m_group) + "] Setting: " + QString(m_name);
                    CONSOLE_PRINT(error, GameConsole::eERROR);
                    *m_value = m_defaultValue;
                }
            }
            else if constexpr (std::is_same<TType, qint32>::value ||
                               std::is_same<TType, qint16>::value ||
                               std::is_same<TType, qint8>::value)
            {
                bool ok = false;
                *m_value = settings.value(m_name, m_defaultValue).toInt(&ok);
                if(!ok || *m_value < m_minValue || *m_value > m_maxValue)
                {
                    QString error = "Error in the Ini File: [" + QString(m_group) + "] Setting: " + QString(m_name);
                    CONSOLE_PRINT(error, GameConsole::eERROR);
                    *m_value = m_defaultValue;
                }
            }
            else if constexpr (std::is_same<TType, qint64>::value)
            {
                bool ok = false;
                *m_value = settings.value(m_name, m_defaultValue).toLongLong(&ok);
                if(!ok || *m_value < m_minValue || *m_value > m_maxValue)
                {
                    QString error = "Error in the Ini File: [" + QString(m_group) + "] Setting: " + QString(m_name);
                    CONSOLE_PRINT(error, GameConsole::eERROR);
                    *m_value = m_defaultValue;
                }
            }
            else if constexpr (std::is_enum<TType>::value)
            {
                bool ok = false;
                *m_value = static_cast<TType>(settings.value(m_name, m_defaultValue).toInt(&ok));
                if(!ok || *m_value < m_minValue || *m_value > m_maxValue)
                {
                    QString error = "Error in the Ini File: [" + QString(m_group) + "] Setting: " + QString(m_name);
                    CONSOLE_PRINT(error, GameConsole::eERROR);
                    *m_value = m_defaultValue;
                }
            }
            else if constexpr (std::is_same<TType, QStringList>::value)
            {
                QString list = settings.value(m_name, m_defaultValue).toString();
                if (!list.isEmpty())
                {
                    *m_value = list.split(",");
                }
            }
            else if constexpr (std::is_same<TType, std::chrono::seconds>::value)
            {
                bool ok = false;
                *m_value = std::chrono::seconds(settings.value(m_name, static_cast<qint64>(m_defaultValue.count())).toUInt(&ok));
                if(!ok || *m_value < m_minValue || *m_value > m_maxValue)
                {
                    QString error = "Error in the Ini File: [" + QString(m_group) + "] Setting: " + QString(m_name);
                    CONSOLE_PRINT(error, GameConsole::eERROR);
                    *m_value = m_defaultValue;
                }
            }
            else if constexpr (std::is_same<TType, std::chrono::minutes>::value)
            {
                bool ok = false;
                *m_value = std::chrono::minutes(settings.value(m_name, static_cast<qint64>(m_defaultValue.count())).toUInt(&ok));
                if(!ok || *m_value < m_minValue || *m_value > m_maxValue)
                {
                    QString error = "Error in the Ini File: [" + QString(m_group) + "] Setting: " + QString(m_name);
                    CONSOLE_PRINT(error, GameConsole::eERROR);
                    *m_value = m_defaultValue;
                }
            }
            else
            {
                // not implemented data type
                Q_ASSERT(false);
            }

            settings.endGroup();
        }
        virtual void saveValue(QSettings & settings) override
        {
            settings.beginGroup(m_group);
            if constexpr (std::is_same<TType, std::chrono::seconds>::value)
            {
                settings.setValue(m_name, static_cast<qint64>((*m_value).count()));
            }
            else if constexpr (std::is_same<TType, QStringList>::value)
            {
                settings.setValue(m_name, Settings::getInstance()->getConfigString(*m_value));
            }
            else
            {
                settings.setValue(m_name, *m_value);
            }
            settings.endGroup();
        }
        virtual void resetValue()  override
        {
            if (!m_excludeFromReset)
            {
                *m_value = m_defaultValue;
            }
        }
    private:
        TType m_defaultValue;
        TType m_minValue;
        TType m_maxValue;
        TType* m_value;
        bool m_excludeFromReset;
    };
#ifdef AUDIOSUPPORT
    struct AudioDeviceValue : public ValueBase
    {
        static constexpr const char* const getTypeName()
        {
            return "AudioDeviceValue";
        }
        AudioDeviceValue(const char* const group, const char* const name, QVariant* value, QString defaultValue)
            : ValueBase(group, name),
            m_value{value},
            m_defaultValue{defaultValue}
        {
        }
        virtual void readValue(QSettings & settings) override
        {
            settings.beginGroup(m_group);
            if (!settings.contains(m_name))
            {
                CONSOLE_PRINT("Key " + QString(m_name) + " in group " + m_group + " not found using default value", GameConsole::eDEBUG);
            }
            const QAudioDevice &defaultDeviceInfo = QMediaDevices::defaultAudioOutput();
            QString description = settings.value(m_name, m_defaultValue).toString();
            if (description == Settings::getInstance()->DEFAULT_AUDIODEVICE)
            {
                *m_value = QVariant(Settings::getInstance()->DEFAULT_AUDIODEVICE);
            }
            else
            {
                const auto audioDevices = QMediaDevices::audioOutputs();
                for (const auto & device : audioDevices)
                {
                    if (device.description() == description)
                    {
                        *m_value = QVariant::fromValue(device);
                        break;
                    }
                }
                if (m_pInstance->m_audioOutput.value<QAudioDevice>().isNull())
                {
                    *m_value = QVariant::fromValue(defaultDeviceInfo);
                }
            }
            settings.endGroup();
        }
        virtual void saveValue(QSettings & settings) override
        {
            settings.beginGroup(m_group);
            if (m_value->typeId() == QMetaType::QString &&
                m_value->toString() == Settings::getInstance()->DEFAULT_AUDIODEVICE)
            {
                settings.setValue(m_name, Settings::getInstance()->DEFAULT_AUDIODEVICE);
            }
            else
            {
                auto device = (*m_value).value<QAudioDevice>();
                settings.setValue(m_name, device.description());
            }
            settings.endGroup();
        }
        virtual void resetValue() override
        {
            *m_value = QVariant(Settings::getInstance()->DEFAULT_AUDIODEVICE);
        }
    private:
        QVariant* m_value;
        QString m_defaultValue;
    };
#endif

public:
    enum class ScreenModes
    {
        Window,
        Borderless,
        FullScreen,
    };

    virtual ~Settings() = default;
    static Settings* getInstance();

    void setup();
    QString getActiveUserPath();

    void setLastSaveGame(const QString &LastSaveGame);

    void setUpdateStep(const QString &newUpdateStep);


    void setAiSlave(bool newAiSlave);

    void setPipeUuid(const QString & newPipeUuid);

    const QVariant getAudioOutput();
    void setAudioOutput(const QVariant newAudioOutput);
    void setAutoSavingCylceTime(const std::chrono::seconds value);
    void getModInfos(QString mod, QString & name, QString & description, QString & version,
                            QStringList & compatibleMods, QStringList & incompatibleMods,
                            QStringList & requiredMods, bool & isCosmetic,
                            QStringList & tags, QString & thumbnail);
    const std::chrono::seconds getSlaveDespawnTime();
    void setSlaveDespawnTime(const std::chrono::seconds newSlaveDespawnTime);
    const std::chrono::seconds getSuspendedDespawnTime();
    void setSuspendedDespawnTime(const std::chrono::seconds newSlaveDespawnTime);

    Q_INVOKABLE void loadSettings();
    Q_INVOKABLE void resetSettings();
    Q_INVOKABLE void saveSettings();
    Q_INVOKABLE float getGameScale();
    Q_INVOKABLE void setGameScale(float newGameScale);
    Q_INVOKABLE float getIngameMenuScaling();
    Q_INVOKABLE void setIngameMenuScaling(float newIngameMenuScaling);
    Q_INVOKABLE quint8 getScreen();
    Q_INVOKABLE void setScreen(quint8 newScreen);
    Q_INVOKABLE QString getLastSaveGame();
    Q_INVOKABLE QString getUpdateStep();
    Q_INVOKABLE bool getAiSlave();
    Q_INVOKABLE QString getPipeUuid();
    Q_INVOKABLE void setSpawnAiProcess(bool newSpawnAiProcess);
    Q_INVOKABLE bool getSpawnAiProcess();
    Q_INVOKABLE bool getAutomaticUpdates();
    Q_INVOKABLE void setAutomaticUpdates(bool newAutomaticUpdates);
    Q_INVOKABLE QString getServerAdress();
    Q_INVOKABLE void setServerAdress(const QString ServerAdress);
    Q_INVOKABLE QString getSecondaryServerAdress();
    Q_INVOKABLE void setSecondaryServerAdress(const QString newSecondaryServerAdress);
    Q_INVOKABLE quint16 getServerPort();
    Q_INVOKABLE void setServerPort(const quint16 ServerPort);
    Q_INVOKABLE QString getServerPassword();
    Q_INVOKABLE void setServerPassword(const QString newServerPassword);
    Q_INVOKABLE QString getMailServerSendAddress();
    Q_INVOKABLE void setMailServerSendAddress(const QString newMailServerSendAddress);
    Q_INVOKABLE qint32 getMailServerAuthMethod();
    Q_INVOKABLE void setMailServerAuthMethod(qint32 newMailServerAuthMethod);
    Q_INVOKABLE QString getMailServerAddress();
    Q_INVOKABLE void setMailServerAddress(const QString newMailServerAddress);
    Q_INVOKABLE quint16 getMailServerPort();
    Q_INVOKABLE void setMailServerPort(quint16 newMailServerPort);
    Q_INVOKABLE qint32 getMailServerConnectionType();
    Q_INVOKABLE void setMailServerConnectionType(qint32 newMailServerConnectionType);
    Q_INVOKABLE QString getMailServerUsername();
    Q_INVOKABLE void setMailServerUsername(QString newMailServerUsername);
    Q_INVOKABLE QString getMailServerPassword();
    Q_INVOKABLE void setMailServerPassword(QString newMailServerPassword);
    Q_INVOKABLE QString getDefaultBannlist();
    Q_INVOKABLE void setDefaultBannlist(const QString newDefaultBannlist);
    Q_INVOKABLE bool getDay2dayScreen();
    Q_INVOKABLE void setDay2dayScreen(bool newDay2dayScreen);
    Q_INVOKABLE bool getMovementAnimations();
    Q_INVOKABLE void setMovementAnimations(bool newMovementAnimations);
    Q_INVOKABLE bool getCaptureAnimation();
    Q_INVOKABLE void setCaptureAnimation(bool newCaptureAnimation);
    Q_INVOKABLE QString getSlaveListenAdress();
    Q_INVOKABLE void setSlaveListenAdress(const QString newSlaveListenAdress);
    Q_INVOKABLE qint32 getPauseAfterAction();
    Q_INVOKABLE void setPauseAfterAction(qint32 newPauseAfterAction);
    Q_INVOKABLE QString getServerListenAdress();
    Q_INVOKABLE void setServerListenAdress(const QString newServerListenAdress);
    Q_INVOKABLE QString getServerSecondaryListenAdress();
    Q_INVOKABLE void setServerSecondaryListenAdress(const QString newServerSecondaryListenAdress);
    Q_INVOKABLE quint16 getSlaveServerPort();
    Q_INVOKABLE void setSlaveServerPort(quint16 newSlaveServerPort);
    Q_INVOKABLE QString getSlaveHostOptions();
    Q_INVOKABLE void setSlaveHostOptions(const QString newSlaveHostOptions);
    Q_INVOKABLE Qt::Key getKey_mapshot();
    Q_INVOKABLE void setKey_mapshot(Qt::Key newKey_mapshot);
    Q_INVOKABLE qint32 getFramesPerSecond();
    Q_INVOKABLE void setFramesPerSecond(qint32 newFramesPerSecond);
    Q_INVOKABLE bool getMuted();
    Q_INVOKABLE void setMuted(bool newMuted);
    Q_INVOKABLE float getSupplyWarning();
    Q_INVOKABLE void setSupplyWarning(float newSupplyWarning);
    Q_INVOKABLE bool getGamepadEnabled();
    Q_INVOKABLE void setGamepadEnabled(bool newGamepadEnabled);
    Q_INVOKABLE float getGamepadSensitivity();
    Q_INVOKABLE void setGamepadSensitivity(float newGamepadSensitivity);
    Q_INVOKABLE QString getDefaultRuleset();
    Q_INVOKABLE void setDefaultRuleset(const QString newDefaultRuleset);
    Q_INVOKABLE bool getUseCoMinis();
    Q_INVOKABLE void setUseCoMinis(bool newUseCoMinis);
    Q_INVOKABLE bool getOverworldAnimations();
    Q_INVOKABLE void setOverworldAnimations(bool newOverworldAnimations);
    Q_INVOKABLE qint32 getTouchPointSensitivity();
    Q_INVOKABLE void setTouchPointSensitivity(qint32 newTouchPointSensitivity);
    Q_INVOKABLE bool getAutoMoveCursor();
    Q_INVOKABLE void setAutoMoveCursor(bool newAutoMoveCursor);
    Q_INVOKABLE bool getShowDetailedBattleForcast();
    Q_INVOKABLE void setShowDetailedBattleForcast(bool newShowDetailedBattleForcast);
    Q_INVOKABLE QString getUserPath();
    Q_INVOKABLE void setUserPath(const QString newUserPath);
    Q_INVOKABLE bool getTouchScreen();
    Q_INVOKABLE void setTouchScreen(bool newTouchScreen);
    Q_INVOKABLE bool hasSmallScreen();
    Q_INVOKABLE bool getSmallScreenDevice();
    Q_INVOKABLE void setSmallScreenDevice(bool newSmallScreenDevice);
    Q_INVOKABLE qint32 getMenuItemRowCount();
    Q_INVOKABLE void setMenuItemRowCount(qint32 newMenuItemRowCount);
    Q_INVOKABLE bool getSimpleDeselect();
    Q_INVOKABLE void setSimpleDeselect(bool newSimpleDeselect);
    Q_INVOKABLE QStringList getActiveModVersions();
    Q_INVOKABLE QStringList getActiveMods();
    Q_INVOKABLE void setActiveMods(const QStringList activeMods);
    Q_INVOKABLE QString getSlaveServerName();
    Q_INVOKABLE void setSlaveServerName(const QString slaveServerName);
    Q_INVOKABLE bool getSyncAnimations();
    Q_INVOKABLE void setSyncAnimations(bool syncAnimations);
    Q_INVOKABLE bool getCenterOnMarkedField();
    Q_INVOKABLE void setCenterOnMarkedField(bool centerOnMarkedField);
    Q_INVOKABLE bool getDialogAnimation();
    Q_INVOKABLE void setDialogAnimation(bool dialogAnimation);
    Q_INVOKABLE float getGamma();
    Q_INVOKABLE void setGamma(float gamma);
    Q_INVOKABLE GameEnums::COInfoPosition getCoInfoPosition();
    Q_INVOKABLE void setCoInfoPosition(const GameEnums::COInfoPosition value);
    Q_INVOKABLE float getBrightness();
    Q_INVOKABLE void setBrightness(float brightness);
    Q_INVOKABLE Qt::Key getKey_screenshot();
    Q_INVOKABLE void setKey_screenshot(const Qt::Key key_screenshot);
    Q_INVOKABLE GameEnums::AutoFocusing getAutoFocusing();
    Q_INVOKABLE void setAutoFocusing(const GameEnums::AutoFocusing autoFocusing);
    Q_INVOKABLE Qt::Key getKey_EditorPlaceTerrain();
    Q_INVOKABLE void setKey_EditorPlaceTerrain(const Qt::Key key_EditorPlaceTerrain);
    Q_INVOKABLE Qt::Key getKey_EditorPlaceBuilding();
    Q_INVOKABLE void setKey_EditorPlaceBuilding(const Qt::Key key_EditorPlaceBuilding);
    Q_INVOKABLE Qt::Key getKey_EditorPlaceUnit();
    Q_INVOKABLE void setKey_EditorPlaceUnit(const Qt::Key key_EditorPlaceUnit);
    Q_INVOKABLE Qt::Key getKey_EditorNextTeam();
    Q_INVOKABLE void setKey_EditorNextTeam(const Qt::Key key_EditorNextTeam);
    Q_INVOKABLE Qt::Key getKey_EditorPreviousTeam();
    Q_INVOKABLE void setKey_EditorPreviousTeam(const Qt::Key key_EditorPreviousTeam);
    Q_INVOKABLE Qt::Key getKey_EditorSelectionRight();
    Q_INVOKABLE void setKey_EditorSelectionRight(const Qt::Key key_EditorSelectionRight);
    Q_INVOKABLE Qt::Key getKey_EditorSelectionLeft();
    Q_INVOKABLE void setKey_EditorSelectionLeft(const Qt::Key key_EditorSelectionLeft);
    Q_INVOKABLE Qt::Key getKey_ShowAttackFields2();
    Q_INVOKABLE void setKey_ShowAttackFields2(const Qt::Key key_ShowAttackFields2);
    Q_INVOKABLE Qt::Key getKey_ShowIndirectAttackFields2();
    Q_INVOKABLE void setKey_ShowIndirectAttackFields2(const Qt::Key key_ShowIndirectAttackFields2);
    Q_INVOKABLE Qt::Key getKey_up2();
    Q_INVOKABLE void setKey_up2(const Qt::Key key_up2);
    Q_INVOKABLE Qt::Key getKey_down2();
    Q_INVOKABLE void setKey_down2(const Qt::Key key_down2);
    Q_INVOKABLE Qt::Key getKey_right2();
    Q_INVOKABLE void setKey_right2(const Qt::Key key_right2);
    Q_INVOKABLE Qt::Key getKey_left2();
    Q_INVOKABLE void setKey_left2(const Qt::Key key_left2);
    Q_INVOKABLE Qt::Key getKey_confirm2();
    Q_INVOKABLE void setKey_confirm2(const Qt::Key key_confirm2);
    Q_INVOKABLE Qt::Key getKey_cancel2();
    Q_INVOKABLE void setKey_cancel2(const Qt::Key key_cancel2);
    Q_INVOKABLE Qt::Key getKey_next2();
    Q_INVOKABLE void setKey_next2(const Qt::Key key_next2);
    Q_INVOKABLE Qt::Key getKey_previous2();
    Q_INVOKABLE void setKey_previous2(const Qt::Key key_previous2);
    Q_INVOKABLE Qt::Key getKey_information2();
    Q_INVOKABLE void setKey_information2(const Qt::Key key_information2);
    Q_INVOKABLE Qt::Key getKey_moveMapUp2();
    Q_INVOKABLE void setKey_moveMapUp2(const Qt::Key key_moveMapUp2);
    Q_INVOKABLE Qt::Key getKey_moveMapDown2();
    Q_INVOKABLE void setKey_moveMapDown2(const Qt::Key key_moveMapDown2);
    Q_INVOKABLE Qt::Key getKey_moveMapRight2();
    Q_INVOKABLE void setKey_moveMapRight2(const Qt::Key key_moveMapRight2);
    Q_INVOKABLE Qt::Key getKey_moveMapLeft2();
    Q_INVOKABLE void setKey_moveMapLeft2(const Qt::Key key_moveMapLeft2);
    Q_INVOKABLE Qt::Key getKey_MapZoomOut2();
    Q_INVOKABLE void setKey_MapZoomOut2(const Qt::Key key_MapZoomOut2);
    Q_INVOKABLE Qt::Key getKey_MapZoomIn2();
    Q_INVOKABLE void setKey_MapZoomIn2(const Qt::Key key_MapZoomIn2);
    Q_INVOKABLE bool getAutoCamera();
    Q_INVOKABLE void setAutoCamera(bool autoCamera);
    Q_INVOKABLE bool getAutoScrolling();
    Q_INVOKABLE void setAutoScrolling(bool autoScrolling);
    Q_INVOKABLE bool getLogActions();
    Q_INVOKABLE void setLogActions(bool LogActions);
    Q_INVOKABLE bool getShowIngameCoordinates();
    Q_INVOKABLE void setShowIngameCoordinates(bool showIngameCoordinates);
    Q_INVOKABLE quint32 getWalkAnimationSpeedValue();
    Q_INVOKABLE void setWalkAnimationSpeed(const quint32 value);
    Q_INVOKABLE float getWalkAnimationSpeed();
    Q_INVOKABLE qint32 getShowCoCount();
    Q_INVOKABLE void setShowCoCount(const qint32 showCoCount);
    Q_INVOKABLE std::chrono::seconds getAutoSavingCylceTime();
    Q_INVOKABLE quint64 getAutoSavingCylceTimeRaw();
    Q_INVOKABLE void setAutoSavingCylceTimeRaw(const quint32 value);
    Q_INVOKABLE qint32 getAutoSavingCycle();
    Q_INVOKABLE void setAutoSavingCycle(const qint32 value);
    Q_INVOKABLE Qt::Key getKey_ShowIndirectAttackFields();
    Q_INVOKABLE void setKey_ShowIndirectAttackFields(const Qt::Key key_ShowIndirectAttackFields);
    Q_INVOKABLE Qt::Key getKey_ShowAttackFields();
    Q_INVOKABLE void setKey_ShowAttackFields(const Qt::Key key_ShowAttackFields);
    Q_INVOKABLE QString getLanguage();
    Q_INVOKABLE void setLanguage(const QString language);
    Q_INVOKABLE bool getRecord();
    Q_INVOKABLE void setRecord(bool record);
    Q_INVOKABLE bool getStaticMarkedFields();
    Q_INVOKABLE void setStaticMarkedFields(bool staticMarkedFields);
    Q_INVOKABLE qint32 getMenuItemCount();
    Q_INVOKABLE void setMenuItemCount(const qint32 MenuItemCount);
    Q_INVOKABLE QString getModString();
    Q_INVOKABLE void filterCosmeticMods(QStringList mods, QStringList versions, bool filter);
    Q_INVOKABLE QString getConfigString(QStringList mods);
    Q_INVOKABLE quint32 getMultiTurnCounter();
    Q_INVOKABLE void setMultiTurnCounter(const quint32 value);
    Q_INVOKABLE bool getShowCursor();
    Q_INVOKABLE void setShowCursor(bool ShowCursor);
    Q_INVOKABLE bool getAutoEndTurn();
    Q_INVOKABLE void setAutoEndTurn(bool AutoEndTurn);
    Q_INVOKABLE GameEnums::BattleAnimationType getBattleAnimationType();
    Q_INVOKABLE void setBattleAnimationType(const GameEnums::BattleAnimationType value);
    Q_INVOKABLE void setUsername(const QString Username);
    Q_INVOKABLE QString getUsername();
    Q_INVOKABLE void setX(const qint32 x);
    Q_INVOKABLE inline qint32 getX()
    {
        return m_pInstance->m_x;
    }
    Q_INVOKABLE void setY(const qint32 y);
    Q_INVOKABLE inline qint32 getY()
    {
        return m_pInstance->m_y;
    }
    Q_INVOKABLE inline qint32 getWidth()
    {
        return m_pInstance->m_width;
    }
    Q_INVOKABLE inline qint32 getHeight()
    {
        return m_pInstance->m_height;
    }
    Q_INVOKABLE qint32 getStageWidth();
    Q_INVOKABLE qint32 getStageHeight();
    Q_INVOKABLE inline bool getBorderless()
    {
        return m_pInstance->m_borderless;
    }
    Q_INVOKABLE inline bool getFullscreen()
    {
        return m_pInstance->m_fullscreen;
    }
    Q_INVOKABLE inline Qt::Key getKeyConsole()
    {
        return m_pInstance->m_key_console;
    }
    Q_INVOKABLE inline void setTotalVolume(qint32 value)
    {
        m_pInstance->m_TotalVolume = value;
    }
    Q_INVOKABLE inline qint32 getTotalVolume()
    {
        return m_pInstance->m_TotalVolume;
    }
    Q_INVOKABLE inline void setMusicVolume(qint32 value)
    {
        m_pInstance->m_MusicVolume = value;
    }
    Q_INVOKABLE inline qint32 getMusicVolume()
    {
        return m_pInstance->m_MusicVolume;
    }
    Q_INVOKABLE inline void setSoundVolume(qint32 value)
    {
        m_pInstance->m_SoundVolume = value;
    }
    Q_INVOKABLE inline qint32 getSoundVolume()
    {
        return m_pInstance->m_SoundVolume;
    }
    Q_INVOKABLE inline quint16 getGamePort()
    {
        return m_pInstance->m_GamePort;
    }
    Q_INVOKABLE inline void setGamePort(quint16 value)
    {
        m_pInstance->m_GamePort = value;
    }
    Q_INVOKABLE void setServer(bool Server);
    Q_INVOKABLE inline bool getServer()
    {
        return m_pInstance->m_Server;
    }
    Q_INVOKABLE inline QStringList getMods()
    {
        return m_pInstance->m_activeMods;
    }
    Q_INVOKABLE void addMod(QString mod)
    {
        if (!m_pInstance->m_activeMods.contains(mod))
        {
            m_pInstance->m_activeMods.append(mod);
        }
    }
    Q_INVOKABLE void removeMod(QString mod)
    {
        if (m_pInstance->m_activeMods.contains(mod))
        {
            m_pInstance->m_activeMods.removeOne(mod);
        }
    }
    Q_INVOKABLE float getMouseSensitivity();
    Q_INVOKABLE void setMouseSensitivity(float value);
    Q_INVOKABLE GameEnums::BattleAnimationMode getBattleAnimationMode();
    Q_INVOKABLE void setBattleAnimationMode(GameEnums::BattleAnimationMode value);
    Q_INVOKABLE void setFullscreen(bool fullscreen);
    Q_INVOKABLE void setBorderless(bool borderless);
    Q_INVOKABLE void setWidth(const qint32 width);
    Q_INVOKABLE void setHeight(const qint32 height);
    Q_INVOKABLE float getAnimationSpeedValue();
    Q_INVOKABLE float getAnimationSpeed();
    Q_INVOKABLE void setAnimationSpeed(const quint32 value);
    Q_INVOKABLE float getBattleAnimationSpeedValue();
    Q_INVOKABLE float getBattleAnimationSpeed();
    Q_INVOKABLE void setBattleAnimationSpeed(const quint32 value);
    Q_INVOKABLE float getDialogAnimationSpeedValue();
    Q_INVOKABLE float getDialogAnimationSpeed();
    Q_INVOKABLE void setDialogAnimationSpeed(const quint32 value);
    Q_INVOKABLE float getCaptureAnimationSpeedValue();
    Q_INVOKABLE float getCaptureAnimationSpeed();
    Q_INVOKABLE void setCaptureAnimationSpeed(const quint32 value);
    Q_INVOKABLE Qt::Key getKey_up();
    Q_INVOKABLE void setKey_up(const Qt::Key key_up);
    Q_INVOKABLE Qt::Key getKey_down();
    Q_INVOKABLE void setKey_down(const Qt::Key key_down);
    Q_INVOKABLE Qt::Key getKey_right();
    Q_INVOKABLE void setKey_right(const Qt::Key key_right);
    Q_INVOKABLE Qt::Key getKey_left();
    Q_INVOKABLE void setKey_left(const Qt::Key key_left);
    Q_INVOKABLE Qt::Key getKey_confirm();
    Q_INVOKABLE void setKey_confirm(const Qt::Key key_confirm);
    Q_INVOKABLE Qt::Key getKey_cancel();
    Q_INVOKABLE void setKey_cancel(const Qt::Key key_cancel);
    Q_INVOKABLE Qt::Key getKey_next();
    Q_INVOKABLE void setKey_next(const Qt::Key key_next);
    Q_INVOKABLE Qt::Key getKey_previous();
    Q_INVOKABLE void setKey_previous(const Qt::Key key_previous);
    Q_INVOKABLE Qt::Key getKey_quicksave1();
    Q_INVOKABLE void setKey_quicksave1(const Qt::Key key_quicksave1);
    Q_INVOKABLE Qt::Key getKey_quicksave2();
    Q_INVOKABLE void setKey_quicksave2(const Qt::Key key_quicksave2);
    Q_INVOKABLE Qt::Key getKey_quickload1();
    Q_INVOKABLE void setKey_quickload1(const Qt::Key key_quickload1);
    Q_INVOKABLE Qt::Key getKey_quickload2();
    Q_INVOKABLE void setKey_quickload2(const Qt::Key key_quickload2);
    Q_INVOKABLE Qt::Key getKey_moveMapUp();
    Q_INVOKABLE void setKey_moveMapUp(const Qt::Key key_moveMapUp);
    Q_INVOKABLE Qt::Key getKey_moveMapDown();
    Q_INVOKABLE void setKey_moveMapDown(const Qt::Key key_moveMapDown);
    Q_INVOKABLE Qt::Key getKey_moveMapRight();
    Q_INVOKABLE void setKey_moveMapRight(const Qt::Key key_moveMapRight);
    Q_INVOKABLE Qt::Key getKey_moveMapLeft();
    Q_INVOKABLE void setKey_moveMapLeft(const Qt::Key key_moveMapLeft);
    Q_INVOKABLE Qt::Key getKey_information();
    Q_INVOKABLE void setKey_information(const Qt::Key key_information);
    Q_INVOKABLE Qt::Key getKey_MapZoomOut();
    Q_INVOKABLE void setKey_MapZoomOut(const Qt::Key key_MapZoomOut);
    Q_INVOKABLE Qt::Key getKey_MapZoomIn();
    Q_INVOKABLE void setKey_MapZoomIn(const Qt::Key key_MapZoomIn);
    Q_INVOKABLE Qt::Key getKey_Escape();
    Q_INVOKABLE void setKey_Escape(const Qt::Key key_Escape);
    /**
     * @brief getAvailableMods
     * @return
     */
    Q_INVOKABLE QStringList getAvailableMods();
    /**
     * @brief getModName
     * @param mod
     * @return
     */
    Q_INVOKABLE QString getModName(QString mod);
    /**
     * @brief getModVersion
     * @param mod
     * @return
     */
    Q_INVOKABLE QString getModVersion(QString mod);
    /**
     * @brief getIsCosmetic
     * @param mod
     * @return
     */
    Q_INVOKABLE bool getIsCosmetic(QString mod);
    /**
     * @brief getAudioDevices
     * @return
     */
    Q_INVOKABLE QStringList getAudioDevices();
    /**
     * @brief getCurrentDevice
     * @return
     */
    Q_INVOKABLE qint32 getCurrentDevice();
    /**
     * @brief setAudioDevice
     * @param value
     */
    Q_INVOKABLE void setAudioDevice(qint32 value);
    /**
     * @brief getScreenSize
     * @return
     */
    Q_INVOKABLE QSize getScreenSize();
    /**
     * @brief getScreenMode
     * @return
     */
    Q_INVOKABLE qint32 getScreenMode();
    /**
     * @brief setScreenMode
     * @param value
     */
    Q_INVOKABLE void setScreenMode(qint32 value);
    /**
     * @brief changeBrightness
     * @param value
     */
    Q_INVOKABLE void changeBrightness(qint32 value);
    /**
     * @brief changeGamma
     * @param value
     */
    Q_INVOKABLE void changeGamma(float value);
    /**
     * @brief isGamepadSupported
     * @return
     */
    Q_INVOKABLE bool isGamepadSupported();
    /**
     * @brief getLanguageNames
     * @return
     */
    Q_INVOKABLE QStringList getLanguageNames();
    /**
     * @brief getLanguageIds
     * @return
     */
    Q_INVOKABLE QStringList getLanguageIds();
    /**
     * @brief getCurrentLanguageIndex
     * @return
     */
    Q_INVOKABLE qint32 getCurrentLanguageIndex();
    /**
     * @brief getZoomModifier
     * @return
     */
    Q_INVOKABLE float getZoomModifier() const;
    /**
     * @brief setZoomModifier
     * @param newZoomModifier
     */
    Q_INVOKABLE void setZoomModifier(float newZoomModifier);
    /**
     * @brief getMuteOnFcousedLost
     * @return
     */
    Q_INVOKABLE bool getMuteOnFcousedLost() const;
    /**
     * @brief setMuteOnFcousedLost
     * @param newMuteOnFcousedLost
     */
    Q_INVOKABLE void setMuteOnFcousedLost(bool newMuteOnFcousedLost);
    /**
     * @brief getCreateAiTrainingData
     * @return
     */
    Q_INVOKABLE bool getCreateAiTrainingData() const;
    /**
     * @brief setCreateAiTrainingData
     * @param newCreateAiTrainingData
     */
    Q_INVOKABLE void setCreateAiTrainingData(bool newCreateAiTrainingData);

    bool getAllowMapUpload() const;
    void setAllowMapUpload(bool newAllowMapUpload);

    std::chrono::seconds getReplayDeleteTime() const;
    void setReplayDeleteTime(const std::chrono::seconds & newReplayDeleteTime);

private:
    friend class MemoryManagement;
    explicit Settings();

private:
    // setting variables
    QVector<spValueBase> m_SettingValues;
    qint32 m_x{0};
    qint32 m_y{0};
    qint32 m_width{1024};
    qint32 m_height{800};
    quint8 m_screen{0};
    float m_brightness{0.0f};
    float m_gamma{1.0f};
    bool m_smallScreenDevice{false};
    bool m_touchScreen{false};
    qint32 m_touchPointSensitivity{15};
    bool m_gamepadEnabled{false};
    float m_gamepadSensitivity{1.0f};
    qint32 m_framesPerSecond{60};
    float m_ingameMenuScaling{1.0f};
    bool m_automaticUpdates{true};
    float m_gameScale{1.0f};
    float m_zoomModifier{2.0f};

    bool m_borderless{true};
    bool m_fullscreen{false};

    Qt::Key m_key_escape{Qt::Key_Escape};
    Qt::Key m_key_console{Qt::Key_F1};
    Qt::Key m_key_screenshot{Qt::Key_F5};
    Qt::Key m_key_mapshot{Qt::Key_F6};
    Qt::Key m_key_up{Qt::Key_W};
    Qt::Key m_key_down{Qt::Key_S};
    Qt::Key m_key_right{Qt::Key_D};
    Qt::Key m_key_left{Qt::Key_A};
    Qt::Key m_key_confirm{Qt::Key_Space};
    Qt::Key m_key_cancel{Qt::Key_B};
    Qt::Key m_key_next{Qt::Key_E};
    Qt::Key m_key_previous{Qt::Key_Q};
    Qt::Key m_key_quicksave1{Qt::Key_F9};
    Qt::Key m_key_quicksave2{Qt::Key_F11};
    Qt::Key m_key_quickload1{Qt::Key_F10};
    Qt::Key m_key_quickload2{Qt::Key_F12};
    Qt::Key m_key_information{Qt::Key_I};
    Qt::Key m_key_moveMapUp{Qt::Key_Down};
    Qt::Key m_key_moveMapDown{Qt::Key_Up};
    Qt::Key m_key_moveMapRight{Qt::Key_Left};
    Qt::Key m_key_moveMapLeft{Qt::Key_Right};
    Qt::Key m_key_MapZoomOut{Qt::Key_Minus};
    Qt::Key m_key_MapZoomIn{Qt::Key_Plus};
    Qt::Key m_key_ShowAttackFields{Qt::Key_2};
    Qt::Key m_key_ShowIndirectAttackFields{Qt::Key_1};
    Qt::Key m_key_up2{Qt::Key(0)};
    Qt::Key m_key_down2{Qt::Key(0)};
    Qt::Key m_key_right2{Qt::Key(0)};
    Qt::Key m_key_left2{Qt::Key(0)};
    Qt::Key m_key_confirm2{Qt::Key_Return};
    Qt::Key m_key_cancel2{Qt::Key_Backspace};
    Qt::Key m_key_next2{Qt::Key_Tab};
    Qt::Key m_key_previous2{Qt::Key(0)};
    Qt::Key m_key_information2{Qt::Key(0)};
    Qt::Key m_key_moveMapUp2{Qt::Key(0)};
    Qt::Key m_key_moveMapDown2{Qt::Key(0)};
    Qt::Key m_key_moveMapRight2{Qt::Key(0)};
    Qt::Key m_key_moveMapLeft2{Qt::Key(0)};
    Qt::Key m_key_MapZoomOut2{Qt::Key(0)};
    Qt::Key m_key_MapZoomIn2{Qt::Key(0)};
    Qt::Key m_key_ShowAttackFields2{Qt::Key(0)};
    Qt::Key m_key_ShowIndirectAttackFields2{Qt::Key(0)};

    Qt::Key m_key_EditorPlaceTerrain{Qt::Key_1};
    Qt::Key m_key_EditorPlaceBuilding{Qt::Key_2};
    Qt::Key m_key_EditorPlaceUnit{Qt::Key_3};
    Qt::Key m_key_EditorNextTeam{Qt::Key_Tab};
    Qt::Key m_key_EditorPreviousTeam{Qt::Key_Asterisk};
    Qt::Key m_key_EditorSelectionUp;
    Qt::Key m_key_EditorSelectionDown;
    Qt::Key m_key_EditorSelectionRight{Qt::Key_T};
    Qt::Key m_key_EditorSelectionLeft{Qt::Key_R};


    QString m_language{"en"};
    // Sound
    qint32 m_TotalVolume{100};
    qint32 m_MusicVolume{80};
    qint32 m_SoundVolume{100};
    QVariant m_audioOutput;
    bool m_muted{false};
    bool m_muteOnFcousedLost{true};
    // Network
    quint16 m_GamePort{9001};
    QString m_Username;
    QString m_slaveServerName;
    QString m_serverPassword;
    bool m_allowMapUpload{true};

    bool m_Server{false};
    quint16 m_ServerPort{9002};
    quint16 m_slaveServerPort{9003};
    QString m_ServerAdress{"192.46.216.113"};
    QString m_secondaryServerAdress{"2600:3c00::f03c:93ff:fe86:009e"};
    QString m_serverListenAdress;
    QString m_serverSecondaryListenAdress;
    QString m_slaveListenAdress{"::1"};
    QString m_slaveHostOptions{"::1&&10000&20000;::1&&50000&65535"};
    std::chrono::seconds m_slaveDespawnTime{std::chrono::minutes(0)};
    std::chrono::seconds m_suspendedDespawnTime{std::chrono::minutes(0)};
    std::chrono::seconds m_replayDeleteTime{std::chrono::minutes(0)};

    // mailing
    QString m_mailServerAddress;
    quint16 m_mailServerPort{0};
    qint32 m_mailServerConnectionType{2};
    QString m_mailServerUsername;
    QString m_mailServerPassword;
    QString m_mailServerSendAddress;
    qint32 m_mailServerAuthMethod{1};

    // auto saving
    std::chrono::seconds m_autoSavingCylceTime{std::chrono::minutes(0)};
    qint32 m_autoSavingCycle{0};


    // ingame options
    bool m_overworldAnimations{true};
    GameEnums::BattleAnimationMode m_battleAnimationsMode{GameEnums::BattleAnimationMode_All};
    GameEnums::BattleAnimationType m_battleAnimationType{GameEnums::BattleAnimationType_Detail};
    quint32 m_animationSpeed{1};
    quint32 m_walkAnimationSpeed{20};
    quint32 m_battleAnimationSpeed{1};
    quint32 m_dialogAnimationSpeed{20};
    quint32 m_captureAnimationSpeed{1};
    bool m_useCoMinis{true};
    bool m_dialogAnimation{true};
    bool m_captureAnimation{true};
    bool m_day2dayScreen{true};
    bool m_movementAnimations{true};
    quint32 multiTurnCounter{4};
    QString m_LastSaveGame;
    QString m_defaultRuleset;
    QString m_defaultBannlist;
    bool m_ShowCursor{true};
    bool m_AutoEndTurn{false};
    qint32 m_MenuItemCount{13};
    qint32 m_MenuItemRowCount{2};
    bool m_StaticMarkedFields{false};
    float m_mouseSensitivity{-0.75f};
    bool m_record{true};
    qint32 m_showCoCount{0};
    bool m_showIngameCoordinates{true};
    GameEnums::COInfoPosition m_coInfoPosition{GameEnums::COInfoPosition_Flipping};
    bool m_autoScrolling{true};
    bool m_autoCamera{true};
    GameEnums::AutoFocusing m_autoFocusing{GameEnums::AutoFocusing_LastPos};
    bool m_centerOnMarkedField{false};
    bool m_syncAnimations{false};
    bool m_simpleDeselect{false};
    bool m_showDetailedBattleForcast{true};
    bool m_autoMoveCursor{false};
    float m_supplyWarning{0.33f};    

    qint32 m_pauseAfterAction{0};
    bool m_createAiTrainingData{false};

    // internal members
    static spSettings m_pInstance;
#ifdef USEAPPCONFIGPATH
    QString m_settingFile = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/Commander_Wars.ini";
#else
    QString m_settingFile = "Commander_Wars.ini";
#endif
    QStringList m_activeMods;
    QStringList m_activeModVersions;
    QString m_userPath;
    QVector<spQTranslator> m_translators;
    QString m_updateStep;
    QString m_pipeUuid;
    bool m_spawnAiProcess{DEFAULTAIPIPE};
    bool m_aiSlave{false};

    // logging
    bool m_LogActions{false};
    GameConsole::eLogLevels m_defaultLogLevel{static_cast<GameConsole::eLogLevels>(DEBUG_LEVEL)};
    quint64 m_defaultLogModuls{GameConsole::eGeneral | GameConsole::eJavaScript};
};

Q_DECLARE_INTERFACE(Settings, "Settings");

#endif // SETTINGS_H
