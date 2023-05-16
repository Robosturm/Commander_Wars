#include "coreengine/interpreter.h"
#include "coreengine/audiomanager.h"
#include "coreengine/globalutils.h"
#include "coreengine/userdata.h"

#include "ai/productionSystem/simpleproductionsystem.h"

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

static constexpr double DOUBLE_PRECISION = 100000000.0;

CO::CO(QString coID, Player* owner, GameMap* pMap)
    : m_pOwner(owner),
      m_coID(coID),
      m_pMap{pMap}
{
#ifdef GRAPHICSUPPORT
    setObjectName("CO");
#endif
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
        QJSValueList args1({pInterpreter->newQObject(this),
                           pInterpreter->newQObject(m_pMap)});
        pInterpreter->doFunction(m_coID, function1, args1);

        QString function2 = "getGlobalZone";
        QJSValueList args2({pInterpreter->newQObject(this),
                            pInterpreter->newQObject(m_pMap)});
        m_globalCoZone = pInterpreter->doFunction(m_coID, function2, args2).toBool();
    }
}

bool CO::isValid()
{
    return COSpriteManager::getInstance()->exists(m_coID);
}

qreal CO::getUnitBuildValue(const QString & unitID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getUnitBuildValue";
    QJSValueList args({pInterpreter->newQObject(this),
                       unitID,
                       pInterpreter->newQObject(m_pMap)});
    qreal ergValue = 0.0;
    QJSValue erg = pInterpreter->doFunction(m_coID, function1, args);
    if (erg.isNumber())
    {
        ergValue += erg.toNumber();
    }
    return ergValue;
}

bool CO::isJsFunctionEnabled(QString perk) const
{
    
    return perk != m_coID || // perks are always enabled
           m_PowerMode != GameEnums::PowerMode_Off || // d2d is active during if no power is active
           m_pMap == nullptr || // no map means no rules
           m_pMap->getGameRules()->getEnableDayToDayCoAbilities(); // or if d2d is active
}

void CO::setMenu(GameMenue *newMenu)
{
    m_pMenu = newMenu;
}

bool CO::getGlobalCoZone() const
{
    return m_globalCoZone;
}

void CO::setGlobalCoZone(bool newGlobalCoZone)
{
    m_globalCoZone = newGlobalCoZone;
}

GameMap *CO::getMap() const
{
    return m_pMap;
}

void CO::setCOUnit(Unit* pUnit)
{
    if (pUnit == nullptr &&
        m_pCOUnit.get() != nullptr &&
        m_pMenu != nullptr)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "onCOUnitLost";
        QJSValueList args({pInterpreter->newQObject(this),
                           pInterpreter->newQObject(m_pMap)});
        for (const auto & perk : qAsConst(m_perkList))
        {
            if (isJsFunctionEnabled(perk))
            {
                pInterpreter->doFunction(perk, function1, args);
            }
        }
    }
    m_pCOUnit = pUnit;
}

qreal CO::getCoGroupModifier(QStringList unitIds, SimpleProductionSystem* system)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getCoGroupModifier";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(system),
                       pInterpreter->arraytoJSValue(unitIds),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_coID, function1, args);
    qreal ret = 1.0;
    if (erg.isNumber())
    {
        ret = erg.toNumber();
    }
    return ret;
}

QString CO::getCoID() const
{
    return m_coID;
}

double CO::getPowerFilled() const
{
    return m_powerFilled;
}

void CO::endOfTurn()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "endOfTurn";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            pInterpreter->doFunction(perk, function1, args);
        }
    }
}

void CO::startOfTurn()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "startOfTurn";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            pInterpreter->doFunction(perk, function1, args);
        }
    }
}

void CO::onUnitDeath(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "onUnitDeath";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            pInterpreter->doFunction(perk, function1, args);
        }
    }
}

