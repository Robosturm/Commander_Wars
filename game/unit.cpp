#include "unit.h"

#include <QColor>

#include "game/player.h"

#include "resource_management/unitspritemanager.h"

#include "resource_management/gamemanager.h"

#include "game/gamemap.h"

#include "coreengine/console.h"

#include "game/gameaction.h"

#include "game/co.h"

#include "game/building.h"

#include "coreengine/tweentogglevisibility.h"

Unit::Unit()
{
    Interpreter::setCppOwnerShip(this);
}

Unit::Unit(QString unitID, Player* pOwner)
    : QObject(),
      m_UnitID(unitID),
      m_pOwner(pOwner)
{
    Interpreter::setCppOwnerShip(this);
    if (!m_UnitID.isEmpty())
    {
        initUnit();
        updateSprites();
    }
}

Unit::~Unit()
{
    if (m_pOwner != nullptr)
    {
        CO* pCO = m_pOwner->getCO(0);
        if (pCO != nullptr)
        {
            if (pCO->getCOUnit() == this)
            {
                pCO->setCOUnit(nullptr);
            }
        }
        pCO = m_pOwner->getCO(1);
        if (pCO != nullptr)
        {
            pCO->setCOUnit(nullptr);
        }
    }
}

void Unit::setOwner(Player* pOwner)
{
    // change ownership
    m_pOwner = pOwner;
    // update sprites :)
    updateSprites();
}

void Unit::setTerrain(Terrain* pTerrain)
{
    m_pTerrain = pTerrain;
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
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
            pSprite->addTween(tween);

            oxygine::spTween tweenWait = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
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

        oxygine::Sprite::TweenColor tweenColor(oxygine::Color(100, 100, 100, 100));
        oxygine::spTween tweenWait = oxygine::createTween(tweenColor, 1);
        pWaitSprite->addTween(tweenWait);
        pWaitSprite->setScale(GameMap::Imagesize / pAnim->getWidth());
        pWaitSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::Imagesize) / 2, -(pSprite->getScaledHeight() - GameMap::Imagesize));
        pWaitSprite->setPriority(static_cast<short>(Priorities::Waiting));
        this->addChild(pWaitSprite);
        pWaitSprite->setVisible(false);
        m_pUnitWaitSprites.append(pWaitSprite);
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
}

GameEnums::UnitRanks Unit::getUnitRank() const
{
    return m_UnitRank;
}

void Unit::setUnitRank(const GameEnums::UnitRanks &UnitRank)
{
    GameMap* pMap = GameMap::getInstance();
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
    qint32 points = vision + rangeModifier;

    if (points < 0)
    {
        points = 0;
    }
    return points;
}

void Unit::setVision(const qint32 &value)
{
    vision = value;
}

qint32 Unit::getMaxRange()
{
    return getMaxRange(QPoint(getX(), getY()));
}

qint32 Unit::getMaxRange(QPoint position)
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
    qint32 points = maxRange + rangeModifier;

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
    // todo check which icon we need to load
    loadIcon("transport", GameMap::Imagesize / 2, GameMap::Imagesize / 2);
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
    for (qint32 i = 0; i < m_TransportUnits.size(); i++)
    {
        if (m_TransportUnits[i] == pUnit)
        {
            GameMap::getInstance()->getTerrain(position.x(), position.y())->setUnit(m_TransportUnits[i]);
            m_TransportUnits.removeAt(i);
            break;
        }
    }
    if (m_TransportUnits.size() <= 0)
    {
        unloadIcon("transport");
        unloadIcon("transport+hidden");
    }
}

qint32 Unit::getLoadedUnitCount()
{
    return m_TransportUnits.size();
}

qint32 Unit::getBonusOffensive(QPoint position, Unit* pDefender, QPoint defPosition, bool isDefender)
{
    qint32 bonus = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        bonus += pCO->getOffensiveBonus(this, position, pDefender, defPosition, isDefender);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        bonus += pCO->getOffensiveBonus(this, position, pDefender, defPosition, isDefender);
    }
    GameMap* pMap = GameMap::getInstance();
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

qint32 Unit::getBonusDefensive(QPoint position, Unit* pAttacker, QPoint atkPosition)
{
    qint32 bonus = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        bonus += pCO->getDeffensiveBonus(pAttacker, atkPosition, this, position);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        bonus += pCO->getDeffensiveBonus(pAttacker, atkPosition, this, position);
    }
    GameMap* pMap = GameMap::getInstance();
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
        bonus += pMap->getTerrain(position.x(), position.y())->getDefense(this) * 10;
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
    if (pCO != nullptr)
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
}

qint32 Unit::getBonusLuck(QPoint position)
{
    qint32 bonus = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        bonus += pCO->getBonusLuck(this, position);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        bonus += pCO->getBonusLuck(this, position);
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
}

qint32 Unit::getCapturePoints() const
{
    return capturePoints;
}

