#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"
#include "coreengine/globalutils.h"
#include "coreengine/userdata.h"

#include "game/player.h"
#include "game/unit.h"
#include "game/co.h"
#include "game/gameanimation/gameanimationfactory.h"
#include "game/gameanimation/gameanimationdialog.h"
#include "game/gameanimation/gameanimationpower.h"
#include "game/gameaction.h"

#include "menue/gamemenue.h"

#include "resource_management/cospritemanager.h"

#include "spritingsupport/spritecreator.h"

CO::CO(QString coID, Player* owner)
    : m_Owner(owner),
      m_coID(coID)
{
    setObjectName("CO");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_perkList.append(coID);
    m_perkList.append("TAGPOWER");
    init();
}

void CO::init()
{
    if (!m_coID.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "init";
        QJSValueList args1;
        QJSValue obj1 = pInterpreter->newQObject(this);
        args1 << obj1;
        pInterpreter->doFunction(this->m_coID, function1, args1);
    }
}

bool CO::isValid()
{
    return COSpriteManager::getInstance()->exists(m_coID);
}

float CO::getUnitBuildValue(QString unitID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getUnitBuildValue";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    args1 << unitID;

    float ergValue = 0.0f;
    QJSValue erg = pInterpreter->doFunction(m_coID, function1, args1);
    if (erg.isNumber())
    {
        ergValue += erg.toNumber();
    }
    return ergValue;
}

void CO::setCOUnit(Unit* pUnit)
{
    spGameMenue pMenu = GameMenue::getInstance();
    if ((pUnit == nullptr) &&
        (m_pCOUnit.get() != nullptr) &&
        (pMenu.get() != nullptr))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "onCOUnitLost";
        QJSValueList args1;
        QJSValue obj1 = pInterpreter->newQObject(this);
        args1 << obj1;
        for (qint32 i = 0; i < m_perkList.size(); ++i)
        {
            pInterpreter->doFunction(m_perkList[i], function1, args1);
        }
    }
    m_pCOUnit = pUnit;
}

QString CO::getCoID() const
{
    return m_coID;
}

double CO::getPowerFilled() const
{
    return m_powerFilled;
}

void CO::startOfTurn()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "startOfTurn";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        pInterpreter->doFunction(m_perkList[i], function1, args1);
    }
}

void CO::setPowerFilled(const double &value)
{
    if (!GameMap::getInstance()->getGameRules()->getNoPower())
    {
        float currentValue = m_powerFilled;
        m_powerFilled = value;
        if (!m_powerCharging)
        {
            limitPowerbar(currentValue);
        }
    }
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr)
    {
        pMenu->updatePlayerinfo();
    }
}
void CO::limitPowerbar(float previousValue)
{
    if (m_powerFilled > m_powerStars + m_superpowerStars)
    {
        m_powerFilled = m_powerStars + m_superpowerStars;
    }
    else if (m_powerFilled < 0)
    {
        m_powerFilled = 0;
    }
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr)
    {
        Mainapp* pApp = Mainapp::getInstance();
        if (previousValue < m_powerStars && m_powerFilled >= m_powerStars)
        {
            pApp->getAudioThread()->playSound("powerready.wav");
        }
        else if (previousValue < m_powerStars + m_superpowerStars && m_powerFilled >= m_powerStars + m_superpowerStars)
        {
            pApp->getAudioThread()->playSound("superpowerready.wav");
        }
    }
}

void CO::addPowerFilled(const double &value)
{
    setPowerFilled(value + m_powerFilled);
}

qint32 CO::getSuperpowerStars() const
{
    return m_superpowerStars;
}

void CO::setSuperpowerStars(const qint32 &value)
{        
    m_superpowerStars = value;
}

bool CO::canUseSuperpower() const
{
    if ((m_powerFilled >= m_powerStars + m_superpowerStars) &&
        (m_superpowerStars > 0) &&
        (m_PowerMode == GameEnums::PowerMode_Off))
    {
        return true;
    }
    return false;
}

