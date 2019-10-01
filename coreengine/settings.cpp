#include "coreengine/settings.h"
#include "coreengine/console.h"
#include "coreengine/mainapp.h"

#include <QSettings>
#include <QTranslator>
#include <QLocale>

const QString Settings::m_settingFile = "Commander_Wars.ini";
float Settings::m_mouseSensitivity   = -0.75f;
qint32 Settings::m_x                 = 0;
qint32 Settings::m_y                 = 0;
qint32 Settings::m_width             = 1024;
qint32 Settings::m_height            = 800;
bool Settings::m_borderless       = false;
bool Settings::m_fullscreen       = false;
SDL_Keycode Settings::m_key_escape      = SDLK_ESCAPE;
SDL_Keycode Settings::m_key_console     = SDLK_F1;
SDL_Keycode Settings::m_key_up          = SDLK_w;
SDL_Keycode Settings::m_key_down        = SDLK_s;
SDL_Keycode Settings::m_key_right       = SDLK_d;
SDL_Keycode Settings::m_key_left        = SDLK_a;
SDL_Keycode Settings::m_key_confirm     = SDLK_SPACE;
SDL_Keycode Settings::m_key_cancel      = SDLK_b;
SDL_Keycode Settings::m_key_next        = SDLK_e;
SDL_Keycode Settings::m_key_previous    = SDLK_q;
SDL_Keycode Settings::m_key_quicksave1  = SDLK_F9;
SDL_Keycode Settings::m_key_quicksave2  = SDLK_F11;
SDL_Keycode Settings::m_key_quickload1  = SDLK_F10;
SDL_Keycode Settings::m_key_quickload2  = SDLK_F12;
SDL_Keycode Settings::m_key_information = SDLK_i;
SDL_Keycode Settings::m_key_moveMapUp   = SDLK_UP;
SDL_Keycode Settings::m_key_moveMapDown = SDLK_DOWN;
SDL_Keycode Settings::m_key_moveMapRight= SDLK_RIGHT;
SDL_Keycode Settings::m_key_moveMapLeft = SDLK_LEFT;

QString Settings::m_language      = "en";
// Sound
qint32 Settings::m_TotalVolume       = 100;
qint32 Settings::m_MusicVolume       = 80;
qint32 Settings::m_SoundVolume       = 100;
// Network
quint16 Settings::m_GamePort          = 9001;
quint16 Settings::m_ServerPort        = 9002;
QString Settings::m_ServerAdress      = "";
bool Settings::m_Server               = false;

// ingame options
GameEnums::AnimationMode Settings::showAnimations = GameEnums::AnimationMode_All;
GameEnums::BattleAnimationMode Settings::battleAnimations = GameEnums::BattleAnimationMode_Detail;
quint32 Settings::animationSpeed = 1;
quint32 Settings::battleAnimationSpeed = 1;
quint32 Settings::multiTurnCounter = 4;
QString Settings::m_LastSaveGame = "";
QString Settings::m_Username = "";
bool Settings::m_ShowCursor = true;
bool Settings::m_AutoEndTurn = false;

// add mod path
QStringList Settings::m_activeMods;
// this Object
Settings* Settings::m_pInstance = nullptr;

Settings::Settings()
{
    Interpreter::setCppOwnerShip(this);
    loadSettings();
}

