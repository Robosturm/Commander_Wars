#include "mainapp.h"
#include "game/player.h"
#include "game/co.h"

#include "wiki/wikidatabase.h"

#include "coreengine/userdata.h"

#include "qfile.h"
#include "qguiapplication.h"
#include "qscreen.h"
#include "qdir.h"

#include "qmessagebox.h"
#include "qthread.h"

#include "coreengine/interpreter.h"
#include "coreengine/audiothread.h"
#include "coreengine/workerthread.h"

#include "network/mainserver.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/gameanimationmanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/gamerulemanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/terrainmanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/battleanimationmanager.h"
#include "resource_management/coperkmanager.h"
#include "resource_management/achievementmanager.h"


Mainapp* Mainapp::m_pMainapp;
QThread Mainapp::m_Workerthread;
QThread Mainapp::m_AudioWorker;
QThread Mainapp::m_Networkthread;
QThread Mainapp::m_GameServerThread;
bool Mainapp::m_slave{false};
QMutex Mainapp::crashMutex;

Mainapp::Mainapp()
    : m_Audiothread(new AudioThread()),
      m_Worker(new WorkerThread())
{
    pMainThread = QThread::currentThread();
    m_pMainapp = this;
    Interpreter::setCppOwnerShip(this);


    connect(this, &Mainapp::sigShowCrashReport, this, &Mainapp::showCrashReport, Qt::QueuedConnection);
    connect(this, &Mainapp::sigChangePosition, this, &Mainapp::changePosition, Qt::QueuedConnection);
    connect(this, &Mainapp::activeChanged, this, &Mainapp::onActiveChanged, Qt::QueuedConnection);
    connect(this, &Mainapp::sigApplyFilter, this, &Mainapp::applyFilter, Qt::BlockingQueuedConnection);
}

Mainapp::~Mainapp()
{
    m_Audiothread->deleteLater();
    m_Worker->deleteLater();
    m_Workerthread.quit();
    m_Workerthread.wait();
    m_AudioWorker.quit();
    m_AudioWorker.wait();
    m_Networkthread.quit();
    m_Networkthread.wait();
}

bool Mainapp::isWorker()
{
    return QThread::currentThread() == &m_Workerthread ||
           QThread::currentThread() == pMainThread;
}

Mainapp* Mainapp::getInstance()
{
    return m_pMainapp;
}

void Mainapp::loadRessources()
{
    // load ressources by creating the singletons
    BackgroundManager::getInstance();
    BuildingSpriteManager::getInstance();
    COSpriteManager::getInstance();
    FontManager::getInstance();
    GameAnimationManager::getInstance();
    GameManager::getInstance();
    GameRuleManager::getInstance();
    ObjectManager::getInstance();
    TerrainManager::getInstance();
    UnitSpriteManager::getInstance();
    BattleAnimationManager::getInstance();
    COPerkManager::getInstance();
    WikiDatabase::getInstance();
    Userdata::getInstance();
    AchievementManager::getInstance();
    applyFilter(Settings::getSpriteFilter());
    // start after ressource loading
    m_AudioWorker.setObjectName("AudioThread");
    m_Networkthread.setObjectName("NetworkThread");
    m_Workerthread.setObjectName("WorkerThread");
    m_AudioWorker.start(QThread::Priority::LowPriority);
    m_Networkthread.start(QThread::Priority::NormalPriority);
    m_Workerthread.start(QThread::Priority::TimeCriticalPriority);
    emit m_Audiothread->sigInitAudio();
    emit m_Worker->sigStart();
    while (!m_Worker->getStarted())
    {
        QThread::msleep(100);
    }
    // only launch the server if the rest is ready for it ;)
    if (Settings::getServer() && !m_slave)
    {
        MainServer::getInstance();
        m_GameServerThread.start(QThread::Priority::TimeCriticalPriority);
    }
    if (!m_noUi)
    {
        m_Timer.start(1, this);
    }
    if (!m_slave)
    {
        emit m_Worker->sigShowMainwindow();
    }
    else
    {
        emit m_Worker->sigStartSlaveGame();
    }
}

void Mainapp::applyFilter(quint32 filter)
{
    // load ressources by creating the singletons
    BuildingSpriteManager::getInstance()->setLinearFilter(filter);
    COSpriteManager::getInstance()->setLinearFilter(filter);
    GameAnimationManager::getInstance()->setLinearFilter(filter);
    GameManager::getInstance()->setLinearFilter(filter);
    GameRuleManager::getInstance()->setLinearFilter(filter);
    TerrainManager::getInstance()->setLinearFilter(filter);
    UnitSpriteManager::getInstance()->setLinearFilter(filter);
    BattleAnimationManager::getInstance()->setLinearFilter(filter);
    COPerkManager::getInstance()->setLinearFilter(filter);
    WikiDatabase::getInstance()->setLinearFilter(filter);
}

void Mainapp::doScreenshot()
{
    auto* currentScreen = screen();
    if (currentScreen != nullptr)
    {
        auto picture = currentScreen->grabWindow(winId());
        qint32 i = 0;
        QDir dir("screenshots/");
        dir.mkpath(".");
        while (i < std::numeric_limits<qint32>::max())
        {
            QString filename = "screenshots/screenshot+" + QString::number(i) + ".png";
            if (!QFile::exists(filename))
            {
                picture.save(filename);
                break;
            }
            ++i;
        }
    }
}

