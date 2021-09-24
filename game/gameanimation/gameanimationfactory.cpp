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

#include "menue/gamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"

spGameAnimationFactory GameAnimationFactory::m_pInstance;
QVector<spGameAnimation> GameAnimationFactory::m_Animations;

GameAnimationFactory::GameAnimationFactory()
    : QObject()
{
    setObjectName("GameAnimationFactory");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
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
        m_pInstance = spGameAnimationFactory::create();
    }
    return m_pInstance.get();
}

void GameAnimationFactory::release()
{
    m_Animations.clear();
    m_pInstance = nullptr;
}

GameAnimation* GameAnimationFactory::createAnimation(quint32 x, quint32 y, quint32 frameTime, bool mapPosition)
{
    
    spGameAnimation animation = spGameAnimation::create(frameTime);
    if (mapPosition)
    {
        animation->setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
    }
    else
    {
        animation->setPosition(x, y);
    }
    animation->setPriority(static_cast<qint32>(Mainapp::ZOrder::Animation));
    GameMap::getInstance()->addChild(animation);
    m_Animations.append(animation);
    
    return animation.get();
}

GameAnimationWalk* GameAnimationFactory::createWalkingAnimation(Unit* pUnit, GameAction* pAction)
{    
    spGameAnimationWalk pGameAnimationWalk = spGameAnimationWalk::create(pUnit, pAction->getMovePath());
    pGameAnimationWalk->setPriority(static_cast<qint32>(Mainapp::ZOrder::Animation));
    GameMap::getInstance()->addChild(pGameAnimationWalk);
    m_Animations.append(pGameAnimationWalk);
    return pGameAnimationWalk.get();
}

GameAnimationPower* GameAnimationFactory::createAnimationPower(QColor color, GameEnums::PowerMode powerMode, CO* pCO, quint32 frameTime)
{    
    spGameAnimationPower pGameAnimationPower = GameAnimationPower::createGameAnimationPower(frameTime, color, powerMode, pCO);
    pGameAnimationPower->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        pGameMenue->addChild(pGameAnimationPower);
    }
    m_Animations.append(pGameAnimationPower);
    return pGameAnimationPower.get();
}

GameAnimationDialog* GameAnimationFactory::createGameAnimationDialog(QString text, QString coid, GameEnums::COMood mood, QColor color, quint32 frameTime)
{
    
    spGameAnimationDialog pGameAnimationDialog = spGameAnimationDialog::create(frameTime);
    pGameAnimationDialog->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));
    pGameAnimationDialog->setDialog(text);
    pGameAnimationDialog->setCO(coid, mood);
    pGameAnimationDialog->setColor(color);
    oxygine::Stage::getStage()->addChild(pGameAnimationDialog);
    m_Animations.append(pGameAnimationDialog);
    return pGameAnimationDialog.get();
}

GameAnimationNextDay* GameAnimationFactory::createGameAnimationNextDay(Player* pPlayer, quint32 frameTime, quint32 uptimeMs)
{
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        spGameAnimationNextDay pAnim = spGameAnimationNextDay::create(pPlayer, frameTime, false, uptimeMs);
        pGameMenue->addChild(pAnim);
        m_Animations.append(pAnim);
        return pAnim.get();
    }
    return nullptr;
}

GameAnimationCapture* GameAnimationFactory::createGameAnimationCapture(qint32 x, qint32 y, qint32 startPoints, qint32 endPoints, qint32 maxPoints)
{
    
    spGameAnimationCapture pGameAnimationCapture = spGameAnimationCapture::create(startPoints, endPoints, maxPoints);
    pGameAnimationCapture->setPriority(static_cast<qint32>(Mainapp::ZOrder::Animation));
    pGameAnimationCapture->setPosition(x, y);
    GameMap::getInstance()->addChild(pGameAnimationCapture);
    m_Animations.append(pGameAnimationCapture);
    
    return pGameAnimationCapture.get();
}

