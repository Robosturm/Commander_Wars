#pragma once

#include <QObject>
#include <QImage>
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

    public slots:
        void quit();
        void onPaint();
        void loadResources(qint32 step);
        void start();
        void resize(qint32 w, qint32 h);
        void loadSingleResAnim(oxygine::spResAnim pAnim, QImage image, qint32 columns, qint32 rows, float scaleFactor, bool clamp2Edge = true, quint32 linearFilter = 0);
        void doMapshot(BaseGamemenu* pMenu);
        /**
         * @brief saveMapAsImage
         * @param pMinimap
         * @param img
         */
        void saveMapAsImage(Minimap* pMinimap, QImage * img);
    private slots:
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

    private:
        WindowBase & m_window;
        qint32 m_repeatedFramesDropped{0};
    };
}

