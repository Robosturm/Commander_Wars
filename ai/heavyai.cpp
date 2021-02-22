#include <QSettings>

#include "coreengine/qmlvector.h"
#include "coreengine/console.h"
#include "coreengine/globalutils.h"

#include "ai/heavyai.h"

#include "game/player.h"
#include "game/gameaction.h"

#include "resource_management/unitspritemanager.h"

const qint32 HeavyAi::minSiloDamage = 7000;
const QString HeavyAi::heavyAiObject = "HEAVY_AI";

HeavyAi::HeavyAi()
    : CoreAI(GameEnums::AiTypes_Heavy),
      m_scoreInfos({
{ACTION_CAPTURE,  std::bind(&HeavyAi::scoreCapture, this, std::placeholders::_1)},
{ACTION_FIRE,     std::bind(&HeavyAi::scoreFire, this, std::placeholders::_1)},
                   }),
      m_InfluenceFrontMap(m_IslandMaps)
{
    m_timer.setSingleShot(false);
    connect(&m_timer, &QTimer::timeout, this, &HeavyAi::process, Qt::QueuedConnection);
    loadIni("heavy/heavy.ini");
}

void HeavyAi::readIni(QString name)
{
    if (QFile::exists(name))
    {
        QSettings settings(name, QSettings::IniFormat);
        settings.beginGroup("general");
        bool ok = false;
        m_minActionScore = settings.value("MinActionScore", 0.1f).toFloat(&ok);
        if(!ok)
        {
            m_minActionScore = 0.1f;
        }
        m_actionScoreVariant = settings.value("ActionScoreVariant", 0.05f).toFloat(&ok);
        if(!ok)
        {
            m_actionScoreVariant = 0.05f;
        }
    }
}

void HeavyAi::toggleAiPause()
{
    m_pause = !m_pause;
}

void HeavyAi::showFrontMap()
{
    m_InfluenceFrontMap.show();
}

void HeavyAi::showFrontLines()
{
    m_InfluenceFrontMap.showFrontlines();
}

void HeavyAi::hideFrontMap()
{
    m_InfluenceFrontMap.hide();
}

void HeavyAi::process()
{
    spQmlVectorBuilding pBuildings = m_pPlayer->getBuildings();
    pBuildings->randomize();
    if (m_pause)
    {
        m_timer.start(1000);
        return;
    }
    else
    {
        m_timer.stop();
    }
    if (useBuilding(pBuildings.get())){}
    else if (useCOPower(m_pPlayer->getUnits(), m_pPlayer->getEnemyUnits())){}
    else
    {
        setupTurn(pBuildings);
        turnMode = GameEnums::AiTurnMode_DuringDay;
        if (!selectActionToPerform())
        {
            // nothing we could do with our units try to move them or/and let them wait
            scoreActionWait();
            if (!selectActionToPerform())
            {
                scoreProduction();
                if (!buildUnits())
                {
                    endTurn();
                }
            }
        }
    }
}

void HeavyAi::endTurn()
{
    turnMode = GameEnums::AiTurnMode_EndOfDay;
    m_pUnits = nullptr;
    m_pEnemyUnits = nullptr;
    if (useCOPower(m_pPlayer->getUnits(), m_pPlayer->getEnemyUnits()))
    {
    }
    else
    {
        // end the turn for the player
        turnMode = GameEnums::AiTurnMode_StartOfDay;
        finishTurn();
    }
}

bool HeavyAi::selectActionToPerform()
{
    Console::print("HeavyAi selecting action to be performed", Console::eDEBUG);
    float bestScore = std::numeric_limits<float>::min();
    qint32 index = -1;
    for (qint32 i = 0; i < m_ownUnits.size(); ++i)
    {
        if (m_ownUnits[i].m_action.get() != nullptr &&
            m_ownUnits[i].m_score > bestScore)
        {
            bestScore = m_ownUnits[i].m_score;
            index = i;
        }
    }
    if (index >= 0)
    {
        m_updatePoints.append(m_ownUnits[index].m_action->getTarget());
        m_updatePoints.append(m_ownUnits[index].m_action->getMovePath()[0]);
        emit performAction(m_ownUnits[index].m_action);
        m_ownUnits[index].m_action = nullptr;
        m_ownUnits[index].m_score = 0;
        return true;
    }
    return false;
}

