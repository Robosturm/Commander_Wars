#include "co.h"

#include "coreengine/mainapp.h"

#include "game/unit.h"

CO::CO(QString coID)
    : coID(coID)
{

}

QString CO::getCoID() const
{
    return coID;
}

float CO::getPowerFilled() const
{
    return powerFilled;
}

void CO::setPowerFilled(const float &value)
{
    powerFilled = value;
}

qint32 CO::getSuperpowerStars() const
{
    return superpowerStars;
}

void CO::setSuperpowerStars(const qint32 &value)
{
    superpowerStars = value;
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

qint32 CO::getMovementPointModifier(Unit* pUnit)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getMovementPointModifier";
    QJSValueList args1;
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

bool CO::getCanMoveAndFire(Unit* pUnit, QPoint position)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getCanMoveAndFire";
    QJSValueList args1;
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

void CO::activatePower()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "activatePower";
    QJSValueList args1;
    pApp->getInterpreter()->doFunction(coID, function1, args1);
}
void CO::activateSuperpower()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "activateSuperpower";
    QJSValueList args1;
    pApp->getInterpreter()->doFunction(coID, function1, args1);
}

qint32 CO::getOffensiveBonus(Unit* pAttacker, QPoint atkPosition,Unit* pDefender,  QPoint defPosition)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getOffensiveBonus";
    QJSValueList args1;
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

qint32 CO::getDeffensiveBonus(Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getDeffensiveBonus";
    QJSValueList args1;
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
