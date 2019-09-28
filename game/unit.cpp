#include "unit.h"

#include <QColor>

#include "game/player.h"

#include "resource_management/unitspritemanager.h"

#include "resource_management/gamemanager.h"

#include "resource_management/weaponmanager.h"

#include "resource_management/movementtablemanager.h"

#include "game/gamemap.h"

#include "coreengine/console.h"

#include "game/gameaction.h"

#include "game/co.h"

#include "game/building.h"

#include "menue/editormenue.h"

#include "coreengine/tweentogglevisibility.h"

#include "coreengine/tweenaddcolorall.h"

const float Unit::animationSpeed = 1.5f;

Unit::Unit()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    setHeight(GameMap::Imagesize);
    setWidth(GameMap::Imagesize);
}

Unit::Unit(QString unitID, Player* pOwner, bool aquireId)
    : QObject(),
      m_UnitID(unitID),
      m_pOwner(pOwner)
{
    setHeight(GameMap::Imagesize);
    setWidth(GameMap::Imagesize);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    if (!m_UnitID.isEmpty())
    {
        initUnit();
        setFuel(maxFuel);
        setAmmo1(maxAmmo1);
        setAmmo2(maxAmmo2);
        updateSprites();
        if (aquireId)
        {
            m_UniqueID = GameMap::getInstance()->getUniqueIdCounter();
        }
    }        
}

Unit::~Unit()
{
    if (m_pOwner != nullptr)
    {
        CO* pCO = m_pOwner->getCO(0);
        if (pCO != nullptr && pCO->getCOUnit() != nullptr)
        {
            if (pCO->getCOUnit() == this)
            {
                pCO->setCOUnit(nullptr);
            }
        }
        pCO = m_pOwner->getCO(1);
        if (pCO != nullptr && pCO->getCOUnit() != nullptr)
        {
            if (pCO->getCOUnit() == this)
            {
                pCO->setCOUnit(nullptr);
            }
        }
        if (m_CORange.get() != nullptr)
        {
            m_CORange->removeChildren();
        }
    }
}

QString Unit::getDescription()
{
    Mainapp* pApp = Mainapp::getInstance();
    QJSValueList args;
    // load sprite of the base terrain
    QString function = "getDescription";
    QJSValue ret = pApp->getInterpreter()->doFunction(m_UnitID, function, args);
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
}

void Unit::setOwner(Player* pOwner)
{
    // change ownership
    if (m_UnitRank <= GameEnums::UnitRank_CO0)
    {
        m_pOwner = pOwner;
        if (m_pOwner != nullptr)
        {
            // update sprites :)
            updateSprites();
        }
    }
}

void Unit::setTerrain(Terrain* pTerrain)
{
    m_pTerrain = pTerrain;
}

void Unit::addShineTween()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    removeShineTween();
    m_ShineTween = oxygine::createTween(TweenAddColorAll(oxygine::Color(50, 50, 50, 255)), 500, -1, true);
    addTween(m_ShineTween);
    pApp->continueThread();
}

void Unit::removeShineTween()
{
    if (m_ShineTween.get() != nullptr)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        m_ShineTween->remove();
        m_ShineTween = nullptr;
        oxygine::Color addColor(0, 0, 0, 255);
        setAddColor(addColor);
        oxygine::spVStyleActor child = static_cast<oxygine::VStyleActor*>(getFirstChild().get());
        while (child)
        {
            child->setAddColor(addColor);
            child = static_cast<oxygine::VStyleActor*>(child->getNextSibling().get());
        }
        pApp->continueThread();
    }
}

void Unit::loadSprite(QString spriteID, bool addPlayerColor)
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pUnitSpriteManager->getResAnim(spriteID.toStdString());
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();
        oxygine::spSprite pWaitSprite = new oxygine::Sprite();
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), static_cast<qint32>(pAnim->getTotalFrames() * GameMap::frameTime * animationSpeed), -1);
            pSprite->addTween(tween);

            oxygine::spTween tweenWait = oxygine::createTween(oxygine::TweenAnim(pAnim), static_cast<qint32>(pAnim->getTotalFrames() * GameMap::frameTime * animationSpeed), -1);
            pWaitSprite->addTween(tweenWait);
        }
        else
        {
            pSprite->setResAnim(pAnim);
            pWaitSprite->setResAnim(pAnim);
        }
        // repaint the unit?
        if (addPlayerColor)
        {
            QColor color = m_pOwner->getColor();
            oxygine::Sprite::TweenColor tweenColor(oxygine::Color(color.red(), color.green(), color.blue(), 255));
            oxygine::spTween tween = oxygine::createTween(tweenColor, 1);
            pSprite->addTween(tween);
        }
        pSprite->setScale(GameMap::Imagesize / pAnim->getWidth());
        pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::Imagesize) / 2, -(pSprite->getScaledHeight() - GameMap::Imagesize));
        this->addChild(pSprite);
        m_pUnitSprites.append(pSprite);

        oxygine::Sprite::TweenColor tweenColor(oxygine::Color(100, 100, 100, 170));
        oxygine::spTween tweenWait = oxygine::createTween(tweenColor, 1);
        pWaitSprite->addTween(tweenWait);
        pWaitSprite->setScale(GameMap::Imagesize / pAnim->getWidth());
        pWaitSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::Imagesize) / 2, -(pSprite->getScaledHeight() - GameMap::Imagesize));
        pWaitSprite->setPriority(static_cast<short>(Priorities::Waiting));
        this->addChild(pWaitSprite);
        pWaitSprite->setVisible(false);
        m_pUnitWaitSprites.append(pWaitSprite);
        if (!Mainapp::isEven(m_pOwner->getPlayerID()))
        {
            pSprite->setFlippedX(true);
            pWaitSprite->setFlippedX(true);
        }
    }
    else
    {
        Console::print("Unable to load unit sprite: " + spriteID, Console::eERROR);
    }
}

Player* Unit::getOwner()
{
    return m_pOwner;
}

QString Unit::getName()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getName";
    QJSValue ret = pApp->getInterpreter()->doFunction(m_UnitID, function1);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

GameEnums::UnitType Unit::getUnitType()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getUnitType";
    QJSValue ret = pApp->getInterpreter()->doFunction(m_UnitID, function1);
    if (ret.isNumber())
    {
        return static_cast<GameEnums::UnitType>(ret.toInt());
    }
    else
    {
        return GameEnums::UnitType_Ground;
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

void Unit::updateSprites()
{
    Mainapp* pApp = Mainapp::getInstance();
    for (qint32 i = 0; i < m_pUnitSprites.size(); i++)
    {
        this->removeChild(m_pUnitSprites[i]);
    }
    // call the js loader function to do the rest
    m_pUnitSprites.clear();
    QString function1 = "loadSprites";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);

    setHp(hp);
    setFuel(fuel);
    setAmmo1(ammo1);
    setAmmo2(ammo2);
    setUnitRank(m_UnitRank);

    if (m_UnitRank == GameEnums::UnitRank_CO0)
    {
        makeCOUnit(0);
    }
    else if (m_UnitRank == GameEnums::UnitRank_CO1)
    {
        makeCOUnit(1);
    }
    setHasMoved(m_Moved);
    for (qint32 i = 0; i < m_TransportUnits.size(); i++)
    {
        m_TransportUnits[i]->updateSprites();
    }
    CO* pCO1 = m_pOwner->getCO(0);
    CO* pCO2 = m_pOwner->getCO(1);
    if ((pCO1 != nullptr && pCO1->getPowerMode() > GameEnums::PowerMode_Off) ||
        (pCO2 != nullptr && pCO2->getPowerMode() > GameEnums::PowerMode_Off))
    {
        addShineTween();
    }
}