bool CO::canUsePower() const
{
    if ((m_powerFilled >= m_powerStars) &&
        (m_powerStars > 0) &&
        (m_PowerMode == GameEnums::PowerMode_Off))
    {
        return true;
    }
    return false;
}

qint32 CO::getPowerStars() const
{
    return m_powerStars;
}

void CO::setPowerStars(const qint32 &value)
{
    m_powerStars = value;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isBool() && erg.toBool() == true)
        {
            return true;
        }
    }
    return false;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
}

QString CO::getCOName()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getName";
    QJSValue erg = pInterpreter->doFunction(m_coID, function1);
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
    QStringList ret;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        ret.append(erg.toVariant().toStringList());
    }
    return ret;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isBool() && erg.toBool())
        {
            return true;
        }
    }
    return false;
}

bool CO::getPerfectHpView(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getPerfectHpView";
    QJSValueList args1;
    QJSValue obj2 = pInterpreter->newQObject(this);
    args1 << obj2;
    QJSValue obj1 = pInterpreter->newQObject(pUnit);
    args1 << obj1;
    args1 << position.x();
    args1 << position.y();
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isBool() && erg.toBool())
        {
            return true;
        }
    }
    return false;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    QString ret;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isString())
        {
            ret += erg.toString() + ",";
        }
    }
    return ret;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        pInterpreter->doFunction(m_perkList[i], function1, args1);
    }
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
}

QString CO::getCOArmy()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getCOArmy";
    QJSValueList args1;
    QJSValue erg = pInterpreter->doFunction(m_coID, function1, args1);
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
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isBool() && erg.toBool())
        {
            return true;
        }
    }
    return false;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isBool() && !erg.toBool())
        {
            return false;
        }
    }
    return true;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
}

void CO::activatePower()
{
    ++m_powerUsed;
    m_coRangeEnabled = false;
    m_PowerMode = GameEnums::PowerMode_Power;
    m_powerFilled -= m_powerStars;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "activatePower";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        pInterpreter->doFunction(m_perkList[i], function1, args1);
    }
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr)
    {
        pMenu->updatePlayerinfo();
    }
    addUnitShines();
}

void CO::activateSuperpower(GameEnums::PowerMode powerMode)
{
    ++m_powerUsed;
    m_coRangeEnabled = false;
    m_PowerMode = powerMode;
    m_powerFilled = 0;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "activateSuperpower";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    args1 << powerMode;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        pInterpreter->doFunction(m_perkList[i], function1, args1);
    }
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr)
    {
        pMenu->updatePlayerinfo();
    }
    addUnitShines();
}

void CO::addUnitShines()
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    float ergValue = 0.0f;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toNumber();
        }
    }
    return ergValue;
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
    float ergValue = 0.0f;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toNumber();
        }
    }
    return ergValue;
}

GameEnums::PowerMode CO::getPowerMode() const
{
    return m_PowerMode;
}

void CO::setPowerMode(const GameEnums::PowerMode &PowerMode)
{
    m_PowerMode = PowerMode;
}

void CO::gainPowerstar(qint32 fundsDamage, QPoint position, qint32 hpDamage, bool defender, bool counterAttack)
{
    if (m_PowerMode == GameEnums::PowerMode_Off)
    {
        float currentValue = m_powerFilled;
        m_powerCharging = true;
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args1;
        QJSValue obj1 = pInterpreter->newQObject(this);
        args1 << obj1;
        args1 << fundsDamage;
        args1 << position.x();
        args1 << position.y();
        args1 << hpDamage;
        args1 << defender;
        args1 << counterAttack;
        // call co bonus
        QString function1 = "gainPowerstar";
        for (qint32 i = 0; i < m_perkList.size(); ++i)
        {
            pInterpreter->doFunction(m_perkList[i], function1, args1);
        }
        m_powerCharging = false;
        limitPowerbar(currentValue);
    }
}

Unit* CO::getCOUnit()
{
    return m_pCOUnit.get();
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
    QStringList ret;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        ret.append(erg.toVariant().toStringList());
    }
    return ret;
}