void HeavyAi::setupTurn(const spQmlVectorBuilding & buildings)
{
    bool startOfTurn = (m_pUnits.get() == nullptr);
    if (m_pUnits.get() == nullptr)
    {
        m_pUnits = m_pPlayer->getUnits();
        m_pUnits->sortShortestMovementRange(true);
        initUnits(m_pUnits.get(), m_ownUnits, false);
    }
    if (m_pEnemyUnits.get() == nullptr)
    {
        m_pEnemyUnits = m_pPlayer->getEnemyUnits();
        m_pEnemyUnits->randomize();
        initUnits(m_pEnemyUnits.get(), m_enemyUnits, true);
    }
    if (startOfTurn)
    {
        Console::print("HeavyAi initial start of turn calculation", Console::eDEBUG);
        createIslandMaps();

        // create influence map at the start of the turn
        m_InfluenceFrontMap.reset();
        m_InfluenceFrontMap.addBuildingInfluence();
        for (auto & unit : m_ownUnits)
        {
            m_InfluenceFrontMap.addUnitInfluence(unit.m_pUnit, unit.m_pPfs.get(), unit.m_movepoints);
        }
        for (auto & unit : m_enemyUnits)
        {
            m_InfluenceFrontMap.addUnitInfluence(unit.m_pUnit, unit.m_pPfs.get(), unit.m_movepoints);
        }
        m_InfluenceFrontMap.updateOwners();
        m_InfluenceFrontMap.findFrontLines();
        Console::print("HeavyAi front lines created", Console::eDEBUG);
        findHqThreads(buildings);
        Console::print("HeavyAi initial scoring actions for units", Console::eDEBUG);
        for (auto & unit : m_ownUnits)
        {
            scoreActions(unit);
        }
    }
    else
    {
        updateUnits();
    }
    qint32 cost = 0;
    m_pPlayer->getSiloRockettarget(2, 3, cost);
    m_missileTarget = (cost >= minSiloDamage);
}

void HeavyAi::createIslandMaps()
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    qint32 unitCount = pUnitSpriteManager->getCount();
    for (qint32 i = 0; i < unitCount; i++)
    {
        QString unitId = pUnitSpriteManager->getID(i);
        createIslandMap(pUnitSpriteManager->getMovementType(unitId), unitId);
    }
}

void HeavyAi::initUnits(QmlVectorUnit* pUnits, QVector<UnitData> & units, bool enemyUnits)
{
    units.clear();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        UnitData data;
        data.m_pUnit = pUnits->at(i);
        data.m_pPfs = new UnitPathFindingSystem(pUnits->at(i));
        data.m_movepoints = data.m_pUnit->getMovementpoints(data.m_pUnit->getPosition());
        data.m_pPfs->setMovepoints(data.m_movepoints * 2);
        data.m_pPfs->setIgnoreEnemies(enemyUnits);
        data.m_pPfs->explore();
        units.append(data);
    }
}

void HeavyAi::updateUnits()
{
    updateUnits(m_ownUnits, false);
    updateUnits(m_enemyUnits, true);
    m_updatePoints.clear();
}

void HeavyAi::updateUnits(QVector<UnitData> & units, bool enemyUnits)
{
    for (qint32 i = 0; i < units.size(); ++i)
    {
        if (units[i].m_pUnit->getHp() <= 0)
        {
            units.removeAt(i);
        }
    }
    QVector<qint32> updated;
    for (qint32 i = 0; i < m_updatePoints.size(); i++)
    {
        qint32 i2 = 0;
        while (i2 < units.size())
        {
            if (!updated.contains(i2))
            {
                if (qAbs(m_updatePoints[i].x() - units[i2].m_pUnit->getX()) +
                    qAbs(m_updatePoints[i].y() - units[i2].m_pUnit->getY()) <=
                    units[i2].m_pUnit->getMovementpoints(QPoint(units[i2].m_pUnit->getX(), units[i2].m_pUnit->getY())) + 2)
                {
                    units[i2].m_pPfs = new UnitPathFindingSystem(units[i2].m_pUnit);
                    units[i2].m_pPfs->setIgnoreEnemies(enemyUnits);
                    units[i2].m_pPfs->explore();
                    if (!enemyUnits)
                    {
                        scoreActions(units[i2]);
                    }
                    updated.push_back(i2);
                }
                ++i2;
            }
            else
            {
                ++i2;
            }

        }
    }
}

