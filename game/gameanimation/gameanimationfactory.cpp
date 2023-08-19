#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "game/gameanimation/gameanimationfactory.h"
#include "game/gamemap.h"
#include "game/gameanimation/gameanimationwalk.h"
#include "game/gameanimation/gameanimationcapture.h"
#include "game/gameanimation/gameanimationdialog.h"
#include "game/gameanimation/gameanimationpower.h"
#include "game/gameanimation/gameanimationnextday.h"
#include "game/gameanimation/battleanimation.h"
#include "game/unit.h"
#include "game/gameaction.h"

#include "resource_management/gamemanager.h"

#include "menue/basegamemenu.h"

#include "coreengine/interpreter.h"
#include "coreengine/globalutils.h"

spGameAnimationFactory GameAnimationFactory::m_pInstance{nullptr};
QVector<spGameAnimation> GameAnimationFactory::m_Animations;

GameAnimationFactory::GameAnimationFactory()
{
#ifdef GRAPHICSUPPORT
    setObjectName("GameAnimationFactory");
#endif
    Interpreter::setCppOwnerShip(this);
}

void GameAnimationFactory::queueAnimation(GameAnimation* pGameAnimation)
{
    for (qint32 i = 0; i < m_Animations.size(); i++)
    {
        if (m_Animations[i].get() == pGameAnimation)
        {
            m_Animations[i]->detach();
            m_Animations[i]->stop();
            break;
        }
    }
}

void GameAnimationFactory::startQueuedAnimation(GameAnimation* pGameAnimation)
{
    for (qint32 i = 0; i < m_Animations.size(); i++)
    {
        if (m_Animations[i].get() == pGameAnimation)
        {
            m_Animations[i]->restart();
            break;
        }
    }
}

GameAnimationFactory* GameAnimationFactory::getInstance()
{
    if (m_pInstance.get() == nullptr)
    {
        m_pInstance = MemoryManagement::create<GameAnimationFactory>();
    }
    return m_pInstance.get();
}

void GameAnimationFactory::release()
{
    m_Animations.clear();
    m_pInstance.reset();
}

spGameAnimation GameAnimationFactory::createSpAnimation(GameMap* pMap, qint32 x, qint32 y, quint32 frameTime, bool mapPosition)
{
    CONSOLE_PRINT("Creating animation", GameConsole::eDEBUG);
    spGameAnimation animation = MemoryManagement::create<GameAnimation>(frameTime, pMap);
    if (mapPosition)
    {
        animation->setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
    }
    else
    {
        animation->setPosition(x, y);
    }
    animation->setPriority(static_cast<qint32>(Mainapp::ZOrder::Animation));
    if (pMap != nullptr)
    {
        pMap->addChild(animation);
    }
    m_Animations.append(animation);
    return animation;
}

GameAnimation* GameAnimationFactory::createAnimation(GameMap* pMap, qint32 x, qint32 y, quint32 frameTime, bool mapPosition)
{
    return createSpAnimation(pMap, x, y, frameTime, mapPosition).get();
}

GameAnimationWalk* GameAnimationFactory::createWalkingAnimation(GameMap* pMap, Unit* pUnit, GameAction* pAction)
{
    if (pUnit != nullptr)
    {
        return createWalkingAnimationV2(pMap, pUnit, pAction->getMovePath());
    }
    else
    {
        return nullptr;
    }
}

GameAnimationWalk* GameAnimationFactory::createWalkingAnimationV2(GameMap* pMap, Unit* pUnit, const QVector<QPoint> movePath)
{
    if (pUnit != nullptr)
    {
        CONSOLE_PRINT("Creating walking animation", GameConsole::eDEBUG);
        spGameAnimationWalk pGameAnimationWalk = MemoryManagement::create<GameAnimationWalk>(pUnit->getSharedPtr<Unit>(), movePath, pMap);
        pGameAnimationWalk->setPriority(static_cast<qint32>(Mainapp::ZOrder::Animation));
        if (pMap != nullptr)
        {
            pMap->addChild(pGameAnimationWalk);
        }
        m_Animations.append(pGameAnimationWalk);
        return pGameAnimationWalk.get();
    }
    else
    {
        return nullptr;
    }
}

GameAnimationPower* GameAnimationFactory::createAnimationPower(GameMap* pMap, QColor color, GameEnums::PowerMode powerMode, CO* pCO, quint32 frameTime)
{    
    CONSOLE_PRINT("Creating power animation", GameConsole::eDEBUG);
    spGameAnimationPower pGameAnimationPower = GameAnimationPower::createGameAnimationPower(frameTime, color, powerMode, pCO, pMap);
    pGameAnimationPower->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    if (pMap != nullptr)
    {
        auto* pMenu = pMap->getMenu();
        if (pMenu != nullptr)
        {
            pMenu->addChild(pGameAnimationPower);
        }
    }
    m_Animations.append(pGameAnimationPower);
    return pGameAnimationPower.get();
}