qint32 CO::getCORange()
{    
    qint32 ret = 0;
    if (m_coRangeEnabled)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "getCOUnitRange";
        QJSValueList args1;
        QJSValue obj1 = pInterpreter->newQObject(this);
        args1 << obj1;
        for (qint32 i = 0; i < m_perkList.size(); ++i)
        {
            QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
            if (erg.isNumber())
            {
                ret += erg.toInt();
            }
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
    if (position.x() == -2 && position.y() == -2)
    {
        // return false for intel information
        return false;
    }
    else if (position.x() < 0 && position.y() < 0)
    {
        // return true for intel information
        return true;
    }
    else if (m_pCOUnit.get() != nullptr)
    {
        if ((qAbs(m_pCOUnit->Unit::getX() - position.x()) + qAbs(m_pCOUnit->Unit::getY() - position.y())) <= getCORange())
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
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
    qint32 ergValue = 0;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isNumber())
        {
            ergValue += erg.toInt();
        }
    }
    return ergValue;
}

bool CO::getPerfectVision()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getPerfectVision";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isBool() && erg.toBool())
        {
            return true;
        }
    }
    return false;
}

bool CO::getWeatherImmune()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getWeatherImmune";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        QJSValue erg = pInterpreter->doFunction(m_perkList[i], function1, args1);
        if (erg.isBool() && erg.toBool())
        {
            return true;
        }
    }
    return false;
}

void CO::postAction(GameAction* pAction)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "postAction";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    QJSValue obj2 = pInterpreter->newQObject(pAction);
    args1 << obj2;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        pInterpreter->doFunction(m_perkList[i], function1, args1);
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
    QJSValue erg = pInterpreter->doFunction(m_coID, function1, args1);
    if (erg.isNumber())
    {
        return static_cast<GameEnums::PowerMode>(erg.toInt());
    }
    else
    {
        return GameEnums::PowerMode_Unknown;
    }
}

float CO::getAiCoUnitBonus(Unit* pUnit, bool & valid)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    float value = 0;
    valid = false;
    QString function1 = "getAiCoUnitBonus";
    if (pInterpreter->exists(m_coID, function1))
    {
        valid = true;
        QJSValueList args;
        QJSValue obj = pInterpreter->newQObject(this);
        args << obj;
        QJSValue obj1 = pInterpreter->newQObject(pUnit);
        args << obj1;
        QJSValue erg = pInterpreter->doFunction(m_coID, function1, args);
        if (erg.isNumber())
        {
            value = erg.toNumber();
        }
        if (value > MAX_CO_UNIT_VALUE)
        {
            value = MAX_CO_UNIT_VALUE;
        }
        else if (value < -MAX_CO_UNIT_VALUE)
        {
            value = -MAX_CO_UNIT_VALUE;
        }
    }
    return value;
}

QStringList CO::getPerkList()
{
    QStringList ret = m_perkList;
    ret.removeAll(m_coID);
    ret.removeAll("TAGPOWER");
    return ret;
}

void CO::setPerkList(QStringList perks)
{
    m_perkList.clear();
    m_perkList.append(m_coID);
    m_perkList.append("TAGPOWER");
    m_perkList.append(perks);
}

void CO::addPerk(QString perk)
{
    if (!m_perkList.contains(perk))
    {
        m_perkList.append(perk);
    }
}

void CO::removePerk(QString perk)
{
    m_perkList.removeAll(perk);
}

qint32 CO::getPowerUsed() const
{
    return m_powerUsed;
}

void CO::setPowerUsed(const qint32 &value)
{
    m_powerUsed = value;
}

void CO::loadCOMusic()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "loadCOMusic";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(this);
    args1 << obj3;
    pInterpreter->doFunction(m_coID, function1, args1);
}

GameAnimationDialog* CO::createPowerSentence()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QStringList sentences = pInterpreter->doFunction(m_coID, "getPowerSentences", args).toVariant().toStringList();
    QString sentence = sentences[GlobalUtils::randInt(0, sentences.size() - 1)];

    GameAnimationDialog* pGameAnimationDialog = GameAnimationFactory::createGameAnimationDialog(sentence, m_coID, GameEnums::COMood_Normal, m_Owner->getColor());
    pGameAnimationDialog->setFinishDelay(500);

    return pGameAnimationDialog;
}

