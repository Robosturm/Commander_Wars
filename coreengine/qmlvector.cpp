#include "coreengine/qmlvector.h"
#include "coreengine/globalutils.h"
#include "coreengine/interpreter.h"
#include "game/terrain.h"

#include "ai/coreai.h"

QmlVectorPoint::QmlVectorPoint()
{
#ifdef GRAPHICSUPPORT
    setObjectName("QmlVectorPoint");
#endif
    Interpreter::setCppOwnerShip(this);
    Interpreter::getInstance()->trackJsObject(this);
}

QmlVectorUnit::QmlVectorUnit()
{
#ifdef GRAPHICSUPPORT
    setObjectName("QmlVectorUnit");
#endif
    Interpreter::setCppOwnerShip(this);
    Interpreter::getInstance()->trackJsObject(this);
}

void QmlVectorUnit::randomize()
{
    for (auto & unit : m_Vector)
    {
        unit->setSortValues({GlobalUtils::randInt(0, std::numeric_limits<qint32>::max() - 3)});
    }
    std::sort(m_Vector.begin(), m_Vector.end(), [](const spUnit& lhs, const spUnit& rhs)
    {
        return lhs->getSortValues()[0] > rhs->getSortValues()[0];
    });
}

void QmlVectorUnit::sortExpensive()
{
    for (auto & unit : m_Vector)
    {
        unit->setSortValues({unit->getCoUnitValue()});
    }
    std::sort(m_Vector.begin(), m_Vector.end(), [](const spUnit& lhs, const spUnit& rhs)
    {
        return lhs->getSortValues()[0] > rhs->getSortValues()[0];
    });
}

void QmlVectorUnit::sortShortestMovementRange(bool infantriesLast)
{
    if (infantriesLast)
    {
        for (auto & unit : m_Vector)
        {
            unit->setSortValues({unit->getMovementpoints(QPoint(unit->Unit::getX(), unit->Unit::getY())),
                                 unit->getActionList().contains(CoreAI::ACTION_CAPTURE)});
        }
        std::sort(m_Vector.begin(), m_Vector.end(), [](const spUnit& lhs, const spUnit& rhs)
        {
            auto & lhsVec = lhs->getSortValues();
            auto & rhsVec = rhs->getSortValues();
            if (rhsVec[1] == lhsVec[1])
            {
                return lhsVec[0] < rhsVec[0];
            }
            else
            {
                return lhsVec[1] < rhsVec[1];
            }
        });
    }
    else
    {
        for (auto & unit : m_Vector)
        {
            unit->setSortValues({unit->getMovementpoints(QPoint(unit->Unit::getX(), unit->Unit::getY()))});
        }
        std::sort(m_Vector.begin(), m_Vector.end(), [](const spUnit& lhs, const spUnit& rhs)
        {
            return lhs->getSortValues()[0] < rhs->getSortValues()[0];
        });
    }
}

qint32 QmlVectorUnit::getUnitCount(const QString unitId)
{
    qint32 count = 0;
    for (auto & unit : m_Vector)
    {
        if (unit->getUnitID() == unitId)
        {
            ++count;
        }
    }
    return count;
}

void QmlVectorUnit::pruneEnemies(const QmlVectorUnit * pOwnUnits, double distanceMultiplier)
{
    qint32 i = 0;
    while (i < m_Vector.size())
    {
        qint32 movepoints = m_Vector[i]->getBaseMovementPoints();
        QPoint position = m_Vector[i]->getMapPosition();
        bool inRange = false;
        for (auto & ownUnit : pOwnUnits->getVector())
        {
            auto distance = GlobalUtils::getDistance(ownUnit->getMapPosition(), position);
            if (distance < distanceMultiplier * (movepoints + ownUnit->getBaseMovementPoints()))
            {
                inRange = true;
                break;
            }
        }
        if (inRange)
        {
            ++i;
        }
        else
        {
            removeAt(i);
        }
    }
}

QmlVectorBuilding::QmlVectorBuilding()
{
#ifdef GRAPHICSUPPORT
    setObjectName("QmlVectorBuilding");
#endif
    Interpreter::setCppOwnerShip(this);
    Interpreter::getInstance()->trackJsObject(this);
}

void QmlVectorBuilding::randomize()
{
    for (auto & building : m_Vector)
    {
        building->setSortValues({GlobalUtils::randInt(0, std::numeric_limits<qint32>::max() - 3)});
    }
    std::sort(m_Vector.begin(), m_Vector.end(), [](const spBuilding& lhs, const spBuilding& rhs)
    {
        return lhs->getSortValues()[0] > rhs->getSortValues()[0];
    });
}

qint32 QmlVectorBuilding::getBuildingCount(const QString buildingId)
{
    qint32 count = 0;
    for (auto & building : m_Vector)
    {
        if (building->getBuildingID() == buildingId)
        {
            ++count;
        }
    }
    return count;
}

qint32 QmlVectorBuilding::getBuildingGroupCount(const QStringList buildingIds, bool onlyEmpty)
{
    qint32 count = 0;
    for (auto & building : m_Vector)
    {
        if ((!onlyEmpty ||
            (onlyEmpty && building->getTerrain()->getUnit() == nullptr)) &&
            buildingIds.contains(building->getBuildingID()))
        {
            ++count;
        }
    }
    return count;
}

void QmlVectorBuilding::sortClosestToEnemy(const spQmlVectorUnit & pEnemyUnits)
{
    for (qint32 i2 = 0; i2 < m_Vector.size(); ++i2)
    {
        if (i2 == 0)
        {
            m_Vector[i2]->setSortValues({std::numeric_limits<qint32>::max()});
        }
        for (qint32 i = 0; i < pEnemyUnits->size(); ++i)
        {
            qint32 distance = GlobalUtils::getDistance(m_Vector[i2]->getPosition(), pEnemyUnits->at(i)->getMapPosition());
            if (distance < m_Vector[i2]->getSortValues()[0])
            {
                m_Vector[i2]->setSortValues({distance});
            }
        }
    }
    std::sort(m_Vector.begin(), m_Vector.end(), [](const spBuilding& lhs, const spBuilding& rhs)
    {
        return lhs->getSortValues()[0] < rhs->getSortValues()[0];
    });
}

void QmlVectorBuilding::pruneEnemieBuildings(const spQmlVectorUnit & pOwnUnits, qint32 distanceMultiplier)
{
    qint32 i = 0;
    while (i < m_Vector.size())
    {
        QPoint position = m_Vector[i]->getPosition();
        bool inRange = false;
        for (auto & ownUnit : pOwnUnits->getVector())
        {
            qint32 movepoints = ownUnit->getBaseMovementPoints();
            auto distance = GlobalUtils::getDistance(ownUnit->getMapPosition(), position);
            if (distance < distanceMultiplier * movepoints)
            {
                inRange = true;
                break;
            }
        }
        if (inRange)
        {
            ++i;
        }
        else
        {
            removeAt(i);
        }
    }
}
