#include <QSettings>

#include "coreengine/qmlvector.h"
#include "coreengine/console.h"
#include "coreengine/globalutils.h"

#include "ai/heavyai.h"

#include "game/player.h"
#include "game/gameaction.h"
#include "game/gamemap.h"

#include "resource_management/unitspritemanager.h"
#include "resource_management/weaponmanager.h"

const qint32 HeavyAi::minSiloDamage = 7000;
constexpr const char* const NeuralNetworkFileEnding = ".net";
constexpr const char* const NeuralNetworkPath = "aidata/heavy/";
const QStringList HeavyAi::NeuralNetworkNames = {"Production"};

// normally i'm not a big fan of this but else the function table gets unreadable
using namespace std::placeholders;


HeavyAi::HeavyAi(QString type)
    : CoreAI(GameEnums::AiTypes_Heavy),
      m_scoreInfos({
{ACTION_CAPTURE,  std::bind(&HeavyAi::scoreCapture,     this,   _1, _2)},
{ACTION_FIRE,     std::bind(&HeavyAi::scoreFire,        this,   _1, _2)},
{ACTION_WAIT,     std::bind(&HeavyAi::scoreWait,        this,   _1, _2)},
                   }),
      m_InfluenceFrontMap(m_IslandMaps),
      m_aiName(type)
{
    setObjectName("HeavyAi");
    m_timer.setSingleShot(false);
    connect(&m_timer, &QTimer::timeout, this, &HeavyAi::process, Qt::QueuedConnection);
    loadIni("heavy/" + m_aiName.toLower() + ".ini");
    loadNeuralNetworks();
    if (NeuralNetworkNames.length() != NeuralNetworks::Max)
    {
        oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "Missing Enum name mapping");
    }
}

void HeavyAi::loadNeuralNetworks()
{
    loadNeuralNetwork(NeuralNetworkNames[NeuralNetworks::Production], m_neuralNetworks[NeuralNetworks::Production], static_cast<qint32>(BuildingEntry::MaxSize), 10);
}

void HeavyAi::loadNeuralNetwork(QString netName, spNeuralNetwork & network, qint32 inputVectorSize, qint32 netDepth)
{
    network = spNeuralNetwork::create();
    network->setNetworkName(netName);
    QString baseName = NeuralNetworkPath + netName + m_aiName + NeuralNetworkFileEnding;
    QStringList searchFiles;
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        searchFiles.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + Settings::getMods().at(i) + baseName);
        searchFiles.append(Settings::getUserPath() + Settings::getMods().at(i) + baseName);
    }
    searchFiles.append("resources/" + baseName);
    searchFiles.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + "/resources/" + baseName);
    bool found = false;
    for (qint32 i = 0; i < searchFiles.size(); i++)
    {
        if (QFile::exists(searchFiles[i]))
        {
            QFile file(searchFiles[i]);
            file.open(QIODevice::ReadOnly);
            QDataStream stream(&file);
            network->deserializeObject(stream);
            found = true;
            break;
        }
    }
    if (!found)
    {
        QMap<QString, double> parameters;
        parameters.insert(Layer::LAYER_PARAMETER_TYPE, static_cast<double>(Layer::LayerType::INPUT));
        parameters.insert(Layer::LAYER_PARAMETER_ACTIVATION, static_cast<double>(Neuron::ActivationFunction::SIGMOID));
        parameters.insert(Layer::LAYER_PARAMETER_SIZE, static_cast<double>(inputVectorSize));
        network->addLayer(parameters);
        parameters.insert(Layer::LAYER_PARAMETER_TYPE, static_cast<double>(Layer::LayerType::STANDARD));
        for (qint32 i = 0; i < netDepth; ++i)
        {
            network->addLayer(parameters);
        }
        parameters.insert(Layer::LAYER_PARAMETER_TYPE, static_cast<double>(Layer::LayerType::OUTPUT));
        parameters[Layer::LAYER_PARAMETER_SIZE] = 1;
        network->addLayer(parameters);
        network->autogenerate();
        QFile file("resources/" + baseName);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        network->serializeObject(stream);
    }
}

void HeavyAi::saveNeuralNetwork(qint32 network)
{
    if (network >= 0 && network < m_neuralNetworks.size())
    {
        QFile file(QString("resources/") + NeuralNetworkPath + NeuralNetworkNames[network] + m_aiName + NeuralNetworkFileEnding);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        m_neuralNetworks[network]->serializeObject(stream);
    }
    else
    {
        Console::print("HeavyAi::saveNeuralNetwork invalid index " + QString::number(network), Console::eDEBUG);
    }
}

QString HeavyAi::getNeuralNetworkName(qint32 network)
{
    if (network >= 0 && network < m_neuralNetworks.size())
    {
        return m_neuralNetworks[network]->getNetworkName();
    }
    else
    {
        Console::print("HeavyAi::getNeuralNetworkName invalid index " + QString::number(network), Console::eDEBUG);
    }
    return "";
}

void HeavyAi::mutateNeuralNetwork(qint32 network, double mutationChance)
{
    if (network >= 0 && network < m_neuralNetworks.size())
    {
        m_neuralNetworks[network]->mutateAllWeights(mutationChance);
    }
    else
    {
        Console::print("HeavyAi::mutateNeuralNetwork invalid index " + QString::number(network), Console::eDEBUG);
    }
}

