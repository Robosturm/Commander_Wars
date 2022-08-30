#include <QColor>

#include "3rd_party/oxygine-framework/oxygine/tween/tweentogglevisibility.h"

#include "resource_management/unitspritemanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/weaponmanager.h"
#include "resource_management/movementtablemanager.h"

#include "ai/coreai.h"

#include "coreengine/console.h"
#include "coreengine/globalutils.h"

#include "game/gamemap.h"
#include "game/gameaction.h"
#include "game/co.h"
#include "game/createoutline.h"
#include "game/building.h"
#include "game/player.h"
#include "game/unit.h"
#include "game/gameanimation/gameanimation.h"

#include "menue/editormenue.h"

Unit::Unit(GameMap* pMap)
    : m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("Unit");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    setHeight(GameMap::getImageSize());
    setWidth(GameMap::getImageSize());
}

Unit::Unit(const QString & unitID, Player* pOwner, bool aquireId, GameMap* pMap)
    : m_UnitID(unitID),
      m_pOwner(pOwner),
      m_pMap{pMap}
{

#ifdef GRAPHICSUPPORT
    setObjectName("Unit");
#endif
    setHeight(GameMap::getImageSize());
    setWidth(GameMap::getImageSize());
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    if (!m_UnitID.isEmpty())
    {
        initUnit();
        setFuel(m_maxFuel);
        setAmmo1(m_maxAmmo1);
        setAmmo2(m_maxAmmo2);
        updateSprites(false);
        if (aquireId)
        {
            m_UniqueID = m_pMap->getUniqueIdCounter();
        }
    }
}

Unit::~Unit()
{
    if (m_CORange.get() != nullptr)
    {
        m_CORange->removeChildren();
        m_CORange->detach();
    }
    for (auto & customRange: m_customRangeInfo)
    {
        if (customRange.pActor.get())
        {
            customRange.pActor->removeChildren();
            customRange.pActor->detach();
        }
    }
}

bool Unit::isValid()
{
    return UnitSpriteManager::getInstance()->exists(m_UnitID);
}

QString Unit::getDescription()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    // load sprite of the base terrain
    QString function = "getDescription";
    QJSValue ret = pInterpreter->doFunction(m_UnitID, function, args);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

void Unit::setModdingFlags(ModdingFlags value)
{
    m_ModdingFlags = value;
}

Unit::ModdingFlags Unit::getModdingFlags() const
{
    return m_ModdingFlags;
}

void Unit::applyMod()
{
    if ((m_ModdingFlags & ModdingFlags::FlagAmmo1) == ModdingFlags::None)
    {
        setAmmo1(getMaxAmmo1());
    }
    if ((m_ModdingFlags & ModdingFlags::FlagAmmo2) == ModdingFlags::None)
    {
        setAmmo2(getMaxAmmo2());
    }
    if ((m_ModdingFlags & ModdingFlags::FlagFuel) == ModdingFlags::None)
    {
        setFuel(getMaxFuel());
    }
    if (m_UnitRank == GameEnums::UnitRank_CO0)
    {
        makeCOUnit(0);
    }
    else if (m_UnitRank == GameEnums::UnitRank_CO1)
    {
        makeCOUnit(1);
    }
}

void Unit::postBattleActions(float damage, Unit* pUnit, bool gotAttacked, qint32 weapon, GameAction* pAction)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "postBattleActions";
    QJSValueList args({pInterpreter->newQObject(this),
                       damage,
                       pInterpreter->newQObject(pUnit),
                       gotAttacked,
                       weapon,
                       pInterpreter->newQObject(pAction),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_UnitID, function1, args);
}

void Unit::setOwner(Player* pOwner)
{
    // change ownership
    if (m_UnitRank == GameEnums::UnitRank_CO0)
    {
        m_pOwner->getCO(0)->setCOUnit(nullptr);
        setUnitRank(getMaxUnitRang());
    }
    else if (m_UnitRank == GameEnums::UnitRank_CO1)
    {
        m_pOwner->getCO(1)->setCOUnit(nullptr);
        setUnitRank(getMaxUnitRang());
    }
    showRanges();
    m_pOwner = pOwner;
    for (auto & loadedUnit : m_TransportUnits)
    {
        loadedUnit->setOwner(pOwner);
    }
    if (m_pOwner != nullptr)
    {
        // update sprites :)
        updateSprites(false);
    }
}

void Unit::setTerrain(Terrain* pTerrain)
{
    m_pTerrain = pTerrain;
}

void Unit::addShineTween()
{    
    removeShineTween();
    for (auto & child : m_children)
    {
        oxygine::spVStyleActor pActor = oxygine::dynamic_pointer_cast<oxygine::VStyleActor>(child);
        if (pActor.get() != nullptr)
        {
            oxygine::spTween shineTween = oxygine::createTween(oxygine::VStyleActor::TweenAddColor(QColor(50, 50, 50, 0)), oxygine::timeMS(500), -1, true);
            pActor->addTween(shineTween);
            m_ShineTweens.append(shineTween);
        }
    }
}

void Unit::removeShineTween()
{
    QColor addColor(0, 0, 0, 0);
    for (qint32 i = 0; i < m_ShineTweens.size(); ++i)
    {
        if (m_ShineTweens[i].get() != nullptr)
        {
            oxygine::spActor pActor = oxygine::spActor(m_ShineTweens[i]->getClient());
            if (pActor.get() != nullptr)
            {
                m_ShineTweens[i]->removeFromActor();
                oxygine::spVStyleActor pVStyle = oxygine::dynamic_pointer_cast<oxygine::VStyleActor>(pActor);
                if (pVStyle.get() != nullptr)
                {
                    pVStyle->setAddColor(addColor);
                }
            }
        }
    }
    m_ShineTweens.clear();
}

void Unit::loadSprite(const QString & spriteID, bool addPlayerColor, bool flipSprite, qint32 frameTime)
{
    if (addPlayerColor)
    {
        loadSpriteV2(spriteID, GameEnums::Recoloring_Mask, flipSprite, frameTime);
    }
    else
    {
        loadSpriteV2(spriteID, GameEnums::Recoloring_None, flipSprite, frameTime);
    }
}

void Unit::loadSpriteV2(const QString & spriteID, GameEnums::Recoloring mode, bool flipSprite, qint32 frameTime)
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pUnitSpriteManager->getResAnim(spriteID, oxygine::ep_ignore_error);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        oxygine::spSprite pWaitSprite = oxygine::spSprite::create();
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(static_cast<qint64>(pAnim->getTotalFrames() * frameTime)), -1);
            pSprite->addTween(tween);

            oxygine::spTween tweenWait = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(static_cast<qint64>(pAnim->getTotalFrames() * frameTime)), -1);
            pWaitSprite->addTween(tweenWait);
        }
        else
        {
            pSprite->setResAnim(pAnim);
            pWaitSprite->setResAnim(pAnim);
        }
        pSprite->setPriority(static_cast<short>(Priorities::Colored));
        pWaitSprite->setPriority(static_cast<short>(Priorities::Waiting));
        // repaint the unit?
        if (mode == GameEnums::Recoloring_Mask)
        {
            QColor color = m_pOwner->getColor();
            pSprite->setColor(color);
        }
        else if (mode == GameEnums::Recoloring_Table ||
                 mode == GameEnums::Recoloring_Matrix)
        {
            bool matrixMode = mode == GameEnums::Recoloring_Matrix;
            pSprite->setColorTable(m_pOwner->getColorTableAnim(), matrixMode);
            pWaitSprite->setColorTable(m_pOwner->getColorTableAnim(), matrixMode);
        }
        else
        {
            pSprite->setPriority(static_cast<short>(Priorities::Outline));
            pWaitSprite->setPriority(static_cast<short>(Priorities::OutlineWaiting));
        }
        pSprite->setScale(GameMap::getImageSize() / pAnim->getWidth());
        pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::getImageSize()) / 2, -(pSprite->getScaledHeight() - GameMap::getImageSize()));
        addChild(pSprite);
        m_pUnitSprites.append(pSprite);
        pWaitSprite->setColor(QColor(100, 100, 100, 170));
        pWaitSprite->setScale(GameMap::getImageSize() / pAnim->getWidth());
        pWaitSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::getImageSize()) / 2, -(pSprite->getScaledHeight() - GameMap::getImageSize()));
        addChild(pWaitSprite);
        pWaitSprite->setVisible(false);
        m_pUnitWaitSprites.append(pWaitSprite);
        if (m_pOwner->getFlipUnitSprites() && flipSprite)
        {
            pSprite->setFlippedX(true);
            pWaitSprite->setFlippedX(true);
        }
    }
    else
    {
        CONSOLE_PRINT("Unable to load unit sprite: " + spriteID, Console::eDEBUG);
    }
}

void Unit::syncAnimation(oxygine::timeMS syncTime)
{
#ifdef GRAPHICSUPPORT
    for (auto & sprite : m_pUnitSprites)
    {
        auto & tweens = sprite->getTweens();
        for (auto & pTween : tweens)
        {
            pTween->setElapsed(syncTime);
        }
    }
    for (auto & sprite : m_pUnitWaitSprites)
    {
        auto & tweens = sprite->getTweens();
        for (auto & pTween : tweens)
        {
            pTween->setElapsed(syncTime);
        }
    }
    for (auto & icons : m_pIconSprites)
    {
        auto & tweens = icons->getTweens();
        for (auto & pTween : tweens)
        {
            pTween->setElapsed(syncTime);
        }
    }
#endif
}

Player* Unit::getOwner()
{
    return m_pOwner;
}

QString Unit::getName()
{
    if (m_customName.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "getName";
        QJSValueList args({pInterpreter->newQObject(this),
                           pInterpreter->newQObject(m_pMap)});
        QJSValue ret = pInterpreter->doFunction(m_UnitID, function1, args);
        if (ret.isString())
        {
            return ret.toString();
        }
        else
        {
            return "";
        }
    }
    return m_customName;
}

qint32 Unit::getUnitType()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getUnitType";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_UnitID, function1, args);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return static_cast<qint32>(GameEnums::UnitType_Ground);
    }
}

bool Unit::getCOSpecificUnit()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getCOSpecificUnit";
    QJSValueList args({pInterpreter->newQObject(nullptr),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_UnitID, function1, args);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    else
    {
        return false;
    }
}

qint32 Unit::getFuelCostModifier(QPoint position, qint32 costs)
{
    qint32 modifier = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        modifier += pCO->getFuelCostModifier(this, position, costs);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        modifier += pCO->getFuelCostModifier(this, position, costs);
    }
    return modifier;
}

void Unit::resetSprites()
{
    for (qint32 i = 0; i < m_pUnitSprites.size(); i++)
    {
        m_pUnitSprites[i]->detach();
    }
    for (qint32 i = 0; i < m_pUnitWaitSprites.size(); i++)
    {
        m_pUnitWaitSprites[i]->detach();
    }
    // call the js loader function to do the rest
    m_pUnitSprites.clear();
    m_pUnitWaitSprites.clear();
}

void Unit::updateSprites(bool editor)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    resetSprites();
    QString function1 = "loadSprites";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_UnitID, function1, args);

    setHp(m_hp);
    setFuel(m_fuel);
    setAmmo1(m_ammo1);
    setAmmo2(m_ammo2);
    setUnitRank(m_UnitRank);
    if (!editor)
    {
        if (m_UnitRank == GameEnums::UnitRank_CO0)
        {
            makeCOUnit(0);
        }
        else if (m_UnitRank == GameEnums::UnitRank_CO1)
        {
            makeCOUnit(1);
        }
    }
    setHasMoved(m_Moved);
    for (qint32 i = 0; i < m_TransportUnits.size(); i++)
    {
        m_TransportUnits[i]->updateSprites(false);
    }
    if (m_pOwner != nullptr)
    {
        CO* pCO1 = m_pOwner->getCO(0);
        CO* pCO2 = m_pOwner->getCO(1);
        if ((pCO1 != nullptr && pCO1->getPowerMode() > GameEnums::PowerMode_Off) ||
            (pCO2 != nullptr && pCO2->getPowerMode() > GameEnums::PowerMode_Off))
        {
            addShineTween();
        }
    }
}

qint32 Unit::getUnitRank() const
{
    return m_UnitRank;
}

qint32 Unit::getMaxUnitRang()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMaxRang";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction("UNITRANKINGSYSTEM", function1, args);
    if (ret.isNumber())
    {
        return ret.toNumber();
    }
    return 0;
}

QString Unit::getUnitRangIcon()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getIcon";
    QJSValueList args({m_UnitRank,
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction("UNITRANKINGSYSTEM", function1, args);
    if (ret.isString())
    {
        return ret.toString();
    }
    return "";
}

QString Unit::getUnitRangName(qint32 rang)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getName";
    QJSValueList args({QJSValue(rang)});
    QJSValue ret = pInterpreter->doFunction("UNITRANKINGSYSTEM", function1, args);
    if (ret.isString())
    {
        return ret.toString();
    }
    return "Unknown";
}