GameEnums::UnitRanks Unit::getUnitRank() const
{
    return m_UnitRank;
}

void Unit::setUnitRank(const GameEnums::UnitRanks &UnitRank)
{
    GameMap* pMap = GameMap::getInstance();
    if (pMap != nullptr)
    {
        if (pMap->getGameRules()->getRankingSystem() ||
            (UnitRank == GameEnums::UnitRank_CO0) ||
            (UnitRank == GameEnums::UnitRank_CO1))
        {
            m_UnitRank = UnitRank;
        }
        unloadIcon("lieutenant");
        unloadIcon("general");
        unloadIcon("veteran");
        unloadIcon("co0");
        unloadIcon("co1");
        switch (m_UnitRank)
        {
            case GameEnums::UnitRank_None:
            {
                break;
            }
            case GameEnums::UnitRank_Lieutenant:
            {
                loadIcon("lieutenant", GameMap::Imagesize / 2, GameMap::Imagesize / 2);
                break;
            }
            case GameEnums::UnitRank_General:
            {
                loadIcon("general", GameMap::Imagesize / 2, GameMap::Imagesize / 2);
                break;
            }
            case GameEnums::UnitRank_Veteran:
            {
                loadIcon("veteran", GameMap::Imagesize / 2, GameMap::Imagesize / 2);
                break;
            }
            case GameEnums::UnitRank_CO0:
            {
                loadIcon("co0", GameMap::Imagesize / 2, GameMap::Imagesize / 2);
                break;
            }
            case GameEnums::UnitRank_CO1:
            {
                loadIcon("co1", GameMap::Imagesize / 2, GameMap::Imagesize / 2);
                break;
            }
        }
    }
}

qint32 Unit::getVision(QPoint position)
{
    qint32 rangeModifier = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        rangeModifier += pCO->getVisionrangeModifier(this, position);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        rangeModifier += pCO->getVisionrangeModifier(this, position);
    }
    GameMap* pMap = GameMap::getInstance();
    rangeModifier += pMap->getGameRules()->getCurrentWeather()->getVisionrangeModifier();
    qint32 mapHeigth = pMap->getMapHeight();
    qint32 mapWidth = pMap->getMapWidth();
    for (qint32 x = 0; x < mapWidth; x++)
    {
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
            if ((pBuilding != nullptr) &&
                (pBuilding->getOwner() == m_pOwner))
            {
                rangeModifier += pBuilding->getVisionBonus();
            }
        }
    }
    rangeModifier += m_pTerrain->getBonusVision(this);
    qint32 points = vision + rangeModifier;

    if (points < 1)
    {
        points = 1;
    }
    return points;
}

void Unit::setVision(const qint32 &value)
{
    vision = value;
}

qint32 Unit::getBaseVision()
{
    return vision;
}

qint32 Unit::getMaxRange()
{
    return getMaxRangeAtPosition(QPoint(getX(), getY()));
}

qint32 Unit::getBonusMaxRange(QPoint position)
{
    qint32 rangeModifier = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        rangeModifier += pCO->getFirerangeModifier(this, position);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        rangeModifier += pCO->getFirerangeModifier(this, position);
    }

    GameMap* pMap = GameMap::getInstance();
    rangeModifier += pMap->getGameRules()->getCurrentWeather()->getFirerangeModifier();
    // add terrain modifiers
    if (pMap->onMap(position.x(), position.y()))
    {
        rangeModifier += pMap->getTerrain(position.x(), position.y())->getFirerangeModifier(this);
    }
    return rangeModifier;
}

qint32 Unit::getMaxRangeAtPosition(QPoint position)
{

    qint32 points = maxRange + getBonusMaxRange(position);

    if (points < minRange)
    {
        points = minRange;
    }
    return points;
}

void Unit::setMaxRange(const qint32 &value)
{
    maxRange = value;
}

qint32 Unit::getBaseMaxRange()
{
    return maxRange;
}

qint32 Unit::getMinRange() const
{
    return minRange;
}

void Unit::setMinRange(const qint32 &value)
{
    minRange = value;
}

qint32 Unit::getCosts() const
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getBaseCost";
    QJSValueList args1;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return -1;
    }
}

QString Unit::getTerrainAnimationBase()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getTerrainAnimationBase";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(m_pTerrain);
    args1 << obj2;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

QString Unit::getTerrainAnimationForeground()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getTerrainAnimationForeground";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(m_pTerrain);
    args1 << obj2;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

QString Unit::getTerrainAnimationBackground()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getTerrainAnimationBackground";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(m_pTerrain);
    args1 << obj2;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

bool Unit::canMoveOver(qint32 x, qint32 y)
{
    GameMap* pMap = GameMap::getInstance();
    if (MovementTableManager::getInstance()->getBaseMovementPoints(getMovementType(), pMap->getTerrain(x, y), this) > 0)
    {
        return true;
    }
    return  false;
}

qint32 Unit::getUnitValue()
{
    return static_cast<qint32>(getCosts() * hp / 10.0f);
}

