#include "coreengine/qmlvector.h"
#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"

#include "ai/coreai.h"

QmlVectorPoint::QmlVectorPoint()
    : QObject()
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
    : QObject()
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

void QmlVectorUnit::sortUnitsFarFromEnemyFirst(spQmlVectorUnit & pEnemyUnits)
{
    auto & pEnemyUnitsVec = pEnemyUnits->getUnits();
    for (auto & pUnit : m_Vector)
    {
        qint32 distance = std::numeric_limits<qint32>::max();
        for (auto & pEnemy : pEnemyUnitsVec)
        {
            qint32 newDistance = GlobalUtils::getDistance(pEnemy->getPosition(), pUnit->getPosition());
            if (newDistance < distance)
            {
                distance = newDistance;
            }
        }
        pUnit->setSortValues({pUnit->getMovementpoints(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY())),
                             pUnit->getActionList().contains(CoreAI::ACTION_CAPTURE),
                             distance});
    }
    std::sort(m_Vector.begin(), m_Vector.end(), [](const spUnit& lhs, const spUnit& rhs)
    {
        auto & lhsVec = lhs->getSortValues();
        auto & rhsVec = rhs->getSortValues();
        if (rhsVec[1] == lhsVec[1])
        {
            if (lhsVec[2] == rhsVec[2])
            {
                return lhsVec[0] < rhsVec[0];
            }
            else
            {
                return lhsVec[2] > rhsVec[2];
            }
        }
        else
        {
            return lhsVec[1] < rhsVec[1];
        }
    });
}

QmlVectorBuilding::QmlVectorBuilding()
    : QObject()
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
