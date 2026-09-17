#include "3rd_party/oxygine-framework/oxygine/core/renderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "game/GameEnums.h"
#include "game/gamerecording/gamemapimagesaver.h"
#include "menue/basegamemenu.h"
#include "objects/minimap.h"

using namespace oxygine;

Renderer::Renderer(WindowBase & window)
    : m_window(window),
      m_pausedCounter(window.m_pausedCounter),
      m_renderSync(window.m_renderSync),
      m_timer(this),
      m_mouseDelayTimer(this)
{
    connect(this, &Renderer::sigPaint, this, &Renderer::onPaint, Qt::QueuedConnection);
    connect(&m_timer, &QTimer::timeout, this, &Renderer::onPaint);
    connect(this, &Renderer::sigSetTimerCycle, this, &Renderer::setTimerCycle);
    connect(this, &Renderer::sigSetRendering, this, &Renderer::setRendering);
    connect(&m_mouseDelayTimer, &QTimer::timeout, this, &Renderer::mouseMoveEventDelayed, Qt::QueuedConnection);
    m_mouseDelayTimer.setSingleShot(true);
}

Renderer::~Renderer()
{
}

void Renderer::connectSignals()
{
    auto conntectionType = Qt::BlockingQueuedConnection;
    if (m_window.m_noUi)
    {
        conntectionType = Qt::AutoConnection;
    }
    connect(this, &Renderer::sigLoadResources, this, &Renderer::asyncLoadResources, Qt::QueuedConnection);
    connect(this, &Renderer::sigSyncLoadResources, this, &Renderer::loadResources, conntectionType);
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

void Renderer::asyncLoadResources(qint32 step)
{
    loadResources(step);
    emit m_window.sigNextStartUpStep(static_cast<GameEnums::StartupPhase>(static_cast<qint8>(step) + 1));
}

void Renderer::loadResources(qint32 step)
{
    CONSOLE_PRINT("Loading resources for step: " + QString::number(step), GameConsole::eDEBUG);
    switch (static_cast<GameEnums::StartupPhase>(step))
    {
    case GameEnums::StartupPhase::StartupPhase_General:
    {        
        FontManager::getInstance();
        BackgroundManager::getInstance();
        ObjectManager::getInstance();
        break;
    }
    case GameEnums::StartupPhase::StartupPhase_ObjectManager:
    {
        BackgroundManager::getInstance()->loadRessources("/images/backgrounds/res.xml");
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
    default:
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

void Renderer::mouseMoveEvent(int x, int y)
{
    bool handled = false;
    if (acquireLock())
    {
        oxygine::Input* input = &oxygine::Input::getInstance();
        m_lastMousePosition = QPoint(x, y);
        auto delayed = input->sendPointerMotionEvent(oxygine::Stage::getStage(), x, y, 1.0f, input->getPointerMouse());
        if (delayed > 0)
        {
            m_mouseDelayTimer.start(delayed);
        }
        else
        {
            m_mouseDelayTimer.stop();
        }
        m_window.m_renderSync.unlock();
        handled = true;
    }
    if (!handled)
    {
        QTimer::singleShot(5, this, [this, x, y]()
        {
            mouseMoveEvent(x, y);
        });
    }
}

void Renderer::mouseMoveEventDelayed()
{
    mouseMoveEvent(m_lastMousePosition.x(), m_lastMousePosition.y());
}

void Renderer::wheelEvent(qint32 x, qint32 y)
{
    bool handled = false;
    if (acquireLock())
    {
        oxygine::Input & input = oxygine::Input::getInstance();
        input.sendPointerWheelEvent(oxygine::Stage::getStage(), QPoint(x, y), input.getPointerMouse());
        m_window.m_renderSync.unlock();
        handled = true;
    }
    if (!handled)
    {
        QTimer::singleShot(5, this, [this, x, y]()
        {
            wheelEvent(x, y);
        });
    }
}

void Renderer::mousePressEvent(oxygine::MouseButton button, qint32 x, qint32 y)
{
    bool handled = false;
    if (acquireLock())
    {
        oxygine::Input & input = oxygine::Input::getInstance();
        input.sendPointerButtonEvent(oxygine::Stage::getStage(), button, x, y, 1.0f,
                                      oxygine::TouchEvent::TOUCH_DOWN, input.getPointerMouse());
        m_window.m_renderSync.unlock();
        handled = true;
    }
    if (!handled)
    {
        QTimer::singleShot(5, this, [this, button, x, y]()
        {
            mousePressEvent(button, x, y);
        });
    }
}

void Renderer::mouseReleaseEvent(oxygine::MouseButton button, qint32 x, qint32 y)
{
    bool handled = false;
    if (acquireLock())
    {
        oxygine::Input & input = oxygine::Input::getInstance();
        input.sendPointerButtonEvent(oxygine::Stage::getStage(), button, x, y, 1.0f,
                                 oxygine::TouchEvent::TOUCH_UP, input.getPointerMouse());
        m_window.m_renderSync.unlock();
        handled = true;
    }
    if (!handled)
    {
        QTimer::singleShot(5, this, [this, button, x, y]()
        {
            mouseReleaseEvent(button, x, y);
        });
    }
}
