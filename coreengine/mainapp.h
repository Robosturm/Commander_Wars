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
#include "coreengine/LUPDATE_MACROS.h"
#include "coreengine/Gamepad.h"
#include "coreengine/commandlineparser.h"

#include "network/rsacypherhandler.h"

#ifdef UPDATESUPPORT
#include "updater/gameupdater.h"
#endif

class BaseGamemenu;
class WorkerThread;
using spWorkerThread = oxygine::intrusive_ptr<WorkerThread>;
class AudioManager;
using spAudioThread = oxygine::intrusive_ptr<AudioManager>;
class TCPClient;
using spTCPClient = oxygine::intrusive_ptr<TCPClient>;
class AiProcessPipe;
using spAiProcessPipe = oxygine::intrusive_ptr<AiProcessPipe>;

class Mainapp final : public oxygine::GameWindow
{
    Q_OBJECT
public:
    static const char* const GAME_CONTEXT;
    static constexpr qint32 stepProgress = 4;
    enum StartupPhase
    {
        Start,
        General = Start,
        UpdateManager,
        ObjectManager,
        Building,
        COSprites,
        GameAnimations,
        GameManager,
        GameRuleManager,
        TerrainManager,
        UnitSpriteManager,
        BattleAnimationManager,
        COPerkManager,
        WikiDatabase,
        Userdata,
        Achievementmanager,
        MovementPlannerAddInManager,
        UiManager,
        ShopLoader,
        Sound,
        LoadingScripts,
        Finalizing,
    };
    Q_ENUM(StartupPhase)
    static constexpr qint16 SCRIPT_PROCESS = ShopLoader * stepProgress;
    /**
     * @brief The ZOrder enum for z-order of actors directly attached to the game map or the menu
     */
    ENUM_CLASS ZOrder
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
        Dialogs,
        DropDownList,
        Tooltip,
        Loadingscreen,
        Achievement,
        Console,
    };

    explicit Mainapp();
    ~Mainapp();
    virtual void shutdown() override;
    static inline Mainapp* getInstance()
    {
        return m_pMainapp;
    }

    inline AudioManager* getAudioManager()
    {
        return getInstance()->m_AudioManager.get();
    }

    inline static QThread* getWorkerthread()
    {
        return getInstance()->m_Workerthread.get();
    }

    inline static QThread* getNetworkThread()
    {
        return getInstance()->m_Networkthread.get();
    }
    /**
     * @brief loadRessources
     */
    virtual void loadRessources() override;
    virtual bool isWorker() override;
    virtual bool isWorkerRunning() override;
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
    /**
     * @brief getGameServer
     * @return
     */
    static QThread* getGameServerThread();
    /**
     * @brief qsTr
     * @param text
     * @return
     */
    static QString qsTr(const char* const text);
    /**
     * @brief getWorker
     * @return
     */
    static WorkerThread* getWorker()
    {
        return getInstance()->m_Worker.get();
    }
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
    EventTextEdit* getLastCreateLineEdit() const
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
    static spTCPClient getSlaveClient();
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
    /**
     * @brief getCypher
     * @return
     */
    RsaCypherHandler & getCypher()
    {
        return m_rsaCypher;
    }
    StartupPhase getStartUpStep() const;
    /**
     * @brief getAiProcessPipe
     * @return
     */
    static AiProcessPipe & getAiProcessPipe();

public slots:
    /**
     * @brief createBaseDirs
     */
    void createBaseDirs();
    void changeScreenMode(Settings::ScreenModes mode);
    void changeScreenSize(qint32 width, qint32 heigth);
    void changePosition(QPoint pos, bool invert);
    Settings::ScreenModes getScreenMode();
    /**
     * @brief getGameVersion
     * @return
     */
    static QString getGameVersion()
    {
        return "Version: " + QString::number(VERSION_MAJOR) + "." + QString::number(VERSION_MINOR) + "." + QString::number(VERSION_REVISION)+ "-" + QString(VERSION_SUFFIX);
    }
    /**
     * @brief showCrashReport
     * @param log
     */
    static void showCrashReport(const QString & log);
    /**
     * @brief onActiveChanged
     */
    void onActiveChanged();
    /**
     * @brief doScreenshot
     */
    void doScreenshot();
    void nextStartUpStep(Mainapp::StartupPhase step);
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
    void doMapshot(BaseGamemenu* pMenu);
signals:
    void sigKeyDown(oxygine::KeyEvent event);
    void sigKeyUp(oxygine::KeyEvent event);
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

    void sigNextStartUpStep(Mainapp::StartupPhase step);
    void sigCreateLineEdit();
    void sigDoMapshot(BaseGamemenu* pMenu);
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    bool keyInputMethodEvent(QInputMethodEvent *event);
    virtual bool event(QEvent *ev) override;    
    virtual void onQuit() override;

private:
    EventTextEdit* m_pLineEdit{nullptr};

    static Mainapp* m_pMainapp;
    static bool m_slave;
    QMutex m_crashMutex;
    QScopedPointer<QThread> m_Workerthread;
    QScopedPointer<QThread> m_Networkthread;
    QScopedPointer<QThread> m_GameServerThread;
    QScopedPointer<QProcess> m_aiSubProcess;
    QScopedPointer<WorkerThread> m_Worker;
    QScopedPointer<AudioManager> m_AudioManager;
    spAiProcessPipe m_aiProcessPipe;
    spTCPClient m_slaveClient;
    QString m_initScript;
    bool m_createSlaveLogs{false};
    Gamepad m_gamepad{0};
    bool m_noAudio{false};
    CommandLineParser m_parser;
    StartupPhase m_startUpStep{StartupPhase::Start};
    RsaCypherHandler m_rsaCypher;
#ifdef UPDATESUPPORT
    spGameUpdater m_gameUpdater;
#endif
};

Q_DECLARE_INTERFACE(Mainapp, "Mainapp");

#endif // MAINAPP_H
