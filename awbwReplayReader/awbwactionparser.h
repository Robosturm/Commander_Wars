#pragma once

#include <QJsonObject>
#include "game/gameaction.h"

class AwbwActionParser
{
public:
    AwbwActionParser(GameMap* pMap);

    spGameAction getAction(const QJsonObject & object);
private:
    GameMap* m_pMap;
};