void Mainapp::changeScreenMode(qint32 mode)
{
    hide();
    switch (mode)
    {
        case 1:
        {
            setWindowState(Qt::WindowState::WindowNoState);
            setFlag(Qt::FramelessWindowHint);
            show();
            Settings::setFullscreen(false);
            Settings::setBorderless(true);
            break;
        }
        case 2:
        {
            showFullScreen();
            QScreen *screen = QGuiApplication::primaryScreen();
            QSize  screenSize = screen->availableSize ();
            // set window info
            Settings::setFullscreen(true);
            Settings::setBorderless(true);
            Settings::setWidth(screenSize.width());
            Settings::setHeight(screenSize.height());
            break;
        }
        default:
        {
            setWindowState(Qt::WindowState::WindowNoState);
            setFlag(Qt::FramelessWindowHint, false);
            showNormal();
            Settings::setFullscreen(false);
            Settings::setBorderless(false);
        }
    }
    // change screen size after changing the border flags
    changeScreenSize(Settings::getWidth(), Settings::getHeight());
}

void Mainapp::changeScreenSize(qint32 width, qint32 heigth)
{    
    resize(width, heigth);
    setMinimumSize(QSize(width, heigth));
    setMaximumSize(QSize(width, heigth));
    Settings::setWidth(width);
    Settings::setHeight(heigth);
    if (oxygine::Stage::instance.get() != nullptr)
    {
        oxygine::Stage::instance->setSize(width, heigth);
    }
    Settings::saveSettings();
    emit sigWindowLayoutChanged();
    emit sigChangePosition(QPoint(-1, -1), true);
}

void Mainapp::changePosition(QPoint pos, bool invert)
{
    setPosition(position() + pos);
    if (invert)
    {
        emit sigChangePosition(-pos, false);
    }
}

qint32 Mainapp::getScreenMode()
{
    if (Settings::getFullscreen())
    {
        return 2;
    }
    else if (Settings::getBorderless())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Mainapp::keyPressEvent(QKeyEvent *event)
{
    if (Console::getInstance()->getVisible())
    {
        emit sigConsoleKeyDown(oxygine::KeyEvent(event));
    }
    else
    {
        Qt::Key cur = static_cast<Qt::Key>(event->key());
        if (cur == Settings::getKeyConsole())
        {
            Console::getInstance()->toggleView();
        }
        else if (cur == Settings::getKey_screenshot())
        {
            doScreenshot();
        }
        else
        {
            emit sigKeyDown(oxygine::KeyEvent(event));
        }
    }
}

void Mainapp::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        if (Console::getInstance()->getVisible())
        {
            emit sigConsoleKeyUp(oxygine::KeyEvent(event));
        }
        else
        {
            emit sigKeyUp(oxygine::KeyEvent(event));
        }
    }
}

bool Mainapp::getNoUi() const
{
    return m_noUi;
}

QThread* Mainapp::getGameServerThread()
{
    return &m_GameServerThread;
}

bool Mainapp::getSlave()
{
    return m_slave;
}

void Mainapp::setSlave(bool slave)
{
    m_slave = slave;
}

void Mainapp::showCrashReport(QString log)
{
    static qint32 counter = 0;
    if (QGuiApplication::instance()->thread() == QThread::currentThread())
    {
        // gui thread cool show the crash report
        QString title = tr("Whoops Sturm crashed a meteor into the PC.");
        // QMessageBox::critical(nullptr, title, log);
        QMessageBox criticalBox;
        criticalBox.setIcon(QMessageBox::Critical);
        criticalBox.setWindowTitle(title);
        criticalBox.setTextFormat(Qt::RichText);
        criticalBox.setText(tr("Please use the details or the crashlog to report a bug at \n") +
                            "<a href='https://github.com/Robosturm/Commander_Wars/issues'>https://github.com/Robosturm/Commander_Wars/issues</a>" +
                            tr("\n The game will be terminated sadly. :("));
        criticalBox.setDetailedText(log);
        criticalBox.exec();

        if (counter > 0)
        {
            // unlock crashed process
            counter--;
            crashMutex.unlock();
        }
    }
    else
    {
        // swap to gui thread
        counter++;
        crashMutex.lock();
        emit Mainapp::getInstance()->sigShowCrashReport(log);
        // lock crash thread
        crashMutex.lock();
        crashMutex.unlock();
    }
}

void Mainapp::loadArgs(const QStringList & args)
{
    if (args.contains("-mods"))
    {
        Settings::setActiveMods(args[args.indexOf("-mods") + 1].split(","));
    }
    if (args.contains("-slave"))
    {
        setSlave(true);
        Settings::setServer(false);
        Settings::setUsername("Server");
    }
    if (args.contains("-noui"))
    {
        m_noUi = true;
        Settings::setBattleAnimations(GameEnums::BattleAnimationMode_Overworld);
        Settings::setShowAnimations(GameEnums::AnimationMode_None);
        Settings::setAnimationSpeed(100);
        Settings::setWalkAnimationSpeed(100);
        Settings::setBattleAnimationSpeed(100);
        Settings::setTotalVolume(0);
        m_Timer.stop();
    }
    if (args.contains("-slaveServer"))
    {
        Settings::setSlaveServerName(args[args.indexOf("-slaveServer") + 1]);
    }
}

void Mainapp::onActiveChanged()
{
    if (!isActive())
    {
        FocusableObject::looseFocus();
    }
}
