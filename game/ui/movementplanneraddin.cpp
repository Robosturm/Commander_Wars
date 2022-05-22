#include "game/ui/movementplanneraddin.h"

#include "coreengine/interpreter.h"
#include "game/gamemap.h"

#include "menue/movementplanner.h"

#include "resource_management/gameanimationmanager.h"

constexpr const char* const AddIn = "addIn";

MovementPlannerAddIn::MovementPlannerAddIn(QString addIn, GameMap* pMap, MovementPlanner* pPlanner)
    : m_addIn(addIn),
      m_pMap(pMap),
      m_pPlanner(pPlanner)
{
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    setObjectName("MovementPlannerAddIn");
    Interpreter::setCppOwnerShip(this);
}

QString MovementPlannerAddIn::getAddIn() const
{
    return m_addIn;
}

void MovementPlannerAddIn::addSprite(QString spriteID, qint32 x, qint32 y, float offsetX, float offsetY, qint32 frameTime)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID, oxygine::error_policy::ep_ignore_error);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(static_cast<qint64>(pAnim->getTotalFrames() * frameTime)), -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
        pSprite->setPosition(offsetX + x * GameMap::getImageSize(), offsetY + y  * GameMap::getImageSize());
        m_pMap->addChild(pSprite);
        m_sprites.append(pSprite);
    }
}

void MovementPlannerAddIn::removeAllSprites()
{
    for (auto & sprite : m_sprites)
    {
        sprite->detach();
    }
    m_sprites.clear();;
}

GameMap* MovementPlannerAddIn::getMap() const
{
    return m_pMap;
}

MovementPlanner* MovementPlannerAddIn::getPlanner() const
{
    return m_pPlanner;
}

void MovementPlannerAddIn::onMenuInputDone()
{
    m_pPlanner->onMenuInputDone();
}

void MovementPlannerAddIn::hide()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->deleteObject(AddIn);
    resetUi();
    detach();
}

void MovementPlannerAddIn::show()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->setGlobal(AddIn, pInterpreter->newQObject(this));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::FocusedObjects));
    m_pPlanner->addChild(oxygine::spActor(this));
}