void Settings::loadSettings(){
    bool ok = false;
    QSettings settings(m_settingFile, QSettings::IniFormat);

    settings.beginGroup("general");
    m_language    = settings.value("language","en").toString();
    m_mouseSensitivity           = settings.value("MouseSensitivity",-0.75f).toFloat(&ok);
    if(!ok){
        QString error = tr("Error in the Ini File: ") + "[General] " + tr("Setting:") + " MouseSensitivity";
        Console::print(error, Console::eERROR);
        m_mouseSensitivity = -0.75f;
    }
    settings.endGroup();

    // Resolution
    settings.beginGroup("Resolution");
    m_x           = settings.value("x",50).toInt(&ok);
    if(!ok){
        QString error = tr("Error in the Ini File: ") + "[Resolution] " + tr("Setting:") + " x";
        Console::print(error, Console::eERROR);
        m_x = 50;
    }
    m_y           = settings.value("y",50).toInt(&ok);
    if(!ok){
        QString error = tr("Error in the Ini File: ") + "[Resolution] " + tr("Setting:") + " y";
        Console::print(error, Console::eERROR);
        m_y = 50;
    }
    m_width       = settings.value("width", 1152).toInt(&ok);
    if(!ok){
        QString error = tr("Error in the Ini File: ") + "[Resolution] " + tr("Setting:") + " width";
        Console::print(error, Console::eERROR);
        m_width = 1152;
    }
    m_height      = settings.value("height", 864).toInt(&ok);
    if(!ok){
        QString error = tr("Error in the Ini File: ") + "[Resolution] " + tr("Setting:") + " heigth";
        Console::print(error, Console::eERROR);
        m_height = 864;
    }
    m_borderless  = settings.value("borderless",false).toBool();
    m_fullscreen  = settings.value("fullscreen",false).toBool();
    settings.endGroup();

    // Keys
    settings.beginGroup("Keys");
    m_key_escape      = static_cast<SDL_Keycode>(settings.value("key_escape", SDLK_ESCAPE).toInt(&ok));
    if(!ok){
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_escape";
        Console::print(error, Console::eERROR);
        m_key_escape = SDLK_ESCAPE;
    }
    m_key_console      = static_cast<SDL_Keycode>(settings.value("key_console", SDLK_F1).toInt(&ok));
    if(!ok){
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_console";
        Console::print(error, Console::eERROR);
        m_key_console = SDLK_F1;
    }
    m_key_up = static_cast<SDL_Keycode>(settings.value("key_up", SDLK_w).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_up";
        Console::print(error, Console::eERROR);
        m_key_up = SDLK_w;
    }
    m_key_down = static_cast<SDL_Keycode>(settings.value("key_down", SDLK_s).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_down";
        Console::print(error, Console::eERROR);
        m_key_down = SDLK_s;
    }
    m_key_right = static_cast<SDL_Keycode>(settings.value("key_right", SDLK_d).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_right";
        Console::print(error, Console::eERROR);
        m_key_right = SDLK_d;
    }
    m_key_left = static_cast<SDL_Keycode>(settings.value("key_left", SDLK_a).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_left";
        Console::print(error, Console::eERROR);
        m_key_left = SDLK_a;
    }
    m_key_confirm = static_cast<SDL_Keycode>(settings.value("key_confirm", SDLK_SPACE).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_confirm";
        Console::print(error, Console::eERROR);
        m_key_confirm = SDLK_SPACE;
    }
    m_key_cancel = static_cast<SDL_Keycode>(settings.value("key_cancel", SDLK_b).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_cancel";
        Console::print(error, Console::eERROR);
        m_key_cancel = SDLK_b;
    }
    m_key_next = static_cast<SDL_Keycode>(settings.value("key_next", SDLK_e).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_next";
        Console::print(error, Console::eERROR);
        m_key_next = SDLK_e;
    }
    m_key_previous = static_cast<SDL_Keycode>(settings.value("key_previous", SDLK_q).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_previous";
        Console::print(error, Console::eERROR);
        m_key_previous = SDLK_q;
    }
    m_key_quicksave1 = static_cast<SDL_Keycode>(settings.value("key_quicksave1", SDLK_F9).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_quicksave1";
        Console::print(error, Console::eERROR);
        m_key_quicksave1 = SDLK_F9;
    }
    m_key_quicksave2 = static_cast<SDL_Keycode>(settings.value("key_quicksave2", SDLK_F11).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_quicksave2";
        Console::print(error, Console::eERROR);
        m_key_quicksave2 = SDLK_F11;
    }
    m_key_quickload1 = static_cast<SDL_Keycode>(settings.value("key_quickload1", SDLK_F10).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_quickload1";
        Console::print(error, Console::eERROR);
        m_key_quickload1 = SDLK_F10;
    }
    m_key_quickload2 = static_cast<SDL_Keycode>(settings.value("key_quickload2", SDLK_F12).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_quickload2";
        Console::print(error, Console::eERROR);
        m_key_quickload2 = SDLK_F12;
    }
    m_key_information = static_cast<SDL_Keycode>(settings.value("key_information", SDLK_i).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_information";
        Console::print(error, Console::eERROR);
        m_key_information = SDLK_i;
    }
    m_key_moveMapUp = static_cast<SDL_Keycode>(settings.value("key_moveMapUp", SDLK_UP).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_moveMapUp";
        Console::print(error, Console::eERROR);
        m_key_moveMapUp = SDLK_UP;
    }
    m_key_moveMapDown = static_cast<SDL_Keycode>(settings.value("key_moveMapDown", SDLK_DOWN).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_moveMapDown";
        Console::print(error, Console::eERROR);
        m_key_moveMapDown = SDLK_DOWN;
    }
    m_key_moveMapRight = static_cast<SDL_Keycode>(settings.value("key_moveMapRight", SDLK_RIGHT).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_moveMapRight";
        Console::print(error, Console::eERROR);
        m_key_moveMapRight = SDLK_RIGHT;
    }
    m_key_moveMapLeft = static_cast<SDL_Keycode>(settings.value("key_moveMapLeft", SDLK_LEFT).toInt(&ok));
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Key] " + tr("Setting:") + " key_moveMapLeft";
        Console::print(error, Console::eERROR);
        m_key_moveMapLeft = SDLK_LEFT;
    }
    settings.endGroup();

    // Sound
    settings.beginGroup("Sound");
    m_TotalVolume      = settings.value("TotalVolume", 100).toInt(&ok);
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Sound] " + tr("Setting:") + " TotalVolume";
        Console::print(error, Console::eERROR);
        m_TotalVolume = 100;
    }
    m_MusicVolume      = settings.value("MusicVolume", 80).toInt(&ok);
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Sound] " + tr("Setting:") + " MusicVolume";
        Console::print(error, Console::eERROR);
        m_MusicVolume = 80;
    }
    m_SoundVolume      = settings.value("SoundVolume", 100).toInt(&ok);
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Sound] " + tr("Setting:") + " SoundVolume";
        Console::print(error, Console::eERROR);
        m_SoundVolume = 100;
    }

    settings.endGroup();

    settings.beginGroup("Game");
    showAnimations  = static_cast<GameEnums::AnimationMode>(settings.value("ShowAnimations", 1).toInt(&ok));
    if (!ok || showAnimations < GameEnums::AnimationMode_None || showAnimations > GameEnums::AnimationMode_OnlyBattleEnemy)
    {
        QString error = tr("Error in the Ini File: ") + "[Game] " + tr("Setting:") + " ShowAnimations";
        Console::print(error, Console::eERROR);
        animationSpeed = GameEnums::AnimationMode_All;
    }
    battleAnimations  = static_cast<GameEnums::BattleAnimationMode>(settings.value("BattleAnimations", 0).toInt(&ok));
    if (!ok || battleAnimations < GameEnums::BattleAnimationMode_Detail || battleAnimations > GameEnums::BattleAnimationMode_Overworld)
    {
        QString error = tr("Error in the Ini File: ") + "[Game] " + tr("Setting:") + " BattleAnimations";
        Console::print(error, Console::eERROR);
        battleAnimations = GameEnums::BattleAnimationMode_Detail;
    }
    animationSpeed = settings.value("AnimationSpeed", 1u).toUInt(&ok);
    if(!ok || animationSpeed <= 0 ||  animationSpeed > 100u)
    {
        QString error = tr("Error in the Ini File: ") + "[Game] " + tr("Setting:") + " AnimationSpeed";
        Console::print(error, Console::eERROR);
        animationSpeed = 1u;
    }
    battleAnimationSpeed = settings.value("BattleAnimationSpeed", 1u).toUInt(&ok);
    if(!ok || animationSpeed <= 0 ||  animationSpeed > 100u)
    {
        QString error = tr("Error in the Ini File: ") + "[Game] " + tr("Setting:") + " BattleAnimationSpeed";
        Console::print(error, Console::eERROR);
        battleAnimationSpeed = 1u;
    }
    multiTurnCounter = settings.value("MultiTurnCounter", 4u).toUInt(&ok);
    if(!ok || animationSpeed <= 0 || multiTurnCounter > 10u)
    {
        QString error = tr("Error in the Ini File: ") + "[Game] " + tr("Setting:") + " MultiTurnCounter";
        Console::print(error, Console::eERROR);
        battleAnimationSpeed = 4u;
    }

    m_LastSaveGame = settings.value("LastSaveGame", "").toString();
    m_Username = settings.value("Username", "").toString();
    m_ShowCursor = settings.value("ShowCursor", true).toBool();
    m_AutoEndTurn = settings.value("AutoEndTurn", false).toBool();
    settings.endGroup();

    // network
    settings.beginGroup("Network");
    m_ServerAdress = settings.value("ServerAdress", "").toString();
    m_GamePort = settings.value("GamePort", 9001).toUInt(&ok);
    if (!ok)
    {
        m_GamePort = 9001;
    }
    m_ServerPort = settings.value("ServerPort", 9002).toUInt(&ok);
    if (!ok)
    {
        m_GamePort = 9002;
    }
    m_Server  = settings.value("Server", false).toBool();
    settings.endGroup();

    // mods
    settings.beginGroup("Mods");
    QString modList = settings.value("Mods", "").toString();
    if (!modList.isEmpty())
    {
        m_activeMods = modList.split(",");
    }
    m_activeMods.sort();
    settings.endGroup();
}