QString CO::getDefeatSentence()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QStringList sentences = pInterpreter->doFunction(m_coID, "getDefeatSentences", args).toVariant().toStringList();
    QString sentence = "";
    if (sentences.size() > 0)
    {
        sentence = sentences[GlobalUtils::randInt(0, sentences.size() - 1)];
    }
    return sentence;
}

QString CO::getVictorySentence()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QStringList sentences = pInterpreter->doFunction(m_coID, "getVictorySentences", args).toVariant().toStringList();
    QString sentence = "";
    if (sentences.size() > 0)
    {
        sentence = sentences[GlobalUtils::randInt(0, sentences.size() - 1)];
    }
    return sentence;
}

GameAnimationPower* CO::createPowerScreen(GameEnums::PowerMode powerMode, quint32 frameTime)
{
    GameAnimationPower* pGameAnimationPower = GameAnimationFactory::createAnimationPower(m_Owner->getColor(), powerMode, this, frameTime);
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

QString CO::getBio()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString ret;
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QJSValue value = pInterpreter->doFunction(m_coID, "getBio", args);
    if (value.isString())
    {
        ret = value.toString();
    }
    return ret;
}

QString CO::getLongBio()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString ret;
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QJSValue value = pInterpreter->doFunction(m_coID, "getLongBio", args);
    if (value.isString())
    {
        ret = value.toString();
    }
    return ret;
}

QString CO::getHits()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString ret;
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QJSValue value = pInterpreter->doFunction(m_coID, "getHits", args);
    if (value.isString())
    {
        ret = value.toString();
    }
    return ret;
}

QString CO::getMiss()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString ret;
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QJSValue value = pInterpreter->doFunction(m_coID, "getMiss", args);
    if (value.isString())
    {
        ret = value.toString();
    }
    return ret;
}

QString CO::getCODescription()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString ret;
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QJSValue value = pInterpreter->doFunction(m_coID, "getCODescription", args);
    if (value.isString())
    {
        ret = value.toString();
    }
    return ret;
}

QString CO::getLongCODescription()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString ret;
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QJSValue value = pInterpreter->doFunction(m_coID, "getLongCODescription", args);
    if (value.isString())
    {
        ret = value.toString();
    }
    return ret;
}

QString CO::getPowerDescription()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString ret;
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QJSValue value = pInterpreter->doFunction(m_coID, "getPowerDescription", args);
    if (value.isString())
    {
        ret = value.toString();
    }
    return ret;
}

QString CO::getPowerName()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString ret;
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QJSValue value = pInterpreter->doFunction(m_coID, "getPowerName", args);
    if (value.isString())
    {
        ret = value.toString();
    }
    return ret;
}

QString CO::getSuperPowerDescription()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString ret;
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QJSValue value = pInterpreter->doFunction(m_coID, "getSuperPowerDescription", args);
    if (value.isString())
    {
        ret = value.toString();
    }
    return ret;
}

QString CO::getSuperPowerName()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString ret;
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QJSValue value = pInterpreter->doFunction(m_coID, "getSuperPowerName", args);
    if (value.isString())
    {
        ret = value.toString();
    }
    return ret;
}


void CO::postBattleActions(Unit* pAttacker, float atkDamage, Unit* pDefender, bool gotAttacked, qint32 weapon)
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
    args1 << weapon;
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        pInterpreter->doFunction(m_perkList[i], function1, args1);
    }
}

void CO::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_coID;
    pStream << m_powerStars;
    pStream << m_superpowerStars;
    pStream << m_powerFilled;
    pStream << static_cast<qint32>(m_PowerMode);
    m_Variables.serializeObject(pStream);
    pStream << m_powerUsed;
    pStream << static_cast<qint32>(m_perkList.size());
    for (qint32 i = 0; i < m_perkList.size(); ++i)
    {
        pStream << m_perkList[i];
    }
    writeCoStyleToStream(pStream);
    pStream << m_coRangeEnabled;
}

