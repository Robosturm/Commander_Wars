#include <QJsonArray>

#include "awbwReplayReader/awbwactionparser.h"
#include "awbwReplayReader/awbwreplayplayer.h"
#include "awbwReplayReader/awbwdatatypes.h"
#include "coreengine/memorymanagement.h"
#include "coreengine/mainapp.h"
#include "game/gameaction.h"
#include "game/gamemap.h"
#include "game/unit.h"
#include "ai/coreai.h"

#include "menue/replaymenu.h"

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
const char* const AwbwActionParser::JSONKEY_REPAIR = "Repair";
const char* const AwbwActionParser::JSONKEY_LOAD = "Load";
const char* const AwbwActionParser::JSONKEY_TRANSPORT = "transport";
const char* const AwbwActionParser::JSONKEY_LOADED = "loaded";
const char* const AwbwActionParser::JSONKEY_ACTIONTYPE_POWER = "power";
const char* const AwbwActionParser::JSONKEY_ACTIONTYPE_FIRE = "fire";
const char* const AwbwActionParser::JSONKEY_ACTIONTYPE_BUILD = "build";
const char* const AwbwActionParser::JSONKEY_SUPPLY= "Supply";
const char* const AwbwActionParser::JSONKEY_HIDE = "Hide";
const char* const AwbwActionParser::JSONKEY_UNIT_ID = "unitId";
const char* const AwbwActionParser::JSONKEY_DELETE = "Delete";
const char* const AwbwActionParser::JSONKEY_UNHIDE = "Unhide";
const char* const AwbwActionParser::JSONKEY_EXPLODE = "Explode";
const char* const AwbwActionParser::JSONKEY_GAMEOVER = "GameOver";
const char* const AwbwActionParser::JSONKEY_LOSERS = "losers";
const char* const AwbwActionParser::JSONKEY_WINNERS = "winners";

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
        QJsonObject startPoint = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject(), m_playerId, m_team).toObject();
        addMovepath(object, action, getUnitPosFromObject(startPoint));
        updateUnitPos(action);
    }
    else if (actionType == "capt")
    {
        action->setActionID(CoreAI::ACTION_CAPTURE);
        QJsonValue startPoint = object.value(JSONKEY_CAPT);
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, getUnitPosFromObject(startPoint));
        updateUnitPos(action);
    }
    else if (actionType == JSONKEY_ACTIONTYPE_FIRE)
    {
        action->setActionID(CoreAI::ACTION_FIRE);
        QJsonValue startPoint = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject(), m_playerId, m_team);
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, getUnitPosFromObject(startPoint));
        addActionFireInfo(object, action);
        updateUnitPos(action);
    }
    else if (actionType == JSONKEY_ACTIONTYPE_BUILD)
    {
        action->setActionID(CoreAI::ACTION_BUILD_UNITS);
        addBuildInfo(object, action);
    }
    else if (actionType == "delete")
    {
        action->setActionID("ACTION_DELETE_UNIT");
        QJsonValue startPoint = getPlayerObjectData(object.value(JSONKEY_DELETE).toObject().value(JSONKEY_UNIT_ID).toObject(), m_playerId, m_team);
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, getUnitPosFromObject(startPoint));
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
        QJsonValue startPoint = getPlayerObjectData(object.value(JSONKEY_UNHIDE).toObject().value(JSONKEY_UNIT).toObject(), m_playerId, m_team);
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, getUnitPosFromObject(startPoint));
        updateUnitPos(action);
    }
    else if (actionType == "tag")
    {
        action->setActionID(CoreAI::ACTION_SWAP_COS);
    }
    else if (actionType == "supply")
    {
        action->setActionID(CoreAI::ACTION_SUPPORTALL_RATION);
        QJsonValue startPoint = getPlayerObjectData(object.value(JSONKEY_SUPPLY).toObject().value(JSONKEY_UNIT).toObject(), m_playerId, m_team);
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, getUnitPosFromObject(startPoint));
        updateUnitPos(action);
    }
    else if (actionType == "resign")
    {
        action->setActionID("ACTION_SURRENDER_INTERNAL");
    }
    else if (actionType == "repair")
    {
        action->setActionID(CoreAI::ACTION_SUPPORTSINGLE_REPAIR);
        QJsonValue startPoint = getPlayerObjectData(object.value(JSONKEY_REPAIR).toObject().value(JSONKEY_UNIT).toObject(), m_playerId, m_team);
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, getUnitPosFromObject(startPoint));
        addRepairData(object.value(JSONKEY_REPAIR).toObject(), action);
        updateUnitPos(action);
    }
    else if (actionType == JSONKEY_ACTIONTYPE_POWER)
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
        QJsonValue startPoint = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject(), m_playerId, m_team);
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, getUnitPosFromObject(startPoint));
        updateLoadInfo(object);
    }
    else if (actionType == "launch")
    {
        action->setActionID(CoreAI::ACTION_MISSILE);
        QJsonValue launchValue= object.value(JSONKEY_LAUNCH);
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, getUnitPosFromObject(launchValue));
        QJsonObject launchData = launchValue.toObject();
        action->writeDataInt32(launchData[JSONKEY_TARGETX].toInt(-1));
        action->writeDataInt32(launchData[JSONKEY_TARGETY].toInt(-1));
        updateUnitPos(action);
    }
    else if (actionType == "join")
    {
        action->setActionID(CoreAI::ACTION_JOIN);
        QJsonObject startPoint = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject(), m_playerId, m_team).toObject();
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, getUnitPosFromObject(startPoint));
    }
    else if (actionType == "hide")
    {
        action->setActionID(CoreAI::ACTION_STEALTH);
        QJsonValue startPoint = getPlayerObjectData(object.value(JSONKEY_HIDE).toObject().value(JSONKEY_UNIT).toObject(), m_playerId, m_team);
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, getUnitPosFromObject(startPoint));
        updateUnitPos(action);
    }
    else if (actionType == "gameover")
    {
        action = nullptr;
    }
    else if (actionType == "explode")
    {
        action->setActionID(CoreAI::ACTION_EXPLODE);
        QJsonValue startPoint = getPlayerObjectData(object.value(JSONKEY_EXPLODE).toObject().value(JSONKEY_UNIT_ID).toObject(), m_playerId, m_team);
        addMovepath(object.value(JSONKEY_MOVE).toObject(), action, getUnitPosFromObject(startPoint));
    }
    else if (actionType == "end")
    {
        action->setActionID(CoreAI::ACTION_NEXT_PLAYER);
    }
    else
    {
        action = nullptr;
        fetchNextAction = true;
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
            Mainapp::getInstance()->pauseRendering();
            m_pParent.seekToDay(m_pParent.getDayFromPosition(m_pParent.getCurrentActionPos()));
            m_pParent.getReplayMenu()->updatePlayerUiData();
            m_pMap->updateSprites();
            m_pMap->getGameRules()->createFogVision();
            Mainapp::getInstance()->continueRendering();
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

void AwbwActionParser::addMovepath(const QJsonObject & object, spGameAction & action, QPoint unitPos)
{
    QJsonArray jsonPath = getPlayerObjectData(object.value(JSONKEY_PATH).toObject(), m_playerId, m_team).toArray();
    QJsonObject unitObj = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject(), m_playerId, m_team).toObject();
    if (jsonPath.size() > 0)
    {
        QJsonObject startPoint = jsonPath[0].toObject();
        action->setTarget(QPoint(startPoint[JSONKEY_X].toInt(-1), startPoint[JSONKEY_Y].toInt(-1)));
        auto* pUnit = action->getTargetUnit();
        if (pUnit != nullptr)
        {
            QVector<QPoint> path;
            for (qint32 i = jsonPath.size() - 1; i >= 0; --i)
            {
                QJsonObject point = jsonPath[i].toObject();
                path.append(QPoint(point[JSONKEY_X].toInt(-1), point[JSONKEY_Y].toInt(-1)));
            }
            action->setMovepath(path, pUnit->getFuel() - unitObj[JSONKEY_UNITS_FUEL].toInt(-1));
        }
    }
    else
    {
        action->setTarget(unitPos);
        QVector<QPoint> path = {unitPos};
        action->setMovepath(path, 0);
    }
}

