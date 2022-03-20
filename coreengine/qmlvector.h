#ifndef QMLVECTOR_H
#define QMLVECTOR_H

#include <QPoint>
#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "game/unit.h"
#include "game/building.h"
#include "game/terrain.h"

#include "coreengine/globalutils.h"

class QmlVectorPoint;
using spQmlVectorPoint = oxygine::intrusive_ptr<QmlVectorPoint>;

class QmlVectorPoint : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit QmlVectorPoint();
    virtual ~QmlVectorPoint();
    const std::vector<QPoint> & getVector() const
    {
        return m_Vector;
    }

public slots:
    inline QPoint at(qint32 i) const
    {
        return m_Vector[i];
    }
    inline void append(QPoint t)
    {
        m_Vector.push_back(t);
    }
    inline qint32 size() const
    {
        return m_Vector.size();
    }
    bool contains(QPoint pos)
    {
        return GlobalUtils::contains(m_Vector, pos);
    }
    void remove()
    {
        delete this;
    }
private:
    std::vector<QPoint> m_Vector;
};

Q_DECLARE_INTERFACE(QmlVectorPoint, "QmlVectorPoint");

class QmlVectorUnit;
using spQmlVectorUnit = oxygine::intrusive_ptr<QmlVectorUnit>;
class QmlVectorUnit : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit QmlVectorUnit();
    virtual ~QmlVectorUnit();
    const std::vector<spUnit> & getVector() const
    {
        return m_Vector;
    }

public slots:
    inline Unit* at(qint32 i) const
    {
        return m_Vector[i].get();
    }
    inline void append(Unit* t)
    {
        m_Vector.push_back(spUnit(t));
    }
    inline qint32 size() const
    {
        return m_Vector.size();
    }
    void remove()
    {
        delete this;
    }
    void removeAt(qint32 i)
    {
        m_Vector.erase(m_Vector.cbegin() + i);
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
    std::vector<spUnit> m_Vector;
};

Q_DECLARE_INTERFACE(QmlVectorUnit, "QmlVectorUnit");

class QmlVectorBuilding;
using spQmlVectorBuilding = oxygine::intrusive_ptr<QmlVectorBuilding>;
class QmlVectorBuilding: public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit QmlVectorBuilding();
    virtual ~QmlVectorBuilding();
    const std::vector<spBuilding> & getVector() const
    {
        return m_Vector;
    }

public slots:
    inline Building* at(qint32 i) const
    {
        return m_Vector[i].get();
    }
    inline void append(Building* t)
    {
        m_Vector.push_back(spBuilding(t));
    }
    inline qint32 size() const
    {
        return m_Vector.size();
    }
    void remove()
    {
        delete this;
    }
    void removeAt(qint32 i)
    {
        m_Vector.erase(m_Vector.cbegin() + i);
    }
    void randomize();
private:
    std::vector<spBuilding> m_Vector;
};

Q_DECLARE_INTERFACE(QmlVectorBuilding, "QmlVectorBuilding");

#endif // QMLVECTOR_H
