#include "3rd_party/oxygine-framework/oxygine/core/opengl/renderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/opengl/windowBase.h"

#include "game/GameEnums.h"

using namespace oxygine;

Renderer::Renderer(WindowBase & window)
    : m_window(window)
{
    connect(this, &Renderer::sigPaintGl, this, &Renderer::onPaintGl, Qt::QueuedConnection);
    connect(this, &Renderer::sigLoadResources, this, &Renderer::loadResources, Qt::QueuedConnection);
    connect(this, &Renderer::sigStart, this, &Renderer::start, Qt::BlockingQueuedConnection);
    connect(this, &Renderer::sigResize, this, &Renderer::resize, Qt::QueuedConnection);
}

Renderer::~Renderer()
{
}

void Renderer::start()
{
    m_window.context()->makeCurrent(m_window.context()->surface());
    // init oxygine engine
    CONSOLE_PRINT("initialize oxygine", GameConsole::eDEBUG);
    VideoDriver::instance = MemoryManagement::create<VideoDriver>();
    VideoDriver::instance->setDefaultSettings();
    rsCache().setDriver(VideoDriver::instance.get());

    STDRenderer::initialize();

    STDRenderer::instance = MemoryManagement::create<STDRenderer>();
    RenderDelegate::instance = MemoryManagement::create<RenderDelegate>();
    Material::null = MemoryManagement::create<Material>();
    Material::current = Material::null;

    STDRenderer::current = STDRenderer::instance;

    FontManager::getInstance();
    // load ressources by creating the singletons
    BackgroundManager::getInstance();
}

void Renderer::resize(qint32 w, qint32 h)
{
    CONSOLE_PRINT("core::restore()", GameConsole::eDEBUG);
    VideoDriver::instance->restore();
    STDRenderer::restore();
    CONSOLE_PRINT("core::restore() done", GameConsole::eDEBUG);
}

bool Renderer::beginRendering()
{
    if (!m_window.m_renderEnabled)
    {
        return false;
    }
    bool ready = STDRenderer::isReady();
    if (ready)
    {
        rsCache().reset();
        VideoDriver::instance->reset();
    }
    else
    {
        CONSOLE_PRINT("!ready", GameConsole::eDEBUG);
    }
    return ready;
}

void Renderer::loadResources(qint32 step)
{
    CONSOLE_PRINT("Loading resources for step: " + QString::number(step), GameConsole::eDEBUG);
    switch (static_cast<GameEnums::StartupPhase>(step))
    {
    case GameEnums::StartupPhase::StartupPhase_General:
    {
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

void Renderer::onPaintGl()
{
    if (m_window.m_pausedCounter == 0)
    {
        if (m_window.m_renderSync.tryLock())
        {
            if (!m_window.m_terminating && m_window.m_pausedCounter == 0)
            {
                m_window.updateData();
                if (oxygine::Stage::getStage().get() != nullptr)
                {
                    oxygine::Stage::getStage()->updateStage();
                    if (beginRendering())
                    {
                        auto* context = m_window.context();
                        QColor clearColor(0, 0, 0, 255);
                        QSize windowSize = m_window.size();
                        QRect viewport(0, 0, windowSize.width(), windowSize.height());
                        // Render all actors inside the stage. Actor::render will also be called for all its children
                        oxygine::Stage::getStage()->renderStage(clearColor, viewport);
                        context->swapBuffers(context->surface());
                        m_repeatedFramesDropped = 0;
                    }
                }
            }
            m_window.m_renderSync.unlock();
        }
    }
}