void CO::setPowerFilled(const double &value)
{

    if (m_pMap != nullptr &&
        !m_pMap->getGameRules()->getNoPower())
    {
        qreal currentValue = m_powerFilled;
        m_powerFilled = GlobalUtils::roundFloor(value, DOUBLE_PRECISION);
        if (!m_powerCharging)
        {
            limitPowerbar(currentValue);
        }
        CONSOLE_PRINT("Powerbar changed by: " + QString::number(value - currentValue) + " for co " + m_coID + " of player " + QString::number(m_pOwner->getPlayerID()), GameConsole::eDEBUG);
    }
    if (m_pMenu != nullptr)
    {
        m_pMenu->updatePlayerinfo();
    }
}
void CO::limitPowerbar(qreal previousValue)
{
    if (m_powerFilled > m_powerStars + m_superpowerStars)
    {
        m_powerFilled = m_powerStars + m_superpowerStars;
    }
    else if (m_powerFilled < 0)
    {
        m_powerFilled = 0;
    }
    if (m_pMenu != nullptr)
    {
        Mainapp* pApp = Mainapp::getInstance();
        if (previousValue < m_powerStars && m_powerFilled >= m_powerStars)
        {
            pApp->getAudioManager()->playSound("powerready.wav");
        }
        else if (previousValue < m_powerStars + m_superpowerStars && m_powerFilled >= m_powerStars + m_superpowerStars)
        {
            pApp->getAudioManager()->playSound("superpowerready.wav");
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
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

bool CO::getFirstStrike(Unit* pUnit, QPoint position, Unit* pAttacker, bool isDefender, QPoint attackerPosition)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getFirstStrike";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(pAttacker),
                       isDefender,
                       pInterpreter->newQObject(m_pMap),
                       attackerPosition.x(),
                       attackerPosition.y()});
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isBool() && erg.toBool() == true)
            {
                return true;
            }
        }
    }
    return false;
}

qint32 CO::getEnemyTerrainDefenseModifier(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getEnemyTerrainDefenseModifier";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getVisionrangeModifier(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getVisionrangeModifier";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
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
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pBuilding),
                       pInterpreter->newQObject(m_pMap)});
    QStringList ret;
    for (const auto & perk : qAsConst(m_perkList))
    {
        QJSValue erg = pInterpreter->doFunction(perk, function1, args);
        ret.append(erg.toVariant().toStringList());
    }
    return ret;
}

QStringList CO::getTransportUnits(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTransportUnits";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       pInterpreter->newQObject(m_pMap)});
    QStringList ret;
    for (const auto & perk : qAsConst(m_perkList))
    {
        QJSValue erg = pInterpreter->doFunction(perk, function1, args);
        ret.append(erg.toVariant().toStringList());
    }
    return ret;
}

qint32 CO::getMovementpointModifier(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMovementpointModifier";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getFirerangeModifier(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getFirerangeModifier";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getMinFirerangeModifier(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMinFirerangeModifier";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

bool CO::getHpHidden(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getHpHidden";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isBool() && erg.toBool())
            {
                return true;
            }
        }
    }
    return false;
}

bool CO::getRankInfoHidden(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRankInfoHidden";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isBool() && erg.toBool())
            {
                return true;
            }
        }
    }
    return false;
}

bool CO::getPerfectHpView(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getPerfectHpView";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isBool() && erg.toBool())
            {
                return true;
            }
        }
    }
    return false;
}

qint32 CO::getAttackHpBonus(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getAttackHpBonus";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getBonusLuck(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBonusLuck";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getEnemyBonusLuck(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getEnemyBonusLuck";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getBonusMisfortune(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBonusMisfortune";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getEnemyBonusMisfortune(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getEnemyBonusMisfortune";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qreal CO::getEnemyRepairCostModifier(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getEnemyRepairCostModifier";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       pInterpreter->newQObject(m_pMap)});
    qreal ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toNumber();
            }
        }
    }
    return ergValue;
}

qreal CO::getRepairCostModifier(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRepairCostModifier";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       pInterpreter->newQObject(m_pMap)});
    qreal ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toNumber();
            }
        }
    }
    return ergValue;
}