GameAnimationDialog* GameAnimationFactory::createGameAnimationDialog(GameMap* pMap, const QString text, const QString coid, GameEnums::COMood mood, QColor color, quint32 frameTime)
{
    CONSOLE_PRINT("Creating dialog animation", GameConsole::eDEBUG);
    spGameAnimationDialog pGameAnimationDialog = MemoryManagement::create<GameAnimationDialog>(frameTime, pMap);
    pGameAnimationDialog->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));
    pGameAnimationDialog->setDialog(text);
    pGameAnimationDialog->setCO(coid, mood);
    pGameAnimationDialog->setColor(color);
    oxygine::Stage::getStage()->addChild(pGameAnimationDialog);
    m_Animations.append(pGameAnimationDialog);
    return pGameAnimationDialog.get();
}

GameAnimationNextDay* GameAnimationFactory::createGameAnimationNextDay(GameMap* pMap, Player* pPlayer, quint32 frameTime, quint32 uptimeMs)
{
    CONSOLE_PRINT("Creating next day animation", GameConsole::eDEBUG);
    if (pMap != nullptr)
    {
        auto* pMenu = pMap->getMenu();
        if (pMenu != nullptr)
        {
            spGameAnimationNextDay pAnim = MemoryManagement::create<GameAnimationNextDay>(pMap, pPlayer, frameTime, false, uptimeMs);
            pMenu->addChild(pAnim);
            m_Animations.append(pAnim);
            return pAnim.get();
        }
    }
    return nullptr;
}

GameAnimationCapture* GameAnimationFactory::createGameAnimationCapture(GameMap* pMap, qint32 x, qint32 y, qint32 startPoints, qint32 endPoints, qint32 maxPoints)
{
    CONSOLE_PRINT("Creating capture animation", GameConsole::eDEBUG);
    spGameAnimationCapture pGameAnimationCapture = MemoryManagement::create<GameAnimationCapture>(startPoints, endPoints, maxPoints, pMap);
    pGameAnimationCapture->setPriority(static_cast<qint32>(Mainapp::ZOrder::Animation));
    pGameAnimationCapture->setPosition(x, y);
    pMap->addChild(pGameAnimationCapture);
    m_Animations.append(pGameAnimationCapture);
    
    return pGameAnimationCapture.get();
}

