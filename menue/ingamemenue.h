#ifndef INGAMEMENUE_H
#define INGAMEMENUE_H

#include <QPoint>
#include <qrect.h>
#include <QTimer>
#include <QThread>

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"

#include "game/cursor.h"
#include "menue/basemenu.h"

#include "gameinput/mapmover.h"

class GameMap;
using spGameMap = oxygine::intrusive_ptr<GameMap>;
class InGameMenue;
using spInGameMenue = oxygine::intrusive_ptr<InGameMenue>;

class InGameMenue : public Basemenu
{
    Q_OBJECT
public:
    explicit InGameMenue(spGameMap pMap);
    explicit InGameMenue(qint32 width, qint32 heigth, QString map, bool savegame);
    virtual ~InGameMenue();
    void deleteMenu();

    Cursor* getCursor();
    void calcNewMousePosition(qint32 x, qint32 y);
    QPoint getMousePos(qint32 x, qint32 y);
    void MoveMap(qint32 x, qint32 y);
    virtual void autoScroll(QPoint cursorPosition);
    void initSlidingActor(qint32 x, qint32 y, qint32 width, qint32 height);
    void updateSlidingActorSize();
    oxygine::spActor getMapSlidingActor() const;
    oxygine::spSlidingActorNoClipRect getMapSliding() const;
    static InGameMenue* getMenuInstance()
    {
        return m_pInstance.get();
    }

signals:
    void sigMouseWheel(float direction);
    void sigRightClick(qint32 x, qint32 y);
    void sigRightClickDown(qint32 x, qint32 y);
    void sigRightClickUp(qint32 x, qint32 y);
    void sigLeftClick(qint32 x, qint32 y);
    void sigLeftClickDown(qint32 x, qint32 y);
    void sigLeftClickUp(qint32 x, qint32 y);
    void sigMouseMove(qint32 x, qint32 y);
public slots:
    GameMap* getMap() const;
    virtual void keyInput(oxygine::KeyEvent event);
    virtual void keyUp(oxygine::KeyEvent event);
    void centerMapOnCursor();
    void changeBackground(QString background);
    virtual void setFocused(bool Focused) override;
protected:
    void loadBackground();
    void loadHandling();
    void connectMapCursor();
    bool m_moveMap{false};
    QPoint m_MoveMapMousePoint;

    spCursor m_Cursor;
    QRect m_autoScrollBorder{50, 50, 300, 50};

    QThread m_MapMoveThread;
    spMapMover m_MapMover;
    oxygine::spSprite m_backgroundSprite;

    oxygine::spSlidingActorNoClipRect m_mapSliding;
    oxygine::spActor m_mapSlidingActor;

    QPoint m_lastMapPoint{-1, -1};

    bool m_handlingLoaded{false};
    static spInGameMenue m_pInstance;
    spGameMap m_pMap{nullptr};
};

Q_DECLARE_INTERFACE(InGameMenue, "InGameMenue");

#endif // INGAMEMENUE_H
