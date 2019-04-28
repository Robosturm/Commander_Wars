#include "coreai.h"

#include "coreengine/mainapp.h"

#include "game/gameanimationfactory.h"

#include "game/gamemap.h"

#include "game/unit.h"

#include "game/gameaction.h"

#include "game/unitpathfindingsystem.h"

#include "menue/gamemenue.h"

#include "resource_management/cospritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/buildingspritemanager.h"

#include <qfile.h>
#include <qtextstream.h>

const QString CoreAI::ACTION_WAIT = "ACTION_WAIT";
const QString CoreAI::ACTION_REPAIR = "ACTION_REPAIR";
const QString CoreAI::ACTION_RATION = "ACTION_RATION";
const QString CoreAI::ACTION_BUILD_UNITS = "ACTION_BUILD_UNITS";
const QString CoreAI::ACTION_CAPTURE = "ACTION_CAPTURE";
const QString CoreAI::ACTION_MISSILE = "ACTION_MISSILE";
const QString CoreAI::ACTION_FIRE = "ACTION_FIRE";
const QString CoreAI::ACTION_UNLOAD = "ACTION_UNLOAD";
const QString CoreAI::ACTION_LOAD = "ACTION_LOAD";
const QString CoreAI::ACTION_NEXT_PLAYER = "ACTION_NEXT_PLAYER";
const QString CoreAI::ACTION_ACTIVATE_POWER_CO_0 = "ACTION_ACTIVATE_POWER_CO_0";
const QString CoreAI::ACTION_ACTIVATE_POWER_CO_1 = "ACTION_ACTIVATE_POWER_CO_1";
const QString CoreAI::ACTION_ACTIVATE_SUPERPOWER_CO_0 = "ACTION_ACTIVATE_SUPERPOWER_CO_0";
const QString CoreAI::ACTION_ACTIVATE_SUPERPOWER_CO_1 = "ACTION_ACTIVATE_SUPERPOWER_CO_1";
const QString CoreAI::ACTION_CO_UNIT_0 = "ACTION_CO_UNIT_0";
const QString CoreAI::ACTION_CO_UNIT_1 = "ACTION_CO_UNIT_1";

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
    if (m_pPlayer == GameMap::getInstance()->getCurrentPlayer())
    {
        // if so execute next action
        process();
    }
}

void CoreAI::getBestTarget(Unit* pUnit, GameAction* pAction, UnitPathFindingSystem* pPfs, QVector<QVector3D>& ret, QVector<QPoint>& moveTargetFields)
{
    getAttacksFromField(pUnit, pAction, ret, moveTargetFields);
    if (pUnit->canMoveAndFire(QPoint(pUnit->getX(), pUnit->getY())))
    {
        GameMap* pMap = GameMap::getInstance();
        QVector<QPoint> targets = pPfs->getAllNodePoints();
        for (qint32 i2 = 0; i2 < targets.size(); i2++)
        {
            if (pMap->getTerrain(targets[i2].x(), targets[i2].y())->getUnit() == nullptr)
            {
                pAction->setMovepath(pPfs->getPath(targets[i2].x(), targets[i2].y()));
                getAttacksFromField(pUnit, pAction, ret, moveTargetFields);
            }
        }
    }
}

void CoreAI::getAttacksFromField(Unit* pUnit, GameAction* pAction, QVector<QVector3D>& ret, QVector<QPoint>& moveTargetFields)
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
                float atkDamage = damage.x() / 10.0f;
                if (atkDamage > pDef->getHp())
                {
                    atkDamage = pDef->getHp();
                }
                float fondsDamage = pDef->getUnitCosts() * atkDamage / 10.0f;
                if (damage.width() > 0)
                {
                    atkDamage = damage.width() / 10.0f;
                    if (atkDamage > pUnit->getHp())
                    {
                        atkDamage = pUnit->getHp();
                    }
                    fondsDamage -= pUnit->getUnitCosts() * atkDamage / 10.0f;
                }
                if (ret.size() == 0)
                {
                    ret.append(QVector3D(target.x(), target.y(), fondsDamage));
                    moveTargetFields.append(pAction->getActionTarget());
                }
                else if (ret[0].z() == fondsDamage)
                {
                    ret.append(QVector3D(target.x(), target.y(), fondsDamage));
                    moveTargetFields.append(pAction->getActionTarget());
                }
                else if (fondsDamage > ret[0].z())
                {
                    ret.clear();
                    moveTargetFields.clear();
                    ret.append(QVector3D(target.x(), target.y(), fondsDamage));
                    moveTargetFields.append(pAction->getActionTarget());
                }
            }
            else
            {
                if (ret.size() == 0)
                {
                    ret.append(QVector3D(target.x(), target.y(), damage.x()));
                    moveTargetFields.append(pAction->getActionTarget());
                }
                else if (ret[0].z() == damage.x())
                {
                    ret.append(QVector3D(target.x(), target.y(), damage.x()));
                    moveTargetFields.append(pAction->getActionTarget());
                }
                else if (damage.x() > ret[0].z())
                {
                    ret.clear();
                    moveTargetFields.clear();
                    ret.append(QVector3D(target.x(), target.y(), damage.x()));
                    moveTargetFields.append(pAction->getActionTarget());
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
                        }
                        else if (items[i] == "BUILDING")
                        {
                            for (qint32 i2 = 0; i2 < pBuildingSpriteManager->getBuildingCount(); i2++)
                            {
                                readQuestions[i - 1]->appendQuestion(new Question(i2, index, GameEnums::AIQuestionType_Equal));
                            }
                        }
                        else if (items[i] == "UNIT")
                        {
                            for (qint32 i2 = 0; i2 < pUnitSpriteManager->getUnitCount(); i2++)
                            {
                                readQuestions[i - 1]->appendQuestion(new Question(i2, index, GameEnums::AIQuestionType_Equal));
                            }
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
