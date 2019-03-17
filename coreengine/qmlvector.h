#ifndef QMLVECTOR_H
#define QMLVECTOR_H

#include <QPoint>

#include <QObject>

#include <QVector>

#include "game/unit.h"

#include "game/building.h"

#include "game/terrain.h"


class QmlVectorPoint : public QObject
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
    void remove()
    {
        delete this;
    }
private:
    QVector<QPoint> m_Vector;
};

class QmlVectorUnit : public QObject
{
    Q_OBJECT
public:
    explicit QmlVectorUnit();
    virtual ~QmlVectorUnit() = default;
signals:

public slots:
    inline Unit* at(qint32 i) const
    {
        return m_Vector.at(i);
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
private:
    QVector<Unit*> m_Vector;
};

class QmlVectorBuilding: public QObject
{
    Q_OBJECT
public:
    explicit QmlVectorBuilding();
    virtual ~QmlVectorBuilding() = default;
signals:

public slots:
    inline Building* at(qint32 i) const
    {
        return m_Vector.at(i);
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
    QVector<Building*> m_Vector;
};

#endif // QMLVECTOR_H
