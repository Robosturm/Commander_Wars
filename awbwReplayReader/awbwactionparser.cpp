#include <QJsonArray>

#include "awbwReplayReader/awbwactionparser.h"
#include "awbwReplayReader/awbwreplayplayer.h"
#include "awbwReplayReader/awbwdatatypes.h"
#include "coreengine/memorymanagement.h"
#include "game/gameaction.h"
#include "game/gamemap.h"
#include "game/unit.h"
#include "ai/coreai.h"

const char* const AwbwActionParser::JSONKEY_ACTION = "action";
const char* const AwbwActionParser::JSONKEY_UNIT = "unit";
const char* const AwbwActionParser::JSONKEY_UNITNAME = "units_name";
const char* const AwbwActionParser::JSONKEY_NEWUNIT = "newUnit";
const char* const AwbwActionParser::JSONKEY_UNITS_FUEL = "units_fuel";
const char* const AwbwActionParser::JSONKEY_PATH = "paths";
const char* const AwbwActionParser::JSONKEY_GLOBAL = "global";
const char* const AwbwActionParser::JSONKEY_X = "x";
const char* const AwbwActionParser::JSONKEY_Y = "y";
const char* const AwbwActionParser::JSONKEY_UNITS_X = "units_x";
const char* const AwbwActionParser::JSONKEY_UNITS_Y = "units_y";
const char* const AwbwActionParser::JSONKEY_UNITS_COST = "units_cost";
const char* const AwbwActionParser::JSONKEY_UNITS_NAME = "units_name";
const char* const AwbwActionParser::JSONKEY_COPVALUES = "copValues";
const char* const AwbwActionParser::JSONKEY_COPVALUE = "copValue";
const char* const AwbwActionParser::JSONKEY_TAGVALUE = "tagValue";
const char* const AwbwActionParser::JSONKEY_ATTACKER = "attacker";
const char* const AwbwActionParser::JSONKEY_DEFENDER = "defender";
const char* const AwbwActionParser::JSONKEY_UNITS_HIT_POINTS = "units_hit_points";
const char* const AwbwActionParser::JSONKEY_COMBATINFOVISION = "combatInfoVision";
const char* const AwbwActionParser::JSONKEY_COMBATINFO = "combatInfo";
const char* const AwbwActionParser::JSONKEY_FIRE = "Fire";
const char* const AwbwActionParser::JSONKEY_MOVE = "Move";
const char* const AwbwActionParser::JSONKEY_AMMO = "units_ammo";
const char* const AwbwActionParser::JSONKEY_BUILDINGINFO = "buildingInfo";
const char* const AwbwActionParser::JSONKEY_BUILDINGS_X = "buildings_x";
const char* const AwbwActionParser::JSONKEY_BUILDINGS_Y = "buildings_y";
const char* const AwbwActionParser::JSONKEY_CAPT = "Capt";
const char* const AwbwActionParser::JSONKEY_PLAYERID = "playerId";
const char* const AwbwActionParser::JSONKEY_LAUNCH = "Launch";
const char* const AwbwActionParser::JSONKEY_TARGETX = "targetX";
const char* const AwbwActionParser::JSONKEY_TARGETY = "targetY";
const char* const AwbwActionParser::JSONKEY_SILOX = "siloX";
const char* const AwbwActionParser::JSONKEY_SILOY = "siloY";
const char* const AwbwActionParser::JSONKEY_COPOWER = "coPower";
const char* const AwbwActionParser::JSONKEY_TRANSPORTID = "transportID";
const char* const AwbwActionParser::JSONKEY_UNITS_ID = "units_id";
const char* const AwbwActionParser::JSONKEY_REPAIRED = "repaired";
const char* const AwbwActionParser::JSONKEY_LOAD = "Load";
const char* const AwbwActionParser::JSONKEY_TRANSPORT = "transport";
const char* const AwbwActionParser::JSONKEY_LOADED = "loaded";

AwbwActionParser::AwbwActionParser(AwbwReplayPlayer & pParent, GameMap* pMap)
    : m_pParent(pParent),
      m_pMap(pMap)
{
}

