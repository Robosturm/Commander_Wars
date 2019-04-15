#include "renderthread.h"

#include "oxygine-framework.h"

#include "mainapp.h"

RenderThread::RenderThread()
{
    Interpreter::setCppOwnerShip(this);
    // create update timer
    m_Timer.setSingleShot(false);
    m_Timer.start(5);
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()), Qt::QueuedConnection);
}

RenderThread::~RenderThread()
{
    terminate();
    wait();
}

void RenderThread::run()
{
    while (true)
    {
        exec();
    }
}

void RenderThread::update()
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
    if (done)
    {
        Mainapp::getInstance()->quit();
    }
}
