#include "testgamewindow.h"

#include "3rd_party/oxygine-framework/oxygine/core/renderer.h"

TestGameWindow::TestGameWindow()
    : oxygine::GameWindow()
{
}

void TestGameWindow::startRenderThread()
{
    // same wiring as WindowBase::setupRendering(), minus the GL context handling
    m_renderer.connectSignals();
    m_renderer.moveToThread(m_renderThread.get());
    m_renderThread->start(QThread::Priority::HighestPriority);
    m_renderingInitialized = true;
    // intentionally no emit m_renderer.sigStart(): Renderer::start() initializes
    // the GL driver stack which tests neither need nor support headless.
}

void TestGameWindow::launch()
{
    launchGame();
}

void TestGameWindow::shutdownTest()
{
    m_renderThread->quit();
    m_renderThread->wait();
    shutdown();
}