spGameAction AwbwActionParser::getAction(const QJsonObject & object, bool & fetchNextAction)
{
    m_lastAction = object;
    fetchNextAction = false;
    spGameAction action = MemoryManagement::create<GameAction>(m_pMap);
    QString actionType = object.value(JSONKEY_ACTION).toString().toLower();
    if (actionType == "move")
    {
        action->setActionID(CoreAI::ACTION_WAIT);
        QJsonObject startPoint = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject());
        addMovepath(object, action, startPoint.value(JSONKEY_UNITS_X).toInt(), startPoint.value(JSONKEY_UNITS_Y).toInt());
        updateUnitPos(action);
    }
    else if (actionType == "capt")
    {
        action->setActionID(CoreAI::ACTION_CAPTURE);
        QJsonObject startPoint = object.value(JSONKEY_CAPT).toObject()[JSONKEY_BUILDINGINFO].toObject();
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, startPoint[JSONKEY_BUILDINGS_X].toInt(), startPoint[JSONKEY_BUILDINGS_Y].toInt());
        updateUnitPos(action);
    }
    else if (actionType == "fire")
    {
        action->setActionID(CoreAI::ACTION_FIRE);
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, -1, -1);
        addActionFireInfo(object, action);
        updateUnitPos(action);
    }
    else if (actionType == "build")
    {
        action->setActionID(CoreAI::ACTION_BUILD_UNITS);
        addBuildInfo(object, action);
    }
    else if (actionType == "delete")
    {
        action->setActionID("ACTION_DELETE_UNIT");
        QJsonObject startPoint = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject());
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, startPoint.value(JSONKEY_UNITS_X).toInt(), startPoint.value(JSONKEY_UNITS_Y).toInt());
    }
    else if (actionType == "eliminated")
    {
        action = nullptr;
        fetchNextAction = true;
    }
    else if (actionType == "unload")
    {
        action->setActionID(CoreAI::ACTION_UNLOAD);
        addUnloadData(object, action);
    }
    else if (actionType == "unhide")
    {
        action->setActionID(CoreAI::ACTION_UNSTEALTH);
        QJsonObject startPoint = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject());
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, startPoint.value(JSONKEY_UNITS_X).toInt(), startPoint.value(JSONKEY_UNITS_Y).toInt());
        updateUnitPos(action);
    }
    else if (actionType == "tag")
    {
        action->setActionID(CoreAI::ACTION_SWAP_COS);
    }
    else if (actionType == "supply")
    {
        action->setActionID(CoreAI::ACTION_SUPPORTALL_RATION);
        QJsonObject startPoint = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject());
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, startPoint.value(JSONKEY_UNITS_X).toInt(), startPoint.value(JSONKEY_UNITS_Y).toInt());
    }
    else if (actionType == "resign")
    {
        action->setActionID("ACTION_SURRENDER_INTERNAL");
    }
    else if (actionType == "repair")
    {
        action->setActionID(CoreAI::ACTION_SUPPORTSINGLE_REPAIR);
        QJsonObject startPoint = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject());
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, startPoint.value(JSONKEY_UNITS_X).toInt(), startPoint.value(JSONKEY_UNITS_Y).toInt());
        addRepairData(object, action);
    }
    else if (actionType == "power")
    {
        QString type = object.value(JSONKEY_COPOWER).toString().toLower();
        if (type == "y")
        {
            action->setActionID(CoreAI::ACTION_ACTIVATE_POWER_CO_0);
        }
        else
        {
            action->setActionID(CoreAI::ACTION_ACTIVATE_SUPERPOWER_CO_0);
        }
    }
    else if (actionType == "load")
    {
        action->setActionID(CoreAI::ACTION_LOAD);
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, -1, -1);
        updateLoadInfo(object);
    }
    else if (actionType == "launch")
    {
        action->setActionID(CoreAI::ACTION_MISSILE);
        QJsonObject launchData = object.value(JSONKEY_LAUNCH).toObject();
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, launchData[JSONKEY_SILOX].toInt(), launchData[JSONKEY_SILOY].toInt());
        action->writeDataInt32(launchData[JSONKEY_TARGETX].toInt());
        action->writeDataInt32(launchData[JSONKEY_TARGETY].toInt());
        updateUnitPos(action);
    }
    else if (actionType == "join")
    {
        action->setActionID(CoreAI::ACTION_JOIN);
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, -1, -1);
    }
    else if (actionType == "hide")
    {
        action->setActionID(CoreAI::ACTION_STEALTH);
        QJsonObject startPoint = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject());
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, startPoint.value(JSONKEY_UNITS_X).toInt(), startPoint.value(JSONKEY_UNITS_Y).toInt());
        updateUnitPos(action);
    }
    else if (actionType == "gameover")
    {
        action = nullptr;
    }
    else if (actionType == "explode")
    {
        action->setActionID(CoreAI::ACTION_EXPLODE);
        QJsonObject startPoint = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject());
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, startPoint.value(JSONKEY_UNITS_X).toInt(), startPoint.value(JSONKEY_UNITS_Y).toInt());
    }
    else if (actionType == "end")
    {
        action->setActionID(CoreAI::ACTION_NEXT_PLAYER);
    }

    action->setMapHash(m_pMap->getMapHash());

    return action;
}

