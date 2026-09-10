#pragma once

#include <QObject>

namespace oxygine
{
    class WindowBase;
    class Renderer : public QObject
    {
        Q_OBJECT
    public:
        Renderer(WindowBase & window);
        virtual ~Renderer();

        bool beginRendering();
    signals:    
        void sigPaintGl();
        void sigLoadResources(qint32 step);
        void sigStart();
        void sigResize(qint32 w, qint32 h);
    public slots:
        void onPaintGl();
        void loadResources(qint32 step);
        void start();
        void resize(qint32 w, qint32 h);
    private:
        WindowBase & m_window;
        qint32 m_repeatedFramesDropped{0};
    };
}

