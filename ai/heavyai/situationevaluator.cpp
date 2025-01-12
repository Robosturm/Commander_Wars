#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"

#include "ai/heavyai/situationevaluator.h"
#include "ai/heavyai/unittargetedpathfindingsystem.h"
#include "ai/coreai.h"

#include "coreengine/settings.h"
#include "coreengine/globalutils.h"
#include "game/gamemap.h"
#include "game/GameEnums.h"
#include "game/unit.h"

SituationEvaluator::SituationEvaluator(Player* pOwner)
    : m_inputVector(1, HeavyAiSharedData::INPUT_VECTOR_SIZE),
    m_searchRange(GlobalUtils::getSpCircle(0, HeavyAiSharedData::UNIT_SEARCH_RANGE)),
    m_pOwner(pOwner)
{
    for (qint32 i = 0; i < HeavyAiSharedData::UNIT_COUNT; ++i)
    {
        m_unitsInfo[i] = MemoryManagement::create<HeavyAiSharedData::UnitInfo>();
        m_unitsInfo[i]->stealthed.resize(pOwner->getMap()->getPlayerCount());
    }
}

void SituationEvaluator::loadNetwork(const QString & filePath)
{
    QString finalPath = Settings::userPath() + "/" + filePath;
    if (QFile::exists(finalPath))
    {
        finalPath = QString(oxygine::Resource::RCC_PREFIX_PATH) + "/" + filePath;
    }
    if (QFile::exists(finalPath))
    {
        QFile file(finalPath);
        file.open(QIODevice::ReadOnly);
        QTextStream stream(&file);
        QString content = stream.readAll();
        tinyxml2::XMLDocument xml;
        xml.Parse(content.toStdString().c_str());
        m_neuralNetwork.from_XML(xml);
    }
}

void SituationEvaluator::updateInputVector(GameMap* pMap, const QPoint & searchPoint, bool updateUnitData)
{
    if (updateUnitData)
    {
        getUnitsInRange(pMap, searchPoint);
        createPathFindingSystems(pMap);
    }
    for (qint32 i = 0; i < HeavyAiSharedData::INPUT_VECTOR_SIZE; ++i)
    {
        m_inputVector(0, i) = 0;
    }
    for (qint32 i = 0; i < HeavyAiSharedData::UNIT_COUNT; ++i)
    {
        Unit* pUnit = m_unitsInfo[i]->pUnit;
        if (pUnit != nullptr)
        {
            fillUnitInput(i);
        }
    }
}

QString SituationEvaluator::getInputVector() const
{
    QString input;
    for (qint32 i = 0; i < HeavyAiSharedData::INPUT_VECTOR_SIZE; ++i)
    {
        input += QString::number(m_inputVector(0, i)) + ";";
    }
    return input;
}

float SituationEvaluator::getOutput()
{
    auto inputDimension = opennn::get_dimensions(m_inputVector);
    Tensor<opennn::type, 2> outputs = m_neuralNetwork.calculate_outputs(m_inputVector.data(), inputDimension);
    return outputs(0);
}