void AwbwActionParser::updateUnitPos(const spGameAction & action)
{
    if (action->getMovePathLength() > 0)
    {
        QVector<QPoint> path = action->getMovePath();
        QPoint start = path[path.size() - 1];
        QPoint end = path[0];
        bool done = false;
        for (auto & unit : m_currentGameState.units)
        {
            if (start.x() == unit.x &&
                start.y() == unit.y)
            {
                unit.x = end.x();
                unit.y = end.y();
                done = true;
            }
        }
        Q_ASSERT(done);
    }
}

void AwbwActionParser::onPostAction()
{
    if (m_lastAction.value(JSONKEY_ACTION).isString())
    {
        QString actionType = m_lastAction.value(JSONKEY_ACTION).toString().toLower();
        if (actionType == "end")
        {
            m_pParent.seekToDay(m_pParent.getDayFromPosition(m_pParent.getCurrentActionPos()));
        }
        else if (actionType == "fire")
        {
            QJsonObject fireInfo = m_lastAction.value(JSONKEY_FIRE).toObject();
            QJsonObject copValues = fireInfo.value(JSONKEY_COPVALUES).toObject();
            updateCoData(copValues[JSONKEY_ATTACKER].toObject());
            updateCoData(copValues[JSONKEY_DEFENDER].toObject());
        }
    }
    m_lastAction = QJsonObject();
}

void AwbwActionParser::storeCurrentGameState()
{
    const auto & replayReader = m_pParent.getReplayReader();
    const auto & gameStates = replayReader.getGameStates();
    qint32 actionIdx = 0;
    auto dayIndex = m_pParent.getCurrentTurnIndex(actionIdx);
    m_currentGameState = gameStates[dayIndex];
}

void AwbwActionParser::addMovepath(const QJsonObject & object, spGameAction & action, qint32 x, qint32 y)
{
    QJsonArray jsonPath = object.value(JSONKEY_PATH).toObject().value(JSONKEY_GLOBAL).toArray();
    QJsonObject unitObj = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject());
    if (jsonPath.size() > 0)
    {
        QJsonObject startPoint = jsonPath[0].toObject();
        action->setTarget(QPoint(startPoint[JSONKEY_X].toInt(), startPoint[JSONKEY_Y].toInt()));
        auto* pUnit = action->getTargetUnit();
        if (pUnit != nullptr)
        {
            QVector<QPoint> path;
            for (qint32 i = jsonPath.size() - 1; i >= 0; --i)
            {
                QJsonObject point = jsonPath[i].toObject();
                path.append(QPoint(point[JSONKEY_X].toInt(), point[JSONKEY_Y].toInt()));
            }
            action->setMovepath(path, pUnit->getFuel() - unitObj[JSONKEY_UNITS_FUEL].toInt());
        }
    }
    else
    {
        QPoint pos(x, y);
        action->setTarget(pos);
        QVector<QPoint> path = {pos};
        action->setMovepath(path, 0);
    }
}

void AwbwActionParser::addBuildInfo(const QJsonObject & object, spGameAction & action)
{
    QJsonObject unitObj = getPlayerObjectData(object.value(JSONKEY_NEWUNIT).toObject());
    QString unitId = AwbwDataTypes::UNIT_ID_ID_MAP[unitObj[JSONKEY_UNITS_NAME].toString().toLower()];
    action->setTarget(QPoint(unitObj[JSONKEY_UNITS_X].toInt(), unitObj[JSONKEY_UNITS_Y].toInt()));
    action->setCosts(action->getCosts() + unitObj[JSONKEY_UNITS_COST].toInt());
    action->writeDataString(unitId);
    action->setInputStep(action->getInputStep() + 1);
}

void AwbwActionParser::addActionFireInfo(const QJsonObject & object, spGameAction & action)
{
    QJsonObject fireInfo = object.value(JSONKEY_FIRE).toObject();
    QJsonObject combatInfo = getPlayerObjectData(fireInfo.value(JSONKEY_COMBATINFOVISION).toObject()).value(JSONKEY_COMBATINFO).toObject();
    QJsonObject attackerData = combatInfo.value(JSONKEY_ATTACKER).toObject();
    QJsonObject defenderData = combatInfo.value(JSONKEY_DEFENDER).toObject();
    if (action->getTarget().x() < 0)
    {
        QPoint pos(attackerData[JSONKEY_UNITS_X].toInt(), attackerData[JSONKEY_UNITS_Y].toInt());
        action->setTarget(pos);
        QVector<QPoint> path = {pos};
        action->setMovepath(path, 0);
    }
    qint32 defX = defenderData[JSONKEY_UNITS_X].toInt();
    qint32 defY = defenderData[JSONKEY_UNITS_Y].toInt();
    action->writeDataInt32(defX);
    action->writeDataInt32(defY);

    Unit* pAttacker = action->getTargetUnit();
    Unit* pDefender = m_pMap->getTerrain(defX, defY)->getUnit();
    addDamageData(pAttacker, pDefender, attackerData, defenderData, action);
    addDamageData(pDefender, pAttacker, defenderData, attackerData, action);
}

