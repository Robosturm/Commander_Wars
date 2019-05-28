#include "normalai.h"

#include "coreengine/qmlvector.h"

#include "game/player.h"
#include "game/unit.h"
#include "game/co.h"
#include "game/gameaction.h"
#include "game/gamemap.h"
#include "game/building.h"
#include "game/unitpathfindingsystem.h"

#include "ai/targetedunitpathfindingsystem.h"

NormalAi::NormalAi()
    : CoreAI (BaseGameInputIF::AiTypes::Normal)
{
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
}

void NormalAi::process()
{
    QmlVectorBuilding* pBuildings = m_pPlayer->getBuildings();
    pBuildings->randomize();
    QmlVectorUnit* pUnits = nullptr;
    QmlVectorUnit* pEnemyUnits = nullptr;
    QmlVectorBuilding* pEnemyBuildings = nullptr;

    if (useBuilding(pBuildings)){}
    else
    {
        pUnits = m_pPlayer->getUnits();
        // pUnits->sortShortestMovementRange();
        pEnemyUnits = m_pPlayer->getEnemyUnits();
        pEnemyUnits->randomize();
        pEnemyBuildings = m_pPlayer->getEnemyBuildings();
        pEnemyBuildings->randomize();
        if (CoreAI::moveOoziums(pUnits, pEnemyUnits)){}
        else if (CoreAI::moveBlackBombs(pUnits, pEnemyUnits)){}
        else if (captureBuildings(pUnits)){}
        else
        {

            finishTurn();
        }
    }



    delete pBuildings;
    delete pUnits;
    delete pEnemyBuildings;
    delete pEnemyUnits;
}

bool NormalAi::isUsingUnit(Unit* pUnit)
{
    if (pUnit->getMaxFuel() > 0 &&
        pUnit->getFuel() / static_cast<float>(pUnit->getMaxFuel()) < 1.0f / 3.0f)
    {
        return false;
    }
    if (pUnit->getMaxAmmo1() > 0 &&
        pUnit->getAmmo1() / static_cast<float>(pUnit->getMaxAmmo1()) < 1.0f / 3.0f)
    {
        return false;
    }
    if (pUnit->getMaxAmmo2() > 0 &&
        pUnit->getAmmo2() / static_cast<float>(pUnit->getMaxAmmo2()) < 1.0f / 3.0f)
    {
        return false;
    }
    if (pUnit->getHpRounded() < 5)
    {
        return false;
    }
    if (!pUnit->getHasMoved())
    {
        return false;
    }
    return true;
}

void NormalAi::calcVirtualDamage()
{

}

bool NormalAi::captureBuildings(QmlVectorUnit* pUnits)
{
    QVector<QVector3D> captureBuildings;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (!pUnit->getHasMoved() && pUnit->getActionList().contains(ACTION_CAPTURE))
        {
            if (pUnit->getCapturePoints() > 0)
            {
                GameAction* pAction = new GameAction(ACTION_CAPTURE);
                pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                emit performAction(pAction);
                return true;
            }
            else
            {
                GameAction action(ACTION_CAPTURE);
                action.setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                UnitPathFindingSystem pfs(pUnit);
                pfs.explore();
                QVector<QPoint> targets = pfs.getAllNodePoints();
                for (qint32 i2 = 0; i2 < targets.size(); i2++)
                {
                    action.setActionID(ACTION_CAPTURE);
                    action.setMovepath(QVector<QPoint>(1, targets[i2]));
                    if (action.canBePerformed())
                    {
                        captureBuildings.append(QVector3D(targets[i2].x(), targets[i2].y(), i));
                    }
                    else
                    {
                        action.setActionID(ACTION_MISSILE);
                        if (action.canBePerformed())
                        {
                            captureBuildings.append(QVector3D(targets[i2].x(), targets[i2].y(), i));
                        }
                    }
                }
            }
        }
    }
    if (captureBuildings.size() > 0)
    {
        GameMap* pMap = GameMap::getInstance();
        for (qint32 i = 0; i < pUnits->size(); i++)
        {
            Unit* pUnit = pUnits->at(i);
            if (!pUnit->getHasMoved() && pUnit->getActionList().contains(ACTION_CAPTURE))
            {
                QVector<QVector3D> captures;
                for (qint32 i2 = 0; i2 < captureBuildings.size(); i2++)
                {
                    if (static_cast<qint32>(captureBuildings[i2].z()) == i)
                    {
                        captures.append(captureBuildings[i2]);
                    }
                }
                bool perform = false;
                qint32 targetIndex = 0;
                if (captures.size() == 0)
                {
                    // we have only one target go for it
                    targetIndex = 0;
                    perform = true;
                }
                else
                {
                    // check if we have a building only we can capture and capture it
                    for (qint32 i2 = 0; i2 < captures.size(); i2++)
                    {
                        qint32 captureCount = 0;
                        for (qint32 i3 = 0; i3 < captureBuildings.size(); i3++)
                        {
                            if (captureBuildings[i3].x() == captures[i2].x() &&
                                captureBuildings[i3].y() == captures[i2].y())
                            {
                                captureCount++;
                            }
                        }
                        if (captureCount == 1)
                        {
                            targetIndex = i2;
                            perform = true;
                            break;
                        }
                    }
                    // check if there unique captures open
                    bool skipUnit = false;
                    for (qint32 i2 = 0; i2 < captureBuildings.size(); i2++)
                    {
                        qint32 captureCount = 0;
                        for (qint32 i3 = 0; i3 < captureBuildings.size(); i3++)
                        {
                            if (captureBuildings[i3].x() == captureBuildings[i2].x() &&
                                captureBuildings[i3].y() == captureBuildings[i2].y())
                            {
                                captureCount++;
                            }
                        }
                        if (captureCount == 1)
                        {
                           skipUnit = true;
                        }
                    }
                    // if not we can select a target from the list
                    if (!skipUnit)
                    {
                        targetIndex = 0;
                        // priorities production buildings over over captures
                        for (qint32 i2 = 0; i2 < captures.size(); i2++)
                        {
                            if (pMap->getTerrain(captures[i2].x(), captures[i2].y())->getBuilding()->getActionList().contains(ACTION_BUILD_UNITS))
                            {
                                targetIndex = i2;
                                break;
                            }
                        }
                        perform = true;
                    }
                }
                // perform capturing
                if (perform)
                {
                    UnitPathFindingSystem pfs(pUnit);
                    pfs.explore();
                    GameAction* pAction = new GameAction(ACTION_CAPTURE);
                    pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                    pAction->setMovepath(pfs.getPath(captures[targetIndex].x(), captures[targetIndex].y()));
                    if (pAction->canBePerformed())
                    {
                        emit performAction(pAction);
                        return true;
                    }
                    else
                    {
                        QPoint rocketTarget = m_pPlayer->getRockettarget(2, 3);
                        CoreAI::addSelectedFieldData(pAction, rocketTarget);
                        pAction->setActionID(ACTION_MISSILE);
                        emit performAction(pAction);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void NormalAi::serializeObject(QDataStream&)
{
}
void NormalAi::deserializeObject(QDataStream&)
{
}
