#include "co.h"

#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"

#include "game/unit.h"

#include "menue/gamemenue.h"

#include "game/gameanimationfactory.h"
#include "game/gameanimationdialog.h"
#include "game/gameanimationpower.h"

#include "resource_management/cospritemanager.h"

#include "game/player.h"

CO::CO(QString coID, Player* owner)
    : m_Owner(owner),
      coID(coID)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    init();
}

void CO::init()
{
    if (!coID.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "init";
        QJSValueList args1;
        QJSValue obj1 = pInterpreter->newQObject(this);
        args1 << obj1;
        QJSValue erg = pInterpreter->doFunction(this->coID, function1, args1);
    }
}

bool CO::isValid()
{
    return COSpriteManager::getInstance()->exists(coID);
}

float CO::getUnitBuildValue(QString unitID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getUnitBuildValue";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    args1 << unitID;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toNumber();
    }
    else
    {
        return 0.0f;
    }
}

void CO::setCOUnit(Unit* pUnit)
{
    if (pUnit == nullptr && m_pCOUnit != nullptr && GameMenue::getInstance() != nullptr)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "onCOUnitLost";
        QJSValueList args1;
        QJSValue obj1 = pInterpreter->newQObject(this);
        args1 << obj1;
        QJSValue erg = pInterpreter->doFunction(this->coID, function1, args1);
    }
    m_pCOUnit = pUnit;
}

QString CO::getCoID() const
{
    return coID;
}

double CO::getPowerFilled() const
{
    return powerFilled;
}

void CO::startOfTurn()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "startOfTurn";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    pInterpreter->doFunction(coID, function1, args1);
}

void CO::setPowerFilled(const double &value)
{
    if (!GameMap::getInstance()->getGameRules()->getNoPower())
    {
        if (GameMenue::getInstance() != nullptr)
        {
            Mainapp* pApp = Mainapp::getInstance();
            Interpreter* pInterpreter = Interpreter::getInstance();
            if (powerFilled < powerStars && value >= powerStars)
            {
                pApp->getAudioThread()->playSound("powerready.wav");
            }
            else if (powerFilled < powerStars + superpowerStars && value >= powerStars + superpowerStars)
            {
                pApp->getAudioThread()->playSound("superpowerready.wav");
            }
        }
        powerFilled = value;
        if (powerFilled > powerStars + superpowerStars)
        {
            powerFilled = powerStars + superpowerStars;
        }
        else if (powerFilled < 0)
        {
            powerFilled = 0;
        }
    }
    if (GameMenue::getInstance() != nullptr)
    {
        GameMenue::getInstance()->updatePlayerinfo();
    }
}

void CO::addPowerFilled(const double &value)
{
    setPowerFilled(value + powerFilled);
}

qint32 CO::getSuperpowerStars() const
{
    return superpowerStars;
}

void CO::setSuperpowerStars(const qint32 &value)
{        
    superpowerStars = value;
}

bool CO::canUseSuperpower() const
{
    if ((powerFilled >= powerStars + superpowerStars) &&
        (superpowerStars > 0) &&
        (m_PowerMode == GameEnums::PowerMode_Off))
    {
        return true;
    }
    return false;
}

bool CO::canUsePower() const
{
    if ((powerFilled >= powerStars) &&
        (powerStars > 0) &&
        (m_PowerMode == GameEnums::PowerMode_Off))
    {
        return true;
    }
    return false;
}

qint32 CO::getPowerStars() const
{
    return powerStars;
}

void CO::setPowerStars(const qint32 &value)
{
    powerStars = value;
}

qint32 CO::getTerrainDefenseModifier(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTerrainDefenseModifier";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

bool CO::getFirstStrike(Unit* pUnit, QPoint position, Unit* pAttacker)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getFirstStrike";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue obj3 = pInterpreter->newQObject(pAttacker);
    args1 << obj3;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isBool())
    {
        return erg.toBool();
    }
    else
    {
        return 0;
    }
}

qint32 CO::getEnemyTerrainDefenseModifier(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getEnemyTerrainDefenseModifier";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 CO::getVisionrangeModifier(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getVisionrangeModifier";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

QString CO::getCOName()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getName";
    QJSValue erg = pInterpreter->doFunction(coID, function1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return tr("Unknown");
    }
}