QStringList CO::getAdditionalBuildingActions(Building* pBuilding)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getAdditionalBuildingActions";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pBuilding),
                       pInterpreter->newQObject(m_pMap)});
    QStringList ret;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isString())
            {
                QString result = erg.toString();
                if (!result.isEmpty())
                {
                    ret.append(result.split(","));
                }
            }
            else
            {
                ret.append(erg.toVariant().toStringList());
            }
        }
    }
    return ret;
}

qint32 CO::getFuelCostModifier(Unit* pUnit, QPoint position, qint32 costs)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getFuelCostModifier";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       costs,
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getMovementcostModifier(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMovementcostModifier";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getMovementFuelCostModifier(Unit* pUnit, qint32 fuelCost)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMovementFuelCostModifier";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       fuelCost,
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

void CO::buildedUnit(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "buildedUnit";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            pInterpreter->doFunction(perk, function1, args);
        }
    }
}

qint32 CO::getCostModifier(const QString & id, qint32 baseCost, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getCostModifier";
    QJSValueList args({pInterpreter->newQObject(this),
                       id,
                       baseCost,
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getEnemyCostModifier(const QString & id, qint32 baseCost, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getEnemyCostModifier";
    QJSValueList args({pInterpreter->newQObject(this),
                       id,
                       baseCost,
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

QString CO::getCOArmy()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getCOArmy";
    QJSValueList args({pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_coID, function1, args);
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
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isBool() && erg.toBool())
            {
                return true;
            }
        }
    }
    return false;
}

qint32 CO::getRepairBonus(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRepairBonus";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

bool CO::canBeRepaired(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "canBeRepaired";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isBool() && !erg.toBool())
            {
                return false;
            }
        }
    }
    return true;
}

qint32 CO::getCaptureBonus(Unit* pUnit, QPoint position)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getCaptureBonus";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
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
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        pInterpreter->doFunction(perk, function1, args);
    }
    if (m_pMenu != nullptr)
    {
        m_pMenu->updatePlayerinfo();
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
    QJSValueList args({pInterpreter->newQObject(this),
                       powerMode,
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        pInterpreter->doFunction(perk, function1, args);
    }
    if (m_pMenu != nullptr)
    {
        m_pMenu->updatePlayerinfo();
    }
    addUnitShines();
}

void CO::addUnitShines()
{
    
    if (m_pMap != nullptr)
    {
        qint32 heigth = m_pMap->getMapHeight();
        qint32 width = m_pMap->getMapWidth();
        for (qint32 y = 0; y < heigth; y++)
        {
            for (qint32 x = 0; x < width; x++)
            {
                Unit* pUnit = m_pMap->getTerrain(x, y)->getUnit();
                if (pUnit != nullptr &&
                    pUnit->getOwner() == m_pOwner)
                {
                    pUnit->addShineTween();
                }
            }
        }
    }
}

qint32 CO::getOffensiveBonus(GameAction* pAction, Unit* pAttacker, QPoint atkPosition,Unit* pDefender,  QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getOffensiveBonus";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pAttacker),
                       atkPosition.x(),
                       atkPosition.y(),
                       pInterpreter->newQObject(pDefender),
                       defPosition.x(),
                       defPosition.y(),
                       isDefender,
                       pInterpreter->newQObject(pAction),
                       luckMode,
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getOffensiveReduction(GameAction* pAction, Unit* pAttacker, QPoint atkPosition,Unit* pDefender,  QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getOffensiveReduction";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pAttacker),
                       atkPosition.x(),
                       atkPosition.y(),
                       pInterpreter->newQObject(pDefender),
                       defPosition.x(),
                       defPosition.y(),
                       isDefender,
                       pInterpreter->newQObject(pAction),
                       luckMode,
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getCoBonus(QPoint position, Unit* pUnit, const QString & function)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       position.x(),
                       position.y(),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getDeffensiveBonus(GameAction* pAction, Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDeffensiveBonus";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pAttacker),
                       atkPosition.x(),
                       atkPosition.y(),
                       pInterpreter->newQObject(pDefender),
                       defPosition.x(),
                       defPosition.y(),
                       isDefender,
                       pInterpreter->newQObject(pAction),
                       luckMode,
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getDeffensiveReduction(GameAction* pAction, Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition, bool isAttacker, GameEnums::LuckDamageMode luckMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDeffensiveReduction";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pAttacker),
                       atkPosition.x(),
                       atkPosition.y(),
                       pInterpreter->newQObject(pDefender),
                       defPosition.x(),
                       defPosition.y(),
                       isAttacker,
                       pInterpreter->newQObject(pAction),
                       luckMode,
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qreal CO::getDamageReduction(GameAction* pAction, qreal damage, Unit* pAttacker, QPoint atkPosition, qint32 attackerBaseHp,
                             Unit* pDefender, QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDamageReduction";
    QJSValueList args({pInterpreter->newQObject(this),
                       damage,
                       pInterpreter->newQObject(pAttacker),
                       atkPosition.x(),
                       atkPosition.y(),
                       attackerBaseHp,
                       pInterpreter->newQObject(pDefender),
                       defPosition.x(),
                       defPosition.y(),
                       isDefender,
                       luckMode,
                       pInterpreter->newQObject(pAction),
                       pInterpreter->newQObject(m_pMap)});
    qreal ergValue = 0.0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toNumber();
            }
        }
    }
    return ergValue;
}

qreal CO::getTrueDamage(GameAction* pAction, qreal damage, Unit* pAttacker, QPoint atkPosition, qint32 attackerBaseHp,
                        Unit* pDefender, QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getTrueDamage";
    QJSValueList args({pInterpreter->newQObject(this),
                          damage,
                          pInterpreter->newQObject(pAttacker),
                          atkPosition.x(),
                          atkPosition.y(),
                          attackerBaseHp,
                          pInterpreter->newQObject(pDefender),
                          defPosition.x(),
                          defPosition.y(),
                          isDefender,
                          pInterpreter->newQObject(pAction),
                          luckMode,
                          pInterpreter->newQObject(m_pMap)});
    qreal ergValue = 0.0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toNumber();
            }
        }
    }
    return ergValue;
}

