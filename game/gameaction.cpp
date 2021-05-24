#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "game/gameaction.h"
#include "game/gamemap.h"
#include "game/unit.h"
#include "game/building.h"
#include "game/player.h"
#include "game/co.h"

GameAction::GameAction()
    : m_target(-1, -1)
{
    setObjectName("GameAction");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_buffer.open(QIODevice::ReadWrite);
    m_seed = QRandomGenerator::global()->bounded(std::numeric_limits<quint32>::max());
}

GameAction::GameAction(QString actionID)
    : m_actionID(actionID),
      m_target(-1, -1)
{
    setObjectName("GameAction");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_buffer.open(QIODevice::ReadWrite);
    m_seed = QRandomGenerator::global()->bounded(std::numeric_limits<quint32>::max());
}

void GameAction::setSeed(quint32 seed)
{
    m_seed = seed;
}

GameAction::~GameAction()
{
}

void GameAction::deleteAction()
{
    delete this;
}

bool GameAction::getIsLocal() const
{
    return m_isLocal;
}

void GameAction::setIsLocal(bool value)
{
    m_isLocal = value;
}

QVector<QPoint> GameAction::getMultiTurnPath() const
{
    return m_MultiTurnPath;
}

void GameAction::reset()
{
    m_actionID = "";
    m_costs = 0;
    m_inputStep = 0;
    m_MultiTurnPath.clear();
    m_buffer.close();
    m_buffer.open(QIODevice::ReadWrite | QIODevice::Truncate);
}

qint64 GameAction::getSyncCounter() const
{
    return m_syncCounter;
}

void GameAction::setSyncCounter(const qint64 &syncCounter)
{
    m_syncCounter = syncCounter;
}

void GameAction::setMultiTurnPath(const QVector<QPoint> &MultiTurnPath)
{
    m_MultiTurnPath = MultiTurnPath;
}

void GameAction::setTargetUnit(Unit *pTargetUnit)
{
    m_pTargetUnit = pTargetUnit;
}

