#ifndef MAPMOVER_H
#define MAPMOVER_H

#include <QObject>
#include <QTimer>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class InGameMenue;

class MapMover;
typedef oxygine::intrusive_ptr<MapMover> spMapMover;

class MapMover : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit MapMover(InGameMenue* pOwner);
    virtual ~MapMover();
public slots:
    void mouseWheel(qint32 direction);
    void MoveMap(qint32 x, qint32 y);
    virtual void keyInput(oxygine::KeyEvent event);
//    virtual void keyUp(oxygine::KeyEvent event);
    void autoScroll();

private:
    InGameMenue* m_pOwner{nullptr};
    QTimer scrollTimer;
    qint64 m_lastUpdateTimestamp{0};
};

#endif // MAPMOVER_H