GameAnimation* GameAnimationFactory::createBattleAnimation(Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp, qint32 atkWeapon,
                                                           Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp, qint32 defWeapon, float defenderDamage)
{    
    spGameAnimation pRet;
    spGameMap pMap = GameMap::getInstance();
    if (pDefUnit != nullptr && pMap.get() != nullptr)
    {
        // log this attack to our battle log
        qint32 atkDamage = GlobalUtils::roundUp(defStartHp) - GlobalUtils::roundUp(defEndHp);
        qint32 defDamage = GlobalUtils::roundUp(atkStartHp) - GlobalUtils::roundUp(atkEndHp);

        pMap->getGameRecorder()->logAttack(pMap->getCurrentDay(),
                                           atkDamage, pAtkTerrain->Terrain::getX(), pAtkTerrain->Terrain::getY(), pAtkUnit->getUnitID(), pAtkUnit->getOwner()->getPlayerID(), (atkEndHp <= 0),
                                           defDamage, pDefTerrain->Terrain::getX(), pDefTerrain->Terrain::getY(), pDefUnit->getUnitID(), pDefUnit->getOwner()->getPlayerID(), (defEndHp <= 0),
                                           pDefUnit->getOwner()->getFieldVisible(pAtkTerrain->Terrain::getX(), pAtkTerrain->Terrain::getY()));
        auto battleViewMode = Settings::getBattleAnimations();
        if (battleViewMode == GameEnums::BattleAnimationMode_Overworld)
        {
            pRet = createOverworldBattleAnimation(pAtkTerrain, pAtkUnit, atkStartHp, atkEndHp, atkWeapon,
                                                  pDefTerrain, pDefUnit, defStartHp, defEndHp, defWeapon, defenderDamage);
        }
        else
        {
            pRet = spBattleAnimation::create(pAtkTerrain, pAtkUnit, atkStartHp, atkEndHp, atkWeapon,
                                             pDefTerrain, pDefUnit, defStartHp, defEndHp, defWeapon, defenderDamage);
            oxygine::spSprite pBack;

            if (battleViewMode == GameEnums::BattleAnimationMode_Fullscreen ||
                battleViewMode == GameEnums::BattleAnimationMode_FullscreenTransparent ||
                battleViewMode == GameEnums::BattleAnimationMode_FullscreenTransparent ||
                battleViewMode == GameEnums::BattleAnimationMode_DetailTransparent)
            {
                oxygine::ResAnim* pAnim = GameManager::getInstance()->getResAnim("fullscreen_battlebackground", oxygine::ep_ignore_error);
                if (pAnim != nullptr)
                {
                    pBack = oxygine::spSprite::create();
                    pBack->setResAnim(pAnim);
                    pBack->setScaleX(Settings::getWidth() / pAnim->getWidth());
                    pBack->setScaleY(Settings::getHeight() / pAnim->getHeight());
                    pBack->setSize(pAnim->getSize());
                }
                else
                {
                    oxygine::spColorRectSprite pRect = oxygine::spColorRectSprite::create();
                    pRect->setSize(Settings::getWidth(), Settings::getHeight());
                    pRect->setColor(pAtkUnit->getOwner()->getColor().darker(120));
                    pBack = pRect;
                }
            }
            if (battleViewMode == GameEnums::BattleAnimationMode_DetailTransparent ||
                battleViewMode == GameEnums::BattleAnimationMode_FullscreenTransparent)
            {
                pBack->setAlpha(128);
            }
            qint32 scaleFactor = 2.0f;
            if (scaleFactor * (pRet->getHeight() - 30) > Settings::getHeight())
            {
                scaleFactor = 1.0f;
                while (scaleFactor * (pRet->getHeight() - 30) > Settings::getHeight())
                {
                    scaleFactor /= 2;
                }
            }
            if (battleViewMode == GameEnums::BattleAnimationMode_Fullscreen ||
                battleViewMode == GameEnums::BattleAnimationMode_FullscreenTransparent)
            {
                float scale = Settings::getHeight() / (pRet->getHeight() - 30);
                float widthScale = Settings::getWidth() / (pRet->getWidth() - 30);
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
            pRet->setPosition(static_cast<qint32>(Settings::getWidth() / 2 - pRet->getScaledWidth() / 2),
                              static_cast<qint32>(Settings::getHeight() / 2 - pRet->getScaledHeight() / 2));
            spGameMenue pMenu = GameMenue::getInstance();
            if (pMenu.get())
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
            m_Animations.append(pRet);
        }
    }
    else
    {
        // attacking building or terrain
        pRet = createAnimation(pDefTerrain->Terrain::getX(), pDefTerrain->Terrain::getY(), 70);
        pRet->addSprite("blackhole_shot", -GameMap::getImageSize() * 0.5f, -GameMap::getImageSize() * 0.5f, 0, 2.0f);
        pRet->setSound("talongunhit.wav", 1);
    }

    return pRet.get();
}

GameAnimation* GameAnimationFactory::createOverworldBattleAnimation(Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp, qint32 atkWeapon,
                                                                    Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp, qint32 defWeapon, float defenderDamage)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(pAtkTerrain);
    args << obj;
    QJSValue obj1 = pInterpreter->newQObject(pAtkUnit);
    args << obj1;
    args << atkStartHp;
    args << atkEndHp;
    args << atkWeapon;
    QJSValue obj2 = pInterpreter->newQObject(pDefTerrain);
    args << obj2;
    QJSValue obj3 = pInterpreter->newQObject(pDefUnit);
    args << obj3;
    args << defStartHp;
    args << defEndHp;
    args << defWeapon;
    args << defenderDamage;
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
    removeAnimation(pAnimation, skipping, true);
}

void GameAnimationFactory::removeAnimation(GameAnimation* pAnimation, bool skipping, bool removeFromQueue)
{
    spGameAnimation spAnimation = spGameAnimation(pAnimation);
    if (removeFromQueue)
    {
        removeAnimationFromQueue(spAnimation);
    }
    if (spAnimation.get() != nullptr)
    {
        spAnimation->detach();
    }
    if (m_Animations.size() == 0 && !skipping)
    {
        CONSOLE_PRINT("GameAnimationFactory -> emitting animationsFinished()", Console::eDEBUG);
        emit GameAnimationFactory::getInstance()->animationsFinished();
    }
}

void GameAnimationFactory::clearAllAnimations()
{
    CONSOLE_PRINT("GameAnimationFactory -> clearAllAnimations()", Console::eDEBUG);
    for (qint32 i = 0; i < m_Animations.size(); i++)
    {
        m_Animations[i]->detach();
    }
    m_Animations.clear();

}

void GameAnimationFactory::finishAllAnimations()
{
    CONSOLE_PRINT("GameAnimationFactory::finishAllAnimations()", Console::eDEBUG);
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
        CONSOLE_PRINT("GameAnimationFactory -> emitting animationsFinished()", Console::eDEBUG);
        emit GameAnimationFactory::getInstance()->animationsFinished();
    }
}
