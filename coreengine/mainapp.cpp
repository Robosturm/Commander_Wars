#include "mainapp.h"

#include "ox/oxygine.hpp"
#include "ox/Stage.hpp"

#include "network/NetworkInterface.h"
#include "network/tcpclient.h"
#include "network/tcpserver.h"
#include <QRandomGenerator>

#include "game/player.h"
#include "game/co.h"

Mainapp* Mainapp::m_pMainapp = nullptr;
QRandomGenerator Mainapp::randGenerator;

Mainapp::Mainapp(int argc, char* argv[])
    : QCoreApplication(argc, argv),
      m_Audiothread(new AudioThread)
{
    Interpreter::setCppOwnerShip(this);
    // create update timer
    m_Timer.setSingleShot(false);
    m_Timer.start(5);
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()), Qt::QueuedConnection);
    m_pMainapp = this;

    m_Audiothread->start();
    quint32 seedValue = QRandomGenerator::global()->bounded(0u, std::numeric_limits<quint32>::max());
    randGenerator.seed(seedValue);
}

Mainapp::~Mainapp()
{
    m_Audiothread->deleteLater();
}

Mainapp* Mainapp::getInstance()
{
    return m_pMainapp;
}

qint32 Mainapp::randInt(qint32 low, qint32 high)
{
    return randGenerator.bounded(low, high + 1);
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

void Mainapp::quitGame()
{
    m_quit = true;
}

void Mainapp::update()
{
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
    // check for termination
    if (done || m_quit)
    {
        exit();
    }
}

void Mainapp::setup()
{
    oxygine::EventCallback cb = CLOSURE(this, &Mainapp::onEvent);
    ox::core::getDispatcher()->addEventListener(ox::core::EVENT_SYSTEM, cb);
    setupNetwork();
    m_Settings.setup();
}

void Mainapp::onEvent(oxygine::Event* ev)
{
    SDL_Event* event = reinterpret_cast<SDL_Event*>(ev->userData);

    if (event->type == SDL_KEYDOWN)
    {
        emit sigKeyDown(event);
    }
    if (event->type == SDL_TEXTINPUT)
    {
        emit sigText(event);
    }
    else if (event->type == SDL_KEYUP)
    {
        emit sigKeyUp(event);
    }
}

void Mainapp::setupNetwork()
{
    if (m_pNetworkInterface != nullptr)
    {
        m_pNetworkInterface->deleteLater();
        m_pNetworkInterface = nullptr;
    }
    if (m_Settings.getServer())
    {
        m_pNetworkInterface = new TCPServer();
    }
    else
    {
        m_pNetworkInterface = new TCPClient();
    }
    m_pNetworkInterface->start(QThread::HighPriority);
}