void HeavyAi::findHqThreads(const spQmlVectorBuilding & buildings)
{
    Console::print("Searching for HQ Threads", Console::eDEBUG);
    QVector<QVector3D> hqPositions;
    for (qint32 i = 0; i < buildings->size(); ++i)
    {
        const auto * pBuilding = buildings->at(i);
        if (pBuilding->getBuildingID() == "HQ")
        {
            hqPositions.append(QVector3D(pBuilding->getX(), pBuilding->getY(), 1));
        }
    }
    for (auto & enemy : m_enemyUnits)
    {
        if (isCaptureTransporterOrCanCapture(enemy.m_pUnit))
        {
            constexpr qint32 dayDistance = 3;
            QPoint pos = enemy.m_pUnit->getMapPosition();
            qint32 movePoints = enemy.m_pUnit->getMovementpoints(enemy.m_pUnit->getMapPosition());
            for (const auto & hqPos : hqPositions)
            {
                if (GlobalUtils::getDistance(pos, QPoint(hqPos.x(), hqPos.y())) <= dayDistance * movePoints)
                {
                    TargetedUnitPathFindingSystem pfs = TargetedUnitPathFindingSystem(enemy.m_pUnit, hqPositions, &m_MoveCostMap);
                    pfs.setIgnoreEnemies(true);
                    pfs.setMovepoints(movePoints * dayDistance);
                    pfs.setAbortOnCostExceed(true);
                    pfs.setFast(true);
                    pfs.setUseBasecosts(true);
                    pfs.explore();
                    QPoint targetFields = pfs.getReachableTargetField(movePoints * 3);
                    if (targetFields.x() >= 0)
                    {
                        // todo maybe we need to save more data here
                        enemy.m_threadLevel = ThreadLevel::Hq;
                        enemy.m_hqThread = targetFields;
                    }
                }
            }
        }
    }
}

bool HeavyAi::isCaptureTransporterOrCanCapture(Unit* pUnit)
{
    bool canCapture = false;
    if (pUnit->getActionList().contains(CoreAI::ACTION_CAPTURE))
    {
        canCapture = true;
    }
    else if (pUnit->getLoadedUnitCount() > 0)
    {
        for (qint32 i = 0; i < pUnit->getLoadedUnitCount(); ++i)
        {
            if (isCaptureTransporterOrCanCapture(pUnit->getLoadedUnit(i)))
            {
                canCapture = true;
                break;
            }
        }
    }
    return canCapture;
}

void HeavyAi::scoreActions(UnitData & unit)
{
    if (!unit.m_pUnit->getHasMoved())
    {
        const QVector<QString> forbiddenActions = {ACTION_WAIT};
        QVector<float> bestScores;
        QVector<spGameAction> bestActions;
        float bestScore = 0.0f;
        QStringList actions = unit.m_pUnit->getActionList();
        qint32 movePoints = unit.m_pUnit->getMovementpoints(unit.m_pUnit->getMapPosition());
        auto moveTargets = unit.m_pPfs->getAllNodePoints(movePoints);
        for (const auto & action : actions)
        {
            if (!forbiddenActions.contains(action))
            {
                FunctionType type;
                qint32 index = -1;
                getFunctionType(action, type, index);
                mutateActionForFields(unit, moveTargets, action, type, index,
                                      bestScore, bestScores, bestActions);
            }
        }
        if (bestActions.size() > 0)
        {
            qint32 item = GlobalUtils::randInt(0, bestScores.size() - 1);
            unit.m_score = bestScores[item];
            unit.m_action = bestActions[item];
        }
        else
        {
            unit.m_score = 0.0f;
            unit.m_action = nullptr;
        }
    }
}

