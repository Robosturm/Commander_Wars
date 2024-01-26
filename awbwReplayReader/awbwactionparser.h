#pragma once

#include <QJsonObject>
#include "game/gameaction.h"

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

    AwbwActionParser(GameMap* pMap);

    spGameAction getAction(const QJsonObject & object);

private:
    void addMovepath(const QJsonObject & object, spGameAction & action);
    void addBuildInfo(const QJsonObject & object, spGameAction & action);
    void addActionFireInfo(const QJsonObject & object, spGameAction & action);
private:
    GameMap* m_pMap;
};

