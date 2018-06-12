#include "coreengine/settings.h"
#include "coreengine/console.h"
#include "coreengine/mainapp.h"

#include <QSettings>
#include <QTranslator>
#include <QLocale>

const QString Settings::m_settingFile = "Commander_Wars.ini";

qint32 Settings::m_x                 = 0;
qint32 Settings::m_y                 = 0;
qint32 Settings::m_width             = 800;
qint32 Settings::m_heigth            = 600;
bool Settings::m_borderless       = false;
bool Settings::m_fullscreen       = false;
SDL_Keycode Settings::m_key_escape    = SDLK_ESCAPE;
SDL_Keycode Settings::m_key_console   = SDLK_F1;
QString Settings::m_language      = "en";

// this Object
Settings* Settings::m_pInstance = NULL;

Settings* Settings::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new Settings();
    }
    return m_pInstance;
}

Settings::Settings()
{
    loadSettings();
}

void Settings::loadSettings(){
    bool ok = false;
    QSettings settings(m_settingFile, QSettings::IniFormat);

    settings.beginGroup("general");
    m_language    = settings.value("language","en").toString();
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
    m_width       = settings.value("width",800).toInt(&ok);
    if(!ok){
        QString error = tr("Error in the Ini File: ") + "[Resolution] " + tr("Setting:") + " width";
        Console::print(error, Console::eERROR);
        m_width = 800;
    }
    m_heigth      = settings.value("heigth",600).toInt(&ok);
    if(!ok){
        QString error = tr("Error in the Ini File: ") + "[Resolution] " + tr("Setting:") + " heigth";
        Console::print(error, Console::eERROR);
        m_heigth = 600;
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
}

void Settings::saveSettings(){
    QSettings settings(m_settingFile, QSettings::IniFormat);

    settings.beginGroup("general");
    settings.setValue("language",                   m_language);
    settings.endGroup();

    settings.beginGroup("Resolution");
    settings.setValue("x",                          m_x);
    settings.setValue("y",                          m_y);
    settings.setValue("width",                      m_width);
    settings.setValue("heigth",                     m_heigth);
    settings.setValue("borderless",                 m_borderless);
    settings.setValue("fullscreen",                 m_fullscreen);
    settings.endGroup();

    settings.beginGroup("Keys");
    settings.setValue("key_escape",                 m_key_escape);
    settings.setValue("key_console",                m_key_console);
    settings.endGroup();
}

void Settings::setup()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    if(pMainapp->getTranslator()->load(QLocale(m_language),"general","_","resources/language/" + m_language,".qm")){
        pMainapp->installTranslator(pMainapp->getTranslator());
    }
    else if (m_language != "en")
    {
        QString error = "Error: Unknown Language " + m_language + " selected.";
        Console::print(error, Console::eERROR);
    }
}