bool Unit::canBeRepaired(QPoint position)
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = pMap->getPlayer(i);
        if (pPlayer != nullptr &&
            m_pOwner->isEnemy(pPlayer))
        {
            CO* pCO = pPlayer->getCO(0);
            if (pCO != nullptr)
            {
                if (!pCO->canBeRepaired(this, position))
                {
                    return false;
                }
            }
            pCO = pPlayer->getCO(1);
            if (pCO != nullptr)
            {
                if (!pCO->canBeRepaired(this, position))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool Unit::isAttackable(Unit* pDefender, bool ignoreOutOfVisionRange)
{
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    if (m_pOwner->getFieldVisible(pDefender->getX(), pDefender->getY()) || ignoreOutOfVisionRange)
    {
        if (!pDefender->isStealthed(m_pOwner, ignoreOutOfVisionRange))
        {
            if (!pDefender->getHidden() ||
                (pDefender->getHidden() && pDefender->getUnitType() == getUnitType()))
            {
                if (m_pOwner->isEnemyUnit(pDefender) == true)
                {
                    if (hasAmmo1() && !weapon1ID.isEmpty())
                    {
                        if (pWeaponManager->getBaseDamage(weapon1ID, pDefender) > 0)
                        {
                            return true;
                        }
                    }
                    if (hasAmmo2() && !weapon2ID.isEmpty())
                    {
                        if (pWeaponManager->getBaseDamage(weapon2ID, pDefender) > 0)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

Terrain* Unit::getTerrain()
{
    return m_pTerrain;
}

bool Unit::canMoveAndFire(QPoint position)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "canMoveAndFire";
    QJSValueList args1;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
    if (erg.isBool() && erg.toBool())
    {
        return true;
    }
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        if (pCO->getCanMoveAndFire(this, position))
        {
            return true;
        }
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        if (pCO->getCanMoveAndFire(this, position))
        {
            return true;
        }
    }
    return false;
}

void Unit::loadUnit(Unit* pUnit)
{
    m_TransportUnits.append(pUnit->getTerrain()->getSpUnit());
    pUnit->removeUnit();
    updateIcons(GameMap::getInstance()->getCurrentViewPlayer());
}

Unit* Unit::spawnUnit(QString unitID)
{
    GameMap* pMap = GameMap::getInstance();
    if (pMap != nullptr)
    {
        qint32 unitLimit = pMap->getGameRules()->getUnitLimit();
        qint32 unitCount = m_pOwner->getUnitCount();
        if (unitLimit > 0 && unitCount >= unitLimit)
        {
            return nullptr;
        }
        spUnit pUnit = new Unit(unitID, m_pOwner, true);
        m_TransportUnits.append(pUnit);
        updateIcons(pMap->getCurrentViewPlayer());
        return pUnit.get();
    }
    return nullptr;
}

Unit* Unit::getLoadedUnit(qint32 index)
{
    if ((index >= 0) && (index < m_TransportUnits.size()))
    {
        return m_TransportUnits[index].get();
    }
    return nullptr;
}

void Unit::unloadUnit(Unit* pUnit, QPoint position)
{
    GameMap* pMap = GameMap::getInstance();
    if (pMap != nullptr)
    {
        for (qint32 i = 0; i < m_TransportUnits.size(); i++)
        {
            if (m_TransportUnits[i] == pUnit)
            {
                pMap->getTerrain(position.x(), position.y())->setUnit(m_TransportUnits[i]);
                m_TransportUnits.removeAt(i);
                break;
            }
        }
        updateIcons(pMap->getCurrentViewPlayer());
    }
}

qint32 Unit::getLoadedUnitCount()
{
    return m_TransportUnits.size();
}

QStringList  Unit::getTransportUnits()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getTransportUnits";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
    return erg.toVariant().toStringList();
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

qint32 Unit::getBonusOffensive(QPoint position, Unit* pDefender, QPoint defPosition, bool isDefender)
{
    qint32 bonus = 0;
    GameMap* pMap = GameMap::getInstance();
    CO* pCO0 = m_pOwner->getCO(0);
    if (pCO0 != nullptr)
    {
        bonus += pCO0->getOffensiveBonus(this, position, pDefender, defPosition, isDefender);
    }
    CO* pCO1 = m_pOwner->getCO(1);
    if (pCO1 != nullptr)
    {
        bonus += pCO1->getOffensiveBonus(this, position, pDefender, defPosition, isDefender);
    }

    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = pMap->getPlayer(i);
        if (pPlayer != nullptr &&
            m_pOwner->isEnemy(pPlayer))
        {
            pCO0 = pPlayer->getCO(0);
            if (pCO0 != nullptr)
            {
                bonus -= pCO0->getOffensiveReduction(this, position, pDefender, defPosition, isDefender);
            }
            pCO1 = pPlayer->getCO(1);
            if (pCO1 != nullptr)
            {
                bonus -= pCO1->getOffensiveReduction(this, position, pDefender, defPosition, isDefender);
            }
        }
    }

    pCO0 = m_pOwner->getCO(0);
    pCO1 = m_pOwner->getCO(1);
    if (pCO0 != nullptr && pCO1 != nullptr &&
        pCO0->getPowerMode() == GameEnums::PowerMode_Tagpower)
    {
        Mainapp* pApp = Mainapp::getInstance();
        QString function1 = "getTagpower";
        QJSValueList args1;
        QJSValue obj1 = pApp->getInterpreter()->newQObject(pCO0);
        args1 << obj1;
        QJSValue obj2 = pApp->getInterpreter()->newQObject(pCO1);
        args1 << obj2;
        QJSValue erg = pApp->getInterpreter()->doFunction("TAGPOWER", function1, args1);
        if (erg.isNumber())
        {
            bonus += erg.toNumber();
        }
    }


    qint32 mapHeigth = pMap->getMapHeight();
    qint32 mapWidth = pMap->getMapWidth();
    for (qint32 x = 0; x < mapWidth; x++)
    {
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
            if ((pBuilding != nullptr) && pBuilding->getOwner() == m_pOwner)
            {
                bonus += pBuilding->getOffensiveBonus();
            }
        }
    }
    bonus += pMap->getGameRules()->getCurrentWeather()->getOffensiveModifier();
    if (pMap->getGameRules()->getRankingSystem())
    {
        switch (m_UnitRank)
        {
            case GameEnums::UnitRank_None:
            {
                break;
            }
            case GameEnums::UnitRank_Lieutenant:
            {
                bonus += 5;
                break;
            }
            case GameEnums::UnitRank_General:
            {
                bonus += 10;
                break;
            }
            case GameEnums::UnitRank_Veteran:
            case GameEnums::UnitRank_CO0:
            case GameEnums::UnitRank_CO1:
            {
                bonus += 20;
                break;
            }
        }
    }
    return bonus;
}

qint32 Unit::getTerrainDefense()
{
    GameMap* pMap = GameMap::getInstance();
    if (useTerrainDefense() && m_pTerrain != nullptr)
    {
        return pMap->getTerrain(getX(), getY())->getDefense(this);
    }
    return 0;
}

float Unit::getDamageReduction(float damage, Unit* pAttacker, QPoint position, qint32 attackerBaseHp,
                          QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode)
{
    float bonus = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        bonus += pCO->getDamageReduction(damage, pAttacker, position, attackerBaseHp,
                                         this, defPosition, isDefender, luckMode);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        bonus += pCO->getDamageReduction(damage, pAttacker, position, attackerBaseHp,
                                         this, defPosition, isDefender, luckMode);
    }
    return bonus;
}

float Unit::getTrueDamage(float damage, QPoint position, qint32 attackerBaseHp,
                          Unit* pDefender, QPoint defPosition, bool isDefender)
{
    float bonus = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        bonus += pCO->getTrueDamage(damage, this, position, attackerBaseHp,
                                    pDefender, defPosition, isDefender);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        bonus += pCO->getTrueDamage(damage, this, position, attackerBaseHp,
                                    pDefender, defPosition, isDefender);
    }
    return bonus;
}

qint32 Unit::getBonusDefensive(QPoint position, Unit* pAttacker, QPoint atkPosition, bool isDefender)
{
    GameMap* pMap = GameMap::getInstance();
    qint32 bonus = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        bonus += pCO->getDeffensiveBonus(pAttacker, atkPosition, this, position, isDefender);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        bonus += pCO->getDeffensiveBonus(pAttacker, atkPosition, this, position, isDefender);
    }
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = pMap->getPlayer(i);
        if (pPlayer != nullptr &&
            m_pOwner->isEnemy(pPlayer))
        {
            pCO = pPlayer->getCO(0);
            if (pCO != nullptr)
            {
                bonus -= pCO->getDeffensiveReduction(pAttacker, atkPosition, this, position, isDefender);
            }
            pCO = pPlayer->getCO(1);
            if (pCO != nullptr)
            {
                bonus -= pCO->getDeffensiveReduction(pAttacker, atkPosition, this, position, isDefender);
            }
        }
    }
    qint32 mapHeigth = pMap->getMapHeight();
    qint32 mapWidth = pMap->getMapWidth();
    for (qint32 x = 0; x < mapWidth; x++)
    {
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
            if ((pBuilding != nullptr) && pBuilding->getOwner() == m_pOwner)
            {
                bonus += pBuilding->getDefensiveBonus();
            }
        }
    }
    if (useTerrainDefense())
    {
        bonus += getTerrainDefense() * 10;
    }
    bonus += pMap->getGameRules()->getCurrentWeather()->getDefensiveModifier();
    switch (m_UnitRank)
    {
        case GameEnums::UnitRank_None:
        {
            break;
        }
        case GameEnums::UnitRank_Lieutenant:
        {
            bonus += 0;
            break;
        }
        case GameEnums::UnitRank_General:
        {
            bonus += 0;
            break;
        }
        case GameEnums::UnitRank_Veteran:
        case GameEnums::UnitRank_CO0:
        case GameEnums::UnitRank_CO1:
        {
            bonus += 20;
            break;
        }
    }
    return bonus;
}

