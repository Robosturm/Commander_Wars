#pragma once

#include <QObject>
#include <QImage>
#include <QTimer>
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

class BaseGamemenu;
class Minimap;

namespace oxygine
{
    class WindowBase;
    class Renderer : public QObject
    {
        Q_OBJECT
    public:
        Renderer(WindowBase & window);
        virtual ~Renderer();

        void connectSignals();
        bool beginRendering();

    signals:    
        void sigPaint();
        void sigLoadResources(qint32 step);
        void sigStart();
        void sigResize(qint32 w, qint32 h);
        void sigDoMapshot(BaseGamemenu* pMenu);
        void sigSaveMapAsImage(Minimap* pMinimap, QImage * img);
        void sigSetAddColor(oxygine::spVStyleActor actor, QColor color);
        void sigRebuildText(oxygine::spTextField actor);
        void sigChangeAnimFrame(oxygine::spSprite actor, const oxygine::AnimationFrame& frame);
        void sigSetColorTable(oxygine::spSprite actor, const oxygine::spResAnim pAnim, bool matrix);
        void sigRestartAllTweens(oxygine::spActor actor);
        void sigSyncAllTweens(oxygine::spActor actor, oxygine::timeMS syncTime);
        void sigAddChild(oxygine::spActor parent, oxygine::spActor actor);
        void sigRemoveChild(oxygine::spActor parent, oxygine::spActor actor);
        void sigDetachAndRemove(oxygine::spActor actor);
        void sigDetach(oxygine::spActor actor);
        void sigSetPriority(oxygine::spActor parent, qint32 zorder);
        void sigAddTween(oxygine::spActor parent, oxygine::spTween tween);
        void sigRemoveTween(oxygine::spActor parent, oxygine::spTween tween);
        void sigRemoveChildren(oxygine::spActor actor);
        void sigRemoveTweens(oxygine::spActor actor);
        void sigAddEventListener(oxygine::spEventDispatcher dispatcher, oxygine::eventType et, const oxygine::EventCallback cb);
        void sigRemoveEventListener(oxygine::spEventDispatcher dispatcher, qint32 id);
        void sigRemoveEventListeners(oxygine::spEventDispatcher dispatcher, oxygine::IClosureOwner* callbackThis);
        void sigQuit();
        void sigSetRendering(bool render);
        void sigSetTimerCycle(qint32 newTimerCycle);
    public slots:
        void loadSingleResAnim(oxygine::spResAnim pAnim, QImage image, qint32 columns, qint32 rows, float scaleFactor, bool clamp2Edge = true, quint32 linearFilter = 0);

        // event loop handling
        void mouseMoveEvent(int x, int y);
        void mouseMoveEventDelayed();
        void wheelEvent(qint32 x, qint32 y);
        void mousePressEvent(oxygine::MouseButton button, qint32 x, qint32 y);
        void mouseReleaseEvent(oxygine::MouseButton button, qint32 x, qint32 y);
    private slots:
        void quit();
        void onPaint();
        void loadResources(qint32 step);
        void start();
        void resize(qint32 w, qint32 h);
        void doMapshot(BaseGamemenu* pMenu);
        /**
         * @brief saveMapAsImage
         * @param pMinimap
         * @param img
         */
        void saveMapAsImage(Minimap* pMinimap, QImage * img);
        void setAddColor(oxygine::spVStyleActor actor, QColor color);
        void rebuildText(oxygine::spTextField actor);
        void changeAnimFrame(oxygine::spSprite actor, const oxygine::AnimationFrame& frame);
        void setColorTable(oxygine::spSprite actor, const oxygine::spResAnim pAnim, bool matrix);
        void restartAllTweens(oxygine::spActor actor);
        void syncAllTweens(oxygine::spActor actor, oxygine::timeMS syncTime);
        void addChild(oxygine::spActor parent, oxygine::spActor actor);
        void removeChild(oxygine::spActor parent, oxygine::spActor actor);
        void setPriority(oxygine::spActor parent, qint32 zorder);
        void addTween(oxygine::spActor parent, oxygine::spTween tween);
        void removeTween(oxygine::spActor parent, oxygine::spTween tween);
        void removeChildren(oxygine::spActor actor);
        void removeTweens(oxygine::spActor actor);
        void addEventListener(oxygine::spEventDispatcher dispatcher, oxygine::eventType et, const oxygine::EventCallback cb);
        void removeEventListener(oxygine::spEventDispatcher dispatcher, qint32 id);
        void removeEventListeners(oxygine::spEventDispatcher dispatcher, oxygine::IClosureOwner* callbackThis);
        void detachAndRemove(oxygine::spActor actor);
        void detach(oxygine::spActor actor);
        void setTimerCycle(qint32 newTimerCycle);
        void setRendering(bool render);
    private:
        inline bool acquireLock()
        {
            if (m_pausedCounter == 0)
            {
                if (m_renderSync.try_lock())
                {
                    if (m_pausedCounter == 0)
                    {
                        return true;
                    }
                    else
                    {
                        m_renderSync.unlock();
                    }
                }
            }
            return false;
        }

    private:
        WindowBase & m_window;
        std::atomic<quint8> & m_pausedCounter;
        std::mutex & m_renderSync;
        qint32 m_repeatedFramesDropped{0};
        qint64 m_lastFrameTime{0};
        QTimer m_timer;
        QTimer m_mouseDelayTimer;
        QPoint m_lastMousePosition;
    };
}

