#include "mainapp.h"
#include <QRandomGenerator>

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

#include "game/gamemap.h"

Mainapp* Mainapp::m_pMainapp;
QRandomGenerator Mainapp::randGenerator;
QThread Mainapp::m_Workerthread;
QThread Mainapp::m_AudioWorker;
QThread Mainapp::m_Networkthread;
QThread Mainapp::m_GameServerThread;
bool Mainapp::m_slave{false};
bool Mainapp::m_useSeed{false};
quint32 Mainapp::m_seed = 0;
QMutex Mainapp::crashMutex;

Mainapp::Mainapp()
    : m_Audiothread(new AudioThread()),
      m_Worker(new WorkerThread())
{
    pMainThread = QThread::currentThread();
    m_pMainapp = this;
    Interpreter::setCppOwnerShip(this);
    quint32 seedValue = QRandomGenerator::global()->bounded(0u, std::numeric_limits<quint32>::max());
    randGenerator.seed(seedValue);

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

void Mainapp::seed(quint32 seed)
{
    m_seed = seed;
    randGenerator.seed(seed);
    Console::print("Seeding with " + QString::number(m_seed), Console::eDEBUG);
}

quint32 Mainapp::getSeed()
{
    return m_seed;
}

qint32 Mainapp::randInt(qint32 low, qint32 high)
{
    if (high <= low)
    {
        return low;
    }
    if (m_useSeed)
    {
        return randGenerator.bounded(low, high + 1);
    }
    else
    {
        return randIntBase(low, high);
    }
}


qint32 Mainapp::randIntBase(qint32 low, qint32 high)
{
    if (high <= low)
    {
        return low;
    }
    return QRandomGenerator::global()->bounded(low, high + 1);
}

qint32 Mainapp::roundUp(float value)
{
    qint32 roundDown = static_cast<qint32>(value);
    // little cheat
    if (static_cast<float>(roundDown) == value)
    {
        return roundDown;
    }
    else
    {
        if (value > 0)
        {
            return roundDown + 1;
        }
        else
        {
            return roundDown;
        }
    }
}

qint32 Mainapp::roundDown(float value)
{
    qint32 roundDown = static_cast<qint32>(value);
    return roundDown;
}

QmlVectorPoint* Mainapp::getCircle(qint32 min, qint32 max)
{
    QmlVectorPoint* ret = new QmlVectorPoint();
    qint32 x2 = 0;
    qint32 y2 = 0;

        for (qint32 currentRadius = min; currentRadius <= max; currentRadius++)
        {
            x2 = -currentRadius;
            y2 = 0;
            if (currentRadius == 0)
            {
                ret->append(QPoint(0, 0));
            }
            else
            {
                for (qint32 i = 0; i < currentRadius; i++)
                {
                    x2 += 1;
                    y2 += 1;
                    ret->append(QPoint(x2, y2));
                }
                for (qint32 i = 0; i < currentRadius; i++)
                {
                    x2 += 1;
                    y2 -= 1;
                    ret->append(QPoint(x2, y2));
                }
                for (qint32 i = 0; i < currentRadius; i++)
                {
                    x2 -= 1;
                    y2 -= 1;
                    ret->append(QPoint(x2, y2));
                }
                for (qint32 i = 0; i < currentRadius; i++)
                {
                    x2 -= 1;
                    y2 += 1;
                    ret->append(QPoint(x2, y2));
                }
            }
        }
    return ret;
}

QmlVectorPoint* Mainapp::getShotFields(qint32 min, qint32 max, qint32 xDirection, qint32 yDirection)
{
    QmlVectorPoint* ret = new QmlVectorPoint();
    for (qint32 i = min; i <= max; i++)
    {
        if (xDirection > 0)
        {
            for (qint32 y = -i + 1; y < i; y++)
            {
                ret->append(QPoint(i, y));
            }
        }
        else if (xDirection < 0)
        {
            for (qint32 y = -i + 1; y < i; y++)
            {
                ret->append(QPoint(-i, y));
            }
        }

        if (yDirection > 0)
        {
            for (qint32 x = -i + 1; x < i; x++)
            {
                ret->append(QPoint(x, i));
            }
        }
        else if (yDirection < 0)
        {
            for (qint32 x = -i + 1; x < i; x++)
            {
                ret->append(QPoint(x, -i));
            }
        }
    }
    return ret;
}

void Mainapp::setImageSize(qint32 value)
{
    GameMap::setImagesize(value);
}

qint32 Mainapp::getDistance(QPoint p1, QPoint p2)
{
    return qAbs(p1.x() - p2.x()) + qAbs(p1.y() - p2.y());
}

QmlVectorPoint* Mainapp::getEmptyPointArray()
{
    return new QmlVectorPoint();
}

bool Mainapp::isEven(qint32 value)
{
    float val = value/2.0f;
    qint32 val2 = value/2;
    if(val == val2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Mainapp::storeList(QString file, QStringList items, QString folder)
{
    QDir dir(folder);
    dir.mkpath(".");
    QFile dataFile(folder + file + ".bl");
    dataFile.open(QIODevice::WriteOnly);
    QDataStream stream(&dataFile);
    stream << file;
    stream << static_cast<qint32>(items.size());
    for (qint32 i = 0; i < items.size(); i++)
    {
        stream << items[i];
    }
}

std::tuple<QString, QStringList> Mainapp::readList(QString file, QString folder)
{
    return readList(folder + file);
}

std::tuple<QString, QStringList> Mainapp::readList(QString file)
{
    QFile dataFile(file);
    dataFile.open(QIODevice::ReadOnly);
    QDataStream stream(&dataFile);
    std::tuple<QString, QStringList> ret;
    stream >> std::get<0>(ret);
    qint32 size = 0;
    stream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        QString name;
        stream >> name;
        std::get<1>(ret).append(name);
    }
    return ret;
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

bool Mainapp::getUseSeed()
{
    return m_useSeed;
}

void Mainapp::setUseSeed(bool useSeed)
{
    m_useSeed = useSeed;
}

void Mainapp::createTrainingData()
{
    QFile file("data.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    for (qint32 i = 0; i < 800; i++)
    {
        stream << QRandomGenerator::global()->bounded(4.0) << " ";
        stream << QRandomGenerator::global()->bounded(0.4) << " ";
        qint32 chance = QRandomGenerator::global()->bounded(0, 100);
        if (chance < 40)
        {
            stream << 1 << " ";
            stream << 0 << " ";
            stream << 0 << " ";
        }
        else if (chance < 80)
        {
            stream << 0 << " ";
            stream << 1 << " ";
            stream << 0 << " ";
        }
        else
        {
            if (chance < 95)
            {
                stream << 0 << " ";
                stream << 1 << " ";
                stream << 1 << " ";
            }
            else
            {
                stream << 1 << " ";
                stream << 0 << " ";
                stream << 1 << " ";
            }
        }
        stream << QRandomGenerator::global()->bounded(2.0) << " ";
        stream << QRandomGenerator::global()->bounded(2.0) << " ";
        stream << QRandomGenerator::global()->bounded(0, 2) << " ";
        stream << QRandomGenerator::global()->bounded(20, 100) << " ";
        stream << "\n";
    }
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

QStringList Mainapp::getActiveMods()
{
    return Settings::getMods();
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
