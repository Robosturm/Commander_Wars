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
                                                 "ACTION_CAPTURE"
                                                 "WAIT_DISTANCE_MULTIPLIER"};
// normally i'm not a big fan of this but else the function table gets unreadable
using namespace std::placeholders;


HeavyAi::HeavyAi(QString type, GameEnums::AiTypes aiType)
    : CoreAI(aiType),
      m_scoreInfos({
{ACTION_CAPTURE,  std::bind(&HeavyAi::scoreCapture,     this,   _1, _2, _3)},
{ACTION_FIRE,     std::bind(&HeavyAi::scoreFire,        this,   _1, _2, _3)},
{ACTION_JOIN,     std::bind(&HeavyAi::scoreJoin,        this,   _1, _2, _3)},
{ACTION_MISSILE,  std::bind(&HeavyAi::scoreMissile,     this,   _1, _2, _3)},

{ACTION_LOAD,     std::bind(&HeavyAi::scoreLoad,        this,   _1, _2, _3)},
{ACTION_WAIT,     std::bind(&HeavyAi::scoreWait,        this,   _1, _2, _3)},
                   }),
      m_InfluenceFrontMap(m_IslandMaps),
      m_timer(this),
      m_aiName(type)
{
    setObjectName("HeavyAi");
    m_timer.setSingleShot(false);
    connect(&m_timer, &QTimer::timeout, this, &HeavyAi::process, Qt::QueuedConnection);
    loadIni("heavy/" + m_aiName.toLower() + ".ini");
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
    loadNeuralNetwork(NeuralNetworkNames[NeuralNetworks::ActionFire], m_neuralNetworks[NeuralNetworks::ActionFire], static_cast<qint32>(AttackInfo::AttackInfoMaxSize), AttackInfoMaxSize / 4, randomize);
    loadNeuralNetwork(NeuralNetworkNames[NeuralNetworks::ActionCapture], m_neuralNetworks[NeuralNetworks::ActionCapture], static_cast<qint32>(CaptureInfo::CaptureInfoMaxSize), CaptureInfoMaxSize / 4, randomize);
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

void HeavyAi::readIni(QString name)
{
    if (QFile::exists(name))
    {
        QSettings settings(name, QSettings::IniFormat);
        settings.beginGroup("general");
        bool ok = false;
        m_minActionScore = settings.value("MinActionScore", 0.2f).toFloat(&ok);
        if(!ok || m_minActionScore <= 0)
        {
            m_minActionScore = 0.2f;
        }
        m_actionScoreVariant = settings.value("ActionScoreVariant", 0.05f).toFloat(&ok);
        if(!ok || m_actionScoreVariant <= 0)
        {
            m_actionScoreVariant = 0.05f;
        }
        m_stealthDistanceMultiplier = settings.value("StealthDistanceMultiplier", 2.00f).toFloat(&ok);
        if(!ok )
        {
            m_stealthDistanceMultiplier = 2.0f;
        }
        if (m_stealthDistanceMultiplier < 1.0f)
        {
            m_stealthDistanceMultiplier = 1.0f;
        }

        m_alliedDistanceModifier = settings.value("AlliedDistanceModifier", 5.0f).toFloat(&ok);
        if(!ok || m_alliedDistanceModifier <= 0)
        {
            m_alliedDistanceModifier = 5.0f;
        }
        m_maxMovementpoints = settings.value("MaxMovementpoints", 20.0f).toFloat(&ok);
        if(!ok || m_maxMovementpoints <= 0)
        {
            m_maxMovementpoints = 20.0f;
        }
        m_maxProductionTurnRange = settings.value("MaxProductionTurnRange", 4.0f).toFloat(&ok);
        if(!ok || m_maxProductionTurnRange <= 0)
        {
            m_maxProductionTurnRange = 4.0f;
        }
        m_maxFirerange = settings.value("MaxFirerange", 10).toFloat(&ok);
        if(!ok || m_maxFirerange <= 0)
        {
            m_maxFirerange = 10.0f;
        }
        m_primaryEnemyMultiplier = settings.value("PrimaryEnemyMultiplier", 1.2f).toFloat(&ok);
        if(!ok || m_primaryEnemyMultiplier <= 0)
        {
            m_primaryEnemyMultiplier = 1.2f;
        }
        m_maxLoadingPlace = settings.value("MaxLoadingPlace", 4.0f).toFloat(&ok);
        if(!ok || m_maxLoadingPlace <= 0)
        {
            m_maxLoadingPlace = 4.0f;
        }
        m_notAttackableDamage = settings.value("NotAttackableDamage", 30.0f).toFloat(&ok);
        if(!ok || m_notAttackableDamage <= 0)
        {
            m_notAttackableDamage = 30.0f;
        }
        m_ownUnitProtection = settings.value("OwnUnitProtection", 5.0f).toFloat(&ok);
        if(!ok || m_ownUnitProtection <= 0)
        {
            m_ownUnitProtection = 5.0f;
        }
        m_enemyUnitThread = settings.value("EnemyUnitThread", 5.0f).toFloat(&ok);
        if(!ok || m_enemyUnitThread <= 0)
        {
            m_enemyUnitThread = 5.0f;
        }
        m_maxVision = settings.value("MaxVision", 10.0f).toFloat(&ok);
        if(!ok || m_maxVision <= 0)
        {
            m_maxVision = 10.0f;
        }
        m_maxUnitValue = settings.value("MaxUnitValue", 40000.0f).toFloat(&ok);
        if(!ok || m_maxUnitValue <= 0)
        {
            m_maxUnitValue = 40000.0f;
        }
        m_maxScore = settings.value("MaxScore", 10.0f).toFloat(&ok);
        if(!ok || m_maxScore <= 0)
        {
            m_maxScore = 10.0f;
        }
        m_maxTerrainDefense = settings.value("MaxTerrainDefense", 15.0f).toFloat(&ok);
        if(!ok || m_maxTerrainDefense <= 0)
        {
            m_maxTerrainDefense = 15.0f;
        }
        m_maxCapturePoints = settings.value("MaxCapturePoints", 20.0f).toFloat(&ok);
        if(!ok || m_maxCapturePoints <= 0)
        {
            m_maxCapturePoints = 20.0f;
        }
        m_minSameIslandDistance = settings.value("MinSameIslandDistance", 3.0f).toFloat(&ok);
        if(!ok || m_minSameIslandDistance <= 0)
        {
            m_minSameIslandDistance = 3.0f;
        }
        m_slowUnitSpeed = settings.value("SlowUnitSpeed", 4).toInt(&ok);
        if(!ok)
        {
            m_slowUnitSpeed = 3;
        }
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
    if (useBuilding(pBuildings)){}
    else if (useCOPower(spQmlVectorUnit(m_pPlayer->getUnits()),
                        spQmlVectorUnit(m_pPlayer->getEnemyUnits()))){}
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
    if (useCOPower(spQmlVectorUnit(m_pPlayer->getUnits()),
                   spQmlVectorUnit(m_pPlayer->getEnemyUnits())))
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
    CONSOLE_PRINT("HeavyAi selecting action to be performed", Console::eDEBUG);
    float bestScore = std::numeric_limits<float>::min();
    qint32 index = -1;
    qint32 pos = 0;
    for (const auto & ownUnit : qAsConst(m_ownUnits))
    {
        if (ownUnit.m_action.get() != nullptr &&
            ownUnit.m_score > bestScore)
        {
            if (ownUnit.m_pUnit->getTerrain() == nullptr ||
                ownUnit.m_pUnit->getHp() <= 0)
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
        QPoint target = m_ownUnits[index].m_action->getTarget();
        CONSOLE_PRINT("HeavyAi selected action " + m_ownUnits[index].m_action->getActionID() + " to be performed with score " + QString::number(bestScore), Console::eDEBUG);
        m_updatePoints.append(target);
        m_updatePoints.append(m_ownUnits[index].m_action->getActionTarget());
        if (target != m_ownUnits[index].m_pUnit->Unit::getPosition())
        {
            oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "HeavyAi::selectActionToPerform action error");
            m_ownUnits[index].m_action = nullptr;
            m_ownUnits[index].m_score = 0;
            return false;
        }
        emit performAction(m_ownUnits[index].m_action);
        m_ownUnits[index].m_action = nullptr;
        m_ownUnits[index].m_score = 0;
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
        CONSOLE_PRINT("HeavyAi initial start of turn calculation", Console::eDEBUG);
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
        CONSOLE_PRINT("HeavyAi front lines created", Console::eDEBUG);
        findHqThreads(buildings);
        CONSOLE_PRINT("HeavyAi initial scoring actions for units", Console::eDEBUG);
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

void HeavyAi::initUnits(spQmlVectorUnit pUnits, QVector<UnitData> & units, bool enemyUnits)
{
    units.clear();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        addNewUnitToUnitData(units, pUnits->at(i), enemyUnits);
    }
}

void HeavyAi::addNewUnitToUnitData(QVector<UnitData> & units, Unit* pUnit, bool enemyUnits)
{
    UnitData data;
    data.m_pUnit = pUnit;
    data.m_pPfs = spUnitPathFindingSystem::create(pUnit);
    data.m_movepoints = data.m_pUnit->getMovementpoints(data.m_pUnit->getPosition());
    data.m_pPfs->setMovepoints(data.m_movepoints * 2);
    data.m_pPfs->setIgnoreEnemies(enemyUnits);
    data.m_pPfs->explore();
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

void HeavyAi::updateUnits(QVector<UnitData> & units, spQmlVectorUnit & pUnits, bool enemyUnits)
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() == nullptr)
    {
        return;
    }
    qint32 i = 0;
    while (i < units.size())
    {
        if (units[i].m_pUnit->getHp() <= 0 ||
            units[i].m_pUnit->getTerrain() == nullptr)
        {
            units.removeAt(i);
        }
        else
        {
            if (units[i].m_pUnit->getHasMoved())
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
                if (qAbs(m_updatePoints[i].x() - units[i2].m_pUnit->Unit::getX()) +
                    qAbs(m_updatePoints[i].y() - units[i2].m_pUnit->Unit::getY()) <=
                    units[i2].m_pUnit->getMovementpoints(QPoint(units[i2].m_pUnit->Unit::getX(), units[i2].m_pUnit->Unit::getY())) + 2)
                {
                    units[i2].m_pPfs = spUnitPathFindingSystem::create(units[i2].m_pUnit);
                    units[i2].m_pPfs->setIgnoreEnemies(enemyUnits);
                    units[i2].m_pPfs->explore();
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
                if (pMap->onMap(pos.x(), pos.y()))
                {
                    bool found = false;
                    Unit* pUnit = pMap->getTerrain(pos.x(), pos.y())->getUnit();
                    if (pUnit != nullptr &&
                        pUnit->getOwner() == m_pPlayer)
                    {
                        for (const auto & unit : units)
                        {
                            if (unit.m_pUnit == pUnit)
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

void HeavyAi::updateCaptureBuildings(UnitData & unitData)
{
    if (unitData.m_pUnit->getActionList().contains(ACTION_CAPTURE))
    {
        unitData.m_capturePoints.clear();
        GameAction action(ACTION_CAPTURE);
        action.setTarget(QPoint(unitData.m_pUnit->Unit::getX(), unitData.m_pUnit->Unit::getY()));
        QVector<QPoint> targets = unitData.m_pPfs->getAllNodePoints(unitData.m_movepoints);
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
    CONSOLE_PRINT("Searching for HQ Threads", Console::eDEBUG);
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
    CONSOLE_PRINT("HeavyAi::scoreActions", Console::eDEBUG);
    if (!unit.m_pUnit->getHasMoved())
    {
        if (unit.m_pUnit->getTerrain() == nullptr ||
            unit.m_pUnit->getHp() <= 0)
        {
            oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "invalid unit found");
            unit.m_action = nullptr;
            unit.m_score = 0;
            return;
        }
        QVector<float> bestScores;
        QVector<spGameAction> bestActions;
        float bestScore = 0.0f;
        unit.m_actions = unit.m_pUnit->getActionList();
        auto moveTargets = unit.m_pPfs->getAllNodePoints(unit.m_movepoints + 1);
        if (m_aiStep >= AISteps::moveToTargets ||
            unit.m_actions.contains(ACTION_CAPTURE))
        {
            prepareWaitPfs(unit, unit.m_actions);
        }
        for (const auto & action : unit.m_actions)
        {
            if (isScoringAllowed(action, unit.m_actions))
            {
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

bool HeavyAi::isScoringAllowed(QString action, QStringList actions)
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

void HeavyAi::mutateActionForFields(UnitData & unitData, const QVector<QPoint> & moveTargets,
                                    QString action, FunctionType type, qint32 index,
                                    float & bestScore, QVector<float> & bestScores,
                                    QVector<spGameAction> & bestActions)
{
    CONSOLE_PRINT("HeavyAi::mutateActionForFields " + action, Console::eDEBUG);
    for (const auto & target : moveTargets)
    {
        QVector<QPoint> path = unitData.m_pPfs->getPath(target.x(), target.y());
        qint32 costs = unitData.m_pPfs->getCosts(path);
        bool mutate = true;
        QVector<qint32> stepPosition;
        QVector<double> baseData(static_cast<qsizetype>(BasicFieldInfo::BasicFieldInfoMaxSize));
        spGameAction pDummy  = spGameAction::create();
        pDummy->setActionID(action);
        pDummy->setMovepath(path, costs);
        pDummy->setTarget(QPoint(unitData.m_pUnit->Unit::getX(), unitData.m_pUnit->Unit::getY()));
        getBasicFieldInputVector(pDummy, baseData);
        while (mutate)
        {
            spGameAction pAction  = spGameAction::create();
            pAction->setActionID(action);
            pAction->setMovepath(path, costs);
            pAction->setTarget(QPoint(unitData.m_pUnit->Unit::getX(), unitData.m_pUnit->Unit::getY()));
            qint32 step = 0;
            if (pAction->canBePerformed())
            {
                float score = 0;
                mutate = mutateAction(pAction, unitData, baseData, type, index, step, stepPosition, score);
                if (score > m_maxScore)
                {
                    score = m_maxScore;
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

bool HeavyAi::mutateAction(spGameAction pAction, UnitData & unitData, QVector<double> & baseData, FunctionType type, qint32 functionIndex, qint32 & step, QVector<qint32> & stepPosition, float & score)
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
                score = m_scoreInfos[functionIndex].callback(pAction, unitData, baseData);
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
                        ret = mutateAction(pAction, unitData, baseData, type, functionIndex, step, stepPosition, score);
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
            if (pAction->getActionID() == ACTION_MISSILE)
            {
                // special case for missile
                if (nextStep == 0)
                {
                    qint32 cost = 0;
                    QPoint rocketTarget = m_pPlayer->getSiloRockettarget(2, 3, cost);
                    CoreAI::addSelectedFieldData(pAction, rocketTarget);
                    ret = mutateAction(pAction, unitData, baseData, type, functionIndex, step, stepPosition, score);
                }
            }
            else
            {
                spMarkedFieldData pData = pAction->getMarkedFieldStepData();
                QVector<QPoint>* pFields = pData->getPoints();
                for (qint32 i = nextStep; i < pFields->size(); ++i)
                {
                    CoreAI::addSelectedFieldData(pAction, (*pFields)[i]);
                    stepPosition[step] = i;
                    ret = mutateAction(pAction, unitData, baseData, type, functionIndex, step, stepPosition, score);
                    break;
                }
            }
        }
    }
    return ret;
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
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        bool fogOfWar = pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off;
        QPoint moveTarget = action->getActionTarget();
        Unit* pMoveUnit = action->getTargetUnit();
        double movepoints = pMoveUnit->getMovementpoints(action->getTarget());
        Terrain* pTerrainTarget = pMap->getTerrain(moveTarget.x(), moveTarget.y());
        Building* pBuildingTarget = pTerrainTarget->getBuilding();
        qint32 playerId = m_pPlayer->getPlayerID();
        const double highestInfluece = m_InfluenceFrontMap.getTotalHighestInfluence();
        const auto & info = m_InfluenceFrontMap.getInfluenceInfo(moveTarget.x(), moveTarget.y());
        double notMovedUnitCount = 0;
        double protectionValue = 0;
        double ownValue = pMoveUnit->getUnitValue();
        for (const auto & pUnit : qAsConst(m_ownUnits))
        {
            if (pUnit.m_pUnit->getHasMoved())
            {
                ++notMovedUnitCount;
            }
            qint32 cost = pUnit.m_pPfs->getTargetCosts(moveTarget.x(), moveTarget.y());
            if (cost >= 0 && cost <= movepoints)
            {
                protectionValue += pUnit.m_pUnit->getUnitValue();
            }
        }
        double threadValue = 0;
        for (const auto & pUnit : qAsConst(m_ownUnits))
        {
            qint32 cost = pUnit.m_pPfs->getTargetCosts(moveTarget.x(), moveTarget.y());
            if (cost >= 0 && cost <= movepoints)
            {
                threadValue += pUnit.m_pUnit->getUnitValue();
            }
        }
        data[BasicFieldInfo::OwnInfluenceValue] = static_cast<double>(info.ownInfluence) / highestInfluece;
        data[BasicFieldInfo::EnemyInfluenceValue] = static_cast<double>(info.highestEnemyInfluence) / highestInfluece;
        data[BasicFieldInfo::MoveTurnProgress] = notMovedUnitCount / static_cast<double>(m_ownUnits.size());
        spQmlVectorPoint pCircle = spQmlVectorPoint(GlobalUtils::getCircle(1, 1));
        double wallCount = 0;
        for (qint32 i = 0; i < pCircle->size(); ++i)
        {
            QPoint pos = pCircle->at(i);
            pos += moveTarget;
            if (pMap->onMap(pos.x(), pos.y()))
            {
                Unit* pUnit = pMap->getTerrain(pos.x(), pos.y())->getUnit();
                if (pUnit != nullptr &&
                    m_pPlayer->isAlly(pUnit->getOwner()))
                {
                    ++wallCount;
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
        data[BasicFieldInfo::UsedMovement] = static_cast<double>(action->getCosts()) / movepoints;
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
    }
}

float HeavyAi::scoreCapture(spGameAction action, UnitData & unitData, QVector<double> baseData)
{
    baseData.append(QList<double>(static_cast<qsizetype>(CaptureInfoMaxSize - CaptureInfoStart)));
    Building* pBuilding = action->getMovementBuilding();
    auto target = action->getActionTarget();
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
    if (target == action->getTarget())
    {
        remainingPoints -= unitData.m_pUnit->getCapturePoints();
    }
    double captureRate = unitData.m_pUnit->getCaptureRate(target);
    if (captureRate > remainingPoints)
    {
        baseData[CaptureInfoRemainingDays] = 1.0;
    }
    else
    {
        baseData[CaptureInfoRemainingDays] = captureRate / remainingPoints;
    }
    auto score = m_neuralNetworks[NeuralNetworks::ActionCapture]->predict(baseData);
    return score[0];
}

float HeavyAi::scoreFire(spGameAction action, UnitData & unitData, QVector<double> baseData)
{
    spGameMap pMap = GameMap::getInstance();
    baseData.append(QList<double>(static_cast<qsizetype>(AttackInfoMaxSize - AttackInfoStart)));
    if (pMap.get() != nullptr)
    {
        action->startReading();
        Unit* pAttacker = action->getTargetUnit();
        double attackerHp = pAttacker->getHp() * Unit::MAX_UNIT_HP;
        double attackerUnitValue = pAttacker->getUnitValue();
        qint32 x = action->readDataInt32();
        qint32 y = action->readDataInt32();
        Terrain* pTerrain = pMap->getTerrain(x, y);
        Unit* pDefUnit = pTerrain->getUnit();
        auto data = calcUnitDamage(action, QPoint(x, y));
        if (data.x() < 0)
        {
            return 0;
        }
        else if (pDefUnit != nullptr)
        {
            double defenderUnitHp = pDefUnit->getHp() * Unit::MAX_UNIT_HP;
            double defenderUnitValue = pDefUnit->getUnitValue();
            double atkDamage = data.x();
            if (atkDamage > defenderUnitHp)
            {
                atkDamage = defenderUnitHp;
            }
            double defDamage = data.width();
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
                if (enemy.m_pUnit == pDefUnit)
                {
                    baseData[AttackInfo::HqThread] = static_cast<double>(enemy.m_threadLevel) / static_cast<double>(ThreadLevel::Max);
                    break;
                }
            }
        }
        else if (isAttackOnTerrainAllowed(pTerrain, data.x()))
        {
            Building* pBuilding = pTerrain->getBuilding();
            double atkDamage = data.x();
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
            return 0;
        }
    }
    auto score = m_neuralNetworks[NeuralNetworks::ActionFire]->predict(baseData);
    return score[0];
}

float HeavyAi::scoreJoin(spGameAction action, UnitData & unitData, QVector<double> baseData)
{
    Unit* pJoinUnit = action->getMovementTarget();
    float ret = -m_maxScore;
    if (pJoinUnit != nullptr &&
        pJoinUnit->getHasMoved() &&
        pJoinUnit->getCapturePoints() > 0)
    {
        // simple code to give the ai the option to join capture
        float resultHp = pJoinUnit->getHpRounded() + unitData.m_pUnit->getHpRounded();
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
    return ret;
}

float HeavyAi::scoreMissile(spGameAction action, UnitData & unitData, QVector<double> baseData)
{
    float ret = -m_maxScore;
    bool fireSilos = hasMissileTarget();
    if (fireSilos ||
        baseData[EnemyInfluenceValue] >= baseData[OwnInfluenceValue])
    {
        ret = m_maxScore;
    }
    return ret;
}

float HeavyAi::scoreLoad(spGameAction action, UnitData & unitData, QVector<double> baseData)
{
    float ret = -m_maxCapturePoints;
    // we got a transporter is it a good one
    Unit* pTransporter = action->getMovementTarget();
    if (pTransporter != nullptr &&
        !pTransporter->getHasMoved())
    {
        if (unitData.m_actions.contains(ACTION_CAPTURE))
        {
            // score gets better for infantries with low movement
            ret = m_maxScore / unitData.m_movepoints;
        }
        else
        {
            qint32 ownValue = 0;
            qint32 enemyValue = 0;
            qint32 island = getIsland(unitData.m_pUnit);
            m_IslandMaps[island]->getValueOnIsland(getIslandIndex(unitData.m_pUnit), ownValue, enemyValue);
            QVector<QPoint> ignoreList = {action->getActionTarget()};
            qint32 targets = getNumberOfTargetsOnIsland(ignoreList);
            QVector<QVector3D> transporterTargets;
            appendTransporterTargets(unitData.m_pUnit, m_pUnits, transporterTargets);
            // Is it a useful transporter at all?
            if (CoreAI::contains(transporterTargets, action->getActionTarget()))
            {
                if (targets == 0)
                {
                    ret = m_maxScore * unitData.m_pUnit->getUnitValue() / m_maxUnitValue;
                }
                else
                {
                    float unitValue = unitData.m_pUnit->getUnitValue();
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
    return ret;
}

qint32 HeavyAi::getNumberOfTargetsOnIsland(const QVector<QPoint> & ignoreList)
{
    qint32 ret = 0;
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        const auto & targets = m_currentTargetedfPfs->getTargets();
        for (const auto & point : targets)
        {
            if (!ignoreList.contains(QPoint(point.x(), point.y())))
            {
                Unit* pUnit = pMap->getTerrain(point.x(), point.y())->getUnit();
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
    CONSOLE_PRINT("HeavyAi scoring wait actions if needed", Console::eDEBUG);
    for (auto & unit : m_ownUnits)
    {
        if (!unit.m_pUnit->getHasMoved() &&
            unit.m_action.get() == nullptr)
        {
            QStringList actions = unit.m_pUnit->getActionList();
            prepareWaitPfs(unit, actions);
            for (const auto & action : qAsConst(actions))
            {
                QVector<float> bestScores;
                QVector<spGameAction> bestActions;
                float bestScore = 0.0f;
                spGameAction pAction;
                FunctionType type = FunctionType::CPlusPlus;
                qint32 index = -1;
                getFunctionType(action, type, index);
                if (index >= 0)
                {
                    auto moveTargets = unit.m_pPfs->getAllNodePoints(unit.m_movepoints + 1);
                    mutateActionForFields(unit, moveTargets, action, type, index,
                                          bestScore, bestScores, bestActions);
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
            m_currentTargetedfPfs = nullptr;
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

void HeavyAi::getMoveTargets(UnitData & unit, QStringList & actions, QVector<QVector3D> & targets)
{
    // todo
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        qint32 mapWidth = pMap->getMapWidth();
        qint32 mapHeight = pMap->getMapHeight();
        qint32 unitIslandIdx = getIslandIndex(unit.m_pUnit);
        qint32 minFirerange = unit.m_pUnit->getMinRange(unit.m_pUnit->getPosition());
        qint32 maxFirerange = unit.m_pUnit->getMaxRange(unit.m_pUnit->getPosition());
        spQmlVectorPoint pTargetFields = spQmlVectorPoint(GlobalUtils::getCircle(minFirerange, maxFirerange));

        QVector<double> input;
        // todo create input vector
        m_neuralNetworks[NeuralNetworks::WaitDistanceMultiplier]->predict(input);

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
                    addCaptureTransporterTargets(unit.m_pUnit, actions, pTerrain, targets);
                    addTransporterTargets(unit.m_pUnit, pTerrain, targets);
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
        bool missileTarget = hasMissileTarget();
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
            QVector3D possibleTarget(x, y, captureDistanceModifier);
            if (!targets.contains(possibleTarget))
            {
                targets.append(possibleTarget);
            }
        }
    }
}

void HeavyAi::addAttackTargets(Unit* pUnit, Terrain* pTerrain, QmlVectorPoint* pTargetFields, QVector<QVector3D> & targets)
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
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

float HeavyAi::scoreWait(spGameAction action, UnitData & unitData, QVector<double> baseData)
{
    float score = 0.0f;
    if (unitData.m_pUnit->getLoadedUnitCount() > 0)
    {
        // todo check if unloading can be performed in that case return a negativ result
    }
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
