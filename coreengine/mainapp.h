#ifndef MAINAPP_H
#define MAINAPP_H

#include <atomic>
#include <QTimer>
#include <QTranslator>
#include <QThread>
#include <QCoreApplication>
#include "QMutex"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

#include "coreengine/settings.h"
#include "coreengine/LUPDATE_MACROS.h"

class AudioThread;
class WorkerThread;

class Mainapp : public oxygine::GameWindow
{
    Q_OBJECT
public:
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
        FogFields,
        MarkedFields,
        Weather,
        Cursor,
        Animation,
        Objects,
        FocusedObjects,
        AnimationFullScreen,
        Dialogs,
        DropDownList,
        Tooltip,
        Loadingscreen,
        Achievement,
        Console
    };

    explicit Mainapp();
    virtual ~Mainapp();

    static inline Mainapp* getInstance()
    {
        return m_pMainapp;
    }

    inline AudioThread* getAudioThread()
    {
        return m_Audiothread;
    }    

    inline static QThread* getWorkerthread()
    {
        return &m_Workerthread;
    }

    inline static QThread* getNetworkThread()
    {
        return &m_Networkthread;
    }
    inline static QThread* getAudioWorker()
    {
        return &m_AudioWorker;
    }
    /**
     * @brief loadRessources
     */
    virtual void loadRessources() override;
    virtual bool isWorker() override;
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
    bool getNoUi() const;

public slots:
    void changeScreenMode(qint32 mode);
    void changeScreenSize(qint32 width, qint32 heigth);
    void changePosition(QPoint pos, bool invert);
    qint32 getScreenMode();
    void applyFilter(quint32 filter);
    /**
     * @brief getGameVersion
     * @return
     */
    static QString getGameVersion()
    {
        return "Version: " + QString::number(MAJOR) + "." + QString::number(MINOR) + "." + QString::number(REVISION);
    }
    /**
     * @brief showCrashReport
     * @param log
     */
    static void showCrashReport(QString log);
    /**
     * @brief loadArgs
     * @param args
     */
    void loadArgs(const QStringList & args);
    /**
     * @brief onActiveChanged
     */
    void onActiveChanged();
    /**
     * @brief doScreenshot
     */
    void doScreenshot();
signals:
    void sigKeyDown(oxygine::KeyEvent event);
    void sigKeyUp(oxygine::KeyEvent event);
    void sigConsoleKeyDown(oxygine::KeyEvent event);
    void sigConsoleKeyUp(oxygine::KeyEvent event);
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
    /**
     * @brief sigApplyFilter
     * @param filter
     */
    void sigApplyFilter(quint32 filter);
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
private:
    static Mainapp* m_pMainapp;
    static QMutex crashMutex;
    static QThread m_Workerthread;
    static QThread m_AudioWorker;
    static QThread m_Networkthread;
    static QThread m_GameServerThread;
    QThread* pMainThread{nullptr};
    AudioThread* m_Audiothread;
    WorkerThread* m_Worker;
    static bool m_slave;
    bool m_noUi{false};
};

#endif // MAINAPP_H
