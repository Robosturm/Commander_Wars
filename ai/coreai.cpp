#include "ai/coreai.h"

#include "ai/targetedunitpathfindingsystem.h"


#include "game/gameanimation/gameanimationfactory.h"

#include "game/gamemap.h"

#include "game/unit.h"

#include "game/gameaction.h"

#include "game/unitpathfindingsystem.h"

#include "game/player.h"

#include "game/co.h"

#include "menue/gamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/globalutils.h"

#include "resource_management/cospritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/buildingspritemanager.h"

#include <qfile.h>

const QString CoreAI::ACTION_WAIT = "ACTION_WAIT";
const QString CoreAI::ACTION_HOELLIUM_WAIT = "ACTION_HOELLIUM_WAIT";
const QString CoreAI::ACTION_SUPPORTSINGLE = "ACTION_SUPPORTSINGLE";
const QString CoreAI::ACTION_SUPPORTSINGLE_REPAIR = "ACTION_SUPPORTSINGLE_REPAIR";
const QString CoreAI::ACTION_SUPPORTSINGLE_FREEREPAIR = "ACTION_SUPPORTSINGLE_FREEREPAIR";
const QString CoreAI::ACTION_SUPPORTALL = "ACTION_SUPPORTALL";
const QString CoreAI::ACTION_SUPPORTALL_RATION = "ACTION_SUPPORTALL_RATION";
const QString CoreAI::ACTION_UNSTEALTH = "ACTION_UNSTEALTH";
const QString CoreAI::ACTION_STEALTH = "ACTION_STEALTH";
const QString CoreAI::ACTION_BUILD_UNITS = "ACTION_BUILD_UNITS";
const QString CoreAI::ACTION_CAPTURE = "ACTION_CAPTURE";
const QString CoreAI::ACTION_MISSILE = "ACTION_MISSILE";
const QString CoreAI::ACTION_PLACE = "ACTION_PLACE";
const QString CoreAI::ACTION_FIRE = "ACTION_FIRE";
const QString CoreAI::ACTION_UNLOAD = "ACTION_UNLOAD";
const QString CoreAI::ACTION_LOAD = "ACTION_LOAD";
const QString CoreAI::ACTION_NEXT_PLAYER = "ACTION_NEXT_PLAYER";
const QString CoreAI::ACTION_SWAP_COS = "ACTION_SWAP_COS";
const QString CoreAI::ACTION_ACTIVATE_TAGPOWER = "ACTION_ACTIVATE_TAGPOWER";
const QString CoreAI::ACTION_ACTIVATE_POWER_CO_0 = "ACTION_ACTIVATE_POWER_CO_0";
const QString CoreAI::ACTION_ACTIVATE_POWER_CO_1 = "ACTION_ACTIVATE_POWER_CO_1";
const QString CoreAI::ACTION_ACTIVATE_SUPERPOWER_CO_0 = "ACTION_ACTIVATE_SUPERPOWER_CO_0";
const QString CoreAI::ACTION_ACTIVATE_SUPERPOWER_CO_1 = "ACTION_ACTIVATE_SUPERPOWER_CO_1";
const QString CoreAI::ACTION_CO_UNIT_0 = "ACTION_CO_UNIT_0";
const QString CoreAI::ACTION_CO_UNIT_1 = "ACTION_CO_UNIT_1";
const QString CoreAI::ACTION_EXPLODE = "ACTION_EXPLODE";
const QString CoreAI::ACTION_FLARE = "ACTION_FLARE";

CoreAI::CoreAI(GameEnums::AiTypes aiType)
    : BaseGameInputIF(aiType),
      m_COPowerTree("resources/aidata/copower.tree", "resources/aidata/copower.txt")
{
    setObjectName("CoreAI");
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    // load static information from scripts to improve performance
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue erg = pInterpreter->doFunction(ACTION_FLARE, "getMinRange");
    if (erg.isNumber())
    {
        m_flareInfo.minRange = erg.toNumber();
    }
    erg = pInterpreter->doFunction(ACTION_FLARE, "getMaxRange");
    if (erg.isNumber())
    {
        m_flareInfo.maxRange = erg.toNumber();
    }
    erg = pInterpreter->doFunction(ACTION_FLARE, "getUnfogRange");
    if (erg.isNumber())
    {
        m_flareInfo.unfogRange = erg.toNumber();
    }
}

CoreAI::~CoreAI()
{
}

void CoreAI::init()
{
    connect(GameMenue::getInstance(), &GameMenue::sigActionPerformed, this, &CoreAI::nextAction, Qt::QueuedConnection);
    connect(this, &CoreAI::performAction, GameMenue::getInstance(), &GameMenue::performAction, Qt::QueuedConnection);

    spGameMap pMap = GameMap::getInstance();
    qint32 heigth = pMap->getMapHeight();
    qint32 width = pMap->getMapWidth();
    for (qint32 x = 0; x < width; x++)
    {
        m_MoveCostMap.append(QVector<std::tuple<qint32, bool>>());
        for (qint32 y = 0; y < heigth; y++)
        {
            m_MoveCostMap[x].append(std::tuple<qint32, bool>(0, false));
        }
    }
}

void CoreAI::loadIni(QString file)
{
    m_files.append(file);
    QStringList searchFiles;
    if (!file.isEmpty())
    {
        searchFiles.append("resources/aidata/" + file);
        // make sure to overwrite existing js stuff
        for (qint32 i = 0; i < Settings::getMods().size(); i++)
        {
            searchFiles.append(Settings::getMods().at(i) + "/aidata/" + file);
        }
    }
    for (qint32 i = 0; i < searchFiles.size(); i++)
    {
        if (QFile::exists(searchFiles[i]))
        {
            readIni(searchFiles[i]);
        }
    }
}

void CoreAI::nextAction()
{
    Console::print("CoreAI::nextAction", Console::eDEBUG);
    // check if it's our turn
    spGameMenue pMenue = GameMenue::getInstance();
    if (pMenue.get() != nullptr &&
        m_pPlayer == GameMap::getInstance()->getCurrentPlayer() &&
        pMenue->getGameStarted())
    {

        if (!processPredefinedAi())
        {
            Console::print("Processing ai specific behaviour", Console::eDEBUG);
            // if so execute next action
            process();
        }
    }
}

bool CoreAI::contains(QVector<QVector3D>& points, QPoint point)
{
    for (qint32 i = 0; i < points.size(); i++)
    {
        if (static_cast<qint32>(points[i].x()) == point.x() &&
            static_cast<qint32>(points[i].y()) == point.y())
        {
            return true;
        }
    }
    return false;
}

qint32 CoreAI::index(QVector<QVector3D>& points, QPoint point)
{
    for (qint32 i = 0; i < points.size(); i++)
    {
        if (static_cast<qint32>(points[i].x()) == point.x() &&
            static_cast<qint32>(points[i].y()) == point.y())
        {
            return i;
        }
    }
    return -1;
}

bool CoreAI::useCOPower(spQmlVectorUnit pUnits, spQmlVectorUnit pEnemyUnits)
{
    Console::print("CoreAI::useCOPower()", Console::eDEBUG);
    QVector<float> data;
    data.append(-1);
    data.append(0);
    data.append(-1);
    data.append(pUnits->size());
    qint32 repairUnits = 0;
    qint32 indirectUnits = 0;
    qint32 directUnits = 0;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (pUnit->getHpRounded() < Unit::MAX_UNIT_HP)
        {
            repairUnits++;
        }
        if (pUnit->getBaseMaxRange() > 1)
        {
            indirectUnits++;
        }
        else
        {
            directUnits++;
        }
    }
    data.append(repairUnits);
    data.append(indirectUnits);
    data.append(directUnits);
    data.append(pEnemyUnits->size());
    data.append(m_pPlayer->getFunds());
    data.append(static_cast<float>(turnMode));

    for (quint8 i = 0; i <= 1; i++)
    {
        CO* pCO = m_pPlayer->getCO(i);
        if (pCO != nullptr)
        {
            data[0] = COSpriteManager::getInstance()->getIndex(pCO->getCoID());
            if (pCO->canUseSuperpower())
            {
                data[1] = 2;
            }
            else if (pCO->canUsePower())
            {
                data[1] = 1;
            }
            else
            {
                data[1] = 0;
            }
            data[2] = pCO->getPowerFilled() - pCO->getPowerStars();



            GameEnums::PowerMode result = pCO->getAiUsePower(data[2], pUnits->size(), repairUnits, indirectUnits,
                    directUnits, pEnemyUnits->size(), turnMode);
            if (result == GameEnums::PowerMode_Unknown)
            {
                result = static_cast<GameEnums::PowerMode>(m_COPowerTree.getDecision(data));
            }

            if (result == GameEnums::PowerMode_Power)
            {
                spGameAction pAction = spGameAction::create(ACTION_ACTIVATE_POWER_CO_0);
                if (i == 1)
                {
                    pAction->setActionID(ACTION_ACTIVATE_POWER_CO_1);
                }
                if (pAction->canBePerformed())
                {
                    emit performAction(pAction);
                    return true;
                }
            }
            else if (result == GameEnums::PowerMode_Superpower)
            {
                spGameAction pAction = spGameAction::create(ACTION_ACTIVATE_SUPERPOWER_CO_0);
                if (i == 1)
                {
                    pAction->setActionID(ACTION_ACTIVATE_SUPERPOWER_CO_1);
                }
                if (pAction->canBePerformed())
                {
                    pAction->setActionID(ACTION_ACTIVATE_TAGPOWER);
                    if (pAction->canBePerformed())
                    {
                        emit performAction(pAction);
                        return true;
                    }
                    else if (i == 1)
                    {
                        pAction->setActionID(ACTION_ACTIVATE_SUPERPOWER_CO_1);
                    }
                    else
                    {
                        pAction->setActionID(ACTION_ACTIVATE_SUPERPOWER_CO_0);
                    }
                    emit performAction(pAction);
                    return true;
                }
            }
        }
    }
    return false;
}

