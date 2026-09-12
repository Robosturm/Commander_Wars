#include "3rd_party/oxygine-framework/oxygine/core/renderer.h"
#include "game/GameEnums.h"
#include "game/gamerecording/gamemapimagesaver.h"
#include "menue/basegamemenu.h"
#include "objects/minimap.h"

using namespace oxygine;

Renderer::Renderer(WindowBase & window)
    : m_window(window),
      m_timer(this)
{
}

Renderer::~Renderer()
{
}

void Renderer::connectSignals()
{
    connect(this, &Renderer::sigPaint, this, &Renderer::onPaint, Qt::QueuedConnection);
    connect(&m_timer, &QTimer::timeout, this, &Renderer::onPaint);
    connect(this, &Renderer::sigSetTimerCycle, this, &Renderer::setTimerCycle);
    connect(this, &Renderer::sigSetRendering, this, &Renderer::setRendering);

    auto conntectionType = Qt::BlockingQueuedConnection;
    if (m_window.m_noUi)
    {
        conntectionType = Qt::AutoConnection;
    }
    connect(this, &Renderer::sigLoadResources, this, &Renderer::loadResources, conntectionType);
    connect(this, &Renderer::sigStart, this, &Renderer::start, conntectionType);
    connect(this, &Renderer::sigResize, this, &Renderer::resize, conntectionType);
    connect(this, &Renderer::sigDoMapshot, this, &Renderer::doMapshot, conntectionType);
    connect(this, &Renderer::sigSaveMapAsImage, this, &Renderer::saveMapAsImage, conntectionType);
    connect(this, &Renderer::sigSetAddColor, this, &Renderer::setAddColor, conntectionType);
    connect(this, &Renderer::sigRebuildText, this, &Renderer::rebuildText, conntectionType);
    connect(this, &Renderer::sigChangeAnimFrame, this, &Renderer::changeAnimFrame, conntectionType);
    connect(this, &Renderer::sigSetColorTable, this, &Renderer::setColorTable, conntectionType);
    connect(this, &Renderer::sigRestartAllTweens, this, &Renderer::restartAllTweens, conntectionType);
    connect(this, &Renderer::sigSyncAllTweens, this, &Renderer::syncAllTweens, conntectionType);
    connect(this, &Renderer::sigAddChild, this, &Renderer::addChild, conntectionType);
    connect(this, &Renderer::sigRemoveChild, this, &Renderer::removeChild, conntectionType);
    connect(this, &Renderer::sigSetPriority, this, &Renderer::setPriority, conntectionType);
    connect(this, &Renderer::sigAddTween, this, &Renderer::addTween, conntectionType);
    connect(this, &Renderer::sigRemoveTween, this, &Renderer::removeTween, conntectionType);
    connect(this, &Renderer::sigRemoveTweens, this, &Renderer::removeTweens, conntectionType);
    connect(this, &Renderer::sigRemoveChildren, this, &Renderer::removeChildren, conntectionType);
    connect(this, &Renderer::sigAddEventListener, this, &Renderer::addEventListener, conntectionType);
    connect(this, &Renderer::sigRemoveEventListener, this, &Renderer::removeEventListener, conntectionType);
    connect(this, &Renderer::sigRemoveEventListeners, this, &Renderer::removeEventListeners, conntectionType);
    connect(this, &Renderer::sigDetachAndRemove, this, &Renderer::detachAndRemove, conntectionType);
    connect(this, &Renderer::sigDetach, this, &Renderer::detach, conntectionType);
    connect(this, &Renderer::sigQuit, this, &Renderer::quit, conntectionType);
}