void Settings::saveSettings(){
    QSettings settings(m_settingFile, QSettings::IniFormat);

    settings.beginGroup("general");
    settings.setValue("language",                   m_language);
    settings.setValue("MouseSensitivity",           m_mouseSensitivity);
    settings.endGroup();

    settings.beginGroup("Resolution");
    settings.setValue("x",                          m_x);
    settings.setValue("y",                          m_y);
    settings.setValue("width",                      m_width);
    settings.setValue("height",                     m_height);
    settings.setValue("borderless",                 m_borderless);
    settings.setValue("fullscreen",                 m_fullscreen);
    settings.endGroup();

    settings.beginGroup("Keys");
    settings.setValue("key_escape",                 m_key_escape);
    settings.setValue("key_console",                m_key_console);
    settings.setValue("key_up",                     m_key_up);
    settings.setValue("key_down",                   m_key_down);
    settings.setValue("key_right",                  m_key_right);
    settings.setValue("key_left",                   m_key_left);
    settings.setValue("key_confirm",                m_key_confirm);
    settings.setValue("key_cancel",                 m_key_cancel);
    settings.setValue("key_next",                   m_key_next);
    settings.setValue("key_previous",               m_key_previous);
    settings.setValue("key_quicksave1",             m_key_quicksave1);
    settings.setValue("key_quicksave2",             m_key_quicksave2);
    settings.setValue("key_quickload1",             m_key_quickload1);
    settings.setValue("key_quickload2",             m_key_quickload2);
    settings.setValue("key_information",            m_key_information);
    settings.setValue("key_moveMapUp",              m_key_moveMapUp);
    settings.setValue("key_moveMapDown",            m_key_moveMapDown);
    settings.setValue("key_moveMapRight",           m_key_moveMapRight);
    settings.setValue("key_moveMapLeft",            m_key_moveMapLeft);

    settings.endGroup();

    // Sound
    settings.beginGroup("Sound");
    settings.setValue("TotalVolume",               m_TotalVolume);
    settings.setValue("MusicVolume",               m_MusicVolume);
    settings.setValue("SoundVolume",               m_SoundVolume);
    settings.endGroup();

    settings.beginGroup("Game");
    settings.setValue("ShowAnimations",                 static_cast<qint32>(showAnimations));
    settings.setValue("BattleAnimations",               static_cast<qint32>(battleAnimations));
    settings.setValue("BattleAnimationSpeed",           static_cast<qint32>(battleAnimationSpeed));
    settings.setValue("AnimationSpeed",                 animationSpeed);
    settings.setValue("MultiTurnCounter",               multiTurnCounter);
    settings.setValue("LastSaveGame",                   m_LastSaveGame);
    settings.setValue("Username",                       m_Username);
    settings.setValue("ShowCursor",                     m_ShowCursor);
    settings.setValue("AutoEndTurn",                    m_AutoEndTurn);
    settings.endGroup();

    // network
    settings.beginGroup("Network");
    settings.setValue("ServerAdress",              m_ServerAdress);
    settings.setValue("GamePort",                  m_GamePort);
    settings.setValue("ServerPort",                m_ServerPort);
    settings.setValue("Server",                    m_Server);
    settings.endGroup();

    // mods
    settings.beginGroup("Mods");    
    settings.setValue("Mods",                    getModConfigString());
    settings.endGroup();
}