void HeavyAi::mutateActionForFields(UnitData & unit, const QVector<QPoint> & moveTargets,
                                    QString action, FunctionType type, qint32 index,
                                    float & bestScore, QVector<float> & bestScores,
                                    QVector<spGameAction> & bestActions)
{
    for (const auto & target : moveTargets)
    {
        QVector<QPoint> path = unit.m_pPfs->getPath(target.x(), target.y());
        qint32 costs = unit.m_pPfs->getCosts(path);
        bool mutate = true;
        QVector<qint32> stepPosition;
        while (mutate)
        {
            qint32 step = 0;
            spGameAction pAction  = new GameAction();
            pAction->setActionID(action);
            pAction->setMovepath(path, costs);
            pAction->setTarget(QPoint(unit.m_pUnit->getX(), unit.m_pUnit->getY()));
            if (pAction->canBePerformed())
            {
                float score = 0;
                mutate = mutateAction(pAction, type, index, step, stepPosition, score);
                if (score > m_minActionScore)
                {
                    if (score > bestScore)
                    {
                        bestScore = score;
                        qint32 i = 0;
                        bestScores.append(score);
                        bestActions.append(pAction);
                        while (i < bestScores.size())
                        {
                            if (bestScores[i] - m_actionScoreVariant <= score && score <= bestScores[i] + m_actionScoreVariant)
                            {
                                i++;
                            }
                            else
                            {
                                bestScores.removeAt(i);
                                bestActions.removeAt(i);
                            }
                        }
                    }
                    else if (bestScore - m_actionScoreVariant <= score && score <= bestScore + m_actionScoreVariant)
                    {
                        bestScores.append(score);
                        bestActions.append(pAction);
                    }
                }
            }
            // stop mutating?
            if (stepPosition.size() == 0)
            {
                break;
            }
        }
    }
}

bool HeavyAi::mutateAction(spGameAction pAction, FunctionType type, qint32 functionIndex, qint32 & step, QVector<qint32> & stepPosition, float & score)
{
    bool ret = false;
    if (pAction->isFinalStep())
    {
        switch (type)
        {
            case FunctionType::JavaScript:
            {
                Interpreter* pInterpreter = Interpreter::getInstance();
                QJSValueList args;
                QJSValue obj = pInterpreter->newQObject(pAction.get());
                args << obj;
                QJSValue erg = pInterpreter->doFunction(HeavyAi::heavyAiObject, pAction->getActionID(), args);
                if (erg.isNumber())
                {
                    score = erg.toInt();
                }
                break;
            }
            case FunctionType::CPlusPlus:
            {
                score = m_scoreInfos[functionIndex].callback(pAction);
                break;
            }
        }
        ret = true;
    }
    else
    {
        // reset mutation for following steps
        for (qint32 i = step + 1; i < stepPosition.size(); ++i)
        {
            stepPosition[i] = 0;
        }
        QString stepType = pAction->getStepInputType();
        if (stepType.toUpper() == "MENU")
        {
            spMenuData pData = pAction->getMenuStepData();
            if (pData->validData())
            {
                qint32 nextStep = 0;
                if (step < stepPosition.size())
                {
                    nextStep = stepPosition[step] + 1;
                }
                else
                {
                    stepPosition.append(0);
                }
                const auto & enableList = pData->getEnabledList();
                const auto & costList = pData->getCostList();
                const auto & actionList = pData->getActionIDs();
                for (qint32 i = nextStep; i < actionList.size(); ++i)
                {
                    if (enableList[i])
                    {
                        CoreAI::addMenuItemData(pAction, actionList[i], costList[i]);
                        stepPosition[step] = i;
                        ret = mutateAction(pAction, type, functionIndex, step, stepPosition, score);
                        break;
                    }
                }
            }
        }
        else if (stepType.toUpper() == "FIELD")
        {
            MarkedFieldData* pData = pAction->getMarkedFieldStepData();
            QVector<QPoint>* pFields = pData->getPoints();
            qint32 nextStep = 0;
            if (step < stepPosition.size())
            {
                nextStep = stepPosition[step] + 1;
            }
            else
            {
                stepPosition.append(0);
            }
            for (qint32 i = nextStep; i < pFields->size(); ++i)
            {
                CoreAI::addSelectedFieldData(pAction, (*pFields)[i]);
                stepPosition[step] = i;
                ret = mutateAction(pAction, type, functionIndex, step, stepPosition, score);
                break;
            }
        }
    }
    return ret;
}

