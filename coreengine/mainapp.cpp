#include "mainapp.h"

#include "ox/oxygine.hpp"
#include "ox/Stage.hpp"

Mainapp* Mainapp::m_pMainapp = nullptr;

Mainapp::Mainapp(int argc, char* argv[])
    : QCoreApplication(argc, argv)
{
    // create update timer
    m_Timer.setSingleShot(false);
    m_Timer.start(5);
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()));
    m_pMainapp = this;
}

Mainapp* Mainapp::getInstance()
{
    return m_pMainapp;
}

qint32 Mainapp::randInt(qint32 low, qint32 high)
{
    if ((high + 1) - low != 0)
    {
    // Random number between low and high
        return qrand() % ((high + 1) - low) + low;
    }
    else
    {
        return low;
    }
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
    if (done)
    {
        exit();
    }
}

void Mainapp::setup()
{
    oxygine::EventCallback cb = CLOSURE(this, &Mainapp::onEvent);
    ox::core::getDispatcher()->addEventListener(ox::core::EVENT_SYSTEM, cb);
}

void Mainapp::onEvent(oxygine::Event* ev)
{
    SDL_Event* event = static_cast<SDL_Event*>(ev->userData);

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