void Unit::setCapturePoints(const qint32 &value)
{
    capturePoints = value;
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
    fuel = value;
    if (static_cast<float>(fuel) / static_cast<float>(maxFuel) <= 1.0f / 3.0f)
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

    if (static_cast<float>(ammo2) / static_cast<float>(maxAmmo2) <= 1.0f / 3.0f)
    {
        loadIcon("ammo", GameMap::Imagesize / 2, 0);
    }
    else
    {
        unloadIcon("ammo");
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

    if (static_cast<float>(ammo1) / static_cast<float>(maxAmmo1) <= 1.0f / 3.0f)
    {
        loadIcon("ammo", GameMap::Imagesize / 2, 0);
    }
    else
    {
        unloadIcon("ammo");
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
    qint32 hpValue = Mainapp::roundUp(hp);
    // unload the number icons
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
    if ((hpValue < 10) && (hpValue > 0))
    {
        loadIcon(QString::number(hpValue), 0, GameMap::Imagesize / 2);
    }
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

QString Unit::getUnitID()
{
    return m_UnitID;
}

qint32 Unit::getX() const
{
    return m_pTerrain->getX();
}

qint32 Unit::getY() const
{
    return m_pTerrain->getY();
}

QPoint Unit::getPosition() const
{
    return QPoint(getX(), getY());
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

qint32 Unit::getMovementPoints()
{
    qint32 movementModifier = 0;
    CO* pCO = m_pOwner->getCO(0);
    if (pCO != nullptr)
    {
        movementModifier += pCO->getMovementPointModifier(this);
    }
    pCO = m_pOwner->getCO(1);
    if (pCO != nullptr)
    {
        movementModifier += pCO->getMovementPointModifier(this);
    }
    qint32 points = baseMovementPoints + movementModifier;
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
    QJSValueList args1;
    QJSValue ret = pApp->getInterpreter()->doFunction(m_UnitID, function1, args1);
    if (ret.isString())
    {
        return ret.toString().split(",");
    }
    else
    {
        return QStringList();
    }
}

void Unit::moveUnitAction(GameAction* pAction)
{
    // reduce fuel
    setFuel(fuel - pAction->getCosts());
    moveUnit(pAction->getMovePath());
}

void Unit::moveUnit(QVector<QPoint> movePath)
{
    if (movePath.size() > 0)
    {
        // reset capture points when moving  a unit
        setCapturePoints(0);

        GameMap* pMap = GameMap::getInstance();
        spUnit pUnit = m_pTerrain->getSpUnit();
        m_pTerrain->setUnit(nullptr);
        // teleport unit to target position
        Console::print("Moving Unit from " + QString::number(getX()) + " , " + QString::number(getY()) + " to " + QString::number(movePath[0].x()) + " , " + QString::number(movePath[0].y()), Console::eLogLevels::eDEBUG);
        pMap->getTerrain(movePath[0].x(), movePath[0].y())->setUnit(pUnit);
        if (m_CORange.get() != nullptr)
        {
            if (m_UnitRank == GameEnums::UnitRank_CO0)
            {
                createCORange(m_pOwner->getCO(0)->getCORange());
            }
            else
            {
                createCORange(m_pOwner->getCO(1)->getCORange());
            }
        }
    }
}

void Unit::removeUnit()
{
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
    if (m_UnitRank == GameEnums::UnitRank_CO0)
    {
        m_pOwner->getCO(0)->setCOUnit(nullptr);
    }
    else if (m_UnitRank == GameEnums::UnitRank_CO1)
    {
        m_pOwner->getCO(1)->setCOUnit(nullptr);
    }
    if (m_CORange.get() != nullptr)
    {
        GameMap::getInstance()->removeChild(m_CORange);
    }

    removeUnit();
}

void Unit::increaseCapturePoints(QPoint position)
{
    // todo add ko modifications
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
                this->removeChild(m_pIconSprites[i]);
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
    }
}

void Unit::serialize(QDataStream& pStream)
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
    qint32 units = m_TransportUnits.size();
    pStream << units;
    for (qint32 i = 0; i < units; i++)
    {
        m_TransportUnits[i]->serialize(pStream);
    }
    pStream << capturePoints;
    pStream << m_Hidden;
    m_Variables.serialize(pStream);
}

void Unit::deserialize(QDataStream& pStream)
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
    if (m_UnitRank == GameEnums::UnitRank_CO0)
    {
        makeCOUnit(0);
    }
    else if (m_UnitRank == GameEnums::UnitRank_CO1)
    {
        makeCOUnit(1);
    }
    quint32 playerID = 0;
    pStream >> playerID;
    m_pOwner = GameMap::getInstance()->getPlayer(playerID);
    if (version > 1)
    {
        pStream >> m_Moved;
        setHasMoved(m_Moved);
        qint32 units;
        pStream >> units;
        for (qint32 i = 0; i < units; i++)
        {
            m_TransportUnits.append(new Unit());
            m_TransportUnits[i]->deserialize(pStream);
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
    }
    if (version > 4)
    {
        m_Variables.deserialize(pStream);
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
