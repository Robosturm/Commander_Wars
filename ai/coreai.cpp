 #include "ai/coreai.h"

#include "ai/targetedunitpathfindingsystem.h"

#include "coreengine/mainapp.h"

#include "game/gameanimationfactory.h"

#include "game/gamemap.h"

#include "game/unit.h"

#include "game/gameaction.h"

#include "game/unitpathfindingsystem.h"

#include "game/player.h"

#include "game/co.h"

#include "menue/gamemenue.h"

#include "resource_management/cospritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/buildingspritemanager.h"

#include <qfile.h>
#include <qtextstream.h>

const QString CoreAI::ACTION_WAIT = "ACTION_WAIT";
const QString CoreAI::ACTION_HOELLIUM_WAIT = "ACTION_HOELLIUM_WAIT";
const QString CoreAI::ACTION_REPAIR = "ACTION_REPAIR";
const QString CoreAI::ACTION_RATION = "ACTION_RATION";
const QString CoreAI::ACTION_UNSTEALTH = "ACTION_UNSTEALTH";
const QString CoreAI::ACTION_STEALTH = "ACTION_STEALTH";
const QString CoreAI::ACTION_BUILD_UNITS = "ACTION_BUILD_UNITS";
const QString CoreAI::ACTION_CAPTURE = "ACTION_CAPTURE";
const QString CoreAI::ACTION_MISSILE = "ACTION_MISSILE";
const QString CoreAI::ACTION_FIRE = "ACTION_FIRE";
const QString CoreAI::ACTION_UNLOAD = "ACTION_UNLOAD";
const QString CoreAI::ACTION_LOAD = "ACTION_LOAD";
const QString CoreAI::ACTION_NEXT_PLAYER = "ACTION_NEXT_PLAYER";
const QString CoreAI::ACTION_SWAP_COS = "ACTION_SWAP_COS";
const QString CoreAI::ACTION_TAGPOWER = "ACTION_TAGPOWER";
const QString CoreAI::ACTION_ACTIVATE_POWER_CO_0 = "ACTION_ACTIVATE_POWER_CO_0";
const QString CoreAI::ACTION_ACTIVATE_POWER_CO_1 = "ACTION_ACTIVATE_POWER_CO_1";
const QString CoreAI::ACTION_ACTIVATE_SUPERPOWER_CO_0 = "ACTION_ACTIVATE_SUPERPOWER_CO_0";
const QString CoreAI::ACTION_ACTIVATE_SUPERPOWER_CO_1 = "ACTION_ACTIVATE_SUPERPOWER_CO_1";
const QString CoreAI::ACTION_CO_UNIT_0 = "ACTION_CO_UNIT_0";
const QString CoreAI::ACTION_CO_UNIT_1 = "ACTION_CO_UNIT_1";
const QString CoreAI::ACTION_EXPLODE = "ACTION_EXPLODE";

CoreAI::CoreAI(BaseGameInputIF::AiTypes aiType)
    : BaseGameInputIF(aiType)
{
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
}

CoreAI::~CoreAI()
{
}

void CoreAI::init()
{
    connect(GameMenue::getInstance(), &GameMenue::sigActionPerformed, this, &CoreAI::nextAction, Qt::QueuedConnection);
    connect(this, &CoreAI::performAction, GameMenue::getInstance(), &GameMenue::performAction, Qt::QueuedConnection);
}