void Unit::setUnitRank(const qint32 &UnitRank, bool force)
{
    if (m_pMap != nullptr)
    {
        if (m_pMap->getGameRules()->getRankingSystem() ||
            (UnitRank == GameEnums::UnitRank_CO0) ||
            (UnitRank == GameEnums::UnitRank_CO1) ||
            force)
        {
            m_UnitRank = UnitRank;
        }
        unloadIcon("co0");
        unloadIcon("co1");
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "unloadIcons";
        QJSValueList args({pInterpreter->newQObject(this),
                           pInterpreter->newQObject(m_pMap)});
        pInterpreter->doFunction("UNITRANKINGSYSTEM", function1, args);
        loadIcon(getUnitRangIcon(), GameMap::getImageSize() / 2, GameMap::getImageSize() / 2);
        updateRankInfoVisibility(m_pMap->getCurrentViewPlayer());
    }
}

bool Unit::getWeatherImmune()
{
    bool immune = false;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getWeatherImmune";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, function1, args);
    if (erg.isBool())
    {
        immune = erg.toBool();
    }
    if (!immune)
    {
        immune = m_pOwner->getWeatherImmune();
    }
    return immune;
}

qint32 Unit::getVision(QPoint position)
{
    qint32 rangeModifier = 0;
    rangeModifier += getBonus(m_VisionBonus);
    for (qint32 i = 0; i < m_pOwner->getMaxCoCount(); ++i)
    {
        CO* pCO = m_pOwner->getCO(i);
        if (pCO != nullptr)
        {
            rangeModifier += pCO->getVisionrangeModifier(this, position);
        }
    }
    rangeModifier += getCoBonus(position, "getEnemyVisionBonus", &Player::getCoBonus);
    if (!getWeatherImmune())
    {
        rangeModifier += m_pMap->getGameRules()->getCurrentWeather()->getVisionrangeModifier();
    }
    qint32 mapHeigth = m_pMap->getMapHeight();
    qint32 mapWidth = m_pMap->getMapWidth();
    for (qint32 x = 0; x < mapWidth; x++)
    {
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            Building* pBuilding = m_pMap->getTerrain(x, y)->getBuilding();
            if ((pBuilding != nullptr) &&
                (pBuilding->getOwner() == m_pOwner))
            {
                rangeModifier += pBuilding->getVisionBonus();
            }
        }
    }
    Terrain* pTerrain = m_pMap->getTerrain(position.x(), position.y());
    if (pTerrain != nullptr)
    {
        rangeModifier += pTerrain->getBonusVision(this);
    }
    qint32 points = m_vision + rangeModifier;
    if (m_vision >= 1 && points < 1)
    {
        points = 1;
    }
    else if (points < 0)
    {
        points = 0;
    }
    return points;
}

qint32 Unit::getCoBonus(QPoint position, const QString & function, qint32(Player::*pBonusFunction)(QPoint, Unit*, const QString &))
{

    qint32 bonus = 0;
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = m_pMap->getPlayer(i);
        if (pPlayer != nullptr &&
            m_pOwner->isEnemy(pPlayer) &&
            !pPlayer->getIsDefeated())
        {
            bonus += (*pPlayer.*pBonusFunction)(position, this, function);
        }
    }
    return bonus;
}

GameMap *Unit::getMap() const
{
    return m_pMap;
}

void Unit::setVision(const qint32 &value)
{
    m_vision = value;
}

qint32 Unit::getBaseVision()
{
    return m_vision;
}

qint32 Unit::getMaxRange(QPoint position)
{
    return getMaxRangeAtPosition(position);
}

qint32 Unit::getBonusMaxRange(QPoint position)
{
    qint32 rangeModifier = 0;
    rangeModifier += getBonus(m_FirerangeBonus);
    for (qint32 i = 0; i < m_pOwner->getMaxCoCount(); ++i)
    {
        CO* pCO = m_pOwner->getCO(i);
        if (pCO != nullptr)
        {
            rangeModifier += pCO->getFirerangeModifier(this, position);
        }
    }
    rangeModifier += getCoBonus(position, "getEnemyFirerangeModifier", &Player::getCoBonus);


    if (!getWeatherImmune())
    {
        rangeModifier += m_pMap->getGameRules()->getCurrentWeather()->getFirerangeModifier();
    }
    // add terrain modifiers
    if (m_pMap->onMap(position.x(), position.y()))
    {
        rangeModifier += m_pMap->getTerrain(position.x(), position.y())->getFirerangeModifier(this);
    }
    return rangeModifier;
}

qint32 Unit::getMaxRangeAtPosition(QPoint position)
{
    qint32 points = m_maxRange + getBonusMaxRange(position);
    qint32 min = getMinRange(position);
    if (points < min)
    {
        points = min;
    }
    return points;
}

void Unit::setMaxRange(const qint32 &value)
{
    m_maxRange = value;
}

qint32 Unit::getBaseMaxRange()
{
    return m_maxRange;
}

qint32 Unit::getBaseMinRange() const
{
    return m_minRange;
}

qint32 Unit::getMinRange(QPoint position)
{
    qint32 points = m_minRange + getBonusMinRange(position);
    qint32 maxBonus = getBonusMaxRange(position);
    if (maxBonus > 0 && points > maxBonus + m_maxRange)
    {
        points = maxBonus + m_maxRange;
    }
    else if (points > m_maxRange)
    {
        points = m_maxRange;
    }
    if (points < 1)
    {
        points = 1;
    }
    return points;
}

qint32 Unit::getBonusMinRange(QPoint position)
{
    qint32 rangeModifier = 0;
    for (qint32 i = 0; i < m_pOwner->getMaxCoCount(); ++i)
    {
        CO* pCO = m_pOwner->getCO(i);
        if (pCO != nullptr)
        {
            rangeModifier += pCO->getMinFirerangeModifier(this, position);
        }
    }
    rangeModifier += getCoBonus(position, "getEnemyMinFirerangeModifier", &Player::getCoBonus);


    if (!getWeatherImmune())
    {
        rangeModifier += m_pMap->getGameRules()->getCurrentWeather()->getMinFirerangeModifier();
    }
    // add terrain modifiers
    if (m_pMap->onMap(position.x(), position.y()))
    {
        rangeModifier += m_pMap->getTerrain(position.x(), position.y())->getMinFirerangeModifier(this);
    }
    return rangeModifier;
}

void Unit::setMinRange(const qint32 &value)
{
    m_minRange = value;
}

qint32 Unit::getCosts() const
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBaseCost";
    QJSValueList args({pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, function1, args);
    if (erg.isNumber())
    {
        qint32 baseCost = erg.toInt();
        qint32 bonusCost = m_pOwner->getCostModifier(m_UnitID, baseCost, getPosition());
        return bonusCost + baseCost;
    }
    else
    {
        return -1;
    }
}

QString Unit::getTerrainAnimationBase(Unit* pDefender)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainAnimationBase";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pTerrain),
                       pInterpreter->newQObject(pDefender),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, function1, args);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

QString Unit::getTerrainAnimationForeground(Unit* pDefender)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainAnimationForeground";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pTerrain),
                       pInterpreter->newQObject(pDefender),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, function1, args);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

QString Unit::getTerrainAnimationBackground(Unit* pDefender)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainAnimationBackground";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pTerrain),
                       pInterpreter->newQObject(pDefender),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, function1, args);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

float Unit::getTerrainAnimationMoveSpeed()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainAnimationMoveSpeed";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, function1, args);
    if (erg.isNumber())
    {
        return erg.toNumber();
    }
    else
    {
        return 0.0f;
    }
}

bool Unit::canMoveOver(qint32 x, qint32 y)
{
    if (MovementTableManager::getInstance()->getBaseMovementPoints(getMovementType(), m_pMap->getTerrain(x, y), m_pMap->getTerrain(x, y), this) > 0)
    {
        return true;
    }
    return  false;
}

qint32 Unit::getCoUnitValue()
{
    qint32 value = getUnitValue();
    if (m_UnitRank < 0)
    {
        value *= 1.5f;
    }
    return value;
}

qint32 Unit::getUnitValue()
{
    return static_cast<qint32>(getCosts()  * m_hp / Unit::MAX_UNIT_HP);
}

