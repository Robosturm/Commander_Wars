#include "game/gameanimationfactory.h"
#include "game/gamemap.h"
#include "game/gameanimationwalk.h"
#include "game/gameanimationcapture.h"
#include "game/gameanimationdialog.h"
#include "game/gameanimationpower.h"
#include "game/gameanimationnextday.h"
#include "game/battleanimation.h"
#include "game/unit.h"
#include "game/gameaction.h"

#include "menue/gamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"

GameAnimationFactory* GameAnimationFactory::m_pInstance = nullptr;
QVector<spGameAnimation> GameAnimationFactory::m_Animations;

GameAnimationFactory::GameAnimationFactory()
    : QObject()
{
    setObjectName("GameAnimationFactory");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
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
    if (m_pInstance == nullptr)
    {
        m_pInstance = new GameAnimationFactory();
    }
    return m_pInstance;
}

GameAnimation* GameAnimationFactory::createAnimation(quint32 x, quint32 y, quint32 frameTime, bool mapPosition)
{
    
    spGameAnimation animation = new GameAnimation(frameTime);
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
    
    GameAnimationWalk* pGameAnimationWalk = new GameAnimationWalk(pUnit, pAction->getMovePath());
    pGameAnimationWalk->setPriority(static_cast<qint32>(Mainapp::ZOrder::Animation));
    GameMap::getInstance()->addChild(pGameAnimationWalk);
    m_Animations.append(pGameAnimationWalk);
    
    return pGameAnimationWalk;
}

GameAnimationPower* GameAnimationFactory::createAnimationPower(QColor color, GameEnums::PowerMode powerMode, CO* pCO, quint32 frameTime)
{
    
    GameAnimationPower* pAnim = GameAnimationPower::createGameAnimationPower(frameTime, color, powerMode, pCO);
    pAnim->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        pGameMenue->addChild(pAnim);
    }
    m_Animations.append(pAnim);
    
    return pAnim;
}

GameAnimationDialog* GameAnimationFactory::createGameAnimationDialog(QString text, QString coid, GameEnums::COMood mood, QColor color, quint32 frameTime)
{
    
    GameAnimationDialog* pAnim = new GameAnimationDialog(frameTime);
    pAnim->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));
    pAnim->setDialog(text);
    pAnim->setCO(coid, mood);
    pAnim->setColor(color);
    oxygine::getStage()->addChild(pAnim);
    m_Animations.append(pAnim);
    
    return pAnim;
}

GameAnimationNextDay* GameAnimationFactory::createGameAnimationNextDay(Player* pPlayer, quint32 frameTime)
{
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        GameAnimationNextDay* pAnim = new GameAnimationNextDay(pPlayer, frameTime, false);
        pGameMenue->addChild(pAnim);
        m_Animations.append(pAnim);
        
        return pAnim;
    }
    return nullptr;
}

GameAnimationCapture* GameAnimationFactory::createGameAnimationCapture(qint32 x, qint32 y, qint32 startPoints, qint32 endPoints, qint32 maxPoints)
{
    
    GameAnimationCapture* pGameAnimationCapture = new GameAnimationCapture(startPoints, endPoints, maxPoints);
    pGameAnimationCapture->setPriority(static_cast<qint32>(Mainapp::ZOrder::Animation));
    pGameAnimationCapture->setPosition(x, y);
    GameMap::getInstance()->addChild(pGameAnimationCapture);
    m_Animations.append(pGameAnimationCapture);
    
    return pGameAnimationCapture;
}

