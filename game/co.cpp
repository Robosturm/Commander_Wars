#include "co.h"

#include "coreengine/mainapp.h"

#include "game/unit.h"

#include "menue/gamemenue.h"

#include "game/gameanimationfactory.h"
#include "game/gameanimationdialog.h"
#include "game/gameanimationpower.h"

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
        Mainapp* pApp = Mainapp::getInstance();
        QString function1 = "init";
        QJSValueList args1;
        QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
        args1 << obj1;
        QJSValue erg = pApp->getInterpreter()->doFunction(this->coID, function1, args1);
    }
}

void CO::setCOUnit(Unit* pUnit)
{
    if (pUnit == nullptr && m_pCOUnit != nullptr && GameMenue::getInstance() != nullptr)
    {
        Mainapp* pApp = Mainapp::getInstance();
        QString function1 = "onCOUnitLost";
        QJSValueList args1;
        QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
        args1 << obj1;
        QJSValue erg = pApp->getInterpreter()->doFunction(this->coID, function1, args1);
    }
    m_pCOUnit = pUnit;
}

QString CO::getCoID() const
{
    return coID;
}

float CO::getPowerFilled() const
{
    return powerFilled;
}

void CO::startOfTurn()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "startOfTurn";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    pApp->getInterpreter()->doFunction(coID, function1, args1);
}

void CO::setPowerFilled(const float &value)
{
    if (!GameMap::getInstance()->getGameRules()->getNoPower())
    {
        if (GameMenue::getInstance() != nullptr)
        {
            Mainapp* pApp = Mainapp::getInstance();
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getTerrainDefenseModifier";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getFirstStrike";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue obj3 = pApp->getInterpreter()->newQObject(pAttacker);
    args1 << obj3;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getEnemyTerrainDefenseModifier";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getVisionrangeModifier";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 CO::getMovementpointModifier(Unit* pUnit, QPoint position)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getMovementpointModifier";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getFirerangeModifier";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getHpHidden";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getAttackHpBonus";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getBonusLuck";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getBonusMisfortune";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getAdditionalBuildingActions";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pBuilding);
    args1 << obj1;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getFuelCostModifier";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    args1 << costs;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getMovementcostModifier";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getMovementFuelCostModifier";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj2;
    args1 << fuelCost;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "buildedUnit";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
}

qint32 CO::getCostModifier(QString id, qint32 baseCost)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getCostModifier";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    args1 << id;
    args1 << baseCost;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getCOArmy";
    QJSValueList args1;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getCanMoveAndFire";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getRepairBonus";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "canBeRepaired";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getCaptureBonus";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "activatePower";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    pApp->getInterpreter()->doFunction(coID, function1, args1);
    if (GameMenue::getInstance() != nullptr)
    {
        GameMenue::getInstance()->updatePlayerinfo();
    }
}

void CO::activateSuperpower(GameEnums::PowerMode powerMode)
{
    m_PowerMode = powerMode;
    powerFilled = 0;
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "activateSuperpower";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    args1 << powerMode;
    pApp->getInterpreter()->doFunction(coID, function1, args1);
    if (GameMenue::getInstance() != nullptr)
    {
        GameMenue::getInstance()->updatePlayerinfo();
    }
}

qint32 CO::getOffensiveBonus(Unit* pAttacker, QPoint atkPosition,Unit* pDefender,  QPoint defPosition, bool isDefender)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getOffensiveBonus";
    QJSValueList args1;
    QJSValue obj3 = pApp->getInterpreter()->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkPosition.x();
    args1 << atkPosition.y();
    QJSValue obj2 = pApp->getInterpreter()->newQObject(pDefender);
    args1 << obj2;
    args1 << defPosition.x();
    args1 << defPosition.y();
    args1 << isDefender;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getOffensiveReduction";
    QJSValueList args1;
    QJSValue obj3 = pApp->getInterpreter()->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkPosition.x();
    args1 << atkPosition.y();
    QJSValue obj2 = pApp->getInterpreter()->newQObject(pDefender);
    args1 << obj2;
    args1 << defPosition.x();
    args1 << defPosition.y();
    args1 << isDefender;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getDeffensiveBonus";
    QJSValueList args1;
    QJSValue obj3 = pApp->getInterpreter()->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkPosition.x();
    args1 << atkPosition.y();
    QJSValue obj2 = pApp->getInterpreter()->newQObject(pDefender);
    args1 << obj2;
    args1 << defPosition.x();
    args1 << defPosition.y();    
    args1 << isDefender;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getDeffensiveReduction";
    QJSValueList args1;
    QJSValue obj3 = pApp->getInterpreter()->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkPosition.x();
    args1 << atkPosition.y();
    QJSValue obj2 = pApp->getInterpreter()->newQObject(pDefender);
    args1 << obj2;
    args1 << defPosition.x();
    args1 << defPosition.y();
    args1 << isDefender;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getDamageReduction";
    QJSValueList args1;
    QJSValue obj3 = pApp->getInterpreter()->newQObject(this);
    args1 << obj3;
    args1 << damage;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkPosition.x();
    args1 << atkPosition.y();
    args1 << attackerBaseHp;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(pDefender);
    args1 << obj2;
    args1 << defPosition.x();
    args1 << defPosition.y();
    args1 << isDefender;
    args1 << luckMode;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getTrueDamage";
    QJSValueList args1;
    QJSValue obj3 = pApp->getInterpreter()->newQObject(this);
    args1 << obj3;
    args1 << damage;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkPosition.x();
    args1 << atkPosition.y();
    args1 << attackerBaseHp;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(pDefender);
    args1 << obj2;
    args1 << defPosition.x();
    args1 << defPosition.y();
    args1 << isDefender;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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

void CO::gainPowerstar(qint32 fondsDamage, QPoint position)
{
    if (!inCORange(position, nullptr))
    {
        // reduce power meter gain when not in co range
        fondsDamage /= 2;
    }
    if (m_PowerMode == GameEnums::PowerMode_Off)
    {
        Mainapp* pApp = Mainapp::getInstance();
        QString function1 = "gainPowerstar";
        QJSValueList args1;
        QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
        args1 << obj1;
        args1 << fondsDamage;
        args1 << position.x();
        args1 << position.y();
        pApp->getInterpreter()->doFunction(coID, function1, args1);
    }
}

Unit* CO::getCOUnit()
{
    return m_pCOUnit;
}

QStringList CO::getActionModifierList(Unit* pUnit)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getActionModifierList";
    QJSValueList args1;
    QJSValue obj3 = pApp->getInterpreter()->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
    return erg.toVariant().toStringList();
}

