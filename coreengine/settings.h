#ifndef SETTINGS_H
#define SETTINGS_H

#include <Qt>
#include <QObject>

#include "oxygine-framework.h"

#include "game/GameEnums.h"

class GLGraphicsView;

class Settings : public QObject
{
    Q_OBJECT
public:
    Settings();
    virtual ~Settings() = default;

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
    inline qint32 getHeight()
    {
        return m_height;
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

    inline void setTotalVolume(qint32 value)
    {
        m_TotalVolume = value;
    }
    inline qint32 getTotalVolume()
    {
        return m_TotalVolume;
    }

    inline void setMusicVolume(qint32 value)
    {
        m_MusicVolume = value;
    }
    inline qint32 getMusicVolume()
    {
        return m_MusicVolume;
    }

    inline void setSoundVolume(qint32 value)
    {
        m_SoundVolume = value;
    }
    inline qint32 getSoundVolume()
    {
        return m_SoundVolume;
    }

    inline QString getNetworkData()
    {
        return m_NetworkData;
    }
    inline void setNetworkData(QString value)
    {
        m_NetworkData = value;
    }
    inline qint32 getGamePort()
    {
        return m_GamePort;
    }
    inline void setGamePort(qint32 value)
    {
        m_GamePort = value;
    }
    inline bool getServer()
    {
        return m_Server;
    }
    inline QStringList getMods()
    {
        return m_activeMods;
    }
    void addMod(QString mod)
    {
        if (!m_activeMods.contains(mod))
        {
            m_activeMods.append(mod);
        }
    }
    void removeMod(QString mod)
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

    static quint32 getAnimationSpeed();
    static void setAnimationSpeed(const quint32 &value);

private:
    // setting variables
    static qint32 m_x;
    static qint32 m_y;
    static qint32 m_width;
    static qint32 m_height;

    static bool m_borderless;
    static bool m_fullscreen;

    static SDL_Keycode m_key_escape;
    static SDL_Keycode m_key_console;

    static QString m_language;
    // Sound
    static qint32 m_TotalVolume;
    static qint32 m_MusicVolume;
    static qint32 m_SoundVolume;
    // Network
    static QString m_NetworkData;
    static qint32 m_GamePort;
    static bool m_Server;

    // ingame options
    static GameEnums::AnimationMode showAnimations;
    static quint32 animationSpeed;

    // internal members
    static Settings* m_pInstance;
    static const QString m_settingFile;
    static QStringList m_activeMods;

    static float m_mouseSensitivity;
};

#endif // SETTINGS_H
