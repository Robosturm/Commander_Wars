#include <QSettings>
#include <QTranslator>
#include <QLocale>
#ifdef GRAPHICSUPPORT
#include <QApplication>
#else
#include <QCoreApplication>
#endif
#include <QScreen>
#include <QLocale>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfoList>
#include <QInputDevice>
#include <QDirIterator>

#include "coreengine/settings.h"
#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"
#include "coreengine/userdata.h"
#include "coreengine/audiothread.h"
#include "coreengine/Gamepad.h"

#include "game/gamemap.h"

#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"

#ifdef USEAPPCONFIGPATH
    const QString Settings::m_settingFile = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/Commander_Wars.ini";
#else
    const QString Settings::m_settingFile = "Commander_Wars.ini";
#endif

const char* const Settings::DEFAULT_AUDIODEVICE = "@@default@@";

float Settings::m_mouseSensitivity   = -0.75f;
qint32 Settings::m_x                 = 0;
qint32 Settings::m_y                 = 0;
qint32 Settings::m_width             = 1024;
qint32 Settings::m_height            = 800;
bool Settings::m_borderless         = true;
bool Settings::m_fullscreen         = false;
float Settings::m_brightness        = 0.0f;
float Settings::m_gamma             = 1.0f;
bool Settings::m_smallScreenDevice  = false;
bool Settings::m_touchScreen = false;
bool Settings::m_gamepadEnabled = false;
float Settings::m_gamepadSensitivity = 1.0f;
bool Settings::m_useHighDpi = true;
qint32 Settings::m_framesPerSecond = 60;
qint32 Settings::m_touchPointSensitivity = 15;
Qt::Key Settings::m_key_escape                      = Qt::Key_Escape;
Qt::Key Settings::m_key_console                     = Qt::Key_F1;
Qt::Key Settings::m_key_screenshot                  = Qt::Key_F5;
Qt::Key Settings::m_key_mapshot                     = Qt::Key_F6;
Qt::Key Settings::m_key_up                          = Qt::Key_W;
Qt::Key Settings::m_key_down                        = Qt::Key_S;
Qt::Key Settings::m_key_right                       = Qt::Key_D;
Qt::Key Settings::m_key_left                        = Qt::Key_A;
Qt::Key Settings::m_key_confirm                     = Qt::Key_Space;
Qt::Key Settings::m_key_cancel                      = Qt::Key_B;
Qt::Key Settings::m_key_next                        = Qt::Key_E;
Qt::Key Settings::m_key_previous                    = Qt::Key_Q;
Qt::Key Settings::m_key_quicksave1                  = Qt::Key_F9;
Qt::Key Settings::m_key_quicksave2                  = Qt::Key_F11;
Qt::Key Settings::m_key_quickload1                  = Qt::Key_F10;
Qt::Key Settings::m_key_quickload2                  = Qt::Key_F12;
Qt::Key Settings::m_key_information                 = Qt::Key_I;
Qt::Key Settings::m_key_moveMapUp                   = Qt::Key_Up;
Qt::Key Settings::m_key_moveMapDown                 = Qt::Key_Down;
Qt::Key Settings::m_key_moveMapRight                = Qt::Key_Right;
Qt::Key Settings::m_key_moveMapLeft                 = Qt::Key_Left;
Qt::Key Settings::m_key_MapZoomOut                  = Qt::Key_Minus;
Qt::Key Settings::m_key_MapZoomIn                   = Qt::Key_Plus;
Qt::Key Settings::m_key_ShowAttackFields            = Qt::Key_2;
Qt::Key Settings::m_key_ShowIndirectAttackFields    = Qt::Key_1;
Qt::Key Settings::m_key_up2                         = Qt::Key(0);
Qt::Key Settings::m_key_down2                       = Qt::Key(0);
Qt::Key Settings::m_key_right2                      = Qt::Key(0);
Qt::Key Settings::m_key_left2                       = Qt::Key(0);
Qt::Key Settings::m_key_confirm2                    = Qt::Key_Return;
Qt::Key Settings::m_key_cancel2                     = Qt::Key_Backspace;
Qt::Key Settings::m_key_next2                       = Qt::Key_Tab;
Qt::Key Settings::m_key_previous2                   = Qt::Key(0);
Qt::Key Settings::m_key_information2                = Qt::Key(0);
Qt::Key Settings::m_key_moveMapUp2                  = Qt::Key(0);
Qt::Key Settings::m_key_moveMapDown2                = Qt::Key(0);
Qt::Key Settings::m_key_moveMapRight2               = Qt::Key(0);
Qt::Key Settings::m_key_moveMapLeft2                = Qt::Key(0);
Qt::Key Settings::m_key_MapZoomOut2                 = Qt::Key(0);
Qt::Key Settings::m_key_MapZoomIn2                  = Qt::Key(0);
Qt::Key Settings::m_key_ShowAttackFields2           = Qt::Key(0);
Qt::Key Settings::m_key_ShowIndirectAttackFields2   = Qt::Key(0);

Qt::Key Settings::m_key_EditorPlaceTerrain          = Qt::Key_1;
Qt::Key Settings::m_key_EditorPlaceBuilding         = Qt::Key_2;
Qt::Key Settings::m_key_EditorPlaceUnit             = Qt::Key_3;
Qt::Key Settings::m_key_EditorNextTeam              = Qt::Key_Tab;
Qt::Key Settings::m_key_EditorPreviousTeam          = Qt::Key_Asterisk;
Qt::Key Settings::m_key_EditorSelectionRight        = Qt::Key_T;
Qt::Key Settings::m_key_EditorSelectionLeft         = Qt::Key_R;

QString Settings::m_language      = "en";
// Sound
qint32 Settings::m_TotalVolume       = 100;
qint32 Settings::m_MusicVolume       = 80;
qint32 Settings::m_SoundVolume       = 100;
QVariant Settings::m_audioOutput = QVariant();
bool Settings::m_muted = false;
// Network
quint16 Settings::m_GamePort          = 9001;
quint16 Settings::m_ServerPort        = 9002;
quint16 Settings::m_slaveServerPort   = 9003;
QString Settings::m_ServerAdress      = "";
QString Settings::m_slaveServerName   = "";
QString Settings::m_slaveHostOptions      = "::1&10000&20000;::1&50000&65535";
QString Settings::m_serverListenAdress = "";
QString Settings::m_slaveListenAdress = "::1";
bool Settings::m_Server               = false;
bool Settings::m_record               = true;
std::chrono::seconds Settings::m_slaveDespawnTime = std::chrono::minutes(0);
QString Settings::m_serverPassword = "";

// mailing
QString Settings::m_mailServerAddress = "";
quint16 Settings::m_mailServerPort = 0;
qint32 Settings::m_mailServerConnectionType = 2;
QString Settings::m_mailServerUsername = "";
QString Settings::m_mailServerPassword = "";
qint32 Settings::m_mailServerAuthMethod = 1;
QString Settings::m_mailServerSendAddress = "";
// auto saving
std::chrono::seconds Settings::m_autoSavingCylceTime = std::chrono::minutes(0);
qint32 Settings::m_autoSavingCycle = 0;

