#pragma once

#include <QThread>

#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

/**
 * @brief Minimal GameWindow for unit tests.
 * Wires the renderer onto the render thread exactly like
 * WindowBase::setupRendering(), but skips all OpenGL context handling so the
 * tests run headless (e.g. with QT_QPA_PLATFORM=offscreen).
 * Renderer::start() is intentionally never emitted: it initializes the GL
 * driver stack which the tests neither need nor support without a GL context.
 */
class TestGameWindow final : public oxygine::GameWindow
{
public:
    explicit TestGameWindow();
    ~TestGameWindow() override = default;

    /**
     * @brief startRenderThread moves the renderer to the render thread and starts it.
     * After this call queued actor updates can be drained via the renderer.
     */
    void startRenderThread();
    /**
     * @brief launch creates the oxygine stage (GameWindow::launchGame).
     */
    void launch();
    /**
     * @brief shutdownTest stops the render thread and shuts down the oxygine singletons.
     */
    void shutdownTest();

protected:
    void onQuit() override
    {
    }
};