void AwbwActionParser::addDamageData(Unit* pAtk, Unit* pDef, const QJsonObject & atkData, const QJsonObject & defData, spGameAction & action)
{
    if (pAtk->getHp() > 0)
    {
        qint32 hp = (pDef->getHp() - defData[JSONKEY_UNITS_HIT_POINTS].toInt()) * Unit::MAX_UNIT_HP;
        if (hp > 0)
        {
            action->writeDataInt32(hp);
        }
        else
        {
            action->writeDataInt32(-1);
        }
        if (pAtk->getAmmo1() > atkData[JSONKEY_AMMO].toInt() ||
            !pAtk->hasAmmo2())
        {
            action->writeDataInt32(0);
        }
        else
        {
            action->writeDataInt32(1);
        }
    }
    else
    {
        action->writeDataInt32(-1);
        action->writeDataInt32(-1);
    }
}

void AwbwActionParser::updateCoData(const QJsonObject & data)
{
    const auto & replayReader = m_pParent.getReplayReader();
    const auto & gameStates = replayReader.getGameStates();
    for (auto & player : gameStates[0].players)
    {
        if (player.playerId == data[JSONKEY_PLAYERID].toInt())
        {
            auto* pPlayer = m_pMap->getPlayer(player.playerIdx);
            auto* pCO = pPlayer->getCO(0);
            if (pCO != nullptr)
            {
                auto powerStars = pCO->getPowerStars();
                pCO->setPowerFilled(data[JSONKEY_COPVALUE].toDouble() / (static_cast<double>(player.coData.maxPower) / static_cast<double>(powerStars)));
            }
            pCO = pPlayer->getCO(1);
            if (pCO != nullptr)
            {
                auto powerStars = pCO->getPowerStars();
                pCO->setPowerFilled(data[JSONKEY_TAGVALUE].toDouble() / (static_cast<double>(player.tagCoData.maxPower) / static_cast<double>(powerStars)));
            }
            break;
        }
    }
}

void AwbwActionParser::updateLoadInfo(const QJsonObject & object)
{
    QJsonObject loadData = object.value(JSONKEY_LOAD).toObject();
    qint32 transport = loadData[JSONKEY_TRANSPORT].toObject()[JSONKEY_GLOBAL].toInt();
    qint32 loaded = loadData[JSONKEY_LOADED].toObject()[JSONKEY_GLOBAL].toInt();
    for (auto & unit : m_currentGameState.units)
    {
        if (unit.unitId == transport)
        {
            if (unit.loadedUnitId1 == 0)
            {
                unit.loadedUnitId1 = loaded;
            }
            else
            {
                unit.loadedUnitId2 = loaded;
            }
            break;
        }
    }
}

void AwbwActionParser::addUnloadData(const QJsonObject & object, spGameAction & action)
{
    for (const auto & unit : m_currentGameState.units)
    {
        if (unit.unitId == object[JSONKEY_TRANSPORTID].toInt())
        {
            Q_ASSERT(m_pMap->getTerrain(unit.x, unit.y)->getUnit() != nullptr);
            QJsonObject unitData = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject());            
            QPoint pos(unit.x, unit.y);
            action->setTarget(pos);
            QVector<QPoint> path = {pos};
            action->setMovepath(path, 0);
            if (unit.loadedUnitId1 == unitData[JSONKEY_UNITS_ID].toInt())
            {
                action->writeDataString("0");
            }
            else
            {
                action->writeDataString("1");
            }
            action->writeDataInt32(unitData[JSONKEY_UNITS_X].toInt());
            action->writeDataInt32(unitData[JSONKEY_UNITS_Y].toInt());
            action->writeDataString("ACTION_WAIT");
            action->setInputStep(3);
            break;
        }
    }
}

void AwbwActionParser::addRepairData(const QJsonObject & object, spGameAction & action)
{
    QJsonObject data = getPlayerObjectData(object.value(JSONKEY_REPAIRED).toObject());
    action->writeDataInt32(data[JSONKEY_UNITS_X].toInt());
    action->writeDataInt32(data[JSONKEY_UNITS_Y].toInt());
}

void AwbwActionParser::setCurrentPlayerData(qint32 playerId, const QString & team)
{
    m_playerId = QString::number(playerId);
    m_team = team;
    storeCurrentGameState();
}

QJsonObject AwbwActionParser::getPlayerObjectData(const QJsonObject & object)
{
    if (object.value(m_playerId).isObject())
    {
        return object.value(m_playerId).toObject();
    }
    else if (object.value(m_team).isObject())
    {
        return object.value(m_team).toObject();
    }
    else
    {
        return object.value(JSONKEY_GLOBAL).toObject();
    }
}
