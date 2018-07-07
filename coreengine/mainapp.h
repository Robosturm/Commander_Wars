#ifndef MAINAPP_H
#define MAINAPP_H

#include <QTimer>
#include <QTranslator>
#include <QCoreApplication>

#include "coreengine/interpreter.h"
#include "coreengine/audiothread.h"
#include "coreengine/settings.h"

class NetworkInterface;

class Mainapp : public QCoreApplication
{
    Q_OBJECT

public slots:
    void update();

    static qint32 randInt(qint32 low, qint32 high);
    static bool isEven(qint32 value);
    void quitGame();
signals:
    void sigText(SDL_Event* event);
    void sigKeyDown(SDL_Event* event);
    void sigKeyUp(SDL_Event* event);
public:
    /**
      * @brief this enum contains all message recievers of the network
      */
    enum class NetworkSerives
    {
        Console = 0,
        Game,
        Max,
    };

    explicit Mainapp(int argc, char* argv[]);
    virtual ~Mainapp();

    static Mainapp* getInstance();

    inline QTranslator* getTranslator()
    {
        return &m_Translator;
    }

    inline Interpreter* getInterpreter()
    {
        if (m_pInterpreter == nullptr)
        {
            m_pInterpreter = new Interpreter();
        }
        return m_pInterpreter;
    }

    inline AudioThread* getAudioThread()
    {
        return m_Audiothread;
    }

    inline Settings* getSettings()
    {
        return &m_Settings;
    }

    inline NetworkInterface* getNetworkInterface()
    {
        return m_pNetworkInterface;
    }

    void setup();

    void setupNetwork();
protected:
    void onEvent(oxygine::Event* ev);
private:
    QTranslator m_Translator;
    QTimer m_Timer;
    static Mainapp* m_pMainapp;

    Interpreter* m_pInterpreter{nullptr};
    AudioThread* m_Audiothread{nullptr};
    Settings m_Settings;
    NetworkInterface* m_pNetworkInterface{nullptr};
    bool m_quit{false};
};

#endif // MAINAPP_H