QStringList CO::getCOUnits(Building* pBuilding)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getCOUnits";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pBuilding);
    args1 << obj1;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    return erg.toVariant().toStringList();
}

qint32 CO::getMovementpointModifier(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMovementpointModifier";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 CO::getFirerangeModifier(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getFirerangeModifier";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 CO::getMinFirerangeModifier(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMinFirerangeModifier";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

bool CO::getHpHidden(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getHpHidden";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isBool())
    {
        return erg.toBool();
    }
    else
    {
        return 0;
    }
}

qint32 CO::getAttackHpBonus(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getAttackHpBonus";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 CO::getBonusLuck(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBonusLuck";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 CO::getBonusMisfortune(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBonusMisfortune";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

QString CO::getAdditionalBuildingActions(Building* pBuilding)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getAdditionalBuildingActions";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pBuilding);
    args1 << obj1;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

qint32 CO::getFuelCostModifier(Unit* pUnit, QPoint position, qint32 costs)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getFuelCostModifier";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    args1 << costs;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 CO::getMovementcostModifier(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMovementcostModifier";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 CO::getMovementFuelCostModifier(Unit* pUnit, qint32 fuelCost)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMovementFuelCostModifier";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    args1 << fuelCost;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

void CO::buildedUnit(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "buildedUnit";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
}

qint32 CO::getCostModifier(QString id, qint32 baseCost)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getCostModifier";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    args1 << id;
    args1 << baseCost;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

QString CO::getCOArmy()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getCOArmy";
    QJSValueList args1;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "OS";
    }
}

bool CO::getCanMoveAndFire(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getCanMoveAndFire";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isBool())
    {
        return erg.toBool();
    }
    else
    {
        return false;
    }
}

qint32 CO::getRepairBonus(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRepairBonus";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

bool CO::canBeRepaired(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "canBeRepaired";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isBool())
    {
        return erg.toBool();
    }
    else
    {
        return false;
    }
}

qint32 CO::getCaptureBonus(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getCaptureBonus";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

void CO::activatePower()
{
    m_PowerMode = GameEnums::PowerMode_Power;
    powerFilled -= powerStars;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "activatePower";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    pInterpreter->doFunction(coID, function1, args1);
    if (GameMenue::getInstance() != nullptr)
    {
        GameMenue::getInstance()->updatePlayerinfo();
    }
    addUnitShines();
}

void CO::activateSuperpower(GameEnums::PowerMode powerMode)
{
    m_PowerMode = powerMode;
    powerFilled = 0;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "activateSuperpower";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    args1 << powerMode;
    pInterpreter->doFunction(coID, function1, args1);
    if (GameMenue::getInstance() != nullptr)
    {
        GameMenue::getInstance()->updatePlayerinfo();
    }
    addUnitShines();
}

void CO::addUnitShines()
{
    GameMap* pMap = GameMap::getInstance();
    qint32 heigth = pMap->getMapHeight();
    qint32 width = pMap->getMapWidth();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
            if (pUnit != nullptr &&
                pUnit->getOwner() == m_Owner)
            {
                pUnit->addShineTween();
            }
        }
    }
}

qint32 CO::getOffensiveBonus(Unit* pAttacker, QPoint atkPosition,Unit* pDefender,  QPoint defPosition, bool isDefender)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getOffensiveBonus";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pInterpreter->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkPosition.x();
    args1 << atkPosition.y();
    QJSValue obj2 = pInterpreter->newQObject(pDefender);
    args1 << obj2;
    args1 << defPosition.x();
    args1 << defPosition.y();
    args1 << isDefender;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 CO::getOffensiveReduction(Unit* pAttacker, QPoint atkPosition,Unit* pDefender,  QPoint defPosition, bool isDefender)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getOffensiveReduction";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pInterpreter->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkPosition.x();
    args1 << atkPosition.y();
    QJSValue obj2 = pInterpreter->newQObject(pDefender);
    args1 << obj2;
    args1 << defPosition.x();
    args1 << defPosition.y();
    args1 << isDefender;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 CO::getDeffensiveBonus(Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition, bool isDefender)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDeffensiveBonus";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pInterpreter->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkPosition.x();
    args1 << atkPosition.y();
    QJSValue obj2 = pInterpreter->newQObject(pDefender);
    args1 << obj2;
    args1 << defPosition.x();
    args1 << defPosition.y();    
    args1 << isDefender;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 CO::getDeffensiveReduction(Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition, bool isDefender)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDeffensiveReduction";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pInterpreter->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkPosition.x();
    args1 << atkPosition.y();
    QJSValue obj2 = pInterpreter->newQObject(pDefender);
    args1 << obj2;
    args1 << defPosition.x();
    args1 << defPosition.y();
    args1 << isDefender;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

float CO::getDamageReduction(float damage, Unit* pAttacker, QPoint atkPosition, qint32 attackerBaseHp,
                             Unit* pDefender, QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDamageReduction";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    args1 << damage;
    QJSValue obj1 = pInterpreter->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkPosition.x();
    args1 << atkPosition.y();
    args1 << attackerBaseHp;
    QJSValue obj2 = pInterpreter->newQObject(pDefender);
    args1 << obj2;
    args1 << defPosition.x();
    args1 << defPosition.y();
    args1 << isDefender;
    args1 << luckMode;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

float CO::getTrueDamage(float damage, Unit* pAttacker, QPoint atkPosition, qint32 attackerBaseHp,
                        Unit* pDefender, QPoint defPosition, bool isDefender)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTrueDamage";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    args1 << damage;
    QJSValue obj1 = pInterpreter->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkPosition.x();
    args1 << atkPosition.y();
    args1 << attackerBaseHp;
    QJSValue obj2 = pInterpreter->newQObject(pDefender);
    args1 << obj2;
    args1 << defPosition.x();
    args1 << defPosition.y();
    args1 << isDefender;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

GameEnums::PowerMode CO::getPowerMode() const
{
    return m_PowerMode;
}

void CO::setPowerMode(const GameEnums::PowerMode &PowerMode)
{
    m_PowerMode = PowerMode;
}

void CO::gainPowerstar(qint32 fundsDamage, QPoint position)
{
    double powerGain = fundsDamage;
    // reduce power meter filling based on power usages
    powerGain *= 1 / (1.0 + powerUsed * 0.1);
    if (!inCORange(position, nullptr))
    {
        // reduce power meter gain when not in co range
        powerGain /= 2.0;
    }
    if (m_PowerMode == GameEnums::PowerMode_Off)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "gainPowerstar";
        QJSValueList args1;
        QJSValue obj1 = pInterpreter->newQObject(this);
        args1 << obj1;
        args1 << powerGain;
        args1 << position.x();
        args1 << position.y();
        pInterpreter->doFunction(coID, function1, args1);
    }
}

Unit* CO::getCOUnit()
{
    return m_pCOUnit;
}

QStringList CO::getActionModifierList(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getActionModifierList";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    return erg.toVariant().toStringList();
}

qint32 CO::getCORange()
{    
    qint32 ret = 0;
    if (m_PowerMode == GameEnums::PowerMode_Off)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "getCOUnitRange";
        QJSValueList args1;
        QJSValue obj1 = pInterpreter->newQObject(this);
        args1 << obj1;
        QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
        if (erg.isNumber())
        {
            ret += erg.toNumber();
        }
        if (canUsePower())
        {
            ret += 1;
        }
        if (canUseSuperpower())
        {
            ret += 1;
        }
    }
    return ret;
}