float CoreAI::calcBuildingDamage(Unit* pUnit, QPoint newPosition, Building* pBuilding)
{
    float counterDamage = 0.0f;
    GameEnums::BuildingTarget targets = pBuilding->getBuildingTargets();
    if (targets == GameEnums::BuildingTarget_All ||
        (targets == GameEnums::BuildingTarget_Enemy && m_pPlayer->isEnemy(pBuilding->getOwner())) ||
        (targets == GameEnums::BuildingTarget_Own && m_pPlayer == pBuilding->getOwner()))
    {
        if (pBuilding->getFireCount() <= 1 &&
            pBuilding->getOwner() != nullptr)
        {
            QPoint pos = newPosition - pBuilding->getActionTargetOffset() - pBuilding->getPosition();
            spQmlVectorPoint pTargets = pBuilding->getActionTargetFields();
            if (pTargets.get() != nullptr)
            {
                if (pTargets->contains(pos))
                {
                    float damage = pBuilding->getDamage(pUnit);
                    if (damage > pUnit->getHp())
                    {
                        damage = pBuilding->getHp();
                    }
                    counterDamage = damage / Unit::MAX_UNIT_HP * pUnit->getUnitCosts();
                }
            }
        }
    }
    return counterDamage;
}

void CoreAI::createMovementMap(spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings)
{
    Console::print("CoreAI::createMovementMap", Console::eDEBUG);
    spGameMap pMap = GameMap::getInstance();
    qint32 heigth = pMap->getMapHeight();
    qint32 width = pMap->getMapWidth();
    for (qint32 x = 0; x < width; x++)
    {
        for (qint32 y = 0; y < heigth; y++)
        {
            if (std::get<1>(m_MoveCostMap[x][y]) == false)
            {
                std::get<0>(m_MoveCostMap[x][y]) = 0.0f;
            }
        }
    }
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        float damage = pBuilding->getDamage(nullptr);
        addMovementMap(pBuilding, damage);
    }
    for (qint32 i = 0; i < pEnemyBuildings->size(); i++)
    {
        Building* pBuilding = pEnemyBuildings->at(i);
        if (pBuilding->getOwner() != nullptr)
        {
            float damage = pBuilding->getDamage(nullptr);
            addMovementMap(pBuilding, damage);
        }
    }
}

void CoreAI::addMovementMap(Building* pBuilding, float damage)
{
    GameEnums::BuildingTarget targets = pBuilding->getBuildingTargets();
    QPoint offset = pBuilding->getPosition() + pBuilding->getActionTargetOffset() ;
    if (targets == GameEnums::BuildingTarget_All ||
        (targets == GameEnums::BuildingTarget_Enemy && m_pPlayer->isEnemy(pBuilding->getOwner())))
    {
        if (pBuilding->getFireCount() <= 1)
        {
            spQmlVectorPoint pTargets = pBuilding->getActionTargetFields();
            if (pTargets.get() != nullptr)
            {
                for (qint32 i2 = 0; i2 < pTargets->size(); i2++)
                {
                    QPoint point = pTargets->at(i2) + offset;
                    if ((m_MoveCostMap.size() > point.x() && point.x() >= 0) &&
                        (m_MoveCostMap[point.x()].size() > point.y() && point.y() >= 0))
                    {
                        if (std::get<1>(m_MoveCostMap[point.x()][point.y()]) == false)
                        {
                            std::get<0>(m_MoveCostMap[point.x()][point.y()]) += damage;
                        }
                    }
                }
            }
        }
    }
}

void CoreAI::getBestTarget(Unit* pUnit, spGameAction pAction, UnitPathFindingSystem* pPfs, QVector<QVector3D>& ret, QVector<QVector3D>& moveTargetFields)
{
    pAction->setMovepath(QVector<QPoint>(1, QPoint(pUnit->Unit::getX(), pUnit->Unit::getY())), 0);
    getBestAttacksFromField(pUnit, pAction, ret, moveTargetFields);
    if (pUnit->canMoveAndFire(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY())))
    {
        spGameMap pMap = GameMap::getInstance();
        QVector<QPoint> targets = pPfs->getAllNodePoints();
        for (qint32 i2 = 0; i2 < targets.size(); i2++)
        {
            if (pMap->getTerrain(targets[i2].x(), targets[i2].y())->getUnit() == nullptr)
            {
                pAction->setMovepath(QVector<QPoint>(1, targets[i2]), 0);
                getBestAttacksFromField(pUnit, pAction, ret, moveTargetFields);
            }
        }
    }
}

void CoreAI::getBestAttacksFromField(Unit* pUnit, spGameAction pAction, QVector<QVector3D>& ret, QVector<QVector3D>& moveTargetFields)
{
    spGameMap pMap = GameMap::getInstance();
    // much easier case
    // check if the action can be performed here
    if (pAction->canBePerformed())
    {
        spMarkedFieldData pMarkedFieldData = pAction->getMarkedFieldStepData();
        for (qint32 i = 0; i < pMarkedFieldData->getPoints()->size(); i++)
        {
            QPoint target = pMarkedFieldData->getPoints()->at(i);
            QRectF damage = calcUnitDamage(pAction, target);
            Terrain* pTerrain = pMap->getTerrain(target.x(), target.y());
            Unit* pDef = pTerrain->getUnit();
            if (pDef != nullptr)
            {
                QPointF dmg = calcFundsDamage(damage, pUnit, pDef);
                if (ret.size() == 0)
                {
                    ret.append(QVector3D(target.x(), target.y(), dmg.y()));
                    QPoint point = pAction->getActionTarget();
                    moveTargetFields.append(QVector3D(point.x(), point.y(), 1));
                }
                else if (static_cast<float>(ret[0].z()) == dmg.y())
                {
                    ret.append(QVector3D(target.x(), target.y(), dmg.y()));
                    QPoint point = pAction->getActionTarget();
                    moveTargetFields.append(QVector3D(point.x(), point.y(), 1));
                }
                else if (dmg.y() > ret[0].z())
                {
                    ret.clear();
                    moveTargetFields.clear();
                    ret.append(QVector3D(target.x(), target.y(), dmg.y()));
                    QPoint point = pAction->getActionTarget();
                    moveTargetFields.append(QVector3D(point.x(), point.y(), 1));
                }
            }
            else
            {
                if (isAttackOnTerrainAllowed(pTerrain))
                {
                    if (ret.size() == 0)
                    {
                        ret.append(QVector3D(target.x(), target.y(), static_cast<float>(damage.x()) * m_buildingValue));
                        QPoint point = pAction->getActionTarget();
                        moveTargetFields.append(QVector3D(point.x(), point.y(), 1));
                    }
                    else if (ret[0].z() == static_cast<float>(damage.x()) * m_buildingValue)
                    {
                        ret.append(QVector3D(target.x(), target.y(), static_cast<float>(damage.x()) * m_buildingValue));
                        QPoint point = pAction->getActionTarget();
                        moveTargetFields.append(QVector3D(point.x(), point.y(), 1));
                    }
                    else if (static_cast<float>(damage.x()) > ret[0].z())
                    {
                        ret.clear();
                        moveTargetFields.clear();
                        ret.append(QVector3D(target.x(), target.y(), static_cast<float>(damage.x()) * m_buildingValue));
                        QPoint point = pAction->getActionTarget();
                        moveTargetFields.append(QVector3D(point.x(), point.y(), 1));
                    }
                }
            }
        }
    }
}

void CoreAI::getAttackTargets(Unit* pUnit, spGameAction pAction, UnitPathFindingSystem* pPfs, QVector<QVector4D>& ret, QVector<QVector3D>& moveTargetFields)
{
    pAction->setMovepath(QVector<QPoint>(1, QPoint(pUnit->Unit::getX(), pUnit->Unit::getY())), 0);
    getAttacksFromField(pUnit, pAction, ret, moveTargetFields);
    if (pUnit->canMoveAndFire(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY())))
    {
        spGameMap pMap = GameMap::getInstance();
        QVector<QPoint> targets = pPfs->getAllNodePoints();
        for (qint32 i2 = 0; i2 < targets.size(); i2++)
        {
            if (pMap->getTerrain(targets[i2].x(), targets[i2].y())->getUnit() == nullptr)
            {
                pAction->setMovepath(QVector<QPoint>(1, targets[i2]), 0);
                getAttacksFromField(pUnit, pAction, ret, moveTargetFields);
            }
        }
    }
}

