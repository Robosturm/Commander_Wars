#ifndef DIJKSTRAMAP_H
#define DIJKSTRAMAP_H

#include <QVector>
#include <QPoint>

template<typename NodeData>
class DijkstraMap
{
public:
    /**
     * @brief node we want to check or checked already
     */
    struct Node
    {
        Node(qint32 x, qint32 y, qint32 cost, qint32 remaingCost)
            : x(x),
              y(y),
              currentCost(cost),
              remaingCost(remaingCost)
        {
        }
        /**
         * @brief x coordinates of this node
         */
        qint32 x;
        /**
         * @brief y coordinates of this node
         */
        qint32 y;
        /**
         * @brief currentCost minimum costs needed to reach this field
         */
        qint32 currentCost;
        /**
         * @brief remaingCost costs left to reach this field
         */
        qint32 remaingCost;
        /**
         * @brief nextNodes nodes we can go to
         */
        QVector<Node*> nextNodes;
        /**
         * @brief previousNode node we come from
         */
        QVector<Node*> previousNodes;

        NodeData pNodeData;
    };

    explicit DijkstraMap(qint32 width, qint32 heigth)
        : m_Map(width, QVector<Node*>(heigth))
    {
        for (qint32 x = 0; x < width; x++)
        {
            for (qint32 y = 0; y < heigth; y++)
            {
                m_Map[x][y] = nullptr;
            }
        }
    }
    virtual ~DijkstraMap()
    {
        for (qint32 i = 0; i < m_OpenList.size(); i++)
        {
            delete m_OpenList[i];
        }
        for (qint32 x = 0; x < m_Map.size(); x++)
        {
            for (qint32 y = 0; y < m_Map[x].size(); y++)
            {
                delete m_Map[x][y];
            }
        }
    }
    /**
     * @brief addStart
     * @param x
     * @param y
     */
    void addStart(qint32 x, qint32 y, qint32 startCosts)
    {
        Node* pNode = new Node(x, y, startCosts, getRemainingCost(x, y, startCosts));
        bool inserted = false;
        for (qint32 i3 = 0; i3 < m_OpenList.size(); i3++)
        {
            if (m_OpenList.at(i3)->currentCost <= pNode->currentCost)
            {
                // exit
                m_OpenList.insert(i3, pNode);
                inserted = true;
                break;
            }
        }
        if (!inserted)
        {
            m_OpenList.push_front(pNode);
        }
    }
    /**
     * @brief getRemainingCost
     * @param x
     * @param y
     * @return the remaining costs for this node to reach the target
     */
    virtual qint32 getRemainingCost(qint32 x, qint32 y, qint32 currentCost) = 0;
    /**
     * @brief finished checks if this would be the target node to reach
     * @param x
     * @param y
     * @return
     */
    virtual bool finished(qint32 x, qint32 y) = 0;
    /**
     * @brief getCosts
     * @param x
     * @param y
     * @return the exact costs needed to get onto the given field. -1 = unreachable
     */
    virtual qint32 getCosts(qint32 x, qint32 y) = 0;
    /**
     * @brief addNodeData
     */
    virtual void addNodeData(Node*){}
    /**
     * @brief explores the map until the open list is empty or finished returned true.
     * This will also add the start point to the open list
     */
    void explore()
    {
        bool finshed = false;
        // explore till we reached the end
        while ((m_OpenList.size() > 0) && (finshed == false))
        {
            Node* pCurrentNode = m_OpenList.last();
            // move node to close list
            m_OpenList.removeLast();
            m_Map[pCurrentNode->x, pCurrentNode->y] = pCurrentNode;
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
                    if (m_Map[x][y] != nullptr)
                    {

                        skipNode = true;
                        addNodeData(m_Map[x][y]);
                        m_Map[x][y]->previousNodes.append(pCurrentNode);
                        pCurrentNode->nextNodes.append(m_Map[x][y]);
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
                        addNodeData(nextNode);
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
                                (m_OpenList.at(i3)->currentCost >= nextNode->currentCost))
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
    }
    /**
     * @brief get the Path to the given field as vector of qpoints. an empty vector means unreachable
     * @param x
     * @param y
     * @return the first point is the target and the last poin is the start
     */
    QVector<QPoint> getPath(qint32 x, qint32 y)
    {
        QVector<QPoint> points;
        if (m_Map[x][y] != nullptr)
        {
            points.append(QPoint(x, y));
            Node* pNode = m_Map[x][y];
            while (pNode->previousNodes.size() > 0)
            {
                // use a random node?
                pNode = pNode->previousNodes[0];
                points.append(QPoint(pNode->x, pNode->y));
            }
            return points;
        }
        return points;
    }
    /**
     * @brief isReachable
     * @param x
     * @param y
     * @return if the given field is reachable
     */
    bool isReachable(qint32 x, qint32 y)
    {
        if (m_Map[x][y] != nullptr)
        {
            return true;
        }
        return false;
    }
private:
    /**
     * @brief m_OpenList sorted list of nodes we need to check. The last item is the next item we gonna check. this should avoid a lot of resizing of the array
     */
    QList<Node*> m_OpenList;
    QVector<QVector<Node*>> m_Map;
};

#endif // DIJKSTRAMAP_H
