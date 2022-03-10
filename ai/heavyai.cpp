#include <QSettings>

#include "coreengine/qmlvector.h"
#include "coreengine/console.h"
#include "coreengine/globalutils.h"

#include "ai/heavyai.h"

#include "game/player.h"
#include "game/gameaction.h"
#include "game/gamemap.h"

#include "resource_management/unitspritemanager.h"

const qint32 HeavyAi::minSiloDamage = 7000;
const char* const HeavyAi::NeuralNetworkFileEnding = ".net";
const char* const HeavyAi::NeuralNetworkPath = "aidata/heavy/";
const QStringList HeavyAi::NeuralNetworkNames = {"Production",
                                                 "ACTION_FIRE",
                                                 "ACTION_CAPTURE",
                                                 "ACTION_UNLOADING",
                                                 "WAIT_DISTANCE_MULTIPLIER"};
// normally i'm not a big fan of this but else the function table gets unreadable
using namespace std::placeholders;

HeavyAi::HeavyAi(GameMap* pMap, QString type, GameEnums::AiTypes aiType)
    : CoreAI(pMap, aiType),
      m_scoreInfos({
{ACTION_CAPTURE,  std::bind(&HeavyAi::scoreCapture,     this,   _1, _2, _3)},
{ACTION_FIRE,     std::bind(&HeavyAi::scoreFire,        this,   _1, _2, _3)},
{ACTION_JOIN,     std::bind(&HeavyAi::scoreJoin,        this,   _1, _2, _3)},
{ACTION_MISSILE,  std::bind(&HeavyAi::scoreMissile,     this,   _1, _2, _3)},

{ACTION_LOAD,     std::bind(&HeavyAi::scoreLoad,        this,   _1, _2, _3)},
{ACTION_UNLOAD,     std::bind(&HeavyAi::scoreUnload,    this,   _1, _2, _3)},
{ACTION_WAIT,     std::bind(&HeavyAi::scoreWait,        this,   _1, _2, _3)},
                   }),
      m_InfluenceFrontMap(pMap, m_IslandMaps),
      m_timer(this),
      m_aiName(type)
{
    setObjectName("HeavyAi");
    Interpreter::setCppOwnerShip(this);
    m_timer.setSingleShot(false);
    connect(&m_timer, &QTimer::timeout, this, &HeavyAi::process, Qt::QueuedConnection);

    m_iniData = { // General
                  {"MinActionScore", "General", &m_minActionScore, 0.2f, 0.0f, 1.0f},
                  {"ActionScoreVariant", "General", &m_actionScoreVariant, 0.05f, 0.01f, 0.5f},
                  {"StealthDistanceMultiplier", "General", &m_stealthDistanceMultiplier, 2.0f, 1.0f, 10.0f},
                  {"AlliedDistanceModifier", "General", &m_alliedDistanceModifier, 5.0f, 1.0f, 10.0f},
                  {"MaxMovementpoints", "General", &m_maxMovementpoints, 20.0f, 20.0f, 20.0f},
                  {"MaxProductionTurnRange", "General", &m_maxProductionTurnRange, 4.0f, 1.0f, 10.0f},
                  {"MaxFirerange", "General", &m_maxFirerange, 10.0f, 10.0f, 10.0f},
                  {"PrimaryEnemyMultiplier", "General", &m_primaryEnemyMultiplier, 1.2f, 1.0f, 10.0f},
                  {"MaxLoadingPlace", "General", &m_maxLoadingPlace, 4.0f, 1.0f, 10.0f},
                  {"NotAttackableDamage", "General", &m_notAttackableDamage, 30.0f, 1.0f, 45.0f},
                  {"OwnUnitProtection", "General", &m_notAttackableDamage, 5.0f, 1.0f, 10.0f},
                  {"EnemyUnitThread", "General", &m_notAttackableDamage, 5.0f, 1.0f, 10.0f},
                  {"MaxVision", "General", &m_notAttackableDamage, 10.0f, 10.0f, 10.0f},
                  {"MaxUnitValue", "General", &m_maxUnitValue, 40000.0f, 40000.0f, 40000.0f},
                  {"MaxScore", "General", &m_maxScore, 10.0f, 10.0f, 10.0f},
                  {"MaxTerrainDefense", "General", &m_maxTerrainDefense, 15.0f, 15.0f, 15.0f},
                  {"MaxCapturePoints", "General", &m_maxCapturePoints, 20.0f, 20.0f, 20.0f},
                  {"MinSameIslandDistance", "General", &m_minSameIslandDistance, 3.0f, 3.0f, 3.0f},
                  {"SlowUnitSpeed", "General", &m_slowUnitSpeed, 2.0f, 2.0f, 2.0f},
                  {"UsedCapturePointIncrease", "General", &m_usedCapturePointIncrease, 1.5f, 1.5f, 1.5f},
                };
    
    if (m_pMap != nullptr &&
        !m_pMap->getSavegame())
    {
        loadIni("heavy/" + m_aiName.toLower() + ".ini");
    }
    loadNeuralNetworks(aiType);
    if (NeuralNetworkNames.length() != NeuralNetworksMax)
    {
        oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "Missing Enum name mapping");
    }
}

void HeavyAi::loadNeuralNetworks(GameEnums::AiTypes aiType)
{
    bool randomize = static_cast<qint32>(aiType) - GameEnums::AiTypes_Heavy >= 3;
    loadNeuralNetwork(NeuralNetworkNames[NeuralNetworks::Production], m_neuralNetworks[NeuralNetworks::Production], static_cast<qint32>(BuildingEntryMaxSize), BuildingEntryMaxSize / 4, randomize);
    loadNeuralNetwork(NeuralNetworkNames[NeuralNetworks::ActionFire], m_neuralNetworks[NeuralNetworks::ActionFire], static_cast<qint32>(AttackInfoMaxSize), AttackInfoMaxSize / 4, randomize);
    loadNeuralNetwork(NeuralNetworkNames[NeuralNetworks::ActionCapture], m_neuralNetworks[NeuralNetworks::ActionCapture], static_cast<qint32>(CaptureInfoMaxSize), CaptureInfoMaxSize / 4, randomize);
    loadNeuralNetwork(NeuralNetworkNames[NeuralNetworks::ActionUnloading], m_neuralNetworks[NeuralNetworks::ActionUnloading], static_cast<qint32>(UnloadingInfoMaxSize), UnloadingInfoMaxSize / 4, randomize);
    // todo
    loadNeuralNetwork(NeuralNetworkNames[NeuralNetworks::WaitDistanceMultiplier], m_neuralNetworks[NeuralNetworks::WaitDistanceMultiplier], 5, 5, randomize, WaitTargetTypesMaxSize);
}

