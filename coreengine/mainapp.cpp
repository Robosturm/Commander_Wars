#include <QFile>
#ifdef GRAPHICSUPPORT
#include <QApplication>
#else
#include <QCoreApplication>
#endif
#include <QScreen>
#include <QDir>
#include <QMessageBox>
#include <QThread>
#include <QResource>
#include <QCoreApplication>
#include <QClipboard>

#include "coreengine/userdata.h"
#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"
#include "coreengine/audiothread.h"
#include "coreengine/workerthread.h"
#include "coreengine/globalutils.h"

#include "ui_reader/uifactory.h"

#include "game/gamerecording/gamemapimagesaver.h"

#include "objects/loadingscreen.h"

#include "network/mainserver.h"
#include "network/tcpclient.h"

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
#include "resource_management/movementplanneraddinmanager.h"
#include "resource_management/uimanager.h"

#include "wiki/wikidatabase.h"

Mainapp* Mainapp::m_pMainapp;
QThread Mainapp::m_Workerthread;
QThread Mainapp::m_Networkthread;
QThread Mainapp::m_GameServerThread;
WorkerThread* Mainapp::m_Worker = new WorkerThread();
AudioThread* Mainapp::m_Audiothread = nullptr;
spTCPClient Mainapp::m_slaveClient;

bool Mainapp::m_slave{false};
QMutex Mainapp::m_crashMutex;
const char* const Mainapp::GAME_CONTEXT = "GAME";

#include "network/rsacypherhandler.h"

Mainapp::Mainapp()
{
#ifdef GRAPHICSUPPORT
    setObjectName("Mainapp");
#endif
    m_pMainThread = QThread::currentThread();
    m_pMainapp = this;
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    m_pMainThread->setObjectName("Mainthread");
    m_Workerthread.setObjectName("Workerthread");
    m_Networkthread.setObjectName("Networkthread");
    m_GameServerThread.setObjectName("GameServerThread");
#endif

    connect(this, &Mainapp::sigShowCrashReport, this, &Mainapp::showCrashReport, Qt::QueuedConnection);
    connect(this, &Mainapp::sigChangePosition, this, &Mainapp::changePosition, Qt::QueuedConnection);
    connect(this, &Mainapp::activeChanged, this, &Mainapp::onActiveChanged, Qt::QueuedConnection);
    connect(this, &Mainapp::sigNextStartUpStep, this, &Mainapp::nextStartUpStep, Qt::QueuedConnection);
    connect(this, &Mainapp::sigCreateLineEdit, this, &Mainapp::createLineEdit, Qt::BlockingQueuedConnection);
}

void Mainapp::createLineEdit()
{
#ifdef GRAPHICSUPPORT    
    m_pLineEdit = new QLineEdit();
    m_pLineEdit->setVisible(false);
#endif
}

void Mainapp::shutdown()
{
    if (BuildingSpriteManager::created())
    {
        BuildingSpriteManager::getInstance()->free();
    }
    if (COSpriteManager::created())
    {
        COSpriteManager::getInstance()->free();
    }
    if (GameManager::created())
    {
        GameManager::getInstance()->free();
    }
    if (GameRuleManager::created())
    {
        GameRuleManager::getInstance()->free();
    }
    if (MovementTableManager::created())
    {
        MovementTableManager::getInstance()->free();
    }
    if (TerrainManager::created())
    {
        TerrainManager::getInstance()->free();
    }
    if (UnitSpriteManager::created())
    {
        UnitSpriteManager::getInstance()->free();
    }
    if (WeaponManager::created())
    {
        WeaponManager::getInstance()->free();
    }
    if (BattleAnimationManager::created())
    {
        BattleAnimationManager::getInstance()->free();
    }
    if (COPerkManager::created())
    {
        COPerkManager::getInstance()->free();
    }
    if (WikiDatabase::created())
    {
        WikiDatabase::getInstance()->free();
    }
    if (AchievementManager::created())
    {
        AchievementManager::getInstance()->free();
    }
    if (BackgroundManager::created())
    {
        BackgroundManager::getInstance()->free();
    }
    // FontManager::getInstance()->free();
    if (GameAnimationManager::created())
    {
        GameAnimationManager::getInstance()->free();
    }
    if (ObjectManager::created())
    {
        ObjectManager::getInstance()->free();
    }
    if (ShopLoader::created())
    {
        ShopLoader::getInstance()->free();
    }
    GameWindow::shutdown();
    UiFactory::shutdown();
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
    redrawUi();
    emit sigNextStartUpStep(StartupPhase::Start);
}

