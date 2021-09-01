#include "qfile.h"
#include "qguiapplication.h"
#include "qscreen.h"
#include "qdir.h"
#include "qmessagebox.h"
#include "qthread.h"
#include "qresource.h"
#include <QApplication>

#include "coreengine/userdata.h"
#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"
#include "coreengine/audiothread.h"
#include "coreengine/workerthread.h"
#include "coreengine/globalutils.h"

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
#include "resource_management/movementtablemanager.h"
#include "resource_management/weaponmanager.h"
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
    createBaseDirs();
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
}

void Mainapp::shutdown()
{
    BuildingSpriteManager::getInstance()->free();
    COSpriteManager::getInstance()->free();
    GameManager::getInstance()->free();
    GameRuleManager::getInstance()->free();
    MovementTableManager::getInstance()->free();
    TerrainManager::getInstance()->free();
    UnitSpriteManager::getInstance()->free();
    WeaponManager::getInstance()->free();
    BattleAnimationManager::getInstance()->free();
    COPerkManager::getInstance()->free();
    WikiDatabase::getInstance()->free();
    AchievementManager::getInstance()->free();
    BackgroundManager::getInstance()->free();
    FontManager::getInstance()->free();
    GameAnimationManager::getInstance()->free();
    ObjectManager::getInstance()->free();
    ShopLoader::getInstance()->free();
    GameWindow::shutdown();
}

bool Mainapp::isWorker()
{
    return QThread::currentThread() == &m_Workerthread ||
           (QThread::currentThread() == m_pMainThread &&
           (m_shuttingDown || !m_Worker->getStarted()));
}

