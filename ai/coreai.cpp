#include "coreai.h"

#include "coreengine/mainapp.h"

#include "game/gameanimationfactory.h"

#include "game/gamemap.h"

#include "game/unit.h"

#include "game/gameaction.h"

#include "game/unitpathfindingsystem.h"

#include "menue/gamemenue.h"

const QString CoreAI::ACTION_CAPTURE = "ACTION_CAPTURE";
const QString CoreAI::ACTION_FIRE = "ACTION_FIRE";
const QString CoreAI::ACTION_NEXT_PLAYER = "ACTION_NEXT_PLAYER";

CoreAI::CoreAI()
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
    connect(GameAnimationFactory::getInstance(), &GameAnimationFactory::animationsFinished, this, &CoreAI::nextAction, Qt::QueuedConnection);
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
        QVector<QPoint> targets = pPfs->getAllNodePoints();
        for (qint32 i2 = 0; i2 < targets.size(); i2++)
        {
            pAction->setMovepath(pPfs->getPath(targets[i2].x(), targets[i2].y()));
            getAttacksFromField(pUnit, pAction, ret, moveTargetFields);
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
                // attacking a building or terrain
                // do nothing for now
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
