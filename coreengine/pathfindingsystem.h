#ifndef PATHFINDINGSYSTEM_H
#define PATHFINDINGSYSTEM_H

#include <QObject>
#include <QSharedPointer>
#include <QVector>
#include <QPoint>

class PathFindingSystem : public QObject
{
    Q_OBJECT
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
    };
    /**
     * @brief PathFindingSystem
     * @param startX start x for the node
     * @param startY start y for the node
     */
    explicit PathFindingSystem(qint32 startX, qint32 startY);
    virtual ~PathFindingSystem();
    /**
     * @brief getRemainingCost
     * @param x
     * @param y
     * @return the remaining costs for this node to reach the target
     */
    virtual qint32 getRemainingCost(qint32 x, qint32 y) = 0;
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
     * @brief explores the map until the open list is empty or finished returned true.
     * This will also add the start point to the open list
     */
    void explore();
    /**
     * @brief get the Path to the given field as vector of qpoints. an empty vector means unreachable
     * @param x
     * @param y
     */
    QVector<QPoint> getPath(qint32 x, qint32 y);
    /**
     * @brief getClosedList pointer to all closed nodes -> found nodes
     * @return
     */
    inline const QVector<Node*>* getClosedList() const
    {
        return &m_ClosedList;
    }
    /**
     * @brief getAllNodePoints returns all reachable fields in a point vector
     * @return
     */
    QVector<QPoint> getAllNodePoints();
protected:
    QPoint m_StartPoint;
    /**
     * @brief closedList nodes already found
     */
    QVector<Node*> m_ClosedList;
    /**
     * @brief m_OpenList sorted list of nodes we need to check. The last item is the next item we gonna check. this should avoid a lot of resizing of the array
     */
    QVector<Node*> m_OpenList;


signals:

public slots:

    /**
     * @brief getFields searches for all fields in the range of min and max ignoring all movement costs
     * @param min minimum search range
     * @param max maximum search range
     * @return shared pointer to the points
     */
    static QVector<QPoint> getFields(qint32 startX, qint32 startY, qint32 min, qint32 max);
};

#endif // PATHFINDINGSYSTEM_H
