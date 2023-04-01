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
#include <QUuid>

#include "coreengine/settings.h"
#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"
#include "coreengine/userdata.h"
#include "coreengine/audiomanager.h"
#include "coreengine/Gamepad.h"
#include "coreengine/interpreter.h"

#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"

const char* const Settings::DEFAULT_AUDIODEVICE = "@@default@@";

// this Object
spSettings Settings::m_pInstance;

Settings* Settings::getInstance()
{
    if (m_pInstance.get() == nullptr)
    {
        m_pInstance.reset(new Settings());
    }
    return m_pInstance.get();
}

Settings::Settings()
{
#ifdef GRAPHICSUPPORT
    setObjectName("Settings");
#endif
    Interpreter::setCppOwnerShip(this);
}

float Settings::getIngameMenuScaling()
{
    return Settings::getInstance()->m_ingameMenuScaling;
}

void Settings::setIngameMenuScaling(float newIngameMenuScaling)
{
    Settings::getInstance()->m_ingameMenuScaling = newIngameMenuScaling;
}

quint8 Settings::getScreen()
{
    return Settings::getInstance()->m_screen;
}

void Settings::setScreen(quint8 newScreen)
{
    Settings::getInstance()->m_screen = newScreen;
}

QString Settings::getPipeUuid()
{
    return Settings::getInstance()->m_pipeUuid;
}

void Settings::setPipeUuid(const QString & newPipeUuid)
{
    Settings::getInstance()->m_pipeUuid = newPipeUuid;
}

bool Settings::getAiSlave()
{
    return Settings::getInstance()->m_aiSlave;
}

void Settings::setAiSlave(bool newAiSlave)
{
    Settings::getInstance()->m_aiSlave = newAiSlave;
}

bool Settings::getSpawnAiProcess()
{
    return Settings::getInstance()->m_spawnAiProcess;
}

void Settings::setSpawnAiProcess(bool newSpawnAiProcess)
{
    Settings::getInstance()->m_spawnAiProcess = newSpawnAiProcess;
}

bool Settings::getAutomaticUpdates()
{
    return Settings::getInstance()->m_automaticUpdates;
}

void Settings::setAutomaticUpdates(bool newAutomaticUpdates)
{
    Settings::getInstance()->m_automaticUpdates = newAutomaticUpdates;
}

QString Settings::getUpdateStep()
{
    return Settings::getInstance()->m_updateStep;
}

void Settings::setUpdateStep(const QString &newUpdateStep)
{
    Settings::getInstance()->m_updateStep = newUpdateStep;
}

QString Settings::getSecondaryServerAdress()
{
    return Settings::getInstance()->m_secondaryServerAdress;
}

void Settings::setSecondaryServerAdress(const QString &newSecondaryServerAdress)
{
    Settings::getInstance()->m_secondaryServerAdress = newSecondaryServerAdress;
}

QString Settings::getServerSecondaryListenAdress()
{
    return Settings::getInstance()->m_serverSecondaryListenAdress;
}

void Settings::setServerSecondaryListenAdress(const QString &newServerSecondaryListenAdress)
{
    Settings::getInstance()->m_serverSecondaryListenAdress = newServerSecondaryListenAdress;
}

QString Settings::getServerPassword()
{
    return Settings::getInstance()->m_serverPassword;
}

void Settings::setServerPassword(const QString &newServerPassword)
{
    CONSOLE_PRINT("Changed buffered server login password", GameConsole::eDEBUG);
    Settings::getInstance()->m_serverPassword = newServerPassword;
}

QString Settings::getMailServerSendAddress()
{
    return Settings::getInstance()->m_mailServerSendAddress;
}

void Settings::setMailServerSendAddress(const QString &newMailServerSendAddress)
{
    Settings::getInstance()->m_mailServerSendAddress = newMailServerSendAddress;
}

qint32 Settings::getMailServerAuthMethod()
{
    return Settings::getInstance()->m_mailServerAuthMethod;
}

void Settings::setMailServerAuthMethod(qint32 newMailServerAuthMethod)
{
    Settings::getInstance()->m_mailServerAuthMethod = newMailServerAuthMethod;
}

QString Settings::getMailServerPassword()
{
    return Settings::getInstance()->m_mailServerPassword;
}

void Settings::setMailServerPassword(QString newMailServerPassword)
{
    Settings::getInstance()->m_mailServerPassword = newMailServerPassword;
}

QString Settings::getMailServerUsername()
{
    return Settings::getInstance()->m_mailServerUsername;
}

void Settings::setMailServerUsername(QString newMailServerUsername)
{
    Settings::getInstance()->m_mailServerUsername = newMailServerUsername;
}

qint32 Settings::getMailServerConnectionType()
{
    return Settings::getInstance()->m_mailServerConnectionType;
}

void Settings::setMailServerConnectionType(qint32 newMailServerConnectionType)
{
    Settings::getInstance()->m_mailServerConnectionType = newMailServerConnectionType;
}

quint16 Settings::getMailServerPort()
{
    return Settings::getInstance()->m_mailServerPort;
}

void Settings::setMailServerPort(quint16 newMailServerPort)
{
    Settings::getInstance()->m_mailServerPort = newMailServerPort;
}

QString Settings::getMailServerAddress()
{
    return Settings::getInstance()->m_mailServerAddress;
}

void Settings::setMailServerAddress(const QString &newMailServerAddress)
{
    Settings::getInstance()->m_mailServerAddress = newMailServerAddress;
}

const std::chrono::seconds &Settings::getSuspendedDespawnTime()
{
    return Settings::getInstance()->m_suspendedDespawnTime;
}

void Settings::setSuspendedDespawnTime(const std::chrono::seconds &newSlaveDespawnTime)
{
    Settings::getInstance()->m_suspendedDespawnTime = newSlaveDespawnTime;
}

const std::chrono::seconds &Settings::getSlaveDespawnTime()
{
    return Settings::getInstance()->m_slaveDespawnTime;
}

void Settings::setSlaveDespawnTime(const std::chrono::seconds &newSlaveDespawnTime)
{
    Settings::getInstance()->m_slaveDespawnTime = newSlaveDespawnTime;
}

QString Settings::getDefaultBannlist()
{
    return Settings::getInstance()->m_defaultBannlist;
}

void Settings::setDefaultBannlist(const QString &newDefaultBannlist)
{
    Settings::getInstance()->m_defaultBannlist = newDefaultBannlist;
}

bool Settings::getUseHighDpi()
{
    return Settings::getInstance()->m_useHighDpi;
}

