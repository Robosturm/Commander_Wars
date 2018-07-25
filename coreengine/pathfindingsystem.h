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
    explicit PathFindingSystem();

signals:

public slots:

    /**
     * @brief getFields searches for all fields in the range of min and max ignoring all movement costs
     * @param min minimum search range
     * @param max maximum search range
     * @return shared pointer to the points
     */
    static QSharedPointer<QVector<QPoint>> getFields(qint32 min, qint32 max);
};

#endif // PATHFINDINGSYSTEM_H
