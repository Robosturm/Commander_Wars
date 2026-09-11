#ifndef MAINAPP_H
#define MAINAPP_H

#include <atomic>
#include <QTranslator>
#include <QThread>
#include <QCoreApplication>
#include <QMutex>
#include <QProcess>
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"

#include "objects/base/EventTextEdit.h"

#include "coreengine/settings.h"
#include "coreengine/Gamepad.h"
#include "coreengine/commandlineparser.h"
#include "coreengine/workerObject.h"
#include "game/GameEnums.h"

#ifdef UPDATESUPPORT
#include "updater/gameupdater.h"
#endif

class BaseGamemenu;
class AudioManager;
using spAudioManager = std::shared_ptr<AudioManager>;
using spAudioThread = std::shared_ptr<AudioManager>;
class NetworkInterface;
using spNetworkInterface = std::shared_ptr<NetworkInterface>;
class AiProcessPipe;
using spAiProcessPipe = std::shared_ptr<AiProcessPipe>;
class Minimap;
using spQProcess = std::shared_ptr<QProcess>;

class Mainapp final : public oxygine::GameWindow
{
    Q_OBJECT
public:
    static const char* const GAME_CONTEXT;
    static constexpr qint32 stepProgress = 4;
    static constexpr qint16 SCRIPT_PROCESS = GameEnums::StartupPhase::StartupPhase_ShopLoader * stepProgress;
    /**
     * @brief The ZOrder enum for z-order of actors directly attached to the game map or the menu
     */
    enum class ZOrder
    {
        Background = std::numeric_limits<qint32>::min(),
        Map,
        Terrain,
        // gap for stacking the terrain sprites

        CORange = std::numeric_limits<qint32>::max() - 200,
        GridLayout,
        FogFields,
        MarkedFields,
        Units,
        MoveArrow,
        Weather,
        Cursor,
        Animation,
        Objects,
        FocusedObjects = Objects + 2,
        AnimationFullScreen,
        DropDownList,
        Dialogs,
        Tooltip,
        Loadingscreen,
        Achievement,
        Console,
    };

    explicit Mainapp();
    virtual ~Mainapp();
    virtual void shutdown() override;
    static inline Mainapp* getInstance()
    {
        return m_pMainapp;
    }

    inline AudioManager* getAudioManager()
    {
        return getInstance()->m_AudioManager.get();
    }

    inline static QThread* getNetworkThread()
    {
        return getInstance()->m_networkThread.get();
    }
    /**
     * @brief loadRessources
     */
    virtual void loadRessources() override;
    /**
     * @brief getSlave
     * @return
     */
    static bool getSlave();
    /**
     * @brief setSlave
     * @param slave
     */
    static void setSlave(bool slave);
    // Argv passed to QProcess::startDetached when main() restarts after exit(1). Set this before exit(1) to thread args (e.g. --rejoin-password=...) into the child without persisting them to disk.
    static QStringList getRestartArgv();
    static void setRestartArgv(const QStringList & argv);
    static QString getRejoinPassword();
    static void setRejoinPassword(const QString & password);
    /**
     * @brief qsTr
     * @param text
     * @return
     */
    static QString qsTr(const char* const text);

    static QProcess & GetAiSubProcess()
    {
        return *(getInstance()->m_aiSubProcess.get());
    }
    /**
     * @brief qsTr
     * @param text
     * @return
     */
    static QString qsTr(QString text);

    bool getNoUi() const;
    /**
     * @brief getLastCreateLineEdit
     * @return
     */
    spEventTextEdit getLastCreateLineEdit() const
    {
        return m_pLineEdit;
    }
    /**
     * @brief getInitScript
     * @return
     */
    const QString &getInitScript() const;
    /**
     * @brief getCreateSlaveLogs
     * @return
     */
    bool getCreateSlaveLogs() const;
    /**
     * @brief setCreateSlaveLogs
     * @param create
     */
    void setCreateSlaveLogs(bool create);

