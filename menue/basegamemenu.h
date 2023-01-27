#pragma once

#include <QObject>
#include <QPoint>
#include <QRect>
#include <QThread>

#include "3rd_party/oxygine-framework/oxygine/actor/SlidingActorNoClipRect.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

#include "menue/basemenu.h"

#include "game/cursor.h"
#include "game/gamemap.h"

#include "gameinput/mapmover.h"

class BaseGamemenu;
using spBaseGamemenu = oxygine::intrusive_ptr<BaseGamemenu>;

class BaseGamemenu : public Basemenu
{
    Q_OBJECT
public:
    explicit BaseGamemenu(spGameMap pMap, bool clearPlayerlist);
    explicit BaseGamemenu(qint32 width, qint32 heigth, QString map, bool savegame);
    virtual ~BaseGamemenu();
    Cursor* getCursor();
    void calcNewMousePosition(qint32 x, qint32 y);
    QPoint getMousePos(qint32 x, qint32 y);
    void MoveMap(qint32 x, qint32 y);
    virtual void autoScroll(QPoint cursorPosition);
    void initSlidingActor(qint32 x, qint32 y, qint32 width, qint32 height);
    void updateSlidingActorSize();
    oxygine::spActor getMapSlidingActor() const;
    oxygine::spSlidingActorNoClipRect getMapSliding() const;
    /**
     * @brief getCurrentViewPlayer
     * @return
     */
    virtual Player* getCurrentViewPlayer();
    void registerAtInterpreter(QString name = "game");
signals:
    void sigMouseWheel(qint32 direction);
    void sigRightClick(qint32 x, qint32 y);
    void sigRightClickDown(qint32 x, qint32 y);
    void sigRightClickUp(qint32 x, qint32 y);
    void sigLeftClick(qint32 x, qint32 y);
    void sigLeftClickDown(qint32 x, qint32 y);
    void sigLeftClickUp(qint32 x, qint32 y);
    void sigMouseMove(qint32 x, qint32 y);
public slots:
    GameMap* getMap() const;
    void centerMapOnCursor();
    void changeBackground(QString background);
    virtual void setFocused(bool Focused) override;
protected slots:
    virtual void keyInput(oxygine::KeyEvent event);
    virtual void keyUp(oxygine::KeyEvent event);
protected:
    void loadBackground();
    void loadHandling();
    void connectMapCursor();
protected:
    QThread m_MapMoveThread;
    bool m_moveMap{false};
    QPoint m_MoveMapMousePoint;
    spCursor m_Cursor;
    QRect m_autoScrollBorder{50, 50, 300, 50};
    spMapMover m_MapMover;
    oxygine::spSprite m_backgroundSprite;
    oxygine::spSlidingActorNoClipRect m_mapSliding;
    oxygine::spActor m_mapSlidingActor;
    QPoint m_lastMapPoint{-1, -1};
    bool m_handlingLoaded{false};
    spGameMap m_pMap;
    QString m_jsName;
};

Q_DECLARE_INTERFACE(BaseGamemenu, "BaseGamemenu");