void CO::deserializeObject(QDataStream& pStream)
{
    deserializer(pStream, false);
}

void CO::deserializer(QDataStream& pStream, bool fast)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_coID;
    pStream >> m_powerStars;
    pStream >> m_superpowerStars;

    if (version > 2)
    {
        pStream >> m_powerFilled;
    }
    else
    {
        float power = 0.0f;
        pStream >> power;
        m_powerFilled = static_cast<double>(power);
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
        pStream >> m_powerUsed;
    }
    m_perkList.clear();
    if (version > 3)
    {
        qint32 size = 0;
        pStream >> size;
        m_perkList.clear();
        for (qint32 i = 0; i < size; i++)
        {
            QString perk;
            pStream >> perk;
            m_perkList.append(perk);
        }
    }
    else
    {
        m_perkList.append(m_coID);
    }
    if (!m_perkList.contains("TAGPOWER"))
    {
        m_perkList.append("TAGPOWER");
    }
    if (version > 4)
    {
        readCoStyleFromStream(pStream);
    }
    if (version > 5)
    {
        pStream >> m_coRangeEnabled;
    }
    if (!fast)
    {
        init();
    }
}

void CO::writeCoStyleToStream(QDataStream& pStream) const
{
    pStream << static_cast<qint32>(m_customCOStyles.size());
    for (qint32 i = 0; i < m_customCOStyles.size(); i++)
    {
        pStream << std::get<0>(m_customCOStyles[i]);
        pStream << std::get<1>(m_customCOStyles[i]);
        qint32 width = static_cast<qint32>(std::get<2>(m_customCOStyles[i]).width());
        pStream << width;
        for (qint32 x = 0; x < width; x++)
        {
            pStream << std::get<2>(m_customCOStyles[i]).pixel(x, 0);
            pStream << std::get<3>(m_customCOStyles[i]).pixel(x, 0);
        }
        pStream << std::get<4>(m_customCOStyles[i]);
    }
}

void CO::readCoStyleFromStream(QDataStream& pStream)
{
    qint32 size = 0;
    pStream >> size;
    m_customCOStyles.clear();
    for (qint32 i = 0; i < size; i++)
    {
        QString coid;
        QString file;
        QImage colorTable;
        QImage maskTable;
        bool useColorBox = false;
        pStream >> coid;
        pStream >> file;
        qint32 width = 0;
        pStream >> width;
        colorTable = QImage(width, 1, QImage::Format_ARGB32);
        maskTable = QImage(width, 1, QImage::Format_ARGB32);
        QRgb rgb;
        for (qint32 x = 0; x < width; x++)
        {
            pStream >> rgb;
            colorTable.setPixel(x, 0, rgb);
            pStream >> rgb;
            maskTable.setPixel(x, 0, rgb);
        }
        pStream >> useColorBox;
        colorTable.convertTo(QImage::Format_ARGB32);
        maskTable.convertTo(QImage::Format_ARGB32);
        m_customCOStyles.append(std::tuple<QString, QString, QImage, QImage, bool>(coid, file, colorTable, maskTable, useColorBox));
        loadResAnim(coid, file, colorTable, maskTable, useColorBox);
    }
}


void CO::setCoStyleFromUserdata()
{
    m_customCOStyles.clear();
    auto * style = Userdata::getInstance()->getCOStyle(m_coID);
    QString file;
    QImage colorTable;
    QImage maskTable;
    bool useColorBox = false;
    if (style != nullptr)
    {
        file = std::get<1>(*style);
        colorTable = std::get<2>(*style);
        maskTable = std::get<3>(*style);
        useColorBox = std::get<4>(*style);
    }
    else
    {
        COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
        file = pCOSpriteManager->getResAnim(m_coID + "+nrm")->getResPath();
        file.replace("+nrm.png", "");
    }
    if (m_coID != "CO_RANDOM")
    {
        loadResAnim(m_coID, file, colorTable, maskTable, useColorBox);
        m_customCOStyles.append(std::tuple<QString, QString, QImage, QImage, bool>(m_coID, file, colorTable, maskTable, useColorBox));
    }
}

