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

    static quint16 getServerPort();
    static void setServerPort(const quint16 &ServerPort);

    static QString getServerAdress();
    static void setServerAdress(const QString &ServerAdress);

    static void setServer(bool Server);

    static QString getLastSaveGame();
    static void setLastSaveGame(const QString &LastSaveGame);


    static void setUsername(const QString &Username);




public slots:

    static bool getShowCursor();
    static void setShowCursor(bool ShowCursor);

    static bool getAutoEndTurn();
    static void setAutoEndTurn(bool AutoEndTurn);

    static GameEnums::BattleAnimationMode getBattleAnimations();
    static void setBattleAnimations(const GameEnums::BattleAnimationMode &value);

    static QString getUsername();
    inline static qint32 getX()
    {
        return m_x;
    }
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

    static quint32 getAnimationSpeed();
    static void setAnimationSpeed(const quint32 &value);

    static SDL_Keycode getKey_up();
    static void setKey_up(const SDL_Keycode &key_up);

    static SDL_Keycode getKey_down();
    static void setKey_down(const SDL_Keycode &key_down);

    static SDL_Keycode getKey_right();
    static void setKey_right(const SDL_Keycode &key_right);

    static SDL_Keycode getKey_left();
    static void setKey_left(const SDL_Keycode &key_left);

    static SDL_Keycode getKey_confirm();
    static void setKey_confirm(const SDL_Keycode &key_confirm);

    static SDL_Keycode getKey_cancel();
    static void setKey_cancel(const SDL_Keycode &key_cancel);

    static SDL_Keycode getKey_next();
    static void setKey_next(const SDL_Keycode &key_next);

    static SDL_Keycode getKey_previous();
    static void setKey_previous(const SDL_Keycode &key_previous);

    static SDL_Keycode getKey_quicksave1();
    static void setKey_quicksave1(const SDL_Keycode &key_quicksave1);

    static SDL_Keycode getKey_quicksave2();
    static void setKey_quicksave2(const SDL_Keycode &key_quicksave2);

    static SDL_Keycode getKey_quickload1();
    static void setKey_quickload1(const SDL_Keycode &key_quickload1);

    static SDL_Keycode getKey_quickload2();
    static void setKey_quickload2(const SDL_Keycode &key_quickload2);

    static SDL_Keycode getKey_moveMapUp();
    static void setKey_moveMapUp(const SDL_Keycode &key_moveMapUp);

    static SDL_Keycode getKey_moveMapDown();
    static void setKey_moveMapDown(const SDL_Keycode &key_moveMapDown);

    static SDL_Keycode getKey_moveMapRight();
    static void setKey_moveMapRight(const SDL_Keycode &key_moveMapRight);

    static SDL_Keycode getKey_moveMapLeft();
    static void setKey_moveMapLeft(const SDL_Keycode &key_moveMapLeft);

    static SDL_Keycode getKey_information();
    static void setKey_information(const SDL_Keycode &key_information);
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
    static SDL_Keycode m_key_up;
    static SDL_Keycode m_key_down;
    static SDL_Keycode m_key_right;
    static SDL_Keycode m_key_left;
    static SDL_Keycode m_key_confirm;
    static SDL_Keycode m_key_cancel;
    static SDL_Keycode m_key_next;
    static SDL_Keycode m_key_previous;
    static SDL_Keycode m_key_quicksave1;
    static SDL_Keycode m_key_quicksave2;
    static SDL_Keycode m_key_quickload1;
    static SDL_Keycode m_key_quickload2;
    static SDL_Keycode m_key_information;
    static SDL_Keycode m_key_moveMapUp;
    static SDL_Keycode m_key_moveMapDown;
    static SDL_Keycode m_key_moveMapRight;
    static SDL_Keycode m_key_moveMapLeft;

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
    static bool m_Server;

    // ingame options
    static GameEnums::AnimationMode showAnimations;
    static GameEnums::BattleAnimationMode battleAnimations;
    static quint32 animationSpeed;
    static QString m_LastSaveGame;
    static bool m_ShowCursor;
    static bool m_AutoEndTurn;

    // internal members
    static Settings* m_pInstance;
    static const QString m_settingFile;
    static QStringList m_activeMods;

    static float m_mouseSensitivity;
};

#endif // SETTINGS_H