bool Mainapp::isWorkerRunning()
{
    return m_Worker->getStarted();
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
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->moveToThread(&m_Workerthread);
    if (Settings::getSmallScreenDevice())
    {
        m_timerCycle = 33;
    }
    switch (step)
    {
        case StartupPhase::General:
        {
            m_Audiothread = new AudioThread();
            m_AudioWorker.start(QThread::Priority::NormalPriority);
            emit m_Audiothread->sigInitAudio();
            m_Audiothread->clearPlayList();
            m_Audiothread->loadFolder("resources/music/hauptmenue");
            FontManager::getInstance();
            // load ressources by creating the singletons
            BackgroundManager::getInstance();
            spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
            pLoadingScreen->show();
            pLoadingScreen->setProgress(tr("Loading Building Textures..."), step  * stepProgress);
            if (!m_noUi)
            {
                update();
            }
            break;
        }
        case StartupPhase::Building:
        {
            m_Audiothread->playRandom();
            if (!m_noUi)
            {
                update();
            }
            BuildingSpriteManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading CO Textures..."), step  * stepProgress);
            break;
        }
        case StartupPhase::COSprites:
        {
            if (!m_noUi)
            {
                update();
            }
            COSpriteManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Animation Textures..."), step  * stepProgress);
            break;
        }
        case StartupPhase::GameAnimations:
        {
            if (!m_noUi)
            {
                update();
            }
            GameAnimationManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Game Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::GameManager:
        {
            if (!m_noUi)
            {
                update();
            }
            GameManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Rule Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::GameRuleManager:
        {
            if (!m_noUi)
            {
                update();
            }
            GameRuleManager::getInstance();
            WeaponManager::getInstance();
            MovementTableManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Objects Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::ObjectManager:
        {
            if (!m_noUi)
            {
                update();
            }
            ObjectManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Terrains Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::TerrainManager:
        {
            if (!m_noUi)
            {
                update();
            }
            TerrainManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Units Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::UnitSpriteManager:
        {
            if (!m_noUi)
            {
                update();
            }
            UnitSpriteManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Battleanimation Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::BattleAnimationManager:
        {

            BattleAnimationManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading CO-Perk Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::COPerkManager:
        {
            if (!m_noUi)
            {
                update();
            }
            COPerkManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Wiki Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::WikiDatabase:
        {
            if (!m_noUi)
            {
                update();
            }
            WikiDatabase::getInstance();
            pLoadingScreen->setProgress(tr("Loading Userdata ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::Userdata:
        {
            if (!m_noUi)
            {
                update();
            }
            Userdata::getInstance();
            pLoadingScreen->setProgress(tr("Loading Achievement Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::Achievementmanager:
        {
            if (!m_noUi)
            {
                update();
            }
            AchievementManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Shop Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::ShopLoader:
        {
            if (!m_noUi)
            {
                update();
            }
            ShopLoader::getInstance();
            pLoadingScreen->setProgress(tr("Loading sounds ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::Sound:
        {
            if (!m_noUi)
            {
                update();
                emit m_Audiothread->sigCreateSoundCache();
            }
            pLoadingScreen->setProgress(tr("Loading Scripts ..."), SCRIPT_PROCESS);
            break;
        }
        case StartupPhase::LoadingScripts:
        {
            applyFilter(Settings::getSpriteFilter());
            // start after ressource loading
            m_Networkthread.setObjectName("NetworkThread");
            m_Workerthread.setObjectName("WorkerThread");
            m_Networkthread.start(QThread::Priority::NormalPriority);
            m_Workerthread.start(QThread::Priority::NormalPriority);
            emit m_Worker->sigStart();
            if (!m_noUi)
            {
                update();
                m_Timer.start(m_timerCycle, this);
            }
            break;
        }
        case StartupPhase::Finalizing:
        {
            // only launch the server if the rest is ready for it ;)
            if (Settings::getServer() && !m_slave)
            {
                MainServer::getInstance();
                m_GameServerThread.start(QThread::Priority::NormalPriority);
            }
            if (!m_slave)
            {
                emit m_Worker->sigShowMainwindow();
            }
            else
            {
                emit m_Worker->sigStartSlaveGame();
            }
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
    Console::print("Changing screen mode to " + QString::number(mode), Console::eDEBUG);
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
            QScreen* screen = QGuiApplication::primaryScreen();
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
            QScreen* screen = QGuiApplication::primaryScreen();
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
            QScreen* screen = QGuiApplication::primaryScreen();
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
    Console::print("Changing screen size to width: " + QString::number(width) + " height: " + QString::number(heigth), Console::eDEBUG);
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

bool Mainapp::event(QEvent *ev)
{
    if (ev->type() == QEvent::InputMethod)
    {
        QInputMethodEvent* inputEvent = static_cast<QInputMethodEvent*>(ev);
        return keyInputMethodEvent(inputEvent);
    }
    if (ev->type() == QEvent::InputMethodQuery)
    {
        QInputMethodQueryEvent * inputEvent = static_cast<QInputMethodQueryEvent *>(ev);
        return keyInputMethodQueryEvent(inputEvent);
    }
    else
    {
        return oxygine::GameWindow::event(ev);
    }
}

bool Mainapp::keyInputMethodQueryEvent(QInputMethodQueryEvent *event)
{
    if (FocusableObject::getFocusedObject() != nullptr && event->queries() == Qt::ImTextBeforeCursor)
    {
        FocusableObject::getFocusedObject()->keyInputMethodQueryEvent(event);
        event->accept();
        return true;
    }
    return false;
}

bool Mainapp::keyInputMethodEvent(QInputMethodEvent *event)
{
    emit sigKeyDown(oxygine::KeyEvent(event));
    return true;
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
        Settings::setDialogAnimation(false);
        Settings::setDialogAnimationSpeed(100);
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

void Mainapp::createBaseDirs()
{
    QString userPath = Settings::getUserPath();
    Console::print("Creating base dirs in " + userPath, Console::eDEBUG);
    if (!userPath.isEmpty())
    {
        QDir newDir(userPath);
        newDir.mkpath(".");
    }
    QDir dir(userPath + "temp/");
    dir.removeRecursively();
    QStringList dirs =
    {
        "temp",
        "savegames",
        "mods",
        "data/gamerules",
        "data/randommaps",
        "data/records",
        "maps",
        "customTerrainImages",
        "resources",
        "resources/aidata",
        "resources/aidata/very_easy",
        "resources/aidata/normal",
        "resources/aidata/heavy",
    };
    for (const auto & path : qAsConst(dirs))
    {
        QDir newDir(userPath + path);
        newDir.mkpath(".");
    }
    auto virtList = QDir(QString(oxygine::Resource::RCC_PREFIX_PATH) + "maps").entryInfoList(QDir::Dirs);
    for (const auto & item : qAsConst(virtList))
    {
        QString path = GlobalUtils::makePathRelative(item.absoluteFilePath());
        if (!path.endsWith(".camp"))
        {
            QDir newDir(userPath + path);
            newDir.mkpath(".");
        }
    }
}

void Mainapp::onQuit()
{
    QApplication::processEvents();
    if (m_Workerthread.isRunning())
    {
        m_Worker->deleteLater();
        m_Workerthread.quit();
        m_Workerthread.wait();
    }
    if (m_AudioWorker.isRunning())
    {
        m_Audiothread->deleteLater();
        m_AudioWorker.quit();
        m_AudioWorker.wait();
    }
    if (m_Networkthread.isRunning())
    {
        m_Networkthread.quit();
        m_Networkthread.wait();
    }
}