void CO::setCoStyle(QString file, qint32 style)
{
    m_customCOStyles.clear();
    QImage colorTable;
    QImage maskTable;
    QImage baseColorTable;
    baseColorTable.load(file + "+table.png");
    bool useColorBox = false;
    colorTable = baseColorTable.copy(0, 0, baseColorTable.width(), 1);
    maskTable = baseColorTable.copy(0, style, baseColorTable.width(), 1);
    loadResAnim(m_coID, file, colorTable, maskTable, useColorBox);
    m_customCOStyles.append(std::tuple<QString, QString, QImage, QImage, bool>(m_coID, file, colorTable, maskTable, useColorBox));

}

QString CO::getActiveCoStyle()
{
    if (m_customCOStyles.size() > 0)
    {
        return std::get<1>(m_customCOStyles[0]);
    }
    return "";
}

void CO::loadResAnim(QString coid, QString file, QImage colorTable, QImage maskTable, bool useColorBox)
{
    Console::print("Loading sprites for CO " + coid, Console::eDEBUG);
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    colorTable.convertTo(QImage::Format_ARGB32);
    maskTable.convertTo(QImage::Format_ARGB32);
    QString coidLower = coid.toLower();
    QStringList filenameList = file.split("/");
    QString filename = filenameList[filenameList.size() - 1];
    oxygine::spResAnim pAnim = pCOSpriteManager->oxygine::Resources::getResAnim(filename + "+face", oxygine::error_policy::ep_ignore_error);
    oxygine::spResAnim pCOAnim = nullptr;
    if (pAnim.get() != nullptr)
    {
        pCOAnim = SpriteCreator::createAnim(file + "+face.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
        if (pCOAnim.get() != nullptr)
        {
            m_Ressources.append(std::tuple<QString, oxygine::spResAnim>(coidLower + "+face", pCOAnim));
        }
    }
    pAnim = pCOSpriteManager->oxygine::Resources::getResAnim(filename + "+info", oxygine::error_policy::ep_ignore_error);
    pCOAnim = nullptr;
    if (pAnim.get() != nullptr)
    {
        oxygine::spResAnim pCOAnim = SpriteCreator::createAnim(file + "+info.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
        if (pCOAnim.get() != nullptr)
        {
            m_Ressources.append(std::tuple<QString, oxygine::spResAnim>(coidLower + "+info", pCOAnim));
        }
    }
    pAnim = pCOSpriteManager->oxygine::Resources::getResAnim(filename + "+nrm", oxygine::error_policy::ep_ignore_error);
    pCOAnim = nullptr;
    if (pAnim.get() != nullptr)
    {
        oxygine::spResAnim pCOAnim = SpriteCreator::createAnim(file + "+nrm.png", colorTable, maskTable, useColorBox, pAnim->getColumns(), pAnim->getRows(), pAnim->getScaleFactor());
        if (pCOAnim.get() != nullptr)
        {
            m_Ressources.append(std::tuple<QString, oxygine::spResAnim>(coidLower + "+nrm", pCOAnim));
        }
    }
    pCOAnim = nullptr;
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr)
    {
        pMenu->updatePlayerinfo();
    }
}

bool CO::getCoRangeEnabled() const
{
    return m_coRangeEnabled;
}

void CO::setCoRangeEnabled(bool coRangeEnabled)
{
    m_coRangeEnabled = coRangeEnabled;
}

oxygine::ResAnim* CO::getResAnim(QString id, oxygine::error_policy ep) const
{
    for (qint32 i = 0; i < m_Ressources.size(); i++)
    {
        if (id.toLower() == std::get<0>(m_Ressources[i]).toLower())
        {
            return std::get<1>(m_Ressources[i]).get();
        }
    }
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    return pCOSpriteManager->getResAnim(id, ep);
}