void GameAction::perform()
{
    if (Settings::getLogActions())
    {
        printAction();
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "perform";
    QJSValueList args1;
    args1 << pInterpreter->newQObject(this);
    pInterpreter->doFunction(m_actionID, function1, args1);
}

void GameAction::printAction()
{
    Console::print("Performing Action " + m_actionID, Console::eINFO);
    Console::print("Target X " + QString::number(m_target.x()) +
                   " Target Y " + QString::number(m_target.y()), Console::eINFO);
    Console::print("Costs " + QString::number(m_costs), Console::eINFO);
    Console::print("Seed " + QString::number(m_seed), Console::eINFO);
    Unit* pUnit = getTargetUnit();
    Building* pBuilding = getTargetBuilding();
    if (pUnit != nullptr)
    {
        Console::print("Unit " + pUnit->getUnitID(), Console::eINFO);
    }
    else if (pBuilding != nullptr)
    {
        Console::print("Building " + pBuilding->getBuildingID(), Console::eINFO);
    }
    if (m_Movepath.size() > 0)
    {
        Console::print("Moving to X " + QString::number(m_Movepath[0].x()) +
                " Moving to Y " + QString::number(m_Movepath[0].y()), Console::eINFO);
    }
    QString data;
    QByteArray bytes = m_buffer.data();
    for (qint32 i = 0; i < bytes.size(); i++)
    {
        data += "0x" + QString::number(bytes[i])+ " ";
    }
    Console::print("Data " + data, Console::eINFO);
}

qint32 GameAction::getPlayer() const
{
    return m_player;
}

void GameAction::setPlayer(qint32 player)
{
    m_player = player;
}

qint64 GameAction::getRoundTimerTime() const
{
    return m_roundTimerTime;
}

void GameAction::setRoundTimerTime(const qint64 &roundTimerTime)
{
    m_roundTimerTime = roundTimerTime;
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
    if (m_pTargetUnit == nullptr && GameMap::getInstance()->onMap(m_target.x(), m_target.y()))
    {
        return GameMap::getInstance()->getTerrain(m_target.x(), m_target.y())->getUnit();
    }
    return m_pTargetUnit;
}

Building* GameAction::getTargetBuilding()
{
    if (GameMap::getInstance()->onMap(m_target.x(), m_target.y()))
    {
        return GameMap::getInstance()->getTerrain(m_target.x(), m_target.y())->getBuilding();
    }
    else
    {
        return nullptr;
    }

}

void GameAction::setMovepath(QVector<QPoint> points, qint32 fuelCost)
{
    m_Movepath = points;
    m_costs = fuelCost;
}

bool GameAction::canBePerformed()
{
    return canBePerformed(m_actionID);
}

QVector<QPoint> GameAction::getMovePath()
{
    return m_Movepath;
}

qint32 GameAction::getMovePathLength()
{
    return m_Movepath.size();
}

bool GameAction::canBePerformed(QString actionID, bool emptyField)
{
    if (!actionID.isEmpty())
    {
        spGameMap pMap = GameMap::getInstance();
        if (pMap->getGameRules()->getAllowedActions().contains(actionID))
        {
            Unit* pUnit = getTargetUnit();
            Building* pBuilding = getTargetBuilding();
            if (!emptyField)
            {
                if (pUnit != nullptr)
                {
                    if (!pUnit->hasAction(actionID))
                    {
                        return false;
                    }
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
            Interpreter* pInterpreter = Interpreter::getInstance();
            QString function1 = "canBePerformed";
            QJSValueList args1;
            args1 << pInterpreter->newQObject(this);
            QJSValue ret = pInterpreter->doFunction(actionID, function1, args1);
            if (ret.isBool())
            {
                return ret.toBool();
            }
        }
    }
    return false;
}

bool GameAction::isFinalStep()
{
    return isFinalStep(m_actionID);
}

bool GameAction::isFinalStep(QString actionID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "isFinalStep";
    QJSValueList args1;
    args1 << pInterpreter->newQObject(this);
    QJSValue ret = pInterpreter->doFunction(actionID, function1, args1);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    return false;
}

QString GameAction::getActionText(QString actionID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getActionText";
    QJSValueList args1;
    QJSValue ret = pInterpreter->doFunction(actionID, function1, args1);
    if (ret.isString())
    {
        return ret.toString();
    }
    return "";
}

QString GameAction::getActionIcon(QString actionID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getIcon";
    QJSValueList args1;
    QJSValue ret = pInterpreter->doFunction(actionID, function1, args1);
    if (ret.isString())
    {
        return ret.toString();
    }
    return "";
}

QString GameAction::getStepInputType()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getStepInputType";
    QJSValueList args1;
    args1 << pInterpreter->newQObject(this);
    QJSValue ret = pInterpreter->doFunction(m_actionID, function1, args1);
    if (ret.isString())
    {
        return ret.toString();
    }
    return "";
}

bool GameAction::getRequiresEmptyField()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRequiresEmptyField";
    QJSValueList args1;
    args1 << pInterpreter->newQObject(this);
    QJSValue ret = pInterpreter->doFunction(m_actionID, function1, args1);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    return true;
}

spCursorData GameAction::getStepCursor()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getStepCursor";
    spCursorData data = spCursorData::create();
    QJSValueList args1;
    args1 << pInterpreter->newQObject(this);
    args1 << pInterpreter->newQObject(data.get());
    QJSValue ret = pInterpreter->doFunction(m_actionID, function1, args1);
    if (ret.isString())
    {
        data->setCursor(ret.toString());
    }
    return data;
}

spMenuData GameAction::getMenuStepData()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    spMenuData data = spMenuData::create();
    QString function1 = "getStepData";
    QJSValueList args1;
    args1 << pInterpreter->newQObject(this);
    args1 << pInterpreter->newQObject(data.get());
    pInterpreter->doFunction(m_actionID, function1, args1);
    return data;
}

