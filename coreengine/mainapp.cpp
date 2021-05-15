#include "qfile.h"
#include "qguiapplication.h"
#include "qscreen.h"
#include "qdir.h"
#include "qmessagebox.h"
#include "qthread.h"

#include "coreengine/userdata.h"
#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"
#include "coreengine/audiothread.h"
#include "coreengine/workerthread.h"

#include "objects/loadingscreen.h"

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
#include "resource_management/shoploader.h"
#include "wiki/wikidatabase.h"

Mainapp* Mainapp::m_pMainapp;
QThread Mainapp::m_Workerthread;
QThread Mainapp::m_AudioWorker;
QThread Mainapp::m_Networkthread;
QThread Mainapp::m_GameServerThread;
WorkerThread* Mainapp::m_Worker = new WorkerThread();
AudioThread* Mainapp::m_Audiothread = nullptr;
bool Mainapp::m_slave{false};
QMutex Mainapp::m_crashMutex;
const char* const Mainapp::GAME_CONTEXT = "GAME";

Mainapp::Mainapp()
{
    setObjectName("Mainapp");
    m_pMainThread = QThread::currentThread();
    m_pMainapp = this;
    Interpreter::setCppOwnerShip(this);

    m_pMainThread->setObjectName("Mainthread");
    m_Workerthread.setObjectName("Workerthread");
    m_AudioWorker.setObjectName("AudioWorker");
    m_Networkthread.setObjectName("Networkthread");
    m_GameServerThread.setObjectName("GameServerThread");

    connect(this, &Mainapp::sigShowCrashReport, this, &Mainapp::showCrashReport, Qt::QueuedConnection);
    connect(this, &Mainapp::sigChangePosition, this, &Mainapp::changePosition, Qt::QueuedConnection);
    connect(this, &Mainapp::activeChanged, this, &Mainapp::onActiveChanged, Qt::QueuedConnection);
    connect(this, &Mainapp::sigApplyFilter, this, &Mainapp::applyFilter, Qt::BlockingQueuedConnection);
    connect(this, &Mainapp::sigNextStartUpStep, this, &Mainapp::nextStartUpStep, Qt::QueuedConnection);
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
            QThread::currentThread() == m_pMainThread;
}

void Mainapp::loadRessources()
{
    if (!m_noUi)
    {
        update();
    }
    emit sigNextStartUpStep(StartupPhase::Start);
}