void CoreAI::getAttacksFromField(Unit* pUnit, spGameAction pAction, QVector<QVector4D>& ret, QVector<QVector3D>& moveTargetFields)
{
    spGameMap pMap = GameMap::getInstance();
    // much easier case
    // check if the action can be performed here
    if (pAction->canBePerformed())
    {
        spMarkedFieldData pMarkedFieldData = pAction->getMarkedFieldStepData();
        for (qint32 i = 0; i < pMarkedFieldData->getPoints()->size(); i++)
        {
            QPoint target = pMarkedFieldData->getPoints()->at(i);
            QRectF damage = calcUnitDamage(pAction, target);
            Terrain* pTerrain = pMap->getTerrain(target.x(), target.y());
            Unit* pDef = pTerrain->getUnit();
            if (pDef != nullptr)
            {
                qint32 stealthMalus = 0;
                bool terrainHide = false;
                if (pDef->isStatusStealthedAndInvisible(m_pPlayer, terrainHide))
                {
                    stealthMalus = 4;
                    if (terrainHide)
                    {
                        stealthMalus /= 2;
                    }
                }
                QPointF dmg = calcFundsDamage(damage, pUnit, pDef);
                ret.append(QVector4D(target.x(), target.y(), dmg.y(), dmg.x()));
                QPoint point = pAction->getActionTarget();
                moveTargetFields.append(QVector3D(point.x(), point.y(), 1 + stealthMalus));
            }
            else
            {
                if (isAttackOnTerrainAllowed(pTerrain))
                {
                    ret.append(QVector4D(target.x(), target.y(), static_cast<float>(damage.x()) * m_buildingValue, damage.x()));
                    QPoint point = pAction->getActionTarget();
                    moveTargetFields.append(QVector3D(point.x(), point.y(), 1));
                }
            }
        }
    }
}

bool CoreAI::isAttackOnTerrainAllowed(Terrain* pTerrain)
{
    Building* pBuilding = pTerrain->getBuilding();
    if ((m_enableNeutralTerrainAttack && pTerrain->getHp() > 0) ||
        (pBuilding != nullptr && pBuilding->getHp() > 0 && pBuilding->getOwner() != m_pPlayer) ||
        (m_enableNeutralTerrainAttack && pBuilding != nullptr && pBuilding->getHp() > 0 && pBuilding->getOwner() == nullptr))
    {
        return true;
    }
    return false;
}

QPointF CoreAI::calcFundsDamage(QRectF damage, Unit* pAtk, Unit* pDef)
{
    float atkDamage = static_cast<float>(damage.x()) / Unit::MAX_UNIT_HP;
    if (atkDamage > pDef->getHp())
    {
        atkDamage = pDef->getHp();
    }
    float fundsDamage = pDef->getUnitCosts() * atkDamage / Unit::MAX_UNIT_HP;
    if (damage.width() >= 0.0)
    {
        atkDamage = static_cast<float>(damage.width()) / Unit::MAX_UNIT_HP;
        if (atkDamage > pAtk->getHp())
        {
            atkDamage = pAtk->getHp();
        }
        fundsDamage -= pAtk->getUnitCosts() * atkDamage / Unit::MAX_UNIT_HP * m_ownUnitValue;
    }
    return QPointF(atkDamage, fundsDamage);
}

QRectF CoreAI::calcUnitDamage(spGameAction pAction, QPoint target)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "calcBattleDamage";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(pAction.get());
    args1 << obj1;
    args1 << target.x();
    args1 << target.y();
    args1 << static_cast<qint32>(GameEnums::LuckDamageMode_Average);
    QJSValue erg = pInterpreter->doFunction(ACTION_FIRE, function1, args1);
    return erg.toVariant().toRectF();
}

QRectF CoreAI::calcVirtuelUnitDamage(Unit* pAttacker, float attackerTakenDamage, QPoint atkPos,
                                     Unit* pDefender, float defenderTakenDamage, QPoint defPos,
                                     bool ignoreOutOfVisionRange)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "calcBattleDamage3";
    QJSValueList args1;
    QJSValue obj3 = pInterpreter->newQObject(nullptr);
    args1 << obj3;
    QJSValue obj1 = pInterpreter->newQObject(pAttacker);
    args1 << obj1;
    args1 << attackerTakenDamage;
    args1 << atkPos.x();
    args1 << atkPos.y();
    QJSValue obj2 = pInterpreter->newQObject(pDefender);
    args1 << obj2;
    args1 << defPos.x();
    args1 << defPos.y();
    args1 << defenderTakenDamage;
    args1 << static_cast<qint32>(GameEnums::LuckDamageMode_Average);
    args1 << ignoreOutOfVisionRange;
    QJSValue erg = pInterpreter->doFunction(ACTION_FIRE, function1, args1);
    return erg.toVariant().toRectF();
}


bool CoreAI::moveAwayFromProduction(spQmlVectorUnit pUnits)
{
    Console::print("CoreAI::moveAwayFromProduction", Console::eDEBUG);
    aiStep = AISteps::moveAway;
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit and does it block a production center cause it has nothing to do this turn?
        if (!pUnit->getHasMoved() &&
            pUnit->getTerrain()->getBuilding() != nullptr &&
            !m_pPlayer->isEnemy(pUnit->getTerrain()->getBuilding()->getOwner()) &&
            pUnit->getTerrain()->getBuilding()->isProductionBuilding())
        {
            UnitPathFindingSystem turnPfs(pUnit);
            turnPfs.explore();
            QVector<QPoint> points = turnPfs.getAllNodePoints();
            QPoint target(-1 , -1);
            for (qint32 i = 0; i < points.size(); i++)
            {
                Terrain* pTerrain = pMap->getTerrain(points[i].x(), points[i].y());
                if (pTerrain->getUnit() == nullptr)
                {
                    if (pTerrain->getBuilding() == nullptr)
                    {
                        target = points[i];
                        break;
                    }
                    else if (!pTerrain->getBuilding()->isProductionBuilding())
                    {
                        target = points[i];
                        break;
                    }
                }
            }
            if (target.x() >= 0 && target.y() >= 0)
            {
                spGameAction pAction = spGameAction::create(ACTION_WAIT);
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                QVector<QPoint> path = turnPfs.getPath(target.x(), target.y());
                pAction->setMovepath(path, turnPfs.getCosts(path));
                emit performAction(pAction);
                return true;
            }
        }
    }
    return false;
}


void CoreAI::getTrainingData(QString file, QVector<QVector<float>>& trainingData, QVector<QVector<spDecisionQuestion>>& questions)
{
    QFile trainingFile(file);
    trainingFile.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QTextStream stream(&trainingFile);
    bool questionsFound = false;
    QStringList types;
    QVector<spDecisionQuestion> readQuestions;

    readTrainingFile(stream, questionsFound, types, readQuestions, trainingData, questions);

    QStringList mods = Settings::getMods();
    for (qint32 i = 0; i < mods.size(); i++)
    {
        QString modFilename = file;
        QFile modFile(modFilename.replace("resources/", mods[i] + "/"));
        if (modFile.exists())
        {
            modFile.open(QIODevice::ReadOnly | QIODevice::Truncate);
            QTextStream modStream(&modFile);
            readTrainingFile(modStream, questionsFound, types, readQuestions, trainingData, questions);
        }
    }
}

void CoreAI::readTrainingFile(QTextStream& stream, bool& questionsFound, QStringList& types,
                              QVector<spDecisionQuestion>& readQuestions,
                              QVector<QVector<float>>& trainingData, QVector<QVector<spDecisionQuestion>>& questions)
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();

    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        line = line.simplified();
        if (line.startsWith("//"))
        {
            // skip comments
        }
        else
        {
            if (line.startsWith("type") && !questionsFound )
            {
                questionsFound = true;
                QStringList items = line.split(" ");
                for (qint32 i = 1; i < items.size(); i++)
                {
                    readQuestions.append(spDecisionQuestion::create());
                    qint32 index = types.size();
                    QString typeLine = items[i];
                    if (typeLine.startsWith("NUMBER:"))
                    {
                        if (typeLine.startsWith("NUMBERFILE:"))
                        {
                            QFile numberFile(typeLine.split(":")[1]);
                            numberFile.open(QIODevice::ReadOnly | QIODevice::Truncate);
                            QTextStream stream(&numberFile);
                            typeLine = stream.readLine();
                        }
                        QStringList questionString = typeLine.split(":")[1].split("|");
                        for (qint32 i2 = 0; i2 < questionString.size(); i2++)
                        {
                            QStringList questionData = questionString[i2].split("_");
                            if (questionData.size() == 2)
                            {
                                float value = questionData[0].toFloat();
                                if (questionData[1] == "<")
                                {
                                    readQuestions[i - 1]->appendQuestion(spQuestion::create(value, index, GameEnums::AIQuestionType_Greater));
                                }
                                else if (questionData[1] == ">")
                                {
                                    readQuestions[i - 1]->appendQuestion(spQuestion::create(value, index, GameEnums::AIQuestionType_Smaler));
                                }
                                else if (questionData[1] == "=")
                                {
                                    readQuestions[i - 1]->appendQuestion(spQuestion::create(value, index, GameEnums::AIQuestionType_Equal));
                                }
                            }
                            else if (questionData.size() == 3)
                            {
                                float value1 = questionData[0].toFloat();
                                float value2 = questionData[2].toFloat();
                                readQuestions[i - 1]->appendQuestion(spQuestion::create(value1, value2, index, GameEnums::AIQuestionType_Between));
                            }
                        }
                        types.append("NUMBER");
                    }
                    else
                    {
                        if (items[i] == "CO")
                        {
                            for (qint32 i2 = 0; i2 < pCOSpriteManager->getCount(); i2++)
                            {
                                readQuestions[i - 1]->appendQuestion(spQuestion::create(i2, index, GameEnums::AIQuestionType_Equal));
                            }
                            readQuestions[i - 1]->appendQuestion(spQuestion::create(-1, index, GameEnums::AIQuestionType_Equal));
                        }
                        else if (items[i] == "BUILDING")
                        {
                            for (qint32 i2 = 0; i2 < pBuildingSpriteManager->getCount(); i2++)
                            {
                                readQuestions[i - 1]->appendQuestion(spQuestion::create(i2, index, GameEnums::AIQuestionType_Equal));
                            }
                            readQuestions[i - 1]->appendQuestion(spQuestion::create(-1, index, GameEnums::AIQuestionType_Equal));
                        }
                        else if (items[i] == "UNIT")
                        {
                            for (qint32 i2 = 0; i2 < pUnitSpriteManager->getCount(); i2++)
                            {
                                readQuestions[i - 1]->appendQuestion(spQuestion::create(i2, index, GameEnums::AIQuestionType_Equal));
                            }
                            readQuestions[i - 1]->appendQuestion(spQuestion::create(-1, index, GameEnums::AIQuestionType_Equal));
                        }
                        types.append(items[i]);
                    }
                }
            }
            else if (questionsFound)
            {
                QStringList items = line.split(" ");
                // check for identic match here
                if (items.size() == types.size())
                {
                    trainingData.append(QVector<float>());
                    questions.append(QVector<spDecisionQuestion>());
                    qint32 item = trainingData.size() - 1;
                    for (qint32 i = 0; i < types.size(); i++)
                    {
                        // convert all data to numbers
                        if (types[i] == "CO")
                        {
                            qint32 index = pCOSpriteManager->getIndex(items[i]);
                            trainingData[item].append(index);
                            if (i < types.size() - 1)
                            {
                                questions[item].append(readQuestions[i]);
                            }
                        }
                        else if (types[i] == "BUILDING")
                        {
                            qint32 index = pBuildingSpriteManager->getIndex(items[i]);
                            trainingData[item].append(index);
                            if (i < types.size() - 1)
                            {
                                questions[item].append(readQuestions[i]);
                            }
                        }
                        else if (types[i] == "UNIT")
                        {
                            qint32 index = pUnitSpriteManager->getIndex(items[i]);
                            trainingData[item].append(index);
                            if (i < types.size() - 1)
                            {
                                questions[item].append(readQuestions[i]);
                            }
                        }
                        else if (types[i] == "NUMBER")
                        {
                            float value = items[i].toFloat();
                            trainingData[item].append(value);
                            if (i < types.size() - 1)
                            {
                                questions[item].append(readQuestions[i]);
                            }
                        }
                    }
                }
            }
        }
    }
}