bool Unit::useTerrainDefense()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "useTerrainDefense";
    QJSValueList args1;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
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
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        bonus += pCO->getBonusMisfortune(this, position);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        bonus += pCO->getBonusMisfortune(this, position);
    }
    return bonus;
}

qint32 Unit::getUnitCosts()
{
    return m_pOwner->getCosts(m_UnitID);
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

void Unit::setUnitVisible(bool value)
{
    if (m_CORange.get() != nullptr)
    {
        m_CORange->setVisible(value);
    }
    setVisible(value);
}

void Unit::makeCOUnit(quint8 co)
{
    CO* pCO = m_pOwner->getCO(co);
    if (pCO != nullptr &&
        pCO->getCOUnit() == nullptr)
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
    else if (pCO->getCOUnit() != this)
    {
        setUnitRank(GameEnums::UnitRank_Veteran);
    }
}

qint32 Unit::getBonusLuck(QPoint position)
{
    qint32 bonus = 0;
    CO* pCO0 = m_pOwner->getCO(0);
    if (pCO0 != nullptr)
    {
        bonus += pCO0->getBonusLuck(this, position);
    }
    CO* pCO1 = m_pOwner->getCO(1);
    if (pCO1 != nullptr)
    {
        bonus += pCO1->getBonusLuck(this, position);
    }
    // apply star bonus
    pCO0 = m_pOwner->getCO(0);
    pCO1 = m_pOwner->getCO(1);
    if (pCO0 != nullptr && pCO1 != nullptr &&
        pCO0->getPowerMode() == GameEnums::PowerMode_Tagpower)
    {
        Mainapp* pApp = Mainapp::getInstance();
        QString function1 = "getTagstars";
        QJSValueList args1;
        QJSValue obj1 = pApp->getInterpreter()->newQObject(pCO0);
        args1 << obj1;
        QJSValue obj2 = pApp->getInterpreter()->newQObject(pCO1);
        args1 << obj2;
        QJSValue erg = pApp->getInterpreter()->doFunction("TAGPOWER", function1, args1);
        if (erg.isNumber())
        {
            bonus += erg.toInt() * 5;
        }
    }
    return bonus;
}

void Unit::startOfTurn()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "startOfTurn";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
    for (qint32 i = 0; i < m_TransportUnits.size(); i++)
    {
        m_TransportUnits[i]->startOfTurn();
    }
}

qint32 Unit::getCapturePoints() const
{
    return capturePoints;
}

void Unit::setCapturePoints(const qint32 &value)
{
    capturePoints = value;
    if (capturePoints < 0)
    {
        capturePoints = 0;
    }
    if (capturePoints > 0)
    {
        loadIcon("capture", GameMap::Imagesize / 2, GameMap::Imagesize / 2);
    }
    else
    {
        unloadIcon("capture");
    }
}

qint32 Unit::getBaseMovementPoints() const
{
    return baseMovementPoints;
}

void Unit::setBaseMovementPoints(const qint32 &value)
{
    baseMovementPoints = value;
}

qint32 Unit::getBaseMovementCosts(qint32 x, qint32 y)
{
    GameMap* pMap = GameMap::getInstance();
    return MovementTableManager::getInstance()->getBaseMovementPoints(getMovementType(), pMap->getTerrain(x, y), this);
}

qint32 Unit::getMovementCosts(qint32 x, qint32 y)
{
    qint32 baseCosts = getBaseMovementCosts(x, y);
    qint32 costs = baseCosts;
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = pMap->getPlayer(i);
        if (pPlayer != nullptr &&
            (pPlayer->isEnemy(m_pOwner) ||
            m_pOwner == pPlayer))
        {
            costs += pPlayer->getMovementcostModifier(this, QPoint(x, y));
        }
    }
    if (baseCosts >= 0)
    {
        if ((costs <= 0) && (baseCosts > 0))
        {
            return 1;
        }
        else
        {
            return costs;
        }
    }
    else
    {
        return -1;
    }
}

void Unit::initUnit()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "init";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
    setFuel(fuel);
    setAmmo1(ammo1);
    setAmmo2(ammo2);
    setHp(hp);
}

qint32 Unit::getMaxFuel() const
{
    return maxFuel;
}

void Unit::setMaxFuel(const qint32 &value)
{
    maxFuel = value;
}

qint32 Unit::getFuel() const
{
    return fuel;
}

void Unit::setFuel(const qint32 &value)
{
    if (maxFuel > 0)
    {
        fuel = value;
    }
    if (maxFuel > 0 && static_cast<float>(fuel) / static_cast<float>(maxFuel) <= 1.0f / 3.0f)
    {
        loadIcon("fuel", GameMap::Imagesize / 2, 0);
    }
    else
    {
        unloadIcon("fuel");
    }
}

qint32 Unit::getMaxAmmo2() const
{
    return maxAmmo2;
}

void Unit::setMaxAmmo2(const qint32 &value)
{
    maxAmmo2 = value;
}

qint32 Unit::getAmmo2() const
{
    return ammo2;
}