void Settings::setUseHighDpi(bool newUseHighDpi)
{
    Settings::getInstance()->m_useHighDpi = newUseHighDpi;
}

bool Settings::getMovementAnimations()
{
    return Settings::getInstance()->m_movementAnimations;
}

void Settings::setMovementAnimations(bool newMovementAnimations)
{
    Settings::getInstance()->m_movementAnimations = newMovementAnimations;
}

bool Settings::getDay2dayScreen()
{
    return Settings::getInstance()->m_day2dayScreen;
}

void Settings::setDay2dayScreen(bool newDay2dayScreen)
{
    Settings::getInstance()->m_day2dayScreen = newDay2dayScreen;
}

bool Settings::getCaptureAnimation()
{
    return Settings::getInstance()->m_captureAnimation;
}

void Settings::setCaptureAnimation(bool newCaptureAnimation)
{
    Settings::getInstance()->m_captureAnimation = newCaptureAnimation;
}

QString Settings::getSlaveListenAdress()
{
    return Settings::getInstance()->m_slaveListenAdress;
}

void Settings::setSlaveListenAdress(const QString &newSlaveListenAdress)
{
    Settings::getInstance()->m_slaveListenAdress = newSlaveListenAdress;
}

QString Settings::getServerListenAdress()
{
    return Settings::getInstance()->m_serverListenAdress;
}

void Settings::setServerListenAdress(const QString &newServerListenAdress)
{
    Settings::getInstance()->m_serverListenAdress = newServerListenAdress;
}

qint32 Settings::getPauseAfterAction()
{
    return Settings::getInstance()->m_pauseAfterAction;
}

void Settings::setPauseAfterAction(qint32 newPauseAfterAction)
{
    Settings::getInstance()->m_pauseAfterAction = newPauseAfterAction;
}

QString Settings::getSlaveHostOptions()
{
    return Settings::getInstance()->m_slaveHostOptions;
}

void Settings::setSlaveHostOptions(const QString &newSlaveHostOptions)
{
    Settings::getInstance()->m_slaveHostOptions = newSlaveHostOptions;
}

quint16 Settings::getSlaveServerPort()
{
    return Settings::getInstance()->m_slaveServerPort;
}

void Settings::setSlaveServerPort(quint16 newSlaveServerPort)
{
    Settings::getInstance()->m_slaveServerPort = newSlaveServerPort;
}

void Settings::setKey_mapshot(Qt::Key newKey_mapshot)
{
    Settings::getInstance()->m_key_mapshot = newKey_mapshot;
}

Qt::Key Settings::getKey_mapshot()
{
    return Settings::getInstance()->m_key_mapshot;
}

qint32 Settings::getFramesPerSecond()
{
    return Settings::getInstance()->m_framesPerSecond;
}

void Settings::setFramesPerSecond(qint32 newFramesPerSecond)
{
    constexpr qint32 msPerSec = 1000;
    Settings::getInstance()->m_framesPerSecond = newFramesPerSecond;
    if (Settings::getInstance()->m_framesPerSecond >= msPerSec)
    {
        Settings::getInstance()->m_framesPerSecond = msPerSec;
    }
    else if (Settings::getInstance()->m_framesPerSecond < 30)
    {
        Settings::getInstance()->m_framesPerSecond = 30;
    }
    Mainapp* pApp = Mainapp::getInstance();
    if (pApp != nullptr)
    {
        pApp->setTimerCycle(msPerSec / Settings::getInstance()->m_framesPerSecond);
    }
}

bool Settings::getMuted()
{
    return Settings::getInstance()->m_muted;
}

void Settings::setMuted(bool newMuted)
{
    Settings::getInstance()->m_muted = newMuted;
}

float Settings::getSupplyWarning()
{
    return Settings::getInstance()->m_supplyWarning;
}

void Settings::setSupplyWarning(float newSupplyWarning)
{
    Settings::getInstance()->m_supplyWarning = newSupplyWarning;
}

QString Settings::getDefaultRuleset()
{
    return Settings::getInstance()->m_defaultRuleset;
}

void Settings::setDefaultRuleset(const QString &newDefaultRuleset)
{
    Settings::getInstance()->m_defaultRuleset = newDefaultRuleset;
}

float Settings::getGamepadSensitivity()
{
    return Settings::getInstance()->m_gamepadSensitivity;
}

void Settings::setGamepadSensitivity(float newGamepadSensitivity)
{
    Settings::getInstance()->m_gamepadSensitivity = newGamepadSensitivity;
}

bool Settings::getGamepadEnabled()
{
    return Settings::getInstance()->m_gamepadEnabled;
}

void Settings::setGamepadEnabled(bool newGamepadEnabled)
{
    Settings::getInstance()->m_gamepadEnabled = newGamepadEnabled;
    if (Settings::getInstance()->m_gamepadEnabled)
    {
        Mainapp::getInstance()->getGamepad().init();
    }
}

bool Settings::getUseCoMinis()
{
    return Settings::getInstance()->m_useCoMinis;
}

void Settings::setUseCoMinis(bool newUseCoMinis)
{
    Settings::getInstance()->m_useCoMinis = newUseCoMinis;
}

bool Settings::getOverworldAnimations()
{
    return Settings::getInstance()->m_overworldAnimations;
}

void Settings::setOverworldAnimations(bool newOverworldAnimations)
{
    Settings::getInstance()->m_overworldAnimations = newOverworldAnimations;
}

qint32 Settings::getTouchPointSensitivity()
{
    return Settings::getInstance()->m_touchPointSensitivity;
}

void Settings::setTouchPointSensitivity(qint32 newTouchPointSensitivity)
{
    Settings::getInstance()->m_touchPointSensitivity = newTouchPointSensitivity;
}

const QVariant &Settings::getAudioOutput()
{
    return Settings::getInstance()->m_audioOutput;
}

void Settings::setAudioOutput(const QVariant &newAudioOutput)
{
    Settings::getInstance()->m_audioOutput = newAudioOutput;
}

bool Settings::getAutoMoveCursor()
{
    return Settings::getInstance()->m_autoMoveCursor;
}

void Settings::setAutoMoveCursor(bool newAutoMoveCursor)
{
    Settings::getInstance()->m_autoMoveCursor = newAutoMoveCursor;
}

bool Settings::getShowDetailedBattleForcast()
{
    return Settings::getInstance()->m_showDetailedBattleForcast;
}

void Settings::setShowDetailedBattleForcast(bool newShowDetailedBattleForcast)
{
    Settings::getInstance()->m_showDetailedBattleForcast = newShowDetailedBattleForcast;
}

