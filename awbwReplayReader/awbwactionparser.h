#pragma once

#include <QJsonObject>
#include "game/gameaction.h"
#include "awbwReplayReader/awbwreplayerreader.h"

class AwbwReplayPlayer;

class AwbwActionParser
{
public:
    static const char* const JSONKEY_ACTION;
    static const char* const JSONKEY_NEWUNIT;
    static const char* const JSONKEY_UNITNAME;
    static const char* const JSONKEY_UNIT;
    static const char* const JSONKEY_UNITS_FUEL;
    static const char* const JSONKEY_PATH;
    static const char* const JSONKEY_GLOBAL;
    static const char* const JSONKEY_X;
    static const char* const JSONKEY_Y;
    static const char* const JSONKEY_UNITS_X;
    static const char* const JSONKEY_UNITS_Y;
    static const char* const JSONKEY_UNITS_COST;
    static const char* const JSONKEY_UNITS_NAME;
    static const char* const JSONKEY_COPVALUES;
    static const char* const JSONKEY_COPVALUE;
    static const char* const JSONKEY_TAGVALUE;
    static const char* const JSONKEY_ATTACKER;
    static const char* const JSONKEY_DEFENDER;
    static const char* const JSONKEY_UNITS_HIT_POINTS;
    static const char* const JSONKEY_COMBATINFOVISION;
    static const char* const JSONKEY_REPAIRED;
    static const char* const JSONKEY_REPAIR;
    static const char* const JSONKEY_COMBATINFO;
    static const char* const JSONKEY_FIRE;
    static const char* const JSONKEY_MOVE;
    static const char* const JSONKEY_AMMO;
    static const char* const JSONKEY_BUILDINGINFO;
    static const char* const JSONKEY_BUILDINGS_X;
    static const char* const JSONKEY_BUILDINGS_Y;
    static const char* const JSONKEY_CAPT;
    static const char* const JSONKEY_PLAYERID;
    static const char* const JSONKEY_LAUNCH;
    static const char* const JSONKEY_TARGETX;
    static const char* const JSONKEY_TARGETY;
    static const char* const JSONKEY_SILOX;
    static const char* const JSONKEY_SILOY;
    static const char* const JSONKEY_COPOWER;
    static const char* const JSONKEY_TRANSPORTID;
    static const char* const JSONKEY_UNITS_ID;
    static const char* const JSONKEY_LOAD;
    static const char* const JSONKEY_TRANSPORT;
    static const char* const JSONKEY_LOADED;
    static const char* const JSONKEY_ACTIONTYPE_POWER;
    static const char* const JSONKEY_ACTIONTYPE_FIRE;
    static const char* const JSONKEY_ACTIONTYPE_BUILD;

    AwbwActionParser(AwbwReplayPlayer & pParent, GameMap* pMap);

    spGameAction getAction(const QJsonObject & object, bool & fetchNextAction);
    void onPostAction();
    void setCurrentPlayerData(qint32 playerId, const QString & team);
    static QJsonObject getPlayerObjectData(const QJsonObject & object, const QString & playerId, const QString & team);

private:
    void addMovepath(const QJsonObject & object, spGameAction & action, qint32 x, qint32 y);
    void addBuildInfo(const QJsonObject & object, spGameAction & action);
    void addActionFireInfo(const QJsonObject & object, spGameAction & action);
    void addDamageData(Unit* pAtk, Unit* pDef, const QJsonObject & atkData, const QJsonObject & defData, spGameAction & action);
    void updateCoData(const QJsonObject & data);
    void addUnloadData(const QJsonObject & object, spGameAction & action);
    void addRepairData(const QJsonObject & object, spGameAction & action);
    void storeCurrentGameState();
    void updateUnitPos(const spGameAction & action);
    void updateLoadInfo(const QJsonObject & object);
private:
    AwbwReplayPlayer & m_pParent;
    GameMap* m_pMap;
    QJsonObject m_lastAction;
    QString m_playerId;
    QString m_team;
    AwbwReplayerReader::GameState m_currentGameState;
};