// ingame options
bool Settings::m_overworldAnimations = true;
GameEnums::BattleAnimationMode Settings::m_battleAnimationsMode = GameEnums::BattleAnimationMode_All;
GameEnums::BattleAnimationType Settings::m_battleAnimationType = GameEnums::BattleAnimationType_Detail;
quint32 Settings::m_animationSpeed = 1;
quint32 Settings::m_battleAnimationSpeed = 1;
quint32 Settings::m_walkAnimationSpeed = 20;
quint32 Settings::m_dialogAnimationSpeed = 20;
quint32 Settings::m_captureAnimationSpeed = 1;
bool Settings::m_useCoMinis = true;
bool Settings::m_dialogAnimation = true;
bool Settings::m_captureAnimation = true;
quint32 Settings::multiTurnCounter = 4;
QString Settings::m_LastSaveGame = "";
QString Settings::m_defaultRuleset = "";
QString Settings::m_defaultBannlist = "";
QString Settings::m_Username = "";
bool Settings::m_ShowCursor = true;
bool Settings::m_AutoEndTurn = false;
qint32 Settings::m_MenuItemCount = 13;
qint32 Settings::m_MenuItemRowCount = 2;
bool Settings::m_StaticMarkedFields = false;
qint32 Settings::m_showCoCount = 0;
GameEnums::COInfoPosition Settings::m_coInfoPosition = GameEnums::COInfoPosition_Flipping;
bool Settings::m_autoScrolling = true;
bool Settings::m_autoCamera = true;
bool Settings::m_showIngameCoordinates  = true;
GameEnums::AutoFocusing Settings::m_autoFocusing = GameEnums::AutoFocusing_LastPos;
bool Settings::m_centerOnMarkedField = false;
bool Settings::m_syncAnimations = false;
bool Settings::m_simpleDeselect = false;
bool Settings::m_showDetailedBattleForcast = true;
bool Settings::m_autoMoveCursor = false;
QString Settings::m_userPath = "";
float Settings::m_supplyWarning = 0.33f;
qint32 Settings::m_pauseAfterAction = 0;
bool Settings::m_day2dayScreen = true;
bool Settings::m_movementAnimations = true;

// add mod path
QStringList Settings::m_activeMods;
QStringList Settings::m_activeModVersions;
// this Object
spSettings Settings::m_pInstance;
QTranslator Settings::m_Translator;

// logging
bool Settings::m_LogActions = false;
Console::eLogLevels Settings::m_defaultLogLevel = static_cast<Console::eLogLevels>(DEBUG_LEVEL);

QVector<Settings::ValueBase*> Settings::m_SettingValues;

Settings* Settings::getInstance()
{
    if (m_pInstance.get() == nullptr)
    {
        m_pInstance = spSettings::create();
    }
    return m_pInstance.get();
}

void Settings::shutdown()
{
    m_pInstance = nullptr;
}