bool Settings::getTouchScreen()
{
    return Settings::getInstance()->m_touchScreen;
}

void Settings::setTouchScreen(bool newTouchScreen)
{
    Settings::getInstance()->m_touchScreen = newTouchScreen;
}

QString Settings::getUserPath()
{
    if (Settings::getInstance()->m_userPath.isEmpty())
    {
        return Settings::getInstance()->m_userPath;
    }
    else
    {
        QString folder = Settings::getInstance()->m_userPath + "/";
        while (folder.contains("//"))
        {
            folder = folder.replace("//", "/");
        }
        return folder;
    }
}

void Settings::setUserPath(const QString &newUserPath)
{
    Settings::getInstance()->m_userPath = newUserPath;
}

bool Settings::getSmallScreenDevice()
{
    return Settings::getInstance()->m_smallScreenDevice;
}

void Settings::setSmallScreenDevice(bool newSmallScreenDevice)
{
    Settings::getInstance()->m_smallScreenDevice = newSmallScreenDevice;
}

qint32 Settings::getMenuItemRowCount()
{
    return Settings::getInstance()->m_MenuItemRowCount;
}

void Settings::setMenuItemRowCount(qint32 newMenuItemRowCount)
{
    Settings::getInstance()->m_MenuItemRowCount = newMenuItemRowCount;
}

bool Settings::getSimpleDeselect()
{
    return Settings::getInstance()->m_simpleDeselect;
}

void Settings::setSimpleDeselect(bool newSimpleDeselect)
{
    Settings::getInstance()->m_simpleDeselect = newSimpleDeselect;
}

bool Settings::getSyncAnimations()
{
    return Settings::getInstance()->m_syncAnimations;
}

void Settings::setSyncAnimations(bool syncAnimations)
{
    Settings::getInstance()->m_syncAnimations = syncAnimations;
}

bool Settings::getCenterOnMarkedField()
{
    return Settings::getInstance()->m_centerOnMarkedField;
}

void Settings::setCenterOnMarkedField(bool centerOnMarkedField)
{
    Settings::getInstance()->m_centerOnMarkedField = centerOnMarkedField;
}

bool Settings::getDialogAnimation()
{
    return Settings::getInstance()->m_dialogAnimation;
}

void Settings::setDialogAnimation(bool dialogAnimation)
{
    Settings::getInstance()->m_dialogAnimation = dialogAnimation;
}

float Settings::getGamma()
{
    return Settings::getInstance()->m_gamma;
}

void Settings::setGamma(float gamma)
{
    Settings::getInstance()->m_gamma = gamma;
}

float Settings::getBrightness()
{
    return Settings::getInstance()->m_brightness;
}

void Settings::setBrightness(float brightness)
{
    Settings::getInstance()->m_brightness = brightness;
}

Qt::Key Settings::getKey_screenshot()
{
    return Settings::getInstance()->m_key_screenshot;
}

void Settings::setKey_screenshot(const Qt::Key &key_screenshot)
{
    Settings::getInstance()->m_key_screenshot = key_screenshot;
}

GameEnums::AutoFocusing Settings::getAutoFocusing()
{
    return Settings::getInstance()->m_autoFocusing;
}

void Settings::setAutoFocusing(const GameEnums::AutoFocusing &autoFocusing)
{
    Settings::getInstance()->m_autoFocusing = autoFocusing;
}

Qt::Key Settings::getKey_EditorSelectionLeft()
{
    return Settings::getInstance()->m_key_EditorSelectionLeft;
}

void Settings::setKey_EditorSelectionLeft(const Qt::Key &key_EditorSelectionLeft)
{
    Settings::getInstance()->m_key_EditorSelectionLeft = key_EditorSelectionLeft;
}

Qt::Key Settings::getKey_EditorSelectionRight()
{
    return Settings::getInstance()->m_key_EditorSelectionRight;
}

void Settings::setKey_EditorSelectionRight(const Qt::Key &key_EditorSelectionRight)
{
    Settings::getInstance()->m_key_EditorSelectionRight = key_EditorSelectionRight;
}

Qt::Key Settings::getKey_EditorPreviousTeam()
{
    return Settings::getInstance()->m_key_EditorPreviousTeam;
}

void Settings::setKey_EditorPreviousTeam(const Qt::Key &key_EditorPreviousTeam)
{
    Settings::getInstance()->m_key_EditorPreviousTeam = key_EditorPreviousTeam;
}

Qt::Key Settings::getKey_EditorNextTeam()
{
    return Settings::getInstance()->m_key_EditorNextTeam;
}

void Settings::setKey_EditorNextTeam(const Qt::Key &key_EditorNextTeam)
{
    Settings::getInstance()->m_key_EditorNextTeam = key_EditorNextTeam;
}

Qt::Key Settings::getKey_EditorPlaceUnit()
{
    return Settings::getInstance()->m_key_EditorPlaceUnit;
}

void Settings::setKey_EditorPlaceUnit(const Qt::Key &key_EditorPlaceUnit)
{
    Settings::getInstance()->m_key_EditorPlaceUnit = key_EditorPlaceUnit;
}

Qt::Key Settings::getKey_EditorPlaceBuilding()
{
    return Settings::getInstance()->m_key_EditorPlaceBuilding;
}

void Settings::setKey_EditorPlaceBuilding(const Qt::Key &key_EditorPlaceBuilding)
{
    Settings::getInstance()->m_key_EditorPlaceBuilding = key_EditorPlaceBuilding;
}

Qt::Key Settings::getKey_EditorPlaceTerrain()
{
    return Settings::getInstance()->m_key_EditorPlaceTerrain;
}

void Settings::setKey_EditorPlaceTerrain(const Qt::Key &key_EditorPlaceTerrain)
{
    Settings::getInstance()->m_key_EditorPlaceTerrain = key_EditorPlaceTerrain;
}

Qt::Key Settings::getKey_ShowIndirectAttackFields2()
{
    return Settings::getInstance()->m_key_ShowIndirectAttackFields2;
}

void Settings::setKey_ShowIndirectAttackFields2(const Qt::Key &key_ShowIndirectAttackFields2)
{
    Settings::getInstance()->m_key_ShowIndirectAttackFields2 = key_ShowIndirectAttackFields2;
}

Qt::Key Settings::getKey_ShowAttackFields2()
{
    return Settings::getInstance()->m_key_ShowAttackFields2;
}