void Mainapp::nextStartUpStep(StartupPhase step)
{
    Console::print("Loading startup phase: " + QString::number(step), Console::eDEBUG);
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    m_startUpStep = step;
    bool automaticNextStep = true;
    switch (step)
    {
        case StartupPhase::General:
        {
            pLoadingScreen->moveToThread(&m_Workerthread);
            m_Audiothread = new AudioThread(m_noAudio);
            m_Audiothread->initAudio();
            m_Audiothread->clearPlayList();
            m_Audiothread->loadFolder("resources/music/hauptmenue");
            FontManager::getInstance();
            // load ressources by creating the singletons
            BackgroundManager::getInstance();
            spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
            pLoadingScreen->show();
            pLoadingScreen->setProgress(tr("Checking for new version..."), step  * stepProgress);
            redrawUi();
            break;
        }
        case UpdateManager:
        {
#if defined(GRAPHICSUPPORT) && defined(UPDATESUPPORT)
            if (!getGameVersion().endsWith("dev") && !getSlave() && Settings::getAutomaticUpdates())
            {
                automaticNextStep = false;
                m_gameUpdater = spGameUpdater::create();
            }
#endif
            break;
        }
        case StartupPhase::ObjectManager:
        {
#ifdef UPDATESUPPORT
            m_gameUpdater = nullptr;
#endif
            ObjectManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Building Textures ..."), step  * stepProgress);
            redrawUi();
            break;
        }
        case StartupPhase::Building:
        {
            m_Audiothread->playRandom();
            redrawUi();
            BuildingSpriteManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading CO Textures..."), step  * stepProgress);
            break;
        }
        case StartupPhase::COSprites:
        {
            redrawUi();
            COSpriteManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Animation Textures..."), step  * stepProgress);
            break;
        }
        case StartupPhase::GameAnimations:
        {
            redrawUi();
            GameAnimationManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Game Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::GameManager:
        {
            redrawUi();
            GameManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Rule Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::GameRuleManager:
        {
            redrawUi();
            GameRuleManager::getInstance();
            WeaponManager::getInstance();
            MovementTableManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Terrain Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::TerrainManager:
        {
            redrawUi();
            TerrainManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Units Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::UnitSpriteManager:
        {
            redrawUi();
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
            redrawUi();
            COPerkManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Wiki Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::WikiDatabase:
        {
            redrawUi();
            WikiDatabase::getInstance();
            pLoadingScreen->setProgress(tr("Loading Userdata ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::Userdata:
        {
            redrawUi();
            Userdata::getInstance();
            pLoadingScreen->setProgress(tr("Loading Achievement Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::Achievementmanager:
        {
            redrawUi();
            AchievementManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Shop Textures ..."), step  * stepProgress);
            break;
        }
        case MovementPlannerAddInManager:
        {
            redrawUi();
            MovementPlannerAddInManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Movement planner addin Textures ..."), step  * stepProgress);
            break;
        }
        case UiManager:
        {
            redrawUi();
            UiManager::getInstance();
            pLoadingScreen->setProgress(tr("Loading Ui Textures ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::ShopLoader:
        {
            redrawUi();
            ShopLoader::getInstance();
            pLoadingScreen->setProgress(tr("Loading sounds ..."), step  * stepProgress);
            break;
        }
        case StartupPhase::Sound:
        {
            redrawUi();
            if (!m_noAudio)
            {
                m_Audiothread->createSoundCache();
            }
            pLoadingScreen->setProgress(tr("Loading Scripts ..."), SCRIPT_PROCESS);
            break;
        }
        case StartupPhase::LoadingScripts:
        {
            // start after ressource loading
#ifdef GRAPHICSUPPORT
            m_Networkthread.setObjectName("NetworkThread");
            m_Workerthread.setObjectName("WorkerThread");
#endif
            m_Networkthread.start(QThread::Priority::NormalPriority);
            m_Workerthread.start(QThread::Priority::NormalPriority);
            emit m_Worker->sigStart();
            redrawUi();
            if (!m_noUi)
            {
                // refresh timer cycle before using it.
                Settings::setFramesPerSecond(Settings::getFramesPerSecond());
                m_Timer.start(m_timerCycle, this);
            }
            break;
        }
        case StartupPhase::Finalizing:
        {
            if (!m_slave)
            {
                m_gamepad.init();
            }
            // only launch the server if the rest is ready for it ;)
            if (Settings::getServer() && !m_slave)
            {
                MainServer::getInstance();
                m_GameServerThread.start(QThread::Priority::NormalPriority);
            }
            if (m_slave && m_initScript.isEmpty())
            {
                emit m_Worker->sigStartSlaveGame();
            }
            else
            {
                emit m_Worker->sigShowMainwindow();
            }
            break;
        }
    }
    if (step < StartupPhase::LoadingScripts && automaticNextStep)
    {
        emit sigNextStartUpStep(static_cast<StartupPhase>(static_cast<qint8>(step) + 1));
    }
}

void Mainapp::doScreenshot()
{
#ifdef GRAPHICSUPPORT
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
#endif
}

void Mainapp::doMapshot()
{
    if (beginRendering())
    {
        qint32 i = 0;
        QDir dir("screenshots/");
        dir.mkpath(".");
        while (i < std::numeric_limits<qint32>::max())
        {
            QString filename = "screenshots/mapshot+" + QString::number(i) + ".png";
            if (!QFile::exists(filename))
            {
                GamemapImageSaver::saveMapAsImage(filename);
                break;
            }
            ++i;
        }
    }
}

void Mainapp::changeScreenMode(Settings::ScreenModes mode)
{
#ifdef GRAPHICSUPPORT
    if (m_noUi)
    {
        return;
    }
    CONSOLE_PRINT("Changing screen mode to " + QString::number(static_cast<qint32>(mode)), Console::eDEBUG);
    hide();
    switch (mode)
    {
        case Settings::ScreenModes::Borderless:
        {
            setWindowState(Qt::WindowState::WindowNoState);
            setFlag(Qt::FramelessWindowHint);
            show();
            setPosition(0, 0);
            Settings::setFullscreen(false);
            Settings::setBorderless(true);
            QScreen* screen = QApplication::primaryScreen();
            QRect screenSize = screen->availableGeometry();
            if (screenSize.width() < Settings::getWidth())
            {
                setWidth(screenSize.width());
                Settings::setWidth(screenSize.width() * getActiveDpiFactor());
            }
            if (screenSize.height() < Settings::getHeight())
            {
                setHeight(screenSize.height());
                Settings::setHeight(screenSize.height() * getActiveDpiFactor());
            }
            break;
        }
        case Settings::ScreenModes::FullScreen:
        {
            showFullScreen();
            QScreen* screen = QApplication::primaryScreen();
            QRect screenSize = screen->geometry();
            // set window info
            Settings::setFullscreen(true);
            Settings::setBorderless(false);
            Settings::setWidth(screenSize.width() * getActiveDpiFactor());
            Settings::setHeight(screenSize.height() * getActiveDpiFactor());
            setGeometry(screenSize);
            break;
        }
        default:
        {
            setWindowState(Qt::WindowState::WindowNoState);
            setFlag(Qt::FramelessWindowHint, false);
            Settings::setFullscreen(false);
            Settings::setBorderless(false);
            QScreen* screen = QApplication::primaryScreen();
            QRect screenSize = screen->availableGeometry();
            if (screenSize.width() < Settings::getWidth())
            {
                setWidth(screenSize.width());
                Settings::setWidth(screenSize.width() * getActiveDpiFactor());
            }
            if (screenSize.height() < Settings::getHeight())
            {
                setHeight(screenSize.height());
                Settings::setHeight(screenSize.height() * getActiveDpiFactor());
            }
            showNormal();
        }
    }
    // change screen size after changing the border flags
    changeScreenSize(Settings::getWidth(), Settings::getHeight());
#endif
}

void Mainapp::changeScreenSize(qint32 width, qint32 heigth)
{    
#ifdef GRAPHICSUPPORT
    if (m_noUi)
    {
        return;
    }
    CONSOLE_PRINT("Changing screen size to width: " + QString::number(width) + " height: " + QString::number(heigth), Console::eDEBUG);
    auto ratio = getActiveDpiFactor();
    resize(width / ratio, heigth / ratio);
    setMinimumSize(QSize(width / ratio, heigth / ratio));
    setMaximumSize(QSize(width / ratio, heigth / ratio));

    Settings::setWidth(width);
    Settings::setHeight(heigth);
    Settings::saveSettings();
    initStage();
    emit sigWindowLayoutChanged();
    emit sigChangePosition(QPoint(-1, -1), true);
#endif
}

QPoint Mainapp::mapPosFromGlobal(QPoint pos) const
{
    return mapFromGlobal(pos) * getActiveDpiFactor();
}

QPoint Mainapp::mapPosToGlobal(QPoint pos) const
{
    return mapToGlobal(pos / getActiveDpiFactor());
}

void Mainapp::changePosition(QPoint pos, bool invert)
{
    if (m_noUi)
    {
        return;
    }
    setPosition(position() + pos);
    if (invert)
    {
        emit sigChangePosition(-pos, false);
    }
}

Settings::ScreenModes Mainapp::getScreenMode()
{
    if (Settings::getFullscreen())
    {
        return Settings::ScreenModes::FullScreen;
    }
    else if (Settings::getBorderless())
    {
        return Settings::ScreenModes::Borderless;
    }
    else
    {
        return Settings::ScreenModes::Window;
    }
}

void Mainapp::keyPressEvent(QKeyEvent *event)
{
    if (m_startUpStep >= StartupPhase::Finalizing)
    {
        Qt::Key cur = static_cast<Qt::Key>(event->key());
        if (cur == Settings::getKeyConsole())
        {
            emit Console::getInstance()->sigToggleView();
        }
        else if (cur == Settings::getKey_screenshot())
        {
            doScreenshot();
        }
        else if (cur == Settings::getKey_mapshot())
        {
            doMapshot();
        }
        else
        {
            CONSOLE_PRINT("keyPressEvent", Console::eDEBUG);
            emit sigKeyDown(oxygine::KeyEvent(event));
        }
    }
}

void Mainapp::keyReleaseEvent(QKeyEvent *event)
{
    if (m_startUpStep >= StartupPhase::Finalizing)
    {
        if (!event->isAutoRepeat())
        {
            emit sigKeyUp(oxygine::KeyEvent(event));
        }
    }
}

bool Mainapp::event(QEvent *event)
{
    spFocusableObject pObj(FocusableObject::getFocusedObject());
    bool handled = false;
    if (pObj.get() != nullptr)
    {
        handled = FocusableObject::handleEvent(event);
    }
    if (!handled)
    {
        if (event->type() == QEvent::InputMethod)
        {
#ifdef GRAPHICSUPPORT
            QInputMethodEvent* inputEvent = static_cast<QInputMethodEvent*>(event);
            handled = keyInputMethodEvent(inputEvent);
#else
            handled = oxygine::GameWindow::event(event);
#endif
        }
        else
        {
            handled = oxygine::GameWindow::event(event);
        }
    }
    return handled;
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

void Mainapp::showCrashReport(const QString & log)
{
    static qint32 counter = 0;
    if (QCoreApplication::instance()->thread() == QThread::currentThread())
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

void Mainapp::setNoUi()
{
    m_noUi = true;
    m_Timer.stop();
}

void Mainapp::setNoAudio()
{
    m_noAudio = true;
}

void Mainapp::actAsSlave()
{
    setSlave(true);
    Settings::setServer(false);
    Settings::setUsername("Server");
    m_slaveClient = spTCPClient::create(nullptr);
    m_slaveClient->moveToThread(Mainapp::getInstance()->getNetworkThread());
    CONSOLE_PRINT("Running as slave with name : " + Settings::getSlaveServerName(), Console::eDEBUG);
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
    CONSOLE_PRINT("Creating base dirs in " + userPath, Console::eDEBUG);
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
        "data/customStyles",
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
        QString path = GlobalUtils::makePathRelative(item.canonicalFilePath());
        if (!path.endsWith(".camp"))
        {
            QDir newDir(userPath + path);
            newDir.mkpath(".");
        }
    }
}

void Mainapp::onQuit()
{
    QCoreApplication::processEvents();
    if (m_Workerthread.isRunning())
    {
        m_Worker->deleteLater();
        m_Workerthread.quit();
        m_Workerthread.wait();
    }
    QCoreApplication::processEvents();
    delete m_Audiothread;
    QCoreApplication::processEvents();
    if (m_Networkthread.isRunning())
    {
        m_Networkthread.quit();
        m_Networkthread.wait();
    }
    QCoreApplication::processEvents();
    CONSOLE_PRINT("Shutting down game server", Console::eDEBUG);
    if (m_GameServerThread.isRunning())
    {
        if (MainServer::exists())
        {
            MainServer::getInstance()->release();
        }
        m_GameServerThread.quit();
        m_GameServerThread.wait();
    }
    QCoreApplication::processEvents();
}

Mainapp::StartupPhase Mainapp::getStartUpStep() const
{
    return m_startUpStep;
}

void Mainapp::setInitScript(const QString &newInitScript)
{
    m_initScript = newInitScript;
}

spTCPClient Mainapp::getSlaveClient()
{
    return m_slaveClient;
}

const QString &Mainapp::getInitScript() const
{
    return m_initScript;
}

bool Mainapp::getCreateSlaveLogs() const
{
    return m_createSlaveLogs;
}

void Mainapp::setCreateSlaveLogs(bool create)
{
    m_createSlaveLogs = create;
}

void Mainapp::inputMethodQuery(Qt::InputMethodQuery query, QVariant arg)
{
    FocusableObject::handleInputMethodQuery(query, arg);
}

void Mainapp::slotCursorPositionChanged(int oldPos, int newPos)
{
    emit cursorPositionChanged(oldPos, newPos);
}
