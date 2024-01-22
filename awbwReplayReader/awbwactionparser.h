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

    AwbwActionParser(GameMap* pMap);

    spGameAction getAction(const QJsonObject & object);

private:
    void addMovepath(const QJsonObject & object, spGameAction & action);

private:
    GameMap* m_pMap;
};

