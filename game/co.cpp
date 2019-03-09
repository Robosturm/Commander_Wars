#include "co.h"

#include "coreengine/mainapp.h"

#include "game/unit.h"

#include "menue/gamemenue.h"

CO::CO(QString coID, Player* owner)
    : m_Owner(owner),
      coID(coID)
{
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

void CO::setCOUnit(spUnit pUnit)
{
    m_COUnit = pUnit;
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
    powerFilled = value;
    if (powerFilled > powerStars + superpowerStars)
    {
        powerFilled = powerStars + superpowerStars;
    }
    else if (powerFilled < 0)
    {
        powerFilled = 0;
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
        (m_PowerMode == GameEnums::PowerMode_Off))
    {
        return true;
    }
    return false;
}

bool CO::canUsePower() const
{
    if ((powerFilled >= powerStars) &&
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

qint32 CO::getMovementPointModifier(Unit* pUnit)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getMovementPointModifier";
    QJSValueList args1;
    QJSValue obj2 = pApp->getInterpreter()->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;
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
void CO::activateSuperpower()
{
    m_PowerMode = GameEnums::PowerMode_Superpower;
    powerFilled = 0;
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "activateSuperpower";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
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

qint32 CO::getDeffensiveBonus(Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition)
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
    if (!inCORange(position))
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
    return m_COUnit.get();
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

bool CO::inCORange(QPoint position)
{
    if (m_COUnit.get() != nullptr)
    {
        if ((qAbs(m_COUnit->getX() - position.x()) + qAbs(m_COUnit->getY() - position.y())) <= getCORange())
        {
            return true;
        }
    }
    return false;
}

void CO::serialize(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << coID;
    pStream << powerStars;
    pStream << superpowerStars;
    pStream << powerFilled;
    pStream << static_cast<qint32>(m_PowerMode);
}

void CO::deserialize(QDataStream& pStream)
{
    pStream >> coID;
    pStream >> powerStars;
    pStream >> superpowerStars;
    pStream >> powerFilled;
    qint32 value = 0;
    pStream << value;
    m_PowerMode = static_cast<GameEnums::PowerMode>(value);
    init();
}
