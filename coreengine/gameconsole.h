#ifndef CONSOLE_H
#define CONSOLE_H

#include <QVector>
#include <QObject>
#include <QKeyEvent>

#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "objects/base/textinput.h"

class QString;
class QMutex;
class QKeyEvent;
class Interpreter;

class GameConsole;
using spConsole = oxygine::intrusive_ptr<GameConsole>;

class GameConsole final : public TextInput
{
    Q_OBJECT
public:

    static const char* const functions[];
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

    enum eModules : quint64
    {
        eGeneral     = 1 << 1,
        eResources   = 1 << 2,
        eAI          = 1 << 3,
        eFileSupport = 1 << 4,
        eJavaScript  = 1 << 5,
        eAudio       = 1 << 6,
        eUiFactory   = 1 << 7,
    };
    Q_ENUM(eModules)

    enum
    {
        BLINKFREQG = 250,
        MAXLASTMSG = 20
    };
    ~GameConsole() = default;
    static GameConsole* getInstance();
    static bool hasInstance();
    static void dotask(QString message);
    static void draw();
    static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void init();
    void release();


signals:
    void sigToggleView();
    void sigExecuteCommand(QString command);
    // use slots here since they're part of QMetaObject thus they get published to JSEngine.
public slots:
    /**
     * @brief setActiveModules
     * @param newActiveModules
     */
    static void setActiveModules(quint64 newActiveModules);
    /**
     * @brief getActiveModules
     * @return
     */
    static quint64 getActiveModules()
    {
        return m_activeModules;
    }
    /**
     * @brief setActiveModules
     * @param newActiveModules
     */
    static void setModuleMode(quint64 newActiveModule, bool enable);
    /**
     * @brief getActiveModules
     * @return,
     */
    static bool isActiveModule(quint64 module);
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
     * Prints onto the console using the javascript-module
     * @brief print
     * @param message
     * @param LogLevel
     */
    static void print(const QString & message, qint8 logLevel);
    /**
     * @brief Print
     * @param message
     * @param debugMessage false for Errors or Setup Messages. True for Ingame Actions used for Debugging. But unneeded in release build
     */
    static void printDirectly(const QString & message, GameConsole::eLogLevels logLevel);
    /**
     * @brief printObject
     * @param message
     * @param LogLevel
     */
    static void printObjectDetails(QObject* obj, GameConsole::eLogLevels logLevel);
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
    static void setLogLevel(GameConsole::eLogLevels newLogLevel);
    /**
     * @brief getLogLevel
     * @return
     */
    static GameConsole::eLogLevels getLogLevel()
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
     * @brief updateTerrainPaletteMasks
     * @param folder
     * @param filter
     * @param originalPaletteSize
     * @param newPaletteSize
     */
    void updateTerrainPaletteMasks(const QString& folder, const QString& filter, qint32 originalPaletteSize, qint32 newPaletteSize, qint32 xOffset = 0, qint32 yOffset = 0);
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
     * @brief resetMaps
     */
    void resetMapsGameRules(const QString & folder);
    /**
     * @brief update
     * @param us
     */
    virtual void update(const oxygine::UpdateState& us) override;
    /**
     * @brief getConsoleLog
     * @return
     */
    std::vector<QString> getConsoleLog()
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
    virtual bool doHandleEvent(QEvent *event) override;
protected:
    virtual bool onEditFinished() override;
private:
    friend class oxygine::intrusive_ptr<GameConsole>;
    explicit GameConsole();

private:
    static const qint32 m_lastMsgSize{10};
    static spConsole m_pConsole;
    static eLogLevels m_LogLevel;
    static quint64 m_activeModules;
    static std::vector<QString> m_lastmsgs;
    static qint32 m_curlastmsgpos;
    static bool m_show;
    static bool m_toggled;
    static std::vector<QString> m_output;
    static bool m_outputChanged;
    static qint32 m_outputSize;
    static bool m_developerMode;
    static QMutex m_datalocker;
    static QMutex messageOutputMutex;
    oxygine::spSprite m_pBackgroundsprite;
    oxygine::spTextField m_text;
    oxygine::spTextField m_editTextfield;
};

#define CONSOLE_PRINT(message, logLevel) \
    if (logLevel >= GameConsole::getLogLevel() && GameConsole::getActiveModules() & GameConsole::eGeneral) \
    {  \
        GameConsole::printDirectly(message, logLevel); \
    }

#define CONSOLE_PRINT_MODULE(message, logLevel, module) \
    if (logLevel >= GameConsole::getLogLevel() && GameConsole::getActiveModules() & module) \
    {  \
        GameConsole::printDirectly(message, logLevel); \
    }

#ifdef GAMEDEBUG
#define AI_CONSOLE_PRINT(message, logLevel) \
    if (logLevel >= GameConsole::getLogLevel() && GameConsole::getActiveModules() & GameConsole::eAI) \
    { \
        GameConsole::printDirectly(message, logLevel); \
    }
#else
#define AI_CONSOLE_PRINT(text, logLevel)
#endif

#endif // CONSOLE_H