void Settings::setKey_ShowAttackFields2(const Qt::Key &key_ShowAttackFields2)
{
    Settings::getInstance()->m_key_ShowAttackFields2 = key_ShowAttackFields2;
}

Qt::Key Settings::getKey_MapZoomIn2()
{
    return Settings::getInstance()->m_key_MapZoomIn2;
}

void Settings::setKey_MapZoomIn2(const Qt::Key &key_MapZoomIn2)
{
    Settings::getInstance()->m_key_MapZoomIn2 = key_MapZoomIn2;
}

Qt::Key Settings::getKey_MapZoomOut2()
{
    return Settings::getInstance()->m_key_MapZoomOut2;
}

void Settings::setKey_MapZoomOut2(const Qt::Key &key_MapZoomOut2)
{
    Settings::getInstance()->m_key_MapZoomOut2 = key_MapZoomOut2;
}

Qt::Key Settings::getKey_moveMapLeft2()
{
    return Settings::getInstance()->m_key_moveMapLeft2;
}

void Settings::setKey_moveMapLeft2(const Qt::Key &key_moveMapLeft2)
{
    Settings::getInstance()->m_key_moveMapLeft2 = key_moveMapLeft2;
}

Qt::Key Settings::getKey_moveMapRight2()
{
    return Settings::getInstance()->m_key_moveMapRight2;
}

void Settings::setKey_moveMapRight2(const Qt::Key &key_moveMapRight2)
{
    Settings::getInstance()->m_key_moveMapRight2 = key_moveMapRight2;
}

Qt::Key Settings::getKey_moveMapDown2()
{
    return Settings::getInstance()->m_key_moveMapDown2;
}

void Settings::setKey_moveMapDown2(const Qt::Key &key_moveMapDown2)
{
    Settings::getInstance()->m_key_moveMapDown2 = key_moveMapDown2;
}

Qt::Key Settings::getKey_moveMapUp2()
{
    return Settings::getInstance()->m_key_moveMapUp2;
}

void Settings::setKey_moveMapUp2(const Qt::Key &key_moveMapUp2)
{
    Settings::getInstance()->m_key_moveMapUp2 = key_moveMapUp2;
}

Qt::Key Settings::getKey_information2()
{
    return Settings::getInstance()->m_key_information2;
}

void Settings::setKey_information2(const Qt::Key &key_information2)
{
    Settings::getInstance()->m_key_information2 = key_information2;
}

Qt::Key Settings::getKey_previous2()
{
    return Settings::getInstance()->m_key_previous2;
}

void Settings::setKey_previous2(const Qt::Key &key_previous2)
{
    Settings::getInstance()->m_key_previous2 = key_previous2;
}

Qt::Key Settings::getKey_next2()
{
    return Settings::getInstance()->m_key_next2;
}

void Settings::setKey_next2(const Qt::Key &key_next2)
{
    Settings::getInstance()->m_key_next2 = key_next2;
}

Qt::Key Settings::getKey_cancel2()
{
    return Settings::getInstance()->m_key_cancel2;
}

void Settings::setKey_cancel2(const Qt::Key &key_cancel2)
{
    Settings::getInstance()->m_key_cancel2 = key_cancel2;
}

Qt::Key Settings::getKey_confirm2()
{
    return Settings::getInstance()->m_key_confirm2;
}

void Settings::setKey_confirm2(const Qt::Key &key_confirm2)
{
    Settings::getInstance()->m_key_confirm2 = key_confirm2;
}

Qt::Key Settings::getKey_left2()
{
    return Settings::getInstance()->m_key_left2;
}

void Settings::setKey_left2(const Qt::Key &key_left2)
{
    Settings::getInstance()->m_key_left2 = key_left2;
}

Qt::Key Settings::getKey_right2()
{
    return Settings::getInstance()->m_key_right2;
}

void Settings::setKey_right2(const Qt::Key &key_right2)
{
    Settings::getInstance()->m_key_right2 = key_right2;
}

Qt::Key Settings::getKey_down2()
{
    return Settings::getInstance()->m_key_down2;
}

void Settings::setKey_down2(const Qt::Key &key_down2)
{
    Settings::getInstance()->m_key_down2 = key_down2;
}

Qt::Key Settings::getKey_up2()
{
    return Settings::getInstance()->m_key_up2;
}

void Settings::setKey_up2(const Qt::Key &key_up2)
{
    Settings::getInstance()->m_key_up2 = key_up2;
}

bool Settings::getAutoCamera()
{
    return Settings::getInstance()->m_autoCamera;
}

void Settings::setAutoCamera(bool autoCamera)
{
    Settings::getInstance()->m_autoCamera = autoCamera;
}

bool Settings::getAutoScrolling()
{
    return Settings::getInstance()->m_autoScrolling;
}

void Settings::setAutoScrolling(bool autoScrolling)
{
    Settings::getInstance()->m_autoScrolling = autoScrolling;
}

GameEnums::COInfoPosition Settings::getCoInfoPosition()
{
    return Settings::getInstance()->m_coInfoPosition;
}

void Settings::setCoInfoPosition(const GameEnums::COInfoPosition &value)
{
    Settings::getInstance()->m_coInfoPosition = value;
}

void Settings::setY(const qint32 &y)
{
    Settings::getInstance()->m_y = y;
}

void Settings::setX(const qint32 &x)
{
    Settings::getInstance()->m_x = x;
}

void Settings::setSlaveServerName(const QString &slaveServerName)
{
    Settings::getInstance()->m_slaveServerName = slaveServerName;
}

QString Settings::getSlaveServerName()
{
    return Settings::getInstance()->m_slaveServerName;
}

QStringList Settings::getActiveMods()
{
    return Settings::getInstance()->m_activeMods;
}

void Settings::setActiveMods(const QStringList &activeMods)
{
    Settings::getInstance()->m_activeMods = activeMods;
    qint32 i = 0;
    while (i < Settings::getInstance()->m_activeMods.size())
    {
        QDir dir(getUserPath() + Settings::getInstance()->m_activeMods[i]);
        QDir dir2( oxygine::Resource::RCC_PREFIX_PATH + Settings::getInstance()->m_activeMods[i]);
        if (!dir.exists() && !dir2.exists())
        {
            CONSOLE_PRINT("Removing mod from active list: " + Settings::getInstance()->m_activeMods[i] + " cause it wasn't found.", GameConsole::eWARNING);
            Settings::getInstance()->m_activeMods.removeAt(i);
        }
        else
        {
            ++i;
        }
    }
    Settings::getInstance()->m_activeMods.sort();
    for (const auto & mod : qAsConst(Settings::getInstance()->m_activeMods))
    {
        CONSOLE_PRINT("Loaded mod: " + mod, GameConsole::eDEBUG);
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
                    Settings::getInstance()->m_activeModVersions.append(line.split("=")[1]);
                    found = true;
                    break;
                }
            }
        }
        if (!found)
        {
            Settings::getInstance()->m_activeModVersions.append("1.0.0");
        }
    }
}

