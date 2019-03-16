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

    explicit InGameMenue();
    explicit InGameMenue(qint32 width, qint32 heigth, QString map = "");
    virtual ~InGameMenue();

    Cursor* getCursor();
signals:
    void sigMouseWheel(qint32 direction);
    void sigMoveMap(qint32 x, qint32 y);
public slots:
    void mouseWheel(qint32 direction);
    void MoveMap(qint32 x, qint32 y);

protected:
    void loadBackground();
    void loadHandling();
    bool m_moveMap{false};
    QPoint m_MoveMapMousePoint;

    spCursor m_Cursor{new Cursor()};
};

#endif // INGAMEMENUE_H
