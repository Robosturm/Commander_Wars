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
    void connectToServer(const QString&  adresse, qint32 port = -1);
    void getServerAdresse();
    void help(qint32 start = 0, qint32 end = -1);
    virtual void update(const oxygine::UpdateState& us) override;
    void KeyInput(SDL_Event *event);
    void TextInput(SDL_Event *event);
    void recieveNetworkMessage(QByteArray data, Mainapp::NetworkSerives serive);
private:
    static eLogLevels LogLevel;
    static QString curmsg;
    static QList<QString> lastmsgs;
    static qint32 curlastmsgpos;
    static qint32 curmsgpos;
    static QTime toggle;
    static Console* m_pConsole;
    static bool show;
    static bool toggled;
    static QVector<QString> output;
    static QMutex *datalocker;
    oxygine::spTextField m_text;
    void toggleView();
    void init();
    Console();
    virtual  ~Console() = default;
};

#endif // CONSOLE_H