bool Unit::canBeRepaired(QPoint position)
{

    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = m_pMap->getPlayer(i);
        if (pPlayer != nullptr &&
            m_pOwner->isEnemy(pPlayer) &&
            !pPlayer->getIsDefeated())
        {
            for (qint32 i = 0; i < pPlayer->getMaxCoCount(); ++i)
            {
                CO* pCO = pPlayer->getCO(i);
                if (pCO != nullptr)
                {
                    if (!pCO->canBeRepaired(this, position))
                    {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

void Unit::addOffensiveBonus(qint32 value, qint32 duration)
{
    m_OffensiveBonus.append(QPoint(value, duration));
}

void Unit::addDefensiveBonus(qint32 value, qint32 duration)
{
    m_DefensiveBonus.append(QPoint(value, duration));
}

void Unit::addVisionBonus(qint32 value, qint32 duration)
{
    m_VisionBonus.append(QPoint(value, duration));
}

void Unit::addMovementBonus(qint32 value, qint32 duration)
{
    m_MovementBonus.append(QPoint(value, duration));
}

void Unit::addFirerangeBonus(qint32 value, qint32 duration)
{
    m_FirerangeBonus.append(QPoint(value, duration));
}

bool Unit::isEnvironmentAttackable(QString terrainID)
{
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    if (hasAmmo1() && !m_weapon1ID.isEmpty())
    {
        if (pWeaponManager->getEnviromentDamage(m_weapon1ID, terrainID) > 0)
        {
            return true;
        }
    }
    if (hasAmmo2() && !m_weapon2ID.isEmpty())
    {
        if (pWeaponManager->getEnviromentDamage(m_weapon2ID, terrainID) > 0)
        {
            return true;
        }
    }
    return false;
}

float Unit::getEnvironmentDamage(QString terrainID)
{
    float damage = 0;
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    if (hasAmmo1() && !m_weapon1ID.isEmpty())
    {
        damage = pWeaponManager->getEnviromentDamage(m_weapon1ID, terrainID);
    }
    if (hasAmmo2() && !m_weapon2ID.isEmpty())
    {
        float damage2 = pWeaponManager->getEnviromentDamage(m_weapon2ID, terrainID);
        if (damage2 > damage)
        {
            damage = damage2;
        }
    }
    return damage;
}

bool Unit::isAttackableFromPosition(Unit* pDefender, QPoint unitPos)
{
    return isAttackable(pDefender, false, unitPos);
}

bool Unit::isAttackable(Unit* pDefender, bool ignoreOutOfVisionRange, QPoint unitPos, bool isDefenderPos)
{
    WeaponManager* pWeaponManager = WeaponManager::getInstance();

    if (pDefender != nullptr &&
        m_pMap != nullptr)
    {
        QPoint defPos;
        QPoint atkPos;
        if (isDefenderPos)
        {
            defPos = unitPos;
            atkPos = Unit::getPosition();
        }
        else
        {
            defPos = pDefender->Unit::getPosition();
            atkPos = unitPos;
        }

        if (m_pOwner->getFieldVisibleType(pDefender->Unit::getX(), pDefender->Unit::getY()) == GameEnums::VisionType::VisionType_Clear ||
            ignoreOutOfVisionRange)
        {
            if (!pDefender->isStealthed(m_pOwner, ignoreOutOfVisionRange))
            {
                if (!pDefender->isStatusStealthed() ||
                    (pDefender->isStatusStealthed() && canAttackStealthedUnit(pDefender)))
                {
                    if (m_pOwner->isEnemyUnit(pDefender) == true)
                    {
                        if (hasAmmo1() && !m_weapon1ID.isEmpty() &&
                            (!m_pMap->onMap(unitPos.x(), unitPos.y()) || canAttackWithWeapon(0, atkPos.x(), atkPos.y(), defPos.x(), defPos.y())))
                        {
                            if (pWeaponManager->getBaseDamage(m_weapon1ID, pDefender) > 0)
                            {
                                return true;
                            }
                        }
                        if (hasAmmo2() && !m_weapon2ID.isEmpty() &&
                            (!m_pMap->onMap(unitPos.x(), unitPos.y()) || canAttackWithWeapon(1, atkPos.x(), atkPos.y(), defPos.x(), defPos.y())))
                        {
                            if (pWeaponManager->getBaseDamage(m_weapon2ID, pDefender) > 0)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Unit::canAttackWithWeapon(qint32 weaponIndex, qint32 unitX, qint32 unitY, qint32 targetX, qint32 targetY, GameEnums::AttackRangeCheck rangeCheck)
{
    GameEnums::WeaponType weaponType = GameEnums::WeaponType::WeaponType_Both;
    if (weaponIndex == 0)
    {
        weaponType = getTypeOfWeapon1();
    }
    else
    {
        weaponType = getTypeOfWeapon2();
    }
    bool ret = false;
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (pInterpreter->exists("ACTION_FIRE", "extendedCanAttackCheck"))
    {
        QJSValueList args({pInterpreter->newQObject(this),
                           weaponIndex,
                           weaponType,
                           unitX,
                           unitY,
                           targetX,
                           targetY,
                           rangeCheck,
                           pInterpreter->newQObject(m_pMap)});
        QJSValue erg = pInterpreter->doFunction("ACTION_FIRE", "extendedCanAttackCheck", args);
        if (erg.isBool())
        {
            ret = erg.toBool();
        }
    }
    if (!ret)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args({pInterpreter->newQObject(this),
                           weaponIndex,
                           unitX,
                           unitY,
                           targetX,
                           targetY,
                           rangeCheck,
                           pInterpreter->newQObject(m_pMap)});
        QJSValue erg = pInterpreter->doFunction(m_UnitID, "canUseWeapon", args);
        if (!erg.isBool() || erg.toBool())
        {
            qint32 distance = GlobalUtils::getDistance(QPoint(unitX, unitY), QPoint(targetX, targetY));
            QPoint position(unitX, unitY);
            bool inIndirectRange = (distance >= getMinRange(position) || rangeCheck == GameEnums::AttackRangeCheck_None || rangeCheck == GameEnums::AttackRangeCheck_OnlyMax) &&
                                   (distance <= getMaxRange(position) || rangeCheck == GameEnums::AttackRangeCheck_None || rangeCheck == GameEnums::AttackRangeCheck_OnlyMin);
            if (weaponType == GameEnums::WeaponType::WeaponType_Both &&
                (inIndirectRange || distance == 1))
            {
                ret = true;
            }
            else
            {
                if ((weaponType == GameEnums::WeaponType::WeaponType_Direct && distance == 1) ||
                    rangeCheck == GameEnums::AttackRangeCheck_None ||
                    (weaponType == GameEnums::WeaponType::WeaponType_Indirect && inIndirectRange))
                {
                    ret = true;
                }
            }
        }
    }
    return ret;
}

bool Unit::hasDirectWeapon()
{
    return getTypeOfWeapon1() == GameEnums::WeaponType::WeaponType_Direct ||
           getTypeOfWeapon2() == GameEnums::WeaponType::WeaponType_Direct;
}

GameEnums::WeaponType Unit::getTypeOfWeapon1()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, "getTypeOfWeapon1", args);
    if (erg.isNumber())
    {
        return static_cast<GameEnums::WeaponType>(erg.toInt());
    }
    return GameEnums::WeaponType::WeaponType_Both;
}

GameEnums::WeaponType Unit::getTypeOfWeapon2()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, "getTypeOfWeapon2", args);
    if (erg.isNumber())
    {
        return static_cast<GameEnums::WeaponType>(erg.toInt());
    }
    return GameEnums::WeaponType::WeaponType_Both;
}

bool Unit::canAttackStealthedUnit(Unit* pDefender)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "canAttackStealthedUnit";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pDefender),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, function1, args);
    if (erg.isBool())
    {
        return erg.toBool();
    }
    return false;
}

Terrain* Unit::getTerrain()
{
    return m_pTerrain;
}

bool Unit::canMoveAndFire(QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "canMoveAndFire";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, function1, args);
    if (erg.isBool() && erg.toBool())
    {
        return true;
    }
    for (qint32 i = 0; i < m_pOwner->getMaxCoCount(); ++i)
    {
        CO* pCO = m_pOwner->getCO(i);
        if (pCO != nullptr)
        {
            if (pCO->getCanMoveAndFire(this, position))
            {
                return true;
            }
        }
    }
    return false;
}

void Unit::loadUnit(Unit* pUnit, qint32 index)
{
    bool loaded = false;
    if (m_TransportUnits.size() < getLoadingPlace() && index < 0)
    {
        m_TransportUnits.append(spUnit(pUnit));
        loaded = true;
    }
    else if (index < getLoadingPlace())
    {
        if (index < m_TransportUnits.size())
        {
            m_TransportUnits[index] = spUnit(pUnit);
        }
        else
        {
            m_TransportUnits.append(spUnit(pUnit));
        }
        loaded = true;
    }
    if (loaded)
    {
        pUnit->removeUnit(false);
        if (m_pMap != nullptr)
        {
            updateIcons(m_pMap->getCurrentViewPlayer());
        }
        else
        {
            updateIcons(nullptr);
        }
    }
}

void Unit::loadSpawnedUnit(const QString & unitId)
{
    CONSOLE_PRINT("Unit::loadSpawnedUnit " + unitId, Console::eDEBUG);
    spUnit pUnit = spUnit::create(unitId, m_pOwner, true, m_pMap);
    if (canTransportUnit(pUnit.get()))
    {
        loadUnit(pUnit.get());
    }
}

Unit* Unit::spawnUnit(const QString & unitID)
{
    CONSOLE_PRINT("Unit::spawnUnit " + unitID, Console::eDEBUG);

    if (m_pMap != nullptr)
    {
        qint32 unitLimit = m_pMap->getGameRules()->getUnitLimit();
        qint32 unitCount = m_pOwner->getUnitCount();
        if (unitLimit > 0 && unitCount >= unitLimit)
        {
            return nullptr;
        }
        spUnit pUnit = spUnit::create(unitID, m_pOwner, true, m_pMap);
        m_TransportUnits.append(pUnit);
        updateIcons(m_pMap->getCurrentViewPlayer());
        return pUnit.get();
    }
    return nullptr;
}

Unit* Unit::getLoadedUnit(qint32 index) const
{
    if ((index >= 0) && (index < m_TransportUnits.size()))
    {
        return m_TransportUnits[index].get();
    }
    return nullptr;
}

void Unit::unloadUnit(Unit* pUnit, QPoint position)
{

    if (m_pMap != nullptr && m_pMap->onMap(position.x(), position.y()))
    {
        for (qint32 i = 0; i < m_TransportUnits.size(); i++)
        {
            if (m_TransportUnits[i] == pUnit)
            {
                m_pMap->getTerrain(position.x(), position.y())->setUnit(m_TransportUnits[i]);
                m_TransportUnits[i]->updateIcons(m_pMap->getCurrentViewPlayer());
                m_TransportUnits[i]->showRanges();
                m_TransportUnits.removeAt(i);
                break;
            }
        }
        updateIcons(m_pMap->getCurrentViewPlayer());
    }
}

void Unit::unloadUnitAtIndex(qint32 index, QPoint position)
{

    if (index >= 0 && index < m_TransportUnits.size())
    {
        if (m_pMap != nullptr && m_pMap->onMap(position.x(), position.y()))
        {
            m_pMap->getTerrain(position.x(), position.y())->setUnit(m_TransportUnits[index]);
            m_TransportUnits[index]->updateIcons(m_pMap->getCurrentViewPlayer());
            m_TransportUnits[index]->showRanges();
        }
        m_TransportUnits.removeAt(index);
    }
    if (m_pMap != nullptr)
    {
        updateIcons(m_pMap->getCurrentViewPlayer());
    }
    else
    {
        updateIcons(nullptr);
    }
}

qint32 Unit::getLoadedUnitCount() const
{
    return m_TransportUnits.size();
}

QStringList  Unit::getTransportUnits()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTransportUnits";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, function1, args);
    QStringList transportUnits = erg.toVariant().toStringList();
    QStringList extraTransportUnits;
    if (m_pOwner != nullptr)
    {
        extraTransportUnits = m_pOwner->getTransportUnits(this);
    }
    if (extraTransportUnits.size() > 0)
    {
        for (qint32 i = 0; i < extraTransportUnits.size(); i++)
        {
            if (extraTransportUnits[i].startsWith("-"))
            {
                transportUnits.removeAll(extraTransportUnits[i].replace("-", ""));
            }
            else
            {
                transportUnits.append(extraTransportUnits[i]);
            }
        }
    }
    return transportUnits;
}

bool Unit::canTransportUnit(Unit* pUnit, bool ignoreLoadingPlace)
{
    QStringList transportableUnits = getTransportUnits();
    // can we carry the unit
    if (transportableUnits.contains(pUnit->getUnitID()))
    {
        // do we have space left?
        if (getLoadedUnitCount() < getLoadingPlace() || ignoreLoadingPlace)
        {
            return true;
        }
    }
    return false;
}

bool Unit::isTransporter()
{
    if (getLoadingPlace() > 0 &&
        !hasWeapons())
    {
        return true;
    }
    return false;
}

void Unit::postAction(spGameAction & pAction)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "postAction";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pAction.get()),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_UnitID, function1, args);
}

qint32 Unit::getBonusOffensive(GameAction* pAction, QPoint position, Unit* pDefender, QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    qint32 bonus = 0;
    bonus += getBonus(m_OffensiveBonus);
    bonus += getUnitBonusOffensive(pAction, position, pDefender, defPosition, isDefender, luckMode);

    CO* pCO0 = m_pOwner->getCO(0);
    if (pCO0 != nullptr)
    {
        bonus += pCO0->getOffensiveBonus(pAction, this, position, pDefender, defPosition, isDefender, luckMode);
    }
    CO* pCO1 = m_pOwner->getCO(1);
    if (pCO1 != nullptr)
    {
        bonus += pCO1->getOffensiveBonus(pAction, this, position, pDefender, defPosition, isDefender, luckMode);
    }
    if (m_pTerrain != nullptr)
    {
        Building* pBuilding = m_pTerrain->getBuilding();
        if (pBuilding != nullptr)
        {
            bonus += pBuilding->getOffensiveFieldBonus(pAction, this, position, pDefender, defPosition, isDefender, luckMode);
        }
        else
        {
            bonus += m_pTerrain->getOffensiveFieldBonus(pAction, this, position, pDefender, defPosition, isDefender, luckMode);
        }
    }
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = m_pMap->getPlayer(i);
        if (pPlayer != nullptr &&
            m_pOwner->isEnemy(pPlayer) &&
            !pPlayer->getIsDefeated())
        {
            pCO0 = pPlayer->getCO(0);
            if (pCO0 != nullptr)
            {
                bonus -= pCO0->getOffensiveReduction(pAction, this, position, pDefender, defPosition, isDefender, luckMode);
            }
            pCO1 = pPlayer->getCO(1);
            if (pCO1 != nullptr)
            {
                bonus -= pCO1->getOffensiveReduction(pAction, this, position, pDefender, defPosition, isDefender, luckMode);
            }
        }
    }

    pCO0 = m_pOwner->getCO(0);
    pCO1 = m_pOwner->getCO(1);
    if (pCO0 != nullptr && pCO1 != nullptr &&
        pCO0->getPowerMode() == GameEnums::PowerMode_Tagpower)
    {

        QString function1 = "getTagpower";
        QJSValueList args({pInterpreter->newQObject(pCO0),
                           pInterpreter->newQObject(pCO1),
                           pInterpreter->newQObject(m_pMap)});
        QJSValue erg = pInterpreter->doFunction("TAGPOWER", function1, args);
        if (erg.isNumber())
        {
            bonus += erg.toNumber();
        }
    }


    qint32 mapHeigth = m_pMap->getMapHeight();
    qint32 mapWidth = m_pMap->getMapWidth();
    for (qint32 x = 0; x < mapWidth; x++)
    {
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            Building* pBuilding = m_pMap->getTerrain(x, y)->getBuilding();
            if ((pBuilding != nullptr) && pBuilding->getOwner() == m_pOwner)
            {
                bonus += pBuilding->getOffensiveBonus();
            }
        }
    }
    if (!getWeatherImmune())
    {
        bonus += m_pMap->getGameRules()->getCurrentWeather()->getOffensiveModifier();
    }
    if (m_pMap->getGameRules()->getRankingSystem())
    {
        QString function1 = "getOffensiveBonus";
        QJSValueList args({pInterpreter->newQObject(this),
                            pInterpreter->newQObject(m_pMap)});
        QJSValue erg = pInterpreter->doFunction("UNITRANKINGSYSTEM", function1, args);
        if (erg.isNumber())
        {
            bonus += erg.toNumber();
        }
    }
    return bonus;
}