void HeavyAi::loadNeuralNetwork(QString netName, spNeuralNetwork & network, qint32 inputVectorSize, qint32 netDepth, bool randomize, qint32 outputSize)
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
            qint32 inputSize = network->getInputSize();
            if (inputSize > inputVectorSize)
            {
                network->extend(inputSize - inputVectorSize, randomize);
            }
            found = true;
            break;
        }
    }
    if (!found)
    {
        QMap<QString, double> parameters;
        parameters.insert(Layer::LAYER_PARAMETER_TYPE, static_cast<double>(Layer::LayerType::INPUT));
        parameters.insert(Layer::LAYER_PARAMETER_ACTIVATION, static_cast<double>(Neuron::ActivationFunction::Step));
        parameters.insert(Layer::LAYER_PARAMETER_SIZE, static_cast<double>(inputVectorSize));
        network->addLayer(parameters);
        parameters.insert(Layer::LAYER_PARAMETER_TYPE, static_cast<double>(Layer::LayerType::STANDARD));
        for (qint32 i = 0; i < netDepth; ++i)
        {
            network->addLayer(parameters);
        }
        parameters.insert(Layer::LAYER_PARAMETER_TYPE, static_cast<double>(Layer::LayerType::OUTPUT));
        parameters[Layer::LAYER_PARAMETER_SIZE] = outputSize;
        network->addLayer(parameters);
        network->autogenerate();
        QFile file("resources/" + baseName);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        network->serializeObject(stream);
    }
}