bool Settings::getShowIngameCoordinates()
{
    return Settings::getInstance()->m_showIngameCoordinates;
}

void Settings::setShowIngameCoordinates(bool showIngameCoordinates)
{
    Settings::getInstance()->m_showIngameCoordinates = showIngameCoordinates;
}

quint32 Settings::getWalkAnimationSpeedValue()
{
    return Settings::getInstance()->m_walkAnimationSpeed;
}

void Settings::setWalkAnimationSpeed(const quint32 &value)
{
    Settings::getInstance()->m_walkAnimationSpeed = value;
}


float Settings::getWalkAnimationSpeed()
{
    if (Settings::getInstance()->m_walkAnimationSpeed <= 100)
    {
        return 100.0f / (101.0f - static_cast<float>(Settings::getInstance()->m_walkAnimationSpeed));
    }
    return 100;
}

qint32 Settings::getShowCoCount()
{
    return Settings::getInstance()->m_showCoCount;
}

void Settings::setShowCoCount(const qint32 &showCoCount)
{
    Settings::getInstance()->m_showCoCount = showCoCount;
}

qint32 Settings::getAutoSavingCycle()
{
    return Settings::getInstance()->m_autoSavingCycle;
}

void Settings::setAutoSavingCycle(const qint32 &value)
{
    Settings::getInstance()->m_autoSavingCycle = value;
}

std::chrono::seconds Settings::getAutoSavingCylceTime()
{
    return Settings::getInstance()->m_autoSavingCylceTime;
}

void Settings::setAutoSavingCylceTime(const std::chrono::seconds &value)
{
    Settings::getInstance()->m_autoSavingCylceTime = value;
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
    return Settings::getInstance()->m_key_ShowAttackFields;
}

void Settings::setKey_ShowAttackFields(const Qt::Key &key_ShowAttackFields)
{
    Settings::getInstance()->m_key_ShowAttackFields = key_ShowAttackFields;
}

Qt::Key Settings::getKey_ShowIndirectAttackFields()
{
    return Settings::getInstance()->m_key_ShowIndirectAttackFields;
}

void Settings::setKey_ShowIndirectAttackFields(const Qt::Key &key_ShowIndirectAttackFields)
{
    Settings::getInstance()->m_key_ShowIndirectAttackFields = key_ShowIndirectAttackFields;
}

bool Settings::getLogActions()
{
    return Settings::getInstance()->m_LogActions;
}

void Settings::setLogActions(bool LogActions)
{
    Settings::getInstance()->m_LogActions = LogActions;
}

QStringList Settings::getActiveModVersions()
{
    return Settings::getInstance()->m_activeModVersions;
}

bool Settings::getRecord()
{
    return Settings::getInstance()->m_record;
}

void Settings::setRecord(bool record)
{
    Settings::getInstance()->m_record = record;
}

QString Settings::getActiveUserPath()
{
    bool smallScreenDevice = hasSmallScreen();
    QString defaultPath = "";
    if (smallScreenDevice)
    {
        defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/commander_wars/";
    }
#ifdef USEAPPCONFIGPATH
    defaultPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/";
#endif
    Mainapp::getInstance()->getParser().getUserPath(defaultPath);
    if (!defaultPath.isEmpty())
    {
        defaultPath += "/";
    }
    while (defaultPath.contains("//"))
    {
        defaultPath = defaultPath.replace("//", "/");
    }
    return defaultPath;
}