void AwbwActionParser::addBuildInfo(const QJsonObject & object, spGameAction & action)
{
    QJsonObject unitObj = getPlayerObjectData(object.value(JSONKEY_NEWUNIT).toObject(), m_playerId, m_team).toObject();
    QString unitId = AwbwDataTypes::UNIT_ID_ID_MAP[unitObj[JSONKEY_UNITS_NAME].toString().toLower()];
    action->setTarget(QPoint(unitObj[JSONKEY_UNITS_X].toInt(-1), unitObj[JSONKEY_UNITS_Y].toInt(-1)));
    action->setCosts(action->getCosts() + unitObj[JSONKEY_UNITS_COST].toInt(-1));
    action->writeDataString(unitId);
    action->setInputStep(action->getInputStep() + 1);
}

void AwbwActionParser::addActionFireInfo(const QJsonObject & object, spGameAction & action)
{
    QJsonObject fireInfo = object.value(JSONKEY_FIRE).toObject();
    QJsonObject combatInfo = getPlayerObjectData(fireInfo.value(JSONKEY_COMBATINFOVISION).toObject(), m_playerId, m_team).toObject().value(JSONKEY_COMBATINFO).toObject();
    QJsonObject attackerData = combatInfo.value(JSONKEY_ATTACKER).toObject();
    QJsonObject defenderData = combatInfo.value(JSONKEY_DEFENDER).toObject();
    if (action->getTarget().x() < 0)
    {
        QPoint pos(attackerData[JSONKEY_UNITS_X].toInt(-1), attackerData[JSONKEY_UNITS_Y].toInt(-1));
        action->setTarget(pos);
        QVector<QPoint> path = {pos};
        action->setMovepath(path, 0);
    }
    qint32 defX = defenderData[JSONKEY_UNITS_X].toInt(-1);
    qint32 defY = defenderData[JSONKEY_UNITS_Y].toInt(-1);
    action->writeDataInt32(defX);
    action->writeDataInt32(defY);

    Unit* pAttacker = action->getTargetUnit();
    Unit* pDefender = m_pMap->getTerrain(defX, defY)->getUnit();
    addDamageData(pAttacker, pDefender, attackerData, defenderData, action);
    addDamageData(pDefender, pAttacker, defenderData, attackerData, action);
}

