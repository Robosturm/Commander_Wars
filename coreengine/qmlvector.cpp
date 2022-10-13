#include "coreengine/qmlvector.h"
#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"

#include "ai/coreai.h"

QmlVectorPoint::QmlVectorPoint()
{
#ifdef GRAPHICSUPPORT
    setObjectName("QmlVectorPoint");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    Interpreter::getInstance()->trackJsObject(this);
}

QmlVectorUnit::QmlVectorUnit()
{
#ifdef GRAPHICSUPPORT
    setObjectName("QmlVectorUnit");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
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

qint32 QmlVectorUnit::getUnitCount(const QString & unitId)
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

QmlVectorBuilding::QmlVectorBuilding()
{
#ifdef GRAPHICSUPPORT
    setObjectName("QmlVectorBuilding");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
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

qint32 QmlVectorBuilding::getBuildingCount(const QString & buildingId)
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
