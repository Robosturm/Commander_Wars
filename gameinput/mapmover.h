#ifndef MAPMOVER_H
#define MAPMOVER_H

#include <QObject>
#include <QTimer>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class BaseGamemenu;
class MapMover;
using spMapMover = oxygine::intrusive_ptr<MapMover>;

class MapMover : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
    enum Keys
    {
        Up,
        Down,
        Left,
        Right,
        MoveMapUp,
        MoveMapDown,
        MoveMapLeft,
        MoveMapRight,
        ZoomIn,
        ZoomOut,
        Max
    };
public:
    explicit MapMover(BaseGamemenu* pOwner);
    virtual ~MapMover() = default;
public slots:
    void mouseWheel(float direction);
    void autoScroll();
protected slots:
    virtual void keyInput(oxygine::KeyEvent event);
private:
    BaseGamemenu* m_pOwner{nullptr};
    QTimer m_scrollTimer;
    qint64 m_lastUpdateTimestamp[Max] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

#endif // MAPMOVER_H
