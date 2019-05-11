#include "gameaction.h"
#include "coreengine/mainapp.h"
#include "game/gamemap.h"
#include "game/unit.h"
#include "game/building.h"
#include "game/player.h"
#include "game/co.h"

GameAction::GameAction()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    buffer.open(QIODevice::ReadWrite);
    seed = QRandomGenerator::global()->bounded(std::numeric_limits<quint32>::max());
}

GameAction::GameAction(QString actionID)
    : m_actionID(actionID)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    buffer.open(QIODevice::ReadWrite);
    seed = QRandomGenerator::global()->bounded(std::numeric_limits<quint32>::max());
}

void GameAction::deleteAction()
{
    delete this;
}

void GameAction::setTargetUnit(Unit *pTargetUnit)
{
    m_pTargetUnit = pTargetUnit;
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
    if (m_pTargetUnit == nullptr)
    {
        return GameMap::getInstance()->getTerrain(m_target.x(), m_target.y())->getUnit();
    }
    return m_pTargetUnit;
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
    return canBePerformed(m_actionID);
}

QVector<QPoint> GameAction::getMovePath()
{
    return m_Movepath;
}

bool GameAction::canBePerformed(QString actionID, bool emptyField)
{
    GameMap* pMap = GameMap::getInstance();
    Unit* pUnit = getTargetUnit();
    Building* pBuilding = getTargetBuilding();
    if (!emptyField)
    {
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
            if ((pBuilding->getOwner() == nullptr) ||
                (pBuilding->getOwner()->getPlayerID() != pMap->getCurrentPlayer()->getPlayerID()))
            {
                return false;
            }
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

bool GameAction::isFinalStep()
{
    return isFinalStep(m_actionID);
}

bool GameAction::isFinalStep(QString actionID)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "isFinalStep";
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

QString GameAction::getStepInputType()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getStepInputType";
    QJSValueList args1;
    args1 << pApp->getInterpreter()->newQObject(this);
    QJSValue ret = pApp->getInterpreter()->doFunction(m_actionID, function1, args1);
    if (ret.isString())
    {
        return ret.toString();
    }
    return "";
}

CursorData* GameAction::getStepCursor()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getStepCursor";
    CursorData* data = new CursorData();
    QJSValueList args1;
    args1 << pApp->getInterpreter()->newQObject(this);
    args1 << pApp->getInterpreter()->newQObject(data);
    QJSValue ret = pApp->getInterpreter()->doFunction(m_actionID, function1, args1);
    if (ret.isString())
    {
        data->setCursor(ret.toString());
    }
    return data;
}

MenuData* GameAction::getMenuStepData()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    MenuData* data = new MenuData;
    QString function1 = "getStepData";
    QJSValueList args1;
    args1 << pApp->getInterpreter()->newQObject(this);
    args1 << pApp->getInterpreter()->newQObject(data);
    QJSValue ret = pApp->getInterpreter()->doFunction(m_actionID, function1, args1);
    pApp->continueThread();
    return data;
}

MarkedFieldData* GameAction::getMarkedFieldStepData()
{
   MarkedFieldData* data = new MarkedFieldData;
   Mainapp* pApp = Mainapp::getInstance();
   QString function1 = "getStepData";
   QJSValueList args1;
   args1 << pApp->getInterpreter()->newQObject(this);
   args1 << pApp->getInterpreter()->newQObject(data);
   QJSValue ret = pApp->getInterpreter()->doFunction(m_actionID, function1, args1);
   return data;
}

quint32 GameAction::getSeed() const
{
    return seed;
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
    Unit* pUnit = pMap->getTerrain(actionTarget.x(), actionTarget.y())->getUnit();
    // ignore stealthed units
    if (pUnit != nullptr)
    {
        if (pUnit->isStealthed(pMap->getCurrentPlayer()))
        {
            return nullptr;
        }
    }
    return pUnit;
}

Building* GameAction::getMovementBuilding()
{
    GameMap* pMap = GameMap::getInstance();
    QPoint actionTarget = getActionTarget();
    return pMap->getTerrain(actionTarget.x(), actionTarget.y())->getBuilding();
}

qint32 GameAction::getCosts() const
{
    return costs;
}

void GameAction::setCosts(const qint32 &value)
{
    costs = value;
}

qint32 GameAction::getInputStep() const
{
    return inputStep;
}

void GameAction::setInputStep(const qint32 &value)
{
    inputStep = value;
}

void GameAction::serializeObject(QDataStream& stream)
{
    stream << getVersion();
    stream << m_actionID;
    stream << m_target;
    stream << static_cast<qint32>(m_Movepath.size());
    for (qint32 i = 0; i < m_Movepath.size(); i++)
    {
        stream << m_Movepath[i];
    }
    stream << inputStep;
    stream << costs;
    QByteArray data = buffer.data();
    stream << static_cast<qint32>(data.size());
    for (qint32 i = 0; i < data.size(); i++)
    {
        stream << static_cast<qint8>(data[i]);
    }
    stream << seed;
}

void GameAction::deserializeObject(QDataStream& stream)
{
    qint32 version;
    stream >> version;
    stream >> m_actionID;
    stream >> m_target;
    qint32 size = 0;
    stream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        QPoint point;
        stream >> point;
        m_Movepath.append(point);
    }
    stream >> inputStep;
    stream >> costs;
    stream >> size;
    buffer.seek(0);
    for (qint32 i = 0; i < size; i++)
    {
        qint8 value = 0;
        stream >> value;

        actionData << value;
    }
    stream >> seed;
}