void CoreAI::nextAction()
{
    // check if it's our turn
    GameMenue* pMenue = GameMenue::getInstance();
    if (pMenue != nullptr &&
        m_pPlayer == GameMap::getInstance()->getCurrentPlayer() &&
        pMenue->getGameStarted())
    {
        if (!processPredefinedAi())
        {
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

void CoreAI::getBestTarget(Unit* pUnit, GameAction* pAction, UnitPathFindingSystem* pPfs, QVector<QVector3D>& ret, QVector<QVector3D>& moveTargetFields)
{
    pAction->setMovepath(QVector<QPoint>(1, QPoint(pUnit->getX(), pUnit->getY())));
    getAttacksFromField(pUnit, pAction, ret, moveTargetFields);
    if (pUnit->canMoveAndFire(QPoint(pUnit->getX(), pUnit->getY())))
    {
        GameMap* pMap = GameMap::getInstance();
        QVector<QPoint> targets = pPfs->getAllNodePoints();
        for (qint32 i2 = 0; i2 < targets.size(); i2++)
        {
            if (pMap->getTerrain(targets[i2].x(), targets[i2].y())->getUnit() == nullptr)
            {
                pAction->setMovepath(QVector<QPoint>(1, targets[i2]));
                getAttacksFromField(pUnit, pAction, ret, moveTargetFields);
            }
        }
    }
}

void CoreAI::appendAttackTargets(Unit* pUnit, QmlVectorUnit* pEnemyUnits, QVector<QVector3D>& targets)
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i2 = 0; i2 < pEnemyUnits->size(); i2++)
    {
        Unit* pEnemy = pEnemyUnits->at(i2);
        if (pUnit->isAttackable(pEnemy, true))
        {
            qint32 firerange = pUnit->getMaxRange();
            QmlVectorPoint* pTargetFields = Mainapp::getCircle(firerange, firerange);
            for (qint32 i3 = 0; i3 < pTargetFields->size(); i3++)
            {
                qint32 x = pTargetFields->at(i3).x() + pEnemy->getX();
                qint32 y = pTargetFields->at(i3).y() + pEnemy->getY();
                if (pMap->onMap(x, y) &&
                    pMap->getTerrain(x, y)->getUnit() == nullptr)
                {
                    if (pUnit->canMoveOver(x, y))
                    {
                        QVector3D possibleTarget(x, y, 1);
                        if (!targets.contains(possibleTarget))
                        {
                            targets.append(possibleTarget);
                        }
                    }
                }
            }
            delete pTargetFields;
        }
    }
}

void CoreAI::getAttacksFromField(Unit* pUnit, GameAction* pAction, QVector<QVector3D>& ret, QVector<QVector3D>& moveTargetFields)
{
    GameMap* pMap = GameMap::getInstance();
    // much easier case
    // check if the action can be performed here
    if (pAction->canBePerformed())
    {
        MarkedFieldData* pMarkedFieldData = pAction->getMarkedFieldStepData();
        for (qint32 i = 0; i < pMarkedFieldData->getPoints()->size(); i++)
        {
            QPoint target = pMarkedFieldData->getPoints()->at(i);
            QRectF damage = calcUnitDamage(pAction, target);
            Terrain* pTerrain = pMap->getTerrain(target.x(), target.y());
            Unit* pDef = pTerrain->getUnit();
            if (pDef != nullptr)
            {
                float atkDamage = static_cast<float>(damage.x()) / 10.0f;
                if (atkDamage > pDef->getHp())
                {
                    atkDamage = pDef->getHp();
                }
                float fondsDamage = pDef->getUnitCosts() * atkDamage / 10.0f;
                if (damage.width() >= 0.0)
                {
                    atkDamage = static_cast<float>(damage.width()) / 10.0f;
                    if (atkDamage > pUnit->getHp())
                    {
                        atkDamage = pUnit->getHp();
                    }
                    fondsDamage -= pUnit->getUnitCosts() * atkDamage / 10.0f;
                }
                if (ret.size() == 0)
                {
                    ret.append(QVector3D(target.x(), target.y(), fondsDamage));
                    QPoint point = pAction->getActionTarget();
                    moveTargetFields.append(QVector3D(point.x(), point.y(), 1));
                }
                else if (static_cast<float>(ret[0].z()) == fondsDamage)
                {
                    ret.append(QVector3D(target.x(), target.y(), fondsDamage));
                    QPoint point = pAction->getActionTarget();
                    moveTargetFields.append(QVector3D(point.x(), point.y(), 1));
                }
                else if (fondsDamage > ret[0].z())
                {
                    ret.clear();
                    moveTargetFields.clear();
                    ret.append(QVector3D(target.x(), target.y(), fondsDamage));
                    QPoint point = pAction->getActionTarget();
                    moveTargetFields.append(QVector3D(point.x(), point.y(), 1));
                }
            }
            else
            {
                if (ret.size() == 0)
                {
                    ret.append(QVector3D(target.x(), target.y(), static_cast<float>(damage.x())));
                    QPoint point = pAction->getActionTarget();
                    moveTargetFields.append(QVector3D(point.x(), point.y(), 1));
                }
                else if (ret[0].z() == static_cast<float>(damage.x()))
                {
                    ret.append(QVector3D(target.x(), target.y(), static_cast<float>(damage.x())));
                    QPoint point = pAction->getActionTarget();
                    moveTargetFields.append(QVector3D(point.x(), point.y(), 1));
                }
                else if (static_cast<float>(damage.x()) > ret[0].z())
                {
                    ret.clear();
                    moveTargetFields.clear();
                    ret.append(QVector3D(target.x(), target.y(), static_cast<float>(damage.x())));
                    QPoint point = pAction->getActionTarget();
                    moveTargetFields.append(QVector3D(point.x(), point.y(), 1));
                }
            }
        }
        delete pMarkedFieldData;
    }
}

QRectF CoreAI::calcUnitDamage(GameAction* pAction, QPoint target)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "calcBattleDamage";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pAction);
    args1 << obj1;
    args1 << target.x();
    args1 << target.y();
    args1 << static_cast<qint32>(GameEnums::LuckDamageMode_Average);
    QJSValue erg = pApp->getInterpreter()->doFunction(ACTION_FIRE, function1, args1);
    return erg.toVariant().toRectF();
}