GameEnums::CounterAttackMode CO::canCounterAttack(GameAction* pAction, Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition, GameEnums::LuckDamageMode luckMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "canCounterAttack";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pAttacker),
                       atkPosition.x(),
                       atkPosition.y(),
                       pInterpreter->newQObject(pDefender),
                       defPosition.x(),
                       defPosition.y(),
                       pInterpreter->newQObject(pAction),
                       luckMode,
                       pInterpreter->newQObject(m_pMap)});
    auto ergValue = GameEnums::CounterAttackMode_Undefined;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                auto mode2 = static_cast<GameEnums::CounterAttackMode>(erg.toNumber());
                if (mode2 != GameEnums::CounterAttackMode_Undefined)
                {
                    ergValue = mode2;
                    if (ergValue == GameEnums::CounterAttackMode_Impossible)
                    {
                        break;
                    }
                }
            }
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
        qreal currentValue = m_powerFilled;
        m_powerCharging = true;
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args({pInterpreter->newQObject(this),
                           fundsDamage,
                           position.x(),
                           position.y(),
                           hpDamage,
                           defender,
                           counterAttack,
                           pInterpreter->newQObject(m_pMap)});
        // call co bonus
        QString function1 = "gainPowerstar";
        for (const auto & perk : qAsConst(m_perkList))
        {
            pInterpreter->doFunction(perk, function1, args);
        }
        m_powerCharging = false;
        limitPowerbar(currentValue);
    }
}

qreal CO::getStarCost()
{
    qreal starCost = 1.0;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QString function1 = "getStarCost";
    QJSValue ret = pInterpreter->doFunction(m_coID, function1, args);
    if (ret.isNumber())
    {
        starCost = ret.toNumber();
    }
    return starCost;
}