void Renderer::loadResources(qint32 step)
{
    CONSOLE_PRINT("Loading resources for step: " + QString::number(step), GameConsole::eDEBUG);
    switch (static_cast<GameEnums::StartupPhase>(step))
    {
    case GameEnums::StartupPhase::StartupPhase_General:
    {        
        FontManager::getInstance();
        // load ressources by creating the singletons
        BackgroundManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_ObjectManager:
    {
        ObjectManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_Building:
    {
        BuildingSpriteManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_COSprites:
    {
        COSpriteManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_GameAnimations:
    {
        GameAnimationManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_GameManager:
    {
        GameManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_GameRuleManager:
    {
        GameRuleManager::getInstance();
        WeaponManager::getInstance();
        MovementTableManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_TerrainManager:
    {
        TerrainManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_UnitSpriteManager:
    {
        UnitSpriteManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_BattleAnimationManager:
    {
        BattleAnimationManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_COPerkManager:
    {
        COPerkManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_WikiDatabase:
    {
        WikiDatabase::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_Userdata:
    {
        Userdata::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_Achievementmanager:
    {
        AchievementManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_MovementPlannerAddInManager:
    {
        MovementPlannerAddInManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_UiManager:
    {
        UiManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_ShopLoader:
    {
        ShopLoader::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_Sound:
    {
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_LoadingScripts:
    {
        break;
    }
}
}

void Renderer::loadSingleResAnim(oxygine::spResAnim pAnim, QImage image, qint32 columns, qint32 rows, float scaleFactor, bool clamp2Edge, quint32 linearFilter)
{
    if (pAnim.get() != nullptr && !m_window.m_noUi)
    {
        pAnim->init(image, columns, rows, scaleFactor, clamp2Edge, linearFilter);
    }
}


void Renderer::doMapshot(BaseGamemenu* pMenu)
{
    if (!m_window.m_noUi)
    {
        if (beginRendering())
        {
            qint32 i = 0;
            QDir dir("screenshots/");
            dir.mkpath(".");
            while (i < std::numeric_limits<qint32>::max())
            {
                QString filename = "screenshots/mapshot+" + QString::number(i) + ".png";
                if (!QFile::exists(filename))
                {
                    GamemapImageSaver::saveMapAsImage(filename, *pMenu);
                    break;
                }
                ++i;
            }
        }
    }
}

void Renderer::saveMapAsImage(Minimap* pMinimap, QImage * img)
{
    if (!m_window.m_noUi)
    {
        GamemapImageSaver::saveMapAsImage(pMinimap, *img);
    }
}


void Renderer::addEventListener(oxygine::spEventDispatcher dispatcher, oxygine::eventType et, const oxygine::EventCallback cb)
{
    dispatcher->addEventListenerWithId(et, cb);
}

void Renderer::removeEventListener(oxygine::spEventDispatcher dispatcher, qint32 id)
{
    dispatcher->__removeEventListener(id);
}

void Renderer::removeEventListeners(oxygine::spEventDispatcher dispatcher, oxygine::IClosureOwner* callbackThis)
{
    dispatcher->__removeEventListeners(callbackThis);
}

void Renderer::setAddColor(oxygine::spVStyleActor actor, QColor color)
{
    actor->changeAddColor(color);
}

void Renderer::rebuildText(oxygine::spTextField actor)
{
    actor->__rebuildText();
}

void Renderer::changeAnimFrame(oxygine::spSprite actor, const oxygine::AnimationFrame& frame)
{
    actor->__changeAnimFrame(frame);
}

void Renderer::setColorTable(oxygine::spSprite actor, const oxygine::spResAnim pAnim, bool matrix)
{
    actor->__setColorTable(pAnim, matrix);
}

void Renderer::restartAllTweens(oxygine::spActor actor)
{
    actor->__restartAllTweens();
}

void Renderer::syncAllTweens(oxygine::spActor actor, oxygine::timeMS syncTime)
{
    actor->__syncAllTweens(syncTime);
}

void Renderer::addChild(oxygine::spActor parent, oxygine::spActor actor)
{
    parent->__addChild(actor);
}

void Renderer::removeChild(oxygine::spActor parent, oxygine::spActor actor)
{
    parent->__removeChild(actor);
}

void Renderer::setPriority(oxygine::spActor parent, qint32 zorder)
{
    parent->__setPriority(zorder);
}

void Renderer::addTween(oxygine::spActor parent, oxygine::spTween tween)
{
    parent->__addTween(tween);
}

void Renderer::removeTween(oxygine::spActor parent, oxygine::spTween tween)
{
    parent->__removeTween(tween);
}

void Renderer::removeChildren(oxygine::spActor actor)
{
    actor->__removeChildren();
}

void Renderer::removeTweens(oxygine::spActor actor)
{
    actor->__removeTweens();
}

void Renderer::detachAndRemove(oxygine::spActor actor)
{
    actor->__detachAndRemove();
}

void Renderer::detach(oxygine::spActor actor)
{
    actor->__detach();
}

void Renderer::setRendering(bool render)
{
    if (render)
    {
        m_timer.start();
    }
    else
    {
        m_timer.stop();
    }
}

void Renderer::setTimerCycle(qint32 newTimerCycle)
{
    m_timer.setInterval(newTimerCycle);
}