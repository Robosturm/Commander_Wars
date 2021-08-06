#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "3rd_party/oxygine-framework/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/PointerState.h"

#include <qopenglwindow.h>
#include <qopenglfunctions.h>
#include <qmutex.h>
#include <qbasictimer.h>
#include <qthread.h>
#include <QKeyEvent>
#include <QElapsedTimer>

namespace oxygine
{
    class GameWindow : public QOpenGLWindow, public QOpenGLFunctions
    {
        Q_OBJECT
    public:
        explicit GameWindow();
        virtual ~GameWindow() = default;
        spEventDispatcher getDispatcher();

        static QOpenGLContext* getGLContext();
        static GameWindow*  getWindow();
        bool isReady2Render();
        /**
         * @brief quitGame quits this game
         */
        void quitGame()
        {
            m_quit = true;
        }
        void loadResAnim(oxygine::spResAnim pAnim, QImage & image, qint32 columns, qint32  rows, float scaleFactor, bool addTransparentBorder);

        virtual bool isWorker() = 0;
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
            if (!isWorker())
            {
                oxygine::handleErrorPolicy(oxygine::ep_show_error, "pauseRendering not started by worker thread");
            }
            if (m_pausedCounter == 0)
            {
                m_pauseMutex.lock();
                emit sigStopUpdateTimer();
            }
            ++m_pausedCounter;
        }
        /**
         * @brief continueRendering
         */
        void continueRendering()
        {
            if (!isWorker())
            {
                oxygine::handleErrorPolicy(oxygine::ep_show_error, "continueRendering not started by worker thread");
            }
            --m_pausedCounter;
            if (m_pausedCounter == 0)
            {
                m_pauseMutex.unlock();
                if (m_timerCycle > 0)
                {
                    emit sigStartUpdateTimer();
                }
            }
        }
        /**
         * @brief hasCursor
         * @return
         */
        bool hasCursor();
        void shutdown();
        qint32 getTimerCycle() const;

        bool getShuttingDown() const;

        void setShuttingDown(bool newShuttingDown);

    signals:
        void sigLoadSingleResAnim(oxygine::spResAnim pAnim, QImage & image, qint32 columns, qint32 rows, float scaleFactor, bool addTransparentBorder);
        void sigLoadRessources();
        void sigMousePressEvent(oxygine::MouseButton button, qint32 x, qint32 y);
        void sigMouseReleaseEvent(oxygine::MouseButton button, qint32 x, qint32 y);
        void sigWheelEvent(qint32 x, qint32 y);
        void sigMouseMoveEvent(qint32 x, qint32 y);
        void sigStopUpdateTimer();
        void sigStartUpdateTimer();
        void sigWaitOnRelease();
    public slots:
        /**
         * @brief getBrightness
         * @return
         */
        float getBrightness() const;
        /**
         * @brief setBrightness
         * @param brightness
         */
        void setBrightness(float brightness);
        /**
         * @brief getGamma
         * @return
         */
        float getGamma() const;
        /**
         * @brief setGamma
         * @param gamma
         */
        void setGamma(float gamma);
        virtual void initializeGL() override;
    protected slots:
        void loadSingleResAnim(oxygine::spResAnim pAnim, QImage & image, qint32 columns, qint32 rows, float scaleFactor, bool addTransparentBorder);
        virtual void loadRessources(){}
        void stopUpdateTimer()
        {
            m_Timer.stop();
        }
        void startUpdateTimer()
        {
            if (!m_noUi)
            {
                m_Timer.start(m_timerCycle, this);
            }
        }
        void quitApp();
        void waitOnRelease();
    protected:
        virtual void registerResourceTypes();
        virtual void timerEvent(QTimerEvent *) override;
        virtual void paintGL() override;

        virtual void resizeGL(qint32 w, qint32 h) override;
        // input events
        virtual void mousePressEvent(QMouseEvent *event) override;
        virtual void mouseReleaseEvent(QMouseEvent *event) override;
        virtual void wheelEvent(QWheelEvent *event) override;
        virtual void mouseMoveEvent(QMouseEvent *event)override;        
        virtual void touchEvent(QTouchEvent *event) override;

        void updateData();
        bool beginRendering();
        void swapDisplayBuffers();

        void handleZoomGesture(QList<QTouchEvent::TouchPoint> & touchPoints);
    protected:
        bool m_renderEnabled = true;
        spEventDispatcher m_dispatcher;

        bool m_quit{false};
        QBasicTimer m_Timer;
        qint32 m_timerCycle{1};
        QElapsedTimer m_pressDownTime;
        bool m_pressDownTimeRunning{false};

        // touch handling
        bool m_longPressSent{false};
        bool m_touchMousePressSent{false};
        float m_lastZoomValue{1.0f};

        QMutex m_pauseMutex;
        qint32 m_pausedCounter{0};

        static GameWindow* m_window;
        float m_brightness{0.0f};
        float m_gamma{1.0f};
        Qt::HANDLE m_mainHandle{nullptr};

        bool m_shuttingDown{false};
        bool m_noUi{false};
    };
}

#endif // GAMEWINDOW_H