qint32 Unit::getUnitBonusOffensive(GameAction* pAction, QPoint position, Unit* pDefender, QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBonusOffensive";
    QJSValueList args({pInterpreter->newQObject(this),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(pDefender),
                       defPosition.x(),
                       defPosition.y(),
                       isDefender,
                       pInterpreter->newQObject(pAction),
                       luckMode,
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, function1, args);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Unit::getTerrainDefense(qint32 x, qint32 y)
{
    if (x < 0)
    {
        x = Unit::getX();
    }
    if (y < 0)
    {
        y = Unit::getY();
    }

    if (useTerrainDefense() && m_pTerrain != nullptr)
    {
        return m_pMap->getTerrain(x, y)->getDefense(this);
    }
    return 0;
}

float Unit::getDamageReduction(GameAction* pAction, float damage, Unit* pAttacker, QPoint position, qint32 attackerBaseHp,
                               QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode)
{
    float bonus = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        bonus += pCO->getDamageReduction(pAction, damage, pAttacker, position, attackerBaseHp,
                                         this, defPosition, isDefender, luckMode);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        bonus += pCO->getDamageReduction(pAction, damage, pAttacker, position, attackerBaseHp,
                                         this, defPosition, isDefender, luckMode);
    }
    return bonus;
}

float Unit::getTrueDamage(GameAction* pAction, float damage, QPoint position, qint32 attackerBaseHp,
                          Unit* pDefender, QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode)
{
    float bonus = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        bonus += pCO->getTrueDamage(pAction, damage, this, position, attackerBaseHp,
                                    pDefender, defPosition, isDefender, luckMode);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        bonus += pCO->getTrueDamage(pAction, damage, this, position, attackerBaseHp,
                                    pDefender, defPosition, isDefender, luckMode);
    }
    return bonus;
}

bool Unit::canCounterAttack(GameAction* pAction, QPoint position, Unit* pDefender, QPoint defPosition, GameEnums::LuckDamageMode luckMode)
{
    bool directCombat = qAbs(position.x() - defPosition.x()) + qAbs(position.y() - defPosition.y()) == 1 ;
    CO* pCO = m_pOwner->getCO(0);
    auto mode = GameEnums::CounterAttackMode_Undefined;
    if (pCO != nullptr)
    {
        mode = pCO->canCounterAttack(pAction, this, position, pDefender, defPosition, luckMode);
    }
    if (mode != GameEnums::CounterAttackMode_Impossible)
    {
        pCO = m_pOwner->getCO(1);
        if (pCO != nullptr)
        {
            auto mode2 = pCO->canCounterAttack(pAction, this, position, pDefender, defPosition, luckMode);
            if (mode2 != GameEnums::CounterAttackMode_Undefined)
            {
                mode = mode2;
            }
        }
    }
    if (mode != GameEnums::CounterAttackMode_Impossible)
    {

        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            if (pPlayer != nullptr &&
                m_pOwner->isEnemy(pPlayer) &&
                !pPlayer->getIsDefeated())
            {
                pCO = pPlayer->getCO(0);
                if (pCO != nullptr)
                {
                    auto mode2 = pCO->canCounterAttack(pAction, this, position, pDefender, defPosition, luckMode);
                    if (mode2 != GameEnums::CounterAttackMode_Undefined)
                    {
                        mode = mode2;
                    }
                }
                if (mode == GameEnums::CounterAttackMode_Impossible)
                {
                    break;
                }
                pCO = pPlayer->getCO(1);
                if (pCO != nullptr)
                {
                    auto mode2 = pCO->canCounterAttack(pAction, this, position, pDefender, defPosition, luckMode);
                    if (mode2 != GameEnums::CounterAttackMode_Undefined)
                    {
                        mode = mode2;
                    }
                }
                if (mode == GameEnums::CounterAttackMode_Impossible)
                {
                    break;
                }
            }
        }
    }
    return (directCombat || mode == GameEnums::CounterAttackMode_Possible) &&
            mode != GameEnums::CounterAttackMode_Impossible;
}

qint32 Unit::getUnitBonusDefensive(GameAction* pAction, QPoint position, Unit* pAttacker, QPoint atkPosition, bool isAttacker, GameEnums::LuckDamageMode luckMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBonusDefensive";
    QJSValueList args({pInterpreter->newQObject(this),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(pAttacker),
                       atkPosition.x(),
                       atkPosition.y(),
                       isAttacker,
                       pInterpreter->newQObject(pAction),
                       luckMode,
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, function1, args);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Unit::getBonusDefensive(GameAction* pAction, QPoint position, Unit* pAttacker, QPoint atkPosition, bool isAttacker, GameEnums::LuckDamageMode luckMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();

    qint32 bonus = 0;
    bonus += getBonus(m_DefensiveBonus);
    bonus += getUnitBonusDefensive(pAction, position, pAttacker, atkPosition, isAttacker, luckMode);
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        bonus += pCO->getDeffensiveBonus(pAction, pAttacker, atkPosition, this, position, isAttacker, luckMode);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        bonus += pCO->getDeffensiveBonus(pAction, pAttacker, atkPosition, this, position, isAttacker, luckMode);
    }
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = m_pMap->getPlayer(i);
        if (pPlayer != nullptr &&
            m_pOwner->isEnemy(pPlayer) &&
            !pPlayer->getIsDefeated())
        {
            pCO = pPlayer->getCO(0);
            if (pCO != nullptr)
            {
                bonus -= pCO->getDeffensiveReduction(pAction, pAttacker, atkPosition, this, position, isAttacker, luckMode);
            }
            pCO = pPlayer->getCO(1);
            if (pCO != nullptr)
            {
                bonus -= pCO->getDeffensiveReduction(pAction, pAttacker, atkPosition, this, position, isAttacker, luckMode);
            }
        }
    }
    qint32 mapHeigth = m_pMap->getMapHeight();
    qint32 mapWidth = m_pMap->getMapWidth();
    for (qint32 x = 0; x < mapWidth; x++)
    {
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            Building* pBuilding = m_pMap->getTerrain(x, y)->getBuilding();
            if ((pBuilding != nullptr) && pBuilding->getOwner() == m_pOwner)
            {
                bonus += pBuilding->getDefensiveBonus();
            }
        }
    }
    if (useTerrainDefense())
    {
        float hpReductionMalus = 1.0f;
        if ( m_pMap->getGameRules()->getHpDefenseReduction())
        {
            hpReductionMalus = static_cast<float>(getHpRounded()) / MAX_UNIT_HP;
        }
        bonus += getTerrainDefense(position.x(), position.y()) * m_pMap->getGameRules()->getTerrainDefense() * hpReductionMalus;
    }
    if (m_pTerrain != nullptr)
    {
        Building* pBuilding = m_pTerrain->getBuilding();
        if (pBuilding != nullptr)
        {
            bonus += pBuilding->getDeffensiveFieldBonus(pAction, pAttacker, atkPosition, this, position, isAttacker, luckMode);
        }
        else
        {
            bonus += m_pTerrain->getDeffensiveFieldBonus(pAction, pAttacker, atkPosition, this, position, isAttacker, luckMode);
        }
    }
    if (!getWeatherImmune())
    {
        bonus += m_pMap->getGameRules()->getCurrentWeather()->getDefensiveModifier();
    }
    if (m_pMap->getGameRules()->getRankingSystem())
    {
        QString function1 = "getDefensiveBonus";
        QJSValueList args({pInterpreter->newQObject(this),
                           pInterpreter->newQObject(m_pMap)});
        QJSValue erg = pInterpreter->doFunction("UNITRANKINGSYSTEM", function1, args);
        if (erg.isNumber())
        {
            bonus += erg.toNumber();
        }
    }
    return bonus;
}

bool Unit::useTerrainDefense()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "useTerrainDefense";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, function1, args);
    if (erg.isBool() && erg.toBool())
    {
        return true;
    }
    return false;
}

bool Unit::useTerrainHide()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "useTerrainHide";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, function1, args);
    if (erg.isBool() && erg.toBool())
    {
        return true;
    }
    return false;
}

qint32 Unit::getAttackHpBonus(QPoint position)
{
    qint32 bonus = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        bonus += pCO->getAttackHpBonus(this, position);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        bonus += pCO->getAttackHpBonus(this, position);
    }
    return bonus;
}

qint32 Unit::getBonusMisfortune(QPoint position)
{
    qint32 bonus = 0;
    if (m_pMap != nullptr)
    {
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            if (pPlayer != nullptr)
            {
                if (pPlayer->isEnemy(m_pOwner))

                {
                    bonus += pPlayer->getEnemyBonusMisfortune(this, position);
                }
                else if ( m_pOwner == pPlayer)
                {
                    bonus += pPlayer->getBonusMisfortune(this, position);
                }
            }
        }
    }
    return bonus;
}

qint32 Unit::getUnitCosts() const
{
    return m_pOwner->getCosts(m_UnitID, getPosition());
}

qint32 Unit::getRepairBonus(QPoint position)
{
    qint32 bonus = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        bonus += pCO->getRepairBonus(this, position);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        bonus += pCO->getRepairBonus(this, position);
    }
    return bonus;
}

void Unit::setUnitVisible(bool value, Player* pPlayer)
{
    if (m_CORange.get() != nullptr)
    {
        if (!getRankInfoHidden(pPlayer))
        {
            m_CORange->setVisible(value);
        }
        else
        {
            m_CORange->setVisible(false);
        }
    }
    for (auto & customRange: m_customRangeInfo)
    {
        if (customRange.pActor.get() == nullptr)
        {
            updateRangeActor(customRange.pActor,
                             customRange.range,
                             customRange.id,
                             customRange.color);
        }
        customRange.pActor->setVisible(value);
    }
    setVisible(value);
}

void Unit::makeCOUnit(quint8 co, bool force)
{
    CONSOLE_PRINT("Unit::makeCOUnit for " + QString::number(co) + " force=" + QString::number(force), Console::eDEBUG);
    CO* pCO = m_pOwner->getCO(co);
    if (pCO != nullptr &&
        (pCO->getCOUnit() == nullptr || force))
    {
        pCO->setCOUnit(this);
        if (co == 0)
        {
            setUnitRank(GameEnums::UnitRank_CO0);
        }
        else
        {
            setUnitRank(GameEnums::UnitRank_CO1);
        }
        createCORange(pCO->getCORange());
    }
    else if (pCO == nullptr || pCO->getCOUnit() != this)
    {
        setUnitRank(getMaxUnitRang());
    }
}

qint32 Unit::getBonusLuck(QPoint position)
{
    qint32 bonus = 0;
    if (m_pMap != nullptr)
    {
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            if (pPlayer != nullptr)
            {
                if (pPlayer->isEnemy(m_pOwner))

                {
                    bonus += pPlayer->getEnemyBonusLuck(this, position);
                }
                else if ( m_pOwner == pPlayer)
                {
                    bonus += pPlayer->getBonusLuck(this, position);
                }
            }
        }
    }

    // apply star bonus
    auto* pCO0 = m_pOwner->getCO(0);
    auto* pCO1 = m_pOwner->getCO(1);
    if (pCO0 != nullptr && pCO1 != nullptr &&
        pCO0->getPowerMode() == GameEnums::PowerMode_Tagpower)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "getTagstars";
        QJSValueList args({pInterpreter->newQObject(pCO0),
                           pInterpreter->newQObject(pCO1),
                           pInterpreter->newQObject(m_pMap)});
        QJSValue erg = pInterpreter->doFunction("TAGPOWER", function1, args);
        if (erg.isNumber())
        {
            bonus += erg.toInt() * 5;
        }
    }
    return bonus;
}

void Unit::endOfTurn()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "endOfTurn";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_UnitID, function1, args);
    for (qint32 i = 0; i < m_TransportUnits.size(); i++)
    {
        m_TransportUnits[i]->endOfTurn();
    }
}

void Unit::startOfTurn()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "startOfTurn";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_UnitID, function1, args);
    for (qint32 i = 0; i < m_TransportUnits.size(); i++)
    {
        m_TransportUnits[i]->startOfTurn();
    }
}

void Unit::updateStatusDurations(qint32 player)
{
    qint32 i = 0;
    std::vector<QString> removeList;
    removeList.reserve(m_IconDurations.size());
    std::vector<QString> existList;
    existList.reserve(m_IconDurations.size());
    while (i < m_IconDurations.size())
    {
        IconDuration & icon = m_IconDurations[i];
        if (icon.player == player)
        {
            --icon.duration;
            if (icon.duration <= 0)
            {
                removeList.push_back(icon.icon);
                m_IconDurations.removeAt(i);
            }
            else
            {
                existList.push_back(icon.icon);
                ++i;
            }
        }
        else
        {
            existList.push_back(icon.icon);
            ++i;
        }
    }
    for (const auto & item : removeList)
    {
        if (!GlobalUtils::contains(existList, item))
        {
            unloadIcon(item);
        }
    }
    for (qint32 i = 0; i < m_cloaked.size(); ++i)
    {
        if (m_cloaked[i].y() == player)
        {
            m_cloaked[i].setX(m_cloaked[i].x() - 1);
            if (m_cloaked[i].x() <= 0)
            {
                m_cloaked.removeAt(i);
            }
            break;
        }
    }
}

void Unit::updateUnitStatus()
{
    updateBonus(m_OffensiveBonus);
    updateBonus(m_DefensiveBonus);
    updateBonus(m_VisionBonus);
    updateBonus(m_MovementBonus);
    updateBonus(m_FirerangeBonus);
}

