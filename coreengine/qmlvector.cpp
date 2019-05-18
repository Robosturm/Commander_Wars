#include "qmlvector.h"

#include "coreengine/mainapp.h"

QmlVectorPoint::QmlVectorPoint()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

QmlVectorUnit::QmlVectorUnit()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void QmlVectorUnit::randomize()
{
    QVector<Unit*> randVector;
    while (m_Vector.size() > 0)
    {
        qint32 value = Mainapp::randInt(0, m_Vector.size() - 1);
        randVector.push_back(m_Vector[value]);
        m_Vector.removeAt(value);
    }
    m_Vector.swap(randVector);
}

void QmlVectorUnit::sortExpensive()
{
    QVector<Unit*> sortedVector;
    QVector<qint32> costs;
    for (qint32 i = 0; i < m_Vector.size(); i++)
    {
        costs.append(m_Vector[i]->getUnitValue());
    }
    while (m_Vector.size() > 0)
    {
        qint32 item = 0;
        qint32 value = 0;
        for (qint32 i = 0; i < m_Vector.size(); i++)
        {
            if (costs[i] > value)
            {
                item = i;
                value = costs[i];
            }
        }
        sortedVector.append(m_Vector[item]);
        m_Vector.removeAt(item);
        costs.removeAt(item);
    }
    m_Vector.swap(sortedVector);
}

QmlVectorBuilding::QmlVectorBuilding()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void QmlVectorBuilding::randomize()
{
    QVector<Building*> randVector;
    while (m_Vector.size() > 0)
    {
        qint32 value = Mainapp::randInt(0, m_Vector.size() - 1);
        randVector.push_back(m_Vector[value]);
        m_Vector.removeAt(value);
    }
    m_Vector.swap(randVector);
}
