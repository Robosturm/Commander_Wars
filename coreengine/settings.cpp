#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

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
#include "coreengine/virtualpaths.h"

const char* const Settings::DEFAULT_AUDIODEVICE = "@@default@@";

// this Object
spSettings Settings::m_pInstance;

Settings* Settings::getInstance()
{
    if (m_pInstance.get() == nullptr)
    {
        m_pInstance = MemoryManagement::create<Settings>();
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

bool Settings::getContinueCoMusic() const
{
    return m_continueCoMusic;
}

void Settings::setContinueCoMusic(bool newContinueCoMusic)
{
    m_continueCoMusic = newContinueCoMusic;
}

qint32 Settings::getHeavyAiTrainingStartDay() const
{
    return m_heavyAiTrainingStartDay;
}

void Settings::setHeavyAiTrainingStartDay(qint32 newHeavyAiTrainingStartDay)
{
    m_heavyAiTrainingStartDay = newHeavyAiTrainingStartDay;
}

QString Settings::getHeavyAiTrainingType() const
{
    return m_heavyAiTrainingType;
}

void Settings::setHeavyAiTrainingType(const QString & newHeavyAiTrainingType)
{
    m_heavyAiTrainingType = newHeavyAiTrainingType;
}

QString Settings::getHeavyAiTrainingFile() const
{
    return m_heavyAiTrainingFile;
}

void Settings::setHeavyAiTrainingFile(const QString & newHeavyAiTrainingFile)
{
    m_heavyAiTrainingFile = newHeavyAiTrainingFile;
}

qint32 Settings::getMouseUpdateRate() const
{
    return m_mouseUpdateRate;
}

void Settings::setMouseUpdateRate(qint32 newMouseUpdateRate)
{
    m_mouseUpdateRate = newMouseUpdateRate;
}

std::chrono::seconds Settings::getReplayDeleteTime() const
{
    return m_replayDeleteTime;
}

void Settings::setReplayDeleteTime(const std::chrono::seconds & newReplayDeleteTime)
{
    m_replayDeleteTime = newReplayDeleteTime;
}

bool Settings::getAllowMapUpload() const
{
    return m_allowMapUpload;
}

void Settings::setAllowMapUpload(bool newAllowMapUpload)
{
    m_allowMapUpload = newAllowMapUpload;
}

bool Settings::getCreateAiTrainingData() const
{
    return m_createAiTrainingData;
}

void Settings::setCreateAiTrainingData(bool newCreateAiTrainingData)
{
    m_createAiTrainingData = newCreateAiTrainingData;
}

bool Settings::getMuteOnFcousedLost() const
{
    return m_muteOnFcousedLost;
}

void Settings::setMuteOnFcousedLost(bool newMuteOnFcousedLost)
{
    m_muteOnFcousedLost = newMuteOnFcousedLost;
}

float Settings::getZoomModifier() const
{
    return m_zoomModifier;
}

void Settings::setZoomModifier(float newZoomModifier)
{
    m_zoomModifier = newZoomModifier;
}

float Settings::getGameScale()
{
    return m_gameScale;
}

void Settings::setGameScale(float newGameScale)
{
    m_gameScale = newGameScale;
}

float Settings::getIngameMenuScaling()
{
    return m_ingameMenuScaling;
}

void Settings::setIngameMenuScaling(float newIngameMenuScaling)
{
    m_ingameMenuScaling = newIngameMenuScaling;
}

quint8 Settings::getScreen()
{
    return m_screen;
}

void Settings::setScreen(quint8 newScreen)
{
    m_screen = newScreen;
}

QString Settings::getPipeUuid()
{
    return m_pipeUuid;
}

void Settings::setPipeUuid(const QString & newPipeUuid)
{
    m_pipeUuid = newPipeUuid;
}

bool Settings::getAiSlave()
{
    return m_aiSlave;
}

void Settings::setAiSlave(bool newAiSlave)
{
    m_aiSlave = newAiSlave;
}

bool Settings::getSpawnAiProcess()
{
    return m_spawnAiProcess;
}

void Settings::setSpawnAiProcess(bool newSpawnAiProcess)
{
    m_spawnAiProcess = newSpawnAiProcess;
}

bool Settings::getAutomaticUpdates()
{
    return m_automaticUpdates;
}

void Settings::setAutomaticUpdates(bool newAutomaticUpdates)
{
    m_automaticUpdates = newAutomaticUpdates;
}

QString Settings::getUpdateStep()
{
    return m_updateStep;
}

void Settings::setUpdateStep(const QString &newUpdateStep)
{
    m_updateStep = newUpdateStep;
}

QString Settings::getSecondaryServerAdress()
{
    return m_secondaryServerAdress;
}

void Settings::setSecondaryServerAdress(const QString newSecondaryServerAdress)
{
    m_secondaryServerAdress = newSecondaryServerAdress;
}

QString Settings::getServerSecondaryListenAdress()
{
    return m_serverSecondaryListenAdress;
}

void Settings::setServerSecondaryListenAdress(const QString newServerSecondaryListenAdress)
{
    m_serverSecondaryListenAdress = newServerSecondaryListenAdress;
}

QString Settings::getServerPassword()
{
    return m_serverPassword;
}

void Settings::setServerPassword(const QString newServerPassword)
{
    CONSOLE_PRINT("Changed buffered server login password", GameConsole::eDEBUG);
    m_serverPassword = newServerPassword;
}

QString Settings::getMailServerSendAddress()
{
    return m_mailServerSendAddress;
}

void Settings::setMailServerSendAddress(const QString newMailServerSendAddress)
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

QString Settings::getMailServerAddress()
{
    return m_mailServerAddress;
}

void Settings::setMailServerAddress(const QString newMailServerAddress)
{
    m_mailServerAddress = newMailServerAddress;
}

const std::chrono::seconds Settings::getSuspendedDespawnTime()
{
    return m_suspendedDespawnTime;
}

void Settings::setSuspendedDespawnTime(const std::chrono::seconds newSlaveDespawnTime)
{
    m_suspendedDespawnTime = newSlaveDespawnTime;
}

const std::chrono::seconds Settings::getSlaveDespawnTime()
{
    return m_slaveDespawnTime;
}

void Settings::setSlaveDespawnTime(const std::chrono::seconds newSlaveDespawnTime)
{
    m_slaveDespawnTime = newSlaveDespawnTime;
}

QString Settings::getDefaultBannlist()
{
    return m_defaultBannlist;
}

void Settings::setDefaultBannlist(const QString newDefaultBannlist)
{
    m_defaultBannlist = newDefaultBannlist;
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

QString Settings::getSlaveListenAdress()
{
    return m_slaveListenAdress;
}

void Settings::setSlaveListenAdress(const QString newSlaveListenAdress)
{
    m_slaveListenAdress = newSlaveListenAdress;
}

QString Settings::getServerListenAdress()
{
    return m_serverListenAdress;
}

void Settings::setServerListenAdress(const QString newServerListenAdress)
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

void Settings::setSlaveHostOptions(const QString newSlaveHostOptions)
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

void Settings::setDefaultRuleset(const QString newDefaultRuleset)
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

const QVariant Settings::getAudioOutput()
{
    return m_audioOutput;
}

void Settings::setAudioOutput(const QVariant newAudioOutput)
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

QString Settings::getTempPath()
{
    return m_tempDirPath;
}

QTemporaryDir Settings::newTempDir()
{
    return std::move(QTemporaryDir(getTempPath() + "CoW"));
}

void Settings::clearTemp()
{
    m_tempDirObj.remove();
}

void Settings::setUserPath(const QString newUserPath)
{
    if (newUserPath.isEmpty()) {
        m_userPath = newUserPath;
    } else {
        QString folder = newUserPath + "/";
        while (folder.contains("//"))
        {
            folder = folder.replace("//", "/");
        }
        m_userPath = folder;
    }
    VirtualPaths::setSearchPath(m_userPath, m_activeMods);
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

void Settings::setKey_screenshot(const Qt::Key key_screenshot)
{
    m_key_screenshot = key_screenshot;
}

GameEnums::AutoFocusing Settings::getAutoFocusing()
{
    return m_autoFocusing;
}

void Settings::setAutoFocusing(const GameEnums::AutoFocusing autoFocusing)
{
    m_autoFocusing = autoFocusing;
}

Qt::Key Settings::getKey_EditorSelectionLeft()
{
    return m_key_EditorSelectionLeft;
}

void Settings::setKey_EditorSelectionLeft(const Qt::Key key_EditorSelectionLeft)
{
    m_key_EditorSelectionLeft = key_EditorSelectionLeft;
}

Qt::Key Settings::getKey_EditorSelectionRight()
{
    return m_key_EditorSelectionRight;
}

void Settings::setKey_EditorSelectionRight(const Qt::Key key_EditorSelectionRight)
{
    m_key_EditorSelectionRight = key_EditorSelectionRight;
}

Qt::Key Settings::getKey_EditorPreviousTeam()
{
    return m_key_EditorPreviousTeam;
}

void Settings::setKey_EditorPreviousTeam(const Qt::Key key_EditorPreviousTeam)
{
    m_key_EditorPreviousTeam = key_EditorPreviousTeam;
}

Qt::Key Settings::getKey_EditorNextTeam()
{
    return m_key_EditorNextTeam;
}

void Settings::setKey_EditorNextTeam(const Qt::Key key_EditorNextTeam)
{
    m_key_EditorNextTeam = key_EditorNextTeam;
}

Qt::Key Settings::getKey_EditorPlaceUnit()
{
    return m_key_EditorPlaceUnit;
}

void Settings::setKey_EditorPlaceUnit(const Qt::Key key_EditorPlaceUnit)
{
    m_key_EditorPlaceUnit = key_EditorPlaceUnit;
}

Qt::Key Settings::getKey_EditorPlaceBuilding()
{
    return m_key_EditorPlaceBuilding;
}

void Settings::setKey_EditorPlaceBuilding(const Qt::Key key_EditorPlaceBuilding)
{
    m_key_EditorPlaceBuilding = key_EditorPlaceBuilding;
}

Qt::Key Settings::getKey_EditorPlaceTerrain()
{
    return m_key_EditorPlaceTerrain;
}

void Settings::setKey_EditorPlaceTerrain(const Qt::Key key_EditorPlaceTerrain)
{
    m_key_EditorPlaceTerrain = key_EditorPlaceTerrain;
}

Qt::Key Settings::getKey_ShowIndirectAttackFields2()
{
    return m_key_ShowIndirectAttackFields2;
}

void Settings::setKey_ShowIndirectAttackFields2(const Qt::Key key_ShowIndirectAttackFields2)
{
    m_key_ShowIndirectAttackFields2 = key_ShowIndirectAttackFields2;
}

Qt::Key Settings::getKey_toggleGridLayout()
{
    return m_key_toggleGridLayout;
}

void Settings::setKey_toggleGridLayout(const Qt::Key key_toggleGridLayout)
{
    m_key_toggleGridLayout = key_toggleGridLayout;
}

Qt::Key Settings::getKey_ShowAttackFields2()
{
    return m_key_ShowAttackFields2;
}

void Settings::setKey_ShowAttackFields2(const Qt::Key key_ShowAttackFields2)
{
    m_key_ShowAttackFields2 = key_ShowAttackFields2;
}

Qt::Key Settings::getKey_MapZoomIn2()
{
    return m_key_MapZoomIn2;
}

void Settings::setKey_MapZoomIn2(const Qt::Key key_MapZoomIn2)
{
    m_key_MapZoomIn2 = key_MapZoomIn2;
}

Qt::Key Settings::getKey_MapZoomOut2()
{
    return m_key_MapZoomOut2;
}

void Settings::setKey_MapZoomOut2(const Qt::Key key_MapZoomOut2)
{
    m_key_MapZoomOut2 = key_MapZoomOut2;
}

Qt::Key Settings::getKey_moveMapLeft2()
{
    return m_key_moveMapLeft2;
}

void Settings::setKey_moveMapLeft2(const Qt::Key key_moveMapLeft2)
{
    m_key_moveMapLeft2 = key_moveMapLeft2;
}

Qt::Key Settings::getKey_moveMapRight2()
{
    return m_key_moveMapRight2;
}

void Settings::setKey_moveMapRight2(const Qt::Key key_moveMapRight2)
{
    m_key_moveMapRight2 = key_moveMapRight2;
}

Qt::Key Settings::getKey_moveMapDown2()
{
    return m_key_moveMapDown2;
}

void Settings::setKey_moveMapDown2(const Qt::Key key_moveMapDown2)
{
    m_key_moveMapDown2 = key_moveMapDown2;
}

Qt::Key Settings::getKey_moveMapUp2()
{
    return m_key_moveMapUp2;
}

void Settings::setKey_moveMapUp2(const Qt::Key key_moveMapUp2)
{
    m_key_moveMapUp2 = key_moveMapUp2;
}

Qt::Key Settings::getKey_information2()
{
    return m_key_information2;
}

void Settings::setKey_information2(const Qt::Key key_information2)
{
    m_key_information2 = key_information2;
}

Qt::Key Settings::getKey_previous2()
{
    return m_key_previous2;
}

void Settings::setKey_previous2(const Qt::Key key_previous2)
{
    m_key_previous2 = key_previous2;
}

Qt::Key Settings::getKey_next2()
{
    return m_key_next2;
}

void Settings::setKey_next2(const Qt::Key key_next2)
{
    m_key_next2 = key_next2;
}

Qt::Key Settings::getKey_cancel2()
{
    return m_key_cancel2;
}

void Settings::setKey_cancel2(const Qt::Key key_cancel2)
{
    m_key_cancel2 = key_cancel2;
}

Qt::Key Settings::getKey_confirm2()
{
    return m_key_confirm2;
}

void Settings::setKey_confirm2(const Qt::Key key_confirm2)
{
    m_key_confirm2 = key_confirm2;
}

Qt::Key Settings::getKey_left2()
{
    return m_key_left2;
}

void Settings::setKey_left2(const Qt::Key key_left2)
{
    m_key_left2 = key_left2;
}

Qt::Key Settings::getKey_right2()
{
    return m_key_right2;
}

void Settings::setKey_right2(const Qt::Key key_right2)
{
    m_key_right2 = key_right2;
}

Qt::Key Settings::getKey_down2()
{
    return m_key_down2;
}

void Settings::setKey_down2(const Qt::Key key_down2)
{
    m_key_down2 = key_down2;
}

Qt::Key Settings::getKey_up2()
{
    return m_key_up2;
}

void Settings::setKey_up2(const Qt::Key key_up2)
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

void Settings::setCoInfoPosition(const GameEnums::COInfoPosition value)
{
    m_coInfoPosition = value;
}

void Settings::setY(const qint32 y)
{
    m_y = y;
}

void Settings::setX(const qint32 x)
{
    m_x = x;
}

void Settings::setSlaveServerName(const QString slaveServerName)
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

void Settings::setActiveMods(const QStringList activeMods)
{
    m_activeMods = activeMods;
    qint32 i = 0;
    while (i < m_activeMods.size())
    {
        QDir dir(VirtualPaths::find(m_activeMods[i], false));
        if (!dir.exists())
        {
            CONSOLE_PRINT("Removing mod from active list: " + m_activeMods[i] + " because it wasn't found.", GameConsole::eWARNING);
            m_activeMods.removeAt(i);
        }
        else
        {
            ++i;
        }
    }
    m_activeMods.sort();
    for (const auto & mod : std::as_const(m_activeMods))
    {
        CONSOLE_PRINT("Loaded mod: " + mod, GameConsole::eDEBUG);
        bool found = false;
        QFile file(VirtualPaths::find(mod + "/mod.txt", false));
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
    VirtualPaths::setSearchPath(m_userPath, m_activeMods);
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

void Settings::setWalkAnimationSpeed(const quint32 value)
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

void Settings::setShowCoCount(const qint32 showCoCount)
{
    m_showCoCount = showCoCount;
}

qint32 Settings::getAutoSavingCycle()
{
    return m_autoSavingCycle;
}

void Settings::setAutoSavingCycle(const qint32 value)
{
    m_autoSavingCycle = value;
}

std::chrono::seconds Settings::getAutoSavingCylceTime()
{
    return m_autoSavingCylceTime;
}

void Settings::setAutoSavingCylceTime(const std::chrono::seconds value)
{
    m_autoSavingCylceTime = value;
}

quint64 Settings::getAutoSavingCylceTimeRaw()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(getAutoSavingCylceTime()).count();
}

void Settings::setAutoSavingCylceTimeRaw(const quint32 value)
{
    setAutoSavingCylceTime(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(value)));
}

Qt::Key Settings::getKey_ShowAttackFields()
{
    return m_key_ShowAttackFields;
}

void Settings::setKey_ShowAttackFields(const Qt::Key key_ShowAttackFields)
{
    m_key_ShowAttackFields = key_ShowAttackFields;
}

Qt::Key Settings::getKey_ShowIndirectAttackFields()
{
    return m_key_ShowIndirectAttackFields;
}

void Settings::setKey_ShowIndirectAttackFields(const Qt::Key key_ShowIndirectAttackFields)
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
    m_settingFile = defaultPath + m_settingFile;
    auto devices = QInputDevice::devices();
    bool hasTouch = false;
    for (const auto & device: std::as_const(devices))
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
        MemoryManagement::create<Value<qint32>>("Resolution", "x", &m_x, 0, std::numeric_limits<qint32>::min(), std::numeric_limits<qint32>::max(), true),
        MemoryManagement::create<Value<qint32>>("Resolution", "y", &m_y, 0, std::numeric_limits<qint32>::min(), std::numeric_limits<qint32>::max(), true),
        MemoryManagement::create<Value<qint32>>("Resolution", "width", &m_width, size.width(), 1, size.width(), true),
        MemoryManagement::create<Value<qint32>>("Resolution", "height", &m_height, size.height(), 1, size.height(), true),
        MemoryManagement::create<Value<float>>("Resolution", "brightness", &m_brightness, 0, -50, 50),
        MemoryManagement::create<Value<float>>("Resolution", "gamma", &m_gamma, 1, 0, 50),
        MemoryManagement::create<Value<bool>>("Resolution", "borderless", &m_borderless, true, false, true, true),
        MemoryManagement::create<Value<bool>>("Resolution", "fullscreen", &m_fullscreen, false, false, true, true),
        MemoryManagement::create<Value<quint8>>("Resolution", "screen", &m_screen, 0, 0, 255, true),
        MemoryManagement::create<Value<bool>>("Resolution", "recordgames", &m_record, false, false, true),
        MemoryManagement::create<Value<bool>>("Resolution", "SmallScreenDevice", &m_smallScreenDevice, smallScreenDevice, false, true),
        MemoryManagement::create<Value<float>>("Resolution", "IngameMenuScaling", &m_ingameMenuScaling, 1.0f, 0.5f, 10.0f),
        MemoryManagement::create<Value<float>>("Resolution", "GameScale", &m_gameScale, 1.0f, 0.125f, 16.0f),
        MemoryManagement::create<Value<float>>("Resolution", "ZoomModifier", &m_zoomModifier, 2.0f, 1.1f, 2.0f),

        // general
        MemoryManagement::create<Value<QString>>("General", "language", &m_language, "en", "", ""),
        MemoryManagement::create<Value<float>>("General", "MouseSensitivity", &m_mouseSensitivity, -0.75f, -100, 100),
        MemoryManagement::create<Value<QString>>("General", "UserPath", &m_userPath, defaultPath, "", ""),
        MemoryManagement::create<Value<bool>>("General", "TouchScreen", &m_touchScreen, hasTouch, false, true),
        MemoryManagement::create<Value<qint32>>("General", "TouchPointSensitivity", &m_touchPointSensitivity, 15, 0, size.width()),
        MemoryManagement::create<Value<bool>>("General", "GamepadEnabled", &m_gamepadEnabled, false, false, true),
        MemoryManagement::create<Value<bool>>("General", "AutomaticUpdates", &m_automaticUpdates, true, false, true),
        MemoryManagement::create<Value<float>>("General", "GamepadSensitivity", &m_gamepadSensitivity, 1.0f, 0.1f, 100),
        MemoryManagement::create<Value<qint32>>("General", "MaxFPS", &m_framesPerSecond, 60, 30, 60),
        MemoryManagement::create<Value<qint32>>("General", "MouseUpdateRate", &m_mouseUpdateRate, 33, 1, 200),

        // keys
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_escape", &m_key_escape, Qt::Key_Escape, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_console", &m_key_console, Qt::Key_F1, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_screenshot", &m_key_screenshot, Qt::Key_F5, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_mapshot", &m_key_mapshot, Qt::Key_F6, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_up", &m_key_up, Qt::Key_W, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_down", &m_key_down, Qt::Key_S, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_right", &m_key_right, Qt::Key_D, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_left", &m_key_left, Qt::Key_A, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_confirm", &m_key_confirm, Qt::Key_Space, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_cancel", &m_key_cancel, Qt::Key_B, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_next", &m_key_next, Qt::Key_E, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_previous", &m_key_previous, Qt::Key_Q, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_quicksave1", &m_key_quicksave1, Qt::Key_F9, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_quicksave2", &m_key_quicksave2, Qt::Key_F11, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_quickload1", &m_key_quickload1, Qt::Key_F10, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_quickload2", &m_key_quickload2, Qt::Key_F12, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_information", &m_key_information, Qt::Key_I, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_moveMapUp", &m_key_moveMapUp, Qt::Key_Down, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_moveMapDown", &m_key_moveMapDown, Qt::Key_Up, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_moveMapRight", &m_key_moveMapRight, Qt::Key_Left, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_moveMapLeft", &m_key_moveMapLeft, Qt::Key_Right, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_MapZoomOut", &m_key_MapZoomOut, Qt::Key_Minus, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_MapZoomIn", &m_key_MapZoomIn, Qt::Key_Plus, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_ShowAttackFields", &m_key_ShowAttackFields, Qt::Key_2, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_ShowIndirectAttackFields", &m_key_ShowIndirectAttackFields, Qt::Key_1, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_up2", &m_key_up2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_down2", &m_key_down2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_right2", &m_key_right2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_left2", &m_key_left2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_confirm2", &m_key_confirm2, Qt::Key_Return, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_cancel2", &m_key_cancel2, Qt::Key_Backspace, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_next2", &m_key_next2, Qt::Key_Tab, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_previous2", &m_key_previous2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_information2", &m_key_information2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_moveMapUp2", &m_key_moveMapUp2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_moveMapDown2", &m_key_moveMapDown2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_moveMapRight2", &m_key_moveMapRight2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_moveMapLeft2", &m_key_moveMapLeft2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_MapZoomOut2", &m_key_MapZoomOut2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_MapZoomIn2", &m_key_MapZoomIn2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_ShowAttackFields2", &m_key_ShowAttackFields2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_ShowIndirectAttackFields2", &m_key_ShowIndirectAttackFields2, static_cast<Qt::Key>(0), static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_EditorPlaceTerrain", &m_key_EditorPlaceTerrain, Qt::Key_1, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_EditorPlaceBuilding", &m_key_EditorPlaceBuilding, Qt::Key_2, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_EditorPlaceUnit", &m_key_EditorPlaceUnit, Qt::Key_3, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_EditorNextTeam", &m_key_EditorNextTeam, Qt::Key_Tab, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_EditorPreviousTeam", &m_key_EditorPreviousTeam, Qt::Key_Asterisk, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_EditorSelectionRight", &m_key_EditorSelectionRight, Qt::Key_R, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_EditorSelectionLeft", &m_key_EditorSelectionLeft, Qt::Key_T, static_cast<Qt::Key>(0), Qt::Key_unknown),
        MemoryManagement::create<Value<Qt::Key>>("Keys", "key_toggleGridLayout", &m_key_toggleGridLayout, Qt::Key_G, static_cast<Qt::Key>(0), Qt::Key_unknown),
        // sound
        MemoryManagement::create<Value<qint32>>("Sound", "TotalVolume", &m_TotalVolume, 100, 0, 100),
        MemoryManagement::create<Value<qint32>>("Sound", "MusicVolume", &m_MusicVolume, 80, 0, 100),
        MemoryManagement::create<Value<qint32>>("Sound", "SoundVolume", &m_SoundVolume, 100, 0, 100),
        MemoryManagement::create<Value<bool>>("Sound", "Muted", &m_muted, false, false, true),
        MemoryManagement::create<Value<bool>>("Sound", "MuteOnFocusedLost", &m_muteOnFcousedLost, true, false, true),
        MemoryManagement::create<Value<bool>>("Sound", "ContinueCoMusic", &m_continueCoMusic, false, false, true),

#ifdef AUDIOSUPPORT
        MemoryManagement::create<AudioDeviceValue>("Sound", "AudioDevice", &m_audioOutput, DEFAULT_AUDIODEVICE),
#endif
        // game
        MemoryManagement::create<Value<QString>>("Game", "Username", &m_Username, "", "", "", true),
        MemoryManagement::create<Value<bool>>("Game", "OverworldAnimations", &m_overworldAnimations, true, false, true),
        MemoryManagement::create<Value<GameEnums::BattleAnimationMode>>("Game", "BattleAnimationMode", &m_battleAnimationsMode, GameEnums::BattleAnimationMode_All, GameEnums::BattleAnimationMode_None, GameEnums::BattleAnimationMode_Enemy),
        MemoryManagement::create<Value<GameEnums::BattleAnimationType>>("Game", "BattleAnimationType", &m_battleAnimationType, GameEnums::BattleAnimationType_Detail, GameEnums::BattleAnimationType_Detail, GameEnums::BattleAnimationType_FullscreenTransparent),
        MemoryManagement::create<Value<quint32>>("Game", "AnimationSpeed", &m_animationSpeed, 1, 1, 100),
        MemoryManagement::create<Value<quint32>>("Game", "BattleAnimationSpeed", &m_battleAnimationSpeed, 1, 1, 100),
        MemoryManagement::create<Value<quint32>>("Game", "WalkAnimationSpeed", &m_walkAnimationSpeed, 20, 1, 100),
        MemoryManagement::create<Value<quint32>>("Game", "DialogAnimationSpeed", &m_dialogAnimationSpeed, 1, 1, 100),
        MemoryManagement::create<Value<quint32>>("Game", "CaptureAnimationSpeed", &m_captureAnimationSpeed, 1, 1, 100),
        MemoryManagement::create<Value<quint32>>("Game", "MultiTurnCounter", &multiTurnCounter, 1, 1, 10),
        MemoryManagement::create<Value<qint32>>("Game", "MenuItemCount", &m_MenuItemCount, 13, 5, std::numeric_limits<qint32>::max()),
        MemoryManagement::create<Value<qint32>>("Game", "MenuItemRowCount", &m_MenuItemRowCount, 2, 1, std::numeric_limits<qint32>::max()),
        MemoryManagement::create<Value<float>>("Game", "SupplyWarning", &m_supplyWarning, 0.33f, 0.0f, 1.0f),
        MemoryManagement::create<Value<bool>>("Game", "StaticMarkedFields", &m_StaticMarkedFields, false, false, true),
        MemoryManagement::create<Value<qint32>>("Game", "ShowCoCount", &m_showCoCount, defaultCoCount, 0, std::numeric_limits<qint32>::max()),
        MemoryManagement::create<Value<bool>>("Game", "DialogAnimation", &m_dialogAnimation, true, false, true),
        MemoryManagement::create<Value<bool>>("Game", "CaptureAnimation", &m_captureAnimation, true, false, true),
        MemoryManagement::create<Value<bool>>("Game", "Day2DayScreen", &m_day2dayScreen, true, false, true),
        MemoryManagement::create<Value<bool>>("Game", "MovementAnimations", &m_movementAnimations, true, false, true),
        MemoryManagement::create<Value<QString>>("Game", "LastSaveGame", &m_LastSaveGame, "", "", ""),
        MemoryManagement::create<Value<QString>>("Game", "DefaultRuleset", &m_defaultRuleset, "", "", ""),
        MemoryManagement::create<Value<QString>>("Game", "DefaultBannlist", &m_defaultBannlist, "", "", ""),
        MemoryManagement::create<Value<bool>>("Game", "ShowCursor", &m_ShowCursor, true, false, true),
        MemoryManagement::create<Value<bool>>("Game", "AutoEndTurn", &m_AutoEndTurn, false, false, true),
        MemoryManagement::create<Value<bool>>("Game", "AutoCamera", &m_autoCamera, true, false, true),
        MemoryManagement::create<Value<bool>>("Game", "ShowIngameCoordinates", &m_showIngameCoordinates, true, false, true),
        MemoryManagement::create<Value<bool>>("Game", "CenterOnSelection", &m_centerOnMarkedField, true, false, true),
        MemoryManagement::create<Value<bool>>("Game", "SyncAnimations", &m_syncAnimations, false, false, true),
        MemoryManagement::create<Value<bool>>("Game", "AutoMoveCursor", &m_autoMoveCursor, true, false, true),
        MemoryManagement::create<Value<bool>>("Game", "UseCoMinis", &m_useCoMinis, true, false, true),
        MemoryManagement::create<Value<bool>>("Game", "AutoScrolling", &m_autoScrolling, true, false, true),
        MemoryManagement::create<Value<bool>>("Game", "ShowDetailedBattleForcast", &m_showDetailedBattleForcast, true, false, true),
        MemoryManagement::create<Value<bool>>("Game", "SimpleDeselect", &m_simpleDeselect, false, false, true),
        MemoryManagement::create<Value<GameEnums::COInfoPosition>>("Game", "COInfoPosition", &m_coInfoPosition, GameEnums::COInfoPosition_Flipping, GameEnums::COInfoPosition_Flipping, GameEnums::COInfoPosition_Right),
        MemoryManagement::create<Value<GameEnums::AutoFocusing>>("Game", "AutoFocusing", &m_autoFocusing, GameEnums::AutoFocusing_LastPos, GameEnums::AutoFocusing_LastPos, GameEnums::AutoFocusing_Owned),
        MemoryManagement::create<Value<qint32>>("Game", "PauseAfterAction", &m_pauseAfterAction, 0, 0, 100),
        MemoryManagement::create<Value<QString>>("Game", "AiPipeUuid", &m_pipeUuid, "", "", ""),
        MemoryManagement::create<Value<bool>>("Game", "UseAiProcess", &m_spawnAiProcess, false, false, true),

        // network
        MemoryManagement::create<Value<quint16>>("Network", "GamePort", &m_GamePort, 9001, 0, std::numeric_limits<quint16>::max()),
        MemoryManagement::create<Value<QString>>("Network", "ServerAdress", &m_ServerAdress, "192.46.216.113", "", ""),
        MemoryManagement::create<Value<QString>>("Network", "SecondaryServerAdress", &m_secondaryServerAdress, "2600:3c00::f03c:93ff:fe86:009e", "", ""),
        MemoryManagement::create<Value<quint16>>("Network", "SlaveServerPort", &m_slaveServerPort, 9003, 0, std::numeric_limits<quint16>::max()),
        MemoryManagement::create<Value<quint16>>("Network", "ServerPort", &m_ServerPort, 9002, 0, std::numeric_limits<quint16>::max()),
        MemoryManagement::create<Value<bool>>("Network", "Server", &m_Server, false, false, true),
        MemoryManagement::create<Value<bool>>("Network", "AllowMapUpload", &m_allowMapUpload, true, false, true),
        MemoryManagement::create<Value<QString>>("Network", "ServerListenAdress", &m_serverListenAdress, "", "", ""),
        MemoryManagement::create<Value<QString>>("Network", "ServerSecondaryListenAdress", &m_serverSecondaryListenAdress, "", "", ""),
        MemoryManagement::create<Value<QString>>("Network", "SlaveListenAdress", &m_slaveListenAdress, "", "", ""),
        MemoryManagement::create<Value<QString>>("Network", "SlaveHostOptions", &m_slaveHostOptions, "::1&&10000&20000;::1&&50000&65535", "", ""),
        MemoryManagement::create<Value<std::chrono::seconds>>("Network", "SlaveDespawnTime", &m_slaveDespawnTime, std::chrono::seconds(60 * 60 * 24), std::chrono::seconds(1), std::chrono::seconds(60 * 60 * 24 * 96)),
        MemoryManagement::create<Value<std::chrono::seconds>>("Network", "SuspendedDespawnTime", &m_suspendedDespawnTime, std::chrono::seconds(60 * 60 * 24), std::chrono::seconds(1), std::chrono::seconds(60 * 60 * 24 * 96)),
        MemoryManagement::create<Value<std::chrono::seconds>>("Network", "ReplayDeleteTime", &m_replayDeleteTime, std::chrono::seconds(60 * 60 * 24 * 7), std::chrono::seconds(1), std::chrono::seconds(60 * 60 * 24 * 96)),
        // mailing
        MemoryManagement::create<Value<QString>>("Mailing", "MailServerAddress", &m_mailServerAddress, "", "", ""),
        MemoryManagement::create<Value<quint16>>("Mailing", "MailServerPort", &m_mailServerPort, 0, 0, std::numeric_limits<quint16>::max()),
        MemoryManagement::create<Value<qint32>>("Mailing", "MailServerConnectionType", &m_mailServerConnectionType, 2, 0, 2),
        MemoryManagement::create<Value<QString>>("Mailing", "MailServerUsername", &m_mailServerUsername, "", "", ""),
        MemoryManagement::create<Value<qint32>>("Mailing", "MailServerAuthMethod", &m_mailServerAuthMethod, 1, 0, 1),
        MemoryManagement::create<Value<QString>>("Mailing", "MailServerSendAddress", &m_mailServerSendAddress, "", "", ""),

        // auto saving
        MemoryManagement::create<Value<std::chrono::seconds>>("Autosaving", "AutoSavingTime", &m_autoSavingCylceTime, std::chrono::seconds(60 * 5), std::chrono::seconds(0), std::chrono::seconds(60 * 60 * 24)),
        MemoryManagement::create<Value<qint32>>("Autosaving", "AutoSavingCycle", &m_autoSavingCycle, 3, 0, 100),
        // mods
        MemoryManagement::create<Value<QStringList>>("Mods", "Mods", &m_activeMods, QStringList(), QStringList(), QStringList()),
        // logging
        MemoryManagement::create<Value<bool>>("Logging", "LogActions", &m_LogActions, false, false, true),
        MemoryManagement::create<Value<GameConsole::eLogLevels>>("Logging", "LogLevel", &m_defaultLogLevel, static_cast<GameConsole::eLogLevels>(DEBUG_LEVEL), GameConsole::eLogLevels::eOFF, GameConsole::eLogLevels::eFATAL),
        MemoryManagement::create<Value<quint64>>("Logging", "LogModules", &m_defaultLogModuls, GameConsole::eGeneral | GameConsole::eJavaScript, 0, std::numeric_limits<quint64>::max()),
        MemoryManagement::create<Value<bool>>("Logging", "CreateAiTrainingData", &m_createAiTrainingData, false, false, true),
        // heavy ai training
        MemoryManagement::create<Value<QString>>("AiTraining", "HeavyAiTrainingFile", &m_heavyAiTrainingFile, "", "", ""),
        MemoryManagement::create<Value<QString>>("AiTraining", "HeavyAiTrainingType", &m_heavyAiTrainingType, "heavy_ai", "", ""),
        MemoryManagement::create<Value<qint32>>("AiTraining", "HeavyAiTrainingStartDay", &m_heavyAiTrainingStartDay, 5, 1, 99),

    };
    QSettings settings(m_settingFile, QSettings::IniFormat);
    for (auto setting : m_SettingValues)
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
    QSettings settings(m_settingFile, QSettings::IniFormat);
    for (auto setting : m_SettingValues)
    {
        setting->readValue(settings);
    }
    setUserPath(m_userPath);
    setFramesPerSecond(m_framesPerSecond);
    setActiveMods(m_activeMods);
    GameConsole::setLogLevel(m_defaultLogLevel);
    GameConsole::setActiveModules(m_defaultLogModuls);
    if (Settings::hasSmallScreen())
    {
        m_autoScrolling = false;
        m_simpleDeselect = true;
        m_showDetailedBattleForcast = false;
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
    for (auto setting : m_SettingValues)
    {
        setting->resetValue();
    }
    saveSettings();
}

void Settings::saveSettings()
{
    CONSOLE_PRINT("Settings::saveSettings()", GameConsole::eDEBUG);
    Mainapp* pApp = Mainapp::getInstance();
    if (!pApp->getSlave() && !Settings::getAiSlave() && m_updateStep.isEmpty())
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

void Settings::filterCosmeticMods(QStringList mods, QStringList versions, bool filter)
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

void Settings::setWidth(const qint32 width)
{
    m_width = width;
}

void Settings::setHeight(const qint32 height)
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

void Settings::setAnimationSpeed(const quint32 value)
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

void Settings::setBattleAnimationSpeed(const quint32 value)
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

void Settings::setDialogAnimationSpeed(const quint32 value)
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

void Settings::setCaptureAnimationSpeed(const quint32 value)
{
    m_captureAnimationSpeed = value;
}

Qt::Key Settings::getKey_up()
{
    return m_key_up;
}

void Settings::setKey_up(const Qt::Key key_up)
{
    m_key_up = key_up;
}

Qt::Key Settings::getKey_down()
{
    return m_key_down;
}

void Settings::setKey_down(const Qt::Key key_down)
{
    m_key_down = key_down;
}

Qt::Key Settings::getKey_right()
{
    return m_key_right;
}

void Settings::setKey_right(const Qt::Key key_right)
{
    m_key_right = key_right;
}

Qt::Key Settings::getKey_left()
{
    return m_key_left;
}

void Settings::setKey_left(const Qt::Key key_left)
{
    m_key_left = key_left;
}

Qt::Key Settings::getKey_confirm()
{
    return m_key_confirm;
}

void Settings::setKey_confirm(const Qt::Key key_confirm)
{
    m_key_confirm = key_confirm;
}

Qt::Key Settings::getKey_cancel()
{
    return m_key_cancel;
}

void Settings::setKey_cancel(const Qt::Key key_cancel)
{
    m_key_cancel = key_cancel;
}

Qt::Key Settings::getKey_next()
{
    return m_key_next;
}

void Settings::setKey_next(const Qt::Key key_next)
{
    m_key_next = key_next;
}

Qt::Key Settings::getKey_previous()
{
    return m_key_previous;
}

void Settings::setKey_previous(const Qt::Key key_previous)
{
    m_key_previous = key_previous;
}

Qt::Key Settings::getKey_quicksave1()
{
    return m_key_quicksave1;
}

void Settings::setKey_quicksave1(const Qt::Key key_quicksave1)
{
    m_key_quicksave1 = key_quicksave1;
}

Qt::Key Settings::getKey_quicksave2()
{
    return m_key_quicksave2;
}

void Settings::setKey_quicksave2(const Qt::Key key_quicksave2)
{
    m_key_quicksave2 = key_quicksave2;
}

Qt::Key Settings::getKey_quickload1()
{
    return m_key_quickload1;
}

void Settings::setKey_quickload1(const Qt::Key key_quickload1)
{
    m_key_quickload1 = key_quickload1;
}

Qt::Key Settings::getKey_quickload2()
{
    return m_key_quickload2;
}

void Settings::setKey_quickload2(const Qt::Key key_quickload2)
{
    m_key_quickload2 = key_quickload2;
}

Qt::Key Settings::getKey_information()
{
    return m_key_information;
}

void Settings::setKey_information(const Qt::Key key_information)
{
    m_key_information = key_information;
}

Qt::Key Settings::getKey_MapZoomIn()
{
    return m_key_MapZoomIn;
}

void Settings::setKey_MapZoomIn(const Qt::Key key_MapZoomIn)
{
    m_key_MapZoomIn = key_MapZoomIn;
}

Qt::Key Settings::getKey_MapZoomOut()
{
    return m_key_MapZoomOut;
}

void Settings::setKey_MapZoomOut(const Qt::Key key_MapZoomOut)
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

Qt::Key Settings::getKey_Escape()
{
    return m_key_escape;
}

void Settings::setKey_Escape(const Qt::Key key_Escape)
{
    m_key_escape = key_Escape;
}

qint32 Settings::getMenuItemCount()
{
    return m_MenuItemCount;
}

void Settings::setMenuItemCount(const qint32 MenuItemCount)
{
    m_MenuItemCount = MenuItemCount;
}

quint32 Settings::getMultiTurnCounter()
{
    return multiTurnCounter;
}

void Settings::setMultiTurnCounter(const quint32 value)
{
    multiTurnCounter = value;
}

GameEnums::BattleAnimationType Settings::getBattleAnimationType()
{
    return m_battleAnimationType;
}

void Settings::setBattleAnimationType(const GameEnums::BattleAnimationType value)
{
    m_battleAnimationType = value;
}

Qt::Key Settings::getKey_moveMapLeft()
{
    return m_key_moveMapLeft;
}

void Settings::setKey_moveMapLeft(const Qt::Key key_moveMapLeft)
{
    m_key_moveMapLeft = key_moveMapLeft;
}

Qt::Key Settings::getKey_moveMapRight()
{
    return m_key_moveMapRight;
}

void Settings::setKey_moveMapRight(const Qt::Key key_moveMapRight)
{
    m_key_moveMapRight = key_moveMapRight;
}

Qt::Key Settings::getKey_moveMapDown()
{
    return m_key_moveMapDown;
}

void Settings::setKey_moveMapDown(const Qt::Key key_moveMapDown)
{
    m_key_moveMapDown = key_moveMapDown;
}

Qt::Key Settings::getKey_moveMapUp()
{
    return m_key_moveMapUp;
}

void Settings::setKey_moveMapUp(const Qt::Key key_moveMapUp)
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

void Settings::setUsername(const QString Username)
{
    if (Username.length() < 50)
    {
        m_Username = Username;
    }
    else
    {
        m_Username = "";

    }
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

void Settings::setServerAdress(const QString ServerAdress)
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

void Settings::setServerPort(const quint16 ServerPort)
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


QString Settings::getModVersion(QString mod)
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
    return version;
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
    QFile file(VirtualPaths::find(mod + "/mod.txt", false));
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
    QFileInfoList infoList;
    auto searchPath = VirtualPaths::findAllRev("mods", false);
    for (const auto & entry : searchPath)
    {
        if (QFile::exists(entry))
        {
            infoList.append(QDir(entry).entryInfoList(QDir::Dirs));
        }
    }
    QStringList mods;
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

qint32 Settings::getStageWidth()
{
    return oxygine::Stage::getStage()->getWidth();
}

qint32 Settings::getStageHeight()
{
    return oxygine::Stage::getStage()->getHeight();
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
    return m_language;
}

void Settings::setLanguage(const QString language)
{
    for (auto & translator : m_translators)
    {
        QCoreApplication::removeTranslator(translator.get());
    }
    m_translators.clear();
    m_language = language;

    QStringList searchPaths = VirtualPaths::findAll("resources/translation/lang_" + m_language + ".qm");
    for (const auto & file : searchPaths)
    {
        auto translator = MemoryManagement::createNamedQObject<QTranslator>("QTranslator");
        m_translators.append(translator);
        if (translator->load(QLocale(m_language), file))
        {
            CONSOLE_PRINT("Loaded language file " + file + " for language " + m_language, GameConsole::eDEBUG);
            QCoreApplication::installTranslator(translator.get());
        }
        else
        {
            m_translators.removeLast();
        }
    }
}

QStringList Settings::getLanguageNames()
{
     QLocale english("en");
     QStringList items = {english.nativeLanguageName()};
     QStringList paths = VirtualPaths::createSearchPath("resources/translation/");
     QStringList filter;
     filter << "*.qm";
     for (const QString & path : std::as_const(paths))
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
     QStringList paths = VirtualPaths::createSearchPath("resources/translation/");
     QStringList filter;
     filter << "*.qm";
     for (const QString & path : std::as_const(paths))
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
     QStringList paths = VirtualPaths::createSearchPath("resources/translation/");
     QStringList filter;
     filter << "*.qm";
     qint32 i = 0;
     for (const QString & path : std::as_const(paths))
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