void Settings::setup()
{
    QSize size;
#ifdef GRAPHICSUPPORT
    auto screens = QApplication::screens();
    size = screens[Settings::getScreen()]->availableSize() * screens[Settings::getScreen()]->devicePixelRatio();
#endif
    bool smallScreenDevice = hasSmallScreen();
    qint32 defaultCoCount = 0;
    if (smallScreenDevice)
    {
        defaultCoCount = 1;
    }
    QString defaultPath = getActiveUserPath();
    setUserPath(defaultPath);
    Settings::getInstance()->m_settingFile = defaultPath + Settings::getInstance()->m_settingFile;
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
        new Value<quint8>{"Resolution", "screen", &m_screen, 0, 0, 255, true},
        new Value<bool>{"Resolution", "recordgames", &m_record, false, false, true},
        new Value<bool>{"Resolution", "SmallScreenDevice", &m_smallScreenDevice, smallScreenDevice, false, true},
        new Value<bool>{"Resolution", "UseHighDpi", &m_useHighDpi, false, false, true},
        new Value<float>{"Resolution", "IngameMenuScaling", &m_ingameMenuScaling, 1.0f, 0.5f, 10.0f},

        // general
        new Value<QString>{"General", "language", &m_language, "en", "", ""},
        new Value<float>{"General", "MouseSensitivity", &m_mouseSensitivity, -0.75f, -100, 100},
        new Value<QString>{"General", "UserPath", &m_userPath, defaultPath, "", ""},
        new Value<bool>{"General", "TouchScreen", &m_touchScreen, hasTouch, false, true},
        new Value<qint32>{"General", "TouchPointSensitivity", &m_touchPointSensitivity, 15, 0, size.width()},
        new Value<bool>{"General", "GamepadEnabled", &m_gamepadEnabled, false, false, true},
        new Value<bool>{"General", "AutomaticUpdates", &m_automaticUpdates, true, false, true},
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
        new Value<Qt::Key>{"Keys", "key_moveMapUp", &m_key_moveMapUp, Qt::Key_Down, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_moveMapDown", &m_key_moveMapDown, Qt::Key_Up, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_moveMapRight", &m_key_moveMapRight, Qt::Key_Left, static_cast<Qt::Key>(0), Qt::Key_unknown},
        new Value<Qt::Key>{"Keys", "key_moveMapLeft", &m_key_moveMapLeft, Qt::Key_Right, static_cast<Qt::Key>(0), Qt::Key_unknown},
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
        new Value<GameEnums::BattleAnimationType>{"Game", "BattleAnimationType", &m_battleAnimationType, GameEnums::BattleAnimationType_Detail, GameEnums::BattleAnimationType_Detail, GameEnums::BattleAnimationType_FullscreenTransparent},
        new Value<quint32>{"Game", "AnimationSpeed", &m_animationSpeed, 1, 1, 100},
        new Value<quint32>{"Game", "BattleAnimationSpeed", &m_battleAnimationSpeed, 1, 1, 100},
        new Value<quint32>{"Game", "WalkAnimationSpeed", &m_walkAnimationSpeed, 20, 1, 100},
        new Value<quint32>{"Game", "DialogAnimationSpeed", &m_dialogAnimationSpeed, 1, 1, 100},
        new Value<quint32>{"Game", "CaptureAnimationSpeed", &m_captureAnimationSpeed, 1, 1, 100},
        new Value<quint32>{"Game", "MultiTurnCounter", &multiTurnCounter, 1, 1, 10},
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
        new Value<QString>{"Game", "AiPipeUuid", &m_pipeUuid, "", "", ""},
        new Value<bool>{"Game", "UseAiProcess", &m_spawnAiProcess, false, false, true},

        // network
        new Value<quint16>{"Network", "GamePort", &m_GamePort, 9001, 0, std::numeric_limits<quint16>::max()},
        new Value<QString>{"Network", "ServerAdress", &m_ServerAdress, "192.46.216.113", "", ""},
        new Value<QString>{"Network", "SecondaryServerAdress", &m_secondaryServerAdress, "2600:3c00::f03c:93ff:fe86:009e", "", ""},
        new Value<quint16>{"Network", "SlaveServerPort", &m_slaveServerPort, 9003, 0, std::numeric_limits<quint16>::max()},
        new Value<quint16>{"Network", "ServerPort", &m_ServerPort, 9002, 0, std::numeric_limits<quint16>::max()},
        new Value<bool>{"Network", "Server", &m_Server, false, false, true},
        new Value<QString>{"Network", "ServerListenAdress", &m_serverListenAdress, "", "", ""},
        new Value<QString>{"Network", "ServerSecondaryListenAdress", &m_serverSecondaryListenAdress, "", "", ""},
        new Value<QString>{"Network", "SlaveListenAdress", &m_slaveListenAdress, "", "", ""},
        new Value<QString>{"Network", "SlaveHostOptions", &m_slaveHostOptions, "::1&&10000&20000;::1&&50000&65535", "", ""},
        new Value<std::chrono::seconds>{"Network", "SlaveDespawnTime", &m_slaveDespawnTime, std::chrono::seconds(60 * 60 * 24), std::chrono::seconds(1), std::chrono::seconds(60 * 60 * 24 * 96)},
        new Value<std::chrono::seconds>{"Network", "SuspendedDespawnTime", &m_suspendedDespawnTime, std::chrono::seconds(60 * 60 * 24), std::chrono::seconds(1), std::chrono::seconds(60 * 60 * 24 * 96)},
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
        new Value<GameConsole::eLogLevels>{"Logging", "LogLevel", &m_defaultLogLevel, static_cast<GameConsole::eLogLevels>(DEBUG_LEVEL), GameConsole::eLogLevels::eOFF, GameConsole::eLogLevels::eFATAL},
        new Value<quint64>{"Logging", "LogModules", &m_defaultLogModuls, GameConsole::eGeneral | GameConsole::eJavaScript, 0, std::numeric_limits<quint64>::max()},
    };
    QSettings settings(Settings::getInstance()->m_settingFile, QSettings::IniFormat);
    for (auto* setting : Settings::getInstance()->m_SettingValues)
    {
        if (QString(setting->getGroup()) == QString("Network") &&
            QString(setting->getName()) == QString("Server"))
        {
            setting->readValue(settings);
            break;
        }
    }
}

void Settings::loadSettings()
{
    CONSOLE_PRINT("Settings::loadSettings()", GameConsole::eDEBUG);
    bool ok = false;
    QSettings settings(Settings::getInstance()->m_settingFile, QSettings::IniFormat);
    for (auto* setting : Settings::getInstance()->m_SettingValues)
    {
        setting->readValue(settings);
    }
    setFramesPerSecond(Settings::getInstance()->m_framesPerSecond);
    setActiveMods(Settings::getInstance()->m_activeMods);
    GameConsole::setLogLevel(Settings::getInstance()->m_defaultLogLevel);
    GameConsole::setActiveModules(Settings::getInstance()->m_defaultLogModuls);
    if (Settings::hasSmallScreen())
    {
        Settings::getInstance()->m_autoScrolling = false;
        Settings::getInstance()->m_simpleDeselect = true;
        Settings::getInstance()->m_showDetailedBattleForcast = false;
    }
    if (getPipeUuid().isEmpty())
    {
        setPipeUuid(QUuid::createUuid().toString());
        saveSettings();
    }
    setUsername(getUsername());
}

void Settings::resetSettings()
{
    for (auto setting : Settings::getInstance()->m_SettingValues)
    {
        setting->resetValue();
    }
    saveSettings();
}

void Settings::saveSettings()
{
    CONSOLE_PRINT("Settings::saveSettings()", GameConsole::eDEBUG);
    Mainapp* pApp = Mainapp::getInstance();
    if (!pApp->getSlave() && !Settings::getAiSlave() && Settings::getInstance()->m_updateStep.isEmpty())
    {
        QSettings settings(Settings::getInstance()->m_settingFile, QSettings::IniFormat);
        for (auto setting : Settings::getInstance()->m_SettingValues)
        {
            setting->saveValue(settings);
        }
    }
}

QString Settings::getModString()
{
    return getConfigString(Settings::getInstance()->m_activeMods);
}


void Settings::filterCosmeticMods(QStringList & mods, QStringList & versions, bool filter)
{
    if (filter)
    {
        qint32 i = 0;
        while (i < mods.length())
        {
            if (Settings::getIsCosmetic(mods[i]))
            {
                mods.removeAt(i);
                versions.removeAt(i);
            }
            else
            {
                ++i;
            }
        }
    }
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
    return Settings::getInstance()->m_mouseSensitivity;
}

void Settings::setMouseSensitivity(float value)
{
    Settings::getInstance()->m_mouseSensitivity = value;
}

GameEnums::BattleAnimationMode Settings::getBattleAnimationMode()
{
    return Settings::getInstance()->m_battleAnimationsMode;
}

void Settings::setBattleAnimationMode(GameEnums::BattleAnimationMode value)
{
    Settings::getInstance()->m_battleAnimationsMode = value;
}

void Settings::setFullscreen(bool fullscreen)
{
    Settings::getInstance()->m_fullscreen = fullscreen;
}