bool Unit::hasAmmo2() const
{
    if ((maxAmmo2 < 0) || (ammo2 > 0))
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
    return weapon2ID;
}

void Unit::setWeapon2ID(const QString &value)
{
    weapon2ID = value;
}

void Unit::setAmmo2(const qint32 &value)
{
    ammo2 = value;
    if ((ammo2 < 0) && (maxAmmo2 > 0))
    {
        ammo2 = 0;
    }
    else  if (maxAmmo2 > 0 && ammo2 < 0)
    {
        ammo2 = 0;
    }
    if (maxAmmo2 > 0 && static_cast<float>(ammo2) / static_cast<float>(maxAmmo2) <= 1.0f / 3.0f)
    {
        if (weapon2ID.isEmpty())
        {
            loadIcon("material1", GameMap::Imagesize / 2, 0);
        }
        else
        {
            loadIcon("ammo1", GameMap::Imagesize / 2, 0);
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
    if (ammo2 > 0)
    {
        setAmmo2(ammo2 - value);
    }
}

qint32 Unit::getMaxAmmo1() const
{
    return maxAmmo1;
}

void Unit::setMaxAmmo1(const qint32 &value)
{
    maxAmmo1 = value;
}

QString Unit::getWeapon1ID() const
{
    return weapon1ID;
}

void Unit::setWeapon1ID(const QString &value)
{
    weapon1ID = value;
}

qint32 Unit::getAmmo1() const
{
    return ammo1;
}

void Unit::setAmmo1(const qint32 &value)
{
    ammo1 = value;
    if ((ammo1 < 0) && (maxAmmo1 > 0))
    {
        ammo1 = 0;
    }
    else if (maxAmmo1 > 0 && ammo1 < 0)
    {
        ammo1 = 0;
    }

    if (maxAmmo1 > 0 && static_cast<float>(ammo1) / static_cast<float>(maxAmmo1) <= 1.0f / 3.0f)
    {
        if (weapon1ID.isEmpty())
        {
            loadIcon("material", GameMap::Imagesize / 2, 0);
        }
        else
        {
            loadIcon("ammo", GameMap::Imagesize / 2, 0);
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
    if ((maxAmmo1 < 0) || (ammo1 > 0))
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
    if (ammo1 > 0)
    {
        setAmmo1(ammo1 - value);
    }
}

float Unit::getHp() const
{
    return hp;
}

qint32 Unit::getHpRounded() const
{
    return Mainapp::roundUp(hp);
}

void Unit::setHp(const float &value)
{
    hp = value;
    if (hp > 10)
    {
        hp = 10.0f;
    }
    GameMap* pMap = GameMap::getInstance();
    if (pMap != nullptr)
    {
        updateIcons(pMap->getCurrentViewPlayer());
    }
}

bool Unit::getHpHidden(Player* pPlayer)
{
    if (pPlayer != nullptr && pPlayer->isEnemy(m_pOwner))
    {
        CO* pCO = m_pOwner->getCO(0);
        if (pCO != nullptr)
        {
            if (pCO->getHpHidden(this, QPoint(getX(), getY())))
            {
                return true;
            }
        }
        pCO = m_pOwner->getCO(1);
        if (pCO != nullptr)
        {
            if (pCO->getHpHidden(this, QPoint(getX(), getY())))
            {
                return true;
            }
        }

    }
    return false;
}

void Unit::updateIcons(Player* pPlayer)
{
    if (m_pTerrain != nullptr)
    {
        qint32 hpValue = Mainapp::roundUp(hp);
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
        if (!getHpHidden(pPlayer))
        {
            if ((hpValue < 10) && (hpValue > 0))
            {
                loadIcon(QString::number(hpValue), 0, GameMap::Imagesize / 2);
            }
        }
        else
        {
            loadIcon("hp+hidden", 0, GameMap::Imagesize / 2);
        }
        if (getTransportHidden(pPlayer))
        {
            loadIcon("transport+hidden", GameMap::Imagesize / 2, GameMap::Imagesize / 2);
        }
        else if (m_TransportUnits.size() > 0)
        {
            loadIcon("transport", GameMap::Imagesize / 2, GameMap::Imagesize / 2);
        }
    }
}

bool Unit::getTransportHidden(Player* pPlayer)
{
    GameMap* pMap = GameMap::getInstance();
    if (pPlayer != nullptr)
    {
        if ((pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off) &&
            (pPlayer->isEnemy(m_pOwner)) && getLoadingPlace() > 0)
        {
            return true;
        }
    }
    return false;
}

QString Unit::getMovementType()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getMovementType";
    QJSValueList args1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

qint32 Unit::getLoadingPlace()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getLoadingPlace";
    QJSValueList args1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

QString Unit::getUnitID()
{
    return m_UnitID;
}

QString Unit::getUnitDamageID()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getUnitDamageID";
    QJSValueList args1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
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

float Unit::getUnitDamage(QString weaponID)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getUnitDamage";
    QJSValueList args1;
    args1 << weaponID;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
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
        return m_pTerrain->getX();
    }
    else
    {
        return virtuellX;
    }
}

qint32 Unit::getY() const
{
    if (m_pTerrain != nullptr)
    {
       return m_pTerrain->getY();
    }
    else
    {
        return virtuellY;
    }
}

void Unit::refill()
{
    setFuel(maxFuel);
    setAmmo1(maxAmmo1);
    setAmmo2(maxAmmo2);
}

void Unit::setHasMoved(bool value)
{
    m_Moved = value;
    // change unit color
    if (m_Moved)
    {
        for(qint32 i = 0; i < m_pUnitWaitSprites.size(); i++)
        {
            m_pUnitWaitSprites[i]->setVisible(true);
        }
    }
    else
    {
        for(qint32 i = 0; i < m_pUnitWaitSprites.size(); i++)
        {
            m_pUnitWaitSprites[i]->setVisible(false);
        }
    }
}

bool Unit::getHasMoved()
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

bool Unit::getFirstStrike(QPoint position, Unit* pAttacker)
{
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        if (pCO->getFirstStrike(this, position, pAttacker))
        {
            return true;
        }
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        if (pCO->getFirstStrike(this, position, pAttacker))
        {
            return true;
        }
    }
    return false;
}

qint32 Unit::getBonusMovementpoints(QPoint position)
{
    qint32 movementModifier = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        movementModifier += pCO->getMovementpointModifier(this, position);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        movementModifier += pCO->getMovementpointModifier(this, position);
    }
    return movementModifier;
}

qint32 Unit::getMovementpoints(QPoint position)
{
    qint32 points = baseMovementPoints + getBonusMovementpoints(position);
    if (points < 0)
    {
        points = 0;
    }
    if (fuel < points)
    {
        return fuel;
    }
    return points;
}

QStringList Unit::getActionList()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getActions";
    QJSValue ret = pApp->getInterpreter()->doFunction(m_UnitID, function1);
    if (ret.isString())
    {
        QStringList actionList = ret.toString().split(",");
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
    else
    {
        return QStringList();
    }
}