float HeavyAi::scoreCapture(spGameAction action)
{
    // todo
    return 20;
}

float HeavyAi::scoreFire(spGameAction action)
{
    // todo
    return 0;
}

void HeavyAi::getFunctionType(QString action, FunctionType & type, qint32 & index)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    index = -1;
    if (pInterpreter->exists(heavyAiObject, action))
    {
        type = FunctionType::JavaScript;
    }
    else
    {
        for (qint32 i = 0; i < m_scoreInfos.size(); ++i)
        {
            if (m_scoreInfos[i].m_actionId == action)
            {
                type = FunctionType::CPlusPlus;
                index = i;
                break;
            }
        }
    }
}

void HeavyAi::getBasicFieldInputVector(spGameAction action, QVector<double> & data)
{
    // todo
}

void HeavyAi::scoreActionWait()
{
    Console::print("HeavyAi scoring wait actions if needed", Console::eDEBUG);
    for (auto & unit : m_ownUnits)
    {
        if (!unit.m_pUnit->getHasMoved() &&
            unit.m_action.get() == nullptr)
        {
            QVector<float> bestScores;
            QVector<spGameAction> bestActions;
            float bestScore = 0.0f;
            QString action = ACTION_WAIT;
            spGameAction pAction = nullptr;
            FunctionType type;
            qint32 index = -1;
            getFunctionType(action, type, index);
            qint32 movePoints = unit.m_pUnit->getMovementpoints(unit.m_pUnit->getMapPosition());
            auto moveTargets = unit.m_pPfs->getAllNodePoints(movePoints);
            QVector<QVector3D> targets;
            getMoveTargets(unit, targets);
            m_currentTargetefPfs = new TargetedUnitPathFindingSystem(unit.m_pUnit, targets, &m_MoveCostMap);
            m_currentTargetefPfs->explore();
            mutateActionForFields(unit, moveTargets, action, type, index,
                                  bestScore, bestScores, bestActions);
            m_currentTargetefPfs = nullptr;
            if (bestActions.size() > 0)
            {
                qint32 item = GlobalUtils::randInt(0, bestScores.size() - 1);
                unit.m_score = bestScores[item];
                unit.m_action = bestActions[item];
            }
            else
            {
                unit.m_score = 0.0f;
                unit.m_action = nullptr;
            }
        }
    }
}

void HeavyAi::getMoveTargets(UnitData & unit, QVector<QVector3D> & targets)
{
    // todo
}

float HeavyAi::scoreWait(spGameAction action)
{
    return 0.0f;
    // todo
}

void HeavyAi::scoreProduction()
{
    // todo
}

void HeavyAi::getProductionInputVector(Building* pBuilding, Unit* pUnit)
{
    // todo
    QVector<double> in(BuildingEntry::MaxSize);
    QStringList actionList = pUnit->getActionList();
    if (pUnit->getBaseMaxRange() > 1)
    {
        in[BuildingEntry::BasicAttackRange] = 1;
    }
    else
    {
        in[BuildingEntry::BasicAttackRange] = -1;
    }
    if (actionList.contains(ACTION_CAPTURE))
    {
        in[BuildingEntry::CaptureUnit] = 1;
    }
    else
    {
        in[BuildingEntry::CaptureUnit] = 1;
    }
    float value = getAiCoUnitMultiplier(m_pPlayer->getCO(0), pUnit);
    value += getAiCoUnitMultiplier(m_pPlayer->getCO(1), pUnit);
    in[BuildingEntry::CoUnitValue] = value / CO::MAX_CO_UNIT_VALUE;
    float maxMovementpoints = 10;
    in[BuildingEntry::Movementpoints] = pUnit->getMovementpoints(QPoint(pBuilding->getX(), pBuilding->getY())) / maxMovementpoints;
}

bool HeavyAi::buildUnits()
{
    // todo
    return false;
}
