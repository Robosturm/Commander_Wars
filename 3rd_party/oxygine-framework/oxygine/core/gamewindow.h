#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "../oxygine-include.h"
#include "../PointerState.h"

#include <qopenglwindow.h>
#include <qopenglfunctions.h>
#include <qmutex.h>
#include <qbasictimer.h>
#include <qthread.h>

#include "QKeyEvent"

namespace oxygine
{
    class GameWindow : public QOpenGLWindow, public QOpenGLFunctions
    {
        Q_OBJECT
    public:
        GameWindow();
        ~GameWindow();
        spEventDispatcher getDispatcher();

        static QOpenGLContext* getGLContext();
        static GameWindow*  getWindow();
        bool isReady2Render();
        /**
         * @brief suspendThread suspends rendering
         */
        inline void suspendThread()
        {
            Q_ASSERT(isWorker());
            m_lockCounter++;
            m_Mutex.lock();
        }
        /**
         * @brief continueThread continues rendering
         */
        inline void continueThread()
        {
            Q_ASSERT(isWorker());
            m_Mutex.unlock();
            m_lockCounter--;
        }
        /**
         * @brief quitGame quits this game
         */
        void quitGame()
        {
            m_quit = true;
        }
        void loadResAnim(oxygine::spResAnim pAnim, const QImage & image);
        /**
         * @brief getLockCounter
         * @return
         */
        qint32 getLockCounter() const;

        virtual bool isWorker() = 0;
        /**
         * @brief isEvenScale
         * @param width1
         * @param width2
         * @return
         */
        static bool isEvenScale(qint32 width1, qint32 width2);
    signals:
        void sigLoadSingleResAnim(oxygine::spResAnim pAnim, const QImage & image);

        void sigMousePressEvent(oxygine::MouseButton button, qint32 x, qint32 y);
        void sigMouseReleaseEvent(oxygine::MouseButton button, qint32 x, qint32 y);
        void sigWheelEvent(qint32 x, qint32 y);
        void sigMouseMoveEvent(qint32 x, qint32 y);
    protected slots:
        void loadSingleResAnim(oxygine::spResAnim pAnim, const QImage & image);

    protected:
        virtual void initializeGL() override;
        virtual void registerResourceTypes();
        virtual void timerEvent(QTimerEvent *) override;
        virtual void paintGL() override;

        virtual void resizeGL(int w, int h) override;
        // input events
        virtual void mousePressEvent(QMouseEvent *event) override;
        virtual void mouseReleaseEvent(QMouseEvent *event) override;
        virtual void wheelEvent(QWheelEvent *event) override;
        virtual void mouseMoveEvent(QMouseEvent *event)override;

        virtual void loadRessources(){}
        void updateData();
        bool beginRendering();
        void swapDisplayBuffers();

        bool _renderEnabled = true;
        spEventDispatcher _dispatcher;

        bool m_quit{false};
        QMutex m_Mutex{QMutex::RecursionMode::Recursive};
        QBasicTimer m_Timer;

        static GameWindow* _window;

        qint32 m_lockCounter{0};
    };
}

#endif // GAMEWINDOW_H