spMarkedFieldData GameAction::getMarkedFieldStepData()
{
    spMarkedFieldData data = spMarkedFieldData::create();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getStepData";
    QJSValueList args1;
    args1 << pInterpreter->newQObject(this);
    args1 << pInterpreter->newQObject(data.get());
    pInterpreter->doFunction(m_actionID, function1, args1);
    return data;
}

quint32 GameAction::getSeed() const
{
    return m_seed;
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
    spGameMap pMap = GameMap::getInstance();
    QPoint actionTarget = getActionTarget();
    Unit* pUnit = nullptr;
    if (pMap->onMap(actionTarget.x(), actionTarget.y()))
    {
        pUnit = pMap->getTerrain(actionTarget.x(), actionTarget.y())->getUnit();
        // ignore stealthed units
        if (pUnit != nullptr)
        {
            if (pUnit->isStealthed(pMap->getCurrentPlayer()))
            {
                return nullptr;
            }
        }
    }
    return pUnit;
}

Building* GameAction::getMovementBuilding()
{
    spGameMap pMap = GameMap::getInstance();
    QPoint actionTarget = getActionTarget();
    Building* pBuilding = pMap->getTerrain(actionTarget.x(), actionTarget.y())->getBuilding();
    return pBuilding;
}

Terrain* GameAction::getMovementTerrain()
{
    spGameMap pMap = GameMap::getInstance();
    QPoint actionTarget = getActionTarget();
    Terrain* pTerrain = pMap->getTerrain(actionTarget.x(), actionTarget.y());
    return pTerrain;
}

qint32 GameAction::getCosts() const
{
    return m_costs;
}

void GameAction::setCosts(const qint32 &value)
{
    m_costs = value;
}

qint32 GameAction::getInputStep() const
{
    return m_inputStep;
}

void GameAction::setInputStep(const qint32 &value)
{
    m_inputStep = value;
}

void GameAction::serializeObject(QDataStream& stream) const
{
    stream << getVersion();
    stream << m_actionID;
    stream << m_target;
    stream << static_cast<qint32>(m_Movepath.size());
    for (qint32 i = 0; i < m_Movepath.size(); i++)
    {
        stream << m_Movepath[i];
    }
    stream << m_inputStep;
    stream << m_costs;
    QByteArray data = m_buffer.data();
    stream << static_cast<qint32>(data.size());
    for (qint32 i = 0; i < data.size(); i++)
    {
        stream << static_cast<qint8>(data[i]);
    }
    stream << m_seed;
    qint32 size = m_MultiTurnPath.size();
    stream << size;
    for (qint32 i = 0; i < size; i++)
    {
        stream << m_MultiTurnPath[i];
    }
    stream << m_syncCounter;
    stream << m_roundTimerTime;
}

void GameAction::deserializeObject(QDataStream& stream)
{
    qint32 version;
    stream >> version;
    stream >> m_actionID;
    stream >> m_target;
    qint32 size = 0;
    stream >> size;
    m_Movepath.clear();
    for (qint32 i = 0; i < size; i++)
    {
        QPoint point;
        stream >> point;
        m_Movepath.append(point);
    }
    stream >> m_inputStep;
    stream >> m_costs;
    stream >> size;
    m_buffer.seek(0);
    for (qint32 i = 0; i < size; i++)
    {
        qint8 value = 0;
        // stream out of recieved data
        stream >> value;
        // stream into action buffer
        m_actionData << value;
    }
    stream >> m_seed;
    if (version > 1)
    {
        qint32 size = 0;
        stream >> size;
        m_MultiTurnPath.clear();
        for (qint32 i = 0; i < size; i++)
        {
            QPoint point;
            stream >> point;
            m_MultiTurnPath.append(point);
        }
    }
    stream >> m_syncCounter;
    if (version > 2)
    {
        stream >> m_roundTimerTime;
    }
}