Settings::Settings()
{
    setObjectName("Settings");
    Interpreter::setCppOwnerShip(this);
    QSize size;
#ifdef GRAPHICSUPPORT
    size = QApplication::primaryScreen()->availableSize() * QApplication::primaryScreen()->devicePixelRatio();
#endif
    bool smallScreenDevice = hasSmallScreen();
    QString defaultPath = "";
    qint32 defaultCoCount = 0;
    if (smallScreenDevice)
    {
        defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/commander_wars/";
        defaultCoCount = 1;
    }
#ifdef USEAPPCONFIGPATH
    defaultPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/";
#endif

    auto devices = QInputDevice::devices();
    bool hasTouch = false;
    for (const auto & device: qAsConst(devices))
    {
        if (device->type() == QInputDevice::DeviceType::TouchScreen)
        {
            hasTouch = true;
            break;
        }
    }

    m_SettingValues =
    {
        // resolution
        new Value<qint32>{"Resolution", "x", &m_x, 0, std::numeric_limits<qint32>::min(), std::numeric_limits<qint32>::max(), true},
        new Value<qint32>{"Resolution", "y", &m_y, 0, std::numeric_limits<qint32>::min(), std::numeric_limits<qint32>::max(), true},
        new Value<qint32>{"Resolution", "width", &m_width, size.width(), 1, size.width(), true},
        new Value<qint32>{"Resolution", "height", &m_height, size.height(), 1, size.height(), true},
        new Value<float>{"Resolution", "brightness", &m_brightness, 0, -50, 50},
        new Value<float>{"Resolution", "gamma", &m_gamma, 1, 0, 50},
        new Value<bool>{"Resolution", "borderless", &m_borderless, true, false, true, true},
        new Value<bool>{"Resolution", "fullscreen", &m_fullscreen, false, false, true, true},
        new Value<bool>{"Resolution", "recordgames", &m_record, false, false, true},
        new Value<bool>{"Resolution", "SmallScreenDevice", &m_smallScreenDevice, smallScreenDevice, false, true},
        new Value<bool>{"Resolution", "UseHighDpi", &m_useHighDpi, true, false, true},

        // general
        new Value<QString>{"General", "language", &m_language, "en", "", ""},
        new Value<float>{"General", "MouseSensitivity", &m_mouseSensitivity, -0.75f, -100, 100},
        new Value<QString>{"General", "UserPath", &m_userPath, defaultPath, "", ""},
        new Value<bool>{"General", "TouchScreen", &m_touchScreen, hasTouch, false, true},
        new Value<qint32>{"General", "TouchPointSensitivity", &m_touchPointSensitivity, 15, 0, size.width()},
        new Value<bool>{"General", "GamepadEnabled", &m_gamepadEnabled, false, false, true},
        new Value<float>{"General", "GamepadSensitivity", &m_gamepadSensitivity, 1.0f, 0.1f, 100},
        new Value<qint32>{"General", "MaxFPS", &m_framesPerSecond, 60, 30, 60},
        // keys
        new Value<Qt::Key>{"Keys", "key_escape", &m_key_escape, Qt::Key_Escape, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_console", &m_key_console, Qt::Key_F1, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_screenshot", &m_key_screenshot, Qt::Key_F5, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_mapshot", &m_key_mapshot, Qt::Key_F6, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_up", &m_key_up, Qt::Key_W, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_down", &m_key_down, Qt::Key_S, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_right", &m_key_right, Qt::Key_D, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_left", &m_key_left, Qt::Key_A, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_confirm", &m_key_confirm, Qt::Key_Space, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_cancel", &m_key_cancel, Qt::Key_B, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_next", &m_key_next, Qt::Key_E, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_previous", &m_key_previous, Qt::Key_Q, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_quicksave1", &m_key_quicksave1, Qt::Key_F9, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_quicksave2", &m_key_quicksave2, Qt::Key_F11, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_quickload1", &m_key_quickload1, Qt::Key_F10, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_quickload2", &m_key_quickload2, Qt::Key_F12, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_information", &m_key_information, Qt::Key_I, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_moveMapUp", &m_key_moveMapUp, Qt::Key_Up, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_moveMapDown", &m_key_moveMapDown, Qt::Key_Down, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_moveMapRight", &m_key_moveMapRight, Qt::Key_Right, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_moveMapLeft", &m_key_moveMapLeft, Qt::Key_Left, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_MapZoomOut", &m_key_MapZoomOut, Qt::Key_Minus, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_MapZoomIn", &m_key_MapZoomIn, Qt::Key_Plus, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_ShowAttackFields", &m_key_ShowAttackFields, Qt::Key_2, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_ShowIndirectAttackFields", &m_key_ShowIndirectAttackFields, Qt::Key_1, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_up2", &m_key_up2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_down2", &m_key_down2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_right2", &m_key_right2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_left2", &m_key_left2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_confirm2", &m_key_confirm2, Qt::Key_Return, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_cancel2", &m_key_cancel2, Qt::Key_Backspace, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_next2", &m_key_next2, Qt::Key_Tab, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_previous2", &m_key_previous2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_information2", &m_key_information2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_moveMapUp2", &m_key_moveMapUp2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_moveMapDown2", &m_key_moveMapDown2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_moveMapRight2", &m_key_moveMapRight2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_moveMapLeft2", &m_key_moveMapLeft2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_MapZoomOut2", &m_key_MapZoomOut2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_MapZoomIn2", &m_key_MapZoomIn2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_ShowAttackFields2", &m_key_ShowAttackFields2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_ShowIndirectAttackFields2", &m_key_ShowIndirectAttackFields2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_EditorPlaceTerrain", &m_key_EditorPlaceTerrain, Qt::Key_1, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_EditorPlaceBuilding", &m_key_EditorPlaceBuilding, Qt::Key_2, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_EditorPlaceUnit", &m_key_EditorPlaceUnit, Qt::Key_3, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_EditorNextTeam", &m_key_EditorNextTeam, Qt::Key_Tab, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_EditorPreviousTeam", &m_key_EditorPreviousTeam, Qt::Key_Asterisk, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_EditorSelectionRight", &m_key_EditorSelectionRight, Qt::Key_R, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_EditorSelectionLeft", &m_key_EditorSelectionLeft, Qt::Key_T, static_cast<Qt::Key>(0), Qt::Key_unknown},
        // sound
        new Value<qint32>{"Sound", "TotalVolume", &m_TotalVolume, 100, 0, 100},
        new Value<qint32>{"Sound", "MusicVolume", &m_MusicVolume, 80, 0, 100},
        new Value<qint32>{"Sound", "SoundVolume", &m_SoundVolume, 100, 0, 100},
        new Value<bool>{"Sound", "Muted", &m_muted, false, false, true},
#ifdef AUDIOSUPPORT
        new AudioDeviceValue{"Sound", "AudioDevice", &m_audioOutput, DEFAULT_AUDIODEVICE},
#endif
        // game
        new Value<QString>{"Game", "Username", &m_Username, "", "", "", true},
        new Value<bool>{"Game", "OverworldAnimations", &m_overworldAnimations, true, false, true},
        new Value<GameEnums::BattleAnimationMode>{"Game", "BattleAnimationMode", &m_battleAnimationsMode, GameEnums::BattleAnimationMode_All, GameEnums::BattleAnimationMode_None, GameEnums::BattleAnimationMode_Enemy},
        new Value<GameEnums::BattleAnimationType>{"Game", "BattleAnimationType", &m_battleAnimationType, GameEnums::BattleAnimationType_Detail, GameEnums::BattleAnimationType_Detail, GameEnums::BattleAnimationType_Overworld},
        new Value<quint32>{"Game", "AnimationSpeed", &m_animationSpeed, 1, 1, 100},
        new Value<quint32>{"Game", "BattleAnimationSpeed", &m_battleAnimationSpeed, 1, 1, 100},
        new Value<quint32>{"Game", "WalkAnimationSpeed", &m_walkAnimationSpeed, 20, 1, 100},
        new Value<quint32>{"Game", "DialogAnimationSpeed", &m_dialogAnimationSpeed, 1, 1, 100},
        new Value<quint32>{"Game", "CaptureAnimationSpeed", &m_captureAnimationSpeed, 1, 1, 100},
        new Value<quint32>{"Game", "MultiTurnCounter", &multiTurnCounter, 4, 1, 10},
        new Value<qint32>{"Game", "MenuItemCount", &m_MenuItemCount, 13, 5, std::numeric_limits<qint32>::max()},
        new Value<qint32>{"Game", "MenuItemRowCount", &m_MenuItemRowCount, 2, 1, std::numeric_limits<qint32>::max()},
        new Value<float>{"Game", "SupplyWarning", &m_supplyWarning, 0.33f, 0.0f, 1.0f},
        new Value<bool>{"Game", "StaticMarkedFields", &m_StaticMarkedFields, false, false, true},
        new Value<qint32>{"Game", "ShowCoCount", &m_showCoCount, defaultCoCount, 0, std::numeric_limits<qint32>::max()},
        new Value<bool>{"Game", "DialogAnimation", &m_dialogAnimation, true, false, true},
        new Value<bool>{"Game", "CaptureAnimation", &m_captureAnimation, true, false, true},
        new Value<bool>{"Game", "Day2DayScreen", &m_day2dayScreen, true, false, true},
        new Value<bool>{"Game", "MovementAnimations", &m_movementAnimations, true, false, true},
        new Value<QString>{"Game", "LastSaveGame", &m_LastSaveGame, "", "", ""},
        new Value<QString>{"Game", "DefaultRuleset", &m_defaultRuleset, "", "", ""},
        new Value<QString>{"Game", "DefaultBannlist", &m_defaultBannlist, "", "", ""},
        new Value<bool>{"Game", "ShowCursor", &m_ShowCursor, true, false, true},
        new Value<bool>{"Game", "AutoEndTurn", &m_AutoEndTurn, false, false, true},
        new Value<bool>{"Game", "AutoCamera", &m_autoCamera, true, false, true},
        new Value<bool>{"Game", "ShowIngameCoordinates", &m_showIngameCoordinates, true, false, true},
        new Value<bool>{"Game", "CenterOnSelection", &m_centerOnMarkedField, true, false, true},
        new Value<bool>{"Game", "SyncAnimations", &m_syncAnimations, false, false, true},
        new Value<bool>{"Game", "AutoMoveCursor", &m_autoMoveCursor, true, false, true},
        new Value<bool>{"Game", "UseCoMinis", &m_useCoMinis, true, false, true},
        new Value<bool>{"Game", "AutoScrolling", &m_autoScrolling, true, false, true},
        new Value<bool>{"Game", "ShowDetailedBattleForcast", &m_showDetailedBattleForcast, true, false, true},
        new Value<bool>{"Game", "SimpleDeselect", &m_simpleDeselect, false, false, true},
        new Value<GameEnums::COInfoPosition>{"Game", "COInfoPosition", &m_coInfoPosition, GameEnums::COInfoPosition_Flipping, GameEnums::COInfoPosition_Flipping, GameEnums::COInfoPosition_Right},
        new Value<GameEnums::AutoFocusing>{"Game", "AutoFocusing", &m_autoFocusing, GameEnums::AutoFocusing_LastPos, GameEnums::AutoFocusing_LastPos, GameEnums::AutoFocusing_Owned},
        new Value<qint32>{"Game", "PauseAfterAction", &m_pauseAfterAction, 0, 0, 100},

        // network
        new Value<quint16>{"Network", "GamePort", &m_GamePort, 9001, 0, std::numeric_limits<quint16>::max()},
        new Value<QString>{"Network", "ServerAdress", &m_ServerAdress, "", "", ""},
        new Value<quint16>{"Network", "SlaveServerPort", &m_slaveServerPort, 9003, 0, std::numeric_limits<quint16>::max()},
        new Value<quint16>{"Network", "ServerPort", &m_ServerPort, 9002, 0, std::numeric_limits<quint16>::max()},
        new Value<bool>{"Network", "Server", &m_Server, false, false, true},
        new Value<QString>{"Network", "ServerListenAdress", &m_serverListenAdress, "", "", ""},
        new Value<QString>{"Network", "SlaveListenAdress", &m_slaveListenAdress, "", "", ""},
        new Value<QString>{"Network", "SlaveHostOptions", &m_slaveHostOptions, "::1&10000&20000;::1&50000&65535", "", ""},
        new Value<std::chrono::seconds>{"Network", "SlaveDespawnTime", &m_slaveDespawnTime, std::chrono::seconds(60 * 60 * 24), std::chrono::seconds(60), std::chrono::seconds(60 * 60 * 24 * 24)},
        // mailing
        new Value<QString>{"Mailing", "MailServerAddress", &m_mailServerAddress, "", "", ""},
        new Value<quint16>{"Mailing", "MailServerPort", &m_mailServerPort, 0, 0, std::numeric_limits<quint16>::max()},
        new Value<qint32>{"Mailing", "MailServerConnectionType", &m_mailServerConnectionType, 2, 0, 2},
        new Value<QString>{"Mailing", "MailServerUsername", &m_mailServerUsername, "", "", ""},
        new Value<qint32>{"Mailing", "MailServerAuthMethod", &m_mailServerAuthMethod, 1, 0, 1},
        new Value<QString>{"Mailing", "MailServerSendAddress", &m_mailServerSendAddress, "", "", ""},

        // auto saving
        new Value<std::chrono::seconds>{"Autosaving", "AutoSavingTime", &m_autoSavingCylceTime, std::chrono::seconds(60 * 5), std::chrono::seconds(0), std::chrono::seconds(60 * 60 * 24)},
        new Value<qint32>{"Autosaving", "AutoSavingCycle", &m_autoSavingCycle, 3, 0, 100},
        // mods
        new Value<QStringList>{"Mods", "Mods", &m_activeMods, QStringList(), QStringList(), QStringList()},
        // logging
        new Value<bool>{"Logging", "LogActions", &m_LogActions, false, false, true},
        new Value<Console::eLogLevels>{"Logging", "LogLevel", &m_defaultLogLevel, static_cast<Console::eLogLevels>(DEBUG_LEVEL), Console::eLogLevels::eOFF, Console::eLogLevels::eFATAL},
    };
}

const QString &Settings::getServerPassword()
{
    return m_serverPassword;
}

