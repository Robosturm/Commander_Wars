#pragma once

#include <QJsonObject>
#include "game/gameaction.h"

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
    static const char* const JSONKEY_COMBATINFO;
    static const char* const JSONKEY_FIRE;
    static const char* const JSONKEY_MOVE;
    static const char* const JSONKEY_AMMO;
    static const char* const JSONKEY_BUILDINGINFO;
    static const char* const JSONKEY_BUILDINGS_X;
    static const char* const JSONKEY_BUILDINGS_Y;
    static const char* const JSONKEY_CAPT;
    static const char* const JSONKEY_PLAYERID;

    AwbwActionParser(AwbwReplayPlayer & pParent, GameMap* pMap);

    spGameAction getAction(const QJsonObject & object);
    void onPostAction();
private:
    void addMovepath(const QJsonObject & object, spGameAction & action, qint32 x, qint32 y);
    void addBuildInfo(const QJsonObject & object, spGameAction & action);
    void addActionFireInfo(const QJsonObject & object, spGameAction & action);
    void addDamageData(Unit* pAtk, Unit* pDef, const QJsonObject & atkData, const QJsonObject & defData, spGameAction & action);
    void updateCoData(const QJsonObject & data);
private:
    AwbwReplayPlayer & m_pParent;
    GameMap* m_pMap;
    QJsonObject m_lastAction;
};

