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
        return std::find(m_Vector.cbegin(), m_Vector.cend(), pos) != m_Vector.cend();
    }
    void remove()
    {
        delete this;
    }
private:
    std::vector<QPoint> m_Vector;
};

class QmlVectorUnit;
typedef oxygine::intrusive_ptr<QmlVectorUnit> spQmlVectorUnit;
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

class QmlVectorBuilding;
typedef oxygine::intrusive_ptr<QmlVectorBuilding> spQmlVectorBuilding;
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

#endif // QMLVECTOR_H