GameAnimation* GameAnimationFactory::createBattleAnimation(GameMap* pMap, Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp, qint32 atkWeapon,
                                                           Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp, qint32 defWeapon, float defenderDamage)
{    
    CONSOLE_PRINT("Creating battle animation", GameConsole::eDEBUG);
    spGameAnimation pRet;
    if (pDefUnit != nullptr && pMap != nullptr)
    {
        // log this attack to our battle log
        qint32 atkDamage = GlobalUtils::roundUp(defStartHp) - GlobalUtils::roundUp(defEndHp);
        qint32 defDamage = GlobalUtils::roundUp(atkStartHp) - GlobalUtils::roundUp(atkEndHp);

        pMap->getGameRecorder()->logAttack(pMap->getCurrentDay(),
                                           atkDamage, pAtkTerrain->Terrain::getX(), pAtkTerrain->Terrain::getY(), pAtkUnit->getUnitID(), pAtkUnit->getOwner()->getPlayerID(), (atkEndHp <= 0),
                                           defDamage, pDefTerrain->Terrain::getX(), pDefTerrain->Terrain::getY(), pDefUnit->getUnitID(), pDefUnit->getOwner()->getPlayerID(), (defEndHp <= 0),
                                           pDefUnit->getOwner()->getFieldVisible(pAtkTerrain->Terrain::getX(), pAtkTerrain->Terrain::getY()));
        auto battleViewMode = Settings::getInstance()->getBattleAnimationType();
        if (battleViewMode == GameEnums::BattleAnimationType_Overworld)
        {
            GameAnimation* pBattle = createOverworldBattleAnimation(pMap, pAtkTerrain, pAtkUnit, atkStartHp, atkEndHp, atkWeapon,
                                                  pDefTerrain, pDefUnit, defStartHp, defEndHp, defWeapon, defenderDamage);
            if (pBattle != nullptr)
            {
                pRet = pBattle->getSharedPtr<GameAnimation>();
            }
        }
        else
        {
            pRet = MemoryManagement::create<BattleAnimation>(pAtkTerrain, pAtkUnit, atkStartHp, atkEndHp, atkWeapon,
                                             pDefTerrain, pDefUnit, defStartHp, defEndHp, defWeapon, defenderDamage,
                                             pMap);
            oxygine::spSprite pBack;

            if (battleViewMode == GameEnums::BattleAnimationType_Fullscreen ||
                battleViewMode == GameEnums::BattleAnimationType_FullscreenTransparent ||
                battleViewMode == GameEnums::BattleAnimationType_FullscreenTransparent ||
                battleViewMode == GameEnums::BattleAnimationType_DetailTransparent)
            {
                oxygine::ResAnim* pAnim = GameManager::getInstance()->getResAnim("fullscreen_battlebackground", oxygine::ep_ignore_error);
                if (pAnim != nullptr)
                {
                    pBack = MemoryManagement::create<oxygine::Sprite>();
                    pBack->setResAnim(pAnim);
                    pBack->setScaleX(static_cast<float>(oxygine::Stage::getStage()->getWidth()) / static_cast<float>(pAnim->getWidth()));
                    pBack->setScaleY(static_cast<float>(oxygine::Stage::getStage()->getHeight()) / static_cast<float>(pAnim->getHeight()));
                    pBack->setSize(pAnim->getSize());
                }
                else
                {
                    oxygine::spColorRectSprite pRect = MemoryManagement::create<oxygine::ColorRectSprite>();
                    pRect->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
                    pRect->setColor(pAtkUnit->getOwner()->getColor().darker(120));
                    pBack = pRect;
                }
            }
            if (battleViewMode == GameEnums::BattleAnimationType_DetailTransparent ||
                battleViewMode == GameEnums::BattleAnimationType_FullscreenTransparent)
            {
                pBack->setAlpha(128);
            }
            qint32 scaleFactor = 2.0f;
            if (scaleFactor * (pRet->getScaledHeight() - 30) > oxygine::Stage::getStage()->getHeight())
            {
                scaleFactor = 1.0f;
                while (scaleFactor * (pRet->getScaledHeight() - 30) > oxygine::Stage::getStage()->getHeight())
                {
                    scaleFactor /= 2;
                }
            }
            if (battleViewMode == GameEnums::BattleAnimationType_Fullscreen ||
                battleViewMode == GameEnums::BattleAnimationType_FullscreenTransparent)
            {
                float scale = oxygine::Stage::getStage()->getHeight() / (pRet->getScaledHeight() - 30);
                float widthScale = oxygine::Stage::getStage()->getWidth() / (pRet->getScaledWidth() - 30);
                if (scale > widthScale)
                {
                    scale = widthScale;
                }
                qint32 newScale = scaleFactor;
                qint32 lastScale = scaleFactor;
                while (newScale < scale)
                {
                    lastScale = newScale;
                    newScale *= 2;
                }
                scaleFactor = lastScale;
            }
            pRet->setScale(scaleFactor);
            pRet->setPosition(static_cast<qint32>(oxygine::Stage::getStage()->getWidth() / 2 - pRet->getScaledWidth() / 2),
                              static_cast<qint32>(oxygine::Stage::getStage()->getHeight() / 2 - pRet->getScaledHeight() / 2));
            if (pMap != nullptr)
            {
                auto* pMenu = pMap->getMenu();
                if (pMenu != nullptr)
                {
                    if (pBack.get() != nullptr)
                    {
                        pBack->setPriority(BattleAnimation::priorityBackground);
                        pBack->setPosition(-pRet->getPosition() / scaleFactor);
                        pRet->addChild(pBack);
                        pBack->setScaleX(pBack->getScaleX() / scaleFactor);
                        pBack->setScaleY(pBack->getScaleY() / scaleFactor);
                    }
                    pMenu->addChild(pRet);
                }
            }
            m_Animations.append(pRet);
        }
    }
    else
    {
        // attacking building or terrain
        pRet = createSpAnimation(pMap, pDefTerrain->Terrain::getX(), pDefTerrain->Terrain::getY(), 70);
        pRet->addSprite("blackhole_shot", -GameMap::getImageSize() * 0.5f, -GameMap::getImageSize() * 0.5f, 0, 2.0f);
        pRet->setSound("talongunhit.wav", 1);
    }

    return pRet.get();
}