qint32 Unit::getCapturePoints() const
{
    return m_capturePoints;
}

void Unit::setCapturePoints(const qint32 &value)
{
    m_capturePoints = value;
    if (m_capturePoints < 0)
    {
        m_capturePoints = 0;
    }
    if (m_capturePoints > 0)
    {
        loadIcon("capture", GameMap::getImageSize() / 2, GameMap::getImageSize() / 2);
    }
    else
    {
        unloadIcon("capture");
    }
}

bool Unit::canCapture()
{
    return getBaseActionList().contains(CoreAI::ACTION_CAPTURE);
}

qint32 Unit::getBaseMovementPoints() const
{
    return m_baseMovementPoints;
}

void Unit::setBaseMovementPoints(const qint32 &value)
{
    m_baseMovementPoints = value;
}

qint32 Unit::getBaseMovementCosts(qint32 x, qint32 y, qint32 curX, qint32 curY, bool trapChecking)
{

    Terrain* pCurTerrain = nullptr;
    if (curX >= 0 && curY >= 0)
    {
        pCurTerrain = m_pMap->getTerrain(curX, curY);
    }
    else
    {
        pCurTerrain = m_pMap->getTerrain(x, y);
    }
    return MovementTableManager::getInstance()->getBaseMovementPoints(m_MovementType, m_pMap->getTerrain(x, y), pCurTerrain, this, trapChecking);
}

qint32 Unit::getMovementCosts(qint32 x, qint32 y, qint32 curX, qint32 curY, bool trapChecking)
{
    qint32 baseCosts = getBaseMovementCosts(x, y, curX, curY, trapChecking);
    if (baseCosts == 0)
    {
        return baseCosts;
    }
    qint32 costs = baseCosts;

    if (m_pMap != nullptr)
    {
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            if (pPlayer != nullptr &&
                (pPlayer->isEnemy(m_pOwner) ||
                 m_pOwner == pPlayer))
            {
                costs += pPlayer->getMovementcostModifier(this, QPoint(x, y));
            }
        }
    }
    qint32 weatherCosts = m_pOwner->getWeatherMovementCostModifier(this, QPoint(x, y));
    weatherCosts += m_pMap->getTerrain(x, y)->getMovementcostModifier(this, x, y, curX, curY);

    if ((costs <= 0) && (baseCosts > 0))
    {
        return 1;
    }
    else if (weatherCosts + costs >= 0 && costs + baseCosts < 0)
    {
        return -1;
    }
    else
    {
        return costs + weatherCosts;
    }
}

void Unit::initUnit()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QString function1 = "init";
    pInterpreter->doFunction(m_UnitID, function1, args);
    function1 = "initForMods";
    pInterpreter->doFunction(m_UnitID, function1, args);
    function1 = "getMovementType";
    QJSValue  erg = pInterpreter->doFunction(m_UnitID, function1, args);
    if (erg.isString())
    {
        m_MovementType = erg.toString();
    }
    setFuel(m_fuel);
    setAmmo1(m_ammo1);
    setAmmo2(m_ammo2);
    setHp(m_hp);
}

qint32 Unit::getMaxFuel() const
{
    return m_maxFuel;
}

void Unit::setMaxFuel(const qint32 &value)
{
    m_maxFuel = value;
}

qint32 Unit::getFuel() const
{
    return m_fuel;
}

void Unit::setFuel(const qint32 &value)
{
    if (m_maxFuel > 0)
    {
        m_fuel = value;
    }
    if (m_fuel > m_maxFuel)
    {
        m_fuel = m_maxFuel;
    }
    if (m_maxFuel > 0 && static_cast<float>(m_fuel) / static_cast<float>(m_maxFuel) <= Settings::getSupplyWarning())
    {
        loadIcon("fuel", GameMap::getImageSize() / 2, 0);
    }
    else
    {
        unloadIcon("fuel");
    }
}

qint32 Unit::getMaxAmmo2() const
{
    return m_maxAmmo2;
}

void Unit::setMaxAmmo2(const qint32 &value)
{
    m_maxAmmo2 = value;
}

qint32 Unit::getAmmo2() const
{
    return m_ammo2;
}