void Mainapp::nextStartUpStep(StartupPhase step)
{
    LoadingScreen* pLoadingScreen = LoadingScreen::getInstance();
    switch (step)
    {
        case StartupPhase::General:
        {
            Mainapp::m_Audiothread = new AudioThread();
            m_AudioWorker.setObjectName("AudioThread");
            m_AudioWorker.start(QThread::Priority::LowPriority);
            emit m_Audiothread->sigInitAudio();
            m_Audiothread->clearPlayList();
            m_Audiothread->loadFolder("resources/music/hauptmenue");
            m_Audiothread->playRandom();
            FontManager::getInstance();
            // load ressources by creating the singletons
            BackgroundManager::getInstance();
            LoadingScreen* pLoadingScreen = LoadingScreen::getInstance();
            pLoadingScreen->show();

            pLoadingScreen->setProgress(tr("Start Loading..."), step  * stepProgress);
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::Building:
        {
            pLoadingScreen->setProgress(tr("Loading Building Textures..."), step  * stepProgress);
            BuildingSpriteManager::getInstance();
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::COSprites:
        {
            pLoadingScreen->setProgress(tr("Loading CO Textures..."), step  * stepProgress);
            COSpriteManager::getInstance();
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::GameAnimations:
        {
            pLoadingScreen->setProgress(tr("Loading Animation Textures..."), step  * stepProgress);
            GameAnimationManager::getInstance();
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::GameManager:
        {
            pLoadingScreen->setProgress(tr("Loading Game Textures ..."), step  * stepProgress);
            GameManager::getInstance();
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::GameRuleManager:
        {
            pLoadingScreen->setProgress(tr("Loading Rule Textures ..."), step  * stepProgress);
            GameRuleManager::getInstance();
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::ObjectManager:
        {
            pLoadingScreen->setProgress(tr("Loading Objects Textures ..."), step  * stepProgress);
            ObjectManager::getInstance();
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::TerrainManager:
        {
            pLoadingScreen->setProgress(tr("Loading Terrains Textures ..."), step  * stepProgress);
            TerrainManager::getInstance();
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::UnitSpriteManager:
        {
            pLoadingScreen->setProgress(tr("Loading Units Textures ..."), step  * stepProgress);
            UnitSpriteManager::getInstance();
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::BattleAnimationManager:
        {
            pLoadingScreen->setProgress(tr("Loading Battleanimation Textures ..."), step  * stepProgress);
            BattleAnimationManager::getInstance();
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::COPerkManager:
        {
            pLoadingScreen->setProgress(tr("Loading CO-Perk Textures ..."), step  * stepProgress);
            COPerkManager::getInstance();
            update();
            break;
        }
        case StartupPhase::WikiDatabase:
        {
            pLoadingScreen->setProgress(tr("Loading Wiki Textures ..."), step  * stepProgress);
            WikiDatabase::getInstance();
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::Userdata:
        {
            pLoadingScreen->setProgress(tr("Loading Userdata ..."), step  * stepProgress);
            Userdata::getInstance();
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::Achievementmanager:
        {
            pLoadingScreen->setProgress(tr("Loading Achievement Textures ..."), step  * stepProgress);
            AchievementManager::getInstance();
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::ShopLoader:
        {
            pLoadingScreen->setProgress(tr("Loading Shop Textures ..."), step  * stepProgress);
            ShopLoader::getInstance();
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::LoadingScripts:
        {
            pLoadingScreen->setProgress(tr("Loading Scripts ..."), SCRIPT_PROCESS);
            applyFilter(Settings::getSpriteFilter());
            // start after ressource loading
            m_Networkthread.setObjectName("NetworkThread");
            m_Workerthread.setObjectName("WorkerThread");
            m_Networkthread.start(QThread::Priority::NormalPriority);
            m_Workerthread.start(QThread::Priority::TimeCriticalPriority);
            emit m_Worker->sigStart();
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::Finalizing:
        {
            if (!m_noUi)
            {
                m_Timer.start(1, this);
            }
            // only launch the server if the rest is ready for it ;)
            if (Settings::getServer() && !m_slave)
            {
                MainServer::getInstance();
                m_GameServerThread.start(QThread::Priority::TimeCriticalPriority);
            }
            pLoadingScreen->hide();
            if (!m_slave)
            {
                emit m_Worker->sigShowMainwindow();
            }
            else
            {
                emit m_Worker->sigStartSlaveGame();
            }
            update();
            break;
        }
    }
    if (step < StartupPhase::LoadingScripts)
    {
        emit sigNextStartUpStep(static_cast<StartupPhase>(static_cast<qint8>(step) + 1));
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
            setPosition(0, 0);
            Settings::setFullscreen(false);
            Settings::setBorderless(true);
            QScreen *screen = QGuiApplication::primaryScreen();
            QRect screenSize = screen->availableGeometry();
            if (screenSize.width() < Settings::getWidth())
            {
                setWidth(screenSize.width());
                Settings::setWidth(screenSize.width());
            }
            if (screenSize.height() < Settings::getHeight())
            {
                setWidth(screenSize.height());
                Settings::setHeight(screenSize.height());
            }
            break;
        }
        case 2:
        {
            showFullScreen();
            QScreen *screen = QGuiApplication::primaryScreen();
            QRect screenSize = screen->geometry();
            // set window info
            Settings::setFullscreen(true);
            Settings::setBorderless(false);
            Settings::setWidth(screenSize.width());
            Settings::setHeight(screenSize.height());
            setGeometry(screenSize);
            break;
        }
        default:
        {
            setWindowState(Qt::WindowState::WindowNoState);
            setFlag(Qt::FramelessWindowHint, false);
            Settings::setFullscreen(false);
            Settings::setBorderless(false);
            QScreen *screen = QGuiApplication::primaryScreen();
            QRect screenSize = screen->availableGeometry();
            if (screenSize.width() < Settings::getWidth())
            {
                setWidth(screenSize.width());
                Settings::setWidth(screenSize.width());
            }
            if (screenSize.height() < Settings::getHeight())
            {
                setWidth(screenSize.height());
                Settings::setHeight(screenSize.height());
            }
            showNormal();
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
            m_crashMutex.unlock();
        }
    }
    else
    {
        // swap to gui thread
        counter++;
        m_crashMutex.lock();
        emit Mainapp::getInstance()->sigShowCrashReport(log);
        // lock crash thread
        m_crashMutex.lock();
        m_crashMutex.unlock();
    }
}

void Mainapp::loadArgs(const QStringList & args)
{
    if (args.contains("-mods"))
    {
        QStringList modList = args[args.indexOf("-mods") + 1].split(",");
        qint32 i= 0;
        while (i < modList.size())
        {
            if (modList[i].isEmpty())
            {
                modList.removeAt(i);
            }
            else
            {
                ++i;
            }
        }
        Settings::setActiveMods(modList);
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

QString Mainapp::qsTr(QString text)
{
    return Mainapp::qsTr(text.toStdString().c_str());
}

QString Mainapp::qsTr(const char* const text)
{
    return QCoreApplication::translate(GAME_CONTEXT, text);
}