Unit* CO::getCOUnit()
{
    return m_pCOUnit.get();
}

QStringList CO::getActionModifierList(Unit* pUnit)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getActionModifierList";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pUnit),
                       pInterpreter->newQObject(m_pMap)});
    QStringList ret;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            ret.append(erg.toVariant().toStringList());
        }
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
        QJSValueList args({pInterpreter->newQObject(this),
                           pInterpreter->newQObject(m_pMap)});
        for (const auto & perk : qAsConst(m_perkList))
        {
            if (isJsFunctionEnabled(perk))
            {
                QJSValue erg = pInterpreter->doFunction(perk, function1, args);
                if (erg.isNumber())
                {
                    ret += erg.toInt();
                }
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
    else if ((position.x() < 0 && position.y() < 0) ||
             m_globalCoZone)
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
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pBuilding),
                       income,
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

qint32 CO::getPowerChargeBonus()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getPowerChargeBonus";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue + m_pOwner->getPowerChargeBonus();
}

qint32 CO::getBonusIncome(Building* pBuilding, qint32 income)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getBonusIncome";
    QJSValueList args({pInterpreter->newQObject(this),
                      pInterpreter->newQObject(pBuilding),
                      income,
                      pInterpreter->newQObject(m_pMap)});
    qint32 ergValue = 0;
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isNumber())
            {
                ergValue += erg.toInt();
            }
        }
    }
    return ergValue;
}

bool CO::getPerfectVision()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getPerfectVision";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isBool() && erg.toBool())
            {
                return true;
            }
        }
    }
    return false;
}

bool CO::getWeatherImmune()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getWeatherImmune";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            QJSValue erg = pInterpreter->doFunction(perk, function1, args);
            if (erg.isBool() && erg.toBool())
            {
                return true;
            }
        }
    }
    return false;
}

void CO::postAction(GameAction* pAction)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "postAction";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pAction),
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            pInterpreter->doFunction(perk, function1, args);
        }
    }
}

GameEnums::PowerMode CO::getAiUsePower(double powerSurplus, qint32 unitCount, qint32 repairUnits,
                                       qint32 indirectUnits, qint32 directUnits, qint32 enemyUnits,
                                       GameEnums::AiTurnMode turnMode)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getAiUsePower";
    QJSValueList args({pInterpreter->newQObject(this),
                       powerSurplus,
                       unitCount,
                       repairUnits,
                       indirectUnits,
                       directUnits,
                       enemyUnits,
                       turnMode,
                       pInterpreter->newQObject(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_coID, function1, args);
    if (erg.isNumber())
    {
        return static_cast<GameEnums::PowerMode>(erg.toInt());
    }
    else
    {
        return GameEnums::PowerMode_Unknown;
    }
}

qreal CO::getAiCoUnitBonus(Unit* pUnit, bool & valid)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    qreal value = 0;
    valid = false;
    QString function1 = "getAiCoUnitBonus";
    if (pInterpreter->exists(m_coID, function1))
    {
        valid = true;
        QJSValueList args({pInterpreter->newQObject(this),
                           pInterpreter->newQObject(pUnit),
                           pInterpreter->newQObject(m_pMap),});
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

qreal CO::getAiCoBuildRatioModifier()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    qreal value = 1.0;
    QString function1 = "getAiCoBuildRatioModifier";
    if (pInterpreter->exists(m_coID, function1))
    {
        QJSValueList args({pInterpreter->newQObject(this),
                          pInterpreter->newQObject(m_pMap)});
        QJSValue erg = pInterpreter->doFunction(m_coID, function1, args);
        if (erg.isNumber())
        {
            value = erg.toNumber();
        }
        if (value > MAX_CO_UNIT_VALUE)
        {
            value = MAX_CO_UNIT_VALUE;
        }
        else if (value < 0.0001f)
        {
            value = 0.0001f;
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

void CO::setPerkList(const QStringList & perks)
{
    m_perkList.clear();
    m_perkList.append(m_coID);
    m_perkList.append("TAGPOWER");
    m_perkList.append(perks);
}

void CO::addPerk(const QString & perk)
{
    if (!m_perkList.contains(perk))
    {
        m_perkList.append(perk);
    }
}

void CO::removePerk(const QString & perk)
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
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_coID, function1, args);
}

GameAnimationDialog* CO::createPowerSentence()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                      pInterpreter->newQObject(m_pMap)});
    QStringList sentences = pInterpreter->doFunction(m_coID, "getPowerSentences", args).toVariant().toStringList();
    QString sentence = "No sentence found.";
    if (sentences.length() > 0)
    {
        sentence = sentences[GlobalUtils::randInt(0, sentences.size() - 1)];
    }
    else
    {
        CONSOLE_PRINT("Error in co: " + m_coID + " no power sentence defined", GameConsole::eERROR);
    }
    GameAnimationDialog* pGameAnimationDialog = GameAnimationFactory::createGameAnimationDialog(m_pMap, sentence, m_coID, GameEnums::COMood_Normal, m_pOwner->getColor());
    pGameAnimationDialog->setFinishDelay(500);

    return pGameAnimationDialog;
}

