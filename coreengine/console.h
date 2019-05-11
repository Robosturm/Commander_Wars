#ifndef CONSOLE_H
#define CONSOLE_H

#include <QVector>
#include <QObject>

#include "oxygine-framework.h"

#include "coreengine/mainapp.h"

class QString;
class QMutex;
class QKeyEvent;
class Interpreter;

class Console : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:

    static const QString functions[];

    enum eLogLevels
    {
        eDEBUG = 0,
        eINFO,
        eWARNING,
        eERROR,
        eFATAL,
        eOFF
    };

    enum
    {
        BLINKFREQG = 250,
        MAXLASTMSG = 20
    };

    static Console* getInstance();
    static void dotask(const QString& message);
    static void draw();
    void init();
// use slots here since they're part of QMetaObject thus they get published to JSEngine.
public slots:
    static void print(const QString& message, qint8 LogLevel);
    /**
     * @brief Print
     * @param message
     * @param debugMessage false for Errors or Setup Messages. True for Ingame Actions used for Debugging. But unneeded in release build
     */
    static void print(const QString& message, eLogLevels LogLevel);
    static void createfunnymessage(qint32 message = -1);
    // Lua Libs Functions
    void setVolume(qint32 volume);
    void setLogLevel(eLogLevels newLogLevel);
    /**
     * @brief help shows the helps messages between given indexes or all as default
     * @param start
     * @param end
     */
    void help(qint32 start = 0, qint32 end = -1);
    virtual void update(const oxygine::UpdateState& us) override;
    /**
     * @brief KeyInput event called when a key is pressed
     * @param event
     */
    void KeyInput(SDL_Event event);
    /**
     * @brief TextInput event called when we recieved a text from the keyboard
     * @param event
     */
    void TextInput(SDL_Event event);
private:
    static eLogLevels LogLevel;
    static QString curmsg;
    static QList<QString> lastmsgs;
    static const qint32 lastMsgSize{10};
    static qint32 curlastmsgpos;
    static qint32 curmsgpos;
    static QTime toggle;
    static Console* m_pConsole;
    static bool show;
    static bool toggled;
    static QList<QString> output;
    static qint32 outputSize;
    static QMutex *datalocker;
    oxygine::spTextField m_text;
    void toggleView();

    Console();
    virtual  ~Console() = default;
};

#endif // CONSOLE_H