GameAnimation* GameAnimationFactory::createOverworldBattleAnimation(GameMap* pMap, Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp, qint32 atkWeapon,
                                                                    Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp, qint32 defWeapon, float defenderDamage)
{
    CONSOLE_PRINT("Creating overworld battle animation", GameConsole::eDEBUG);
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(pAtkTerrain),
                       pInterpreter->newQObject(pAtkUnit),
                       atkStartHp,
                       atkEndHp,
                       atkWeapon,
                       pInterpreter->newQObject(pDefTerrain),
                       pInterpreter->newQObject(pDefUnit),
                       defStartHp,
                       defEndHp,
                       defWeapon,
                       defenderDamage,
                       pInterpreter->newQObject(pMap)});
    QJSValue ret = pInterpreter->doFunction("ACTION_FIRE", "createOverworldBattleAnimation", args);
    return dynamic_cast<GameAnimation*>(ret.toQObject());
}

qint32 GameAnimationFactory::getAnimationCount()
{
    return m_Animations.size();
}

GameAnimation* GameAnimationFactory::getAnimation(qint32 index)
{
    if ((index >= 0) && (index < m_Animations.size()))
    {
        return m_Animations[index].get();
    }
    return nullptr;
}

void GameAnimationFactory::removeAnimationFromQueue(spGameAnimation pAnimation)
{
    qint32 i = 0;
    while (i < m_Animations.size())
    {
        if (m_Animations[i].get() == pAnimation.get())
        {
            m_Animations.removeAt(i);
        }
        else
        {
            ++i;
        }
    }
}

void GameAnimationFactory::removeAnimation(GameAnimation* pAnimation, bool skipping)
{
    if (pAnimation != nullptr)
    {
        removeAnimation(pAnimation->getSharedPtr<GameAnimation>(), skipping, true);
    }
}

void GameAnimationFactory::removeAnimation(spGameAnimation pAnimation, bool skipping, bool removeFromQueue)
{
    if (removeFromQueue)
    {
        removeAnimationFromQueue(pAnimation);
    }
    if (pAnimation.get() != nullptr)
    {
        pAnimation->detach();
    }
    if (m_Animations.size() == 0 && !skipping)
    {
        CONSOLE_PRINT("GameAnimationFactory -> emitting animationsFinished()", GameConsole::eDEBUG);
        emit GameAnimationFactory::getInstance()->animationsFinished();
    }
}

void GameAnimationFactory::clearAllAnimations()
{
    CONSOLE_PRINT("GameAnimationFactory -> clearAllAnimations()", GameConsole::eDEBUG);
    for (qint32 i = 0; i < m_Animations.size(); i++)
    {
        m_Animations[i]->detach();
    }
    m_Animations.clear();

}

void GameAnimationFactory::finishAllAnimations()
{
    CONSOLE_PRINT("GameAnimationFactory::finishAllAnimations()", GameConsole::eDEBUG);
    qint32 i = 0;
    while (i < m_Animations.size())
    {
        spGameAnimation spAnimation = m_Animations[i];
        while (!spAnimation->onFinished(true))
        {
        }
    }
}

void GameAnimationFactory::finishAllAnimationsWithEmitFinished()
{
    GameAnimationFactory::finishAllAnimations();
    if (m_Animations.size() == 0)
    {
        CONSOLE_PRINT("GameAnimationFactory -> emitting animationsFinished()", GameConsole::eDEBUG);
        emit GameAnimationFactory::getInstance()->animationsFinished();
    }
}

void GameAnimationFactory::skipAllAnimations()
{
    CONSOLE_PRINT("skipAllAnimations()", GameConsole::eDEBUG);
    qint32 i = 0;
    while (i < GameAnimationFactory::getAnimationCount())
    {
        GameAnimation* pAnimation = GameAnimationFactory::getAnimation(i);
        if (pAnimation != nullptr)
        {
            GameAnimationDialog* pDialogAnimation = dynamic_cast<GameAnimationDialog*>(pAnimation);
            BattleAnimation* pBattleAnimation = dynamic_cast<BattleAnimation*>(pAnimation);
            GameAnimationCapture* pGameAnimationCapture = dynamic_cast<GameAnimationCapture*>(pAnimation);
            GameAnimationNextDay* pGameAnimationNextDay = dynamic_cast<GameAnimationNextDay*>(pAnimation);
            GameAnimationWalk* pGameAnimationWalk = dynamic_cast<GameAnimationWalk*>(pAnimation);
            if (shouldSkipDialog(pDialogAnimation) ||
                shouldSkipBattleAnimation(pBattleAnimation) ||
                shouldSkipCapture(pGameAnimationCapture) ||
                shouldSkipDay2Day(pGameAnimationNextDay) ||
                shouldSkipMovement(pGameAnimationWalk) ||
                (pDialogAnimation == nullptr &&
                 pBattleAnimation == nullptr &&
                 pGameAnimationCapture == nullptr &&
                 pGameAnimationWalk == nullptr &&
                 pGameAnimationNextDay == nullptr &&
                 shouldSkipOtherAnimation(pAnimation)))
            {
                while (!pAnimation->onFinished(true));
            }
            else
            {
                i++;
            }
        }
        else
        {
            i++;
        }
    }
    CONSOLE_PRINT("skipAllAnimations remaining Animations=" + QString::number(GameAnimationFactory::getAnimationCount()), GameConsole::eDEBUG);
    GameAnimationFactory::printActiveAnimations();
}