bool Unit::hasAmmo2() const
{
    if ((m_maxAmmo2 < 0) || (m_ammo2 > 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString Unit::getWeapon2ID() const
{
    return m_weapon2ID;
}

void Unit::setWeapon2ID(const QString &value)
{
    m_weapon2ID = value;
}

void Unit::setAmmo2(const qint32 &value)
{
    m_ammo2 = value;
    if ((m_ammo2 < 0) && (m_maxAmmo2 > 0))
    {
        m_ammo2 = 0;
    }
    else  if (m_maxAmmo2 > 0 && m_ammo2 < 0)
    {
        m_ammo2 = 0;
    }
    if (m_ammo2 > m_maxAmmo2)
    {
        m_ammo2 = m_maxAmmo2;
    }
    if (m_maxAmmo2 > 0 && static_cast<float>(m_ammo2) / static_cast<float>(m_maxAmmo2) <= Settings::getSupplyWarning())
    {
        if (m_weapon2ID.isEmpty())
        {
            loadIcon("material1", GameMap::getImageSize() / 2, 0);
        }
        else
        {
            loadIcon("ammo1", GameMap::getImageSize() / 2, 0);
        }
    }
    else
    {
        unloadIcon("ammo1");
        unloadIcon("material1");
    }
}

void Unit::reduceAmmo2(qint32 value)
{
    if (m_ammo2 >= 0)
    {
        setAmmo2(m_ammo2 - value);
    }
}

qint32 Unit::getMaxAmmo1() const
{
    return m_maxAmmo1;
}

void Unit::setMaxAmmo1(const qint32 &value)
{
    m_maxAmmo1 = value;
}

QString Unit::getWeapon1ID() const
{
    return m_weapon1ID;
}

void Unit::setWeapon1ID(const QString &value)
{
    m_weapon1ID = value;
}

qint32 Unit::getAmmo1() const
{
    return m_ammo1;
}

void Unit::setAmmo1(const qint32 &value)
{
    m_ammo1 = value;
    if ((m_ammo1 < 0) && (m_maxAmmo1 > 0))
    {
        m_ammo1 = 0;
    }
    else if (m_maxAmmo1 > 0 && m_ammo1 < 0)
    {
        m_ammo1 = 0;
    }
    if (m_ammo1 > m_maxAmmo1)
    {
        m_ammo1 = m_maxAmmo1;
    }

    if (m_maxAmmo1 > 0 && static_cast<float>(m_ammo1) / static_cast<float>(m_maxAmmo1) <= Settings::getSupplyWarning())
    {
        if (m_weapon1ID.isEmpty())
        {
            loadIcon("material", GameMap::getImageSize() / 2, 0);
        }
        else
        {
            loadIcon("ammo", GameMap::getImageSize() / 2, 0);
        }
    }
    else
    {
        unloadIcon("ammo");
        unloadIcon("material");
    }
}

bool Unit::hasAmmo1() const
{
    if ((m_maxAmmo1 < 0) || (m_ammo1 > 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Unit::reduceAmmo1(qint32 value)
{
    if (m_ammo1 >= 0)
    {
        setAmmo1(m_ammo1 - value);
    }
}

float Unit::getHp() const
{
    return m_hp;
}

qint32 Unit::getHpRounded() const
{
    return GlobalUtils::roundUp(m_hp);
}

void Unit::setHp(const float &value)
{
    m_hp = value;
    if (m_hp > MAX_UNIT_HP)
    {
        m_hp = MAX_UNIT_HP;
    }

    if (m_pMap != nullptr)
    {
        updateIcons(m_pMap->getCurrentViewPlayer());
    }
}

void Unit::setVirtualHpValue(const float &value)
{
    m_virtualHp = value;
}


float Unit::getVirtualHpValue() const
{
    return m_virtualHp;
}

float Unit::getVirtualHp() const
{
    if (m_virtualHp > 0.0f)
    {
        return m_virtualHp;
    }
    return m_hp;
}

bool Unit::getHpHidden(Player* pPlayer)
{
    if (pPlayer != nullptr && pPlayer->isEnemy(m_pOwner))
    {
        CO* pCO = m_pOwner->getCO(0);
        if (pCO != nullptr)
        {
            if (pCO->getHpHidden(this, QPoint(Unit::getX(), Unit::getY())))
            {
                return true;
            }
        }
        pCO = m_pOwner->getCO(1);
        if (pCO != nullptr)
        {
            if (pCO->getHpHidden(this, QPoint(Unit::getX(), Unit::getY())))
            {
                return true;
            }
        }

    }
    return false;
}

bool Unit::getRankInfoHidden(Player* pPlayer)
{
    if (pPlayer != nullptr &&
        m_pOwner != nullptr &&
        pPlayer->isEnemy(m_pOwner))
    {
        CO* pCO = m_pOwner->getCO(0);
        if (pCO != nullptr)
        {
            if (pCO->getRankInfoHidden(this, QPoint(Unit::getX(), Unit::getY())))
            {
                return true;
            }
        }
        pCO = m_pOwner->getCO(1);
        if (pCO != nullptr)
        {
            if (pCO->getRankInfoHidden(this, QPoint(Unit::getX(), Unit::getY())))
            {
                return true;
            }
        }

    }
    return false;
}

bool Unit::getPerfectHpView(Player* pPlayer)
{
    if (pPlayer != nullptr)
    {
        CO* pCO = pPlayer->getCO(0);
        if (pCO != nullptr)
        {
            if (pCO->getPerfectHpView(this, QPoint(Unit::getX(), Unit::getY())))
            {
                return true;
            }
        }
        pCO = pPlayer->getCO(1);
        if (pCO != nullptr)
        {
            if (pCO->getPerfectHpView(this, QPoint(Unit::getX(), Unit::getY())))
            {
                return true;
            }
        }
    }
    return false;
}

void Unit::updateIcons(Player* pPlayer)
{
    qint32 hpValue = GlobalUtils::roundUp(m_hp);
    // unload the icons
    unloadIcon("1");
    unloadIcon("2");
    unloadIcon("3");
    unloadIcon("4");
    unloadIcon("5");
    unloadIcon("6");
    unloadIcon("7");
    unloadIcon("8");
    unloadIcon("9");
    unloadIcon("hp+hidden");
    unloadIcon("transport");
    unloadIcon("transport+hidden");
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "unloadIcons";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction("UNITRANKINGSYSTEM", function1, args);
    if (!getHpHidden(pPlayer))
    {
        if ((hpValue < Unit::MAX_UNIT_HP) && (hpValue > 0))
        {
            loadIcon(QString::number(hpValue), 0, GameMap::getImageSize() / 2);
        }
    }
    else
    {
        loadIcon("hp+hidden", 0, GameMap::getImageSize() / 2);
    }
    if (m_pTerrain != nullptr)
    {
        if (getTransportHidden(pPlayer))
        {
            loadIcon("transport+hidden", GameMap::getImageSize() / 2, GameMap::getImageSize() / 2);
        }
        else if (getLoadedUnitCount() > 0)
        {
            loadIcon("transport", GameMap::getImageSize() / 2, GameMap::getImageSize() / 2);
        }
        updateStealthIcon();
    }
    updateRankInfoVisibility(pPlayer);
}

void Unit::updateRankInfoVisibility(Player* pPlayer)
{
    if (getRankInfoHidden(pPlayer))
    {
        if (m_CORange.get() != nullptr)
        {
            m_CORange->setVisible(false);
        }
        loadIcon("rank+hidden", GameMap::getImageSize() / 2, GameMap::getImageSize() / 2);
    }
    else
    {
        loadIcon(getUnitRangIcon(), GameMap::getImageSize() / 2, GameMap::getImageSize() / 2);
        if (m_CORange.get() != nullptr)
        {
            m_CORange->setVisible(getVisible());
        }
    }
}

bool Unit::getTransportHidden(Player* pPlayer)
{

    if (pPlayer != nullptr)
    {
        if ((m_pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off) &&
            (m_pMap->getGameRules()->getFogMode() != GameEnums::Fog_OfMist) &&
            (pPlayer->isEnemy(m_pOwner)) && getLoadingPlace() > 0 &&
            !isStatusStealthed())
        {
            return true;
        }
    }
    return false;
}

QString Unit::getMovementType() const
{
    return m_MovementType;
}

qint32 Unit::getLoadingPlace()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    qint32 bonus = m_pOwner->getCoBonus(getPosition(), this, "getBonusLoadingPlace");

    QString function1 = "getLoadingPlace";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_UnitID, function1, args);
    if (ret.isNumber())
    {
        return ret.toInt() + bonus;
    }
    else
    {
        return bonus;
    }
}

QString Unit::getUnitID()
{
    return m_UnitID;
}

QString Unit::getUnitDamageID()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getUnitDamageID";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_UnitID, function1, args);
    if (ret.isString())
    {
        QString retStr = ret.toString();
        if (retStr.isEmpty())
        {
            return m_UnitID;
        }
        return retStr;
    }
    return m_UnitID;
}

float Unit::getUnitDamage(const QString & weaponID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getUnitDamage";
    QJSValueList args({weaponID,
                      pInterpreter->newQObject(this),
                      pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_UnitID, function1, args);
    if (ret.isNumber())
    {
        return static_cast<float>(ret.toNumber());
    }
    return -1;
}

qint32 Unit::getX() const
{
    if (m_pTerrain != nullptr)
    {
        return m_pTerrain->Terrain::getX();
    }
    else
    {
        return m_virtuellX;
    }
}

qint32 Unit::getY() const
{
    if (m_pTerrain != nullptr)
    {
        return m_pTerrain->Terrain::getY();
    }
    else
    {
        return m_virtuellY;
    }
}

void Unit::refill(bool noMaterial, float fuelAmount, float ammo1Amount, float ammo2Amount)
{
    setFuel(m_fuel + m_maxFuel * fuelAmount);
    if (!(noMaterial && m_weapon1ID.isEmpty()))
    {
        setAmmo1(m_ammo1 + m_maxAmmo1 * ammo1Amount);
    }
    if (!(noMaterial && m_weapon2ID.isEmpty()))
    {
        setAmmo2(m_ammo2 + m_maxAmmo2 * ammo2Amount);
    }
}

void Unit::setHasMoved(bool value)
{
    m_Moved = value;
    // change unit color
    if (m_Moved)
    {
        for(auto & sprite : m_pUnitWaitSprites)
        {
            sprite->setVisible(true);
        }
    }
    else
    {
        for(auto & sprite : m_pUnitWaitSprites)
        {
            sprite->setVisible(false);
        }
    }
}

bool Unit::getHasMoved() const
{
    return m_Moved;
}

qint32 Unit::getTerrainDefenseModifier(QPoint position)
{
    qint32 modifier = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        modifier += pCO->getTerrainDefenseModifier(this, position);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        modifier += pCO->getTerrainDefenseModifier(this, position);
    }
    return modifier;
}

bool Unit::getFirstStrike(QPoint position, Unit* pAttacker, bool isDefender)
{
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        if (pCO->getFirstStrike(this, position, pAttacker, isDefender))
        {
            return true;
        }
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        if (pCO->getFirstStrike(this, position, pAttacker, isDefender))
        {
            return true;
        }
    }

    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getFirstStrike";
    QJSValueList args({pInterpreter->newQObject(this),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(pAttacker),
                       isDefender,
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_UnitID, function1, args);
    if (erg.isBool() && erg.toBool() == true)
    {
        return true;
    }
    return false;
}

qint32 Unit::getBonusMovementpoints(QPoint position)
{
    qint32 movementModifier = 0;
    movementModifier += getBonus(m_MovementBonus);
    movementModifier += m_pOwner->getBonusMovementpoints(this, position);
    return movementModifier;
}

qint32 Unit::getMovementpoints(QPoint position)
{
    qint32 points = m_baseMovementPoints + getBonusMovementpoints(position);
    if (points < 0)
    {
        points = 0;
    }
    if (m_fuel < points)
    {
        return m_fuel;
    }
    return points;
}

QStringList Unit::getBaseActionList()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getActions";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_UnitID, function1, args);
    QStringList actionList;
    if (ret.isString())
    {
        actionList = ret.toString().split(",");
    }
    else
    {
        actionList = ret.toVariant().toStringList();
    }
    return actionList;
}

QStringList Unit::getActionList()
{
    QStringList actionList = getBaseActionList();
    CO* pCO = m_pOwner->getCO(0);
    QStringList actionModifierList;
    if (pCO != nullptr)
    {
        actionModifierList.append(pCO->getActionModifierList(this));
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        actionModifierList.append(pCO->getActionModifierList(this));
    }
    for (qint32 i = 0; i < actionModifierList.size(); i++)
    {
        QString action = actionModifierList[i];
        if (action.startsWith("-"))
        {
            qint32 index = actionList.indexOf(action.replace("-", ""));
            if (index >= 0)
            {
                actionList.removeAt(index);
            }
        }
        else
        {
            actionList.append(action);
        }
    }
    return actionList;
}

bool Unit::hasAction(const QString & action)
{
    return getActionList().contains(action);
}

qint32 Unit::getMovementFuelCostModifier(qint32 fuelCost)
{

    qint32 ret = 0;
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = m_pMap->getPlayer(i);
        if (pPlayer != nullptr &&
            ((pPlayer->isEnemy(m_pOwner) && !pPlayer->getIsDefeated()) ||
             m_pOwner == pPlayer))
        {
            CO* pCO = pPlayer->getCO(0);
            if (pCO != nullptr)
            {
                ret += pCO->getMovementFuelCostModifier(this, fuelCost);
            }
            pCO = pPlayer->getCO(1);
            if (pCO != nullptr)
            {
                ret += pCO->getMovementFuelCostModifier(this, fuelCost);
            }
        }
    }
    if (!getWeatherImmune())
    {
        ret += m_pMap->getGameRules()->getCurrentWeather()->getMovementFuelCostModifier(this, fuelCost);
    }
    return ret;
}

void Unit::moveUnitAction(GameAction* pAction)
{
    // reduce fuel
    qint32 fuelCost = pAction->getCosts();
    fuelCost += getMovementFuelCostModifier(fuelCost);
    if (fuelCost < 0)
    {
        fuelCost = 0;
    }
    qint32 value = m_fuel - fuelCost;
    if (value < 0)
    {
        value = 0;
    }
    setFuel(value);
    auto path = pAction->getMovePath();
    moveUnit(path);
}

void Unit::moveUnit(QVector<QPoint> & movePath)
{    
    if (movePath.size() < 1)
    {
        movePath.append(QPoint(Unit::getX(), Unit::getY()));
    }
    // update vision based on the movepath of the unit
    for (qint32 i = 0; i < movePath.size(); i++)
    {
        qint32 moveCost = 1;
        if (i < movePath.size() - 1)
        {
            moveCost = getMovementCosts(movePath[i].x(), movePath[i].y(),
                                        movePath[i + 1].x(), movePath[i + 1].y());
        }
        if (moveCost > 0)
        {
            auto fields = getVisionFields(movePath[i]);
            for (auto & field : qAsConst(fields))
            {
                m_pOwner->addVisionField(field.x(), field.y(), 1, field.z());
            }
        }
    }
    if (movePath.size() > 1)
    {
        moveUnitToField(movePath[0].x(), movePath[0].y());
    }
}

QVector<QVector3D> Unit::getVisionFields(QPoint position)
{
    QVector<QVector3D> visionFields;

    bool visionBlock = m_pMap->getGameRules()->getVisionBlock();
    spQmlVectorPoint pCircle;
    qint32 visionRange = getVision(position);
    Terrain* pTerrain = m_pMap->getTerrain(position.x(), position.y());
    if (visionBlock)
    {
        pCircle = m_pMap->getVisionCircle(position.x(), position.y(), 0, visionRange,  getVisionHigh() + pTerrain->getTotalVisionHigh());
    }
    else
    {
        pCircle = GlobalUtils::getCircle(0, visionRange);
    }
    for (qint32 i2 = 0; i2 < pCircle->size(); i2++)
    {
        QPoint circleField = pCircle->at(i2);
        QPoint field = circleField + QPoint(position.x(), position.y());
        if (m_pMap->onMap(field.x(), field.y()))
        {
            if (qAbs(circleField.x()) + qAbs(circleField.y()) <= 1)
            {
                visionFields.append(QVector3D(field.x(), field.y(), true));
            }
            else
            {
                Terrain* pTerrain = m_pMap->getTerrain(field.x(), field.y());
                Unit* pUnit = pTerrain->getUnit();
                bool visionHide = pTerrain->getVisionHide(m_pOwner);
                if ((!visionHide) ||
                    ((pUnit != nullptr) && visionHide &&
                     !pUnit->useTerrainHide() && !pUnit->isStatusStealthed()))
                {
                    visionFields.append(QVector3D(field.x(), field.y(), false));
                }
            }
        }
    }
    return visionFields;
}

void Unit::moveUnitToField(qint32 x, qint32 y)
{
    
    // reset capture points when moving  a unit
    setCapturePoints(0);


    spUnit pUnit = m_pTerrain->getSpUnit();
    // teleport unit to target position
    m_pMap->getTerrain(x, y)->setUnit(pUnit);
    showRanges();
    pUnit = nullptr;
    
}

void Unit::removeUnit(bool killed)
{    
    if (killed)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "onDeath";
        QJSValueList args({pInterpreter->newQObject(this),
                           pInterpreter->newQObject(m_pMap)});
        pInterpreter->doFunction(m_UnitID, function1, args);
        m_pOwner->onUnitDeath(this);
        if (m_UnitRank == GameEnums::UnitRank_CO0)
        {
            CO* pCO = m_pOwner->getCO(0);
            if (pCO != nullptr &&
                (pCO->getCOUnit() == this))
            {
                pCO->setCOUnit(nullptr);
            }
        }
        else if (m_UnitRank == GameEnums::UnitRank_CO1)
        {
            CO* pCO = m_pOwner->getCO(1);
            if (pCO != nullptr &&
                (pCO->getCOUnit() == this))
            {
                pCO->setCOUnit(nullptr);
            }
        }
        // remove transported units as well -> may kill a co unit this way.
        for (qint32 i = 0; i < m_TransportUnits.size(); i++)
        {
            m_TransportUnits[i]->removeUnit();
        }
    }
    createCORange(-1);
    if (m_pTerrain != nullptr)
    {
        spUnit pUnit;
        m_pTerrain->setUnit(pUnit);
    }
}

GameAnimation* Unit::killUnit()
{    
    GameAnimation* pRet = nullptr;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "createExplosionAnimation";
    QJSValueList args({Unit::getX(),
                       Unit::getY(),
                       pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_UnitID, function1, args);
    if (ret.isQObject())
    {
        pRet = dynamic_cast<GameAnimation*>(ret.toQObject());
        pRet->writeDataInt32(getX());
        pRet->writeDataInt32(getY());
        pRet->setStartOfAnimationCall("UNIT", "onKilled");
    }
    // record destruction of this unit
    GameRecorder* pRecorder = m_pMap->getGameRecorder();
    if (pRecorder != nullptr &&
        m_pMap->getCurrentPlayer() != nullptr)
    {
        if (!m_pOwner->getIsDefeated())
        {
            m_pMap->getGameRecorder()->lostUnit(m_pOwner->getPlayerID(), m_UnitID, m_pMap->getCurrentPlayer()->getPlayerID());
        }
    }
    return pRet;
}

void Unit::increaseCapturePoints(QPoint position)
{
    m_capturePoints += getCaptureRate(position);
    // update icons
    setCapturePoints(m_capturePoints);
}

qint32 Unit::getCaptureRate(QPoint position)
{
    qint32 modifier = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        modifier += pCO->getCaptureBonus(this, position);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        modifier += pCO->getCaptureBonus(this, position);
    }
    return getHpRounded() + modifier;
}

void Unit::loadIcon(const QString & iconID, qint32 x, qint32 y, qint32 duration, qint32 player)
{
    if (iconID.isEmpty())
    {
        return;
    }
    if (duration >= 0 && player >= 0)
    {
        IconDuration info;
        info.icon = iconID;
        info.x = x;
        info.y = y;
        info.duration = duration;
        info.player = player;
        m_IconDurations.append(info);
    }

    for (qint32 i = 0; i < m_pIconSprites.size(); i++)
    {
        if (m_pIconSprites[i]->getResAnim()->getName() == iconID)
        {
            // already loaded icon
            return;
        }
    }
    
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pUnitSpriteManager->getResAnim(iconID, oxygine::ep_ignore_error);
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
        pSprite->setScale((GameMap::getImageSize() / 2) / pAnim->getWidth());
        pSprite->setPosition(x, y);
        pSprite->setPriority(static_cast<short>(Priorities::Icons));

        addChild(pSprite);
        m_pIconSprites.append(pSprite);

        updateIconTweens();
    }    
}

void Unit::unloadIcon(const QString & iconID)
{
    
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pUnitSpriteManager->getResAnim(iconID, oxygine::ep_ignore_error);
    if (pAnim != nullptr)
    {
        for (qint32 i = 0; i < m_pIconSprites.size(); i++)
        {
            if (m_pIconSprites[i]->getResAnim() == pAnim)
            {
                m_pIconSprites[i]->detach();
                m_pIconSprites.removeAt(i);
                break;
            }
        }
        updateIconTweens();
    }
    
}