void HeavyAi::combineAi(QStringList aisToUse)
{
    Console::print("HeavyAi::combineAi", Console::eDEBUG);
    for (qint32 i = 0; i < NeuralNetworks::Max; ++i)
    {
        qint32 item = GlobalUtils::randInt(0, aisToUse.length() - 1);
        QString netName = NeuralNetworkNames[i];
        QString targetName = QString("resources/") + NeuralNetworkPath + netName + m_aiName + NeuralNetworkFileEnding;
        QFile::remove(targetName);
        QFile::copy(QString("resources/") + NeuralNetworkPath + netName + aisToUse[item] + NeuralNetworkFileEnding, targetName);
    }
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
        m_stealthDistanceMultiplier = settings.value("StealthDistanceMultiplier", 2.00f).toFloat(&ok);
        if(!ok)
        {
            m_actionScoreVariant = 2.0f;
        }
        else if (m_stealthDistanceMultiplier < 1.0f)
        {
            m_stealthDistanceMultiplier = 1.0f;
        }
        m_alliedDistanceModifier = settings.value("AlliedDistanceModifier", 5.0f).toFloat(&ok);
        if(!ok)
        {
            m_actionScoreVariant = 5.0f;
        }
        m_maxMovementpoints = settings.value("MaxMovementpoints", 20.0f).toFloat(&ok);
        if(!ok)
        {
            m_maxMovementpoints = 20.0f;
        }
        m_maxProductionTurnRange = settings.value("MaxProductionTurnRange", 4.0f).toFloat(&ok);
        if(!ok)
        {
            m_maxProductionTurnRange = 4.0f;
        }
        m_maxFirerange = settings.value("MaxFirerange", 10).toFloat(&ok);
        if(!ok)
        {
            m_maxFirerange = 10.0f;
        }
        m_primaryEnemyMultiplier = settings.value("PrimaryEnemyMultiplier", 1.2f).toFloat(&ok);
        if(!ok)
        {
            m_primaryEnemyMultiplier = 1.2f;
        }
        m_maxLoadingPlace = settings.value("MaxLoadingPlace", 4.0f).toFloat(&ok);
        if(!ok)
        {
            m_maxLoadingPlace = 4.0f;
        }
        m_notAttackableDamage = settings.value("NotAttackableDamage", 30.0f).toFloat(&ok);
        if(!ok)
        {
            m_notAttackableDamage = 30.0f;
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
    spQmlVectorBuilding pEnemyBuildings = m_pPlayer->getEnemyBuildings();
    pEnemyBuildings->randomize();
    if (m_pause)
    {
        setupTurn(pBuildings);
        m_timer.start(1000);
        return;
    }
    else
    {
        m_timer.stop();
    }
    if (useBuilding(pBuildings)){}
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
                if (!buildUnits(pBuildings, m_pUnits, m_pEnemyUnits, pEnemyBuildings))
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
        initUnits(m_pUnits, m_ownUnits, false);
    }
    if (m_pEnemyUnits.get() == nullptr)
    {
        m_pEnemyUnits = m_pPlayer->getEnemyUnits();
        m_pEnemyUnits->randomize();
        initUnits(m_pEnemyUnits, m_enemyUnits, true);
    }
    if (startOfTurn)
    {
        Console::print("HeavyAi initial start of turn calculation", Console::eDEBUG);
        createIslandMaps();

        // create influence map at the start of the turn
        m_InfluenceFrontMap.reset();
        m_InfluenceFrontMap.setOwner(m_pPlayer);
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
        m_InfluenceFrontMap.calculateGlobalData();
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

void HeavyAi::initUnits(spQmlVectorUnit pUnits, QVector<UnitData> & units, bool enemyUnits)
{
    units.clear();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        UnitData data;
        data.m_pUnit = pUnits->at(i);
        data.m_pPfs = spUnitPathFindingSystem::create(pUnits->at(i));
        data.m_movepoints = data.m_pUnit->getMovementpoints(data.m_pUnit->getPosition());
        data.m_pPfs->setMovepoints(data.m_movepoints * 2);
        data.m_pPfs->setIgnoreEnemies(enemyUnits);
        data.m_pPfs->explore();
        units.append(data);
    }
}

void HeavyAi::updateUnits()
{
    updateUnits(m_ownUnits, m_pUnits, false);
    updateUnits(m_enemyUnits, m_pEnemyUnits, true);
    m_updatePoints.clear();
}

void HeavyAi::updateUnits(QVector<UnitData> & units, spQmlVectorUnit & pUnits, bool enemyUnits)
{
    qint32 i = 0;
    while (i < units.size())
    {
        if (units[i].m_pUnit->getHp() <= 0)
        {
            units.removeAt(i);
        }
        else
        {
            ++i;
        }
    }
    i = 0;
    while (i < pUnits->size())
    {
        if (pUnits->at(i)->getHp() <= 0)
        {
            pUnits->removeAt(i);
        }
        else
        {
            ++i;
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
                if (qAbs(m_updatePoints[i].x() - units[i2].m_pUnit->Unit::getX()) +
                    qAbs(m_updatePoints[i].y() - units[i2].m_pUnit->Unit::getY()) <=
                    units[i2].m_pUnit->getMovementpoints(QPoint(units[i2].m_pUnit->Unit::getX(), units[i2].m_pUnit->Unit::getY())) + 2)
                {
                    units[i2].m_pPfs = spUnitPathFindingSystem::create(units[i2].m_pUnit);
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
            hqPositions.append(QVector3D(pBuilding->Building::getX(), pBuilding->Building::getY(), 1));
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
        auto moveTargets = unit.m_pPfs->getAllNodePoints(unit.m_movepoints + 1);
        for (const auto & action : actions)
        {
            if (!forbiddenActions.contains(action) ||
                (action == ACTION_WAIT && actions.contains(ACTION_CAPTURE)))
            {
                if (action == ACTION_WAIT)
                {
                    prepareWaitPfs(unit, actions);
                }
                FunctionType type = FunctionType::CPlusPlus;
                qint32 index = -1;
                getFunctionType(action, type, index);
                if (index >= 0)
                {
                    mutateActionForFields(unit, moveTargets, action, type, index,
                                          bestScore, bestScores, bestActions);
                }
            }
        }
        m_currentTargetedfPfs = nullptr;
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

void HeavyAi::prepareWaitPfs(UnitData & unitData, QStringList & actions)
{
    if (m_currentTargetedfPfs.get() == nullptr)
    {
        QVector<QVector3D> targets;
        getMoveTargets(unitData, actions, targets);
        m_currentTargetedfPfs = spTargetedUnitPathFindingSystem::create(unitData.m_pUnit, targets, &m_MoveCostMap);
        m_currentTargetedfPfs->explore();
    }
}

void HeavyAi::mutateActionForFields(UnitData & unitData, const QVector<QPoint> & moveTargets,
                                    QString action, FunctionType type, qint32 index,
                                    float & bestScore, QVector<float> & bestScores,
                                    QVector<spGameAction> & bestActions)
{
    for (const auto & target : moveTargets)
    {
        QVector<QPoint> path = unitData.m_pPfs->getPath(target.x(), target.y());
        qint32 costs = unitData.m_pPfs->getCosts(path);
        bool mutate = true;
        QVector<qint32> stepPosition;
        while (mutate)
        {
            qint32 step = 0;
            spGameAction pAction  = spGameAction::create();
            pAction->setActionID(action);
            pAction->setMovepath(path, costs);
            pAction->setTarget(QPoint(unitData.m_pUnit->Unit::getX(), unitData.m_pUnit->Unit::getY()));
            if (pAction->canBePerformed())
            {
                float score = 0;
                mutate = mutateAction(pAction, unitData, type, index, step, stepPosition, score);
                if (score > 1.0f)
                {
                    // cap scoring
                    score = 1.0f;
                }
                if (score > m_minActionScore)
                {
                    if (score > bestScore)
                    {
                        bestScore = score;
                        qint32 i = 0;
                        while (i < bestScores.size())
                        {
                            if (score < bestScores[i] - m_actionScoreVariant)
                            {
                                bestScores.removeAt(i);
                                bestActions.removeAt(i);
                            }
                            else
                            {
                                ++i;
                            }
                        }
                        bestScores.append(score);
                        bestActions.append(pAction);
                    }
                    else if (bestScore - m_actionScoreVariant <= score)
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

bool HeavyAi::mutateAction(spGameAction pAction, UnitData & unitData, FunctionType type, qint32 functionIndex, qint32 & step, QVector<qint32> & stepPosition, float & score)
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
                QJSValue erg = pInterpreter->doFunction(m_aiName, pAction->getActionID(), args);
                if (erg.isNumber())
                {
                    score = erg.toNumber();
                }
                break;
            }
            case FunctionType::CPlusPlus:
            {
                score = m_scoreInfos[functionIndex].callback(pAction, unitData);
                break;
            }
            default:
            {
                oxygine::handleErrorPolicy(oxygine::ep_show_error, "HeavyAi::mutateAction invalid calculation type");
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
                        ret = mutateAction(pAction, unitData, type, functionIndex, step, stepPosition, score);
                        break;
                    }
                }
            }
        }
        else if (stepType.toUpper() == "FIELD")
        {
            spMarkedFieldData pData = pAction->getMarkedFieldStepData();
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
                ret = mutateAction(pAction, unitData, type, functionIndex, step, stepPosition, score);
                break;
            }
        }
    }
    return ret;
}

float HeavyAi::scoreCapture(spGameAction action, UnitData & unitData)
{
    // todo
    return 1;
}

float HeavyAi::scoreFire(spGameAction action, UnitData & unitData)
{
    // todo
    return 0.9;
}

void HeavyAi::getFunctionType(QString action, FunctionType & type, qint32 & index)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    index = -1;
    if (pInterpreter->exists(m_aiName, action))
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
            QStringList actions = unit.m_pUnit->getActionList();
            if (actions.contains(ACTION_WAIT))
            {
                QVector<float> bestScores;
                QVector<spGameAction> bestActions;
                float bestScore = 0.0f;
                QString action = ACTION_WAIT;
                spGameAction pAction = nullptr;
                FunctionType type = FunctionType::CPlusPlus;
                qint32 index = -1;
                getFunctionType(action, type, index);
                auto moveTargets = unit.m_pPfs->getAllNodePoints(unit.m_movepoints + 1);
                prepareWaitPfs(unit, actions);
                mutateActionForFields(unit, moveTargets, action, type, index,
                                      bestScore, bestScores, bestActions);
                m_currentTargetedfPfs = nullptr;
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
}

void HeavyAi::getMoveTargets(UnitData & unit, QStringList & actions, QVector<QVector3D> & targets)
{
    // todo
    spGameMap pMap = GameMap::getInstance();
    qint32 mapWidth = pMap->getMapWidth();
    qint32 mapHeight = pMap->getMapHeight();
    qint32 unitIslandIdx = getIslandIndex(unit.m_pUnit);
    qint32 minFirerange = unit.m_pUnit->getMinRange(unit.m_pUnit->getPosition());
    qint32 maxFirerange = unit.m_pUnit->getMaxRange(unit.m_pUnit->getPosition());
    spQmlVectorPoint pTargetFields = GlobalUtils::getCircle(minFirerange, maxFirerange);
    for (qint32 x = 0; x < mapWidth; ++x)
    {
        for (qint32 y = 0; y < mapHeight; ++y)
        {
            if (unit.m_pUnit->canMoveOver(x, y) &&
                onSameIsland(unitIslandIdx, unit.m_pUnit->Unit::getX(), unit.m_pUnit->Unit::getY(), x, y))
            {
                Terrain* pTerrain = pMap->getTerrain(x, y);
                addCaptureTargets(actions, pTerrain, targets);
                addAttackTargets(unit.m_pUnit, pTerrain, pTargetFields.get(), targets);
                // todo implement that list
                //                addRepairTargets();
                //                addSupportTargets();
                //                // transporting stuff
                //                addCaptureTransporterTargets();
                //                addTransporterTargets();
                //                addUnloadTargets(); // capture and attack once
                //                addLoadingTargets();
                //                // we need reparing / supplying
                //                addRepairTargets();
                //                addRefillTargets();
                //                addFlareTargets();
                //                addOoziumTargets();
                //                addBlackbombTargets();
                //                addCustomTargets();
            }
        }
    }
}

void HeavyAi::addCaptureTargets(const QStringList & actions,
                                Terrain* pTerrain, QVector<QVector3D>& targets)
{
    if (actions.contains(ACTION_CAPTURE) ||
        actions.contains(ACTION_MISSILE))
    {
        qint32 x = pTerrain->Terrain::getX();
        qint32 y = pTerrain->Terrain::getY();
        bool missileTarget = hasMissileTarget();
        Building* pBuilding = pTerrain->getBuilding();
        if (pBuilding != nullptr &&
            pBuilding->isCaptureOrMissileBuilding(missileTarget) &&
            pBuilding->getTerrain()->getUnit() == nullptr &&
            pBuilding->isEnemyBuilding(m_pPlayer))
        {
            qint32 captureDistanceModifier = getMovingToCaptureDistanceModifier();
            if (captureDistanceModifier < 1)
            {
                captureDistanceModifier = 1;
            }
            QVector3D possibleTarget(x, y, captureDistanceModifier);
            if (!targets.contains(possibleTarget))
            {
                targets.append(possibleTarget);
            }
        }
    }
}

qint32 HeavyAi::getMovingToCaptureDistanceModifier()
{
    // todo
    return 1.0f;
}

void HeavyAi::addAttackTargets(Unit* pUnit, Terrain* pTerrain, QmlVectorPoint* pTargetFields, QVector<QVector3D> & targets)
{
    spGameMap pMap = GameMap::getInstance();
    qint32 x = pTerrain->Terrain::getX();
    qint32 y = pTerrain->Terrain::getY();
    for (qint32 i = 0; i < pTargetFields->size(); ++i)
    {
        qint32 targetX = pTargetFields->at(i).x() + x;
        qint32 targetY = pTargetFields->at(i).y() + y;
        if (pMap->onMap(targetX, targetY))
        {
            Terrain* pTargetTerrain = pMap->getTerrain(targetX, targetY);
            Unit* pEnemy = pTargetTerrain->getUnit();
            if (pEnemy != nullptr &&
                pUnit->isAttackable(pEnemy, true))
            {
                Unit* pTargetUnit = pTerrain->getUnit();
                float alliedmultiplier = 1.0f;
                if (pTargetUnit != nullptr &&
                    pTargetUnit->getOwner()->checkAlliance(m_pPlayer) == GameEnums::Alliance_Friend)
                {
                    alliedmultiplier = m_alliedDistanceModifier;
                }
                qint32 stealthMalus = 0;
                bool terrainHide = false;
                if (pEnemy->isStatusStealthedAndInvisible(m_pPlayer, terrainHide))
                {
                    stealthMalus = m_stealthDistanceMultiplier;
                    if (terrainHide)
                    {
                        stealthMalus /=  2;
                    }
                }
                qint32 attackDistanceModifier = getMovingToAttackDistanceModifier();
                QVector3D possibleTarget(x, y, (stealthMalus + attackDistanceModifier) * alliedmultiplier);
                if (!targets.contains(possibleTarget))
                {
                    targets.append(possibleTarget);
                    break;
                }
            }
            else
            {
                Building* pBuilding = pTargetTerrain->getBuilding();
                if ((pBuilding != nullptr &&
                     pBuilding->getHp() > 0 &&
                     m_pPlayer->isEnemy(pBuilding->getOwner()) &&
                     pUnit->isEnvironmentAttackable(pBuilding->getBuildingID())) ||
                    (m_enableNeutralTerrainAttack &&
                     isAttackOnTerrainAllowed(pTerrain) &&
                     pUnit->isEnvironmentAttackable(pTerrain->getID())))
                {
                    Unit* pTargetUnit = pTerrain->getUnit();
                    float alliedmultiplier = 1.0f;
                    if (pTargetUnit != nullptr &&
                        pTargetUnit->getOwner()->checkAlliance(m_pPlayer) == GameEnums::Alliance_Friend)
                    {
                        alliedmultiplier = m_alliedDistanceModifier;
                    }
                    qint32 attackDistanceModifier = getMovingToAttackEnvironmentDistanceModifier();
                    QVector3D possibleTarget(x, y, (attackDistanceModifier) * alliedmultiplier);
                    if (!targets.contains(possibleTarget))
                    {
                        targets.append(possibleTarget);
                        break;
                    }
                }
            }
        }
    }
}

qint32 HeavyAi::getMovingToAttackDistanceModifier()
{
    // todo
    return 1.0f;
}

qint32 HeavyAi::getMovingToAttackEnvironmentDistanceModifier()
{
    // todo
    return 1.0f;
}

float HeavyAi::scoreWait(spGameAction action, UnitData & unitData)
{
    float score = 0.0f;
    auto targetPath = m_currentTargetedfPfs->getTargetPath();
    QPoint target = action->getActionTarget();
    if (targetPath.contains(target))
    {
        QVector<QPoint> movePath = unitData.m_pPfs->getClosestReachableMovePath(target);
        if (movePath.size() > 0)
        {
            // todo
            score = m_minActionScore + static_cast<float>(movePath.size() - 1) * 0.5f / static_cast<float>(unitData.m_movepoints);
        }
    }
    return score;
}

bool HeavyAi::buildUnits(spQmlVectorBuilding pBuildings, spQmlVectorUnit pUnits,
                         spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings)
{
    scoreUnitBuildings(pBuildings, pUnits, pEnemyUnits, pEnemyBuildings);
    float bestScore = std::numeric_limits<float>::min();
    qint32 index = -1;
    for (qint32 i = 0; i < m_BuildingData.size(); ++i)
    {
        if (m_BuildingData[i].m_action.get() != nullptr &&
            m_BuildingData[i].m_score > bestScore)
        {
            bestScore = m_BuildingData[i].m_score;
            index = i;
        }
    }
    if (index >= 0)
    {
        auto & item = m_BuildingData[index];
        Console::print("HeavyAi::buildUnits " + item.buildingDataInput[item.m_selectedData].unitId + " with scored value " + QString::number(bestScore), Console::eDEBUG);
        m_updatePoints.append(item.m_action->getTarget());
        emit performAction(item.m_action);
        item.m_action = nullptr;
        item.m_score = 0;
        if (item.buildingDataInput[item.m_selectedData].unitBuildingDataInput[BuildingEntry::CanAttackImmuneUnitRatio] > 0)
        {
            // reset data
            m_BuildingData.clear();
        }
        return true;
    }
    return false;
}

void HeavyAi::scoreUnitBuildings(spQmlVectorBuilding pBuildings, spQmlVectorUnit pUnits,
                                 spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings)
{
    QVector<double> data = getGlobalBuildInfo(pBuildings, pUnits, pEnemyUnits, pEnemyBuildings);
    QVector<Unit*> immuneUnits;
    getImmuneUnits(pUnits, pEnemyUnits, immuneUnits);
    // do the actual scoring
    qint32 funds = m_pPlayer->getFunds();
    for (auto & building : m_BuildingData)
    {
        if (building.buildingDataInput.size() == 0)
        {
            createUnitBuildData(building, data, funds, immuneUnits);
        }
        else
        {
            updateUnitBuildData(building, data, funds);
        }
        scoreBuildingProductionData(building);
    }
}

void HeavyAi::scoreBuildingProductionData(HeavyAi::BuildingData & building)
{
    double bestScore = 0.0;
    QVector<qint32> bestItems;
    QVector<double> scores;
    building.m_action = nullptr;
    building.m_score = 0;
    for (qint32 i = 0; i < building.buildingDataInput.size(); ++i)
    {
        if (building.buildingDataInput[i].enabled)
        {
            auto score = m_neuralNetworks[NeuralNetworks::Production]->predict(building.buildingDataInput[i].unitBuildingDataInput);
            if (score[0] > bestScore && score[0] >= m_minActionScore)
            {
                bestScore = score[0];
                qint32 i2 = 0;
                while (i2 < bestItems.size())
                {
                    if (scores[i2] < bestScore - m_actionScoreVariant)
                    {
                        scores.removeAt(i2);
                        bestItems.removeAt(i2);
                    }
                    else
                    {
                        ++i2;
                    }
                }
                scores.append(bestScore);
                bestItems.append(i);
            }
            else if (bestScore - m_actionScoreVariant <= score[0])
            {
                bestItems.append(i);
                scores.append(score[0]);
            }
        }
    }
    if (bestItems.size() > 0)
    {
        qint32 item = GlobalUtils::randInt(0, bestItems.size() - 1);
        building.m_score = bestScore;
        building.m_selectedData = item;
        building.m_action = spGameAction::create(CoreAI::ACTION_BUILD_UNITS);
        building.m_action->setTarget(QPoint(building.m_pBuilding->Building::getX(), building.m_pBuilding->Building::getY()));
        CoreAI::addMenuItemData(building.m_action, building.buildingDataInput[item].unitId, building.buildingDataInput[item].cost);
    }
}

void HeavyAi::createUnitBuildData(BuildingData & building, QVector<double> & data, qint32 funds, const QVector<Unit*> & immuneUnits)
{
    // create new
    spGameAction pAction = spGameAction::create(ACTION_BUILD_UNITS);
    qint32 x = building.m_pBuilding->Building::getX();
    qint32 y = building.m_pBuilding->Building::getY();
    pAction->setTarget(QPoint(x, y));
    if (pAction->canBePerformed())
    {
        // we're allowed to build units here
        spMenuData pData = pAction->getMenuStepData();
        if (pData->validData())
        {
            auto enableList = pData->getEnabledList();
            auto actionIds = pData->getActionIDs();
            auto costs = pData->getCostList();
            for (qint32 i = 0; i < pData->getActionIDs().size(); i++)
            {
                if (enableList[i])
                {
                    UnitBuildData unitData;
                    unitData.unitId = actionIds[i];
                    unitData.cost = costs[i];
                    Unit dummy(unitData.unitId, m_pPlayer, false);
                    dummy.setVirtuellX(x);
                    dummy.setVirtuellY(y);
                    getProductionInputVector(building.m_pBuilding, &dummy, unitData, immuneUnits);
                    updateUnitBuildData(unitData, data, funds);
                    building.buildingDataInput.append(unitData);
                }
            }
        }
    }
}

void HeavyAi::updateUnitBuildData(BuildingData & building, QVector<double> & data, qint32 funds)
{
    GameAction action = GameAction(ACTION_BUILD_UNITS);
    action.setTarget(QPoint(building.m_pBuilding->Building::getX(), building.m_pBuilding->Building::getY()));
    if (action.canBePerformed())
    {
        spMenuData pData = action.getMenuStepData();
        if (pData->validData())
        {
            // update
            auto enableList = pData->getEnabledList();
            auto actionIds = pData->getActionIDs();
            for (auto & unitData : building.buildingDataInput)
            {
                for (qint32 i = 0; i < enableList.size(); ++i)
                {
                    if (unitData.unitId == actionIds[i])
                    {
                        unitData.enabled = enableList[i];
                        break;
                    }
                }
                updateUnitBuildData(unitData, data, funds);
            }
        }
        else
        {
            building.buildingDataInput.clear();
        }
    }
    else
    {
        building.buildingDataInput.clear();
    }
}

void HeavyAi::updateUnitBuildData(UnitBuildData &unitData, QVector<double> &data, qint32 funds)
{
    for (qint32 i = 0; i < BuildingEntry::LocalUnitData; ++i)
    {
        unitData.unitBuildingDataInput[i] = data[i];
    }
    if (funds > 0)
    {
        unitData.unitBuildingDataInput[FondsUsage] = static_cast<double>(unitData.cost) / static_cast<double>(funds);
    }
    else
    {
        unitData.unitBuildingDataInput[FondsUsage] = 0;
    }
}

const QString &HeavyAi::getAiName() const
{
    return m_aiName;
}

void HeavyAi::setAiName(const QString &newAiName)
{
    m_aiName = newAiName;
}

void HeavyAi::getProductionInputVector(Building* pBuilding, Unit* pUnit, UnitBuildData & data, const QVector<Unit*> & immuneUnits)
{
    QStringList actionList = pUnit->getActionList();
    if (pUnit->getBaseMaxRange() > 1)
    {
        data.unitBuildingDataInput[BuildingEntry::BasicAttackRange] = 1;
    }
    else
    {
        data.unitBuildingDataInput[BuildingEntry::BasicAttackRange] = -1;
    }
    if (actionList.contains(ACTION_CAPTURE))
    {
        data.unitBuildingDataInput[BuildingEntry::CaptureUnit] = 1;
    }
    else
    {
        data.unitBuildingDataInput[BuildingEntry::CaptureUnit] = -1;
    }
    QPoint position(pBuilding->Building::getX(), pBuilding->Building::getY());
    auto & influenceInfo = m_InfluenceFrontMap.getInfluenceInfo(position.x(), position.y());
    float highestInfluence = m_InfluenceFrontMap.getTotalHighestInfluence();
    float value = getAiCoUnitMultiplier(m_pPlayer->getCO(0), pUnit);
    qint32 movementPoints = pUnit->getMovementpoints(position);
    value += getAiCoUnitMultiplier(m_pPlayer->getCO(1), pUnit);
    data.unitBuildingDataInput[BuildingEntry::CoUnitValue] = value / (CO::MAX_CO_UNIT_VALUE * 2);
    data.unitBuildingDataInput[BuildingEntry::Movementpoints] = movementPoints / m_maxMovementpoints;
    data.unitBuildingDataInput[BuildingEntry::FireRange] = pUnit->getMaxRange(position) / m_maxFirerange;
    data.unitBuildingDataInput[BuildingEntry::Flying] = (pUnit->useTerrainDefense() == false);
    data.unitBuildingDataInput[BuildingEntry::LoadingPotential] = pUnit->getLoadingPlace() / 4.0;
    data.unitBuildingDataInput[BuildingEntry::OwnInfluence] = influenceInfo.ownInfluence / highestInfluence;
    data.unitBuildingDataInput[BuildingEntry::HighestEnemyInfluence] = influenceInfo.highestEnemyInfluence / highestInfluence;
    calculateUnitProductionDamage(pBuilding, pUnit, movementPoints, position, data, immuneUnits);
}

void HeavyAi::calculateUnitProductionDamage(Building* pBuilding, Unit* pUnit, qint32 movementPoints, QPoint position, UnitBuildData & data, const QVector<Unit*> & immuneUnits)
{
    spGameMap pMap = GameMap::getInstance();
    qint32 mapWidth = pMap->getMapWidth();
    qint32 mapHeight = pMap->getMapHeight();
    qint32 unitValue = pUnit->getUnitValue();
    bool canCapture = (data.unitBuildingDataInput[BuildingEntry::CaptureUnit] > 0);
    float fundsDamage = 0.0f;
    float possibleFundsDamage = 0.0f;
    float hpDamage = 0.0f;
    float possibleHpDamage = 0.0f;
    float captureRate = 0.0f;
    float maxCaptureRate = 0.0f;
    float counterFundsDamage = 0.0f;
    float counterhpDamage = 0.0f;
    float counterAttackCount = 0.0f;
    float immuneUnit = 0;
    for (qint32 x = 0; x < mapWidth; ++x)
    {
        for (qint32 y = 0; y < mapHeight; ++y)
        {
            Terrain* pTerrain = pMap->getTerrain(x, y);
            Unit* pEnemyUnit = pTerrain->getUnit();
            Building* pBuilding = pTerrain->getBuilding();
            float scoreMultiplier = 1.0f;
            qint32 distance = GlobalUtils::getDistance(position, QPoint(x, y));
            float turnDistance = static_cast<float>(distance) / static_cast<float>(movementPoints);
            if (turnDistance > m_maxProductionTurnRange)
            {
                scoreMultiplier = m_maxProductionTurnRange / turnDistance;
            }
            if (pEnemyUnit != nullptr &&
                m_pPlayer->isEnemyUnit(pEnemyUnit))
            {
                float dmg = getBaseDamage(pUnit, pEnemyUnit);
                float enemyHp = pEnemyUnit->getHp();
                float enemyHpRatio = enemyHp / Unit::MAX_UNIT_HP;
                if (dmg >= 0)
                {
                    qint32 enemyUnitValue = pEnemyUnit->getUnitValue();
                    float damageMultiplier = dmg / (enemyHp * Unit::MAX_UNIT_HP);
                    fundsDamage         += scoreMultiplier * enemyUnitValue * damageMultiplier;
                    possibleFundsDamage += scoreMultiplier * enemyUnitValue;
                    hpDamage            += scoreMultiplier * damageMultiplier;
                    possibleHpDamage    += scoreMultiplier * enemyHp / Unit::MAX_UNIT_HP;
                    if (immuneUnits.contains(pEnemyUnit))
                    {
                        ++immuneUnit;
                    }
                }
                dmg = getBaseDamage(pEnemyUnit, pUnit) * enemyHpRatio;
                if (dmg >= 0)
                {
                    constexpr float maxDamage = Unit::MAX_UNIT_HP  * Unit::MAX_UNIT_HP;
                    counterFundsDamage  += scoreMultiplier * unitValue * dmg / maxDamage;
                    counterhpDamage     += scoreMultiplier * dmg / maxDamage;
                    ++counterAttackCount;
                }
            }
            if (canCapture &&
                pBuilding != nullptr &&
                m_pPlayer->isEnemy(pBuilding->getOwner()) &&
                pBuilding->isCaptureOrMissileBuilding(true))
            {
                if (pBuilding->getActionList().contains(ACTION_BUILD_UNITS))
                {
                    qint32 buildSize = pBuilding->getConstructionList().size();
                    captureRate += scoreMultiplier * buildSize;
                    maxCaptureRate += buildSize;
                }
                else
                {
                    captureRate += scoreMultiplier;
                    maxCaptureRate += 1;
                }
            }
        }
    }
    if (possibleHpDamage > 0)
    {
        data.unitBuildingDataInput[BuildingEntry::DealingHpDamage] = hpDamage / possibleHpDamage;
    }
    if (possibleFundsDamage > 0)
    {
        data.unitBuildingDataInput[BuildingEntry::DealingFundsDamage] = fundsDamage / possibleFundsDamage;
    }
    if (counterAttackCount > 0 && unitValue > 0)
    {
        data.unitBuildingDataInput[BuildingEntry::ReceivingFundsDamge]  = counterFundsDamage / (counterAttackCount * unitValue);
        data.unitBuildingDataInput[BuildingEntry::ReceivingHpDamage]    = counterhpDamage / counterAttackCount;
    }
    if (maxCaptureRate > 0)
    {
        data.unitBuildingDataInput[BuildingEntry::CapturePotential] = captureRate / maxCaptureRate;
    }
    if (immuneUnits.size() > 0)
    {
        data.unitBuildingDataInput[BuildingEntry::CanAttackImmuneUnitRatio] = immuneUnit / static_cast<double>(immuneUnits.size());
    }
}

float HeavyAi::getBaseDamage(Unit* pAttacker, Unit* pDefender)
{
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    float dmg = 0.0f;
    QString weaponId = pAttacker->getWeapon1ID();
    if (!weaponId.isEmpty())
    {
        dmg = pWeaponManager->getBaseDamage(weaponId, pDefender);
    }
    weaponId = pAttacker->getWeapon2ID();
    if (!weaponId.isEmpty())
    {
        float dmg2 = pWeaponManager->getBaseDamage(weaponId, pDefender);
        if (dmg2 > dmg)
        {
            dmg = dmg2;
        }
    }
    dmg *= pAttacker->getHp() / Unit::MAX_UNIT_HP;
    // cap damage
    if (dmg > pDefender->getHp() * Unit::MAX_UNIT_HP)
    {
        dmg = pDefender->getHp() * Unit::MAX_UNIT_HP;
    }
    return dmg;
}

QVector<double> HeavyAi::getGlobalBuildInfo(spQmlVectorBuilding pBuildings, spQmlVectorUnit pUnits,
                                            spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings)
{
    QVector<double> data(BuildingEntry::MaxSize, 0.0);
    spGameMap pMap = GameMap::getInstance();
    qint32 infantryUnits = 0;
    qint32 indirectUnits = 0;
    qint32 directUnits = 0;
    qint32 transporterUnits = 0;
    QVector<std::tuple<Unit*, Unit*>> transportTargets;
    GetOwnUnitCounts(pUnits, pEnemyUnits, pEnemyBuildings,
                     infantryUnits, indirectUnits, directUnits, transporterUnits,
                     transportTargets);
    double count = pUnits->size();
    if (count > 0)
    {
        data[DirectUnitRatio]   = static_cast<double>(directUnits)   / count;
        data[IndirectUnitRatio] = static_cast<double>(indirectUnits) / count;
        data[InfantryUnitRatio] = static_cast<double>(infantryUnits) / count;
        data[TransportUnitRatio] = static_cast<double>(transporterUnits) / count;

    }
    double enemeyCount = 0;
    double playerCount = 0;
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        if (!pMap->getPlayer(i)->getIsDefeated())
        {
            playerCount++;
            if (m_pPlayer->isEnemy(pMap->getPlayer(i)))
            {
                enemeyCount++;
            }
        }
    }
    if (pEnemyBuildings->size() + pBuildings->size() > 0)
    {
        data[TotalBuildingRatio] = pBuildings->size() / (pBuildings->size() + pEnemyBuildings->size());
    }
    if (playerCount > 0)
    {
        data[EnemyRatio] = static_cast<double>(enemeyCount) / playerCount;
    }
    double productionCount = 0;
    double unusedCount = 0;
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        if (pBuilding->isProductionBuilding())
        {
            productionCount++;
            if (pBuilding->getTerrain()->getUnit() == nullptr)
            {
                unusedCount++;
                bool found = false;
                for (auto & building : m_BuildingData)
                {
                    if (building.m_pBuilding == pBuilding)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    BuildingData newData;
                    newData.m_pBuilding = pBuilding;
                    m_BuildingData.append(newData);
                }
            }
            else
            {
                for (qint32 i = 0; i < m_BuildingData.size(); ++i)
                {
                    if (m_BuildingData[i].m_pBuilding == pBuilding)
                    {
                        m_BuildingData.removeAt(i);
                        break;
                    }
                }
            }
        }
    }
    if (productionCount > 0)
    {
        data[ProductionUsage] = static_cast<double>(unusedCount) / productionCount;
    }
    return data;
}


void HeavyAi::getImmuneUnits(spQmlVectorUnit pUnits, spQmlVectorUnit pEnemyUnits, QVector<Unit*> & immuneUnits)
{
    immuneUnits.clear();
    for (qint32 i2 = 0; i2 < pEnemyUnits->size(); i2++)
    {
        bool attackable = false;
        for (qint32 i = 0; i < pUnits->size(); i++)
        {
            Unit* pUnit = pUnits->at(i);
            Unit* pEnemyUnit = pEnemyUnits->at(i2);
            auto dmg = getBaseDamage(pUnit, pEnemyUnit);

            if (dmg > m_notAttackableDamage)
            {
                attackable = true;
                break;
            }
        }
        if (!attackable)
        {
            immuneUnits.append(pEnemyUnits->at(i2));
        }
    }
}
