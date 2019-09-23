#include "pathfindingsystem.h"

#include "QTime"

#include "coreengine/interpreter.h"

#include "coreengine/mainapp.h"

PathFindingSystem::PathFindingSystem(qint32 startX, qint32 startY)
    : m_StartPoint(startX, startY)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

PathFindingSystem::~PathFindingSystem()
{
    for (qint32 i = 0; i < m_OpenList.size(); i++)
    {
        delete m_OpenList[i];
    }
    for (qint32 i = 0; i < m_ClosedList.size(); i++)
    {
        delete m_ClosedList[i];
    }
}

void PathFindingSystem::setStartPoint(qint32 startX, qint32 startY)
{
    m_StartPoint = QPoint(startX, startY);
}

void PathFindingSystem::setFinishNode()
{
    m_FinishNode = m_ClosedList.size() - 1;
}

void PathFindingSystem::explore()
{
    m_OpenList.append(new Node(m_StartPoint.x(), m_StartPoint.y(), 0, getRemainingCost(m_StartPoint.x(), m_StartPoint.y(), 0)));
    // explore till we reached the end
    while ((m_OpenList.size() > 0))
    {
        Node* pCurrentNode = m_OpenList.last();
        // move node to close list
        m_OpenList.removeLast();
        m_ClosedList.append(pCurrentNode);
        if (finished(pCurrentNode->x, pCurrentNode->y, pCurrentNode->currentCost))
        {
            setFinishNode();
            break;
        }
        for (qint32 i = 0; i < 4; i++)
        {
            qint32 x = pCurrentNode->x;
            qint32 y = pCurrentNode->y;
            // get the surrounding nodes
            switch (i)
            {
                case 0:
                {
                    y--;
                    break;
                }
                case 1:
                {
                    x++;
                    break;
                }
                case 2:
                {
                    y++;
                    break;
                }
                case 3:
                {
                    x--;
                    break;
                }
            }
            bool skipNode = false;
            for (qint32 i2 = 0; i2 < pCurrentNode->previousNodes.size(); i2++)
            {
                // faster than checking the close list :)
                if ((pCurrentNode->previousNodes[i2]->x == x) && (y == pCurrentNode->previousNodes[i2]->y))
                {
                    skipNode = true;
                    break;
                }
            }
            if (!skipNode)
            {
                for (qint32 i2 = 0; i2 < m_OpenList.size(); i2++)
                {
                    if ((m_OpenList.at(i2)->x == x) && (m_OpenList.at(i2)->y == y))
                    {
                        qint32 costs = getCosts(x, y);
                        qint32 remaingCosts = getRemainingCost(x, y, pCurrentNode->currentCost + costs);
                        // update node costs if needed
                        if (pCurrentNode->currentCost + costs < m_OpenList.at(i2)->currentCost)
                        {
                            m_OpenList.at(i2)->remaingCost = remaingCosts;
                            m_OpenList.at(i2)->currentCost = pCurrentNode->currentCost + costs;
                            m_OpenList.at(i2)->previousNodes.push_front(pCurrentNode);
                            Node* nextNode = m_OpenList.at(i2);
                            m_OpenList.removeAt(i2);
                            qint32 myTotalCost = nextNode->currentCost + nextNode->remaingCost;
                            // we expect this to be the best possible node so we try to insert it at the start -> here end of the array
                            bool inserted = false;
                            for (qint32 i3 = m_OpenList.size() - 1; i3 >= 0; i3--)
                            {
                                if ((m_OpenList.at(i3)->currentCost + m_OpenList.at(i3)->remaingCost >= myTotalCost) &&
                                    (m_OpenList.at(i3)->currentCost > nextNode->currentCost))
                                {
                                    // exit
                                    m_OpenList.insert(i3 + 1, nextNode);
                                    inserted = true;
                                    break;
                                }
                            }
                            if (!inserted)
                            {
                                m_OpenList.push_front(nextNode);
                            }
                        }
                        else
                        {
                            m_OpenList.at(i2)->previousNodes.append(pCurrentNode);
                        }
                        skipNode = true;
                        break;
                    }
                }
            }
            if (!skipNode)
            {
                // create the next node
                qint32 costs = getCosts(x, y);
                qint32 remaingCosts = getRemainingCost(x, y, pCurrentNode->currentCost + costs);
                if ((costs >= 0) && (remaingCosts >= 0))
                {
                    Node* nextNode = new Node(x, y, pCurrentNode->currentCost + costs, remaingCosts);
                    // check the next
                    pCurrentNode->nextNodes.append(nextNode);
                    nextNode->previousNodes.append(pCurrentNode);
                    // add node to closed list
                    qint32 myTotalCost = nextNode->currentCost + nextNode->remaingCost;
                    // we expect this to be the best possible node so we try to insert it at the start -> here end of the array
                    bool inserted = false;
                    for (qint32 i3 = m_OpenList.size() - 1; i3 >= 0; i3--)
                    {
                        if ((m_OpenList.at(i3)->currentCost + m_OpenList.at(i3)->remaingCost >= myTotalCost) &&
                            (m_OpenList.at(i3)->currentCost > nextNode->currentCost))
                        {
                            // exit
                            m_OpenList.insert(i3 + 1, nextNode);
                            inserted = true;
                            break;
                        }
                    }
                    if (!inserted)
                    {
                        m_OpenList.push_front(nextNode);
                    }
                }
            }
        }
    }
    if (m_FinishNode < 0)
    {
        setFinishNode();
    }
}