void CoreAI::getTrainingData(QString file, QVector<QVector<float>>& trainingData, QVector<QVector<spDecisionQuestion>>& questions)
{
    QFile trainingFile(file);
    trainingFile.open(QIODevice::ReadOnly | QIODevice::Truncate);
    QTextStream stream(&trainingFile);
    bool questionsFound = false;
    QStringList types;

    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();

    QVector<spDecisionQuestion> readQuestions;

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
            if (line.startsWith("type") && !questionsFound)
            {
                questionsFound = true;
                QStringList items = line.split(" ");
                for (qint32 i = 1; i < items.size(); i++)
                {
                    readQuestions.append(new DecisionQuestion());
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
                                    readQuestions[i - 1]->appendQuestion(new Question(value, index, GameEnums::AIQuestionType_Greater));
                                }
                                else if (questionData[1] == ">")
                                {
                                    readQuestions[i - 1]->appendQuestion(new Question(value, index, GameEnums::AIQuestionType_Smaler));
                                }
                                else if (questionData[1] == "=")
                                {
                                    readQuestions[i - 1]->appendQuestion(new Question(value, index, GameEnums::AIQuestionType_Equal));
                                }
                            }
                            else if (questionData.size() == 3)
                            {
                                float value1 = questionData[0].toFloat();
                                float value2 = questionData[2].toFloat();
                                readQuestions[i - 1]->appendQuestion(new Question(value1, value2, index, GameEnums::AIQuestionType_Between));
                            }
                        }
                        types.append("NUMBER");
                    }
                    else
                    {
                        if (items[i] == "CO")
                        {
                            for (qint32 i2 = 0; i2 < pCOSpriteManager->getCOCount(); i2++)
                            {
                                readQuestions[i - 1]->appendQuestion(new Question(i2, index, GameEnums::AIQuestionType_Equal));
                            }
                            readQuestions[i - 1]->appendQuestion(new Question(-1, index, GameEnums::AIQuestionType_Equal));
                        }
                        else if (items[i] == "BUILDING")
                        {
                            for (qint32 i2 = 0; i2 < pBuildingSpriteManager->getBuildingCount(); i2++)
                            {
                                readQuestions[i - 1]->appendQuestion(new Question(i2, index, GameEnums::AIQuestionType_Equal));
                            }
                            readQuestions[i - 1]->appendQuestion(new Question(-1, index, GameEnums::AIQuestionType_Equal));
                        }
                        else if (items[i] == "UNIT")
                        {
                            for (qint32 i2 = 0; i2 < pUnitSpriteManager->getUnitCount(); i2++)
                            {
                                readQuestions[i - 1]->appendQuestion(new Question(i2, index, GameEnums::AIQuestionType_Equal));
                            }
                            readQuestions[i - 1]->appendQuestion(new Question(-1, index, GameEnums::AIQuestionType_Equal));
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
                            qint32 index = pCOSpriteManager->getCOIndex(items[i]);
                            trainingData[item].append(index);
                            if (i < types.size() - 1)
                            {
                                questions[item].append(readQuestions[i]);
                            }
                        }
                        else if (types[i] == "BUILDING")
                        {
                            qint32 index = pBuildingSpriteManager->getBuildingIndex(items[i]);
                            trainingData[item].append(index);
                            if (i < types.size() - 1)
                            {
                                questions[item].append(readQuestions[i]);
                            }
                        }
                        else if (types[i] == "UNIT")
                        {
                            qint32 index = pUnitSpriteManager->getUnitIndex(items[i]);
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

bool CoreAI::getEnableBuildingAttack() const
{
    return enableBuildingAttack;
}

void CoreAI::setEnableBuildingAttack(bool value)
{
    enableBuildingAttack = value;
}

void CoreAI::addMenuItemData(GameAction* pGameAction, QString itemID, qint32 cost)
{
    pGameAction->writeDataString(itemID);
    // increase costs and input step
    pGameAction->setCosts(pGameAction->getCosts() + cost);
    pGameAction->setInputStep(pGameAction->getInputStep() + 1);
}

void CoreAI::addSelectedFieldData(GameAction* pGameAction, QPoint point)
{
    pGameAction->writeDataInt32(point.x());
    pGameAction->writeDataInt32(point.y());
    pGameAction->setInputStep(pGameAction->getInputStep() + 1);
}

void CoreAI::finishTurn()
{
    GameAction* pAction = new GameAction(ACTION_NEXT_PLAYER);
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

bool CoreAI::useBuilding(QmlVectorBuilding* pBuildings)
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        QStringList actions = pBuilding->getActionList();
        if (actions.size() == 1 &&
            actions[0] != ACTION_BUILD_UNITS &&
            !actions[0].isEmpty())
        {
            GameAction* pAction = new GameAction(actions[0]);
            pAction->setTarget(QPoint(pBuilding->getX(), pBuilding->getY()));
            if (pAction->canBePerformed())
            {
                if (pAction->isFinalStep())
                {

                    emit performAction(pAction);
                    return true;
                }
                else
                {
                    if (pAction->getStepInputType() == "FIELD")
                    {
                        MarkedFieldData* pData = pAction->getMarkedFieldStepData();
                        QVector<QPoint>* points = pData->getPoints();
                        qint32 index = -1;
                        QPoint target;
                        qint32 maxValue = std::numeric_limits<qint32>::min();
                        for (qint32 i2 = 0; i2 < points->size(); i2++)
                        {
                            Unit* pUnit = pMap->getTerrain(points->at(i2).x(), points->at(i2).y())->getUnit();
                            if (pUnit != nullptr && pUnit->getUnitValue() > maxValue)
                            {
                                maxValue = pUnit->getUnitValue();
                                index = i2;
                            }
                        }
                        if (index < 0)
                        {
                            target = points->at(Mainapp::randInt(0, points->size() -1));
                        }
                        delete pData;

                        addSelectedFieldData(pAction, target);
                        if (pAction->isFinalStep())
                        {
                            emit performAction(pAction);
                            return true;
                        }
                    }
                }
            }
            delete pAction;
        }
    }
    return false;
}