bool CO::inCORange(QPoint position, Unit* pUnit)
{
    if (position.x() < 0 && position.y() < 0)
    {
        // return true for intel information
        return true;
    }
    else if (m_pCOUnit != nullptr)
    {
        if ((qAbs(m_pCOUnit->getX() - position.x()) + qAbs(m_pCOUnit->getY() - position.y())) <= getCORange())
        {
            return true;
        }
        else if (pUnit == m_pCOUnit)
        {
            return true;
        }
    }
    return false;
}

qint32 CO::getIncomeReduction(Building* pBuilding, qint32 income)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getIncomeReduction";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pInterpreter->newQObject(pBuilding);
    args1 << obj1;
    args1 << income;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 CO::getBonusIncome(Building* pBuilding, qint32 income)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBonusIncome";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pInterpreter->newQObject(pBuilding);
    args1 << obj1;
    args1 << income;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

bool CO::getPerfectVision()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getPerfectVision";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isBool())
    {
        return erg.toBool();
    }
    else
    {
        return false;
    }
}

GameEnums::PowerMode CO::getAiUsePower(double powerSurplus, qint32 unitCount, qint32 repairUnits,
                                   qint32 indirectUnits, qint32 directUnits, qint32 enemyUnits,
                                   GameEnums::AiTurnMode turnMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getAiUsePower";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    args1 << powerSurplus;
    args1 << unitCount;
    args1 << repairUnits;
    args1 << indirectUnits;
    args1 << directUnits;
    args1 << enemyUnits;
    args1 << turnMode;
    QJSValue erg = pInterpreter->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return static_cast<GameEnums::PowerMode>(erg.toInt());
    }
    else
    {
        return GameEnums::PowerMode_Unknown;
    }
}

