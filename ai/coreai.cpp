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

QVector3D CoreAI::getBestTarget(Unit* pUnit, GameAction* pAction, UnitPathFindingSystem* pPfs)
{
    if (pUnit->canMoveAndFire(QPoint(pUnit->getX(), pUnit->getY())))
    {

    }
    else
    {
        // much easier case
        // check if the action can be performed here
        if (pAction->canBePerformed())
        {
            MarkedFieldData* pMarkedFieldData = pAction->getMarkedFieldStepData();

            for (qint32 i = 0; i < pMarkedFieldData->getPoints()->size(); i++)
            {
                QPoint target = pMarkedFieldData->getPoints()->at(i);

            }

            delete pMarkedFieldData;
        }
    }
}

QVector2D CoreAI::calcUnitDamage(Unit* pUnit, QPoint position, QPoint target)
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "calcAttackerDamage";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(pUnit);
    args1 << obj1;

    QJSValue erg = pApp->getInterpreter()->doFunction(ACTION_FIRE, function1, args1);
}