void Settings::setServerPassword(const QString &newServerPassword)
{
    CONSOLE_PRINT("Changed buffered server login password", Console::eDEBUG);
    m_serverPassword = newServerPassword;
}

const QString &Settings::getMailServerSendAddress()
{
    return m_mailServerSendAddress;
}

void Settings::setMailServerSendAddress(const QString &newMailServerSendAddress)
{
    m_mailServerSendAddress = newMailServerSendAddress;
}

qint32 Settings::getMailServerAuthMethod()
{
    return m_mailServerAuthMethod;
}

void Settings::setMailServerAuthMethod(qint32 newMailServerAuthMethod)
{
    m_mailServerAuthMethod = newMailServerAuthMethod;
}

QString Settings::getMailServerPassword()
{
    return m_mailServerPassword;
}

void Settings::setMailServerPassword(QString newMailServerPassword)
{
    m_mailServerPassword = newMailServerPassword;
}

QString Settings::getMailServerUsername()
{
    return m_mailServerUsername;
}

void Settings::setMailServerUsername(QString newMailServerUsername)
{
    m_mailServerUsername = newMailServerUsername;
}

qint32 Settings::getMailServerConnectionType()
{
    return m_mailServerConnectionType;
}

void Settings::setMailServerConnectionType(qint32 newMailServerConnectionType)
{
    m_mailServerConnectionType = newMailServerConnectionType;
}

quint16 Settings::getMailServerPort()
{
    return m_mailServerPort;
}

void Settings::setMailServerPort(quint16 newMailServerPort)
{
    m_mailServerPort = newMailServerPort;
}

const QString &Settings::getMailServerAddress()
{
    return m_mailServerAddress;
}

void Settings::setMailServerAddress(const QString &newMailServerAddress)
{
    m_mailServerAddress = newMailServerAddress;
}

const std::chrono::seconds &Settings::getSlaveDespawnTime()
{
    return m_slaveDespawnTime;
}

void Settings::setSlaveDespawnTime(const std::chrono::seconds &newSlaveDespawnTime)
{
    m_slaveDespawnTime = newSlaveDespawnTime;
}

QString Settings::getDefaultBannlist()
{
    return m_defaultBannlist;
}

void Settings::setDefaultBannlist(const QString &newDefaultBannlist)
{
    m_defaultBannlist = newDefaultBannlist;
}

bool Settings::getUseHighDpi()
{
    return m_useHighDpi;
}

void Settings::setUseHighDpi(bool newUseHighDpi)
{
    m_useHighDpi = newUseHighDpi;
}

bool Settings::getMovementAnimations()
{
    return m_movementAnimations;
}

void Settings::setMovementAnimations(bool newMovementAnimations)
{
    m_movementAnimations = newMovementAnimations;
}

bool Settings::getDay2dayScreen()
{
    return m_day2dayScreen;
}

void Settings::setDay2dayScreen(bool newDay2dayScreen)
{
    m_day2dayScreen = newDay2dayScreen;
}

bool Settings::getCaptureAnimation()
{
    return m_captureAnimation;
}

void Settings::setCaptureAnimation(bool newCaptureAnimation)
{
    m_captureAnimation = newCaptureAnimation;
}

const QString &Settings::getSlaveListenAdress()
{
    return m_slaveListenAdress;
}

void Settings::setSlaveListenAdress(const QString &newSlaveListenAdress)
{
    m_slaveListenAdress = newSlaveListenAdress;
}

const QString &Settings::getServerListenAdress()
{
    return m_serverListenAdress;
}

void Settings::setServerListenAdress(const QString &newServerListenAdress)
{
    m_serverListenAdress = newServerListenAdress;
}

qint32 Settings::getPauseAfterAction()
{
    return m_pauseAfterAction;
}

void Settings::setPauseAfterAction(qint32 newPauseAfterAction)
{
    m_pauseAfterAction = newPauseAfterAction;
}

QString Settings::getSlaveHostOptions()
{
    return m_slaveHostOptions;
}

void Settings::setSlaveHostOptions(const QString &newSlaveHostOptions)
{
    m_slaveHostOptions = newSlaveHostOptions;
}

quint16 Settings::getSlaveServerPort()
{
    return m_slaveServerPort;
}

void Settings::setSlaveServerPort(quint16 newSlaveServerPort)
{
    m_slaveServerPort = newSlaveServerPort;
}

void Settings::setKey_mapshot(Qt::Key newKey_mapshot)
{
    m_key_mapshot = newKey_mapshot;
}

Qt::Key Settings::getKey_mapshot()
{
    return m_key_mapshot;
}

qint32 Settings::getFramesPerSecond()
{
    return m_framesPerSecond;
}

void Settings::setFramesPerSecond(qint32 newFramesPerSecond)
{
    constexpr qint32 msPerSec = 1000;
    m_framesPerSecond = newFramesPerSecond;
    if (m_framesPerSecond >= msPerSec)
    {
        m_framesPerSecond = msPerSec;
    }
    else if (m_framesPerSecond < 30)
    {
        m_framesPerSecond = 30;
    }
    Mainapp* pApp = Mainapp::getInstance();
    if (pApp != nullptr)
    {
        pApp->setTimerCycle(msPerSec / m_framesPerSecond);
    }
}

bool Settings::getMuted()
{
    return m_muted;
}

void Settings::setMuted(bool newMuted)
{
    m_muted = newMuted;
}

float Settings::getSupplyWarning()
{
    return m_supplyWarning;
}

void Settings::setSupplyWarning(float newSupplyWarning)
{
    m_supplyWarning = newSupplyWarning;
}

QString Settings::getDefaultRuleset()
{
    return m_defaultRuleset;
}

void Settings::setDefaultRuleset(const QString &newDefaultRuleset)
{
    m_defaultRuleset = newDefaultRuleset;
}

float Settings::getGamepadSensitivity()
{
    return m_gamepadSensitivity;
}

void Settings::setGamepadSensitivity(float newGamepadSensitivity)
{
    m_gamepadSensitivity = newGamepadSensitivity;
}

bool Settings::getGamepadEnabled()
{
    return m_gamepadEnabled;
}

void Settings::setGamepadEnabled(bool newGamepadEnabled)
{
    m_gamepadEnabled = newGamepadEnabled;
    if (m_gamepadEnabled)
    {
        Mainapp::getInstance()->getGamepad().init();
    }
}

bool Settings::getUseCoMinis()
{
    return m_useCoMinis;
}

void Settings::setUseCoMinis(bool newUseCoMinis)
{
    m_useCoMinis = newUseCoMinis;
}

bool Settings::getOverworldAnimations()
{
    return m_overworldAnimations;
}

void Settings::setOverworldAnimations(bool newOverworldAnimations)
{
    m_overworldAnimations = newOverworldAnimations;
}

qint32 Settings::getTouchPointSensitivity()
{
    return m_touchPointSensitivity;
}

void Settings::setTouchPointSensitivity(qint32 newTouchPointSensitivity)
{
    m_touchPointSensitivity = newTouchPointSensitivity;
}

const QVariant &Settings::getAudioOutput()
{
    return m_audioOutput;
}

void Settings::setAudioOutput(const QVariant &newAudioOutput)
{
    m_audioOutput = newAudioOutput;
}

bool Settings::getAutoMoveCursor()
{
    return m_autoMoveCursor;
}

void Settings::setAutoMoveCursor(bool newAutoMoveCursor)
{
    m_autoMoveCursor = newAutoMoveCursor;
}

bool Settings::getShowDetailedBattleForcast()
{
    return m_showDetailedBattleForcast;
}

void Settings::setShowDetailedBattleForcast(bool newShowDetailedBattleForcast)
{
    m_showDetailedBattleForcast = newShowDetailedBattleForcast;
}

bool Settings::getTouchScreen()
{
    return m_touchScreen;
}

void Settings::setTouchScreen(bool newTouchScreen)
{
    m_touchScreen = newTouchScreen;
}

QString Settings::getUserPath()
{
    return m_userPath;
}

void Settings::setUserPath(const QString &newUserPath)
{
    m_userPath = newUserPath;
}

bool Settings::getSmallScreenDevice()
{
    return m_smallScreenDevice;
}

