#include "pathfindingsystem.h"

PathFindingSystem::PathFindingSystem()
{

}

QSharedPointer<QVector<QPoint>> PathFindingSystem::getFields(qint32 min, qint32 max)
{
    QSharedPointer<QVector<QPoint>> points(new QVector<QPoint>);
    for (qint32 x = -max; x <= max; x++)
    {
        for (qint32 y = -max; y <= max; y++)
        {
            if ((qAbs(x) + qAbs(y) >= min) && (qAbs(x) + qAbs(y) <= max))
            {
                points->append(QPoint(x, y));
            }
        }
    }
    return points;
}
