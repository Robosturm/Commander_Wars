#include <QJsonArray>

#include "awbwReplayReader/awbwactionparser.h"
#include "coreengine/memorymanagement.h"
#include "game/gameaction.h"
#include "game/gamemap.h"
#include "ai/coreai.h"

const char* const AwbwActionParser::JSONKEY_ACTION = "action";
const char* const AwbwActionParser::JSONKEY_UNIT = "unit";
const char* const AwbwActionParser::JSONKEY_UNITNAME = "units_name";
const char* const AwbwActionParser::JSONKEY_NEWUNIT = "NewUnit";
const char* const AwbwActionParser::JSONKEY_UNITS_FUEL = "units_fuel";
const char* const AwbwActionParser::JSONKEY_PATH = "paths";
const char* const AwbwActionParser::JSONKEY_GLOBAL = "global";
const char* const AwbwActionParser::JSONKEY_X = "x";
const char* const AwbwActionParser::JSONKEY_Y = "y";

AwbwActionParser::AwbwActionParser(GameMap* pMap)
    : m_pMap(pMap)
{
}

spGameAction AwbwActionParser::getAction(const QJsonObject & object)
{
    spGameAction action = MemoryManagement::create<GameAction>(m_pMap);
    QString actionType = object.value(JSONKEY_ACTION).toString().toLower();
    if (actionType == "move")
    {
        action->setActionID(CoreAI::ACTION_WAIT);
        addMovepath(object, action);
    }
    else if (actionType == "capt")
    {
        action->setActionID(CoreAI::ACTION_CAPTURE);
        addMovepath(object, action);
    }
    else if (actionType == "fire")
    {
        action->setActionID(CoreAI::ACTION_FIRE);

    }
    else if (actionType == "build")
    {
        action->setActionID(CoreAI::ACTION_BUILD_UNITS);

    }
    else if (actionType == "delete")
    {
        action->setActionID("ACTION_DELETE_UNIT");
    }
    else if (actionType == "eliminated")
    {

    }
    else if (actionType == "unload")
    {
        action->setActionID(CoreAI::ACTION_UNLOAD);

    }
    else if (actionType == "unhide")
    {
        action->setActionID(CoreAI::ACTION_UNSTEALTH);
        addMovepath(object, action);
    }
    else if (actionType == "tag")
    {
        action->setActionID(CoreAI::ACTION_ACTIVATE_TAGPOWER);

    }
    else if (actionType == "supply")
    {
        action->setActionID(CoreAI::ACTION_SUPPORTALL_RATION);
        addMovepath(object, action);
    }
    else if (actionType == "resign")
    {

    }
    else if (actionType == "repair")
    {
        action->setActionID(CoreAI::ACTION_SUPPORTSINGLE_REPAIR);
        addMovepath(object, action);

    }
    else if (actionType == "power")
    {

    }
    else if (actionType == "load")
    {
        action->setActionID(CoreAI::ACTION_LOAD);
        addMovepath(object, action);
    }
    else if (actionType == "launch")
    {
        action->setActionID(CoreAI::ACTION_MISSILE);
    }
    else if (actionType == "join")
    {
        action->setActionID(CoreAI::ACTION_JOIN);
        addMovepath(object, action);

    }
    else if (actionType == "hide")
    {
        action->setActionID(CoreAI::ACTION_STEALTH);
        addMovepath(object, action);
    }
    else if (actionType == "gameover")
    {
    }
    else if (actionType == "explode")
    {
        action->setActionID(CoreAI::ACTION_EXPLODE);
    }
    else if (actionType == "end")
    {
        action->setActionID(CoreAI::ACTION_NEXT_PLAYER);
    }

    action->setMapHash(m_pMap->getMapHash());
    return action;
}

void AwbwActionParser::addMovepath(const QJsonObject & object, spGameAction & action)
{
    QJsonArray jsonPath = object.value(JSONKEY_PATH).toObject().value(JSONKEY_GLOBAL).toArray();
    QJsonObject unitObj = object.value(JSONKEY_UNIT).toObject().value(JSONKEY_GLOBAL).toObject();
    if (jsonPath.size() > 0)
    {
        QJsonObject startPoint = jsonPath[0].toObject();
        action->setTarget(QPoint(startPoint[JSONKEY_X].toInt(), startPoint[JSONKEY_Y].toInt()));
        auto* pUnit = action->getTargetUnit();
        QVector<QPoint> path;
        for (qint32 i = jsonPath.size() - 1; i >= 0; --i)
        {
            QJsonObject point = jsonPath[i].toObject();
            path.append(QPoint(point[JSONKEY_X].toInt(), point[JSONKEY_Y].toInt()));
        }
        action->setMovepath(path, pUnit->getFuel() - unitObj[JSONKEY_UNITS_FUEL].toInt());
    }
}
