#pragma once
#include <QTimer>
#include <QThread>
#include <QKeyEvent>
#include <QElapsedTimer>

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/PointerState.h"

#include "windowBase.h"

using spQThread = std::shared_ptr<QThread>;

namespace oxygine
{
class GameWindow : public WindowBase
{
    Q_OBJECT
public:
    explicit GameWindow();
    virtual ~GameWindow() = default;

    static GameWindow* getWindow()
    {
        return m_window;
    }
    bool isReady2Render();
    bool getNoUi() const
    {
        return m_noUi;
    }
    /**
         * @brief quitGame quits this game
         */
    void quitGame()
    {
        m_quit = true;
    }
    void loadResAnim(oxygine::spResAnim pAnim, QImage & image, qint32 columns, qint32  rows, float scaleFactor, bool clamp2Edge = true, quint32 linearFilter = 0);
    /**
         * @brief isEvenScale
         * @param width1
         * @param width2
         * @return
         */
    static bool isEvenScale(qint32 width1, qint32 width2);
    /**
         * @brief pauseRendering
         */
    void pauseRendering()
    {
        if (!isRenderThread())
        {
            Q_ASSERT(isMainThread());
            if (m_pausedCounter == 0)
            {
                QMutexLocker lock(&m_renderSync);
                ++m_pausedCounter;
            }
            else
            {
                ++m_pausedCounter;
            }
        }
    }
    /**
         * @brief continueRendering
         */
    void continueRendering()
    {
        if (!isRenderThread())
        {
            Q_ASSERT(isMainThread());
            Q_ASSERT(m_pausedCounter > 0);
            --m_pausedCounter;
        }
    }
    bool renderingPaused() const
    {
        return m_pausedCounter > 0;
    }

    virtual void shutdown();
    bool getShuttingDown() const;
    void setShuttingDown(bool newShuttingDown);
    void initStage();
    bool hasCursor() const;
    bool isMainThread() const
    {
        return QThread::currentThread() == m_pMainThread || m_pMainThread == nullptr;
    }
    bool isRenderThread() const
    {
#ifdef GRAPHICSUPPORT
        return QThread::currentThread() == m_renderThread.get() || m_renderThread.get() == nullptr;
#else
        return true;
#endif
    }
    virtual void launchGame() override;
    /**
         * @brief getBrightness
         * @return
         */
    Q_INVOKABLE float getBrightness() const;
    /**
         * @brief setBrightness
         * @param brightness
         */
    Q_INVOKABLE void setBrightness(float brightness);
    /**
         * @brief getGamma
         * @return
         */
    Q_INVOKABLE float getGamma() const;
    /**
         * @brief setGamma
         * @param gamma
         */
    Q_INVOKABLE void setGamma(float gamma);
    QThread * getMainThread() const;

signals:
    void sigLoadSingleResAnim(oxygine::spResAnim pAnim, QImage image, qint32 columns, qint32 rows, float scaleFactor, bool clamp2Edge = true, quint32 linearFilter = 0);
    void sigLoadRessources();
    void sigMousePressEvent(oxygine::MouseButton button, qint32 x, qint32 y);
    void sigMouseReleaseEvent(oxygine::MouseButton button, qint32 x, qint32 y);
    void sigWheelEvent(qint32 x, qint32 y);
    void sigMouseMoveEvent(qint32 x, qint32 y);
    void sigStopUpdateTimer();
    void sigStartUpdateTimer();
    void sigQuit(qint32 exitCode);
    void sigShowKeyboard(bool visible);

protected slots:
    virtual void loadRessources(){}

    void quitApp();
    virtual void onQuit() = 0;
    void quit(qint32 exitCode);
    void showKeyboard(bool visible);
    void mouseMoveEventDelayed();

protected:
    virtual void registerResourceTypes();
    // input events
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event)override;
    virtual void touchEvent(QTouchEvent *event) override;
    virtual void updateData() override;

    void handleZoomGesture(QList<QTouchEvent::TouchPoint> & touchPoints);
    bool sameTouchpoint(const QPointF & pos1, const QPointF & pos2) const;
    void mouseMoveEvent(int x, int y);

protected:
    QTimer m_mouseDelayTimer;
    QPoint m_lastMousePosition;
    QElapsedTimer m_pressDownTime;
    bool m_pressDownTimeRunning{false};

    // touch handling
    bool m_longPressSent{false};
    bool m_touchMousePressSent{false};
    float m_lastZoomValue{1.0f};

    static GameWindow* m_window;
    float m_brightness{0.0f};
    float m_gamma{1.0f};

    bool m_shuttingDown{false};
    bool m_launched{false};
    bool m_workerLaunched{false};
    QThread* m_pMainThread{nullptr};
    struct
    {
        QElapsedTimer leftDown;
        QElapsedTimer rightDown;
        QElapsedTimer middleDown;
        QElapsedTimer leftUp;
        QElapsedTimer rightUp;
        QElapsedTimer middleUp;
    } m_debounceTimer;
};
}