void HeavyAi::process()
{
    spQmlVectorBuilding pBuildings = spQmlVectorBuilding(m_pPlayer->getBuildings());
    pBuildings->randomize();
    spQmlVectorBuilding pEnemyBuildings = spQmlVectorBuilding(m_pPlayer->getEnemyBuildings());
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
    spQmlVectorUnit pUnits(m_pPlayer->getUnits());
    spQmlVectorUnit pEnemyUnits(m_pPlayer->getEnemyUnits());
    if (useBuilding(pBuildings)){}
    else if (useCOPower(pUnits, pEnemyUnits)){}
    else
    {
        setupTurn(pBuildings);
        turnMode = GameEnums::AiTurnMode_DuringDay;
        if (CoreAI::buildCOUnit(m_pUnits)){}
        // use core ai here for the start
        else if (CoreAI::moveFlares(m_pUnits)){}
        else if (CoreAI::moveOoziums(m_pUnits, m_pEnemyUnits)){}
        else if (CoreAI::moveBlackBombs(m_pUnits, m_pEnemyUnits)){}
        // actual heavy ai impl
        else if (!selectActionToPerform())
        {
            if (m_aiStep < AISteps::moveToTargets)
            {
                m_aiStep = AISteps::moveToTargets;
                scoreMoveToTargets();
            }
            if (!selectActionToPerform())
            {
                if (m_aiStep < AISteps::moveTransporters)
                {
                    m_aiStep = AISteps::moveTransporters;
                    scoreMoveToTargets();
                }
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
}

void HeavyAi::endTurn()
{
    m_aiStep = AISteps::moveUnits;
    turnMode = GameEnums::AiTurnMode_EndOfDay;
    m_pUnits = nullptr;
    m_pEnemyUnits = nullptr;
    spQmlVectorUnit pUnits(m_pPlayer->getUnits());
    spQmlVectorUnit pEnemyUnits(m_pPlayer->getEnemyUnits());
    if (useCOPower(pUnits, pEnemyUnits)){}
    else
    {
        // end the turn for the player
        turnMode = GameEnums::AiTurnMode_StartOfDay;
        finishTurn();
    }
}

bool HeavyAi::selectActionToPerform()
{
    AI_CONSOLE_PRINT("HeavyAi selecting action to be performed", Console::eDEBUG);
    float bestScore = std::numeric_limits<float>::lowest();
    qint32 index = -1;
    qint32 pos = 0;
    for (const auto & ownUnit : qAsConst(m_ownUnits))
    {
        if (ownUnit.m_action.get() != nullptr &&
            ownUnit.m_score > bestScore)
        {
            if (ownUnit.pUnit->getTerrain() == nullptr ||
                ownUnit.pUnit->getHp() <= 0)
            {
                oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "invalid unit found");
                continue;
            }
            bestScore = ownUnit.m_score;
            index = pos;
        }
        ++pos;
    }
    if (index >= 0)
    {
        auto & unit = m_ownUnits[index];
        QPoint target = unit.m_action->getTarget();
        AI_CONSOLE_PRINT("HeavyAi selected action " + unit.m_action->getActionID() + " to be performed with score " + QString::number(bestScore), Console::eDEBUG);
        m_updatePoints.append(target);
        m_updatePoints.append(unit.m_action->getActionTarget());
        if (target != unit.pUnit->Unit::getPosition())
        {
            oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "HeavyAi::selectActionToPerform action error");
            unit.m_action = nullptr;
            unit.m_score = 0;
            return false;
        }
        if (unit.captureTarget.x() >= 0)
        {
            m_planedCaptureTargets.append(unit.captureTarget);
        }
        emit performAction(unit.m_action);
        unit.m_action = nullptr;
        unit.m_score = 0;
        unit.captureTarget = QPoint(-1, -1);
        return true;
    }
    return false;
}

void HeavyAi::setupTurn(const spQmlVectorBuilding & buildings)
{
    qint32 cost = 0;
    m_pPlayer->getSiloRockettarget(2, 3, cost);
    m_missileTarget = (cost >= minSiloDamage);
    bool startOfTurn = (m_pUnits.get() == nullptr);
    if (m_pEnemyUnits.get() == nullptr)
    {
        m_pEnemyUnits = m_pPlayer->getEnemyUnits();
        m_pEnemyUnits->randomize();
        initUnits(m_pEnemyUnits, m_enemyUnits, true);
    }
    if (m_pUnits.get() == nullptr)
    {
        m_pUnits = m_pPlayer->getUnits();
        initUnits(m_pUnits, m_ownUnits, false);
    }
    if (startOfTurn)
    {
        AI_CONSOLE_PRINT("HeavyAi initial start of turn calculation", Console::eDEBUG);
        createIslandMaps();

        // create influence map at the start of the turn
        m_InfluenceFrontMap.reset();
        m_InfluenceFrontMap.setOwner(m_pPlayer);
        m_InfluenceFrontMap.addBuildingInfluence();
        for (auto & unit : m_ownUnits)
        {
            m_InfluenceFrontMap.addUnitInfluence(unit.pUnit.get(), unit.pUnitPfs.get(), unit.movementPoints);
        }
        for (auto & unit : m_enemyUnits)
        {
            m_InfluenceFrontMap.addUnitInfluence(unit.pUnit.get(), unit.pUnitPfs.get(), unit.movementPoints);
        }
        m_InfluenceFrontMap.updateOwners();
        m_InfluenceFrontMap.calculateGlobalData();
        AI_CONSOLE_PRINT("HeavyAi front lines created", Console::eDEBUG);
        findHqThreads(buildings);
        AI_CONSOLE_PRINT("HeavyAi initial scoring actions for units", Console::eDEBUG);
        for (auto & unit : m_ownUnits)
        {
            scoreActions(unit);
        }
    }
    else
    {
        updateUnits();
    }
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

void HeavyAi::initUnits(spQmlVectorUnit & pUnits, QVector<MoveUnitData> & units, bool enemyUnits)
{
    units.clear();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        addNewUnitToUnitData(units, pUnits->at(i), enemyUnits);
    }
}

void HeavyAi::addNewUnitToUnitData(QVector<MoveUnitData> & units, Unit* pUnit, bool enemyUnits)
{
    MoveUnitData data;
    data.pUnit = pUnit;
    data.pUnitPfs = spUnitPathFindingSystem::create(m_pMap, pUnit);
    data.movementPoints = data.pUnit->getMovementpoints(data.pUnit->getPosition());
    data.pUnitPfs->setMovepoints(data.movementPoints * 2);
    data.pUnitPfs->setIgnoreEnemies(UnitPathFindingSystem::CollisionIgnore::OnlyNotMovedEnemies);
    data.pUnitPfs->explore();
    if (!enemyUnits)
    {
        updateCaptureBuildings(data);
    }
    units.append(data);
}

void HeavyAi::updateUnits()
{
    updateUnits(m_ownUnits, m_pUnits, false);
    updateUnits(m_enemyUnits, m_pEnemyUnits, true);
    m_updatePoints.clear();
}

void HeavyAi::updateUnits(QVector<MoveUnitData> & units, spQmlVectorUnit & pUnits, bool enemyUnits)
{
    
    if (m_pMap == nullptr)
    {
        return;
    }
    qint32 i = 0;
    while (i < units.size())
    {
        if (units[i].pUnit->getHp() <= 0 ||
            units[i].pUnit->getTerrain() == nullptr)
        {
            units.removeAt(i);
        }
        else
        {
            if (units[i].pUnit->getHasMoved())
            {
                units[i].m_action = nullptr;
                units[i].m_score = 0;
            }
            ++i;
        }
    }
    i = 0;
    while (i < pUnits->size())
    {
        if (pUnits->at(i)->getHp() <= 0 ||
            pUnits->at(i)->getTerrain() == nullptr)
        {
            pUnits->removeAt(i);
        }
        else
        {
            ++i;
        }
    }
    QVector<qint32> updated;
    spQmlVectorPoint pPoints = spQmlVectorPoint(GlobalUtils::getCircle(1, 5));
    for (qint32 i = 0; i < m_updatePoints.size(); i++)
    {
        qint32 i2 = 0;
        while (i2 < units.size())
        {
            if (!updated.contains(i2))
            {
                if (qAbs(m_updatePoints[i].x() - units[i2].pUnit->Unit::getX()) +
                    qAbs(m_updatePoints[i].y() - units[i2].pUnit->Unit::getY()) <=
                    units[i2].pUnit->getMovementpoints(QPoint(units[i2].pUnit->Unit::getX(), units[i2].pUnit->Unit::getY())) + 2)
                {
                    units[i2].pUnitPfs = spUnitPathFindingSystem::create(m_pMap, units[i2].pUnit.get());
                    units[i2].pUnitPfs->setIgnoreEnemies(UnitPathFindingSystem::CollisionIgnore::OnlyNotMovedEnemies);
                    units[i2].pUnitPfs->explore();
                    if (!enemyUnits)
                    {
                        updateCaptureBuildings(units[i2]);
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
        if (!enemyUnits)
        {
            for (qint32 i3 = 0; i3 < pPoints->size(); ++i3)
            {
                QPoint pos = pPoints->at(i3) + m_updatePoints[i];
                if (m_pMap->onMap(pos.x(), pos.y()))
                {
                    bool found = false;
                    Unit* pUnit = m_pMap->getTerrain(pos.x(), pos.y())->getUnit();
                    if (pUnit != nullptr &&
                        pUnit->getOwner() == m_pPlayer)
                    {
                        for (const auto & unit : units)
                        {
                            if (unit.pUnit == pUnit)
                            {
                                found = true;
                                break;
                            }
                        }
                        if (!found)
                        {
                            pUnits->append(pUnit);
                            addNewUnitToUnitData(units, pUnit, enemyUnits);
                        }
                    }
                }
            }
        }
    }
}

void HeavyAi::updateCaptureBuildings(MoveUnitData & unitData)
{
    if (unitData.actions.contains(ACTION_CAPTURE))
    {
        unitData.m_capturePoints.clear();
        GameAction action(ACTION_CAPTURE, m_pMap);
        action.setTarget(QPoint(unitData.pUnit->Unit::getX(), unitData.pUnit->Unit::getY()));
        QVector<QPoint> targets = unitData.pUnitPfs->getAllNodePoints(unitData.movementPoints + 1);
        for (const auto & target : targets)
        {
            action.setMovepath(QVector<QPoint>(1, target), 0);
            if (action.canBePerformed())
            {
                unitData.m_capturePoints.append(target);
            }
        }
    }
}

void HeavyAi::findHqThreads(const spQmlVectorBuilding & buildings)
{
    AI_CONSOLE_PRINT("Searching for HQ Threads", Console::eDEBUG);
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
        if (isCaptureTransporterOrCanCapture(enemy.pUnit.get()))
        {
            constexpr qint32 dayDistance = 3;
            QPoint pos = enemy.pUnit->getMapPosition();
            qint32 movementPoints = enemy.pUnit->getMovementpoints(enemy.pUnit->getMapPosition());
            for (const auto & hqPos : hqPositions)
            {
                if (GlobalUtils::getDistance(pos, QPoint(hqPos.x(), hqPos.y())) <= dayDistance * movementPoints)
                {
                    TargetedUnitPathFindingSystem pfs = TargetedUnitPathFindingSystem(m_pMap, enemy.pUnit.get(), hqPositions, &m_MoveCostMap);
                    pfs.setIgnoreEnemies(UnitPathFindingSystem::CollisionIgnore::OnlyNotMovedEnemies);
                    pfs.setMovepoints(movementPoints * dayDistance);
                    pfs.setAbortOnCostExceed(true);
                    pfs.setFast(true);
                    pfs.setUseBasecosts(true);
                    pfs.explore();
                    QPoint targetFields = pfs.getReachableTargetField(movementPoints * 3);
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

void HeavyAi::scoreActions(MoveUnitData & unit)
{
    AI_CONSOLE_PRINT("HeavyAi::scoreActions", Console::eDEBUG);
    if (!unit.pUnit->getHasMoved())
    {
        if (unit.pUnit->getTerrain() == nullptr ||
            unit.pUnit->getHp() <= 0)
        {
            oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "invalid unit found");
            unit.m_action = nullptr;
            unit.m_score = 0;
            return;
        }
        QVector<ScoreData> scoreInfos;
        float bestScore = 0.0f;
        unit.actions = unit.pUnit->getActionList();
        auto moveTargets = unit.pUnitPfs->getAllNodePoints(unit.movementPoints + 1);
        if (m_aiStep >= AISteps::moveToTargets ||
            unit.actions.contains(ACTION_CAPTURE))
        {
            prepareWaitPfs(unit, unit.actions);
        }
        for (const auto & action : unit.actions)
        {
            if (isScoringAllowed(action, unit.actions))
            {
                FunctionType type = FunctionType::CPlusPlus;
                qint32 index = -1;
                getFunctionType(action, type, index);
                if (index >= 0)
                {
                    mutateActionForFields(unit, moveTargets, action, type, index,
                                          bestScore, scoreInfos);
                }
            }
        }
        if (scoreInfos.size() > 0)
        {
            qint32 item = GlobalUtils::randInt(0, scoreInfos.size() - 1);
            unit.m_score = scoreInfos[item].m_score;
            unit.m_action = scoreInfos[item].m_gameAction;
            unit.captureTarget = scoreInfos[item].m_captureTarget;
        }
        else
        {
            unit.m_score = 0.0f;
            unit.m_action = nullptr;
        }
        m_currentTargetedfPfs = nullptr;
    }
}

bool HeavyAi::isScoringAllowed(const QString & action, const QStringList & actions)
{
    bool ret = false;
    if (m_aiStep >= AISteps::moveTransporters)
    {
        ret = true;
    }
    else if (m_aiStep >= AISteps::moveToTargets &&
          m_secondyActions.contains(action))
    {
        ret = true;
    }
    else if (!m_secondyActions.contains(action) ||
             (action == ACTION_WAIT && actions.contains(ACTION_CAPTURE)))
    {
        ret = true;
    }
    return ret;
}

void HeavyAi::mutateActionForFields(MoveUnitData & unitData, const QVector<QPoint> & moveTargets,
                                    QString action, FunctionType type, qint32 index,
                                    float & bestScore, QVector<ScoreData> & scoreInfos)
{
    AI_CONSOLE_PRINT("HeavyAi::mutateActionForFields " + action, Console::eDEBUG);
    for (const auto & target : moveTargets)
    {
        QVector<QPoint> path = unitData.pUnitPfs->getPath(target.x(), target.y());
        qint32 costs = unitData.pUnitPfs->getCosts(path);
        bool mutate = true;
        QVector<qint32> stepPosition;
        QVector<double> baseData(static_cast<qsizetype>(BasicFieldInfo::BasicFieldInfoMaxSize));
        spGameAction pDummy  = spGameAction::create(m_pMap);
        pDummy->setActionID(action);
        pDummy->setMovepath(path, costs);
        pDummy->setTarget(QPoint(unitData.pUnit->Unit::getX(), unitData.pUnit->Unit::getY()));
        getBasicFieldInputVector(pDummy, baseData);
        while (mutate)
        {
            spGameAction pAction  = spGameAction::create(m_pMap);
            pAction->setActionID(action);
            pAction->setMovepath(path, costs);
            pAction->setTarget(QPoint(unitData.pUnit->Unit::getX(), unitData.pUnit->Unit::getY()));
            qint32 step = 0;
            if (pAction->canBePerformed())
            {
                ScoreData data;
                data.m_gameAction = pAction;
                mutate = mutateAction(data, unitData, baseData, type, index, step, stepPosition);
                if (data.m_score > m_maxScore)
                {
                    data.m_score = m_maxScore;
                }
                if (data.m_score > m_minActionScore)
                {
                    if (data.m_score > bestScore)
                    {
                        bestScore = data.m_score;
                        qint32 i = 0;
                        while (i < scoreInfos.size())
                        {
                            if (data.m_score < scoreInfos[i].m_score - m_actionScoreVariant)
                            {
                                scoreInfos.removeAt(i);
                            }
                            else
                            {
                                ++i;
                            }
                        }
                        scoreInfos.append(data);
                    }
                    else if (bestScore - m_actionScoreVariant <= data.m_score)
                    {
                        scoreInfos.append(data);
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

bool HeavyAi::mutateAction(ScoreData & data, MoveUnitData & unitData, QVector<double> & baseData, FunctionType type, qint32 functionIndex, qint32 & step, QVector<qint32> & stepPosition)
{
    bool ret = false;
    if (data.m_gameAction->isFinalStep())
    {
        switch (type)
        {
            case FunctionType::JavaScript:
            {
                Interpreter* pInterpreter = Interpreter::getInstance();
                QJSValueList args({pInterpreter->newQObject(data.m_gameAction.get())});
                QJSValue erg = pInterpreter->doFunction(m_aiName, data.m_gameAction->getActionID(), args);
                if (erg.isNumber())
                {
                    data.m_score = erg.toNumber();
                }
                break;
            }
            case FunctionType::CPlusPlus:
            {
                m_scoreInfos[functionIndex].callback(data, unitData, baseData);
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
        QString stepType = data.m_gameAction->getStepInputType();
        if (stepType.toUpper() == "MENU")
        {
            spMenuData pData = data.m_gameAction->getMenuStepData();
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
                        CoreAI::addMenuItemData(data.m_gameAction, actionList[i], costList[i]);
                        stepPosition[step] = i;
                        ret = mutateAction(data, unitData, baseData, type, functionIndex, step, stepPosition);
                        break;
                    }
                }
            }
        }
        else if (stepType.toUpper() == "FIELD")
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
            if (data.m_gameAction->getActionID() == ACTION_MISSILE)
            {
                // special case for missile
                if (nextStep == 0)
                {
                    qint32 cost = 0;
                    QPoint rocketTarget = m_pPlayer->getSiloRockettarget(2, 3, cost);
                    CoreAI::addSelectedFieldData(data.m_gameAction, rocketTarget);
                    ret = mutateAction(data, unitData, baseData, type, functionIndex, step, stepPosition);
                }
            }
            else
            {
                spMarkedFieldData pData = data.m_gameAction->getMarkedFieldStepData();
                QVector<QPoint>* pFields = pData->getPoints();
                for (qint32 i = nextStep; i < pFields->size(); ++i)
                {
                    CoreAI::addSelectedFieldData(data.m_gameAction, (*pFields)[i]);
                    stepPosition[step] = i;
                    ret = mutateAction(data, unitData, baseData, type, functionIndex, step, stepPosition);
                    break;
                }
            }
        }
    }
    return ret;
}

void HeavyAi::getFunctionType(const QString & action, FunctionType & type, qint32 & index)
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

void HeavyAi::getBasicFieldInputVector(spGameAction & action, QVector<double> & data)
{
    Unit* pMoveUnit = action->getTargetUnit();
    QPoint moveTarget = action->getActionTarget();
    getBasicFieldInputVector(pMoveUnit, moveTarget, action->getCosts(),
                             pMoveUnit->getMovementpoints(action->getTarget()), data);
}

void HeavyAi::getBasicFieldInputVector(Unit* pMoveUnit, QPoint & moveTarget, double moveCosts, double movementPoints, QVector<double> & data)
{
    
    if (m_pMap != nullptr)
    {
        bool fogOfWar = m_pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off;
        Terrain* pTerrainTarget = m_pMap->getTerrain(moveTarget.x(), moveTarget.y());
        Building* pBuildingTarget = pTerrainTarget->getBuilding();
        qint32 playerId = m_pPlayer->getPlayerID();
        const double highestInfluece = m_InfluenceFrontMap.getTotalHighestInfluence();
        const auto & info = m_InfluenceFrontMap.getInfluenceInfo(moveTarget.x(), moveTarget.y());
        double notMovedUnitCount = 0;
        double protectionValue = 0;
        double ownValue = pMoveUnit->getCoUnitValue();
        for (const auto & pUnit : qAsConst(m_ownUnits))
        {
            if (pUnit.pUnit->getHasMoved())
            {
                ++notMovedUnitCount;
            }
            qint32 cost = pUnit.pUnitPfs->getTargetCosts(moveTarget.x(), moveTarget.y());
            if (cost >= 0 && cost <= movementPoints)
            {
                protectionValue += pUnit.pUnit->getCoUnitValue();
            }
        }
        double threadValue = 0;
        for (const auto & pUnit : qAsConst(m_ownUnits))
        {
            qint32 cost = pUnit.pUnitPfs->getTargetCosts(moveTarget.x(), moveTarget.y());
            if (cost >= 0 && cost <= movementPoints)
            {
                threadValue += pUnit.pUnit->getCoUnitValue();
            }
        }
        data[BasicFieldInfo::OwnInfluenceValue] = static_cast<double>(info.ownInfluence) / highestInfluece;
        data[BasicFieldInfo::EnemyInfluenceValue] = static_cast<double>(info.enemyInfluence) / highestInfluece;
        data[BasicFieldInfo::MoveTurnProgress] = notMovedUnitCount / static_cast<double>(m_ownUnits.size());
        spQmlVectorPoint pCircle = spQmlVectorPoint(GlobalUtils::getCircle(1, 1));
        double wallCount = 0;
        for (qint32 i = 0; i < pCircle->size(); ++i)
        {
            QPoint pos = pCircle->at(i);
            pos += moveTarget;
            if (m_pMap->onMap(pos.x(), pos.y()))
            {
                Unit* pUnit = m_pMap->getTerrain(pos.x(), pos.y())->getUnit();
                if (pUnit != nullptr &&
                    m_pPlayer->isAlly(pUnit->getOwner()))
                {
                    if (pUnit->getHasMoved() ||
                        pUnit->getOwner() != m_pPlayer)
                    {
                        ++wallCount;
                    }
                    else
                    {
                        wallCount += 0.5;
                    }
                }
            }
            else
            {
                ++wallCount;
            }
        }
        constexpr double adjustantTileCount = 4;
        data[BasicFieldInfo::WallCount] = wallCount / adjustantTileCount;
        if (pMoveUnit->isStatusStealthed())
        {
            data[BasicFieldInfo::VisionHide] = 1;
        }
        else if (fogOfWar)
        {
            data[BasicFieldInfo::VisionHide] = (pTerrainTarget->getVisionHide(nullptr) == true) ? 1 : -1;
        }
        data[BasicFieldInfo::UsedMovement] = moveCosts / movementPoints;
        data[BasicFieldInfo::UnitHealth] = pMoveUnit->getHp() / Unit::MAX_UNIT_HP;
        if (threadValue > ownValue * m_enemyUnitThread)
        {
            data[BasicFieldInfo::EnemyThread] = 1;
        }
        else
        {
            data[BasicFieldInfo::EnemyThread] = threadValue / (ownValue * m_enemyUnitThread);
        }
        if (threadValue > ownValue * m_ownUnitProtection)
        {
            data[BasicFieldInfo::OwnProtection] = 1;
        }
        else
        {
            data[BasicFieldInfo::OwnProtection] = protectionValue / (ownValue * m_ownUnitProtection);
        }
        data[BasicFieldInfo::VisionRange] = static_cast<double>(pMoveUnit->getVision(moveTarget)) / m_maxVision;
        data[BasicFieldInfo::TerrainDefense] = static_cast<double>(pMoveUnit->getTerrainDefense(moveTarget.x(), moveTarget.y())) / m_maxTerrainDefense;
        if (pBuildingTarget != nullptr &&
            pBuildingTarget->getActionList().contains(CoreAI::ACTION_BUILD_UNITS))
        {
            bool enemy = m_pPlayer->isEnemy(pBuildingTarget->getOwner());
            bool allied = m_pPlayer->isAlly(pBuildingTarget->getOwner());
            bool neutral = (pBuildingTarget->getOwner() == nullptr);
            if (!neutral)
            {
                if (enemy)
                {
                    data[BasicFieldInfo::TerrainDefense] = -1;
                }
                else if (allied)
                {
                    data[BasicFieldInfo::TerrainDefense] = 1;
                }
                else
                {
                    oxygine::handleErrorPolicy(oxygine::ep_show_error, "HeavyAi::getBasicFieldInputVector unknown building ownership");
                }
            }
        }
        if (info.frontOwners.contains(playerId))
        {
            data[BasicFieldInfo::FrontTile] = 1;
        }
        else if (info.frontOwners.size() > 0)
        {
            data[BasicFieldInfo::FrontTile] = -1;
        }
        data[BasicFieldInfo::IsCoUnit] = (pMoveUnit->getUnitRank() < 0);
    }
}

void HeavyAi::scoreCapture(ScoreData & data, MoveUnitData & unitData, QVector<double> baseData)
{
    baseData.append(QList<double>(static_cast<qsizetype>(CaptureInfoMaxSize - CaptureInfoStart)));
    Building* pBuilding = data.m_gameAction->getMovementBuilding();
    auto target = data.m_gameAction->getActionTarget();
    baseData[CaptureInfoIsHq] = (pBuilding->getBuildingID() == "HQ");
    baseData[CaptureInfoIsComTower] = (pBuilding->getBuildingID() == "TOWER");
    baseData[CaptureInfoCaptureOptions] = 1.0 / static_cast<double>(unitData.m_capturePoints.size());
    if (pBuilding->getActionList().contains(ACTION_BUILD_UNITS))
    {
        baseData[CaptureInfoProductionPotential] = static_cast<double>(pBuilding->getConstructionList().size()) / static_cast<double>(m_pPlayer->getBuildList().size());
    }
    double uniqueCounter = 0;
    for (const auto & unit : qAsConst(m_ownUnits))
    {
        if (unit.m_capturePoints.contains(target))
        {
            ++uniqueCounter;
        }
    }
    baseData[CaptureInfoUniqueCaptureBuilding] = 1.0 / uniqueCounter;
    double remainingPoints = m_maxCapturePoints;
    if (target == data.m_gameAction->getTarget())
    {
        remainingPoints -= unitData.pUnit->getCapturePoints();
    }
    double captureRate = unitData.pUnit->getCaptureRate(target);
    if (captureRate > remainingPoints)
    {
        baseData[CaptureInfoRemainingDays] = 1.0;
    }
    else
    {
        baseData[CaptureInfoRemainingDays] = captureRate / remainingPoints;
    }
    auto score = m_neuralNetworks[NeuralNetworks::ActionCapture]->predict(baseData);
    data.m_score = score[0];
}

void HeavyAi::scoreFire(ScoreData & data, MoveUnitData & unitData, QVector<double> baseData)
{
    
    baseData.append(QList<double>(static_cast<qsizetype>(AttackInfoMaxSize - AttackInfoStart)));
    if (m_pMap != nullptr)
    {
        data.m_gameAction->startReading();
        Unit* pAttacker = data.m_gameAction->getTargetUnit();
        double attackerHp = pAttacker->getHp() * Unit::MAX_UNIT_HP;
        double attackerUnitValue = pAttacker->getCoUnitValue();
        qint32 x = data.m_gameAction->readDataInt32();
        qint32 y = data.m_gameAction->readDataInt32();
        Terrain* pTerrain = m_pMap->getTerrain(x, y);
        Unit* pDefUnit = pTerrain->getUnit();
        auto damageData = calcUnitDamage(data.m_gameAction, QPoint(x, y));
        if (damageData.x() < 0)
        {
            return;
        }
        else if (pDefUnit != nullptr)
        {
            double defenderUnitHp = pDefUnit->getHp() * Unit::MAX_UNIT_HP;
            double defenderUnitValue = pDefUnit->getCoUnitValue();
            double atkDamage = damageData.x();
            if (atkDamage > defenderUnitHp)
            {
                atkDamage = defenderUnitHp;
            }
            double defDamage = damageData.width();
            if (defDamage > attackerHp)
            {
                defDamage = attackerHp;
            }
            static constexpr double maxDamage = Unit::MAX_UNIT_HP * Unit::MAX_UNIT_HP;
            baseData[AttackInfo::AttackDealingHpDamage] = atkDamage / defenderUnitHp;
            baseData[AttackInfo::AttackReceavingHpDamage] = defDamage / attackerHp;
            baseData[AttackInfo::AttackDealingAbsolutDamage] = atkDamage / maxDamage;
            baseData[AttackInfo::AttackReceicingAbsolutDamage] = defDamage / maxDamage;
            baseData[AttackInfo::AttackDealingFundsDamage] = baseData[AttackInfo::AttackDealingHpDamage] * defenderUnitValue / m_maxUnitValue;
            baseData[AttackInfo::AttackReceicingFundsDamage] = baseData[AttackInfo::AttackReceavingHpDamage] * attackerUnitValue / m_maxUnitValue;
            if (pDefUnit->getActionList().contains(CoreAI::ACTION_CAPTURE))
            {
                double captureDays = static_cast<double>(pDefUnit->getCaptureRate(pDefUnit->Unit::getPosition())) / (m_maxCapturePoints - static_cast<double>(pDefUnit->getCapturePoints()));
                if (captureDays > 1.0)
                {
                    captureDays = 1.0;
                }
                baseData[AttackInfo::AttackRemainingCaptureDays] = captureDays;
            }
            for (const auto & enemy : m_enemyUnits)
            {
                if (enemy.pUnit == pDefUnit)
                {
                    baseData[AttackInfo::HqThread] = static_cast<double>(enemy.m_threadLevel) / static_cast<double>(ThreadLevel::Max);
                    break;
                }
            }
        }
        else if (isAttackOnTerrainAllowed(pTerrain, damageData.x()))
        {
            Building* pBuilding = pTerrain->getBuilding();
            double atkDamage = damageData.x();
            double hp = pTerrain->getHp();
            if (hp <= 0 && pBuilding != nullptr)
            {
                hp = pBuilding->getHp();
            }
            if (atkDamage > hp)
            {
                atkDamage = hp;
            }
            baseData[AttackInfo::AttackDealingHpDamage] = atkDamage / hp;
            baseData[AttackInfo::AttackDealingAbsolutDamage] = atkDamage / hp;
        }
        else
        {
            return;
        }
    }
    auto score = m_neuralNetworks[NeuralNetworks::ActionFire]->predict(baseData);
    data.m_score = score[0];
}

void HeavyAi::scoreJoin(ScoreData & data, MoveUnitData & unitData, QVector<double> baseData)
{
    Unit* pJoinUnit = data.m_gameAction->getMovementTarget();
    float ret = -m_maxScore;
    if (pJoinUnit != nullptr &&
        pJoinUnit->getHasMoved() &&
        pJoinUnit->getCapturePoints() > 0)
    {
        // simple code to give the ai the option to join capture
        float resultHp = pJoinUnit->getHpRounded() + unitData.pUnit->getHpRounded();
        float malus = 0.0f;
        if (resultHp > Unit::MAX_UNIT_HP)
        {
            malus = resultHp - Unit::MAX_UNIT_HP;
        }
        else
        {
            malus = (Unit::MAX_UNIT_HP - resultHp) * 2.0f;
        }
        ret = m_maxScore - malus / Unit::MAX_UNIT_HP * m_maxScore;
    }
    data.m_score = ret;
}

void HeavyAi::scoreMissile(ScoreData & data, MoveUnitData & unitData, QVector<double> baseData)
{
    float ret = -m_maxScore;
    bool fireSilos = hasMissileTarget();
    if (fireSilos ||
        baseData[EnemyInfluenceValue] >= baseData[OwnInfluenceValue])
    {
        ret = m_maxScore;
    }
    data.m_score = ret;
}

void HeavyAi::scoreLoad(ScoreData & data, MoveUnitData & unitData, QVector<double> baseData)
{
    float ret = -m_maxCapturePoints;
    // we got a transporter is it a good one
    Unit* pTransporter = data.m_gameAction->getMovementTarget();
    if (pTransporter != nullptr &&
        !pTransporter->getHasMoved())
    {
        if (unitData.actions.contains(ACTION_CAPTURE))
        {
            // score gets better for infantries with low movement
            ret = m_maxScore / unitData.movementPoints;
        }
        else
        {
            qint32 ownValue = 0;
            qint32 enemyValue = 0;
            qint32 island = getIsland(unitData.pUnit.get());
            m_IslandMaps[island]->getValueOnIsland(getIslandIndex(unitData.pUnit.get()), ownValue, enemyValue);
            QVector<QPoint> ignoreList = {data.m_gameAction->getActionTarget()};
            qint32 targets = getNumberOfTargetsOnIsland(ignoreList);
            QVector<QVector3D> transporterTargets;
            appendTransporterTargets(unitData.pUnit.get(), m_pUnits, transporterTargets);
            // Is it a useful transporter at all?
            if (CoreAI::contains(transporterTargets, data.m_gameAction->getActionTarget()))
            {
                if (targets == 0)
                {
                    ret = m_maxScore * unitData.unitCosts / m_maxUnitValue;
                }
                else
                {
                    float unitValue = unitData.unitCosts;
                    float actionRatio = static_cast<float>(targets) / static_cast<float>(m_currentTargetedfPfs->getTargets().size());
                    if (ownValue - unitValue > enemyValue ||
                        actionRatio < 0.6f)
                    {
                        ret = m_maxScore;
                    }
                    else
                    {
                        ret = m_maxScore * actionRatio;
                        if (ownValue > enemyValue)
                        {
                            ret *= static_cast<float>(enemyValue) / static_cast<float>(ownValue);
                        }
                        else
                        {
                            ret -= m_maxScore * (1 - static_cast<float>(ownValue) / static_cast<float>(enemyValue));
                        }
                    }
                }
            }
        }
    }
    data.m_score = ret;
}

void HeavyAi::scoreUnload(ScoreData & data, MoveUnitData & unitData, QVector<double> baseData)
{
    float ret = -m_maxCapturePoints;
    // we got a transporter is it a good one
    Unit* pTransporter = data.m_gameAction->getTargetUnit();
    if (pTransporter != nullptr &&
        !pTransporter->getHasMoved())
    {
        auto targetPath = m_currentTargetedfPfs->getTargetPath();
        QPoint target = data.m_gameAction->getActionTarget();
        double movePathSize = unitData.m_action->getMovePathLength() - 1;
        double loadingCount = pTransporter->getLoadedUnitCount();
        double variableCount = data.m_gameAction->getVariableCount();
        double unloadingCount = static_cast<qint32>(variableCount - 1) % 3;
        double unloadingPercent = unloadingCount / loadingCount;
        double movementPercent = movePathSize / static_cast<double>(unitData.movementPoints);
        double distance = getDistanceToMovepath(targetPath, target);
        double score = 0;
        double maxBonusScore = m_maxScore * 0.5 / loadingCount;



        for (qint32 i = 0; i < unloadingCount; ++i)
        {
            // todo add unloading score per unit
        }
        const double maxBonus = m_maxScore * 0.5 - 1;
        if (score > maxBonus)
        {
            score = maxBonus;
        }
        scoreWaitGeneric(data, unitData, baseData);
        data.m_score *= 0.5f;
        data.m_score += score + unloadingPercent;
    }
    data.m_score = ret;
}

qint32 HeavyAi::getNumberOfTargetsOnIsland(const QVector<QPoint> & ignoreList)
{
    qint32 ret = 0;
    
    if (m_pMap != nullptr)
    {
        const auto & targets = m_currentTargetedfPfs->getTargets();
        for (const auto & point : targets)
        {
            if (!ignoreList.contains(QPoint(point.x(), point.y())))
            {
                Unit* pUnit = m_pMap->getTerrain(point.x(), point.y())->getUnit();
                if (pUnit == nullptr ||
                    m_pPlayer->isEnemyUnit(pUnit))
                {
                    ++ret;
                }
            }
        }
    }
    return ret;
}

void HeavyAi::scoreMoveToTargets()
{
    AI_CONSOLE_PRINT("HeavyAi scoring wait actions if needed", Console::eDEBUG);
    for (auto & unit : m_ownUnits)
    {
        if (!unit.pUnit->getHasMoved() &&
            unit.m_action.get() == nullptr)
        {
            QStringList & actions = unit.actions;
            prepareWaitPfs(unit, actions);
            for (const auto & action : qAsConst(actions))
            {
                QVector<ScoreData> scoreInfos;
                float bestScore = 0.0f;
                spGameAction pAction;
                FunctionType type = FunctionType::CPlusPlus;
                qint32 index = -1;
                getFunctionType(action, type, index);
                if (index >= 0)
                {
                    auto moveTargets = unit.pUnitPfs->getAllNodePoints(unit.movementPoints + 1);
                    mutateActionForFields(unit, moveTargets, action, type, index,
                                          bestScore, scoreInfos);
                }
                if (scoreInfos.size() > 0)
                {
                    qint32 item = GlobalUtils::randInt(0, scoreInfos.size() - 1);
                    unit.m_score = scoreInfos[item].m_score;
                    unit.m_action = scoreInfos[item].m_gameAction;
                }
                else
                {
                    unit.m_score = 0.0f;
                    unit.m_action = nullptr;
                }
            }
            m_currentTargetedfPfs = nullptr;
        }
    }
}

void HeavyAi::prepareWaitPfs(MoveUnitData & unitData, QStringList & actions)
{
    if (m_currentTargetedfPfs.get() == nullptr)
    {
        QVector<QVector3D> targets;
        getMoveTargets(unitData, actions, targets);
        m_currentTargetedfPfs = spTargetedUnitPathFindingSystem::create(m_pMap, unitData.pUnit.get(), targets, &m_MoveCostMap);
        m_currentTargetedfPfs->explore();
    }
}

void HeavyAi::getMoveTargets(MoveUnitData & unit, QStringList & actions, QVector<QVector3D> & targets)
{
    // todo
    
    if (m_pMap != nullptr)
    {
        qint32 mapWidth = m_pMap->getMapWidth();
        qint32 mapHeight = m_pMap->getMapHeight();
        qint32 unitIslandIdx = getIslandIndex(unit.pUnit.get());
        qint32 minFirerange = unit.pUnit->getMinRange(unit.pUnit->getPosition());
        qint32 maxFirerange = unit.pUnit->getMaxRange(unit.pUnit->getPosition());
        spQmlVectorPoint pTargetFields = spQmlVectorPoint(GlobalUtils::getCircle(minFirerange, maxFirerange));

        QVector<double> input;
        // todo create input vector
        m_neuralNetworks[NeuralNetworks::WaitDistanceMultiplier]->predict(input);

        for (qint32 x = 0; x < mapWidth; ++x)
        {
            for (qint32 y = 0; y < mapHeight; ++y)
            {
                if (unit.pUnit->canMoveOver(x, y) &&
                    onSameIsland(unitIslandIdx, unit.pUnit->Unit::getX(), unit.pUnit->Unit::getY(), x, y))
                {
                    Terrain* pTerrain = m_pMap->getTerrain(x, y);
                    addCaptureTargets(actions, pTerrain, targets);
                    addAttackTargets(unit.pUnit.get(), pTerrain, pTargetFields.get(), targets);
                    addCaptureTransporterTargets(unit.pUnit.get(), actions, pTerrain, targets);
                    addTransporterTargets(unit.pUnit.get(), pTerrain, targets);
                    // todo implement that list
                    //                addRepairTargets();
                    //                addSupportTargets();
                    //                // transporting stuff
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
}

void HeavyAi::addCaptureTargets(const QStringList & actions,
                                Terrain* pTerrain, QVector<QVector3D>& targets)
{
    if (actions.contains(ACTION_CAPTURE) ||
        actions.contains(ACTION_MISSILE))
    {
        qint32 x = pTerrain->Terrain::getX();
        qint32 y = pTerrain->Terrain::getY();
        Building* pBuilding = pTerrain->getBuilding();
        if (pBuilding != nullptr&&
            pBuilding->getTerrain()->getUnit() == nullptr &&
            pBuilding->isEnemyBuilding(m_pPlayer))
        {
            qint32 captureDistanceModifier = 0;
            if (pBuilding->isCaptureBuilding())
            {
                m_neuralNetworks[NeuralNetworks::WaitDistanceMultiplier]->output(WaitTargetTypes_Capture);
            }
            else if (pBuilding->isMissile())
            {
                m_neuralNetworks[NeuralNetworks::WaitDistanceMultiplier]->output(WaitTargetTypes_Silo);
            }
            else
            {
                return;
            }
            if (captureDistanceModifier < 1)
            {
                captureDistanceModifier = 1;
            }
            if (m_planedCaptureTargets.contains(QPoint(x, y)))
            {
                captureDistanceModifier *= m_usedCapturePointIncrease;
            }
            QVector3D possibleTarget(x, y, captureDistanceModifier);
            if (!targets.contains(possibleTarget))
            {
                targets.append(possibleTarget);
                m_possibleCaptureTargets.append(QPoint(x, y));
            }
        }
    }
}

void HeavyAi::addAttackTargets(Unit* pUnit, Terrain* pTerrain, QmlVectorPoint* pTargetFields, QVector<QVector3D> & targets)
{
    
    if (m_pMap != nullptr)
    {
        qint32 x = pTerrain->Terrain::getX();
        qint32 y = pTerrain->Terrain::getY();
        for (qint32 i = 0; i < pTargetFields->size(); ++i)
        {
            qint32 targetX = pTargetFields->at(i).x() + x;
            qint32 targetY = pTargetFields->at(i).y() + y;
            if (m_pMap->onMap(targetX, targetY))
            {
                Terrain* pTargetTerrain = m_pMap->getTerrain(targetX, targetY);
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
                    qint32 attackDistanceModifier = m_neuralNetworks[NeuralNetworks::WaitDistanceMultiplier]->output(WaitTargetTypes_Enemy);
                    if (attackDistanceModifier < 1)
                    {
                        attackDistanceModifier = 1;
                    }
                    QVector3D possibleTarget(x, y, (stealthMalus + attackDistanceModifier) * alliedmultiplier);
                    if (!targets.contains(possibleTarget))
                    {
                        targets.append(possibleTarget);
                    }
                }
                else if (m_enableNeutralTerrainAttack)
                {
                    Building* pBuilding = pTargetTerrain->getBuilding();
                    if ((pBuilding != nullptr &&
                         pBuilding->getHp() > 0 &&
                         m_pPlayer->isEnemy(pBuilding->getOwner()) &&
                         pUnit->isEnvironmentAttackable(pBuilding->getBuildingID())) ||
                        (isAttackOnTerrainAllowed(pTerrain, pUnit->getEnvironmentDamage(pTerrain->getID()))))
                    {
                        Unit* pTargetUnit = pTerrain->getUnit();
                        float alliedmultiplier = 1.0f;
                        if (pTargetUnit != nullptr &&
                            pTargetUnit->getOwner()->checkAlliance(m_pPlayer) == GameEnums::Alliance_Friend)
                        {
                            alliedmultiplier = m_alliedDistanceModifier;
                        }
                        qint32 attackDistanceModifier = m_neuralNetworks[NeuralNetworks::WaitDistanceMultiplier]->output(WaitTargetTypes_Enemy);
                        QVector3D possibleTarget(x, y, (attackDistanceModifier) * alliedmultiplier);
                        if (!targets.contains(possibleTarget))
                        {
                            targets.append(possibleTarget);
                        }
                    }
                }
            }
        }
    }
}

void HeavyAi::addCaptureTransporterTargets(Unit* pUnit, const QStringList & actions,
                                  Terrain* pTerrain, QVector<QVector3D>& targets)
{
    if (actions.contains(ACTION_CAPTURE) ||
        actions.contains(ACTION_MISSILE))
    {
        qint32 x = pTerrain->Terrain::getX();
        qint32 y = pTerrain->Terrain::getY();
        Unit* pTransporter = pTerrain->getUnit();
        if (pTransporter != nullptr &&
            pTransporter->getOwner() == m_pPlayer &&
            pTransporter->canTransportUnit(pUnit))
        {
            qint32 distanceModifier = m_neuralNetworks[NeuralNetworks::WaitDistanceMultiplier]->output(WaitTargetTypes_CaptureLoad);
            if (distanceModifier < 1)
            {
                distanceModifier = 1;
            }
            QVector3D possibleTarget(x, y, distanceModifier);
            if (!targets.contains(possibleTarget))
            {
                targets.append(possibleTarget);
            }
        }
    }
}

void HeavyAi::addTransporterTargets(Unit* pUnit, Terrain* pTerrain, QVector<QVector3D>& targets)
{
    qint32 x = pTerrain->Terrain::getX();
    qint32 y = pTerrain->Terrain::getY();
    Unit* pTransporter = pTerrain->getUnit();
    if (pTransporter != nullptr &&
        pTransporter->getOwner() == m_pPlayer &&
        pTransporter->canTransportUnit(pUnit))
    {
        qint32 distanceModifier = m_neuralNetworks[NeuralNetworks::WaitDistanceMultiplier]->output(WaitTargetTypes_Load);
        if (distanceModifier < 1)
        {
            distanceModifier = 1;
        }
        QVector3D possibleTarget(x, y, distanceModifier);
        if (!targets.contains(possibleTarget))
        {
            targets.append(possibleTarget);
        }
    }
}

void HeavyAi::scoreWait(ScoreData & data, MoveUnitData & unitData, QVector<double> baseData)
{
    float score = m_minActionScore - 1;
    if (unitData.pUnit->getLoadedUnitCount() > 0)
    {
        // check if unloading can be performed in that case return a negativ result and go for unloading instead
        GameAction unloadAction(CoreAI::ACTION_UNLOAD, m_pMap);
        unloadAction.setTarget(data.m_gameAction->getTarget());
        if (unloadAction.canBePerformed())
        {
            return;
        }
    }
    scoreWaitGeneric(data, unitData, baseData);
}

void HeavyAi::scoreWaitGeneric(ScoreData & data, MoveUnitData & unitData, QVector<double> baseData)
{
    float score = m_minActionScore - 1;
    auto targetPath = m_currentTargetedfPfs->getTargetPath();
    QPoint target = data.m_gameAction->getActionTarget();
    if (m_possibleCaptureTargets.contains(target))
    {
        data.m_captureTarget = target;
    }
    float movePathSize = unitData.m_action->getMovePathLength() - 1;
    if (movePathSize > 0)
    {
        float distance = getDistanceToMovepath(targetPath, target);
        score += m_maxScore * 0.5f / distance;
        score += m_maxScore * 0.5f * movePathSize / static_cast<float>(unitData.movementPoints);
    }
    data.m_score = score;
}

qint32 HeavyAi::getDistanceToMovepath(const QVector<QPoint> & targetPath, const QPoint & target) const
{
    qint32 distance = std::numeric_limits<qint32>::max();
    if (!targetPath.contains(target))
    {
        for (const auto & path : qAsConst(targetPath))
        {
            qint32 newDistance = GlobalUtils::getDistance(path, target) + 1;
            if (newDistance < distance)
            {
                distance = newDistance;
            }
        }
    }
    else
    {
        distance = 1;
    }
    return distance;
}

bool HeavyAi::isPrimaryEnemy(Player* pPlayer) const
{
    return pPlayer == m_pPrimaryEnemy;
}

bool HeavyAi::isPrimaryEnemy(Unit* pUnit) const
{
    return isPrimaryEnemy(pUnit->getOwner());
}

bool HeavyAi::isPrimaryEnemy(Building* pBuilding) const
{
    return isPrimaryEnemy(pBuilding->getOwner());
}