void Settings::setSmallScreenDevice(bool newSmallScreenDevice)
{
    m_smallScreenDevice = newSmallScreenDevice;
}

qint32 Settings::getMenuItemRowCount()
{
    return m_MenuItemRowCount;
}

void Settings::setMenuItemRowCount(qint32 newMenuItemRowCount)
{
    m_MenuItemRowCount = newMenuItemRowCount;
}

bool Settings::getSimpleDeselect()
{
    return m_simpleDeselect;
}

void Settings::setSimpleDeselect(bool newSimpleDeselect)
{
    m_simpleDeselect = newSimpleDeselect;
}

bool Settings::getSyncAnimations()
{
    return m_syncAnimations;
}

void Settings::setSyncAnimations(bool syncAnimations)
{
    m_syncAnimations = syncAnimations;
}

bool Settings::getCenterOnMarkedField()
{
    return m_centerOnMarkedField;
}

void Settings::setCenterOnMarkedField(bool centerOnMarkedField)
{
    m_centerOnMarkedField = centerOnMarkedField;
}

bool Settings::getDialogAnimation()
{
    return m_dialogAnimation;
}

void Settings::setDialogAnimation(bool dialogAnimation)
{
    m_dialogAnimation = dialogAnimation;
}

float Settings::getGamma()
{
    return m_gamma;
}

void Settings::setGamma(float gamma)
{
    m_gamma = gamma;
}

float Settings::getBrightness()
{
    return m_brightness;
}

void Settings::setBrightness(float brightness)
{
    m_brightness = brightness;
}

Qt::Key Settings::getKey_screenshot()
{
    return m_key_screenshot;
}

void Settings::setKey_screenshot(const Qt::Key &key_screenshot)
{
    m_key_screenshot = key_screenshot;
}

GameEnums::AutoFocusing Settings::getAutoFocusing()
{
    return m_autoFocusing;
}

void Settings::setAutoFocusing(const GameEnums::AutoFocusing &autoFocusing)
{
    m_autoFocusing = autoFocusing;
}

Qt::Key Settings::getKey_EditorSelectionLeft()
{
    return m_key_EditorSelectionLeft;
}

void Settings::setKey_EditorSelectionLeft(const Qt::Key &key_EditorSelectionLeft)
{
    m_key_EditorSelectionLeft = key_EditorSelectionLeft;
}

Qt::Key Settings::getKey_EditorSelectionRight()
{
    return m_key_EditorSelectionRight;
}

void Settings::setKey_EditorSelectionRight(const Qt::Key &key_EditorSelectionRight)
{
    m_key_EditorSelectionRight = key_EditorSelectionRight;
}

Qt::Key Settings::getKey_EditorPreviousTeam()
{
    return m_key_EditorPreviousTeam;
}

void Settings::setKey_EditorPreviousTeam(const Qt::Key &key_EditorPreviousTeam)
{
    m_key_EditorPreviousTeam = key_EditorPreviousTeam;
}

Qt::Key Settings::getKey_EditorNextTeam()
{
    return m_key_EditorNextTeam;
}

void Settings::setKey_EditorNextTeam(const Qt::Key &key_EditorNextTeam)
{
    m_key_EditorNextTeam = key_EditorNextTeam;
}

Qt::Key Settings::getKey_EditorPlaceUnit()
{
    return m_key_EditorPlaceUnit;
}

void Settings::setKey_EditorPlaceUnit(const Qt::Key &key_EditorPlaceUnit)
{
    m_key_EditorPlaceUnit = key_EditorPlaceUnit;
}

Qt::Key Settings::getKey_EditorPlaceBuilding()
{
    return m_key_EditorPlaceBuilding;
}

void Settings::setKey_EditorPlaceBuilding(const Qt::Key &key_EditorPlaceBuilding)
{
    m_key_EditorPlaceBuilding = key_EditorPlaceBuilding;
}

Qt::Key Settings::getKey_EditorPlaceTerrain()
{
    return m_key_EditorPlaceTerrain;
}

void Settings::setKey_EditorPlaceTerrain(const Qt::Key &key_EditorPlaceTerrain)
{
    m_key_EditorPlaceTerrain = key_EditorPlaceTerrain;
}

Qt::Key Settings::getKey_ShowIndirectAttackFields2()
{
    return m_key_ShowIndirectAttackFields2;
}

void Settings::setKey_ShowIndirectAttackFields2(const Qt::Key &key_ShowIndirectAttackFields2)
{
    m_key_ShowIndirectAttackFields2 = key_ShowIndirectAttackFields2;
}

Qt::Key Settings::getKey_ShowAttackFields2()
{
    return m_key_ShowAttackFields2;
}

void Settings::setKey_ShowAttackFields2(const Qt::Key &key_ShowAttackFields2)
{
    m_key_ShowAttackFields2 = key_ShowAttackFields2;
}

Qt::Key Settings::getKey_MapZoomIn2()
{
    return m_key_MapZoomIn2;
}

void Settings::setKey_MapZoomIn2(const Qt::Key &key_MapZoomIn2)
{
    m_key_MapZoomIn2 = key_MapZoomIn2;
}

Qt::Key Settings::getKey_MapZoomOut2()
{
    return m_key_MapZoomOut2;
}

void Settings::setKey_MapZoomOut2(const Qt::Key &key_MapZoomOut2)
{
    m_key_MapZoomOut2 = key_MapZoomOut2;
}

Qt::Key Settings::getKey_moveMapLeft2()
{
    return m_key_moveMapLeft2;
}

void Settings::setKey_moveMapLeft2(const Qt::Key &key_moveMapLeft2)
{
    m_key_moveMapLeft2 = key_moveMapLeft2;
}

Qt::Key Settings::getKey_moveMapRight2()
{
    return m_key_moveMapRight2;
}

void Settings::setKey_moveMapRight2(const Qt::Key &key_moveMapRight2)
{
    m_key_moveMapRight2 = key_moveMapRight2;
}

Qt::Key Settings::getKey_moveMapDown2()
{
    return m_key_moveMapDown2;
}

void Settings::setKey_moveMapDown2(const Qt::Key &key_moveMapDown2)
{
    m_key_moveMapDown2 = key_moveMapDown2;
}

Qt::Key Settings::getKey_moveMapUp2()
{
    return m_key_moveMapUp2;
}

void Settings::setKey_moveMapUp2(const Qt::Key &key_moveMapUp2)
{
    m_key_moveMapUp2 = key_moveMapUp2;
}

Qt::Key Settings::getKey_information2()
{
    return m_key_information2;
}

void Settings::setKey_information2(const Qt::Key &key_information2)
{
    m_key_information2 = key_information2;
}

Qt::Key Settings::getKey_previous2()
{
    return m_key_previous2;
}

void Settings::setKey_previous2(const Qt::Key &key_previous2)
{
    m_key_previous2 = key_previous2;
}

Qt::Key Settings::getKey_next2()
{
    return m_key_next2;
}

void Settings::setKey_next2(const Qt::Key &key_next2)
{
    m_key_next2 = key_next2;
}

Qt::Key Settings::getKey_cancel2()
{
    return m_key_cancel2;
}

void Settings::setKey_cancel2(const Qt::Key &key_cancel2)
{
    m_key_cancel2 = key_cancel2;
}

Qt::Key Settings::getKey_confirm2()
{
    return m_key_confirm2;
}

void Settings::setKey_confirm2(const Qt::Key &key_confirm2)
{
    m_key_confirm2 = key_confirm2;
}

Qt::Key Settings::getKey_left2()
{
    return m_key_left2;
}

void Settings::setKey_left2(const Qt::Key &key_left2)
{
    m_key_left2 = key_left2;
}

Qt::Key Settings::getKey_right2()
{
    return m_key_right2;
}

void Settings::setKey_right2(const Qt::Key &key_right2)
{
    m_key_right2 = key_right2;
}

Qt::Key Settings::getKey_down2()
{
    return m_key_down2;
}

void Settings::setKey_down2(const Qt::Key &key_down2)
{
    m_key_down2 = key_down2;
}

Qt::Key Settings::getKey_up2()
{
    return m_key_up2;
}

