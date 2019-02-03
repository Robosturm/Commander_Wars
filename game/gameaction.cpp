#include "gameaction.h"
#include "coreengine/mainapp.h"
#include "game/gamemap.h"
#include "game/unit.h"
#include "game/building.h"
#include "game/player.h"

GameAction::GameAction()
{
}

GameAction::GameAction(QString actionID)
    : m_actionID(actionID)
{

}

void GameAction::perform()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "perform";
    QJSValueList args1;
    args1 << pApp->getInterpreter()->newQObject(this);
    pApp->getInterpreter()->doFunction(m_actionID, function1, args1);
}

void GameAction::setActionID(QString actionID)
{
    m_actionID = actionID;
}

QString GameAction::getActionID()
{
    return m_actionID;
}

Unit* GameAction::getTargetUnit()
{
    return GameMap::getInstance()->getTerrain(m_target.x(), m_target.y())->getUnit();
}

Building* GameAction::getTargetBuilding()
{
    return GameMap::getInstance()->getTerrain(m_target.x(), m_target.y())->getBuilding();
}

void GameAction::setMovepath(QVector<QPoint> points)
{
    m_Movepath = points;
}

bool GameAction::canBePerformed()
{
    canBePerformed(m_actionID);
}

QVector<QPoint> GameAction::getMovePath()
{
    return m_Movepath;
}

bool GameAction::canBePerformed(QString actionID)
{
    GameMap* pMap = GameMap::getInstance();
    Unit* pUnit = getTargetUnit();
    Building* pBuilding = getTargetBuilding();
    if (pUnit != nullptr)
    {
        if ((pUnit->getOwner()->getPlayerID() != pMap->getCurrentPlayer()->getPlayerID()) &&
            (!pUnit->getHasMoved()))
        {
            return false;
        }
    }

    if ((pBuilding != nullptr) && (pUnit == nullptr))
    {
        if (pBuilding->getOwner()->getPlayerID() != pMap->getCurrentPlayer()->getPlayerID())
        {
            return false;
        }
    }
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "canBePerformed";
    QJSValueList args1;
    args1 << pApp->getInterpreter()->newQObject(this);
    QJSValue ret = pApp->getInterpreter()->doFunction(actionID, function1, args1);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    return false;
}

QString GameAction::getActionText(QString actionID)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getActionText";
    QJSValueList args1;
    QJSValue ret = pApp->getInterpreter()->doFunction(actionID, function1, args1);
    if (ret.isString())
    {
        return ret.toString();
    }
    return "";
}

QString GameAction::getActionIcon(QString actionID)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getIcon";
    QJSValueList args1;
    QJSValue ret = pApp->getInterpreter()->doFunction(actionID, function1, args1);
    if (ret.isString())
    {
        return ret.toString();
    }
    return "";
}

void GameAction::setTarget(QPoint point)
{
    m_target = point;
}

QPoint GameAction::getTarget()
{
    return m_target;
}

QPoint GameAction::getActionTarget()
{
    if (m_Movepath.size() > 0)
    {
        return m_Movepath[0];
    }
    else
    {
        return m_target;
    }
}

Unit* GameAction::getMovementTarget()
{
    GameMap* pMap = GameMap::getInstance();
    QPoint actionTarget = getActionTarget();
    return pMap->getTerrain(actionTarget.x(), actionTarget.y())->getUnit();
}

Building* GameAction::getMovementBuilding()
{
    GameMap* pMap = GameMap::getInstance();
    QPoint actionTarget = getActionTarget();
    return pMap->getTerrain(actionTarget.x(), actionTarget.y())->getBuilding();
}

qint32 GameAction::getInputStep() const
{
    return inputStep;
}

void GameAction::setInputStep(const qint32 &value)
{
    inputStep = value;
}

bool GameAction::isFinalStep()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "isFinalStep";
    QJSValueList args1;
    args1 << pApp->getInterpreter()->newQObject(this);
    QJSValue ret = pApp->getInterpreter()->doFunction(m_actionID, function1, args1);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    return false;
}