void Unit::moveUnitAction(GameAction* pAction)
{
    // reduce fuel
    GameMap* pMap = GameMap::getInstance();
    qint32 fuelCost = pAction->getCosts();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = pMap->getPlayer(i);
        if (pPlayer != nullptr &&
            (pPlayer->isEnemy(m_pOwner) ||
            m_pOwner == pPlayer))
        {
            CO* pCO = pPlayer->getCO(0);
            if (pCO != nullptr)
            {
                fuelCost += pCO->getMovementFuelCostModifier(this, fuelCost);
            }
            pCO = pPlayer->getCO(1);
            if (pCO != nullptr)
            {
                fuelCost += pCO->getMovementFuelCostModifier(this, fuelCost);
            }
        }
    }
    if (fuelCost < 0)
    {
        fuelCost = 0;
    }
    qint32 value = fuel - fuelCost;
    if (value < 0)
    {
        value = 0;
    }
    setFuel(value);
    moveUnit(pAction->getMovePath());
}

void Unit::moveUnit(QVector<QPoint> movePath)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (movePath.size() < 1)
    {
        movePath.append(QPoint(getX(), getY()));
    }
    // update vision based on the movepath of the unit
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < movePath.size(); i++)
    {
        QmlVectorPoint* pCircle = Mainapp::getCircle(0, getVision(movePath[i]));
        for (qint32 i2 = 0; i2 < pCircle->size(); i2++)
        {
            QPoint circleField = pCircle->at(i2);
            QPoint field = circleField + QPoint(movePath[i].x(), movePath[i].y());
            if (pMap->onMap(field.x(), field.y()))
            {
                if (qAbs(circleField.x()) + qAbs(circleField.y()) <= 1)
                {
                    m_pOwner->addVisionField(field.x(), field.y(), 1, true);
                }
                else
                {
                    Terrain* pTerrain = pMap->getTerrain(field.x(), field.y());
                    Unit* pUnit = pTerrain->getUnit();
                    bool visionHide = pTerrain->getVisionHide(m_pOwner);
                    if ((!visionHide) ||
                        ((pUnit != nullptr) && visionHide &&
                         !pUnit->useTerrainDefense() && !pUnit->getHidden()))
                    {
                        m_pOwner->addVisionField(field.x(), field.y(), 1, false);
                    }
                }
            }
        }
        delete pCircle;
    }
    if (movePath.size() > 1)
    {
        moveUnitToField(movePath[0].x(), movePath[0].y());
    }
    pApp->continueThread();
}

void Unit::moveUnitToField(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    // reset capture points when moving  a unit
    setCapturePoints(0);

    GameMap* pMap = GameMap::getInstance();
    spUnit pUnit = m_pTerrain->getSpUnit();
    // teleport unit to target position
    pMap->getTerrain(x, y)->setUnit(pUnit);
    if (m_CORange.get() != nullptr)
    {
        if (m_UnitRank == GameEnums::UnitRank_CO0)
        {
            createCORange(m_pOwner->getCO(0)->getCORange());
        }
        else //if (m_UnitRank == GameEnums::UnitRank_CO1)
        {
            createCORange(m_pOwner->getCO(1)->getCORange());
        }
    }
    pUnit = nullptr;
    pApp->continueThread();
}

void Unit::removeUnit()
{
    if (m_UnitRank == GameEnums::UnitRank_CO0)
    {
        CO* pCO = m_pOwner->getCO(0);
        if (pCO != nullptr)
        {
            pCO->setCOUnit(nullptr);
        }
    }
    else if (m_UnitRank == GameEnums::UnitRank_CO1)
    {
        CO* pCO = m_pOwner->getCO(1);
        if (pCO != nullptr)
        {
            pCO->setCOUnit(nullptr);
        }
    }
    if (m_CORange.get() != nullptr)
    {
        GameMap::getInstance()->removeChild(m_CORange);
    }
    m_pTerrain->setUnit(nullptr);    
}

void Unit::killUnit()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "createExplosionAnimation";
    QJSValueList args1;
    args1 << getX();
    args1 << getY();
    QJSValue ret = pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);

    // record destruction of this unit
    GameRecorder* pRecorder = GameMap::getInstance()->getGameRecorder();
    if (pRecorder != nullptr)
    {
        GameMap::getInstance()->getGameRecorder()->lostUnit(m_pOwner->getPlayerID());
    }

    detach();
    removeUnit();
}

void Unit::increaseCapturePoints(QPoint position)
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

    capturePoints += getHpRounded() + modifier;
    // update icons
    setCapturePoints(capturePoints);
}

void Unit::loadIcon(QString iconID, qint32 x, qint32 y)
{
    for (qint32 i = 0; i < m_pIconSprites.size(); i++)
    {
        if (m_pIconSprites[i]->getResAnim()->getName() == iconID.toStdString())
        {
            // already loaded icon
            return;
        }
    }
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pUnitSpriteManager->getResAnim(iconID.toStdString());
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
        pSprite->setScale((GameMap::Imagesize / 2) / pAnim->getWidth() );
        pSprite->setPosition(x, y);
        pSprite->setPriority(static_cast<short>(Priorities::Icons));

        this->addChild(pSprite);
        m_pIconSprites.append(pSprite);

        updateIconTweens();
    }
    else
    {
        Console::print("Unable to load icon sprite: " + iconID, Console::eERROR);
    }
}

