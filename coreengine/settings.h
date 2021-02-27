#ifndef SETTINGS_H
#define SETTINGS_H

#include <Qt>
#include <QObject>
#include <QTranslator>

#include "oxygine-framework.h"

#include "game/GameEnums.h"

class GLGraphicsView;

class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings* getInstance();

    static void loadSettings();
    static void saveSettings();

    static quint16 getServerPort();
    static void setServerPort(const quint16 &ServerPort);

    static QString getServerAdress();
    static void setServerAdress(const QString &ServerAdress);

    static void setServer(bool Server);

    static QString getLastSaveGame();
    static void setLastSaveGame(const QString &LastSaveGame);

    static void setUsername(const QString &Username);

    static QStringList getActiveModVersions();


    static QStringList getActiveMods();
    static void setActiveMods(const QStringList &activeMods);

    static QString getSlaveServerName();
    static void setSlaveServerName(const QString &slaveServerName);

public slots:
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

    static quint32 getSpriteFilter();
    static void setSpriteFilter(quint32 spriteFilter);

    static qint32 getShowCoCount();
    static void setShowCoCount(const qint32 &showCoCount);

    static std::chrono::seconds getAutoSavingCylceTime();
    static void setAutoSavingCylceTime(const std::chrono::seconds &value);

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

    static QString getModConfigString();
    static QString getModConfigString(QStringList mods);

    static quint32 getMultiTurnCounter();
    static void setMultiTurnCounter(const quint32 &value);

    static bool getShowCursor();
    static void setShowCursor(bool ShowCursor);

    static bool getAutoEndTurn();
    static void setAutoEndTurn(bool AutoEndTurn);

    static GameEnums::BattleAnimationMode getBattleAnimations();
    static void setBattleAnimations(const GameEnums::BattleAnimationMode &value);

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

    static GameEnums::AnimationMode getShowAnimations();
    static void setShowAnimations(GameEnums::AnimationMode value);

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
                            QStringList & requiredMods, bool & isCosmetic);

    static QString getModName(QString mod);
    /**
     * @brief getIsCosmetic
     * @param mod
     * @return
     */
    static bool getIsCosmetic(QString mod);
private:
    Settings();
    virtual ~Settings() = default;

    // setting variables
    static qint32 m_x;
    static qint32 m_y;
    static qint32 m_width;
    static qint32 m_height;
    static float m_brightness;
    static float m_gamma;

    static bool m_borderless;
    static bool m_fullscreen;

    static Qt::Key m_key_escape;
    static Qt::Key m_key_console;
    static Qt::Key m_key_screenshot;
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
    // Network
    static quint16 m_GamePort;
    static quint16 m_ServerPort;
    static QString m_ServerAdress;
    static QString m_Username;
    static QString m_slaveServerName;
    static bool m_Server;
    // auto saving
    static std::chrono::seconds autoSavingCylceTime;    static qint32 autoSavingCycle;


    // ingame options
    static GameEnums::AnimationMode showAnimations;
    static GameEnums::BattleAnimationMode battleAnimations;
    static quint32 animationSpeed;
    static quint32 walkAnimationSpeed;
    static quint32 battleAnimationSpeed;
    static quint32 dialogAnimationSpeed;
    static bool m_dialogAnimation;
    static quint32 multiTurnCounter;
    static QString m_LastSaveGame;
    static bool m_ShowCursor;
    static bool m_AutoEndTurn;
    static qint32 m_MenuItemCount;
    static bool m_StaticMarkedFields;
    static float m_mouseSensitivity;
    static bool m_record;
    static qint32 m_showCoCount;
    static quint32 m_spriteFilter;
    static bool m_showIngameCoordinates;
    static GameEnums::COInfoPosition coInfoPosition;
    static bool m_autoScrolling;
    static bool m_autoCamera;
    static GameEnums::AutoFocusing m_autoFocusing;
    static bool m_centerOnMarkedField;

    // internal members
    static Settings* m_pInstance;
    static const QString m_settingFile;
    static QStringList m_activeMods;
    static QStringList m_activeModVersions;

    static QTranslator m_Translator;

    // logging
    static bool m_LogActions;
};

#endif // SETTINGS_H
