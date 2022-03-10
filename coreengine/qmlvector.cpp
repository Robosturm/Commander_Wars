#include "coreengine/qmlvector.h"
#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"

#include "ai/coreai.h"

QmlVectorPoint::QmlVectorPoint()
{
    setObjectName("QmlVectorPoint");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    oxygine::ref_counter::addInstanceCounter();
}

QmlVectorPoint::~QmlVectorPoint()
{
    oxygine::ref_counter::releaseInstanceCounter();
}

QmlVectorUnit::QmlVectorUnit()
{
    setObjectName("QmlVectorUnit");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    oxygine::ref_counter::addInstanceCounter();
}

QmlVectorUnit::~QmlVectorUnit()
{
    oxygine::ref_counter::releaseInstanceCounter();
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

QmlVectorBuilding::QmlVectorBuilding()
{
    setObjectName("QmlVectorBuilding");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    oxygine::ref_counter::addInstanceCounter();
}

QmlVectorBuilding::~QmlVectorBuilding()
{
    oxygine::ref_counter::releaseInstanceCounter();
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
