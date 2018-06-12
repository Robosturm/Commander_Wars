#ifndef SETTINGS_H
#define SETTINGS_H

#include <Qt>
#include <QObject>

#include "oxygine-framework.h"

class GLGraphicsView;

class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings* getInstance();


    static void loadSettings();
    static void saveSettings();
    static void setup();

    inline qint32 getX()
    {
        return m_x;
    }
    inline qint32 getY()
    {
        return m_y;
    }
    inline qint32 getWidth()
    {
        return m_width;
    }
    inline qint32 getHeigth()
    {
        return m_heigth;
    }

    inline bool getBorderless()
    {
        return m_borderless;
    }

    inline bool getFullscreen()
    {
        return m_fullscreen;
    }

    inline SDL_Keycode getKeyConsole()
    {
        return m_key_console;
    }
private:
    // setting variables
    static qint32 m_x;
    static qint32 m_y;
    static qint32 m_width;
    static qint32 m_heigth;

    static bool m_borderless;
    static bool m_fullscreen;

    static SDL_Keycode m_key_escape;
    static SDL_Keycode m_key_console;

    static QString m_language;

    // internal members
    static Settings* m_pInstance;
    static const QString m_settingFile;

    Settings();

};

#endif // SETTINGS_H
