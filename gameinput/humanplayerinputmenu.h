#ifndef HUMANPLAYERINPUTMENU_H
#define HUMANPLAYERINPUTMENU_H

#include <QObject>
#include <QVector>
#include <QStringList>

#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/TextStyle.h"
#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"

#include "objects/base/h_scrollbar.h"

class Player;
class GameMenue;
class GameMap;
class HumanPlayerInputMenu;
using spHumanPlayerInputMenu = std::shared_ptr<HumanPlayerInputMenu>;

class HumanPlayerInputMenu final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit HumanPlayerInputMenu(GameMenue* pMenu, GameMap* pMap, const QStringList & texts, const QStringList &  actionIDs, const QVector<oxygine::spActor> & icons,
                                  const QVector<qint32> & costList = QVector<qint32>(), const QVector<bool> & enabledList = QVector<bool>());
   virtual ~HumanPlayerInputMenu() = default;
    /**
     * @brief setMenuPosition changes the position of this menu
     * @param x position in pixel
     * @param y position in pixel
     */
    void setMenuPosition(qint32 x, qint32 y);
signals:
    void sigItemSelected(QString actionID, qint32 cost);
    void sigCanceled(qint32 x, qint32 y);
public slots:
    void keyInput(oxygine::KeyEvent event);
    void moveMouseToItem(qint32 x, qint32 y);
private:
    qint32 createTopSprite(qint32 x, qint32 width, Player* pPlayer);
    qint32 createBottomSprite(qint32 x, qint32 y, qint32 width, Player* pPlayer);
    oxygine::spBox9Sprite createMenuItem(bool enabled, qint32& x, qint32& y, qint32 width, oxygine::TextStyle style,
                                         const QString & text, const QString & action, qint32 costs, oxygine::spActor icon, qint32 item,
                                         Player* pPlayer);
    void updateItemPositionAndVisibility();
    void scroll(qint32 count);
    void addTouchMoveEvents();
private:
    oxygine::spSprite m_Cursor;
    qint32 m_startY{0};
    qint32 m_itemHeigth{0};
    qint32 m_itemWidth{0};
    qint32 m_currentAction{0};
    QStringList m_ActionIDs;
    QVector<qint32> m_CostList;
    QVector<bool> m_EnabledList;
    QVector<oxygine::spBox9Sprite> m_ItemActors;
    spH_Scrollbar m_scrollbar;

    bool m_Focused{true};
    qint32 m_maxXCount{0};
    qint32 m_rowCount{0};
    qint32 m_columnCount{0};
    qint32 m_startItem{0};

    bool m_moveScrolling{false};
    QPoint m_lastScrollPoint;
    GameMap* m_pMap{nullptr};    
    GameMenue* m_pMenu{nullptr};
};

#endif // HUMANPLAYERINPUTMENU_H