void Settings::setBorderless(bool borderless)
{
    Settings::getInstance()->m_borderless = borderless;
}

void Settings::setWidth(const qint32 &width)
{
    Settings::getInstance()->m_width = width;
}

void Settings::setHeight(const qint32 &height)
{
    Settings::getInstance()->m_height = height;
}

float Settings::getAnimationSpeedValue()
{
    return Settings::getInstance()->m_animationSpeed;
}

float Settings::getAnimationSpeed()
{
    if (Settings::getInstance()->m_animationSpeed <= 100)
    {
        return 100.0f / (101.0f - static_cast<float>(Settings::getInstance()->m_animationSpeed));
    }
    return 100;
}

void Settings::setAnimationSpeed(const quint32 &value)
{
    Settings::getInstance()->m_animationSpeed = value;
}

float Settings::getBattleAnimationSpeedValue()
{
    return Settings::getInstance()->m_battleAnimationSpeed;
}

float Settings::getBattleAnimationSpeed()
{
    if (Settings::getInstance()->m_battleAnimationSpeed <= 100)
    {
        return 100.0f / (101.0f - static_cast<float>(Settings::getInstance()->m_battleAnimationSpeed));
    }
    return 100;
}

void Settings::setBattleAnimationSpeed(const quint32 &value)
{
    Settings::getInstance()->m_battleAnimationSpeed = value;
}

float Settings::getDialogAnimationSpeedValue()
{
    return Settings::getInstance()->m_dialogAnimationSpeed;
}

float Settings::getDialogAnimationSpeed()
{
    if (Settings::getInstance()->m_dialogAnimationSpeed <= 100)
    {
        return 100.0f / (101.0f - static_cast<float>(Settings::getInstance()->m_dialogAnimationSpeed));
    }
    return 100;
}

void Settings::setDialogAnimationSpeed(const quint32 &value)
{
    Settings::getInstance()->m_dialogAnimationSpeed = value;
}

float Settings::getCaptureAnimationSpeedValue()
{
    return Settings::getInstance()->m_captureAnimationSpeed;
}

float Settings::getCaptureAnimationSpeed()
{
    if (Settings::getInstance()->m_captureAnimationSpeed <= 100)
    {
        return 100.0f / (101.0f - static_cast<float>(Settings::getInstance()->m_captureAnimationSpeed));
    }
    return 100;
}

void Settings::setCaptureAnimationSpeed(const quint32 &value)
{
    Settings::getInstance()->m_captureAnimationSpeed = value;
}

Qt::Key Settings::getKey_up()
{
    return Settings::getInstance()->m_key_up;
}

void Settings::setKey_up(const Qt::Key &key_up)
{
    Settings::getInstance()->m_key_up = key_up;
}

Qt::Key Settings::getKey_down()
{
    return Settings::getInstance()->m_key_down;
}

void Settings::setKey_down(const Qt::Key &key_down)
{
    Settings::getInstance()->m_key_down = key_down;
}

Qt::Key Settings::getKey_right()
{
    return Settings::getInstance()->m_key_right;
}

void Settings::setKey_right(const Qt::Key &key_right)
{
    Settings::getInstance()->m_key_right = key_right;
}

Qt::Key Settings::getKey_left()
{
    return Settings::getInstance()->m_key_left;
}

void Settings::setKey_left(const Qt::Key &key_left)
{
    Settings::getInstance()->m_key_left = key_left;
}

Qt::Key Settings::getKey_confirm()
{
    return Settings::getInstance()->m_key_confirm;
}

void Settings::setKey_confirm(const Qt::Key &key_confirm)
{
    Settings::getInstance()->m_key_confirm = key_confirm;
}

Qt::Key Settings::getKey_cancel()
{
    return Settings::getInstance()->m_key_cancel;
}

void Settings::setKey_cancel(const Qt::Key &key_cancel)
{
    Settings::getInstance()->m_key_cancel = key_cancel;
}

Qt::Key Settings::getKey_next()
{
    return Settings::getInstance()->m_key_next;
}

void Settings::setKey_next(const Qt::Key &key_next)
{
    Settings::getInstance()->m_key_next = key_next;
}

Qt::Key Settings::getKey_previous()
{
    return Settings::getInstance()->m_key_previous;
}

void Settings::setKey_previous(const Qt::Key &key_previous)
{
    Settings::getInstance()->m_key_previous = key_previous;
}

Qt::Key Settings::getKey_quicksave1()
{
    return Settings::getInstance()->m_key_quicksave1;
}

void Settings::setKey_quicksave1(const Qt::Key &key_quicksave1)
{
    Settings::getInstance()->m_key_quicksave1 = key_quicksave1;
}

Qt::Key Settings::getKey_quicksave2()
{
    return Settings::getInstance()->m_key_quicksave2;
}

void Settings::setKey_quicksave2(const Qt::Key &key_quicksave2)
{
    Settings::getInstance()->m_key_quicksave2 = key_quicksave2;
}

Qt::Key Settings::getKey_quickload1()
{
    return Settings::getInstance()->m_key_quickload1;
}

void Settings::setKey_quickload1(const Qt::Key &key_quickload1)
{
    Settings::getInstance()->m_key_quickload1 = key_quickload1;
}

Qt::Key Settings::getKey_quickload2()
{
    return Settings::getInstance()->m_key_quickload2;
}

void Settings::setKey_quickload2(const Qt::Key &key_quickload2)
{
    Settings::getInstance()->m_key_quickload2 = key_quickload2;
}

Qt::Key Settings::getKey_information()
{
    return Settings::getInstance()->m_key_information;
}

void Settings::setKey_information(const Qt::Key &key_information)
{
    Settings::getInstance()->m_key_information = key_information;
}

Qt::Key Settings::getKey_MapZoomIn()
{
    return Settings::getInstance()->m_key_MapZoomIn;
}

void Settings::setKey_MapZoomIn(const Qt::Key &key_MapZoomIn)
{
    Settings::getInstance()->m_key_MapZoomIn = key_MapZoomIn;
}

Qt::Key Settings::getKey_MapZoomOut()
{
    return Settings::getInstance()->m_key_MapZoomOut;
}

void Settings::setKey_MapZoomOut(const Qt::Key &key_MapZoomOut)
{
    Settings::getInstance()->m_key_MapZoomOut = key_MapZoomOut;
}

bool Settings::getStaticMarkedFields()
{
    return Settings::getInstance()->m_StaticMarkedFields;
}

