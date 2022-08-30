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

#include "coreengine/console.h"

#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

#include "game/GameEnums.h"

class GLGraphicsView;
class Settings;
using spSettings = oxygine::intrusive_ptr<Settings>;

class Settings : public QObject, public oxygine::ref_counter
{
public:
    static const char* const DEFAULT_AUDIODEVICE;
private:
    Q_OBJECT
    struct ValueBase
    {
        virtual void readValue(QSettings & settings) = 0;
        virtual void saveValue(QSettings & settings) = 0;
        virtual void resetValue() = 0;
    };

    template<typename TType>
    struct Value : public ValueBase
    {
        Value(const char* const group, const char* const name, TType* value, TType defaultValue, TType minValue, TType maxValue, bool excludeFromReset = false)
            : m_group{group},
              m_name{name},
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
            if constexpr(std::is_floating_point<TType>::value)
            {
                bool ok = false;
                *m_value = settings.value(m_name, m_defaultValue).toFloat(&ok);
                if(!ok || *m_value <= m_minValue || *m_value >= m_maxValue)
                {
                    QString error = "Error in the Ini File: [" + QString(m_group) + "] Setting: " + QString(m_name);
                    CONSOLE_PRINT(error, Console::eERROR);
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
                               std::is_same<TType, quint16>::value)
            {
                bool ok = false;
                *m_value = settings.value(m_name, m_defaultValue).toUInt(&ok);
                if(!ok || *m_value < m_minValue || *m_value > m_maxValue)
                {
                    QString error = "Error in the Ini File: [" + QString(m_group) + "] Setting: " + QString(m_name);
                    CONSOLE_PRINT(error, Console::eERROR);
                    *m_value = m_defaultValue;
                }
            }
            else if constexpr (std::is_same<TType, qint32>::value)
            {
                bool ok = false;
                *m_value = settings.value(m_name, m_defaultValue).toInt(&ok);
                if(!ok || *m_value < m_minValue || *m_value > m_maxValue)
                {
                    QString error = "Error in the Ini File: [" + QString(m_group) + "] Setting: " + QString(m_name);
                    CONSOLE_PRINT(error, Console::eERROR);
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
                    CONSOLE_PRINT(error, Console::eERROR);
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
                    CONSOLE_PRINT(error, Console::eERROR);
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
                    CONSOLE_PRINT(error, Console::eERROR);
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
                settings.setValue(m_name, Settings::getConfigString(*m_value));
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
        const char* const m_group;
        const char* const m_name;
        TType m_defaultValue;
        TType m_minValue;
        TType m_maxValue;
        TType* m_value;
        bool m_excludeFromReset;
    };
#ifdef AUDIOSUPPORT
    struct AudioDeviceValue : public ValueBase
    {
        AudioDeviceValue(const char* const group, const char* const name, QVariant* value, QString defaultValue)
            : m_group{group},
              m_name{name},
              m_value{value},
              m_defaultValue{defaultValue}
        {
        }
        virtual void readValue(QSettings & settings) override
        {
            settings.beginGroup(m_group);
            const QAudioDevice &defaultDeviceInfo = QMediaDevices::defaultAudioOutput();
            QString description = settings.value(m_name, m_defaultValue).toString();
            if (description == DEFAULT_AUDIODEVICE)
            {
                *m_value = QVariant(DEFAULT_AUDIODEVICE);
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
                if (m_audioOutput.value<QAudioDevice>().isNull())
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
                m_value->toString() == DEFAULT_AUDIODEVICE)
            {
                settings.setValue(m_name, DEFAULT_AUDIODEVICE);
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
            *m_value = QVariant(Settings::DEFAULT_AUDIODEVICE);
        }
    private:
        const char* const m_group;
        const char* const m_name;
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
    static void shutdown();

    static void loadSettings();
    static void saveSettings();
    static void resetSettings();

    static QString getLastSaveGame();
    static void setLastSaveGame(const QString &LastSaveGame);

public slots:
    static QString getServerAdress();
    static void setServerAdress(const QString &ServerAdress);

    static QString getSecondaryServerAdress();
    static void setSecondaryServerAdress(const QString &newSecondaryServerAdress);

    static quint16 getServerPort();
    static void setServerPort(const quint16 &ServerPort);

    static const QString &getServerPassword();
    static void setServerPassword(const QString &newServerPassword);

    static const QString &getMailServerSendAddress();
    static void setMailServerSendAddress(const QString &newMailServerSendAddress);

    static qint32 getMailServerAuthMethod();
    static void setMailServerAuthMethod(qint32 newMailServerAuthMethod);

    static const QString &getMailServerAddress();
    static void setMailServerAddress(const QString &newMailServerAddress);

    static quint16 getMailServerPort();
    static void setMailServerPort(quint16 newMailServerPort);

    static qint32 getMailServerConnectionType();
    static void setMailServerConnectionType(qint32 newMailServerConnectionType);

    static QString getMailServerUsername();
    static void setMailServerUsername(QString newMailServerUsername);

    static QString getMailServerPassword();
    static void setMailServerPassword(QString newMailServerPassword);

    static const std::chrono::seconds &getSlaveDespawnTime();
    static void setSlaveDespawnTime(const std::chrono::seconds &newSlaveDespawnTime);

    static QString getDefaultBannlist();
    static void setDefaultBannlist(const QString &newDefaultBannlist);

    static bool getUseHighDpi();
    static void setUseHighDpi(bool newUseHighDpi);

    static bool getDay2dayScreen();
    static void setDay2dayScreen(bool newDay2dayScreen);

    static bool getMovementAnimations();
    static void setMovementAnimations(bool newMovementAnimations);

    static bool getCaptureAnimation();
    static void setCaptureAnimation(bool newCaptureAnimation);

    static const QString &getSlaveListenAdress();
    static void setSlaveListenAdress(const QString &newSlaveListenAdress);

    static qint32 getPauseAfterAction();
    static void setPauseAfterAction(qint32 newPauseAfterAction);

    static const QString &getServerListenAdress();
    static void setServerListenAdress(const QString &newServerListenAdress);

    static const QString &getServerSecondaryListenAdress();
    static void setServerSecondaryListenAdress(const QString &newServerSecondaryListenAdress);

    static quint16 getSlaveServerPort();
    static void setSlaveServerPort(quint16 newSlaveServerPort);

    static QString getSlaveHostOptions();
    static void setSlaveHostOptions(const QString &newSlaveHostOptions);

    static Qt::Key getKey_mapshot();
    static void setKey_mapshot(Qt::Key newKey_mapshot);

    static qint32 getFramesPerSecond();
    static void setFramesPerSecond(qint32 newFramesPerSecond);

    static bool getMuted();
    static void setMuted(bool newMuted);

    static float getSupplyWarning();
    static void setSupplyWarning(float newSupplyWarning);

    static bool getGamepadEnabled();
    static void setGamepadEnabled(bool newGamepadEnabled);

    static float getGamepadSensitivity();
    static void setGamepadSensitivity(float newGamepadSensitivity);

    static QString getDefaultRuleset();
    static void setDefaultRuleset(const QString &newDefaultRuleset);

    static bool getUseCoMinis();
    static void setUseCoMinis(bool newUseCoMinis);

    static bool getOverworldAnimations();
    static void setOverworldAnimations(bool newOverworldAnimations);

    static qint32 getTouchPointSensitivity();
    static void setTouchPointSensitivity(qint32 newTouchPointSensitivity);

    static const QVariant &getAudioOutput();
    static void setAudioOutput(const QVariant &newAudioOutput);

    static bool getAutoMoveCursor();
    static void setAutoMoveCursor(bool newAutoMoveCursor);

    static bool getShowDetailedBattleForcast();
    static void setShowDetailedBattleForcast(bool newShowDetailedBattleForcast);

    static QString getUserPath();
    static void setUserPath(const QString &newUserPath);

    static bool getTouchScreen();
    static void setTouchScreen(bool newTouchScreen);

    static bool hasSmallScreen();
    static bool getSmallScreenDevice();
    static void setSmallScreenDevice(bool newSmallScreenDevice);

    static qint32 getMenuItemRowCount();
    static void setMenuItemRowCount(qint32 newMenuItemRowCount);

    static bool getSimpleDeselect();
    static void setSimpleDeselect(bool newSimpleDeselect);

    static QStringList getActiveModVersions();
    static QStringList getActiveMods();
    static void setActiveMods(const QStringList &activeMods);

    static QString getSlaveServerName();
    static void setSlaveServerName(const QString &slaveServerName);

    static bool getSyncAnimations();
    static void setSyncAnimations(bool syncAnimations);

    static bool getCenterOnMarkedField();
    static void setCenterOnMarkedField(bool centerOnMarkedField);

    static bool getDialogAnimation();
    static void setDialogAnimation(bool dialogAnimation);

    static float getGamma();
    static void setGamma(float gamma);

    static GameEnums::COInfoPosition getCoInfoPosition();
    static void setCoInfoPosition(const GameEnums::COInfoPosition &value);

    static float getBrightness();
    static void setBrightness(float brightness);

    static Qt::Key getKey_screenshot();
    static void setKey_screenshot(const Qt::Key &key_screenshot);

    static GameEnums::AutoFocusing getAutoFocusing();
    static void setAutoFocusing(const GameEnums::AutoFocusing &autoFocusing);

    static Qt::Key getKey_EditorPlaceTerrain();
    static void setKey_EditorPlaceTerrain(const Qt::Key &key_EditorPlaceTerrain);

    static Qt::Key getKey_EditorPlaceBuilding();
    static void setKey_EditorPlaceBuilding(const Qt::Key &key_EditorPlaceBuilding);

    static Qt::Key getKey_EditorPlaceUnit();
    static void setKey_EditorPlaceUnit(const Qt::Key &key_EditorPlaceUnit);

    static Qt::Key getKey_EditorNextTeam();
    static void setKey_EditorNextTeam(const Qt::Key &key_EditorNextTeam);

    static Qt::Key getKey_EditorPreviousTeam();
    static void setKey_EditorPreviousTeam(const Qt::Key &key_EditorPreviousTeam);

    static Qt::Key getKey_EditorSelectionRight();
    static void setKey_EditorSelectionRight(const Qt::Key &key_EditorSelectionRight);

    static Qt::Key getKey_EditorSelectionLeft();
    static void setKey_EditorSelectionLeft(const Qt::Key &key_EditorSelectionLeft);

    static Qt::Key getKey_ShowAttackFields2();
    static void setKey_ShowAttackFields2(const Qt::Key &key_ShowAttackFields2);

    static Qt::Key getKey_ShowIndirectAttackFields2();
    static void setKey_ShowIndirectAttackFields2(const Qt::Key &key_ShowIndirectAttackFields2);

    static Qt::Key getKey_up2();
    static void setKey_up2(const Qt::Key &key_up2);

    static Qt::Key getKey_down2();
    static void setKey_down2(const Qt::Key &key_down2);

    static Qt::Key getKey_right2();
    static void setKey_right2(const Qt::Key &key_right2);

    static Qt::Key getKey_left2();
    static void setKey_left2(const Qt::Key &key_left2);

    static Qt::Key getKey_confirm2();
    static void setKey_confirm2(const Qt::Key &key_confirm2);

    static Qt::Key getKey_cancel2();
    static void setKey_cancel2(const Qt::Key &key_cancel2);

    static Qt::Key getKey_next2();
    static void setKey_next2(const Qt::Key &key_next2);

    static Qt::Key getKey_previous2();
    static void setKey_previous2(const Qt::Key &key_previous2);

    static Qt::Key getKey_information2();
    static void setKey_information2(const Qt::Key &key_information2);

    static Qt::Key getKey_moveMapUp2();
    static void setKey_moveMapUp2(const Qt::Key &key_moveMapUp2);

    static Qt::Key getKey_moveMapDown2();
    static void setKey_moveMapDown2(const Qt::Key &key_moveMapDown2);

    static Qt::Key getKey_moveMapRight2();
    static void setKey_moveMapRight2(const Qt::Key &key_moveMapRight2);

    static Qt::Key getKey_moveMapLeft2();
    static void setKey_moveMapLeft2(const Qt::Key &key_moveMapLeft2);

    static Qt::Key getKey_MapZoomOut2();
    static void setKey_MapZoomOut2(const Qt::Key &key_MapZoomOut2);

    static Qt::Key getKey_MapZoomIn2();
    static void setKey_MapZoomIn2(const Qt::Key &key_MapZoomIn2);

    static bool getAutoCamera();
    static void setAutoCamera(bool autoCamera);

    static bool getAutoScrolling();
    static void setAutoScrolling(bool autoScrolling);

    static bool getLogActions();
    static void setLogActions(bool LogActions);

    static bool getShowIngameCoordinates();
    static void setShowIngameCoordinates(bool showIngameCoordinates);

    static quint32 getWalkAnimationSpeedValue();
    static void setWalkAnimationSpeed(const quint32 &value);
    static float getWalkAnimationSpeed();

    static qint32 getShowCoCount();
    static void setShowCoCount(const qint32 &showCoCount);

    static std::chrono::seconds getAutoSavingCylceTime();
    static void setAutoSavingCylceTime(const std::chrono::seconds &value);
    static quint64 getAutoSavingCylceTimeRaw();
    static void setAutoSavingCylceTimeRaw(quint64 &value);

    static qint32 getAutoSavingCycle();
    static void setAutoSavingCycle(const qint32 &value);

    static Qt::Key getKey_ShowIndirectAttackFields();
    static void setKey_ShowIndirectAttackFields(const Qt::Key &key_ShowIndirectAttackFields);

    static Qt::Key getKey_ShowAttackFields();
    static void setKey_ShowAttackFields(const Qt::Key &key_ShowAttackFields);

    static QString getLanguage();
    static void setLanguage(const QString &language);

    static bool getRecord();
    static void setRecord(bool record);

    static bool getStaticMarkedFields();
    static void setStaticMarkedFields(bool StaticMarkedFields);

    static qint32 getMenuItemCount();
    static void setMenuItemCount(const qint32 &MenuItemCount);

    static QString getModString();
    static void filterCosmeticMods(QStringList & mods, QStringList & versions, bool filter);
    static QString getConfigString(QStringList mods);

    static quint32 getMultiTurnCounter();
    static void setMultiTurnCounter(const quint32 &value);

    static bool getShowCursor();
    static void setShowCursor(bool ShowCursor);

    static bool getAutoEndTurn();
    static void setAutoEndTurn(bool AutoEndTurn);

    static GameEnums::BattleAnimationType getBattleAnimationType();
    static void setBattleAnimationType(const GameEnums::BattleAnimationType &value);

    static void setUsername(const QString &Username);
    static QString getUsername();
    static void setX(const qint32 &x);
    inline static qint32 getX()
    {
        return m_x;
    }
    static void setY(const qint32 &y);
    inline static qint32 getY()
    {
        return m_y;
    }
    inline static qint32 getWidth()
    {
        return m_width;
    }
    inline static qint32 getHeight()
    {
        return m_height;
    }

    inline static bool getBorderless()
    {
        return m_borderless;
    }

    inline static bool getFullscreen()
    {
        return m_fullscreen;
    }

    static inline Qt::Key getKeyConsole()
    {
        return m_key_console;
    }

    static inline void setTotalVolume(qint32 value)
    {
        m_TotalVolume = value;
    }
    static inline qint32 getTotalVolume()
    {
        return m_TotalVolume;
    }

    static inline void setMusicVolume(qint32 value)
    {
        m_MusicVolume = value;
    }
    static inline qint32 getMusicVolume()
    {
        return m_MusicVolume;
    }

    static inline void setSoundVolume(qint32 value)
    {
        m_SoundVolume = value;
    }
    static inline qint32 getSoundVolume()
    {
        return m_SoundVolume;
    }

    static inline quint16 getGamePort()
    {
        return m_GamePort;
    }
    static inline void setGamePort(quint16 value)
    {
        m_GamePort = value;
    }
    static void setServer(bool Server);
    static inline bool getServer()
    {
        return m_Server;
    }
    static inline QStringList getMods()
    {
        return m_activeMods;
    }
    static void addMod(QString mod)
    {
        if (!m_activeMods.contains(mod))
        {
            m_activeMods.append(mod);
        }
    }
    static void removeMod(QString mod)
    {
        if (m_activeMods.contains(mod))
        {
            m_activeMods.removeOne(mod);
        }
    }
    static float getMouseSensitivity();
    static void setMouseSensitivity(float value);

    static GameEnums::BattleAnimationMode getBattleAnimationMode();
    static void setBattleAnimationMode(GameEnums::BattleAnimationMode value);

    static void setFullscreen(bool fullscreen);

    static void setBorderless(bool borderless);

    static void setWidth(const qint32 &width);
    static void setHeight(const qint32 &height);

    static float getAnimationSpeedValue();
    static float getAnimationSpeed();
    static void setAnimationSpeed(const quint32 &value);

    static float getBattleAnimationSpeedValue();
    static float getBattleAnimationSpeed();
    static void setBattleAnimationSpeed(const quint32 &value);

    static float getDialogAnimationSpeedValue();
    static float getDialogAnimationSpeed();
    static void setDialogAnimationSpeed(const quint32 &value);

    static float getCaptureAnimationSpeedValue();
    static float getCaptureAnimationSpeed();
    static void setCaptureAnimationSpeed(const quint32 &value);

    static Qt::Key getKey_up();
    static void setKey_up(const Qt::Key &key_up);

    static Qt::Key getKey_down();
    static void setKey_down(const Qt::Key &key_down);

    static Qt::Key getKey_right();
    static void setKey_right(const Qt::Key &key_right);

    static Qt::Key getKey_left();
    static void setKey_left(const Qt::Key &key_left);

    static Qt::Key getKey_confirm();
    static void setKey_confirm(const Qt::Key &key_confirm);

    static Qt::Key getKey_cancel();
    static void setKey_cancel(const Qt::Key &key_cancel);

    static Qt::Key getKey_next();
    static void setKey_next(const Qt::Key &key_next);

    static Qt::Key getKey_previous();
    static void setKey_previous(const Qt::Key &key_previous);

    static Qt::Key getKey_quicksave1();
    static void setKey_quicksave1(const Qt::Key &key_quicksave1);

    static Qt::Key getKey_quicksave2();
    static void setKey_quicksave2(const Qt::Key &key_quicksave2);

    static Qt::Key getKey_quickload1();
    static void setKey_quickload1(const Qt::Key &key_quickload1);

    static Qt::Key getKey_quickload2();
    static void setKey_quickload2(const Qt::Key &key_quickload2);

    static Qt::Key getKey_moveMapUp();
    static void setKey_moveMapUp(const Qt::Key &key_moveMapUp);

    static Qt::Key getKey_moveMapDown();
    static void setKey_moveMapDown(const Qt::Key &key_moveMapDown);

    static Qt::Key getKey_moveMapRight();
    static void setKey_moveMapRight(const Qt::Key &key_moveMapRight);

    static Qt::Key getKey_moveMapLeft();
    static void setKey_moveMapLeft(const Qt::Key &key_moveMapLeft);

    static Qt::Key getKey_information();
    static void setKey_information(const Qt::Key &key_information);

    static Qt::Key getKey_MapZoomOut();
    static void setKey_MapZoomOut(const Qt::Key &key_MapZoomOut);

    static Qt::Key getKey_MapZoomIn();
    static void setKey_MapZoomIn(const Qt::Key &key_MapZoomIn);

    static void getModInfos(QString mod, QString & name, QString & description, QString & version,
                            QStringList & compatibleMods, QStringList & incompatibleMods,
                            QStringList & requiredMods, bool & isCosmetic,
                            QStringList & tags, QString & thumbnail);
    /**
     * @brief getAvailableMods
     * @return
     */
    static QStringList getAvailableMods();
    /**
     * @brief getModName
     * @param mod
     * @return
     */
    static QString getModName(QString mod);
    /**
     * @brief getIsCosmetic
     * @param mod
     * @return
     */
    static bool getIsCosmetic(QString mod);
    /**
     * @brief getAudioDevices
     * @return
     */
    static QStringList getAudioDevices();
    /**
     * @brief getCurrentDevice
     * @return
     */
    static qint32 getCurrentDevice();
    /**
     * @brief setAudioDevice
     * @param value
     */
    static void setAudioDevice(qint32 value);
    /**
     * @brief getScreenSize
     * @return
     */
    static QSize getScreenSize();
    /**
     * @brief Settings::getDpiFactor
     * @return
     */
    static float getDpiFactor();
    /**
     * @brief getScreenMode
     * @return
     */
    static qint32 getScreenMode();
    /**
     * @brief setScreenMode
     * @param value
     */
    static void setScreenMode(qint32 value);
    /**
     * @brief changeBrightness
     * @param value
     */
    static void changeBrightness(qint32 value);
    /**
     * @brief changeGamma
     * @param value
     */
    static void changeGamma(float value);
    /**
     * @brief isGamepadSupported
     * @return
     */
    static bool isGamepadSupported();
    /**
     * @brief getLanguageNames
     * @return
     */
    static QStringList getLanguageNames();
    /**
     * @brief getLanguageIds
     * @return
     */
    static QStringList getLanguageIds();
    /**
     * @brief getCurrentLanguageIndex
     * @return
     */
    static qint32 getCurrentLanguageIndex();
private:
    friend class oxygine::intrusive_ptr<Settings>;
    explicit Settings();

private:
    static QVector<ValueBase*> m_SettingValues;

    // setting variables
    static qint32 m_x;
    static qint32 m_y;
    static qint32 m_width;
    static qint32 m_height;
    static float m_brightness;
    static float m_gamma;
    static bool m_smallScreenDevice;
    static bool m_touchScreen;
    static qint32 m_touchPointSensitivity;
    static bool m_gamepadEnabled;
    static float m_gamepadSensitivity;
    static qint32 m_framesPerSecond;
    static bool m_useHighDpi;

    static bool m_borderless;
    static bool m_fullscreen;

    static Qt::Key m_key_escape;
    static Qt::Key m_key_console;
    static Qt::Key m_key_screenshot;
    static Qt::Key m_key_mapshot;
    static Qt::Key m_key_up;
    static Qt::Key m_key_down;
    static Qt::Key m_key_right;
    static Qt::Key m_key_left;
    static Qt::Key m_key_confirm;
    static Qt::Key m_key_cancel;
    static Qt::Key m_key_next;
    static Qt::Key m_key_previous;
    static Qt::Key m_key_quicksave1;
    static Qt::Key m_key_quicksave2;
    static Qt::Key m_key_quickload1;
    static Qt::Key m_key_quickload2;
    static Qt::Key m_key_information;
    static Qt::Key m_key_moveMapUp;
    static Qt::Key m_key_moveMapDown;
    static Qt::Key m_key_moveMapRight;
    static Qt::Key m_key_moveMapLeft;
    static Qt::Key m_key_MapZoomOut;
    static Qt::Key m_key_MapZoomIn;
    static Qt::Key m_key_ShowAttackFields;
    static Qt::Key m_key_ShowIndirectAttackFields;
    static Qt::Key m_key_up2;
    static Qt::Key m_key_down2;
    static Qt::Key m_key_right2;
    static Qt::Key m_key_left2;
    static Qt::Key m_key_confirm2;
    static Qt::Key m_key_cancel2;
    static Qt::Key m_key_next2;
    static Qt::Key m_key_previous2;
    static Qt::Key m_key_information2;
    static Qt::Key m_key_moveMapUp2;
    static Qt::Key m_key_moveMapDown2;
    static Qt::Key m_key_moveMapRight2;
    static Qt::Key m_key_moveMapLeft2;
    static Qt::Key m_key_MapZoomOut2;
    static Qt::Key m_key_MapZoomIn2;
    static Qt::Key m_key_ShowAttackFields2;
    static Qt::Key m_key_ShowIndirectAttackFields2;

    static Qt::Key m_key_EditorPlaceTerrain;
    static Qt::Key m_key_EditorPlaceBuilding;
    static Qt::Key m_key_EditorPlaceUnit;
    static Qt::Key m_key_EditorNextTeam;
    static Qt::Key m_key_EditorPreviousTeam;
    static Qt::Key m_key_EditorSelectionUp;
    static Qt::Key m_key_EditorSelectionDown;
    static Qt::Key m_key_EditorSelectionRight;
    static Qt::Key m_key_EditorSelectionLeft;


    static QString m_language;
    // Sound
    static qint32 m_TotalVolume;
    static qint32 m_MusicVolume;
    static qint32 m_SoundVolume;
    static QVariant m_audioOutput;
    static bool m_muted;
    // Network
    static quint16 m_GamePort;
    static QString m_Username;
    static QString m_slaveServerName;
    static QString m_serverPassword;

    static bool m_Server;
    static quint16 m_ServerPort;
    static quint16 m_slaveServerPort;
    static QString m_ServerAdress;
    static QString m_secondaryServerAdress;
    static QString m_serverListenAdress;
    static QString m_serverSecondaryListenAdress;
    static QString m_slaveListenAdress;
    static QString m_slaveHostOptions;
    static std::chrono::seconds m_slaveDespawnTime;

    // mailing
    static QString m_mailServerAddress;
    static quint16 m_mailServerPort;
    static qint32 m_mailServerConnectionType;
    static QString m_mailServerUsername;
    static QString m_mailServerPassword;
    static QString m_mailServerSendAddress;
    static qint32 m_mailServerAuthMethod;

    // auto saving
    static std::chrono::seconds m_autoSavingCylceTime;
    static qint32 m_autoSavingCycle;


    // ingame options
    static bool m_overworldAnimations;
    static GameEnums::BattleAnimationMode m_battleAnimationsMode;
    static GameEnums::BattleAnimationType m_battleAnimationType;
    static quint32 m_animationSpeed;
    static quint32 m_walkAnimationSpeed;
    static quint32 m_battleAnimationSpeed;
    static quint32 m_dialogAnimationSpeed;
    static quint32 m_captureAnimationSpeed;
    static bool m_useCoMinis;
    static bool m_dialogAnimation;
    static bool m_captureAnimation;
    static bool m_day2dayScreen;
    static bool m_movementAnimations;
    static quint32 multiTurnCounter;
    static QString m_LastSaveGame;
    static QString m_defaultRuleset;
    static QString m_defaultBannlist;
    static bool m_ShowCursor;
    static bool m_AutoEndTurn;
    static qint32 m_MenuItemCount;
    static qint32 m_MenuItemRowCount;
    static bool m_StaticMarkedFields;
    static float m_mouseSensitivity;
    static bool m_record;
    static qint32 m_showCoCount;
    static bool m_showIngameCoordinates;
    static GameEnums::COInfoPosition m_coInfoPosition;
    static bool m_autoScrolling;
    static bool m_autoCamera;
    static GameEnums::AutoFocusing m_autoFocusing;
    static bool m_centerOnMarkedField;
    static bool m_syncAnimations;
    static bool m_simpleDeselect;
    static bool m_showDetailedBattleForcast;
    static bool m_autoMoveCursor;
    static float m_supplyWarning;
    static qint32 m_pauseAfterAction;

    // internal members
    static spSettings m_pInstance;
    static const QString m_settingFile;
    static QStringList m_activeMods;
    static QStringList m_activeModVersions;
    static QString m_userPath;
    static QTranslator m_Translator;

    // logging
    static bool m_LogActions;
    static Console::eLogLevels m_defaultLogLevel;
};

Q_DECLARE_INTERFACE(Settings, "Settings");

#endif // SETTINGS_H