qint32 CO::getCORange()
{    
    qint32 ret = 0;
    if (m_PowerMode == GameEnums::PowerMode_Off)
    {
        Mainapp* pApp = Mainapp::getInstance();
        QString function1 = "getCOUnitRange";
        QJSValueList args1;
        QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
        QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getIncomeReduction";
    QJSValueList args1;
    QJSValue obj3 = pApp->getInterpreter()->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pBuilding);
    args1 << obj1;
    args1 << income;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getBonusIncome";
    QJSValueList args1;
    QJSValue obj3 = pApp->getInterpreter()->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pBuilding);
    args1 << obj1;
    args1 << income;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getPerfectVision";
    QJSValueList args1;
    QJSValue obj3 = pApp->getInterpreter()->newQObject(this);
    args1 << obj3;
    QJSValue erg = pApp->getInterpreter()->doFunction(coID, function1, args1);
    if (erg.isBool())
    {
        return erg.toBool();
    }
    else
    {
        return false;
    }
}

void CO::loadCOMusic()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "loadCOMusic";
    QJSValueList args1;
    QJSValue obj3 = pApp->getInterpreter()->newQObject(this);
    args1 << obj3;
    pApp->getInterpreter()->doFunction(coID, function1, args1);
}

GameAnimationDialog* CO::createPowerSentence()
{
    Mainapp* pApp = Mainapp::getInstance();
    QStringList sentences = pApp->getInterpreter()->doFunction(coID, "getPowerSentences").toVariant().toStringList();
    QString sentence = sentences[Mainapp::randInt(0, sentences.size() - 1)];

    GameAnimationDialog* pGameAnimationDialog = GameAnimationFactory::createGameAnimationDialog(sentence, coID, GameEnums::COMood_Normal, m_Owner->getColor());
    pGameAnimationDialog->setFinishDelay(500);

    return pGameAnimationDialog;
}

QString CO::getDefeatSentence()
{
    Mainapp* pApp = Mainapp::getInstance();
    QStringList sentences = pApp->getInterpreter()->doFunction(coID, "getDefeatSentences").toVariant().toStringList();
    QString sentence = sentences[Mainapp::randInt(0, sentences.size() - 1)];
    return sentence;
}

QString CO::getVictorySentence()
{
    Mainapp* pApp = Mainapp::getInstance();
    QStringList sentences = pApp->getInterpreter()->doFunction(coID, "getVictorySentences").toVariant().toStringList();
    QString sentence = sentences[Mainapp::randInt(0, sentences.size() - 1)];
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "postBattleActions";
    QJSValueList args1;
    QJSValue obj3 = pApp->getInterpreter()->newQObject(this);
    args1 << obj3;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pAttacker);
    args1 << obj1;
    args1 << atkDamage;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(pDefender);
    args1 << obj2;
    args1 << gotAttacked;
    pApp->getInterpreter()->doFunction(coID, function1, args1);
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
}

void CO::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> coID;
    pStream >> powerStars;
    pStream >> superpowerStars;
    pStream >> powerFilled;
    qint32 value = 0;
    pStream >> value;
    m_PowerMode = static_cast<GameEnums::PowerMode>(value);
    if (version > 1)
    {
        m_Variables.deserializeObject(pStream);
    }
    init();
}