void GameAnimationFactory::printActiveAnimations()
{
    if (GameConsole::getLogLevel() <= GameConsole::eDEBUG )
    {
        for (auto & animation : m_Animations)
        {
            CONSOLE_PRINT("Currently running animation: " + animation->objectName(), GameConsole::eDEBUG);
        }
    }
}

bool GameAnimationFactory::shouldSkipDialog(GameAnimationDialog* pDialogAnimation)
{
    bool dialogEnabled = Settings::getInstance()->getDialogAnimation();
    return pDialogAnimation != nullptr && !dialogEnabled;
}

bool GameAnimationFactory::shouldSkipCapture(GameAnimationCapture* pGameAnimationCapture)
{
    bool captureEnabled = Settings::getInstance()->getCaptureAnimation();
    return pGameAnimationCapture != nullptr && !captureEnabled;
}

bool GameAnimationFactory::shouldSkipDay2Day(GameAnimationNextDay* pGameAnimationNextDay)
{
    bool day2dayEnabled = Settings::getInstance()->getDay2dayScreen();
    return pGameAnimationNextDay != nullptr && !day2dayEnabled;
}


bool GameAnimationFactory::shouldSkipMovement(GameAnimationWalk* pGameAnimationWalk)
{
    bool movementEnabled = Settings::getInstance()->getMovementAnimations();
    return pGameAnimationWalk != nullptr && !movementEnabled;
}

bool GameAnimationFactory::shouldSkipBattleAnimation(BattleAnimation* pBattleAnimation)
{
    bool battleActive = true;
    if (pBattleAnimation != nullptr)
    {
        GameMap* pMap = pBattleAnimation->getMap();
        GameEnums::BattleAnimationMode animMode = Settings::getInstance()->getBattleAnimationMode();
        Unit* pAtkUnit = pBattleAnimation->getAtkUnit();
        Unit* pDefUnit = pBattleAnimation->getDefUnit();
        if (animMode == GameEnums::BattleAnimationMode_Own)
        {
            // only show animation if at least one player is a human
            if ((pAtkUnit->getOwner()->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human) ||
                (pDefUnit != nullptr && pDefUnit->getOwner()->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human))
            {
                battleActive = true;
            }
            else
            {
                battleActive = false;
            }
        }
        else if (animMode == GameEnums::BattleAnimationMode_Ally)
        {
            Player* pPlayer2 = pMap->getCurrentViewPlayer();
            // only show animation if at least one player is an ally
            if (pPlayer2->isAlly(pAtkUnit->getOwner()) ||
                (pDefUnit != nullptr && pPlayer2->isAlly(pDefUnit->getOwner())))
            {
                battleActive = true;
            }
            else
            {
                battleActive = false;
            }
        }
        else if (animMode == GameEnums::BattleAnimationMode_Enemy)
        {
            Player* pPlayer2 = pMap->getCurrentViewPlayer();
            // only show animation if none of the players is human and all units are enemies of the current view player
            if ((pAtkUnit->getOwner()->getBaseGameInput()->getAiType() != GameEnums::AiTypes_Human) &&
                pDefUnit != nullptr &&
                pDefUnit->getOwner()->getBaseGameInput()->getAiType() != GameEnums::AiTypes_Human &&
                pPlayer2->isEnemy(pAtkUnit->getOwner()) &&
                pPlayer2->isEnemy(pDefUnit->getOwner()))
            {
                battleActive = true;
            }
            else
            {
                battleActive = false;
            }
        }
        else if (animMode == GameEnums::BattleAnimationMode_None)
        {
            battleActive = false;
        }
        else if (animMode == GameEnums::BattleAnimationMode_All)
        {
            battleActive = true;
        }
    }
    return !battleActive;
}

bool GameAnimationFactory::shouldSkipOtherAnimation(GameAnimation* pBattleAnimation)
{
    return !Settings::getInstance()->getOverworldAnimations();
}
