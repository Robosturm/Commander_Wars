#ifndef INGAMEMENUE_H
#define INGAMEMENUE_H

#include <QObject>
#include <QPoint>

#include "oxygine-framework.h"

#include "objects/cursor.h"

class InGameMenue : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:


    explicit InGameMenue(qint32 width = 20, qint32 heigth = 20);
    virtual ~InGameMenue();

signals:
    sigMouseWheel(qint32 direction);
    sigMoveMap(qint32 x, qint32 y);
public slots:
    void mouseWheel(qint32 direction);
    void MoveMap(qint32 x, qint32 y);
protected:
    bool m_moveMap{false};
    QPoint m_MoveMapMousePoint;

    spCursor m_Cursor{new Cursor()};
};

#endif // INGAMEMENUE_H