void Settings::setKey_up2(const Qt::Key &key_up2)
{
    m_key_up2 = key_up2;
}

bool Settings::getAutoCamera()
{
    return m_autoCamera;
}

void Settings::setAutoCamera(bool autoCamera)
{
    m_autoCamera = autoCamera;
}

bool Settings::getAutoScrolling()
{
    return m_autoScrolling;
}

void Settings::setAutoScrolling(bool autoScrolling)
{
    m_autoScrolling = autoScrolling;
}

GameEnums::COInfoPosition Settings::getCoInfoPosition()
{
    return m_coInfoPosition;
}

void Settings::setCoInfoPosition(const GameEnums::COInfoPosition &value)
{
    m_coInfoPosition = value;
}

void Settings::setY(const qint32 &y)
{
    m_y = y;
}

void Settings::setX(const qint32 &x)
{
    m_x = x;
}

void Settings::setSlaveServerName(const QString &slaveServerName)
{
    m_slaveServerName = slaveServerName;
}

QString Settings::getSlaveServerName()
{
    return m_slaveServerName;
}

QStringList Settings::getActiveMods()
{
    return m_activeMods;
}

void Settings::setActiveMods(const QStringList &activeMods)
{
    m_activeMods = activeMods;
    qint32 i = 0;
    while (i < m_activeMods.size())
    {
        QDir dir(getUserPath() + m_activeMods[i]);
        QDir dir2( oxygine::Resource::RCC_PREFIX_PATH + m_activeMods[i]);
        if (!dir.exists() && !dir2.exists())
        {
            CONSOLE_PRINT("Removing mod from active list: " + m_activeMods[i] + " cause it wasn't found.", Console::eWARNING);
            m_activeMods.removeAt(i);
        }
        else
        {
            ++i;
        }
    }
    m_activeMods.sort();
    for (const auto & mod : qAsConst(m_activeMods))
    {
        CONSOLE_PRINT("Loaded mod: " + mod, Console::eDEBUG);
        bool found = false;
        QFile file(mod + "/mod.txt");
        if (file.exists())
        {
            file.open(QFile::ReadOnly);
            QTextStream stream(&file);
            while (!stream.atEnd())
            {
                QString line = stream.readLine();
                if (line.startsWith("version="))
                {
                    m_activeModVersions.append(line.split("=")[1]);
                    found = true;
                    break;
                }
            }
        }
        if (!found)
        {
            m_activeModVersions.append("1.0.0");
        }
    }
}

bool Settings::getShowIngameCoordinates()
{
    return m_showIngameCoordinates;
}

void Settings::setShowIngameCoordinates(bool showIngameCoordinates)
{
    m_showIngameCoordinates = showIngameCoordinates;
}

quint32 Settings::getWalkAnimationSpeedValue()
{
    return m_walkAnimationSpeed;
}

void Settings::setWalkAnimationSpeed(const quint32 &value)
{
    m_walkAnimationSpeed = value;
}


float Settings::getWalkAnimationSpeed()
{
    if (m_walkAnimationSpeed <= 100)
    {
        return 100.0f / (101.0f - static_cast<float>(m_walkAnimationSpeed));
    }
    return 100;
}

qint32 Settings::getShowCoCount()
{
    return m_showCoCount;
}

void Settings::setShowCoCount(const qint32 &showCoCount)
{
    m_showCoCount = showCoCount;
}

qint32 Settings::getAutoSavingCycle()
{
    return m_autoSavingCycle;
}

void Settings::setAutoSavingCycle(const qint32 &value)
{
    m_autoSavingCycle = value;
}

std::chrono::seconds Settings::getAutoSavingCylceTime()
{
    return m_autoSavingCylceTime;
}

void Settings::setAutoSavingCylceTime(const std::chrono::seconds &value)
{
    m_autoSavingCylceTime = value;
}

quint64 Settings::getAutoSavingCylceTimeRaw()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(getAutoSavingCylceTime()).count();
}

void Settings::setAutoSavingCylceTimeRaw(quint64 &value)
{
    setAutoSavingCylceTime(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(value)));
}

Qt::Key Settings::getKey_ShowAttackFields()
{
    return m_key_ShowAttackFields;
}

void Settings::setKey_ShowAttackFields(const Qt::Key &key_ShowAttackFields)
{
    m_key_ShowAttackFields = key_ShowAttackFields;
}

Qt::Key Settings::getKey_ShowIndirectAttackFields()
{
    return m_key_ShowIndirectAttackFields;
}

void Settings::setKey_ShowIndirectAttackFields(const Qt::Key &key_ShowIndirectAttackFields)
{
    m_key_ShowIndirectAttackFields = key_ShowIndirectAttackFields;
}

bool Settings::getLogActions()
{
    return m_LogActions;
}

void Settings::setLogActions(bool LogActions)
{
    m_LogActions = LogActions;
}

QString Settings::getLanguage()
{
    return m_language;
}

