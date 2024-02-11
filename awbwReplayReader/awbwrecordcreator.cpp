#include "awbwReplayReader/awbwrecordcreator.h"
#include "awbwReplayReader/awbwactionparser.h"
#include "awbwReplayReader/awbwdatatypes.h"
#include "game/gamemap.h"
#include "coreengine/globalutils.h"

AwbwRecordCreator::AwbwRecordCreator(GameMap* pMap)
    : m_pMap(pMap)
{

}

void AwbwRecordCreator::loadRecordData(const AwbwReplayerReader & reader, qint32 gameStateIndex)
{
    auto* recorder = m_pMap->getGameRecorder();
    recorder->reset();

    const auto & gameStates = reader.getGameStates();
    const auto & actions = reader.getActions();

    qint32 lastDay = 0;
    for (qint32 i = 0; i < gameStateIndex; ++i)
    {
        if (lastDay != gameStates[i].day)
        {
            recorder->newDay();
            lastDay = gameStates[i].day;
        }
        qint32 playerId = 0;
        for (const auto & player : gameStates[i].players)
        {
            if (player.playerId == gameStates[i].turn)
            {
                break;
            }
            else
            {
                ++playerId;
            }
        }
        updatePlayerData(gameStates[i], playerId);
        updateActionData(gameStates[i], actions[i], playerId, lastDay);
    }
}

void AwbwRecordCreator::updatePlayerData(const AwbwReplayerReader::GameState & gameState, qint32 player)
{
    auto* recorder = m_pMap->getGameRecorder();
    qint32 units = 0;
    qint32 buildings = 0;
    qint32 income = 0;
    qint32 playerStrength = 0;
    for (const auto & unit : gameState.units)
    {
        if (unit.playerId == gameState.players[player].playerId)
        {
            ++units;
            playerStrength += unit.cost * GlobalUtils::roundUp(unit.hp) / Unit::MAX_UNIT_HP;
        }
    }
    for (const auto & building : gameState.buildings)
    {
        const auto & info = AwbwDataTypes::TERRAIN_BUILDING_DATA[building.terrainId];
        if (!info.id.isEmpty())
        {
            if (info.owner == AwbwDataTypes::COUNTRYID_PLAYERID_MAP[gameState.players[player].countryId])
            {
                ++buildings;
                auto* pBuilding = m_pMap->getTerrain(building.x, building.y)->getBuilding();
                if (pBuilding != nullptr && pBuilding->getIncome() > 0)
                {
                    income += gameState.fundsPerBuilding;
                }
            }
        }
    }
    recorder->updatePlayerData(gameState.day, player, gameState.players[player].funds, income, buildings, units, playerStrength);
}

void AwbwRecordCreator::updateActionData(const AwbwReplayerReader::GameState & gameState, const AwbwReplayerReader::ActionInfo & actions, qint32 player, qint32 day)
{
    auto copyGameState = gameState;
    auto* recorder = m_pMap->getGameRecorder();
    QString playerId = QString::number(gameState.players[player].playerId);
    QString team = gameState.players[player].team;
    for (const auto & action : actions.actionData)
    {
        QString actionType = action.value(AwbwActionParser::JSONKEY_ACTION).toString().toLower();
        if (actionType == AwbwActionParser::JSONKEY_ACTIONTYPE_POWER)
        {
            QString type = action.value(AwbwActionParser::JSONKEY_COPOWER).toString().toLower();
            if (type == "y")
            {
                recorder->addSpecialEvent(player, GameEnums::GameRecord_SpecialEvents_Power);
            }
            else
            {
                recorder->addSpecialEvent(player, GameEnums::GameRecord_SpecialEvents_SuperPower);
            }
        }
        else if (actionType == AwbwActionParser::JSONKEY_ACTIONTYPE_FIRE)
        {
            addAttackData(copyGameState, action, player, playerId, team, day);
        }
        else if (actionType == AwbwActionParser::JSONKEY_ACTIONTYPE_BUILD)
        {
            QJsonObject unitObj = AwbwActionParser::getPlayerObjectData(action.value(AwbwActionParser::JSONKEY_NEWUNIT).toObject(), playerId, team);
            QString unitId = AwbwDataTypes::UNIT_ID_ID_MAP[unitObj[AwbwActionParser::JSONKEY_UNITS_NAME].toString().toLower()];
            recorder->buildUnit(player, unitId, player);
        }
    }
}

void AwbwRecordCreator::addAttackData(AwbwReplayerReader::GameState & gameState, const QJsonObject & action, qint32 player, const QString & playerId, const QString & team, qint32 day)
{
    auto* recorder = m_pMap->getGameRecorder();
    QJsonObject fireInfo = action.value(AwbwActionParser::JSONKEY_FIRE).toObject();
    QJsonObject combatInfo = AwbwActionParser::getPlayerObjectData(fireInfo.value(AwbwActionParser::JSONKEY_COMBATINFOVISION).toObject(), playerId, team).value(AwbwActionParser::JSONKEY_COMBATINFO).toObject();
    QJsonObject attackerData = combatInfo.value(AwbwActionParser::JSONKEY_ATTACKER).toObject();
    auto atkHp = attackerData[AwbwActionParser::JSONKEY_UNITS_HIT_POINTS].toInt();
    QJsonObject defenderData = combatInfo.value(AwbwActionParser::JSONKEY_DEFENDER).toObject();
    auto defHp = defenderData[AwbwActionParser::JSONKEY_UNITS_HIT_POINTS].toInt();
    AwbwReplayerReader::UnitInfo * attackerInfo = nullptr;
    AwbwReplayerReader::UnitInfo * defenderInfo = nullptr;
    for (auto & unit : gameState.units)
    {
        if (unit.unitId == attackerData[AwbwActionParser::JSONKEY_UNITS_ID].toInt())
        {
            attackerInfo = &unit;
        }
        else if (unit.unitId == defenderData[AwbwActionParser::JSONKEY_UNITS_ID].toInt())
        {
            defenderInfo = &unit;
        }
        if (defenderInfo != nullptr &&
            attackerInfo != nullptr)
        {
            break;
        }
    }
    if (defenderInfo != nullptr &&
        attackerInfo != nullptr)
    {
        QString defId = AwbwDataTypes::UNIT_ID_ID_MAP[defenderInfo->name.toLower()];
        QString atkId = AwbwDataTypes::UNIT_ID_ID_MAP[attackerInfo->name.toLower()];
        qint32 defenderPlayerId = 0;
        for (const auto & player : gameState.players)
        {
            if (player.playerId == gameState.turn)
            {
                break;
            }
            else
            {
                ++defenderPlayerId;
            }
        }
        if (defHp <= 0)
        {
            recorder->destroyedUnit(player, defId, defenderPlayerId);
            recorder->lostUnit(defenderPlayerId, defId, player);
        }
        if (atkHp <= 0)
        {
            recorder->destroyedUnit(defenderPlayerId, atkId, player);
            recorder->lostUnit(player, atkId, player);
        }
        recorder->logAttack(day,
                            (defenderInfo->hp - defHp) * Unit::MAX_UNIT_HP, attackerData[AwbwActionParser::JSONKEY_UNITS_X].toInt(), attackerData[AwbwActionParser::JSONKEY_UNITS_Y].toInt(), atkId, player, atkHp <= 0,
                            (attackerInfo->hp - atkHp) * Unit::MAX_UNIT_HP, defenderInfo->x, defenderInfo->y, atkId, defenderPlayerId, defHp <= 0,
                            true);
        attackerInfo->hp = atkHp;
        defenderInfo->hp = defHp;
    }
}
