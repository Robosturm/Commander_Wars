#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "../oxygine-include.h"

#include <qopenglwindow.h>
#include <qopenglfunctions.h>
#include <qmutex.h>
#include <qbasictimer.h>

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
            m_Mutex.lock();
        }
        /**
         * @brief continueThread continues rendering
         */
        inline void continueThread()
        {
            m_Mutex.unlock();
        }
        /**
         * @brief quitGame quits this game
         */
        void quitGame()
        {
            m_quit = true;
        }
    protected:
        virtual void timerEvent(QTimerEvent *) override;
        virtual void paintGL() override;
        virtual void initializeGL() override;
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


        QBasicTimer timer;
        bool _useTouchAPI = false;
        bool _renderEnabled = true;
        spEventDispatcher _dispatcher;

        bool m_quit{false};
        QMutex m_Mutex{QMutex::RecursionMode::Recursive};

        static GameWindow* _window;
    };
}

#endif // GAMEWINDOW_H