GameAnimation* GameAnimationFactory::createBattleAnimation(Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp, qint32 atkWeapon,
                                                           Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp, qint32 defWeapon, float defenderDamage)
{
    
    GameAnimation* pRet = nullptr;
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
            // attacking unit
            GameAnimation* pAtk = createAnimation(pDefTerrain->Terrain::getX(), pDefTerrain->Terrain::getY(), 70);
            pAtk->addSprite("blackhole_shot", -GameMap::getImageSize() * 0.5f, -GameMap::getImageSize() * 0.5f, 0, 1.5f);
            pAtk->setSound("talongunhit.wav", 1);
            GameAnimation* pDmgTextAtk = createAnimation(pDefTerrain->Terrain::getX(), pDefTerrain->Terrain::getY());
            pDmgTextAtk->addText(QString::number(atkDamage) + " Hp", -8, 0, 1.5f, Qt::GlobalColor::red);
            pDmgTextAtk->addTweenPosition(QPoint(pDefTerrain->Terrain::getX() * GameMap::getImageSize(), (pDefTerrain->Terrain::getY() - 2) * GameMap::getImageSize()), 1000);
            pDmgTextAtk->addTweenWait(1500);
            pAtk->queueAnimation(pDmgTextAtk);
            if (defenderDamage >= 0)
            {
                // counter damage
                pRet = createAnimation(pAtkTerrain->Terrain::getX(), pAtkTerrain->Terrain::getY(), 70);
                pRet->addSprite("blackhole_shot", -GameMap::getImageSize() * 0.5f, -GameMap::getImageSize() * 0.5f, 0, 1.5f);
                pRet->setSound("talongunhit.wav", 1);
                pDmgTextAtk->queueAnimation(pRet);
                GameAnimation* pDmgTextDef = createAnimation(pAtkTerrain->Terrain::getX(), pAtkTerrain->Terrain::getY());
                pDmgTextDef->addText(QString::number(defDamage) + " Hp", -8, 0, 1.5f, Qt::GlobalColor::red);
                pDmgTextDef->addTweenPosition(QPoint(pAtkTerrain->Terrain::getX() * GameMap::getImageSize(), (pAtkTerrain->Terrain::getY() - 2) * GameMap::getImageSize()), 1000);
                pDmgTextDef->addTweenWait(1500);
                pRet->queueAnimation(pDmgTextDef);
                pRet = pDmgTextDef;
            }
            else
            {
                pRet = pDmgTextAtk;
            }
        }
        else
        {
            pRet = new BattleAnimation(pAtkTerrain, pAtkUnit, atkStartHp, atkEndHp, atkWeapon,
                                       pDefTerrain, pDefUnit, defStartHp, defEndHp, defWeapon, defenderDamage);
            oxygine::spColorRectSprite pBack = new oxygine::ColorRectSprite();
            pBack->setSize(Settings::getWidth(), Settings::getHeight());
            pBack->setColor(pAtkUnit->getOwner()->getColor().darker(120));
            if (battleViewMode == GameEnums::BattleAnimationMode_DetailTransparent ||
                battleViewMode == GameEnums::BattleAnimationMode_FullscreenTransparent)
            {
                pBack->setAlpha(128);
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
                qint32 newScale = 2;
                qint32 lastScale = 2;
                while (newScale < scale)
                {
                    lastScale = newScale;
                    newScale *= 2;
                }
                pRet->setScale(lastScale);
            }
            else
            {
                pRet->setScale(2.0f);
            }
            pRet->setPosition(static_cast<qint32>(Settings::getWidth() / 2 - pRet->getScaledWidth() / 2),
                              static_cast<qint32>(Settings::getHeight() / 2 - pRet->getScaledHeight() / 2));
            spGameMenue pMenu = GameMenue::getInstance();
            if (pMenu.get())
            {
                if (pBack.get() != nullptr)
                {
                    pBack->setPriority(BattleAnimation::priorityBackground);
                    pBack->setPosition(-pRet->getPosition());
                    pRet->addChild(pBack);
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
        pRet->addSprite("blackhole_shot", -GameMap::getImageSize() * 0.5f, -GameMap::getImageSize() * 0.5f, 0, 1.5f);
        pRet->setSound("talongunhit.wav", 1);
    }
    
    return pRet;
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

void GameAnimationFactory::removeAnimation(GameAnimation* pAnimation, bool skipping)
{
    
    qint32 i = 0;
    while (i < m_Animations.size())
    {
        if (m_Animations[i].get() == pAnimation)
        {
            spGameAnimation spAnimation = m_Animations[i];
            spAnimation->detach();
            m_Animations.removeAt(i);
        }
        else
        {
             i++;
        }
    }
    if (m_Animations.size() == 0 && !skipping)
    {
        Console::print("GameAnimationFactory -> emitting animationsFinished()", Console::eDEBUG);
        emit GameAnimationFactory::getInstance()->animationsFinished();
    }    
}

void GameAnimationFactory::clearAllAnimations()
{
    
    for (qint32 i = 0; i < m_Animations.size(); i++)
    {
        m_Animations[i]->detach();
    }
    m_Animations.clear();
    
}

void GameAnimationFactory::finishAllAnimations()
{
    Console::print("GameAnimationFactory::finishAllAnimations()", Console::eDEBUG);
    qint32 i = 0;
    while (i < m_Animations.size())
    {
        spGameAnimation spAnimation = m_Animations[i];
        if (!spAnimation->onFinished(true))
        {
            i++;
        }
    }    
}