    Gamepad& getGamepad()
    {
        return m_gamepad;
    }
    spNetworkInterface getSlaveClient();
    /**
     * @brief setNoUi
     */
    void setNoUi();
    /**
     * @brief setNoAudio
     */
    void setNoAudio();
    /**
     * @brief actAsSlave
     */
    void actAsSlave();
    /**
     * @brief setSlaveClient
     * @param client
     */
    void setSlaveClient(spNetworkInterface & client);
    /**
     * @brief setInitScript
     * @param newInitScript
     */
    void setInitScript(const QString &newInitScript);
    /**
     * @brief getParser
     * @return
     */
    CommandLineParser & getParser()
    {
        return m_parser;
    }

    QPoint mapPosFromGlobal(QPoint pos) const;
    QPoint mapPosToGlobal(QPoint pos) const;
    GameEnums::StartupPhase getStartUpStep() const;
    /**
     * @brief getAiProcessPipe
     * @return
     */
    static AiProcessPipe & getAiProcessPipe();
    static bool existsAiProcessPipe();

    bool isAudioThread() const
    {
        if (!m_useAudioThread)
        {
            return isMainThread();
        }
        else
        {
            return QThread::currentThread() == m_audioThread.get();
        }
    }
    static bool getTrainingSession();
    static void setTrainingSession(bool newTrainingSession);

    static bool getEnableAudioThread();
    static void setEnableAudioThread(bool enable);

public slots:
    void onActiveChanged();
    /**
     * @brief createBaseDirs
     */
    void createBaseDirs();
    void changeScreen(quint8 screen);
    void changeScreenMode(Settings::ScreenModes mode);
    void changeScreenSize(qint32 width, qint32 heigth);
    void changePosition(QPoint pos, bool invert);
    Settings::ScreenModes getScreenMode();
    /**
     * @brief showCrashReport
     * @param log
     */
    static void showCrashReport(const QString & log);
    /**
     * @brief doScreenshot
     */
    void doScreenshot();
    void nextStartUpStep(GameEnums::StartupPhase step);
    /**
     * @brief inputMethodQuery dummy function to rerout qlineedit events
     * @param query
     * @param ret
     */
    void inputMethodQuery(Qt::InputMethodQuery query, QVariant arg);
    /**
     * @brief createLineEdit
     */
    void createLineEdit();
    void resetLineEdit();
signals:
    void sigKeyDown(oxygine::KeyEvent event);
    void sigKeyUp(oxygine::KeyEvent event);
    void sigFocusLost();
    /**
     * @brief sigWindowLayoutChanged
     */
    void sigWindowLayoutChanged();
    /**
     * @brief sigChangePosition
     * @param pos
     */
    void sigChangePosition(QPoint pos, bool invert);
    /**
     * @brief sigShowCrashReport
     * @param log
     */
    void sigShowCrashReport(QString log);

    void sigNextStartUpStep(GameEnums::StartupPhase step);
    void sigCreateLineEdit();
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    bool keyInputMethodEvent(QInputMethodEvent *event);
    virtual bool event(QEvent *ev) override;    
    virtual void onQuit() override;
private:
    spEventTextEdit m_pLineEdit;

    static Mainapp* m_pMainapp;
    static bool m_slave;
    static bool m_trainingSession;
    static bool m_useAudioThread;
    static QStringList m_restartArgv;
    static QString m_rejoinPassword;
    QMutex m_crashMutex;
    spQThread m_networkThread;
    spQThread m_audioThread;
    spQProcess m_aiSubProcess;
    spWorkerObject m_workerObject;
    spAudioManager m_AudioManager;
    spAiProcessPipe m_aiProcessPipe;
    spNetworkInterface m_slaveClient;
    QString m_initScript;
    bool m_createSlaveLogs{false};
    Gamepad m_gamepad{0};
    bool m_noAudio{false};
    CommandLineParser m_parser;
    GameEnums::StartupPhase m_startUpStep{GameEnums::StartupPhase::StartupPhase_Start};
#ifdef UPDATESUPPORT
    spGameUpdater m_gameUpdater;
#endif
};

Q_DECLARE_INTERFACE(Mainapp, "Mainapp");

#endif // MAINAPP_H