void CoreAI::addMenuItemData(spGameAction pGameAction, QString itemID, qint32 cost)
{
    Console::print("CoreAI::addMenuItemData()", Console::eDEBUG);
    pGameAction->writeDataString(itemID);
    // increase costs and input step
    pGameAction->setCosts(pGameAction->getCosts() + cost);
    pGameAction->setInputStep(pGameAction->getInputStep() + 1);
}

void CoreAI::addSelectedFieldData(spGameAction pGameAction, QPoint point)
{
    Console::print("CoreAI::addSelectedFieldData()", Console::eDEBUG);
    pGameAction->writeDataInt32(point.x());
    pGameAction->writeDataInt32(point.y());
    pGameAction->setInputStep(pGameAction->getInputStep() + 1);
}


QVector<Unit*> CoreAI::appendLoadingTargets(Unit* pUnit, spQmlVectorUnit pUnits,
                                            spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings,
                                            bool ignoreCaptureTargets, bool virtualLoading, QVector<QVector3D>& targets,
                                            bool all)
{
    qint32 unitIslandIdx = getIslandIndex(pUnit);
    qint32 unitIsland = getIsland(pUnit);
    spGameMap pMap = GameMap::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
    QVector<Unit*> transportUnits;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pLoadingUnit = pUnits->at(i);
        if (pLoadingUnit != pUnit)
        {
            // can we transport it?
            if (pUnit->canTransportUnit(pLoadingUnit, virtualLoading))
            {
                bool found = false;
                bool canCapture = pLoadingUnit->getActionList().contains(ACTION_CAPTURE);
                qint32 loadingIslandIdx = getIslandIndex(pLoadingUnit);
                qint32 loadingIsland = getIsland(pLoadingUnit);
                QPoint loadingUnitPos = pLoadingUnit->getPosition();
                if (ignoreCaptureTargets && canCapture)
                {
                    // no targets found -> try to speed up those infis
                }
                else
                {
                    found = hasTargets(pLoadingUnit, canCapture, pEnemyUnits, pEnemyBuildings,
                                       loadingIslandIdx, loadingIsland);
                }
                if (!found)
                {
                    // no targets for this unit :(
                    found = false;
                    qint32 distance = std::numeric_limits<qint32>::max();
                    qint32 targetX;
                    qint32 targetY;
                    for (qint32 x = 0; x < width; x++)
                    {
                        for (qint32 y = 0; y < heigth; y++)
                        {
                            // can be reached by both units and is empty
                            // and not added yet
                            if ((m_IslandMaps[loadingIslandIdx]->getIsland(x, y) == loadingIsland) &&
                                (m_IslandMaps[unitIslandIdx]->getIsland(x, y) == unitIsland) &&
                                ((pMap->getTerrain(x, y)->getUnit() == nullptr) ||
                                 (pMap->getTerrain(x, y)->getUnit() == pUnit)))
                            {
                                qint32 dist = GlobalUtils::getDistance(loadingUnitPos, QPoint(x, y));
                                if (dist < distance)
                                {
                                    found = true;
                                    distance = dist;
                                    targetX = x;
                                    targetY = y;
                                    if (all)
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                        if (all && found)
                        {
                            break;
                        }
                    }
                    if (found && (virtualLoading || !targets.contains(QVector3D(targetX, targetY, 1))))
                    {
                        targets.append(QVector3D(targetX, targetY, 1));
                        transportUnits.append(pLoadingUnit);
                        if (!all)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
    return transportUnits;
}

bool CoreAI::hasTargets(Unit* pLoadingUnit, bool canCapture, spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings,
                        qint32 loadingIslandIdx, qint32 loadingIsland)
{
    bool found = false;
    // check if we have anything to do here :)
    for (qint32 i2 = 0; i2 < pEnemyUnits->size(); i2++)
    {
        Unit* pEnemy = pEnemyUnits->at(i2);
        if (m_IslandMaps[loadingIslandIdx]->getIsland(pEnemy->Unit::getX(), pEnemy->Unit::getY()) == loadingIsland &&
            pLoadingUnit->isAttackable(pEnemy, true))
        {
            // this unit can do stuff skip it
            found = true;
            break;
        }
    }
    if (!found)
    {
        // check for capturing or missiles next
        if (canCapture)
        {
            bool missileTarget = hasMissileTarget();
            for (qint32 i2 = 0; i2 < pEnemyBuildings->size(); i2++)
            {
                Building* pBuilding = pEnemyBuildings->at(i2);
                if (m_IslandMaps[loadingIslandIdx]->getIsland(pBuilding->Building::getX(), pBuilding->Building::getY()) == loadingIsland &&
                    pBuilding->isCaptureOrMissileBuilding(missileTarget))
                {
                    // this unit can do stuff skip it
                    found = true;
                    break;
                }
            }
        }
    }
    return found;
}

void CoreAI::appendSupportTargets(QStringList actions, Unit* pCurrentUnit, spQmlVectorUnit pUnits, spQmlVectorUnit pEnemyUnits, QVector<QVector3D>& targets)
{
    spQmlVectorPoint unitFields = GlobalUtils::getCircle(1, 1);
    spGameMap pMap = GameMap::getInstance();
    for (const auto& action : actions)
    {
        if (action.startsWith(ACTION_SUPPORTSINGLE) ||
            action.startsWith(ACTION_SUPPORTALL))
        {
            for (qint32 i = 0; i < pUnits->size(); i++)
            {
                Unit* pUnit = pUnits->at(i);
                if (pUnit != pCurrentUnit)
                {
                    for (qint32 i2 = 0; i2 < unitFields->size(); i2++)
                    {
                        if (pMap->onMap(pUnit->Unit::getX() + unitFields->at(i2).x(), pUnit->Unit::getY() + unitFields->at(i2).y()) &&
                            pMap->getTerrain(pUnit->Unit::getX() + unitFields->at(i2).x(), pUnit->Unit::getY() + unitFields->at(i2).y())->getUnit() == nullptr)
                        {
                            QVector3D point = QVector3D(pUnit->Unit::getX() + unitFields->at(i2).x(), pUnit->Unit::getY() + unitFields->at(i2).y(), 2);
                            if (!targets.contains(point) )
                            {
                                targets.append(point);
                            }
                        }
                    }
                }
            }
        }
        else if (action.startsWith(ACTION_PLACE))
        {
            for (qint32 i = 0; i < pEnemyUnits->size(); i++)
            {
                Unit* pUnit = pEnemyUnits->at(i);
                for (qint32 i2 = 0; i2 < unitFields->size(); i2++)
                {
                    if (pMap->onMap(pUnit->Unit::getX() + unitFields->at(i2).x(), pUnit->Unit::getY() + unitFields->at(i2).y()) &&
                        pMap->getTerrain(pUnit->Unit::getX() + unitFields->at(i2).x(), pUnit->Unit::getY() + unitFields->at(i2).y())->getUnit() == nullptr)
                    {
                        QVector3D point = QVector3D(pUnit->Unit::getX() + unitFields->at(i2).x(), pUnit->Unit::getY() + unitFields->at(i2).y(), 2);
                        if (!targets.contains(point) )
                        {
                            targets.append(point);
                        }
                    }
                }
            }
        }
    }
}

void CoreAI::appendCaptureTargets(QStringList actions, Unit* pUnit, spQmlVectorBuilding pEnemyBuildings, QVector<QVector3D>& targets)
{
    if (actions.contains(ACTION_CAPTURE) ||
        actions.contains(ACTION_MISSILE))
    {
        bool missileTarget = hasMissileTarget();
        for (qint32 i2 = 0; i2 < pEnemyBuildings->size(); i2++)
        {
            Building* pBuilding = pEnemyBuildings->at(i2);
            QPoint point(pBuilding->Building::getX(), pBuilding->Building::getY());
            if (pUnit->canMoveOver(pBuilding->Building::getX(), pBuilding->Building::getY()))
            {
                if (pBuilding->isCaptureOrMissileBuilding(missileTarget) &&
                    pBuilding->getTerrain()->getUnit() == nullptr)
                {
                    targets.append(QVector3D(pBuilding->Building::getX(), pBuilding->Building::getY(), 1));
                }
            }
        }
    }
}

void CoreAI::appendAttackTargets(Unit* pUnit, spQmlVectorUnit pEnemyUnits, QVector<QVector3D>& targets)
{
    spGameMap pMap = GameMap::getInstance();
    qint32 firerange = pUnit->getMaxRange(pUnit->getPosition());
    spQmlVectorPoint pTargetFields = GlobalUtils::getCircle(firerange, firerange);
    for (qint32 i2 = 0; i2 < pEnemyUnits->size(); i2++)
    {
        Unit* pEnemy = pEnemyUnits->at(i2);
        if (pUnit->isAttackable(pEnemy, true))
        {
            for (qint32 i3 = 0; i3 < pTargetFields->size(); i3++)
            {
                qint32 x = pTargetFields->at(i3).x() + pEnemy->Unit::getX();
                qint32 y = pTargetFields->at(i3).y() + pEnemy->Unit::getY();
                if (pMap->onMap(x, y) &&
                    pMap->getTerrain(x, y)->getUnit() == nullptr)
                {
                    if (pUnit->canMoveOver(x, y))
                    {
                        qint32 stealthMalus = 0;                        
                        bool terrainHide = false;
                        if (pEnemy->isStatusStealthedAndInvisible(m_pPlayer, terrainHide))
                        {
                            stealthMalus = 4;
                            if (terrainHide)
                            {
                                stealthMalus /= 2;
                            }
                        }
                        QVector3D possibleTarget(x, y, 1 + stealthMalus);
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

void CoreAI::appendAttackTargetsIgnoreOwnUnits(Unit* pUnit, spQmlVectorUnit pEnemyUnits, QVector<QVector3D>& targets)
{
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i2 = 0; i2 < pEnemyUnits->size(); i2++)
    {
        Unit* pEnemy = pEnemyUnits->at(i2);
        if (pUnit->isAttackable(pEnemy, true))
        {
            qint32 firerange = pUnit->getMaxRange(pUnit->getPosition());
            spQmlVectorPoint pTargetFields = GlobalUtils::getCircle(firerange, firerange);
            for (qint32 i3 = 0; i3 < pTargetFields->size(); i3++)
            {
                qint32 x = pTargetFields->at(i3).x() + pEnemy->Unit::getX();
                qint32 y = pTargetFields->at(i3).y() + pEnemy->Unit::getY();
                if (pMap->onMap(x, y))
                {
                    Unit* pTargetUnit = pMap->getTerrain(x, y)->getUnit();
                    if (pUnit->canMoveOver(x, y) &&
                        pTargetUnit != nullptr &&
                        pTargetUnit->getOwner()->checkAlliance(m_pPlayer) == GameEnums::Alliance_Friend)
                    {
                        qint32 stealthMalus = 0;
                        bool terrainHide = false;
                        if (pEnemy->isStatusStealthedAndInvisible(m_pPlayer, terrainHide))
                        {
                            stealthMalus = 6;
                            if (terrainHide)
                            {
                                stealthMalus /= 2;
                            }
                        }
                        QVector3D possibleTarget(x, y, 4 + stealthMalus);
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


void CoreAI::appendRepairTargets(Unit* pUnit, spQmlVectorBuilding pBuildings, QVector<QVector3D>& targets)
{
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i2 = 0; i2 < pBuildings->size(); i2++)
    {
        Building* pBuilding = pBuildings->at(i2);
        QPoint point(pBuilding->Building::getX(), pBuilding->Building::getY());
        if (pMap->getTerrain(point.x(), point.y())->getUnit() == nullptr &&
            pBuilding->canRepair(pUnit))
        {
            targets.append(QVector3D(pBuilding->Building::getX(), pBuilding->Building::getY(), 1));
        }
    }
}

void CoreAI::appendSupplyTargets(Unit* pUnit, spQmlVectorUnit pUnits, QVector<QVector3D>& targets)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pSupplyUnit = pUnits->at(i);
        if (pSupplyUnit != pUnit)
        {
            if ((pSupplyUnit->getMaxAmmo1() > 0 && pSupplyUnit->hasAmmo1() < pSupplyUnit->getMaxAmmo1()) ||
                (pSupplyUnit->getMaxAmmo2() > 0 && pSupplyUnit->hasAmmo2() < pSupplyUnit->getMaxAmmo2()) ||
                (pSupplyUnit->getMaxFuel() > 0 && static_cast<float>(pSupplyUnit->getFuel()) / static_cast<float>(pSupplyUnit->getMaxFuel()) < 0.5f))
            {
                targets.append(QVector3D(pSupplyUnit->Unit::getX(), pSupplyUnit->Unit::getY(), 1));
            }
        }
    }
}

void CoreAI::appendTransporterTargets(Unit* pUnit, spQmlVectorUnit pUnits, QVector<QVector3D>& targets)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pTransporterUnit = pUnits->at(i);
        if (pTransporterUnit != pUnit)
        {
            if (pTransporterUnit->canTransportUnit(pUnit))
            {
                targets.append(QVector3D(pTransporterUnit->Unit::getX(), pTransporterUnit->Unit::getY(), 1));
            }
        }
    }
}

void CoreAI::appendCaptureTransporterTargets(Unit* pUnit, spQmlVectorUnit pUnits, spQmlVectorBuilding pEnemyBuildings, QVector<QVector3D>& targets)
{
    qint32 unitIslandIdx = getIslandIndex(pUnit);
    qint32 unitIsland = getIsland(pUnit);
    spGameMap pMap = GameMap::getInstance();
    bool missileTarget = hasMissileTarget();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pTransporterUnit = pUnits->at(i);
        if (pTransporterUnit != pUnit)
        {
            // assuming unit transporter only have space for one unit
            if (pTransporterUnit->canTransportUnit(pUnit) &&
                pTransporterUnit->getLoadingPlace() == 1)
            {
                bool goodTransporter = false;
                // check captures on this island
                qint32 transporterIslandIdx = getIslandIndex(pTransporterUnit);
                qint32 transporterIsland = getIsland(pTransporterUnit);
                for (qint32 i2 = 0; i2 < pEnemyBuildings->size(); i2++)
                {
                    qint32 x = pEnemyBuildings->at(i2)->Building::getX();
                    qint32 y = pEnemyBuildings->at(i2)->Building::getY();
                    // check if both can move there on this island
                    // so we avoid loading and unloading a unit recursivly cause we think we need to transport it to another island
                    // eventhough it has something to do here
                    if ((m_IslandMaps[unitIslandIdx]->getIsland(x, y) == unitIsland) &&
                        (m_IslandMaps[transporterIslandIdx]->getIsland(x, y) == transporterIsland) &&
                        (pMap->getTerrain(x, y)->getUnit() == nullptr) &&
                        (pEnemyBuildings->at(i2)->isCaptureOrMissileBuilding(missileTarget)))
                    {
                        goodTransporter = true;
                        break;
                    }
                }
                if (goodTransporter)
                {

                    targets.append(QVector3D(pTransporterUnit->Unit::getX(), pTransporterUnit->Unit::getY(), 1));
                }
            }
        }
    }
}

void CoreAI::appendNearestUnloadTargets(Unit* pUnit, spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings, QVector<QVector3D>& targets)
{
    QVector<QVector<qint32>> checkedIslands;
    QVector<qint32> loadedUnitIslandIdx;

    qint32 unitIslandIdx = getIslandIndex(pUnit);
    qint32 unitIsland = getIsland(pUnit);
    QVector<Unit*> captureUnits;
    for (qint32 i2 = 0; i2 < pUnit->getLoadedUnitCount(); i2++)
    {
        Unit* pLoadedUnit = pUnit->getLoadedUnit(i2);
        if (pLoadedUnit->getActionList().contains(ACTION_CAPTURE))
        {
            captureUnits.append(pLoadedUnit);
        }
        loadedUnitIslandIdx.append(getIslandIndex(pLoadedUnit));
        checkedIslands.append(QVector<qint32>());
    }
    spQmlVectorPoint pUnloadArea = GlobalUtils::getCircle(1, 1);
    // check for enemis
    for (qint32 i = 0; i < pEnemyUnits->size(); i++)
    {
        Unit* pEnemy = pEnemyUnits->at(i);
        for (qint32 i2 = 0; i2 < pUnit->getLoadedUnitCount(); i2++)
        {
            Unit* pLoadedUnit = pUnit->getLoadedUnit(i2);
            qint32 targetIsland = m_IslandMaps[loadedUnitIslandIdx[i2]]->getIsland(pEnemy->Unit::getX(), pEnemy->Unit::getY());
            // check if we could reach the enemy if we would be on his island
            // and we didn't checked this island yet -> improves the speed
            if (targetIsland >= 0 && !checkedIslands[i2].contains(targetIsland))
            {
                // could we beat his ass? -> i mean can we attack him
                // if so this is a great island
                if (pLoadedUnit->isAttackable(pEnemy, true))
                {
                    checkIslandForUnloading(pUnit, pLoadedUnit, checkedIslands[i2], unitIslandIdx, unitIsland,
                                            loadedUnitIslandIdx[i2], targetIsland, pUnloadArea.get(), targets);
                }
            }
        }
    }
    // check for capturable buildings
    if (captureUnits.size() > 0)
    {
        bool missileTarget = hasMissileTarget();
        for (qint32 i = 0; i < pEnemyBuildings->size(); i++)
        {
            Building* pEnemyBuilding = pEnemyBuildings->at(i);
            for (qint32 i2 = 0; i2 < captureUnits.size(); i2++)
            {
                Unit* pLoadedUnit = captureUnits[i2];
                qint32 targetIsland = m_IslandMaps[loadedUnitIslandIdx[i2]]->getIsland(pEnemyBuilding->Building::getX(), pEnemyBuilding->Building::getY());
                // check if we could reach the enemy if we would be on his island
                // and we didn't checked this island yet -> improves the speed
                if (targetIsland >= 0 && !checkedIslands[i2].contains(targetIsland))
                {
                    if (pEnemyBuilding->isCaptureOrMissileBuilding(missileTarget))
                    {
                        checkIslandForUnloading(pUnit, pLoadedUnit, checkedIslands[i2], unitIslandIdx, unitIsland,
                                                loadedUnitIslandIdx[i2], targetIsland, pUnloadArea.get(), targets);
                    }
                }
            }
        }
    }
}

bool CoreAI::isUnloadTerrain(Unit* pUnit, Terrain* pTerrain)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(pUnit);
    args << obj;
    QJSValue obj1 = pInterpreter->newQObject(pTerrain);
    args << obj1;
    QJSValue ret = pInterpreter->doFunction(ACTION_UNLOAD, "isUnloadTerrain", args);
    if (ret.isBool())
    {
        return ret.toBool();
    }
    return false;
}

void CoreAI::getBestFlareTarget(Unit* pUnit, spGameAction pAction, UnitPathFindingSystem* pPfs, QPoint& flareTarget, QPoint& moveTargetField)
{
    flareTarget  = QPoint(-1, -1);
    moveTargetField  = QPoint(-1, -1);
    spQmlVectorPoint pUnfogCircle = GlobalUtils::getCircle(0, m_flareInfo.unfogRange);
    spQmlVectorPoint pTargetCircle = GlobalUtils::getCircle(m_flareInfo.minRange, m_flareInfo.maxRange);
    pAction->setMovepath(QVector<QPoint>(1, QPoint(pUnit->Unit::getX(), pUnit->Unit::getY())), 0);
    spGameMap pMap = GameMap::getInstance();
    QVector<QPoint> targets = pPfs->getAllNodePoints();
    qint32 score = std::numeric_limits<qint32>::min();
    for (qint32 i = 0; i < targets.size(); i++)
    {
        QVector<QPoint> path = pPfs->getPath(targets[i].x(), targets[i].y());
        pAction->setMovepath(path, pPfs->getCosts(path));
        if (pAction->canBePerformed())
        {
            Unit* pFieldUnit = pMap->getTerrain(targets[i].x(), targets[i].y())->getUnit();
            if (pFieldUnit == nullptr ||
                pFieldUnit == pUnit)
            {
                for (qint32 i2 = 0; i2 < pTargetCircle->size(); ++i2)
                {
                    QPoint target = pTargetCircle->at(i2) + targets[i];
                    if (pMap->onMap(target.x(), target.y()))
                    {
                        qint32 currentScore = getFlareTargetScore(targets[i], target, pUnfogCircle);
                        if (currentScore > 0)
                        {
                            if (score < currentScore)
                            {
                                score = currentScore;
                                flareTarget  = target;
                                moveTargetField  = targets[i];
                            }
                            else if (score == currentScore && GlobalUtils::randInt(0, 10) > 5)
                            {
                                flareTarget  = target;
                                moveTargetField  = targets[i];
                            }
                        }
                    }
                }
            }
        }
    }
    if (score > 0)
    {
        Console::print("Found flare target with score: " + QString::number(score), Console::eDEBUG);
    }
}

qint32 CoreAI::getFlareTargetScore(const QPoint& moveTarget, const QPoint& flareTarget, const spQmlVectorPoint& pUnfogCircle)
{
    spGameMap pMap = GameMap::getInstance();
    qint32 score = 0;
    for (qint32 i = 0; i < pUnfogCircle->size(); ++i)
    {
        QPoint target = pUnfogCircle->at(i) + flareTarget;
        if (pMap->onMap(target.x(), target.y()))
        {
            Unit* pFieldUnit = pMap->getTerrain(target.x(), target.y())->getUnit();
            if (pFieldUnit != nullptr && m_pPlayer->isEnemyUnit(pFieldUnit))
            {
                if (pFieldUnit->isStealthed(m_pPlayer))
                {
                    // unstealthing stealthed units is more important.
                    // than unfogging plain fields
                    if (pFieldUnit->isStatusStealthed() ||
                        pFieldUnit->hasTerrainHide(m_pPlayer))
                    {
                        score += 2;
                    }
                    else
                    {
                        score++;
                    }
                }
            }
        }
    }
    if (score > 0)
    {
        // the farther away from the flare the better it is usually
        score += GlobalUtils::getDistance(moveTarget, flareTarget) * pUnfogCircle->size() * 3;
    }
    else
    {
        score = std::numeric_limits<qint32>::min();
    }
    return score;
}

void CoreAI::checkIslandForUnloading(Unit* pUnit, Unit* pLoadedUnit, QVector<qint32>& checkedIslands,
                                     qint32 unitIslandIdx, qint32 unitIsland,
                                     qint32 loadedUnitIslandIdx, qint32 targetIsland,
                                     QmlVectorPoint* pUnloadArea, QVector<QVector3D>& targets)
{
    spGameMap pMap = GameMap::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
    checkedIslands.append(loadedUnitIslandIdx);
    for (qint32 x = 0; x < width; x++)
    {
        for (qint32 y = 0; y < heigth; y++)
        {
            // check if this is the same island as we search for
            // check if it's the same island our transporter is on if so we can reach the field
            // the unloading area is also free
            if (m_IslandMaps[loadedUnitIslandIdx]->getIsland(x, y) == targetIsland &&
                m_IslandMaps[unitIslandIdx]->getIsland(x, y) == unitIsland &&
                (pMap->getTerrain(x, y)->getUnit() == nullptr ||
                 pMap->getTerrain(x, y)->getUnit() == pUnit))
            {
                if (isUnloadTerrain(pUnit, pMap->getTerrain(x, y)))
                {
                    // and on top of that we have same free fields to unload the unit
                    for (qint32 i3 = 0; i3 < pUnloadArea->size(); i3++)
                    {
                        qint32 unloadX = x + pUnloadArea->at(i3).x();
                        qint32 unloadY = y + pUnloadArea->at(i3).y();
                        if (pMap->onMap(unloadX, unloadY) &&
                            pMap->getTerrain(unloadX, unloadY)->getUnit() == nullptr &&
                            pLoadedUnit->getBaseMovementCosts(unloadX, unloadY, unloadX, unloadY) > 0 &&
                            !targets.contains(QVector3D(x, y, 1)) &&
                            pUnit->getBaseMovementCosts(x, y, x, y) > 0)
                        {
                            targets.append(QVector3D(x, y, 1));
                            break;
                        }
                    }
                }
            }
        }
    }
}

void CoreAI::appendUnloadTargetsForCapturing(Unit* pUnit, spQmlVectorBuilding pEnemyBuildings, QVector<QVector3D>& targets)
{
    spGameMap pMap = GameMap::getInstance();

    qint32 unitIslandIdx = getIslandIndex(pUnit);
    qint32 unitIsland = getIsland(pUnit);

    QVector<Unit*> capturUnits;
    for (qint32 i2 = 0; i2 < pUnit->getLoadedUnitCount(); i2++)
    {
        Unit* pLoadedUnit = pUnit->getLoadedUnit(i2);
        if (pLoadedUnit->getActionList().contains(ACTION_CAPTURE))
        {
            capturUnits.append(pLoadedUnit);
        }
    }
    if (capturUnits.size() > 0)
    {
        spQmlVectorPoint pUnloadArea = GlobalUtils::getCircle(1, 1);
        GameAction testAction;
        testAction.setTargetUnit(capturUnits[0]);
        // store has moved
        bool hasMoved = capturUnits[0]->getHasMoved();
        // simulate a not moved unit for checking if we can capture the building or fire a missile from it.
        capturUnits[0]->setHasMoved(false);
        bool missileTarget = hasMissileTarget();
        for (qint32 i = 0; i < pEnemyBuildings->size(); i++)
        {
            Building* pBuilding = pEnemyBuildings->at(i);
            QPoint point(pBuilding->Building::getX(), pBuilding->Building::getY());
            if (capturUnits[0]->canMoveOver(pBuilding->Building::getX(), pBuilding->Building::getY()))
            {
                // we can capture it :)
                if (pBuilding->isCaptureOrMissileBuilding(missileTarget) &&
                    pBuilding->getTerrain()->getUnit() == nullptr)
                {
                    // check unload fields
                    for (qint32 i2 = 0; i2 < pUnloadArea->size(); i2++)
                    {
                        qint32 x = point.x() + pUnloadArea->at(i2).x();
                        qint32 y = point.y() + pUnloadArea->at(i2).y();
                        if (pMap->onMap(x, y) &&
                            pMap->getTerrain(x, y)->getUnit() == nullptr &&
                            !targets.contains(QVector3D(x, y, 1)))
                        {
                            if (isUnloadTerrain(pUnit, pMap->getTerrain(x, y)))
                            {
                                // we can reach this unload field?
                                if (m_IslandMaps[unitIslandIdx]->getIsland(x, y) == unitIsland)
                                {
                                    for (qint32 i3 = 0; i3 < capturUnits.size(); i3++)
                                    {
                                        if (capturUnits[i3]->canMoveOver(x, y))
                                        {
                                            targets.append(QVector3D(x, y, 1));
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        capturUnits[0]->setHasMoved(hasMoved);
    }
}

void CoreAI::appendTerrainBuildingAttackTargets(Unit* pUnit, spQmlVectorBuilding pEnemyBuildings, QVector<QVector3D>& targets)
{
    spGameMap pMap = GameMap::getInstance();
    qint32 firerange = pUnit->getMaxRange(pUnit->getPosition());
    spQmlVectorPoint pTargetFields = GlobalUtils::getCircle(firerange, firerange);
    for (qint32 i = 0; i < pEnemyBuildings->size(); i++)
    {
        Building* pBuilding = pEnemyBuildings->at(i);
        if (pBuilding->getHp() > 0 &&
            pUnit->isEnvironmentAttackable(pBuilding->getBuildingID()))
        {
            qint32 width = pBuilding->getBuildingWidth();
            qint32 heigth = pBuilding->getBuildingHeigth();
            QPoint pos = pBuilding->getPosition();
            QVector<QPoint> attackPosition;
            // find all attackable fields
            for (qint32 x = -width; x <= 0; x++)
            {
                for (qint32 y = -heigth; y <= 0; y++)
                {
                    if (pBuilding->getIsAttackable(x + pos.x(), y + pos.y()))
                    {
                        attackPosition.append(QPoint(pos.x() + x, pos.y() + y));
                    }
                }
            }
            // find attackable fields
            for (qint32 i3 = 0; i3 < pTargetFields->size(); i3++)
            {
                for (qint32 i4 = 0; i4 < attackPosition.size(); i4++)
                {
                    qint32 x = pTargetFields->at(i3).x() + attackPosition[i4].x();
                    qint32 y = pTargetFields->at(i3).y() + attackPosition[i4].y();
                    if (pMap->onMap(x, y) &&
                        pMap->getTerrain(x, y)->getUnit() == nullptr)
                    {
                        if (pUnit->canMoveOver(x, y))
                        {
                            QVector3D possibleTarget(x, y, 2);
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
    if (m_enableNeutralTerrainAttack)
    {
        // find terrains
        qint32 width = pMap->getMapWidth();
        qint32 heigth = pMap->getMapHeight();
        for (qint32 x = 0; x < width; x++)
        {
            for (qint32 y = 0; y < heigth; y++)
            {
                Terrain* pTerrain = pMap->getTerrain(x, y);
                if (isAttackOnTerrainAllowed(pTerrain) &&
                    pUnit->isEnvironmentAttackable(pTerrain->getID()))
                {
                    for (qint32 i3 = 0; i3 < pTargetFields->size(); i3++)
                    {
                        qint32 x1 = pTargetFields->at(i3).x() + x;
                        qint32 y1 = pTargetFields->at(i3).y() + y;
                        if (pMap->onMap(x1, y1) &&
                            pMap->getTerrain(x1, y1)->getUnit() == nullptr)
                        {
                            if (pUnit->canMoveOver(x1, y1))
                            {
                                QVector3D possibleTarget(x1, y1, 2);
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
    }
}

void CoreAI::rebuildIsland(spQmlVectorUnit pUnits)
{
    Console::print("CoreAI::rebuildIsland", Console::eDEBUG);
    // and create one
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (getIslandIndex(pUnit) < 0)
        {
            createIslandMap(pUnit->getMovementType(), pUnit->getUnitID());
        }
        for (qint32 i2 = 0; i2 < pUnit->getLoadedUnitCount(); i2++)
        {
            if (getIslandIndex(pUnit->getLoadedUnit(i2)) < 0)
            {
                createIslandMap(pUnit->getLoadedUnit(i2)->getMovementType(), pUnit->getLoadedUnit(i2)->getUnitID());
            }
        }
    }
}

bool CoreAI::needsRefuel(Unit *pUnit)
{
    if (pUnit->getMaxFuel() > 0 &&
        pUnit->getFuel() / static_cast<float>(pUnit->getMaxFuel()) < m_fuelResupply)
    {
        return true;
    }
    if (pUnit->getMaxAmmo1() > 0 &&
        pUnit->getAmmo1() / static_cast<float>(pUnit->getMaxAmmo1()) < m_ammoResupply &&
        !pUnit->getWeapon1ID().isEmpty())
    {
        return true;
    }
    if (pUnit->getMaxAmmo2() > 0 &&
        pUnit->getAmmo2() / static_cast<float>(pUnit->getMaxAmmo2()) < m_ammoResupply &&
        !pUnit->getWeapon2ID().isEmpty())
    {
        return true;
    }
    return false;
}

void CoreAI::createIslandMap(QString movementType, QString unitID)
{
    bool found = false;

    for (qint32 i = 0; i < m_IslandMaps.size(); i++)
    {
        if (m_IslandMaps[i]->getMovementType() == movementType)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        m_IslandMaps.append(spIslandMap::create(unitID, m_pPlayer));
    }
}

bool CoreAI::onSameIsland(Unit* pUnit1, Unit* pUnit2)
{
    for (auto i = 0; i < m_IslandMaps.size(); i++)
    {
        if (m_IslandMaps[i]->getMovementType() == pUnit1->getMovementType())
        {
            if (m_IslandMaps[i]->getIsland(pUnit1->Unit::getX(), pUnit1->Unit::getY()) ==
                m_IslandMaps[i]->getIsland(pUnit2->Unit::getX(), pUnit2->Unit::getY()))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

bool CoreAI::onSameIsland(Unit* pUnit1, Building* pBuilding)
{
    for (auto i = 0; i < m_IslandMaps.size(); i++)
    {
        if (m_IslandMaps[i]->getMovementType() == pUnit1->getMovementType())
        {
            if (m_IslandMaps[i]->getIsland(pUnit1->Unit::getX(), pUnit1->Unit::getY()) ==
                m_IslandMaps[i]->getIsland(pBuilding->Building::getX(), pBuilding->Building::getY()))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

bool CoreAI::onSameIsland(QString movemnetType, qint32 x, qint32 y, qint32 x1, qint32 y1)
{
    for (auto i = 0; i < m_IslandMaps.size(); i++)
    {
        if (m_IslandMaps[i]->getMovementType() == movemnetType)
        {
            return onSameIsland(i, x, y, x1, y1);
        }
    }
    return false;
}

bool CoreAI::onSameIsland(qint32 islandIdx, qint32 x, qint32 y, qint32 x1, qint32 y1)
{
    return m_IslandMaps[islandIdx]->sameIsland(x, y, x1, y1);
}

qint32 CoreAI::getIsland(Unit* pUnit)
{
    for (qint32 i = 0; i < m_IslandMaps.size(); i++)
    {
        if (m_IslandMaps[i]->getMovementType() == pUnit->getMovementType())
        {
            return m_IslandMaps[i]->getIsland(pUnit->Unit::getX(), pUnit->Unit::getY());
        }
    }
    return -1;
}

qint32 CoreAI::getIslandIndex(Unit* pUnit)
{
    for (qint32 i = 0; i < m_IslandMaps.size(); i++)
    {
        if (m_IslandMaps[i]->getMovementType() == pUnit->getMovementType())
        {
            return i;
        }
    }
    return -1;
}

void CoreAI::finishTurn()
{
    Console::print("CoreAI::finishTurn(()", Console::eDEBUG);
    usedTransportSystem = false;
    spGameAction pAction = spGameAction::create(ACTION_NEXT_PLAYER);
    CO* pCO0 = m_pPlayer->getCO(0);
    CO* pCO1 = m_pPlayer->getCO(1);
    if (pCO0 != nullptr &&
        pCO0->getPowerMode() == GameEnums::PowerMode_Off)
    {
        pAction->setActionID(ACTION_SWAP_COS);
        if (pAction->canBePerformed())
        {
            float remainingC0 = pCO0->getPowerStars() + pCO0->getSuperpowerStars() - pCO0->getPowerFilled();
            float remainingC1 = pCO1->getPowerStars() + pCO1->getSuperpowerStars() - pCO1->getPowerFilled();
            if (remainingC1 - 0.1f > remainingC0)
            {
                pAction->setActionID(ACTION_SWAP_COS);
            }
        }
        else
        {
            pAction->setActionID(ACTION_NEXT_PLAYER);
        }
    }
    else if (pCO1 != nullptr && pCO0 != nullptr &&
             pCO0->getPowerMode() == GameEnums::PowerMode_Tagpower &&
             pCO1->getPowerMode() == GameEnums::PowerMode_Off)
    {
        pAction->setActionID(ACTION_SWAP_COS);
    }
    else
    {
        pAction->setActionID(ACTION_NEXT_PLAYER);
    }
    emit performAction(pAction);
}

bool CoreAI::useBuilding(spQmlVectorBuilding pBuildings)
{
    Console::print("CoreAI::useBuilding", Console::eDEBUG);
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        QStringList actions = pBuilding->getActionList();
        if (actions.size() >= 1 &&
            !actions[0].isEmpty())
        {
            for (qint32 i = 0; i < actions.size(); i++)
            {
                if (actions[i] != ACTION_BUILD_UNITS)
                {
                    spGameAction pAction = spGameAction::create(actions[i]);
                    pAction->setTarget(QPoint(pBuilding->Building::getX(), pBuilding->Building::getY()));
                    if (pAction->canBePerformed())
                    {
                        if (pAction->isFinalStep())
                        {
                            emit performAction(pAction);
                            return true;
                        }
                        else
                        {
                            while (!pAction->isFinalStep())
                            {
                                if (pAction->getStepInputType() == "FIELD")
                                {
                                    spMarkedFieldData pData = pAction->getMarkedFieldStepData();
                                    QVector<QPoint>* points = pData->getPoints();
                                    qint32 index = -1;
                                    QPoint target;
                                    qint32 maxValue = std::numeric_limits<qint32>::lowest();
                                    for (qint32 i2 = 0; i2 < points->size(); i2++)
                                    {
                                        Unit* pUnit = pMap->getTerrain(points->at(i2).x(), points->at(i2).y())->getUnit();
                                        qint32 unitValue = pUnit->getUnitValue();
                                        if (pUnit != nullptr && unitValue > maxValue)
                                        {
                                            maxValue = unitValue;
                                            index = i2;
                                        }
                                    }
                                    if (index < 0)
                                    {
                                        target = points->at(GlobalUtils::randIntBase(0, points->size() -1));
                                    }
                                    else
                                    {
                                        target = points->at(index);
                                    }
                                    addSelectedFieldData(pAction, target);
                                }
                                else if (pAction->getStepInputType() == "MENU")
                                {
                                    spMenuData pData = pAction->getMenuStepData();
                                    if (pData->validData())
                                    {
                                        QStringList items = pData->getActionIDs();
                                        auto enable = pData->getEnabledList();
                                        qint32 i = 0;
                                        while (i < enable.size())
                                        {
                                            if (enable[i])
                                            {
                                                i++;
                                            }
                                            else
                                            {
                                                items.removeAt(i);
                                                enable.removeAt(i);
                                            }
                                        }
                                        qint32 selection = GlobalUtils::randIntBase(0, items.size() - 1);
                                        addMenuItemData(pAction, items[selection], pData->getCostList()[selection]);
                                    }
                                }
                                else
                                {
                                    break;
                                }
                            }
                            if (pAction->isFinalStep())
                            {
                                emit performAction(pAction);
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

float CoreAI::getAiCoUnitMultiplier(CO* pCO, Unit* pUnit)
{
    bool valid = false;
    float multiplier = 0.0f;
    if (pCO != nullptr)
    {
        multiplier = pCO->getAiCoUnitBonus(pUnit, valid);
        if (!valid)
        {
            if (pCO->getOffensiveBonus(nullptr, pUnit, QPoint(-1, -1), nullptr, QPoint(-1, -1), false) > 0 ||
                pCO->getDeffensiveBonus(nullptr, nullptr, QPoint(-1, -1), pUnit, QPoint(-1, -1), false) > 0 ||
                pCO->getFirerangeModifier(pUnit, QPoint(-1, -1)) > 0)
            {
                multiplier = 1.0f;
            }
        }
    }
    return multiplier;
}

void CoreAI::GetOwnUnitCounts(spQmlVectorUnit pUnits, spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings,
                              qint32 & infantryUnits, qint32 & indirectUnits,
                              qint32 & directUnits, QVector<std::tuple<Unit*, Unit*>> & transportTargets)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (pUnit->getActionList().contains(ACTION_CAPTURE))
        {
            infantryUnits++;
        }
        else if (pUnit->hasWeapons())
        {
            if (pUnit->getBaseMaxRange() > 1)
            {
                indirectUnits++;
            }
            else
            {
                directUnits++;
            }
        }
        if (pUnit->getLoadingPlace() > 0)
        {
            QVector<QVector3D> ret;
            QVector<Unit*> transportUnits = appendLoadingTargets(pUnit, pUnits, pEnemyUnits, pEnemyBuildings, false, true, ret, true);
            for (qint32 i2 = 0; i2 < transportUnits.size(); i2++)
            {
                transportTargets.append(std::tuple<Unit*, Unit*>(pUnit, transportUnits[i2]));
            }
        }
    }
}

bool CoreAI::buildCOUnit(spQmlVectorUnit pUnits)
{
    Console::print("CoreAI::buildCOUnit", Console::eDEBUG);
    spGameAction pAction = spGameAction::create();
    for (quint8 i2 = 0; i2 < 2; i2++)
    {
        if (i2 == 0)
        {
            pAction->setActionID(ACTION_CO_UNIT_0);
        }
        else
        {
            pAction->setActionID(ACTION_CO_UNIT_1);
        }
        CO* pCO = m_pPlayer->getCO(i2);
        qint32 bestScore = 0;
        qint32 unitIdx = -1;
        if (pCO != nullptr &&
            pCO->getCOUnit() == nullptr)
        {
            qint32 active = 0;
            bool expensive = false;
            for (qint32 i = 0; i < pUnits->size(); i++)
            {
                Unit* pUnit = pUnits->at(i);

                if (pUnit->getUnitValue() >= m_coUnitValue && pUnit->getUnitRank() >= GameEnums::UnitRank_None)
                {
                    active++;
                }
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                if (pAction->canBePerformed())
                {
                    if (!pUnit->getHasMoved())
                    {
                        if (pUnit->hasWeapons())
                        {
                            qint32 score = 0;
                            score += pUnit->getUnitValue() * m_coUnitScoreMultiplier * getAiCoUnitMultiplier(pCO, pUnit);
                            if (pUnit->getUnitValue() >= m_coUnitValue)
                            {
                                expensive = true;
                            }
                            score -= m_coUnitRankReduction * pUnit->getUnitRank();
                            if (score > bestScore)
                            {
                                bestScore = score;
                                unitIdx = i;
                            }
                        }
                    }
                }
            }
            if (unitIdx >= 0 && bestScore > m_minCoUnitScore && (active > m_minCoUnitCount || expensive))
            {
                Unit* pUnit = pUnits->at(unitIdx);
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                emit performAction(pAction);
                return true;
            }
        }
    }
    return false;
}

void CoreAI::serializeObject(QDataStream& stream) const
{
    stream << getVersion();
    stream << m_enableNeutralTerrainAttack;
    stream << static_cast<qint32>(m_BuildingChanceModifier.size());
    for (qint32 i = 0; i < m_BuildingChanceModifier.size(); i++)
    {
        stream << std::get<0>(m_BuildingChanceModifier[i]);
        stream << std::get<1>(m_BuildingChanceModifier[i]);
    }
    stream << m_MoveCostMap.size();
    for (qint32 x = 0; x < m_MoveCostMap.size(); x++)
    {
        stream << m_MoveCostMap[x].size();
        for (qint32 y = 0; y < m_MoveCostMap[x].size(); y++)
        {
            stream << std::get<0>(m_MoveCostMap[x][y]);
            stream << std::get<1>(m_MoveCostMap[x][y]);
        }
    }
    stream << m_files.size();
    for (qint32 i = 0; i < m_files.size(); i++)
    {
        stream << m_files[i];
    }
}

void CoreAI::deserializeObject(QDataStream& stream)
{
    qint32 version;
    stream >> version;
    if (version > 1)
    {
        stream >> m_enableNeutralTerrainAttack;
    }
    if (version > 2)
    {
        qint32 size = 0;
        stream >> size;
        m_BuildingChanceModifier.clear();
        for (qint32 i = 0; i < size; i++)
        {
            QString unitID;
            float value = 1.0f;
            stream >> unitID;
            stream >> value;
            m_BuildingChanceModifier.append(std::tuple<QString, float>(unitID, value));
        }
    }
    if (version > 3)
    {
        qint32 size = 0;
        stream >> size;
        m_MoveCostMap.clear();
        for (qint32 x = 0; x < size; x++)
        {
            qint32 ysize = 0;
            stream >> ysize;
            m_MoveCostMap.append(QVector<std::tuple<qint32, bool>>());
            for (qint32 y = 0; y < ysize; y++)
            {
                m_MoveCostMap[x].append(std::tuple<qint32, bool>(0, false));
                stream >> std::get<0>(m_MoveCostMap[x][y]);
                stream >> std::get<1>(m_MoveCostMap[x][y]);
            }
        }
    }
    if (version > 4)
    {
        m_files.clear();
        qint32 size = 0;
        stream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            QString list;
            stream >> list;
            loadIni(list);
        }
    }
}
