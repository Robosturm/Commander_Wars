#include "mainapp.h"

#include "network/NetworkInterface.h"
#include "network/tcpclient.h"
#include "network/tcpserver.h"
#include <QRandomGenerator>

#include "game/player.h"
#include "game/co.h"
#include "network/tcpserver.h"

#include "qfile.h"

#include "qdir.h"

Mainapp* Mainapp::m_pMainapp;
QRandomGenerator Mainapp::randGenerator;
QThread Mainapp::m_Workerthread;
QThread Mainapp::m_AudioWorker;
QThread Mainapp::m_Networkthread;
bool Mainapp::m_useSeed{false};

Mainapp::Mainapp(int argc, char* argv[])
    : QCoreApplication(argc, argv)
{
    m_pMainapp = this;
    Interpreter::setCppOwnerShip(this);
    quint32 seedValue = QRandomGenerator::global()->bounded(0u, std::numeric_limits<quint32>::max());
    randGenerator.seed(seedValue);
    // create update timer
    m_Timer.setSingleShot(true);
    connect(&m_Timer, &QTimer::timeout, this, &Mainapp::update, Qt::QueuedConnection);

    emit m_Audiothread->sigInitAudio();
    m_AudioWorker.setObjectName("AudioThread");
    m_Networkthread.setObjectName("NetworkThread");
    m_Workerthread.setObjectName("WorkerThread");
    m_AudioWorker.start(QThread::Priority::LowPriority);
    m_Networkthread.start(QThread::Priority::NormalPriority);
    m_Workerthread.start(QThread::Priority::TimeCriticalPriority);

    // createTrainingData();
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

Mainapp* Mainapp::getInstance()
{
    return m_pMainapp;
}

void Mainapp::seed(quint32 seed)
{
    randGenerator.seed(seed);
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
        return QRandomGenerator::global()->bounded(low, high + 1);
    }
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

void Mainapp::quitGame()
{
    m_quit = true;
}

void Mainapp::start()
{
    emit m_Worker->sigStart();
    while (!m_Worker->getStarted())
    {
        QThread::msleep(100);
    }
    // only launch the server if the rest is ready for it ;)
    if (Settings::getServer())
    {
        m_pGameServer = new TCPServer();
        emit m_pGameServer->sig_connect("", Settings::getServerPort());
    }
    m_Timer.start(1);
}

void Mainapp::update()
{
    m_Mutex.lock();
    // Update engine-internal components
    // If input events are available, they are passed to Stage::instance.handleEvent
    // If the function returns true, it means that the user requested the application to terminate
    bool done = oxygine::core::update();

    // Update our stage
    // Update all actors. Actor::update will also be called for all its children
    oxygine::getStage()->update();

    if (oxygine::core::beginRendering())
    {
        oxygine::Color clearColor(181, 255, 32, 255);
        oxygine::Rect viewport(oxygine::Point(0, 0), oxygine::core::getDisplaySize());
        // Render all actors inside the stage. Actor::render will also be called for all its children
        oxygine::getStage()->render(clearColor, viewport);

        oxygine::core::swapDisplayBuffers();
    }
    m_Mutex.unlock();
    // check for termination
    if (done || m_quit)
    {
        exit();
    }
    m_Timer.start(13);
}

void Mainapp::setup()
{
    oxygine::EventCallback cb = CLOSURE(this, &Mainapp::onEvent);
    oxygine::core::getDispatcher()->addEventListener(oxygine::core::EVENT_SYSTEM, cb);
    m_Settings.setup();
}

void Mainapp::suspendThread()
{
    m_Mutex.lock();
}

void Mainapp::continueThread()
{
    m_Mutex.unlock();
}

void Mainapp::onEvent(oxygine::Event* ev)
{
    SDL_Event* event = reinterpret_cast<SDL_Event*>(ev->userData);

    if (event->type == SDL_KEYDOWN)
    {
        if (Console::getInstance()->getVisible())
        {
            emit sigConsoleKeyDown(*event);
        }
        else
        {
            SDL_Keycode cur = event->key.keysym.sym;
            if (cur == getSettings()->getKeyConsole())
            {
                Console::getInstance()->toggleView();
            }
            else
            {
                emit sigKeyDown(*event);
            }
        }
    }
    if (event->type == SDL_TEXTINPUT)
    {
        if (Console::getInstance()->getVisible())
        {
            emit sigConsoleText(*event);
        }
        else
        {
            emit sigText(*event);
        }
    }
    else if (event->type == SDL_KEYUP)
    {
        if (Console::getInstance()->getVisible())
        {
            emit sigConsoleKeyUp(*event);
        }
        else
        {
            emit sigKeyUp(*event);
        }
    }
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
