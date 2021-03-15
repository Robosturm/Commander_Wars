#ifndef QMLVECTOR_H
#define QMLVECTOR_H

#include <QPoint>
#include <QObject>
#include <QVector>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "game/unit.h"
#include "game/building.h"
#include "game/terrain.h"

class QmlVectorPoint;
typedef oxygine::intrusive_ptr<QmlVectorPoint> spQmlVectorPoint;

class QmlVectorPoint : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit QmlVectorPoint();
    virtual ~QmlVectorPoint() = default;
signals:

public slots:
    inline QPoint at(qint32 i) const
    {
        return m_Vector.at(i);
    }
    inline void append(QPoint t)
    {
        m_Vector.append(t);
    }
    inline qint32 size() const
    {
        return m_Vector.size();
    }
    bool contains(QPoint pos)
    {
        return m_Vector.contains(pos);
    }
    void remove()
    {
        delete this;
    }
private:
    QVector<QPoint> m_Vector;
};

class QmlVectorUnit;
typedef oxygine::intrusive_ptr<QmlVectorUnit> spQmlVectorUnit;
class QmlVectorUnit : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit QmlVectorUnit();
    virtual ~QmlVectorUnit() = default;
signals:

public slots:
    inline Unit* at(qint32 i) const
    {
        return m_Vector.at(i).get();
    }
    inline void append(Unit* t)
    {
        m_Vector.append(t);
    }
    inline qint32 size() const
    {
        return m_Vector.size();
    }
    void remove()
    {
        delete this;
    }
    void randomize();
    /**
     * @brief sortExpensive most expensive units are sorted in first
     */
    void sortExpensive();
    /**
     * @brief sortShortestMovementRange
     */
    void sortShortestMovementRange(bool infantriesLast);
private:
    QVector<spUnit> m_Vector;
};

class QmlVectorBuilding;
typedef oxygine::intrusive_ptr<QmlVectorBuilding> spQmlVectorBuilding;
class QmlVectorBuilding: public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit QmlVectorBuilding();
    virtual ~QmlVectorBuilding() = default;
signals:

public slots:
    inline Building* at(qint32 i) const
    {
        return m_Vector.at(i).get();
    }
    inline void append(Building* t)
    {
        m_Vector.append(t);
    }
    inline qint32 size() const
    {
        return m_Vector.size();
    }
    void remove()
    {
        delete this;
    }
    void randomize();
private:
    QVector<spBuilding> m_Vector;
};

#endif // QMLVECTOR_H