void Unit::unloadIcon(QString iconID)
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pUnitSpriteManager->getResAnim(iconID.toStdString());
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
                x = GameMap::Imagesize / 2;
                y = 0;
                break;
            }
            case 2:
            {
                x = 0;
                y = GameMap::Imagesize / 2;
                break;
            }
            case 3:
            {
                x = GameMap::Imagesize / 2;
                y = GameMap::Imagesize / 2;
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
                    oxygine::spTween tween = oxygine::createTween(TweenToggleVisibility(startTime, endTime), visibileTime * count, -1);
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
    if (!weapon1ID.isEmpty() || !weapon2ID.isEmpty())
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

void Unit::setAiMode(const GameEnums::GameAi &AiMode)
{
    m_AiMode = AiMode;
    unloadIcon("defensive");
    unloadIcon("hold");
    unloadIcon("normal");
    unloadIcon("offensive");
    if (EditorMenue::getInstance() != nullptr)
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

bool Unit::getHidden() const
{
    return m_Hidden;
}

void Unit::setHidden(bool Hidden)
{
    m_Hidden = Hidden;
    if (m_Hidden)
    {
        loadIcon("dive", GameMap::Imagesize / 2, GameMap::Imagesize / 2);
    }
    else
    {
        unloadIcon("dive");
    }
}

bool Unit::isStealthed(Player* pPlayer, bool ignoreOutOfVisionRange, qint32 testX, qint32 testY)
{
    if (pPlayer != nullptr &&
        pPlayer->checkAlliance(m_pOwner) == GameEnums::Alliance_Enemy)
    {
        GameMap* pMap = GameMap::getInstance();
        qint32 x = getX();
        qint32 y = getY();
        if (pMap->onMap(testX, testY))
        {
            x = testX;
            y = testY;
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
        if (getHidden() ||
            (m_pTerrain->getVisionHide(pPlayer) && useTerrainDefense() && !visibleField &&
             pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off))
        {
            QmlVectorPoint* pPoints = Mainapp::getCircle(1, 1);
            for (qint32 i = 0; i < pPoints->size(); i++)
            {
                QPoint point = pPoints->at(i);
                if (pMap->onMap(point.x() + x, point.y() + y))
                {
                    Unit* pVisionUnit = pMap->getTerrain(x + point.x(), y + point.y())->getUnit();
                    if (pVisionUnit != nullptr)
                    {
                        if (pPlayer->checkAlliance(pVisionUnit->getOwner()) == GameEnums::Alliance_Friend)
                        {
                            delete pPoints;
                            return false;
                        }
                    }
                }
            }
            delete pPoints;
            return true;
        }
    }
    return false;
}

void Unit::serializeObject(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << m_UnitID.toStdString().c_str();
    pStream << hp;
    pStream << ammo1;
    pStream << ammo2;
    pStream << fuel;
    pStream << static_cast<qint32>(m_UnitRank);
    pStream << m_pOwner->getPlayerID();
    pStream << m_Moved;
    qint32 units = static_cast<qint32>(m_TransportUnits.size());
    pStream << units;
    for (qint32 i = 0; i < units; i++)
    {
        m_TransportUnits[i]->serializeObject(pStream);
    }
    pStream << capturePoints;
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
}

void Unit::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    char* id;
    pStream >> id;
    m_UnitID = id;
    initUnit();
    pStream >> hp;
    setHp(hp);
    pStream >> ammo1;
    setAmmo1(ammo1);
    pStream >> ammo2;
    setAmmo2(ammo2);
    pStream >> fuel;
    setFuel(fuel);
    qint32 value = 0;
    pStream >> value;
    setUnitRank(static_cast<GameEnums::UnitRanks>(value));
    quint32 playerID = 0;
    pStream >> playerID;
    m_pOwner = GameMap::getInstance()->getPlayer(playerID);
    if (m_UnitRank == GameEnums::UnitRank_CO0)
    {
        makeCOUnit(0);
    }
    else if (m_UnitRank == GameEnums::UnitRank_CO1)
    {
        makeCOUnit(1);
    }
    if (version > 1)
    {
        pStream >> m_Moved;
        setHasMoved(m_Moved);
        qint32 units;
        pStream >> units;
        for (qint32 i = 0; i < units; i++)
        {
            m_TransportUnits.append(new Unit());
            m_TransportUnits[i]->deserializeObject(pStream);
        }
    }
    if (version > 2)
    {
        pStream >> capturePoints;
        setCapturePoints(capturePoints);
    }
    if (version > 3)
    {
        pStream >> m_Hidden;
        setHidden(m_Hidden);
    }
    if (version > 4)
    {
        m_Variables.deserializeObject(pStream);
    }
    if (version > 5)
    {
        pStream >> m_IgnoreUnitCollision;
    }
    if (version > 6)
    {
        pStream >> value;
        setAiMode(static_cast<GameEnums::GameAi>(value));
    }
    else
    {
        setAiMode(GameEnums::GameAi_Normal);
    }
    if (version > 7)
    {
        pStream >> m_UniqueID;
    }
    if (m_UniqueID == 0)
    {
        m_UniqueID = GameMap::getInstance()->getUniqueIdCounter();
    }
    if (version > 8)
    {
        quint8 value = 0;
        pStream >> value;
        m_ModdingFlags = static_cast<ModdingFlags>(value);
    }
    if (version > 9)
    {
        qint32 size = m_MultiTurnPath.size();
        pStream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            QPoint point;
            pStream << point;
            m_MultiTurnPath.append(point);
        }
    }
}



void Unit::createCORange(qint32 coRange)
{

    oxygine::ResAnim* pAnim = GameManager::getInstance()->getResAnim("co+range+marker");
    GameMap* pMap = GameMap::getInstance();
    if (m_CORange.get() == nullptr)
    {
        m_CORange = new oxygine::Actor();
    }
    m_CORange->removeChildren();
    m_CORange->setPriority(static_cast<qint16>(Mainapp::ZOrder::CORange));
    if (m_pTerrain != nullptr)
    {
        qint32 x = getX();
        qint32 y = getY();
        qint32 x2 = 0;
        qint32 y2 = 0;
        x2 = -coRange;
        y2 = 0;
        QColor color = m_pOwner->getColor();
        oxygine::Color playerColor = oxygine::Color(color.red(), color.green(), color.blue());
        oxygine::Color inversColor = oxygine::Color(playerColor.r, playerColor.g, playerColor.b, 255);
        for (qint32 i = 0; i < coRange; i++)
        {
            x2 += 1;
            y2 += 1;
            if (pMap->onMap(x2 + x, y2 + y))
            {
                oxygine::spSprite pSprite = new oxygine::Sprite();
                pSprite->setResAnim(pAnim);
                pSprite->setColor(playerColor);
                if (pAnim->getTotalFrames() > 1)
                {
                    oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                    pSprite->addTween(tween);
                }
                else
                {
                    pSprite->setResAnim(pAnim);
                }
                oxygine::Sprite::TweenColor tweenColor(inversColor);
                oxygine::spTween tween = oxygine::createTween(tweenColor, 500,  -1, true);
                pSprite->addTween(tween);
                pSprite->setPosition(GameMap::Imagesize * x2, GameMap::Imagesize * y2);
                m_CORange->addChild(pSprite);

                pSprite = new oxygine::Sprite();
                pSprite->setResAnim(pAnim);
                pSprite->setColor(playerColor);
                if (pAnim->getTotalFrames() > 1)
                {
                    oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                    pSprite->addTween(tween);
                }
                else
                {
                    pSprite->setResAnim(pAnim);
                }
                tween = oxygine::createTween(tweenColor, 500,  -1, true);
                pSprite->addTween(tween);
                pSprite->setRotationDegrees(90);
                pSprite->setPosition(GameMap::Imagesize * (x2 + 1), GameMap::Imagesize * (y2 + 1));
                m_CORange->addChild(pSprite);
                if (i == coRange - 1)
                {
                    pSprite = new oxygine::Sprite();
                    pSprite->setResAnim(pAnim);
                    pSprite->setColor(playerColor);
                    if (pAnim->getTotalFrames() > 1)
                    {
                        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                        pSprite->addTween(tween);
                    }
                    else
                    {
                        pSprite->setResAnim(pAnim);
                    }
                    tween = oxygine::createTween(tweenColor, 500,  -1, true);
                    pSprite->addTween(tween);
                    pSprite->setRotationDegrees(180);
                    pSprite->setPosition(GameMap::Imagesize * (x2 + 1), GameMap::Imagesize * (y2 + 1));
                    m_CORange->addChild(pSprite);
                }
            }
        }
        for (qint32 i = 0; i < coRange; i++)
        {
            x2 += 1;
            y2 -= 1;
            if (pMap->onMap(x2 + x, y2 + y))
            {
                oxygine::spSprite pSprite = new oxygine::Sprite();
                pSprite->setResAnim(pAnim);
                pSprite->setColor(playerColor);
                if (pAnim->getTotalFrames() > 1)
                {
                    oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                    pSprite->addTween(tween);
                }
                else
                {
                    pSprite->setResAnim(pAnim);
                }
                oxygine::Sprite::TweenColor tweenColor(inversColor);
                oxygine::spTween tween = oxygine::createTween(tweenColor, 500,  -1, true);
                pSprite->addTween(tween);
                pSprite->setRotationDegrees(90);
                pSprite->setPosition(GameMap::Imagesize * (x2 + 1), GameMap::Imagesize * (y2 + 1));
                m_CORange->addChild(pSprite);

                pSprite = new oxygine::Sprite();
                pSprite->setResAnim(pAnim);
                pSprite->setColor(playerColor);
                if (pAnim->getTotalFrames() > 1)
                {
                    oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                    pSprite->addTween(tween);
                }
                else
                {
                    pSprite->setResAnim(pAnim);
                }
                tween = oxygine::createTween(tweenColor, 500,  -1, true);
                pSprite->addTween(tween);
                pSprite->setRotationDegrees(180);
                pSprite->setPosition(GameMap::Imagesize * (x2 + 1), GameMap::Imagesize * (y2 + 1));
                m_CORange->addChild(pSprite);
                if (i == coRange - 1)
                {
                    pSprite = new oxygine::Sprite();
                    pSprite->setResAnim(pAnim);
                    pSprite->setColor(playerColor);
                    if (pAnim->getTotalFrames() > 1)
                    {
                        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                        pSprite->addTween(tween);
                    }
                    else
                    {
                        pSprite->setResAnim(pAnim);
                    }
                    tween = oxygine::createTween(tweenColor, 500,  -1, true);
                    pSprite->addTween(tween);
                    pSprite->setRotationDegrees(270);
                    pSprite->setPosition(GameMap::Imagesize * x2, GameMap::Imagesize * y2);
                    m_CORange->addChild(pSprite);
                }
            }
        }
        for (qint32 i = 0; i < coRange; i++)
        {
            x2 -= 1;
            y2 -= 1;
            if (pMap->onMap(x2 + x, y2 + y))
            {
                oxygine::spSprite pSprite = new oxygine::Sprite();
                pSprite->setResAnim(pAnim);
                pSprite->setColor(playerColor);
                if (pAnim->getTotalFrames() > 1)
                {
                    oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                    pSprite->addTween(tween);
                }
                else
                {
                    pSprite->setResAnim(pAnim);
                }
                oxygine::Sprite::TweenColor tweenColor(inversColor);
                oxygine::spTween tween = oxygine::createTween(tweenColor, 500,  -1, true);
                pSprite->addTween(tween);
                pSprite->setRotationDegrees(270);
                pSprite->setPosition(GameMap::Imagesize * x2, GameMap::Imagesize * y2);
                m_CORange->addChild(pSprite);

                pSprite = new oxygine::Sprite();
                pSprite->setResAnim(pAnim);
                pSprite->setColor(playerColor);
                if (pAnim->getTotalFrames() > 1)
                {
                    oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                    pSprite->addTween(tween);
                }
                else
                {
                    pSprite->setResAnim(pAnim);
                }
                tween = oxygine::createTween(tweenColor, 500,  -1, true);
                pSprite->addTween(tween);
                pSprite->setRotationDegrees(180);
                pSprite->setPosition(GameMap::Imagesize * (x2 + 1), GameMap::Imagesize * (y2 + 1));
                m_CORange->addChild(pSprite);
                if (i == coRange - 1)
                {
                    pSprite = new oxygine::Sprite();
                    pSprite->setResAnim(pAnim);
                    pSprite->setColor(playerColor);
                    if (pAnim->getTotalFrames() > 1)
                    {
                        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                        pSprite->addTween(tween);
                    }
                    else
                    {
                        pSprite->setResAnim(pAnim);
                    }
                    tween = oxygine::createTween(tweenColor, 500,  -1, true);
                    pSprite->addTween(tween);
                    pSprite->setPosition(GameMap::Imagesize * x2, GameMap::Imagesize * y2);
                    m_CORange->addChild(pSprite);
                }
            }
        }
        for (qint32 i = 0; i < coRange; i++)
        {

            x2 -= 1;
            y2 += 1;
            if (pMap->onMap(x2 + x, y2 + y))
            {
                oxygine::spSprite pSprite = new oxygine::Sprite();
                pSprite->setResAnim(pAnim);
                pSprite->setColor(playerColor);
                if (pAnim->getTotalFrames() > 1)
                {
                    oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                    pSprite->addTween(tween);
                }
                else
                {
                    pSprite->setResAnim(pAnim);
                }
                oxygine::Sprite::TweenColor tweenColor(inversColor);
                oxygine::spTween tween = oxygine::createTween(tweenColor, 500,  -1, true);
                pSprite->addTween(tween);
                pSprite->setRotationDegrees(270);
                pSprite->setPosition(GameMap::Imagesize * x2, GameMap::Imagesize * y2);
                m_CORange->addChild(pSprite);

                pSprite = new oxygine::Sprite();
                pSprite->setResAnim(pAnim);
                pSprite->setColor(playerColor);
                if (pAnim->getTotalFrames() > 1)
                {
                    oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                    pSprite->addTween(tween);
                }
                else
                {
                    pSprite->setResAnim(pAnim);
                }
                tween = oxygine::createTween(tweenColor, 500,  -1, true);
                pSprite->addTween(tween);
                pSprite->setPosition(GameMap::Imagesize * x2, GameMap::Imagesize * y2);
                m_CORange->addChild(pSprite);

                if (i == coRange - 1)
                {
                    pSprite = new oxygine::Sprite();
                    pSprite->setResAnim(pAnim);
                    pSprite->setColor(playerColor);
                    if (pAnim->getTotalFrames() > 1)
                    {
                        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                        pSprite->addTween(tween);
                    }
                    else
                    {
                        pSprite->setResAnim(pAnim);
                    }
                    tween = oxygine::createTween(tweenColor, 500,  -1, true);
                    pSprite->addTween(tween);
                    pSprite->setRotationDegrees(90);
                    pSprite->setPosition(GameMap::Imagesize * (x2 + 1), GameMap::Imagesize * (y2 + 1));
                    m_CORange->addChild(pSprite);
                }
            }
        }
        m_CORange->setPosition(GameMap::Imagesize * getX(), GameMap::Imagesize * getY());
        pMap->addChild(m_CORange);
    }
}