void AwbwActionParser::addDamageData(Unit* pAtk, Unit* pDef, const QJsonObject & atkData, const QJsonObject & defData, spGameAction & action)
{
    if (atkData[JSONKEY_UNITS_HIT_POINTS].toInt(-1) > 0)
    {
        qint32 damage = (pDef->getHpRounded() - defData[JSONKEY_UNITS_HIT_POINTS].toInt(-1)) * Unit::MAX_UNIT_HP;
        action->writeDataInt32(damage);
        if (pAtk->getAmmo1() > atkData[JSONKEY_AMMO].toInt(-1) ||
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
        if (player.playerId == data[JSONKEY_PLAYERID].toInt(-1))
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
    qint32 transport = getPlayerObjectData(loadData[JSONKEY_TRANSPORT].toObject(), m_playerId, m_team).toInt(-1);
    qint32 loaded = getPlayerObjectData(loadData[JSONKEY_LOADED].toObject(), m_playerId, m_team).toInt(-1);
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
        if (unit.unitId == object[JSONKEY_TRANSPORTID].toInt(-1))
        {
            Q_ASSERT(m_pMap->getTerrain(unit.x, unit.y)->getUnit() != nullptr);
            QJsonObject unitData = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject(), m_playerId, m_team).toObject();
            QPoint pos(unit.x, unit.y);
            action->setTarget(pos);
            QVector<QPoint> path = {pos};
            action->setMovepath(path, 0);
            if (unit.loadedUnitId1 == unitData[JSONKEY_UNITS_ID].toInt(-1))
            {
                action->writeDataString("0");
            }
            else
            {
                action->writeDataString("1");
            }
            action->writeDataInt32(unitData[JSONKEY_UNITS_X].toInt(-1));
            action->writeDataInt32(unitData[JSONKEY_UNITS_Y].toInt(-1));
            action->writeDataString("ACTION_WAIT");
            action->setInputStep(3);
            break;
        }
    }
}