QString CO::getDefeatSentence()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                      pInterpreter->newQObject(m_pMap)});
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
    QJSValueList args({pInterpreter->newQObject(this),
                      pInterpreter->newQObject(m_pMap)});
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
    GameAnimationPower* pGameAnimationPower = GameAnimationFactory::createAnimationPower(m_pMap, m_pOwner->getColor(), powerMode, this, frameTime);
    return pGameAnimationPower;
}

bool CO::getIsCO0()
{
    if (this == m_pOwner->getCO(0) || m_pMenu == nullptr)
    {
        return true;
    }
    return false;
}

QString CO::getBio()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString ret;
    QJSValueList args({pInterpreter->newQObject(this),
                      pInterpreter->newQObject(m_pMap)});
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
    QJSValueList args({pInterpreter->newQObject(this),
                      pInterpreter->newQObject(m_pMap)});
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
    QJSValueList args({pInterpreter->newQObject(this),
                      pInterpreter->newQObject(m_pMap)});
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
    QJSValueList args({pInterpreter->newQObject(this),
                      pInterpreter->newQObject(m_pMap)});
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
    QJSValueList args({pInterpreter->newQObject(this),
                      pInterpreter->newQObject(m_pMap)});
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
    QJSValueList args({pInterpreter->newQObject(this),
                      pInterpreter->newQObject(m_pMap)});
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
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
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
    QJSValueList args({pInterpreter->newQObject(this),
                      pInterpreter->newQObject(m_pMap)});
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
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
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
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue value = pInterpreter->doFunction(m_coID, "getSuperPowerName", args);
    if (value.isString())
    {
        ret = value.toString();
    }
    return ret;
}

void CO::postBattleActions(Unit* pAttacker, qreal atkDamage, Unit* pDefender, bool gotAttacked, qint32 weapon, GameAction* pAction)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "postBattleActions";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pAttacker),
                       atkDamage,
                       pInterpreter->newQObject(pDefender),
                       gotAttacked,
                       weapon,
                       pInterpreter->newQObject(pAction),
                       pInterpreter->newQObject(m_pMap)});
    for (const auto & perk : qAsConst(m_perkList))
    {
        if (isJsFunctionEnabled(perk))
        {
            pInterpreter->doFunction(perk, function1, args);
        }
    }
}


bool CO::showDefaultUnitGlobalBoost()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    bool ret = true;
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue value = pInterpreter->doFunction(m_coID, "showDefaultUnitGlobalBoost", args);
    if (value.isBool())
    {
        ret = value.toBool();
    }
    return ret;
}

qint32 CO::getCustomUnitGlobalBoostCount()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    qint32 ret = 0;
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue value = pInterpreter->doFunction(m_coID, "getCustomUnitGlobalBoostCount", args);
    if (value.isNumber())
    {
        ret = value.toNumber();
    }
    return ret;
}