QString Settings::getModConfigString()
{
    QString modString = "";
    for (qint32 i = 0; i < m_activeMods.size(); i++)
    {
        modString += m_activeMods[i] ;
        if (i < m_activeMods.size() - 1)
        {
            modString += ",";
        }
    }
    return modString;
}

void Settings::setup()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    // load language file and install it
    if(pMainapp->getTranslator()->load(QLocale(m_language), "resources/translation/lang_" + m_language,".qm"))
    {
        pMainapp->installTranslator(pMainapp->getTranslator());
    }
    else if (m_language != "en")
    {
        QString error = "Error: Unknown Language " + m_language + " selected.";
        Console::print(error, Console::eERROR);
    }
}

float Settings::getMouseSensitivity()
{
    return m_mouseSensitivity;
}

void Settings::setMouseSensitivity(float value)
{
    m_mouseSensitivity = value;
}

GameEnums::AnimationMode Settings::getShowAnimations()
{
    return showAnimations;
}

void Settings::setShowAnimations(GameEnums::AnimationMode value)
{
    showAnimations = value;
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
    return animationSpeed;
}

float Settings::getAnimationSpeed()
{
    if (animationSpeed <= 100)
    {
        return 100.0f / (101.0f - animationSpeed);
    }
    return 100;
}

