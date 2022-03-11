#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/globalutils.h"

#include "game/gameaction.h"
#include "game/gamemap.h"
#include "game/unit.h"
#include "game/building.h"
#include "game/player.h"
#include "game/co.h"

GameAction::GameAction(GameMap* pMap)
    : m_target(-1, -1),
      m_pMap{pMap}
{
    setObjectName("GameAction");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_buffer.open(QIODevice::ReadWrite);
    m_seed = QRandomGenerator::global()->bounded(std::numeric_limits<quint32>::max());
}

GameAction::GameAction(const QString & actionID, GameMap* pMap)
    : m_actionID(actionID),
      m_target(-1, -1),
      m_pMap{pMap}
{
    setObjectName("GameAction");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_buffer.open(QIODevice::ReadWrite);
    m_seed = QRandomGenerator::global()->bounded(std::numeric_limits<quint32>::max());
}

void GameAction::setSeed(quint32 seed)
{
    m_seed = seed;
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
    m_perfomingUnit = getTargetUnit();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "perform";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_actionID, function1, args);
}

void GameAction::printAction()
{
    CONSOLE_PRINT("Performing Action " + m_actionID, Console::eINFO);
    CONSOLE_PRINT("Target X " + QString::number(m_target.x()) +
                   " Target Y " + QString::number(m_target.y()), Console::eINFO);
    CONSOLE_PRINT("Costs " + QString::number(m_costs), Console::eINFO);
    CONSOLE_PRINT("Seed " + QString::number(m_seed), Console::eINFO);
    Unit* pUnit = getTargetUnit();
    Building* pBuilding = getTargetBuilding();
    if (pUnit != nullptr)
    {
        CONSOLE_PRINT("Unit " + pUnit->getUnitID(), Console::eINFO);
    }
    else if (pBuilding != nullptr)
    {
        CONSOLE_PRINT("Building " + pBuilding->getBuildingID(), Console::eINFO);
    }
    if (m_Movepath.size() > 0)
    {
        CONSOLE_PRINT("Moving to X " + QString::number(m_Movepath[0].x()) +
                " Moving to Y " + QString::number(m_Movepath[0].y()), Console::eINFO);
    }
    auto bytes = m_buffer.data();
    QString data = GlobalUtils::getByteArrayString(bytes);
    CONSOLE_PRINT("Data " + data, Console::eINFO);
}

GameMap *GameAction::getMap() const
{
    return m_pMap;
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
    if (m_pMap != nullptr)
    {
        if (m_pTargetUnit == nullptr && m_pMap->onMap(m_target.x(), m_target.y()))
        {
            return m_pMap->getTerrain(m_target.x(), m_target.y())->getUnit();
        }
    }
    return m_pTargetUnit;
}

Building* GameAction::getTargetBuilding()
{    
    if (m_pMap != nullptr)
    {
        if (m_pMap->onMap(m_target.x(), m_target.y()))
        {
            return m_pMap->getTerrain(m_target.x(), m_target.y())->getBuilding();
        }
    }
    return nullptr;
}

void GameAction::setMovepath(const QVector<QPoint> & points, qint32 fuelCost)
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

bool GameAction::canBePerformed(const QString & actionID, bool emptyField)
{
    if (!actionID.isEmpty())
    {        
        if (m_pMap != nullptr &&
            m_pMap->getGameRules()->getAllowedActions().contains(actionID))
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
                    if ((pUnit->getOwner()->getPlayerID() != m_pMap->getCurrentPlayer()->getPlayerID()) &&
                        (!pUnit->getHasMoved()))
                    {
                        return false;
                    }
                }
                if ((pBuilding != nullptr) && (pUnit == nullptr))
                {
                    if ((pBuilding->getOwner() == nullptr) ||
                        (pBuilding->getOwner()->getPlayerID() != m_pMap->getCurrentPlayer()->getPlayerID()))
                    {
                        return false;
                    }
                }
            }
            Interpreter* pInterpreter = Interpreter::getInstance();
            QString function1 = "canBePerformed";
            QJSValueList args({pInterpreter->newQObject(this),
                               pInterpreter->newQObject(m_pMap)});
            QJSValue ret = pInterpreter->doFunction(actionID, function1, args);
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

bool GameAction::isFinalStep(const QString & actionID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "isFinalStep";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(actionID, function1, args);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    return false;
}

QString GameAction::getActionText(GameMap* pMap, const QString & actionID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getActionText";
    QJSValueList args({pInterpreter->newQObject(pMap)});
    QJSValue ret = pInterpreter->doFunction(actionID, function1, args);
    if (ret.isString())
    {
        return ret.toString();
    }
    return "";
}

QString GameAction::getActionIcon(GameMap* pMap, const QString & actionID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getIcon";
    QJSValueList args({pInterpreter->newQObject(pMap)});
    QJSValue ret = pInterpreter->doFunction(actionID, function1, args);
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
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_actionID, function1, args);
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
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_actionID, function1, args);
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
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(data.get()),
                       pInterpreter->newQObject(m_pMap)});
    QJSValue ret = pInterpreter->doFunction(m_actionID, function1, args);
    if (ret.isString())
    {
        data->setCursor(ret.toString());
    }
    return data;
}

spMenuData GameAction::getMenuStepData()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    spMenuData data = spMenuData::create(m_pMap);
    QString function1 = "getStepData";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(data.get()),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_actionID, function1, args);
    return data;
}

spMarkedFieldData GameAction::getMarkedFieldStepData()
{
    spMarkedFieldData data = spMarkedFieldData::create();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getStepData";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(data.get()),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_actionID, function1, args);
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
    QPoint actionTarget = getActionTarget();
    Unit* pUnit = nullptr;
    if (m_pMap->onMap(actionTarget.x(), actionTarget.y()))
    {
        pUnit = m_pMap->getTerrain(actionTarget.x(), actionTarget.y())->getUnit();
        // ignore stealthed units
        if (pUnit != nullptr)
        {
            if (pUnit->isStealthed(m_pMap->getCurrentPlayer()))
            {
                return nullptr;
            }
        }
    }
    return pUnit;
}

Building* GameAction::getMovementBuilding()
{    
    Building* pBuilding = nullptr;
    if (m_pMap != nullptr)
    {
        QPoint actionTarget = getActionTarget();
        pBuilding = m_pMap->getTerrain(actionTarget.x(), actionTarget.y())->getBuilding();
    }
    return pBuilding;
}

Terrain* GameAction::getMovementTerrain()
{    
    Terrain* pTerrain = nullptr;
    if (m_pMap != nullptr)
    {
        QPoint actionTarget = getActionTarget();
        pTerrain = m_pMap->getTerrain(actionTarget.x(), actionTarget.y());
    }
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
    CONSOLE_PRINT("storing game action", Console::eDEBUG);
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
    CONSOLE_PRINT("reading game action", Console::eDEBUG);
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
