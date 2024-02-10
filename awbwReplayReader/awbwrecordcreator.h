#pragma once

#include "awbwReplayReader/awbwreplayerreader.h"

class GameMap;

class AwbwRecordCreator
{
public:
    AwbwRecordCreator(GameMap* pMap);

    void loadRecordData(const AwbwReplayerReader & reader, qint32 gameStateIndex);
private:
    void updatePlayerData(const AwbwReplayerReader::GameState & gameState, qint32 player);
    void updateActionData(const AwbwReplayerReader::GameState & gameState, const AwbwReplayerReader::ActionInfo & actions, qint32 player, qint32 day);
    void addAttackData(AwbwReplayerReader::GameState & gameState, const QJsonObject & action, qint32 player, const QString & playerId, const QString & team, qint32 day);
private:
    GameMap* m_pMap;
};