void Unit::updateBonus(QVector<QPoint>& data)
{
    qint32 i = 0;
    while (i < data.size())
    {
        data[i].setY(data[i].y() - 1);
        if (data[i].y() <= 0)
        {
            data.removeAt(i);
        }
        else
        {
            i++;
        }
    }
}

qint32 Unit::getBonus(QVector<QPoint>& data)
{
    qint32 ret = 0;
    for (qint32 i = 0; i < data.size(); i++)
    {
        ret += data[i].x();
    }
    return ret;
}

QVector<QPoint> Unit::getAiMovePath() const
{
    return m_AiMovePath;
}

void Unit::setAiMovePath(const QVector<QPoint> &AiMovePath)
{
    m_AiMovePath = AiMovePath;
}

void Unit::addAiMovePathPoint(const QPoint &point)
{
    m_AiMovePath.append(point);
}

void Unit::setAiMovePathPoint(qint32 index, const QPoint &point)
{
    if (index < m_AiMovePath.size() && index >= 0)
    {
        m_AiMovePath[index] = point;
    }
}

void Unit::removeLastAiMovePathPoint()
{
    if (m_AiMovePath.size() > 0)
    {
        m_AiMovePath.removeLast();
    }
}

void Unit::removeFirstAiMovePathPoint()
{
    if (m_AiMovePath.size() > 0)
    {
        m_AiMovePath.removeFirst();
    }
}

bool Unit::onMap()
{
    return m_pTerrain != nullptr;
}

QString Unit::getCustomName() const
{
    return m_customName;
}

void Unit::setCustomName(const QString &customName)
{
    m_customName = customName;
}

qint32 Unit::getVisionHigh() const
{
    return m_VisionHigh;
}

void Unit::setVisionHigh(const qint32 &VisionHigh)
{
    m_VisionHigh = VisionHigh;
}

qint32 Unit::getTotalVisionHigh()
{
    qint32 high = m_VisionHigh;
    if (m_pTerrain != nullptr)
    {
        high += m_pTerrain->getTotalVisionHigh();
    }
    if (high < 0)
    {
        high = 0;
    }
    return high;
}

bool Unit::getCloaked() const
{
    return (m_cloaked.size() > 0);
}

void Unit::setCloaked(qint32 cloaked, qint32 byPlayer)
{
    if (byPlayer < 0)
    {
        byPlayer = m_pOwner->getPlayerID();
    }
    bool found = false;
    for (auto & entry : m_cloaked)
    {
        if (entry.y() == byPlayer)
        {
            found = true;
            if (entry.x() < cloaked)
            {
                entry.setX(cloaked);
            }
        }
    }
    if (!found)
    {
        m_cloaked.append(QPoint(cloaked, byPlayer));
    }
    updateStealthIcon();
}

void Unit::removeCloaked(qint32 byPlayer)
{
    if (byPlayer < 0)
    {
        byPlayer = m_pOwner->getPlayerID();
    }
    bool found = false;
    for (qint32 i = 0; i < m_cloaked.size(); ++i)
    {
        if (m_cloaked[i].y() == byPlayer)
        {
            m_cloaked.removeAt(i);
            break;
        }
    }
    updateStealthIcon();
}

void Unit::updateIconTweens()
{
    
    for (qint32 i = 0; i < m_pIconSprites.size(); i++)
    {
        m_pIconSprites[i]->removeTweens();
    }
    for (qint32 i = 0; i < 4; i++)
    {
        // calculate positions of the icons
        qint32 x = 0;
        qint32 y = 0;
        switch (i)
        {
            case 0:
            {
                x = 0;
                y = 0;
                break;
            }
            case 1:
            {
                x = GameMap::getImageSize() / 2;
                y = 0;
                break;
            }
            case 2:
            {
                x = 0;
                y = GameMap::getImageSize() / 2;
                break;
            }
            case 3:
            {
                x = GameMap::getImageSize() / 2;
                y = GameMap::getImageSize() / 2;
                break;
            }
        }
        // check the amount of icons at this position
        qint32 count = 0;
        for (qint32 i2 = 0; i2 < m_pIconSprites.size(); i2++)
        {
            if ((m_pIconSprites[i2]->getPosition().x == x) &&
                (m_pIconSprites[i2]->getPosition().y == y))
            {
                count++;
            }
        }
        // create the toggle visibility sprites
        if (count > 1)
        {
            qint32 step = 0;
            for (qint32 i2 = 0; i2 < m_pIconSprites.size(); i2++)
            {
                if ((m_pIconSprites[i2]->getPosition().x == x) &&
                    (m_pIconSprites[i2]->getPosition().y == y))
                {
                    qint32 visibileTime = 500;
                    float startTime = static_cast<float>(step) / static_cast<float>(count);
                    float endTime = static_cast<float>(step) / static_cast<float>(count) + 1.0f / static_cast<float>(count);
                    oxygine::spTween tween = oxygine::createTween(TweenToggleVisibility(startTime, endTime), oxygine::timeMS(visibileTime * count), -1);
                    m_pIconSprites[i2]->addTween(tween);
                    step++;
                }
            }
        }
        else
        {
            for (qint32 i2 = 0; i2 < m_pIconSprites.size(); i2++)
            {
                if ((m_pIconSprites[i2]->getPosition().x == x) &&
                    (m_pIconSprites[i2]->getPosition().y == y))
                {
                    m_pIconSprites[i2]->setVisible(true);
                }
            }
        }
    }
    
}

void Unit::setMovementType(const QString &movementType)
{
    m_MovementType = movementType;
}

QVector<QPoint> Unit::getMultiTurnPath() const
{
    return m_MultiTurnPath;
}

void Unit::setMultiTurnPath(const QVector<QPoint> &MultiTurnPath)
{
    m_MultiTurnPath = MultiTurnPath;
}

bool Unit::hasWeapons()
{
    if (!m_weapon1ID.isEmpty() || !m_weapon2ID.isEmpty())
    {
        return true;
    }
    return false;
}

qint32 Unit::getUniqueID() const
{
    return m_UniqueID;
}

GameEnums::GameAi Unit::getAiMode() const
{
    return m_AiMode;
}

void Unit::modifyUnit(qint32 hpChange, qint32 ammo1Change, qint32 ammo2Change, qint32 fuelChange)
{
    setHp(getHp() + hpChange);
    if (m_hp <= 0.0f)
    {
        setHp(0.0001f);
    }
    setAmmo1(getAmmo1() + ammo1Change);
    setAmmo2(getAmmo2() + ammo2Change);
    setFuel(getFuel() + fuelChange);
}

void Unit::setAiMode(const GameEnums::GameAi &AiMode)
{
    m_AiMode = AiMode;
    unloadIcon("defensive");
    unloadIcon("hold");
    unloadIcon("normal");
    unloadIcon("offensive");
    unloadIcon("patrol");
    unloadIcon("patrol_loop");
    if (dynamic_cast<EditorMenue*>(BaseGamemenu::getInstance()) != nullptr)
    {
        switch (m_AiMode)
        {
            case GameEnums::GameAi_Hold:
            {
                loadIcon("hold", 0, 0);
                break;
            }
            case GameEnums::GameAi_Offensive:
            {
                loadIcon("offensive", 0, 0);
                break;
            }
            case GameEnums::GameAi_Defensive:
            {
                loadIcon("defensive", 0, 0);
                break;
            }
            case GameEnums::GameAi_Normal:
            {
                loadIcon("normal", 0, 0);
                break;
            }
            case GameEnums::GameAi_Patrol:
            {
                loadIcon("patrol", 0, 0);
                break;
            }
            case GameEnums::GameAi_PatrolLoop:
            {
                loadIcon("patrol_loop", 0, 0);
                break;
            }
        }
    }
}

bool Unit::getIgnoreUnitCollision() const
{
    return m_IgnoreUnitCollision;
}

void Unit::setIgnoreUnitCollision(bool IgnoreUnitCollision)
{
    m_IgnoreUnitCollision = IgnoreUnitCollision;
}

bool Unit::isStatusStealthed() const
{
    return (m_Hidden || (m_cloaked.size() > 0));
}

bool Unit::isStatusStealthedAndInvisible(Player* pPlayer, bool & terrainHide)
{
    terrainHide = hasTerrainHide(pPlayer);
    if ((isStatusStealthed() || terrainHide) &&
        isStealthed(pPlayer))
    {
        return true;
    }
    return false;
}

bool Unit::getHidden() const
{
    return m_Hidden;
}

void Unit::setHidden(bool Hidden)
{
    m_Hidden = Hidden;
    updateStealthIcon();
}

void Unit::updateStealthIcon()
{
    if (isStatusStealthed())
    {
        loadIcon("dive", GameMap::getImageSize() / 2, GameMap::getImageSize() / 2);
    }
    else
    {
        unloadIcon("dive");
    }
}

bool Unit::hasTerrainHide(Player* pPlayer)
{
    qint32 x = Unit::getX();
    qint32 y = Unit::getY();
    bool visibleField = pPlayer->getFieldVisible(x, y);

    return (m_pTerrain->getVisionHide(pPlayer) && useTerrainHide() && !visibleField &&
            m_pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off);
}

bool Unit::isStealthed(Player* pPlayer, bool ignoreOutOfVisionRange, qint32 testX, qint32 testY)
{
    if (pPlayer != nullptr &&
        pPlayer->checkAlliance(m_pOwner) == GameEnums::Alliance_Enemy)
    {

        qint32 x = Unit::getX();
        qint32 y = Unit::getY();
        if (m_pMap->onMap(testX, testY))
        {
            x = testX;
            y = testY;
        }
        if (!m_pMap->onMap(x, y))
        {
            return false;
        }
        // can we see the unit?
        bool visibleField = pPlayer->getFieldVisible(x, y);
        bool directView = pPlayer->getFieldDirectVisible(x, y);
        if (directView || ignoreOutOfVisionRange)
        {
            return false;
        }
        if (!visibleField)
        {
            return true;
        }
        // a unit can be stealth by itself or by the terrain it's on.
        if (isStatusStealthed() ||
            hasTerrainHide(pPlayer))
        {
            spQmlVectorPoint pPoints = spQmlVectorPoint(GlobalUtils::getCircle(1, 1));
            for (qint32 i = 0; i < pPoints->size(); i++)
            {
                QPoint point = pPoints->at(i);
                if (m_pMap->onMap(point.x() + x, point.y() + y))
                {
                    Unit* pVisionUnit = m_pMap->getTerrain(x + point.x(), y + point.y())->getUnit();
                    if (pVisionUnit != nullptr)
                    {
                        if (pPlayer->checkAlliance(pVisionUnit->getOwner()) == GameEnums::Alliance_Friend)
                        {
                            return false;
                        }
                    }
                }
            }
            return true;
        }
    }
    return false;
}

float Unit::getBaseDamage(Unit* pEnemyUnit)
{
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    float dmg = -1.0f;
    if (!getWeapon1ID().isEmpty())
    {
        dmg = pWeaponManager->getBaseDamage(getWeapon1ID(), pEnemyUnit);
    }
    if (!getWeapon2ID().isEmpty())
    {
        float dmg2 = pWeaponManager->getBaseDamage(getWeapon2ID(), pEnemyUnit);
        if (dmg2 > dmg)
        {
            dmg = dmg2;
        }
    }
    return dmg;
}

bool Unit::getShowInEditor(QString unitId)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getShowInEditor";

    QJSValue ret = pInterpreter->doFunction(unitId, function1);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    return true;
}