void Settings::setAnimationSpeed(const quint32 &value)
{
    animationSpeed = value;
}

float Settings::getBattleAnimationSpeedValue()
{
    return battleAnimationSpeed;
}

float Settings::getBattleAnimationSpeed()
{
    if (battleAnimationSpeed <= 100)
    {
        return 100.0f / (101.0f - battleAnimationSpeed);
    }
    return 100;
}

void Settings::setBattleAnimationSpeed(const quint32 &value)
{
    battleAnimationSpeed = value;
}
SDL_Keycode Settings::getKey_up()
{
    return m_key_up;
}

void Settings::setKey_up(const SDL_Keycode &key_up)
{
    m_key_up = key_up;
}

SDL_Keycode Settings::getKey_down()
{
    return m_key_down;
}

void Settings::setKey_down(const SDL_Keycode &key_down)
{
    m_key_down = key_down;
}

SDL_Keycode Settings::getKey_right()
{
    return m_key_right;
}

void Settings::setKey_right(const SDL_Keycode &key_right)
{
    m_key_right = key_right;
}

SDL_Keycode Settings::getKey_left()
{
    return m_key_left;
}

void Settings::setKey_left(const SDL_Keycode &key_left)
{
    m_key_left = key_left;
}

SDL_Keycode Settings::getKey_confirm()
{
    return m_key_confirm;
}

void Settings::setKey_confirm(const SDL_Keycode &key_confirm)
{
    m_key_confirm = key_confirm;
}

SDL_Keycode Settings::getKey_cancel()
{
    return m_key_cancel;
}

void Settings::setKey_cancel(const SDL_Keycode &key_cancel)
{
    m_key_cancel = key_cancel;
}

SDL_Keycode Settings::getKey_next()
{
    return m_key_next;
}

void Settings::setKey_next(const SDL_Keycode &key_next)
{
    m_key_next = key_next;
}

SDL_Keycode Settings::getKey_previous()
{
    return m_key_previous;
}

void Settings::setKey_previous(const SDL_Keycode &key_previous)
{
    m_key_previous = key_previous;
}

SDL_Keycode Settings::getKey_quicksave1()
{
    return m_key_quicksave1;
}

void Settings::setKey_quicksave1(const SDL_Keycode &key_quicksave1)
{
    m_key_quicksave1 = key_quicksave1;
}

SDL_Keycode Settings::getKey_quicksave2()
{
    return m_key_quicksave2;
}

void Settings::setKey_quicksave2(const SDL_Keycode &key_quicksave2)
{
    m_key_quicksave2 = key_quicksave2;
}

SDL_Keycode Settings::getKey_quickload1()
{
    return m_key_quickload1;
}

void Settings::setKey_quickload1(const SDL_Keycode &key_quickload1)
{
    m_key_quickload1 = key_quickload1;
}

SDL_Keycode Settings::getKey_quickload2()
{
    return m_key_quickload2;
}

void Settings::setKey_quickload2(const SDL_Keycode &key_quickload2)
{
    m_key_quickload2 = key_quickload2;
}

SDL_Keycode Settings::getKey_information()
{
    return m_key_information;
}

void Settings::setKey_information(const SDL_Keycode &key_information)
{
    m_key_information = key_information;
}

quint32 Settings::getMultiTurnCounter()
{
    return multiTurnCounter;
}

void Settings::setMultiTurnCounter(const quint32 &value)
{
    multiTurnCounter = value;
}

GameEnums::BattleAnimationMode Settings::getBattleAnimations()
{
    return battleAnimations;
}

void Settings::setBattleAnimations(const GameEnums::BattleAnimationMode &value)
{
    battleAnimations = value;
}

SDL_Keycode Settings::getKey_moveMapLeft()
{
    return m_key_moveMapLeft;
}

void Settings::setKey_moveMapLeft(const SDL_Keycode &key_moveMapLeft)
{
    m_key_moveMapLeft = key_moveMapLeft;
}

SDL_Keycode Settings::getKey_moveMapRight()
{
    return m_key_moveMapRight;
}

void Settings::setKey_moveMapRight(const SDL_Keycode &key_moveMapRight)
{
    m_key_moveMapRight = key_moveMapRight;
}

SDL_Keycode Settings::getKey_moveMapDown()
{
    return m_key_moveMapDown;
}

void Settings::setKey_moveMapDown(const SDL_Keycode &key_moveMapDown)
{
    m_key_moveMapDown = key_moveMapDown;
}

SDL_Keycode Settings::getKey_moveMapUp()
{
    return m_key_moveMapUp;
}

void Settings::setKey_moveMapUp(const SDL_Keycode &key_moveMapUp)
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
