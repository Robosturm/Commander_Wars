#include "awbwReplayReader/awbwactionparser.h"
#include "coreengine/memorymanagement.h"

AwbwActionParser::AwbwActionParser(GameMap* pMap)
    : m_pMap(pMap)
{
}

spGameAction AwbwActionParser::getAction(const QJsonObject & object)
{
    spGameAction action; // = MemoryManagement::create<GameAction>(m_pMap);

    return action;
}