void CO::getCustomUnitGlobalBoost(qint32 index, CustomCoBoostInfo& info)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    qint32 ret = 0;
    QJSValueList args({pInterpreter->newQObject(this),
                       index,
                       pInterpreter->newQObject(&info),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_coID, "getCustomUnitGlobalBoost", args);
}

bool CO::showDefaultUnitZoneBoost()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    bool ret = true;
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue value = pInterpreter->doFunction(m_coID, "showDefaultUnitZoneBoost", args);
    if (value.isBool())
    {
        ret = value.toBool();
    }
    return ret;
}

qint32 CO::getCustomUnitZoneBoostCount()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    qint32 ret = 0;
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue value = pInterpreter->doFunction(m_coID, "getCustomUnitZoneBoostCount", args);
    if (value.isNumber())
    {
        ret = value.toNumber();
    }
    return ret;
}

void CO::getCustomUnitZoneBoost(qint32 index, CustomCoBoostInfo& info)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    qint32 ret = 0;
    QJSValueList args({pInterpreter->newQObject(this),
                       index,
                       pInterpreter->newQObject(&info),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_coID, "getCustomUnitZoneBoost", args);
}

void CO::serializeObject(QDataStream& pStream) const
{
    serializeObject(pStream, false);
}

void CO::serializeObject(QDataStream& pStream, bool forHash) const
{
    CONSOLE_PRINT("storing co", GameConsole::eDEBUG);
    pStream << getVersion();
    pStream << m_coID;
    pStream << m_powerStars;
    pStream << m_superpowerStars;
    if (forHash)
    {
        pStream << GlobalUtils::roundFloorToInt(m_powerFilled, DOUBLE_PRECISION);
    }
    else
    {
        pStream << m_powerFilled;
    }
    pStream << static_cast<qint32>(m_PowerMode);
    m_Variables.serializeObject(pStream, forHash);
    pStream << m_powerUsed;
    pStream << static_cast<qint32>(m_perkList.size());
    for (const auto & perk : qAsConst(m_perkList))
    {
        pStream << perk;
    }
    if (!forHash)
    {
        writeCoStyleToStream(pStream);
    }
    pStream << m_coRangeEnabled;
    pStream << m_globalCoZone;
}

void CO::deserializeObject(QDataStream& pStream)
{
    deserializer(pStream, false);
}

void CO::deserializer(QDataStream& pStream, bool fast)
{
    CONSOLE_PRINT("reading game co", GameConsole::eDEBUG);
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
        float power = 0.0;
        pStream >> power;
        m_powerFilled = static_cast<qreal>(power);
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
    if (version > 6)
    {
        pStream >> m_globalCoZone;
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
    CONSOLE_PRINT("reading co styles " + QString::number(size), GameConsole::eDEBUG);
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
        auto* pAnim = pCOSpriteManager->getResAnim(m_coID + "+nrm");
        if (pAnim != nullptr)
        {
            file = pAnim->getResPath();
            file.replace("+nrm.png", "");
        }
    }
    if (m_coID != CO_RANDOM)
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
    CONSOLE_PRINT("Loading sprites for CO " + coid, GameConsole::eDEBUG);
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    colorTable.convertTo(QImage::Format_ARGB32);
    maskTable.convertTo(QImage::Format_ARGB32);
    QString coidLower = coid.toLower();
    QStringList filenameList = file.split("/");
    QString filename = filenameList[filenameList.size() - 1];
    oxygine::spResAnim pAnim = oxygine::spResAnim(pCOSpriteManager->oxygine::Resources::getResAnim(filename + "+face", oxygine::error_policy::ep_ignore_error));
    oxygine::spResAnim pCOAnim;
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
    if (m_pMenu != nullptr)
    {
        m_pMenu->updatePlayerinfo();
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

oxygine::ResAnim* CO::getResAnim(const QString & id, oxygine::error_policy ep) const
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