void Unit::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_UnitID;
    pStream << m_hp;
    pStream << m_ammo1;
    pStream << m_ammo2;
    pStream << m_fuel;
    pStream << static_cast<qint32>(m_UnitRank);
    pStream << m_pOwner->getPlayerID();
    pStream << m_Moved;
    qint32 units = static_cast<qint32>(m_TransportUnits.size());
    pStream << units;
    for (qint32 i = 0; i < units; i++)
    {
        m_TransportUnits[i]->serializeObject(pStream);
    }
    pStream << m_capturePoints;
    pStream << m_Hidden;
    m_Variables.serializeObject(pStream);
    pStream << m_IgnoreUnitCollision;
    pStream << static_cast<qint32>(m_AiMode);
    pStream << m_UniqueID;
    pStream << static_cast<quint8>(m_ModdingFlags);

    qint32 size = m_MultiTurnPath.size();
    pStream << size;
    for (qint32 i = 0; i < size; i++)
    {
        pStream << m_MultiTurnPath[i];
    }
    size = m_OffensiveBonus.size();
    pStream << size;
    for (qint32 i = 0; i < size; i++)
    {
        pStream << m_OffensiveBonus[i];
    }
    size = m_DefensiveBonus.size();
    pStream << size;
    for (qint32 i = 0; i < size; i++)
    {
        pStream << m_DefensiveBonus[i];
    }
    size = m_VisionBonus.size();
    pStream << size;
    for (qint32 i = 0; i < size; i++)
    {
        pStream << m_VisionBonus[i];
    }
    size = m_MovementBonus.size();
    pStream << size;
    for (qint32 i = 0; i < size; i++)
    {
        pStream << m_MovementBonus[i];
    }
    size = m_FirerangeBonus.size();
    pStream << size;
    for (qint32 i = 0; i < size; i++)
    {
        pStream << m_FirerangeBonus[i];
    }
    size = m_cloaked.size();
    pStream << size;
    for (qint32 i = 0; i < size; i++)
    {
        pStream << m_cloaked[i];
    }
    pStream << m_VisionHigh;
    pStream << m_customName;
    size = m_AiMovePath.size();
    pStream << size;
    for (qint32 i = 0; i < size; i++)
    {
        pStream << m_AiMovePath[i];
    }
    size = m_IconDurations.size();
    pStream << size;
    for (qint32 i = 0; i < size; i++)
    {
        const IconDuration & iconInfo = m_IconDurations[i];
        pStream << iconInfo.icon;
        pStream << iconInfo.x;
        pStream << iconInfo.y;
        pStream << iconInfo.duration;
        pStream << iconInfo.player;
    }

    pStream << m_weapon1ID;
    pStream << m_weapon2ID;
    pStream << m_maxAmmo1;
    pStream << m_maxAmmo2;
    pStream << m_vision;
    pStream << m_minRange;
    pStream << m_maxRange;
    pStream << m_maxFuel;
    pStream << m_baseMovementPoints;
    size = m_customRangeInfo.size();
    pStream << size;
    for (qint32 i = 0; i < size; i++)
    {
        pStream << m_customRangeInfo[i].id;
        pStream << m_customRangeInfo[i].range;
        pStream << m_customRangeInfo[i].color.rgba();
    }
    pStream << m_cursorInfoRange;
}

void Unit::deserializeObject(QDataStream& pStream)
{
    deserializer(pStream, false);
}

void Unit::deserializer(QDataStream& pStream, bool fast)
{

    bool savegame = false;
    if (m_pMap != nullptr)
    {
        savegame = m_pMap->getSavegame();
    }

    qint32 version = 0;
    pStream >> version;
    if (version > 10)
    {
        pStream >> m_UnitID;
    }
    else
    {
        char* id;
        pStream >> id;
        m_UnitID = id;
    }
    qint32 bufAmmo1 = 0;
    qint32 bufAmmo2 = 0;
    qint32 bufFuel = 0;
    pStream >> m_hp;
    pStream >> bufAmmo1;
    pStream >> bufAmmo2;
    pStream >> bufFuel;
    qint32 value = 0;
    pStream >> value;
    if (!fast)
    {
        if (version > 13)
        {
            setUnitRank(value);
        }
        else
        {
            // mapping for older versions
            if (value == 4)
            {
                setUnitRank(GameEnums::UnitRank_CO0);
            }
            else if (value == 5)
            {
                setUnitRank(GameEnums::UnitRank_CO1);
            }
            else
            {
                setUnitRank(value);
            }
        }
    }
    quint32 playerID = 0;
    pStream >> playerID;
    m_pOwner = m_pMap->getPlayer(playerID);
    if (!fast)
    {
        initUnit();
    }
    setHp(m_hp);
    if (version > 1)
    {
        pStream >> m_Moved;
        setHasMoved(m_Moved);
        qint32 units;
        pStream >> units;
        for (qint32 i = 0; i < units; i++)
        {
            m_TransportUnits.append(spUnit::create(m_pMap));
            m_TransportUnits[m_TransportUnits.size() - 1]->deserializer(pStream, fast);
            if (!m_TransportUnits[m_TransportUnits.size() - 1]->isValid())
            {
                m_TransportUnits.removeLast();
            }
        }
    }
    if (version > 2)
    {
        pStream >> m_capturePoints;
        if (!fast)
        {
            setCapturePoints(m_capturePoints);
        }
    }
    if (version > 3)
    {
        pStream >> m_Hidden;
        if (!fast)
        {
            setHidden(m_Hidden);
        }
    }
    if (version > 4)
    {
        m_Variables.deserializeObject(pStream);
    }
    if (version > 5)
    {
        if (savegame)
        {
            pStream >> m_IgnoreUnitCollision;
        }
        else
        {
            bool dummy;
            pStream >> dummy;
        }
    }
    if (version > 6)
    {
        pStream >> value;
        if (!fast)
        {
            setAiMode(static_cast<GameEnums::GameAi>(value));
        }
    }
    else if (!fast)
    {
        setAiMode(GameEnums::GameAi_Normal);
    }
    if (version > 7)
    {
        pStream >> m_UniqueID;
    }
    if (m_UniqueID == 0)
    {
        m_UniqueID = m_pMap->getUniqueIdCounter();
    }
    if (version > 8)
    {
        quint8 value = 0;
        pStream >> value;
        m_ModdingFlags = static_cast<ModdingFlags>(value);
    }
    m_MultiTurnPath.clear();
    m_OffensiveBonus.clear();
    m_DefensiveBonus.clear();
    m_VisionBonus.clear();
    m_MovementBonus.clear();
    m_FirerangeBonus.clear();
    if (version > 9)
    {
        qint32 size = 0;
        pStream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            QPoint point;
            pStream >> point;
            m_MultiTurnPath.append(point);
        }
        if (version > 11)
        {
            pStream >> size;
            for (qint32 i = 0; i < size; i++)
            {
                QPoint data;
                pStream >> data;
                m_OffensiveBonus.append(data);
            }
            pStream >> size;
            for (qint32 i = 0; i < size; i++)
            {
                QPoint data;
                pStream >> data;
                m_DefensiveBonus.append(data);
            }
            pStream >> size;
            for (qint32 i = 0; i < size; i++)
            {
                QPoint data;
                pStream >> data;
                m_VisionBonus.append(data);
            }
            pStream >> size;
            for (qint32 i = 0; i < size; i++)
            {
                QPoint data;
                pStream >> data;
                m_MovementBonus.append(data);
            }
            pStream >> size;
            for (qint32 i = 0; i < size; i++)
            {
                QPoint data;
                pStream >> data;
                m_FirerangeBonus.append(data);
            }
        }
    }
    if (version > 12)
    {
        if (version > 18)
        {
            qint32 size = 0;
            pStream >> size;
            for (qint32 i = 0; i < size; i++)
            {
                QPoint point;
                pStream >> point;
                m_cloaked.append(point);
            }
        }
        else
        {
            qint32 duration = 0;
            pStream >> duration;
            if (duration > 0)
            {
                m_cloaked.append(QPoint(duration, m_pOwner->getPlayerID()));
            }
        }
    }
    if (version > 13)
    {
        pStream >> m_VisionHigh;
    }
    if (version > 14)
    {
        pStream >> m_customName;
    }
    m_AiMovePath.clear();
    if (version > 15)
    {
        qint32 size = 0;
        pStream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            QPoint point;
            pStream >> point;
            m_AiMovePath.append(point);
        }
    }
    if (version > 16)
    {
        qint32 size = 0;
        pStream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            IconDuration iconInfo;
            pStream >> iconInfo.icon;
            pStream >> iconInfo.x;
            pStream >> iconInfo.y;
            pStream >> iconInfo.duration;
            pStream >> iconInfo.player;
            if (!fast)
            {
                loadIcon(iconInfo.icon, iconInfo.x, iconInfo.y,
                         iconInfo.duration, iconInfo.player);
            }
        }
    }
    if (version > 17)
    {
        if (savegame)
        {
            pStream >> m_weapon1ID;
            pStream >> m_weapon2ID;
            pStream >> m_maxAmmo1;
            pStream >> m_maxAmmo2;
            pStream >> m_vision;
            pStream >> m_minRange;
            pStream >> m_maxRange;
            pStream >> m_maxFuel;
            pStream >> m_baseMovementPoints;
        }
        else
        {
            QString dummy;
            pStream >> dummy;
            pStream >> dummy;
            qint32 dummy2;
            pStream >> dummy2;
            pStream >> dummy2;
            pStream >> dummy2;
            pStream >> dummy2;
            pStream >> dummy2;
            pStream >> dummy2;
            pStream >> dummy2;
        }
    }
    setAmmo1(bufAmmo1);
    setAmmo2(bufAmmo2);
    setFuel(bufFuel);
    if (version > 19)
    {
        qint32 size;
        pStream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            CustomRangeInfo customRangeInfo;
            pStream >> customRangeInfo.id;
            pStream >> customRangeInfo.range;
            QRgb rgb;
            pStream >> rgb;
            customRangeInfo.color = rgb;
            m_customRangeInfo.append(customRangeInfo);
            updateRangeActor(customRangeInfo.pActor,
                             customRangeInfo.range,
                             customRangeInfo.id,
                             customRangeInfo.color);
        }
    }
    if (version > 20)
    {
        if (savegame)
        {
            pStream >> m_cursorInfoRange;
        }
        else
        {
            qint32 dummy2;
            pStream >> dummy2;
        }
    }
}

void Unit::showRanges()
{    
    if (m_UnitRank == GameEnums::UnitRank_CO0)
    {
        createCORange(m_pOwner->getCO(0)->getCORange());
    }
    else if (m_UnitRank == GameEnums::UnitRank_CO1)
    {
        createCORange(m_pOwner->getCO(1)->getCORange());
    }
    else
    {
        createCORange(-1);
    }
    updateCustomRangeActors();
}

void Unit::showCustomRange(const QString & id, qint32 range, QColor color)
{
    bool found = false;
    for (auto & customRangeInfo : m_customRangeInfo)
    {
        if (customRangeInfo.id == id)
        {
            customRangeInfo.range = range;
            customRangeInfo.color = color;
            updateRangeActor(customRangeInfo.pActor,
                             customRangeInfo.range,
                             customRangeInfo.id,
                             customRangeInfo.color);
            found = true;
            break;
        }
    }
    if (!found)
    {
        CustomRangeInfo customRangeInfo;
        customRangeInfo.range = range;
        customRangeInfo.id = id;
        customRangeInfo.color = color;
        m_customRangeInfo.append(customRangeInfo);
        updateRangeActor(customRangeInfo.pActor,
                         customRangeInfo.range,
                         customRangeInfo.id,
                         customRangeInfo.color);
    }
}

void Unit::removeCustomRange(const QString & id)
{
    for (qint32 i = 0; i < m_customRangeInfo.size(); ++i)
    {
        if (m_customRangeInfo[i].id == id)
        {
            m_customRangeInfo[i].pActor->removeChildren();
            m_customRangeInfo[i].pActor->detach();
            m_customRangeInfo.removeAt(i);
            break;
        }
    }
}

void Unit::createCORange(qint32 coRange)
{
    QColor color = m_pOwner->getColor();
    updateRangeActor(m_CORange, coRange, "co+range+marker", color);
}

void Unit::updateCustomRangeActors()
{
    for (auto & customRangeInfo : m_customRangeInfo)
    {
        updateRangeActor(customRangeInfo.pActor,
                         customRangeInfo.range,
                         customRangeInfo.id,
                         customRangeInfo.color);
    }
}

void Unit::transformUnit(const QString & unitID)
{
    for (auto & sprite : m_pUnitWaitSprites)
    {
        sprite->detach();
    }
    for (auto & sprite : m_pUnitSprites)
    {
        sprite->detach();
    }
    for (auto & sprite : m_pIconSprites)
    {
        sprite->detach();
    }
    while (m_customRangeInfo.size() > 0)
    {
        removeCustomRange(m_customRangeInfo[0].id);
    }
    m_pIconSprites.clear();
    m_pUnitWaitSprites.clear();
    m_pUnitSprites.clear();
    qint32 fuel = m_fuel;
    qint32 ammo1 = m_ammo1;
    qint32 ammo2 = m_ammo2;
    m_UnitID = unitID;
    initUnit();
    setFuel(fuel);
    setAmmo1(ammo1);
    setAmmo2(ammo2);
    updateSprites(false);
}

qint32 Unit::getCursorInfoRange() const
{
    return m_cursorInfoRange;
}

void Unit::setCursorInfoRange(qint32 newCursorInfoRange)
{
    m_cursorInfoRange = newCursorInfoRange;
}

void Unit::updateRangeActor(oxygine::spActor & pActor, qint32 range, QString resAnim, QColor color)
{
    if (pActor.get() == nullptr)
    {
        pActor = oxygine::spActor::create();
    }
    else
    {
        pActor->detach();
    }
    pActor->removeChildren();
    pActor->setPriority(static_cast<qint32>(Mainapp::ZOrder::CORange));

    if (m_pTerrain != nullptr && range >= 0 && m_pMap)
    {
        CreateOutline::addCursorRangeOutline(pActor, resAnim, range, color);
        pActor->setPosition(GameMap::getImageSize() * Unit::getX(), GameMap::getImageSize() * Unit::getY());
        m_pMap->addChild(pActor);
    }
}
