#ifndef CONSOLE_H
#define CONSOLE_H

#include <QVector>
#include <QObject>
#include <QElapsedTimer>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "QKeyEvent"

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
    static void dotask(QString message);
    static void draw();
    static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void init();

// use slots here since they're part of QMetaObject thus they get published to JSEngine.

public slots:
    static void print(QString message, qint8 LogLevel);
    /**
     * @brief Print
     * @param message
     * @param debugMessage false for Errors or Setup Messages. True for Ingame Actions used for Debugging. But unneeded in release build
     */
    static void print(QString message, eLogLevels LogLevel);
    static void createfunnymessage(qint32 message = -1);
    // Lua Libs Functions
    void setVolume(qint32 volume);
    /**
     * @brief setLogLevel
     * @param newLogLevel
     */
    void setLogLevel(eLogLevels newLogLevel);
    /**
     * @brief getLogLevel
     * @return
     */
    eLogLevels getLogLevel();
    /**
     * @brief createSprites
     * @param input
     * @param colorTable
     * @param maskTable
     */
    void createSprites(QString input, QString colorTable, QString maskTable);
    /**
     * @brief createColorTableSprites
     * @param folder
     * @param filter
     * @param startIndex
     * @param maxColors
     */
    void createColorTableSprites(QString folder, QString filter, qint32 startIndex, qint32 maxColors);
    /**
     * @brief updateMaskImages
     * @param folder
     * @param filter
     */
    void updateMaskImages(QString folder, QString filter, qint32 min);
    /**
     * @brief inversImagesFrames
     * @param folder
     * @param filter
     * @param frames
     */
    void inversImagesFrames(QString folder, QString filter, qint32 frames);
    /**
     * @brief applyImagesTable
     * @param input
     * @param inTable
     * @param outTable
     * @param eraseColor
     */
    void applyImagesTable(QString input, QString inTable, QString outTable, QColor eraseColor = Qt::white);
    /**
     * @brief extendMaskImages
     * @param folder
     * @param filter
     */
    void extendMaskImages(QString folder, QString filter);
    /**
     * @brief help shows the helps messages between given indexes or all as default
     * @param start
     * @param end
     */
    void help(qint32 start = 0, qint32 end = -1);
    /**
     * @brief logActions
     * @param log
     */
    void logActions(bool log);
    /**
     * @brief update
     * @param us
     */
    virtual void update(const oxygine::UpdateState& us) override;
    /**
     * @brief KeyInput event called when a key is pressed
     * @param event
     */
    void KeyInput(oxygine::KeyEvent event);
    /**
     * @brief toggleView
     */
    void toggleView();
    /**
     * @brief getConsoleLog
     * @return
     */
    QList<QString> getConsoleLog()
    {
        QMutexLocker locker(&datalocker);
        return output;
    }
private:
    static eLogLevels LogLevel;
    static QString curmsg;
    static QList<QString> lastmsgs;
    static const qint32 lastMsgSize{10};
    static qint32 curlastmsgpos;
    static qint32 curmsgpos;
    static QElapsedTimer toggle;
    static Console* m_pConsole;
    static bool show;
    static bool toggled;
    static QList<QString> output;
    static qint32 outputSize;
    static QMutex datalocker;
    oxygine::spSprite m_pBackgroundsprite;
    oxygine::spTextField m_text;

    Console();
    virtual  ~Console() = default;

    void createSprites(QString file, QImage& colorTable, QImage maskTable);
};

#endif // CONSOLE_H
