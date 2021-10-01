#ifndef CONSOLE_H
#define CONSOLE_H

#include <QVector>
#include <QObject>
#include <QKeyEvent>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "objects/base/textinput.h"

class QString;
class QMutex;
class QKeyEvent;
class Interpreter;

class Console;
using spConsole = oxygine::intrusive_ptr<Console>;

#define CONSOLE_PRINT(text, logLevel) if (logLevel >= Console::getLogLevel() ) {Console::print(text, logLevel);}

class Console : public TextInput
{
    Q_OBJECT
public:

    static const QString functions[];
    static const char* const compileTime;
    static const char* const compileDate;

    enum eLogLevels
    {
        eOFF = -1,
        eDEBUG = 0,
        eINFO,
        eWARNING,
        eERROR,
        eFATAL,
    };
    Q_ENUM(eLogLevels)

    enum
    {
        BLINKFREQG = 250,
        MAXLASTMSG = 20
    };
    virtual ~Console() = default;
    static spConsole getInstance();
    static void dotask(QString message);
    static void draw();
    static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void init();
    void release();
signals:
    void sigToggleView();
    // use slots here since they're part of QMetaObject thus they get published to JSEngine.
public slots:
    /**
     * @brief getDeveloperMode
     * @return
     */
    static bool getDeveloperMode();
    /**
     * @brief setDeveloperMode
     * @param developerMode
     */
    static void setDeveloperMode(bool developerMode);
    /**
     * @brief print
     * @param message
     * @param LogLevel
     */
    static void print(QString message, qint8 LogLevel);
    /**
     * @brief Print
     * @param message
     * @param debugMessage false for Errors or Setup Messages. True for Ingame Actions used for Debugging. But unneeded in release build
     */
    static void print(QString message, Console::eLogLevels LogLevel);
    /**
     * @brief createfunnymessage
     * @param message
     */
    static void createfunnymessage(qint32 message = -1);
    /**
     * @brief setVolume
     * @param volume
     */
    void setVolume(qint32 volume);
    /**
     * @brief setLogLevel
     * @param newLogLevel
     */
    static void setLogLevel(Console::eLogLevels newLogLevel);
    /**
     * @brief getLogLevel
     * @return
     */
    static Console::eLogLevels getLogLevel()
    {
        return m_LogLevel;
    }
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
     * @brief version
     */
    void version();
    /**
     * @brief logActions
     * @param log
     */
    void logActions(bool log);
    /**
     * @brief extractResources
     */
    void extractResources();
    /**
     * @brief memoryUsage
     */
    void memoryUsage();
    /**
     * @brief update
     * @param us
     */
    virtual void update(const oxygine::UpdateState& us) override;
    /**
     * @brief getConsoleLog
     * @return
     */
    QList<QString> getConsoleLog()
    {
        QMutexLocker locker(&m_datalocker);
        return m_output;
    }
protected slots:
    /**
     * @brief toggleView
     */
    void toggleView();
    /**
     * @brief doHandleEvent
     * @param event
     */
    virtual void doHandleEvent(QEvent *event) override;
protected:
    virtual bool onEditFinished() override;
private:
    friend class oxygine::intrusive_ptr<Console>;
    explicit Console();

    void createSprites(QString file, QImage& colorTable, QImage maskTable);

private:
    static eLogLevels m_LogLevel;
    static QList<QString> m_lastmsgs;
    static const qint32 m_lastMsgSize{10};
    static qint32 m_curlastmsgpos;
    static spConsole m_pConsole;
    static bool m_show;
    static bool m_toggled;
    static QList<QString> m_output;
    static qint32 m_outputSize;
    static QMutex m_datalocker;
    static bool m_developerMode;
    oxygine::spSprite m_pBackgroundsprite;
    oxygine::spTextField m_text;
};

#endif // CONSOLE_H
