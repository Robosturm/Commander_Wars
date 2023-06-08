#ifndef QMLVECTOR_H
#define QMLVECTOR_H

#include <QPoint>
#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"

#include "game/unit.h"
#include "game/building.h"

#include "coreengine/globalutils.h"

class QmlVectorPoint;
using spQmlVectorPoint = oxygine::intrusive_ptr<QmlVectorPoint>;

class QmlVectorPoint final : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit QmlVectorPoint();
   virtual ~QmlVectorPoint() = default;
    const std::vector<QPoint> & getVector() const
    {
        return m_Vector;
    }
    Q_INVOKABLE inline QPoint at(qint32 i) const
    {
        return m_Vector[i];
    }
    Q_INVOKABLE inline void append(QPoint t)
    {
        m_Vector.push_back(t);
    }
    Q_INVOKABLE inline qint32 size() const
    {
        return m_Vector.size();
    }
    Q_INVOKABLE bool contains(QPoint pos)
    {
        return GlobalUtils::contains(m_Vector, pos);
    }
    Q_INVOKABLE void remove()
    {
    }
private:
    std::vector<QPoint> m_Vector;
};

Q_DECLARE_INTERFACE(QmlVectorPoint, "QmlVectorPoint");

class QmlVectorUnit;
using spQmlVectorUnit = oxygine::intrusive_ptr<QmlVectorUnit>;
class QmlVectorUnit final : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit QmlVectorUnit();
   virtual ~QmlVectorUnit() = default;
    const std::vector<spUnit> & getVector() const
    {
        return m_Vector;
    }
    /**
     * @brief pruneEnemies
     * @param pOwnUnits
     * @param distanceMultiplier
     */
    Q_INVOKABLE void pruneEnemies(const QmlVectorUnit * pOwnUnits, double distanceMultiplier);

    Q_INVOKABLE inline Unit* at(qint32 i) const
    {
        return m_Vector[i].get();
    }
    Q_INVOKABLE inline void append(Unit* t)
    {
        m_Vector.push_back(spUnit(t));
    }
    Q_INVOKABLE inline qint32 size() const
    {
        return m_Vector.size();
    }
    Q_INVOKABLE void remove()
    {
    }
    Q_INVOKABLE void removeAt(qint32 i)
    {
        m_Vector.erase(m_Vector.cbegin() + i);
    }
    Q_INVOKABLE void randomize();
    /**
     * @brief sortExpensive most expensive units are sorted in first
     */
    Q_INVOKABLE void sortExpensive();
    /**
     * @brief sortShortestMovementRange
     */
    Q_INVOKABLE void sortShortestMovementRange(bool infantriesLast);
    /**
     * @brief QmlVectorUnit::getUnitCount
     * @param unitId
     * @return
     */
    Q_INVOKABLE qint32 getUnitCount(const QString unitId);
private:
    std::vector<spUnit> m_Vector;
};

Q_DECLARE_INTERFACE(QmlVectorUnit, "QmlVectorUnit");

class QmlVectorBuilding;
using spQmlVectorBuilding = oxygine::intrusive_ptr<QmlVectorBuilding>;
class QmlVectorBuilding final : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit QmlVectorBuilding();
   virtual ~QmlVectorBuilding() = default;
    const std::vector<spBuilding> & getVector() const
    {
        return m_Vector;
    }
    /**
     * @brief sortClosestToEnemy
     * @param pEnemyUnits
     */
    void sortClosestToEnemy(const spQmlVectorUnit & pEnemyUnits);
    /**
     * @brief pruneEnemieBuildings
     * @param pOwnUnits
     * @param distanceMultiplier
     */
    void pruneEnemieBuildings(const spQmlVectorUnit & pOwnUnits, qint32 distanceMultiplier);

    Q_INVOKABLE inline Building* at(qint32 i) const
    {
        return m_Vector[i].get();
    }
    Q_INVOKABLE inline void append(Building* t)
    {
        m_Vector.push_back(spBuilding(t));
    }
    Q_INVOKABLE inline qint32 size() const
    {
        return m_Vector.size();
    }
    Q_INVOKABLE qint32 getBuildingCount(const QString buildingId);
    Q_INVOKABLE qint32 getBuildingGroupCount(const QStringList buildingIds, bool onlyEmpty);
    Q_INVOKABLE void remove()
    {
    }
    Q_INVOKABLE void removeAt(qint32 i)
    {
        m_Vector.erase(m_Vector.cbegin() + i);
    }
    Q_INVOKABLE void randomize();
private:
    std::vector<spBuilding> m_Vector;
};

Q_DECLARE_INTERFACE(QmlVectorBuilding, "QmlVectorBuilding");

#endif // QMLVECTOR_H