void Settings::setStaticMarkedFields(bool StaticMarkedFields)
{
    Settings::getInstance()->m_StaticMarkedFields = StaticMarkedFields;
}

Qt::Key Settings::getKey_Escape()
{
    return Settings::getInstance()->m_key_escape;
}

void Settings::setKey_Escape(const Qt::Key &key_Escape)
{
    Settings::getInstance()->m_key_escape = key_Escape;
}

qint32 Settings::getMenuItemCount()
{
    return Settings::getInstance()->m_MenuItemCount;
}

void Settings::setMenuItemCount(const qint32 &MenuItemCount)
{
    Settings::getInstance()->m_MenuItemCount = MenuItemCount;
}

quint32 Settings::getMultiTurnCounter()
{
    return Settings::getInstance()->multiTurnCounter;
}

void Settings::setMultiTurnCounter(const quint32 &value)
{
    Settings::getInstance()->multiTurnCounter = value;
}

GameEnums::BattleAnimationType Settings::getBattleAnimationType()
{
    return Settings::getInstance()->m_battleAnimationType;
}

void Settings::setBattleAnimationType(const GameEnums::BattleAnimationType &value)
{
    Settings::getInstance()->m_battleAnimationType = value;
}

Qt::Key Settings::getKey_moveMapLeft()
{
    return Settings::getInstance()->m_key_moveMapLeft;
}

void Settings::setKey_moveMapLeft(const Qt::Key &key_moveMapLeft)
{
    Settings::getInstance()->m_key_moveMapLeft = key_moveMapLeft;
}

Qt::Key Settings::getKey_moveMapRight()
{
    return Settings::getInstance()->m_key_moveMapRight;
}

void Settings::setKey_moveMapRight(const Qt::Key &key_moveMapRight)
{
    Settings::getInstance()->m_key_moveMapRight = key_moveMapRight;
}

Qt::Key Settings::getKey_moveMapDown()
{
    return Settings::getInstance()->m_key_moveMapDown;
}

void Settings::setKey_moveMapDown(const Qt::Key &key_moveMapDown)
{
    Settings::getInstance()->m_key_moveMapDown = key_moveMapDown;
}

Qt::Key Settings::getKey_moveMapUp()
{
    return Settings::getInstance()->m_key_moveMapUp;
}

void Settings::setKey_moveMapUp(const Qt::Key &key_moveMapUp)
{
    Settings::getInstance()->m_key_moveMapUp = key_moveMapUp;
}

bool Settings::getAutoEndTurn()
{
    return Settings::getInstance()->m_AutoEndTurn;
}

void Settings::setAutoEndTurn(bool AutoEndTurn)
{
    Settings::getInstance()->m_AutoEndTurn = AutoEndTurn;
}

bool Settings::getShowCursor()
{
    return Settings::getInstance()->m_ShowCursor;
}

void Settings::setShowCursor(bool ShowCursor)
{
    Settings::getInstance()->m_ShowCursor = ShowCursor;
}

QString Settings::getUsername()
{
    return Settings::getInstance()->m_Username;
}

void Settings::setUsername(const QString &Username)
{
    if (Username.length() < 50)
    {
        Settings::getInstance()->m_Username = Username;
    }
    else
    {
        Settings::getInstance()->m_Username = "";

    }
    Userdata::getInstance()->setUniqueIdentifier(getUsername());
}

QString Settings::getLastSaveGame()
{
    return Settings::getInstance()->m_LastSaveGame;
}

void Settings::setLastSaveGame(const QString &LastSaveGame)
{
    Settings::getInstance()->m_LastSaveGame = LastSaveGame;
}

void Settings::setServer(bool Server)
{
    Settings::getInstance()->m_Server = Server;
}

void Settings::setServerAdress(const QString &ServerAdress)
{
    Settings::getInstance()->m_ServerAdress = ServerAdress;
}

QString Settings::getServerAdress()
{
    return Settings::getInstance()->m_ServerAdress;
}

quint16 Settings::getServerPort()
{
    return Settings::getInstance()->m_ServerPort;
}

void Settings::setServerPort(const quint16 &ServerPort)
{
    Settings::getInstance()->m_ServerPort = ServerPort;
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
            if (!mods.contains(mod))
            {
                mods.append(mod);
            }
        }
    }
    return mods;
}

bool Settings::hasSmallScreen()
{
#ifdef GRAPHICSUPPORT
    auto screens = QApplication::screens();
    QScreen* screen = screens[Settings::getScreen()];
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
    AudioManager* pAudio = Mainapp::getInstance()->getAudioManager();
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
    auto screens = QApplication::screens();
    QScreen *screen = screens[Settings::getScreen()];
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

QString Settings::getLanguage()
{
    return Settings::getInstance()->m_language;
}

void Settings::setLanguage(const QString &language)
{
    for (auto & translator : Settings::getInstance()->m_translators)
    {
        QCoreApplication::removeTranslator(translator.get());
    }
    Settings::getInstance()->m_translators.clear();
    Settings::getInstance()->m_language = language;

    QStringList searchPaths;
    const QString filename = "translation/lang_" + Settings::getInstance()->m_language + ".qm";
    searchPaths.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/" + filename);
    searchPaths.append(Settings::getUserPath() + "resources/" + filename);
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        searchPaths.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + Settings::getMods().at(i) + "/" + filename);
        searchPaths.append(Settings::getUserPath() + Settings::getMods().at(i) + "/" + filename);
    }
    for (const auto & file : searchPaths)
    {
        if (QFile::exists(file))
        {
            std::shared_ptr<QTranslator> translator = std::make_shared<QTranslator>();
            Settings::getInstance()->m_translators.append(translator);
            if (translator->load(QLocale(Settings::getInstance()->m_language), file))
            {
                CONSOLE_PRINT("Loaded language file " + file + " for language " + Settings::getInstance()->m_language, GameConsole::eDEBUG);
                QCoreApplication::installTranslator(translator.get());
            }
            else
            {
                Settings::getInstance()->m_translators.removeLast();
            }
        }
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (pInterpreter != nullptr)
    {
        pInterpreter->installExtensions(QJSEngine::Extension::AllExtensions);
    }
}

QStringList Settings::getLanguageNames()
{
     QLocale english("en");
     QStringList items = {english.nativeLanguageName()};
     QStringList paths = {QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/translation/", "resources/translation/"};
     for (qint32 i = 0; i < Settings::getMods().size(); i++)
     {
         paths.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + Settings::getMods().at(i) + "/translation");
         paths.append(Settings::getUserPath() + Settings::getMods().at(i) + "/translation");
     }
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