void Settings::setLanguage(const QString &language)
{
    QCoreApplication::removeTranslator(&m_Translator);
    m_language = language;
    QString languageFile = "resources/translation/lang_" + m_language + ".qm";
    if (!QFile::exists(languageFile))
    {
        languageFile = oxygine::Resource::RCC_PREFIX_PATH + languageFile;
    }
    // load language file and install it
    if (m_Translator.load(QLocale(m_language), languageFile))
    {
         CONSOLE_PRINT("Loaded language " + language, Console::eDEBUG);
    }
    else if (m_language != "en")
    {
        QString error = "Error: Unknown Language " + m_language + " selected.";
        CONSOLE_PRINT(error, Console::eERROR);
        m_language = "en";
        if (m_Translator.load(QLocale(m_language), QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/translation/lang_" + m_language))
        {
            CONSOLE_PRINT("Loaded language " + language, Console::eDEBUG);
        }
    }
    QCoreApplication::installTranslator(&m_Translator);
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (pInterpreter != nullptr)
    {
        pInterpreter->installExtensions(QJSEngine::Extension::AllExtensions);
    }
}

QStringList Settings::getActiveModVersions()
{
    return m_activeModVersions;
}

bool Settings::getRecord()
{
    return m_record;
}

void Settings::setRecord(bool record)
{
    m_record = record;
}

void Settings::loadSettings()
{
    bool ok = false;
    QSettings settings(m_settingFile, QSettings::IniFormat);
    for (auto setting : m_SettingValues)
    {
        setting->readValue(settings);
    }
    setLanguage(m_language);
    setFramesPerSecond(m_framesPerSecond);
    setActiveMods(m_activeMods);
    Console::setLogLevel(m_defaultLogLevel);
    if (Settings::hasSmallScreen())
    {
        m_autoScrolling = false;
        m_simpleDeselect = true;
        m_showDetailedBattleForcast = false;
    }
    Userdata::getInstance()->setUniqueIdentifier(getUsername());
}

void Settings::resetSettings()
{
    for (auto setting : m_SettingValues)
    {
        setting->resetValue();
    }
    saveSettings();
}

void Settings::saveSettings()
{
    CONSOLE_PRINT("Settings::saveSettings()", Console::eDEBUG);
    Mainapp* pApp = Mainapp::getInstance();
    if (!pApp->getSlave())
    {
        QSettings settings(m_settingFile, QSettings::IniFormat);
        for (auto setting : m_SettingValues)
        {
            setting->saveValue(settings);
        }
    }
}

QString Settings::getModString()
{
    return getConfigString(m_activeMods);
}

QString Settings::getConfigString(QStringList list)
{
    QString listString = "";
    for (qint32 i = 0; i < list.size(); i++)
    {
        listString += list[i] ;
        if (i < list.size() - 1)
        {
            listString += ",";
        }
    }
    return listString;
}

float Settings::getMouseSensitivity()
{
    return m_mouseSensitivity;
}

void Settings::setMouseSensitivity(float value)
{
    m_mouseSensitivity = value;
}

GameEnums::BattleAnimationMode Settings::getBattleAnimationMode()
{
    return m_battleAnimationsMode;
}

void Settings::setBattleAnimationMode(GameEnums::BattleAnimationMode value)
{
    m_battleAnimationsMode = value;
}

void Settings::setFullscreen(bool fullscreen)
{
    m_fullscreen = fullscreen;
}

void Settings::setBorderless(bool borderless)
{
    m_borderless = borderless;
}

void Settings::setWidth(const qint32 &width)
{
    m_width = width;
}

void Settings::setHeight(const qint32 &height)
{
    m_height = height;
}

float Settings::getAnimationSpeedValue()
{
    return m_animationSpeed;
}

float Settings::getAnimationSpeed()
{
    if (m_animationSpeed <= 100)
    {
        return 100.0f / (101.0f - static_cast<float>(m_animationSpeed));
    }
    return 100;
}

void Settings::setAnimationSpeed(const quint32 &value)
{
    m_animationSpeed = value;
}

float Settings::getBattleAnimationSpeedValue()
{
    return m_battleAnimationSpeed;
}

float Settings::getBattleAnimationSpeed()
{
    if (m_battleAnimationSpeed <= 100)
    {
        return 100.0f / (101.0f - static_cast<float>(m_battleAnimationSpeed));
    }
    return 100;
}

void Settings::setBattleAnimationSpeed(const quint32 &value)
{
    m_battleAnimationSpeed = value;
}

float Settings::getDialogAnimationSpeedValue()
{
    return m_dialogAnimationSpeed;
}

float Settings::getDialogAnimationSpeed()
{
    if (m_dialogAnimationSpeed <= 100)
    {
        return 100.0f / (101.0f - static_cast<float>(m_dialogAnimationSpeed));
    }
    return 100;
}

void Settings::setDialogAnimationSpeed(const quint32 &value)
{
    m_dialogAnimationSpeed = value;
}

float Settings::getCaptureAnimationSpeedValue()
{
    return m_captureAnimationSpeed;
}

float Settings::getCaptureAnimationSpeed()
{
    if (m_captureAnimationSpeed <= 100)
    {
        return 100.0f / (101.0f - static_cast<float>(m_captureAnimationSpeed));
    }
    return 100;
}

void Settings::setCaptureAnimationSpeed(const quint32 &value)
{
    m_captureAnimationSpeed = value;
}

Qt::Key Settings::getKey_up()
{
    return m_key_up;
}

void Settings::setKey_up(const Qt::Key &key_up)
{
    m_key_up = key_up;
}

Qt::Key Settings::getKey_down()
{
    return m_key_down;
}

void Settings::setKey_down(const Qt::Key &key_down)
{
    m_key_down = key_down;
}

Qt::Key Settings::getKey_right()
{
    return m_key_right;
}

void Settings::setKey_right(const Qt::Key &key_right)
{
    m_key_right = key_right;
}

Qt::Key Settings::getKey_left()
{
    return m_key_left;
}

void Settings::setKey_left(const Qt::Key &key_left)
{
    m_key_left = key_left;
}

Qt::Key Settings::getKey_confirm()
{
    return m_key_confirm;
}

void Settings::setKey_confirm(const Qt::Key &key_confirm)
{
    m_key_confirm = key_confirm;
}

Qt::Key Settings::getKey_cancel()
{
    return m_key_cancel;
}

void Settings::setKey_cancel(const Qt::Key &key_cancel)
{
    m_key_cancel = key_cancel;
}

Qt::Key Settings::getKey_next()
{
    return m_key_next;
}

void Settings::setKey_next(const Qt::Key &key_next)
{
    m_key_next = key_next;
}

Qt::Key Settings::getKey_previous()
{
    return m_key_previous;
}

void Settings::setKey_previous(const Qt::Key &key_previous)
{
    m_key_previous = key_previous;
}

Qt::Key Settings::getKey_quicksave1()
{
    return m_key_quicksave1;
}

void Settings::setKey_quicksave1(const Qt::Key &key_quicksave1)
{
    m_key_quicksave1 = key_quicksave1;
}

Qt::Key Settings::getKey_quicksave2()
{
    return m_key_quicksave2;
}

void Settings::setKey_quicksave2(const Qt::Key &key_quicksave2)
{
    m_key_quicksave2 = key_quicksave2;
}

Qt::Key Settings::getKey_quickload1()
{
    return m_key_quickload1;
}

void Settings::setKey_quickload1(const Qt::Key &key_quickload1)
{
    m_key_quickload1 = key_quickload1;
}

Qt::Key Settings::getKey_quickload2()
{
    return m_key_quickload2;
}

void Settings::setKey_quickload2(const Qt::Key &key_quickload2)
{
    m_key_quickload2 = key_quickload2;
}

Qt::Key Settings::getKey_information()
{
    return m_key_information;
}

void Settings::setKey_information(const Qt::Key &key_information)
{
    m_key_information = key_information;
}

Qt::Key Settings::getKey_MapZoomIn()
{
    return m_key_MapZoomIn;
}

void Settings::setKey_MapZoomIn(const Qt::Key &key_MapZoomIn)
{
    m_key_MapZoomIn = key_MapZoomIn;
}

Qt::Key Settings::getKey_MapZoomOut()
{
    return m_key_MapZoomOut;
}

void Settings::setKey_MapZoomOut(const Qt::Key &key_MapZoomOut)
{
    m_key_MapZoomOut = key_MapZoomOut;
}

bool Settings::getStaticMarkedFields()
{
    return m_StaticMarkedFields;
}

void Settings::setStaticMarkedFields(bool StaticMarkedFields)
{
    m_StaticMarkedFields = StaticMarkedFields;
}

qint32 Settings::getMenuItemCount()
{
    return m_MenuItemCount;
}

void Settings::setMenuItemCount(const qint32 &MenuItemCount)
{
    m_MenuItemCount = MenuItemCount;
}

quint32 Settings::getMultiTurnCounter()
{
    return multiTurnCounter;
}

void Settings::setMultiTurnCounter(const quint32 &value)
{
    multiTurnCounter = value;
}

GameEnums::BattleAnimationType Settings::getBattleAnimationType()
{
    return m_battleAnimationType;
}

void Settings::setBattleAnimationType(const GameEnums::BattleAnimationType &value)
{
    m_battleAnimationType = value;
}

Qt::Key Settings::getKey_moveMapLeft()
{
    return m_key_moveMapLeft;
}

void Settings::setKey_moveMapLeft(const Qt::Key &key_moveMapLeft)
{
    m_key_moveMapLeft = key_moveMapLeft;
}

Qt::Key Settings::getKey_moveMapRight()
{
    return m_key_moveMapRight;
}

void Settings::setKey_moveMapRight(const Qt::Key &key_moveMapRight)
{
    m_key_moveMapRight = key_moveMapRight;
}

Qt::Key Settings::getKey_moveMapDown()
{
    return m_key_moveMapDown;
}

void Settings::setKey_moveMapDown(const Qt::Key &key_moveMapDown)
{
    m_key_moveMapDown = key_moveMapDown;
}

Qt::Key Settings::getKey_moveMapUp()
{
    return m_key_moveMapUp;
}

void Settings::setKey_moveMapUp(const Qt::Key &key_moveMapUp)
{
    m_key_moveMapUp = key_moveMapUp;
}

bool Settings::getAutoEndTurn()
{
    return m_AutoEndTurn;
}

void Settings::setAutoEndTurn(bool AutoEndTurn)
{
    m_AutoEndTurn = AutoEndTurn;
}

bool Settings::getShowCursor()
{
    return m_ShowCursor;
}

void Settings::setShowCursor(bool ShowCursor)
{
    m_ShowCursor = ShowCursor;
}

QString Settings::getUsername()
{
    return m_Username;
}

void Settings::setUsername(const QString &Username)
{
    m_Username = Username;
    Userdata::getInstance()->setUniqueIdentifier(getUsername());
}

QString Settings::getLastSaveGame()
{
    return m_LastSaveGame;
}

void Settings::setLastSaveGame(const QString &LastSaveGame)
{
    m_LastSaveGame = LastSaveGame;
}

void Settings::setServer(bool Server)
{
    m_Server = Server;
}

void Settings::setServerAdress(const QString &ServerAdress)
{
    m_ServerAdress = ServerAdress;
}

QString Settings::getServerAdress()
{
    return m_ServerAdress;
}

quint16 Settings::getServerPort()
{
    return m_ServerPort;
}

void Settings::setServerPort(const quint16 &ServerPort)
{
    m_ServerPort = ServerPort;
}

QString Settings::getModName(QString mod)
{
    QString name;
    QString description;
    QString version;
    QStringList compatibleMods;
    QStringList incompatibleMods;
    QStringList requiredMods;
    bool isCosmetic = false;
    QStringList tags;
    QString thumbnail;
    getModInfos(mod, name, description, version, compatibleMods, incompatibleMods, requiredMods, isCosmetic, tags, thumbnail);
    return name;
}

bool Settings::getIsCosmetic(QString mod)
{
    QString name;
    QString description;
    QString version;
    QStringList compatibleMods;
    QStringList incompatibleMods;
    QStringList requiredMods;
    QStringList tags;
    QString thumbnail;
    bool isCosmetic = false;
    getModInfos(mod, name, description, version, compatibleMods, incompatibleMods, requiredMods, isCosmetic,
                tags, thumbnail);
    return isCosmetic;
}

void Settings::getModInfos(QString mod, QString & name, QString & description, QString & version,
                           QStringList & compatibleMods, QStringList & incompatibleMods,
                           QStringList & requiredMods, bool & isCosmetic,
                           QStringList & tags, QString & thumbnail)
{
    name = mod;
    QFile file(Settings::getUserPath() + mod + "/mod.txt");
    if (!file.exists())
    {
        file.setFileName(oxygine::Resource::RCC_PREFIX_PATH + mod + "/mod.txt");
    }
    isCosmetic = false;
    if (file.exists())
    {
        file.open(QFile::ReadOnly);
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            if (line.startsWith("name="))
            {
                name = line.split("=")[1];
            }
            if (line.startsWith("description="))
            {
                description = line.split("=")[1];
            }
            if (line.startsWith("version="))
            {
                version = line.split("=")[1];
            }
            if (line.startsWith("compatible_mods="))
            {
                compatibleMods = line.split("=")[1].split(";");
                compatibleMods.removeAll("");
            }
            if (line.startsWith("incompatible_mods="))
            {
                incompatibleMods = line.split("=")[1].split(";");
                incompatibleMods.removeAll("");
            }
            if (line.startsWith("required_mods="))
            {
                requiredMods = line.split("=")[1].split(";");
                requiredMods.removeAll("");
            }
            if (line.startsWith("tags="))
            {
                tags = line.split("=")[1].split(";");
                tags.removeAll("");
            }
            if (line.startsWith("cosmetic=true"))
            {
                isCosmetic = true;
            }
            if (line.startsWith("thumbnail="))
            {
                thumbnail = line.split("=")[1];
            }
        }
    }
}

QStringList Settings::getAvailableMods()
{
    QStringList mods;
    QFileInfoList rccinfoList = QDir(QString(oxygine::Resource::RCC_PREFIX_PATH) + "mods").entryInfoList(QDir::Dirs);
    QFileInfoList infoList = QDir(Settings::getUserPath() + "mods").entryInfoList(QDir::Dirs);
    infoList.append(rccinfoList);
    for (const auto & info : infoList)
    {
        QString folder = GlobalUtils::makePathRelative(info.filePath());
        if (!folder.endsWith("."))
        {
            QString mod = GlobalUtils::makePathRelative(info.filePath());
            mods.append(mod);
        }
    }
    return mods;
}

bool Settings::hasSmallScreen()
{
#ifdef GRAPHICSUPPORT
    QScreen* screen = QApplication::primaryScreen();
    QRect screenSize = screen->availableGeometry();
    if (screenSize.width() < 960 ||
        screenSize.height() < 640)
    {
        return true;
    }
#endif
    return false;
}

QStringList Settings::getAudioDevices()
{
    QStringList items = {tr("Default device")};
#ifdef AUDIOSUPPORT
    const auto deviceInfos = QMediaDevices::audioOutputs();
    for (qint32 i = 0; i < deviceInfos.size(); ++i)
    {
        items.append(deviceInfos[i].description());
    }
#endif
    return items;
}

qint32 Settings::getCurrentDevice()
{
    qint32 currentItem = 0;
#ifdef AUDIOSUPPORT
    auto currentDevice = Settings::getAudioOutput().value<QAudioDevice>();
    const auto deviceInfos = QMediaDevices::audioOutputs();

    for (qint32 i = 0; i < deviceInfos.size(); ++i)
    {
        if (deviceInfos[i] == currentDevice)
        {
            currentItem = i + 1;
        }
    }
#endif
    return currentItem;
}

void Settings::setAudioDevice(qint32 value)
{
#ifdef AUDIOSUPPORT
    AudioThread* pAudio = Mainapp::getInstance()->getAudioThread();
    if (value == 0)
    {
        auto item = QVariant::fromValue(QMediaDevices::defaultAudioOutput());
        pAudio->changeAudioDevice(item);
        Settings::setAudioOutput(QVariant(Settings::DEFAULT_AUDIODEVICE));
    }
    else
    {
        const auto deviceInfos = QMediaDevices::audioOutputs();
        if (value <= deviceInfos.size())
        {
            auto item = QVariant::fromValue(deviceInfos[value - 1]);
            Settings::setAudioOutput(item);
            pAudio->changeAudioDevice(item);
        }
    }
#endif
}

QSize Settings::getScreenSize()
{
    QRect screenSize;
#ifdef GRAPHICSUPPORT
    QScreen *screen = QApplication::primaryScreen();
    if (Settings::getFullscreen())
    {
        screenSize  = screen->geometry();
    }
    else
    {
        screenSize  = screen->availableGeometry();
    }
#endif
    return screenSize.size();
}

float Settings::getDpiFactor()
{
    return Mainapp::getInstance()->getActiveDpiFactor();
}

qint32 Settings::getScreenMode()
{
    return static_cast<qint32>(Mainapp::getInstance()->getScreenMode());
}

void Settings::setScreenMode(qint32 value)
{
    Mainapp::getInstance()->changeScreenMode(static_cast<Settings::ScreenModes>(value));
}

void Settings::changeBrightness(qint32 value)
{
    setBrightness(value);
    Mainapp::getInstance()->setBrightness(value);
}

void Settings::changeGamma(float value)
{
    setGamma(value);
    Mainapp::getInstance()->setGamma(value);
}

bool Settings::isGamepadSupported()
{
    return Gamepad::isSupported();
}

QStringList Settings::getLanguageNames()
{
     QLocale english("en");
     QStringList items = {english.nativeLanguageName()};
     QStringList paths = {QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/translation/", "resources/translation/"};
     QStringList filter;
     filter << "*.qm";
     for (const QString & path : qAsConst(paths))
     {
         QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
         while (dirIter.hasNext())
         {
             dirIter.next();
             QString lang = dirIter.fileName().replace(".qm", "").replace("lang_", "");
             if (lang != "en")
             {
                 QLocale langLoc(lang);
                 items.append(langLoc.nativeLanguageName());
             }
         }
     }
     return items;
}

QStringList Settings::getLanguageIds()
{
     QStringList languages = {"en"};
     QStringList paths = {QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/translation/", "resources/translation/"};
     QStringList filter;
     filter << "*.qm";
     for (const QString & path : qAsConst(paths))
     {
         QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
         while (dirIter.hasNext())
         {
             dirIter.next();
             QString lang = dirIter.fileName().replace(".qm", "").replace("lang_", "");
             if (lang != "en")
             {
                 languages.append(lang);
             }
         }
     }
     return languages;
}

qint32 Settings::getCurrentLanguageIndex()
{
     qint32 current = 0;
     QStringList paths = {QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/translation/", "resources/translation/"};
     QStringList filter;
     filter << "*.qm";
     qint32 i = 0;
     for (const QString & path : qAsConst(paths))
     {
         QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
         while (dirIter.hasNext())
         {
             dirIter.next();
             QString lang = dirIter.fileName().replace(".qm", "").replace("lang_", "");
             if (lang != "en")
             {
                 ++i;
                 if (lang == Settings::getLanguage())
                 {
                     current = i;
                     break;
                 }
             }
         }
     }
     return current;
}
