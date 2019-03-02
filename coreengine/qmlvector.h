#ifndef QMLVECTOR_H
#define QMLVECTOR_H

#include <QPoint>

#include <QObject>

#include <QVector>

#include "game/unit.h"


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
    explicit QmlVectorUnit() = default;
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
#endif // QMLVECTOR_H
