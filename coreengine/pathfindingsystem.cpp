#include "pathfindingsystem.h"

PathFindingSystem::PathFindingSystem(qint32 startX, qint32 startY)
    : m_StartPoint(startX, startY)
{

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

void PathFindingSystem::explore()
{
    bool finshed = false;
    m_OpenList.append(new Node(m_StartPoint.x(), m_StartPoint.y(), 0, getRemainingCost(m_StartPoint.x(), m_StartPoint.y())));
    // explore till we reached the end
    while ((m_OpenList.size() > 0) && (finshed == false))
    {
        Node* pCurrentNode = m_OpenList.last();
        // move node to close list
        m_OpenList.removeLast();
        m_ClosedList.append(pCurrentNode);
        for (qint32 i = 0; i < 3; i++)
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
                    y++;
                    x++;
                    break;
                }
                case 2:
                {
                    y++;
                    x--;
                    break;
                }
                case 3:
                {
                    x--;
                    y--;
                    break;
                }
            }
            if (pCurrentNode->previousNode != nullptr)
            {
                // faster than checking the close list :)
                if ((pCurrentNode->previousNode->x) == x && (y == pCurrentNode->previousNode->y == y))
                {
                    // skip node
                    continue;
                }
            }
            bool skipNode = false;
            // check the closed list next :(
            for (qint32 i2 = 0; i2 < m_ClosedList.size(); i2++)
            {
                if ((m_ClosedList.at(i2)->x == x) && (m_ClosedList.at(i2)->y == y))
                {
                    skipNode = true;
                    break;
                }
            }
            if (!skipNode)
            {
                // create the next node
                qint32 costs = getCosts(x, y);
                if (costs >= 0)
                {
                    Node* nextNode = new Node(x, y, pCurrentNode->currentCost + costs, getRemainingCost(x, y));
                    // check the next
                    pCurrentNode->nextNodes.append(nextNode);
                    nextNode->previousNode = pCurrentNode;
                    // add node to closed list
                    qint32 myTotalCost = nextNode->currentCost + nextNode->remaingCost;
                    // we expect this to be the best possible node so we try to insert it at the start -> here end of the array
                    for (qint32 i3 = m_OpenList.size() - 1; i3 >= 0; i3--)
                    {
                        if (m_OpenList.at(i3)->currentCost + m_OpenList.at(i3)->remaingCost > myTotalCost)
                        {
                            // exit
                            m_OpenList.insert(i3 + 1, pCurrentNode);
                            break;
                        }
                    }
                }
            }
        }
    }
}

QVector<QPoint> PathFindingSystem::getFields(qint32 min, qint32 max)
{
    QVector<QPoint> points;
    for (qint32 x = -max; x <= max; x++)
    {
        for (qint32 y = -max; y <= max; y++)
        {
            if ((qAbs(x) + qAbs(y) >= min) && (qAbs(x) + qAbs(y) <= max))
            {
                points.append(QPoint(x, y));
            }
        }
    }
    return points;
}
