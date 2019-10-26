#ifndef MAINAPP_H
#define MAINAPP_H

#include <atomic>
#include <QTimer>
#include <QTranslator>
#include <QThread>
#include <QCoreApplication>
#include <QRandomGenerator>
#include "QMutex"

#include "coreengine/interpreter.h"
#include "coreengine/audiothread.h"
#include "coreengine/workerthread.h"
#include "coreengine/settings.h"

#include "coreengine/qmlvector.h"

#include "network/tcpserver.h"

#include "oxygine/core/gamewindow.h"

class Mainapp : public oxygine::GameWindow
{
    Q_OBJECT
public:
    /**
     * @brief The ZOrder enum for z-order of actors directly attached to the game map or the menu
     */
    enum class ZOrder
    {
        Background = -32000,
        Terrain = -30000,
        CORange = 30000,
        FogFields,
        MarkedFields,
        Weather,
        Cursor,
        Animation,
        Objects = 31998,
        FocusedObjects = 31999,
        Dialogs = 32000,
        Console
    };

    explicit Mainapp();
    virtual ~Mainapp();

    static Mainapp* getInstance();

    inline QTranslator* getTranslator()
    {
        return &m_Translator;
    }

    inline Interpreter* getInterpreter()
    {
        return m_Worker->getInterpreter();
    }

    inline AudioThread* getAudioThread()
    {
        return m_Audiothread;
    }    

    inline static QThread* getWorkerthread()
    {
        return &m_Workerthread;
    }
    inline TCPServer* getGameServer()
    {
        return m_pGameServer.get();
    }
    inline void stopGameServer()
    {
        m_pGameServer = nullptr;
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
    static bool getUseSeed();
    static void setUseSeed(bool useSeed);
    /**
     * @brief Mainapp::storeList
     * @param file
     * @param items
     * @param folder
     */
    static void storeList(QString file, QStringList items, QString folder);
    /**
     * @brief Mainapp::readList
     * @param file
     * @return
     */
    static std::tuple<QString, QStringList> readList(QString file);
    /**
     * @brief readList
     * @param file
     * @param folder
     * @return
     */
    static std::tuple<QString, QStringList> readList(QString file, QString folder);


public slots:
    inline Settings* getSettings()
    {
        return &m_Settings;
    }
    static void seed(quint32 seed);
    static qint32 randInt(qint32 low, qint32 high);
    /**
     * @brief roundUp rounds all numbers up. 9.1 -> 10
     * @param value
     * @return the rounded up integer value
     */
    static qint32 roundUp(float value);
    static qint32 roundDown(float value);
    static bool isEven(qint32 value);
    /**
     * @brief getCircle
     * @param min radius
     * @param max radius
     * @return vector containing all fields in the given radius. Note: the Object needs to be deleted by the reciever
     */
    static QmlVectorPoint* getCircle(qint32 min, qint32 max);
    /**
     * @brief getShotFields
     * @param min
     * @param max
     * @param xDirection
     * @param yDirection
     * @return
     */
    static QmlVectorPoint* getShotFields(qint32 min, qint32 max, qint32 xDirection = 0, qint32 yDirection = 0);
    /**
     * @brief getDistance
     * @param p1
     * @param p2
     * @return
     */
    static qint32 getDistance(QPoint p1, QPoint p2);
    /**
     * @brief getEmptyPointArray
     * @return
     */
    static QmlVectorPoint* getEmptyPointArray();
    /**
     * @brief getGameVersion
     * @return
     */
    static QString getGameVersion()
    {
        return "Version: " + QString::number(MAJOR) + "." + QString::number(MINOR) + "." + QString::number(REVISION);
    }
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
signals:
    void sigText(oxygine::KeyEvent event);
    void sigKeyDown(oxygine::KeyEvent event);
    void sigKeyUp(oxygine::KeyEvent event);

    void sigConsoleText(oxygine::KeyEvent event);
    void sigConsoleKeyDown(oxygine::KeyEvent event);
    void sigConsoleKeyUp(oxygine::KeyEvent event);
    /**
     * @brief sigWindowLayoutChanged
     */
    void sigWindowLayoutChanged();
public slots:
    void changeScreenMode(qint32 mode);
    void changeScreenSize(qint32 width, qint32 heigth);
    qint32 getScreenMode();
private:
    QTranslator m_Translator;
    QTimer m_Timer;
    static Mainapp* m_pMainapp;
    static QRandomGenerator randGenerator;
    static bool m_useSeed;

    static QThread m_Workerthread;
    static QThread m_AudioWorker;
    static QThread m_Networkthread;
    spTCPServer m_pGameServer{nullptr};
    AudioThread* m_Audiothread{new AudioThread()};
    WorkerThread* m_Worker{new WorkerThread()};
    Settings m_Settings;
    void createTrainingData();
};

#endif // MAINAPP_H