void SituationEvaluator::clearUnitInput(qint32 index)
{
    for (qint32 feature = 0; feature < HeavyAiSharedData::SituationFeatures::SituationFeatures_MaxFeatures; ++feature)
    {
        qint32 basePosition = HeavyAiSharedData::UNIT_COUNT * HeavyAiSharedData::UNIT_COUNT * feature + index * HeavyAiSharedData::UNIT_COUNT;
        for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::fillUnitInput(qint32 index)
{
    for (qint32 feature = 0; feature < HeavyAiSharedData::SituationFeatures::SituationFeatures_MaxFeatures; ++feature)
    {
        qint32 basePosition = HeavyAiSharedData::UNIT_COUNT * HeavyAiSharedData::UNIT_COUNT * feature + index * HeavyAiSharedData::UNIT_COUNT;
        using updateFeature = void (SituationEvaluator::*)(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo);
        constexpr std::array<updateFeature, HeavyAiSharedData::SituationFeatures::SituationFeatures_MaxFeatures> featureCb{
            &SituationEvaluator::updateDistance,
            &SituationEvaluator::updateHp,
            &SituationEvaluator::updateHpDamage,
            &SituationEvaluator::updateFundsDamage,
            &SituationEvaluator::updateMovementPoints,
            &SituationEvaluator::updateHasMoved,
            &SituationEvaluator::updateDefense,
            &SituationEvaluator::updateRepairsOnPosition,
            &SituationEvaluator::updateCapturePoints,
            &SituationEvaluator::updateBuildingImportance,
            &SituationEvaluator::updateStealthed,
            &SituationEvaluator::updateMinFirerange,
            &SituationEvaluator::updateMaxFirerange,
            &SituationEvaluator::updateCoBonus,
        };
        (this->*featureCb[feature])(basePosition, m_unitsInfo[index]);
    }
}

void SituationEvaluator::updateHp(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->pUnit->getAiCache()[HeavyAiSharedData::AiCache::CoBonus];
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateCoBonus(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->pUnit->getHpRounded();
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateHpDamage(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    bool hasWeapons = unitInfo->pUnit->hasWeapons();
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (hasWeapons && shouldFillInfo(unitInfo, enemyUnit))
        {
            const QString func = "calcAttackerWeaponDamage";
            QRectF erg;
            QJSValueList args({JsThis::getJsThis(nullptr),
                               JsThis::getJsThis(unitInfo->pUnit),
                               0,
                               pInterpreter->toScriptValue(unitInfo->pUnit->getPosition()),
                               JsThis::getJsThis(m_unitsInfo[enemyUnit]->pUnit),
                               QJSValue(m_unitsInfo[enemyUnit]->pUnit->getX()),
                               QJSValue(m_unitsInfo[enemyUnit]->pUnit->getY()),
                               0,
                               QJSValue(static_cast<qint32>(GameEnums::LuckDamageMode_Average)),
                               pInterpreter->toScriptValue(erg),
                               QJSValue(static_cast<qint32>(GameEnums::AttackRangeCheck_None))});
            QJSValue jsErg = pInterpreter->doFunction(CoreAI::ACTION_FIRE, func, args);
            erg = jsErg.toVariant().toRectF();
            m_inputVector(0, basePosition + enemyUnit) = erg.x() * Unit::MAX_UNIT_HP / Unit::DAMAGE_100;
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateFundsDamage(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{

    qint32 hpOffset = HeavyAiSharedData::UNIT_COUNT * HeavyAiSharedData::UNIT_COUNT * (HeavyAiSharedData::SituationFeatures::SituationFeatures_HpDamage - HeavyAiSharedData::SituationFeatures::SituationFeatures_HP);
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {

            auto hpDamage = m_inputVector(0, basePosition + enemyUnit - hpOffset);
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->pUnit->getAiCache()[HeavyAiSharedData::AiCache::BaseCost] * hpDamage / Unit::MAX_UNIT_HP;
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateMovementPoints(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->pUnit->getAiCache()[HeavyAiSharedData::AiCache::UnitMovementPoints];
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateDistance(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (m_unitsInfo[enemyUnit]->pUnit != nullptr)
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->multiplier * GlobalUtils::getDistance(unitInfo->pUnit->getX(), unitInfo->pUnit->getY(),
                                                                                  m_unitsInfo[enemyUnit]->pUnit->getX(), m_unitsInfo[enemyUnit]->pUnit->getY());
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateHasMoved(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (unitInfo->reachable[enemyUnit])
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->pUnit->getHasMoved();
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateDefense(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->terrainDefense;
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateRepairsOnPosition(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->pUnit->getAiCache()[HeavyAiSharedData::AiCache::CurrentRepair];
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateMinFirerange(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->pUnit->getAiCache()[HeavyAiSharedData::AiCache::MinFirerange];
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateMaxFirerange(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->pUnit->getAiCache()[HeavyAiSharedData::AiCache::MaxFirerange];
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

Player * SituationEvaluator::getOwner() const
{
    return m_pOwner;
}

void SituationEvaluator::setOwner(Player * newPOwner)
{
    m_pOwner = newPOwner;
}

void SituationEvaluator::updateCapturePoints(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->pUnit->getCapturePoints();
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateBuildingImportance(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->buildingImportance;
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }

}

void SituationEvaluator::updateStealthed(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {

            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->stealthed[unitInfo->pUnit->getOwner()->getPlayerID()];
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

bool SituationEvaluator::shouldFillInfo(const HeavyAiSharedData::spUnitInfo & unitInfo, qint32 enemyUnit)
{
    return unitInfo->multiplier != m_unitsInfo[enemyUnit]->multiplier &&
           unitInfo->reachable[enemyUnit] &&
           m_unitsInfo[enemyUnit]->pUnit != nullptr;
}

void SituationEvaluator::getUnitsInRange(GameMap* pMap, const QPoint & searchPoint)
{
    qint32 alliedPosition = 0;
    qint32 enemyPosition = HeavyAiSharedData::UNIT_COUNT - 1;
    for (const auto & point : m_searchRange->getVector())
    {
        QPoint mapPoint = point + searchPoint;
        Terrain* pTerrain = pMap->getTerrain(mapPoint);
        if (pTerrain != nullptr)
        {
            Unit* pUnit = pTerrain->getUnit();
            if (pUnit != nullptr && pUnit->getHpRounded() > 0)
            {
                if (m_pOwner->isEnemyUnit(pUnit))
                {
                    m_unitsInfo[enemyPosition]->pUnit = pUnit;
                    m_unitsInfo[enemyPosition]->multiplier = -1;
                    m_unitsInfo[enemyPosition]->reachable.fill(false);
                    updateStealthInfo(pMap, enemyPosition);
                    m_unitsInfo[enemyPosition]->terrainDefense = pUnit->getTerrainDefense();
                    --enemyPosition;
                }
                else
                {
                    m_unitsInfo[alliedPosition]->pUnit = pUnit;
                    m_unitsInfo[alliedPosition]->multiplier = 1;
                    m_unitsInfo[alliedPosition]->reachable.fill(false);
                    updateStealthInfo(pMap, alliedPosition);
                     m_unitsInfo[alliedPosition]->terrainDefense = pUnit->getTerrainDefense();
                    ++alliedPosition;
                }
                if (alliedPosition > enemyPosition)
                {
                    break;
                }
            }
        }
    }
    for (qint32 i = alliedPosition; i <= enemyPosition; ++i)
    {
        m_unitsInfo[i]->pUnit = nullptr;
        m_unitsInfo[i]->pUnitTargetedPathFindingSystem.reset();
        m_unitsInfo[i]->multiplier = 1;
        m_unitsInfo[i]->terrainDefense = 0;
        m_unitsInfo[i]->buildingImportance = 0;
    }
}

void SituationEvaluator::updateBuildingImportance(qint32 unitPosition)
{
    auto* pUnit = m_unitsInfo[unitPosition]->pUnit;
    auto* pTerrain = pUnit->getTerrain();
    if (pTerrain != nullptr)
    {
        Building* pBuilding = pTerrain->getBuilding();
        if (pBuilding != nullptr)
        {
            if (pBuilding->isHq())
            {
                m_unitsInfo[unitPosition]->buildingImportance = HeavyAiSharedData::HQ_IMPORTANCE;
            }
            else
            {
                m_unitsInfo[unitPosition]->buildingImportance = pBuilding->getConstructionList().size();
            }
        }
    }
    else
    {
        m_unitsInfo[unitPosition]->buildingImportance = 0;
    }
}

void SituationEvaluator::updateStealthInfo(GameMap* pMap, qint32 unitPosition)
{
    for (qint32 i = 0; i < pMap->getPlayerCount(); ++i)
    {
        m_unitsInfo[unitPosition]->stealthed[i] = m_unitsInfo[unitPosition]->pUnit->isStealthed(pMap->getPlayer(i));
    }
}

void SituationEvaluator::createPathFindingSystems(GameMap* pMap)
{
    for (qint32 i = 0; i < HeavyAiSharedData::UNIT_COUNT; ++i)
    {
        if (m_unitsInfo[i]->pUnit != nullptr)
        {
            m_unitsInfo[i]->pUnitTargetedPathFindingSystem = MemoryManagement::create<UnitTargetedPathFindingSystem>(pMap, i, m_unitsInfo);
            m_unitsInfo[i]->pUnitTargetedPathFindingSystem->explore();
        }
        else
        {
            m_unitsInfo[i]->clear();
        }
    }
}