QVector<QPoint> PathFindingSystem::getFields(qint32 startX, qint32 startY, qint32 min, qint32 max)
{
    QVector<QPoint> points;
    for (qint32 x = -max; x <= max; x++)
    {
        for (qint32 y = -max; y <= max; y++)
        {
            if ((qAbs(x) + qAbs(y) >= min) && (qAbs(x) + qAbs(y) <= max))
            {
                points.append(QPoint(startX + x, startY + y));
            }
        }
    }
    return points;
}

QVector<QPoint> PathFindingSystem::getAllNodePoints()
{
    QVector<QPoint> points;
    for (qint32 i = 0; i < m_ClosedList.size(); i++)
    {
        points.append(QPoint(m_ClosedList[i]->x, m_ClosedList[i]->y));
    }
    return points;
}

QmlVectorPoint* PathFindingSystem::getAllQmlVectorPoints()
{
    QmlVectorPoint* ret = new QmlVectorPoint();
    for (qint32 i = 0; i < m_ClosedList.size(); i++)
    {
        ret->append(QPoint(m_ClosedList[i]->x, m_ClosedList[i]->y));
    }
    return ret;
}

QVector<QPoint> PathFindingSystem::getTargetPath()
{
    if (m_FinishNode >= 0)
    {
        return getPath(m_ClosedList[m_FinishNode]->x, m_ClosedList[m_FinishNode]->y);
    }
    return QVector<QPoint>();
}

qint32 PathFindingSystem::getNodeIndex(qint32 x, qint32 y)
{
    for (qint32 i = 0; i < m_ClosedList.size(); i++)
    {
        if ((m_ClosedList[i]->x == x) && (m_ClosedList[i]->y == y))
        {
            return i;
        }
    }
    return -1;
}

QVector<QPoint> PathFindingSystem::getPath(qint32 x, qint32 y)
{
    QVector<QPoint> points;
    for (qint32 i = 0; i < m_ClosedList.size(); i++)
    {
        if ((m_ClosedList[i]->x == x) && (m_ClosedList[i]->y == y))
        {
            points.append(QPoint(m_ClosedList[i]->x, m_ClosedList[i]->y));
            Node* pNode = m_ClosedList[i];
            while (pNode->previousNodes.size() > 0)
            {
                // use a random node?
                pNode = pNode->previousNodes[0];
                points.append(QPoint(pNode->x, pNode->y));
            }
            return points;
        }
    }
    return points;
}

qint32 PathFindingSystem::getTargetCosts(qint32 x, qint32 y)
{
    for (qint32 i = 0; i < m_ClosedList.size(); i++)
    {
        if ((m_ClosedList[i]->x == x) && (m_ClosedList[i]->y == y))
        {
            return m_ClosedList[i]->currentCost;
        }
    }
    return -1;
}

bool PathFindingSystem::isReachable(qint32 x, qint32 y)
{
    for (qint32 i = 0; i < m_ClosedList.size(); i++)
    {
        if ((m_ClosedList[i]->x == x) && (m_ClosedList[i]->y == y))
        {
            return true;
        }
    }
    return false;
}