void CO::loadCOMusic()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "loadCOMusic";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    pInterpreter->doFunction(coID, function1, args1);
}

GameAnimationDialog* CO::createPowerSentence()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QStringList sentences = pInterpreter->doFunction(coID, "getPowerSentences").toVariant().toStringList();
    QString sentence = sentences[Mainapp::randInt(0, sentences.size() - 1)];

    GameAnimationDialog* pGameAnimationDialog = GameAnimationFactory::createGameAnimationDialog(sentence, coID, GameEnums::COMood_Normal, m_Owner->getColor());
    pGameAnimationDialog->setFinishDelay(500);

    return pGameAnimationDialog;
}

QString CO::getDefeatSentence()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QStringList sentences = pInterpreter->doFunction(coID, "getDefeatSentences").toVariant().toStringList();
    QString sentence = "";
    if (sentences.size() > 0)
    {
        sentence = sentences[Mainapp::randInt(0, sentences.size() - 1)];
    }
    return sentence;
}

QString CO::getVictorySentence()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QStringList sentences = pInterpreter->doFunction(coID, "getVictorySentences").toVariant().toStringList();
    QString sentence = "";
    if (sentences.size() > 0)
    {
        sentence = sentences[Mainapp::randInt(0, sentences.size() - 1)];
    }
    return sentence;
}

GameAnimationPower* CO::createPowerScreen(GameEnums::PowerMode powerMode)
{
    GameAnimationPower* pGameAnimationPower = GameAnimationFactory::createAnimationPower(m_Owner->getColor(), powerMode, coID);
    return pGameAnimationPower;
}

bool CO::getIsCO0()
{
    if (this == m_Owner->getCO(0) || GameMenue::getInstance() == nullptr)
    {
        return true;
    }
    return false;
}

void CO::postBattleActions(Unit* pAttacker, float atkDamage, Unit* pDefender, bool gotAttacked)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "postBattleActions";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pInterpreter->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkDamage;
    QJSValue obj2 = pInterpreter->newQObject(pDefender);
    args1 << obj2;
    args1 << gotAttacked;
    pInterpreter->doFunction(coID, function1, args1);
}

void CO::serializeObject(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << coID;
    pStream << powerStars;
    pStream << superpowerStars;
    pStream << powerFilled;
    pStream << static_cast<qint32>(m_PowerMode);
    m_Variables.serializeObject(pStream);
    pStream << powerUsed;
}

void CO::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> coID;
    pStream >> powerStars;
    pStream >> superpowerStars;

    if (version > 2)
    {
        pStream >> powerFilled;
    }
    else
    {
        float power = 0.0f;
        pStream >> power;
        powerFilled = static_cast<double>(power);
    }

    qint32 value = 0;
    pStream >> value;
    m_PowerMode = static_cast<GameEnums::PowerMode>(value);
    if (version > 1)
    {
        m_Variables.deserializeObject(pStream);
    }
    if (version > 2)
    {
        pStream >> powerUsed;
    }
    init();
}