void AwbwActionParser::addRepairData(const QJsonObject & object, spGameAction & action)
{
    QJsonObject data = getPlayerObjectData(object.value(JSONKEY_REPAIRED).toObject(), m_playerId, m_team).toObject();
    for (const auto & unit : m_currentGameState.units)
    {
        if (unit.unitId == data[JSONKEY_UNITS_ID].toInt(-1))
        {
            action->writeDataInt32(unit.x);
            action->writeDataInt32(unit.y);
            break;
        }
    }
    if (action->getTarget().x() < 0)
    {
        auto id = getPlayerObjectData(object.value(JSONKEY_UNIT).toObject(), m_playerId, m_team).toInt();
        for (const auto & unit : m_currentGameState.units)
        {
            if (unit.unitId == id)
            {
                QPoint pos(unit.x, unit.y);
                action->setTarget(pos);
                QVector<QPoint> path = {pos};
                action->setMovepath(path, 0);
                break;
            }
        }
    }
}

void AwbwActionParser::setCurrentPlayerData(qint32 playerId, const QString & team)
{
    m_playerId = QString::number(playerId);
    m_team = team;
    storeCurrentGameState();
}

QJsonValue AwbwActionParser::getPlayerObjectData(const QJsonObject & object, const QString & playerId, const QString & team)
{
    if (!object.value(playerId).isUndefined())
    {
        return object.value(playerId);
    }
    else if (!object.value(team).isUndefined())
    {
        return object.value(team);
    }
    else
    {
        return object.value(JSONKEY_GLOBAL);
    }
}

QPoint AwbwActionParser::getUnitPosFromObject(const QJsonValue & value)
{
    if (value.isObject())
    {
        QJsonObject object = value.toObject();
        if (object.value(JSONKEY_BUILDINGINFO).isObject())
        {
            QJsonObject building = object.value(JSONKEY_BUILDINGINFO).toObject();
            return QPoint(building.value(JSONKEY_BUILDINGS_X).toInt(-1), building.value(JSONKEY_BUILDINGS_Y).toInt(-1));
        }
        else if (object.value(JSONKEY_UNITS_X).isUndefined())
        {
            auto unitId = getPlayerObjectData(object, m_playerId, m_team).toInt();
            for (const auto & unit : m_currentGameState.units)
            {
                if (unitId == unit.unitId)
                {
                    return QPoint(unit.x, unit.y);
                }
            }
        }
        else if (object.value(JSONKEY_SILOX).isDouble())
        {
            return QPoint(object.value(JSONKEY_SILOX).toInt(-1), object.value(JSONKEY_SILOY).toInt(-1));
        }
        else
        {
            return QPoint(object.value(JSONKEY_UNITS_X).toInt(-1), object.value(JSONKEY_UNITS_Y).toInt(-1));
        }
    }
    else
    {
        auto unitId = value.toInt();
        for (const auto & unit : m_currentGameState.units)
        {
            if (unitId == unit.unitId)
            {
                return QPoint(unit.x, unit.y);
            }
        }
    }
    return QPoint(-1, -1);
}
