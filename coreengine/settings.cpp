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
SDL_Keycode Settings::m_key_escape    = SDLK_ESCAPE;
SDL_Keycode Settings::m_key_console   = SDLK_F1;
QString Settings::m_language      = "en";
// Sound
qint32 Settings::m_TotalVolume       = 100;
qint32 Settings::m_MusicVolume       = 80;
qint32 Settings::m_SoundVolume       = 80;
// Network
QString Settings::m_NetworkData   = "";
qint32 Settings::m_GamePort          = 5603;
bool Settings::m_Server           = true;

// ingame options
bool Settings::showAnimations = true;

// add mod path
QStringList Settings::m_activeMods;
// this Object
Settings* Settings::m_pInstance = NULL;

Settings::Settings()
{
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
    m_width       = settings.value("width", 1024).toInt(&ok);
    if(!ok){
        QString error = tr("Error in the Ini File: ") + "[Resolution] " + tr("Setting:") + " width";
        Console::print(error, Console::eERROR);
        m_width = 1024;
    }
    m_height      = settings.value("height", 800).toInt(&ok);
    if(!ok){
        QString error = tr("Error in the Ini File: ") + "[Resolution] " + tr("Setting:") + " heigth";
        Console::print(error, Console::eERROR);
        m_height = 800;
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
    m_SoundVolume      = settings.value("SoundVolume", 80).toInt(&ok);
    if(!ok)
    {
        QString error = tr("Error in the Ini File: ") + "[Sound] " + tr("Setting:") + " SoundVolume";
        Console::print(error, Console::eERROR);
        m_SoundVolume = 80;
    }

    settings.endGroup();

    settings.beginGroup("Game");
    showAnimations  = settings.value("ShowAnimations", true).toBool();
    settings.endGroup();

    // network
    settings.beginGroup("Network");
    m_NetworkData = settings.value("NetworkConfiguration", "").toString();
    m_GamePort = settings.value("GamePort", 5603).toInt();
    if (m_GamePort < 0)
    {
        m_GamePort = 5603;
    }
    m_Server  = settings.value("Server", true).toBool();
    settings.endGroup();

    // mods
    settings.beginGroup("Mods");
    QString modList = settings.value("Mods", "").toString();
    if (!modList.isEmpty())
    {
        m_activeMods = modList.split(",");
    }
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
    settings.endGroup();

    // Sound
    settings.beginGroup("Sound");
    settings.setValue("TotalVolume",               m_TotalVolume);
    settings.setValue("MusicVolume",               m_MusicVolume);
    settings.setValue("SoundVolume",               m_SoundVolume);
    settings.endGroup();

    settings.beginGroup("Game");
    settings.setValue("ShowAnimations",                 showAnimations);
    settings.endGroup();

    // network
    settings.beginGroup("Network");
    settings.setValue("NetworkConfiguration",      m_NetworkData);
    settings.setValue("GamePort",                  m_GamePort);
    settings.setValue("Server",                    m_Server);
    settings.endGroup();

    // mods
    settings.beginGroup("Mods");
    QString modString = "";
    for (qint32 i = 0; i < m_activeMods.size(); i++)
    {
        modString += m_activeMods[i] ;
        if (i < m_activeMods.size() - 1)
        {
            modString += ",";
        }
    }
    settings.setValue("Mods",                    modString);
    settings.endGroup();
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

bool Settings::getShowAnimations()
{
    return showAnimations;
}

void Settings::setShowAnimations(bool value)
{
    showAnimations = value;
}
