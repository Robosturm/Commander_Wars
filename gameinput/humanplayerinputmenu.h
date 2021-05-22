#ifndef HUMANPLAYERINPUTMENU_H
#define HUMANPLAYERINPUTMENU_H

#include <QObject>
#include <QVector>
#include <QStringList>


#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"

#include "objects/base/h_scrollbar.h"

class HumanPlayerInputMenu;
typedef oxygine::intrusive_ptr<HumanPlayerInputMenu> spHumanPlayerInputMenu;

class HumanPlayerInputMenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit HumanPlayerInputMenu(QStringList texts, QStringList actionIDs, QVector<oxygine::spActor> icons,
                                  QVector<qint32> costList = QVector<qint32>(), QVector<bool> enabledList = QVector<bool>());
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
    qint32 createTopSprite(qint32 x, qint32 width);
    qint32 createBottomSprite(qint32 x, qint32 y, qint32 width);
    oxygine::spBox9Sprite createMenuItem(bool enabled, qint32& x, qint32& y, qint32 width, oxygine::TextStyle style,
                                         QString text, QString action, qint32 costs, oxygine::spActor icon, qint32 item);
    void updateItemPositionAndVisibility();
    void scroll(qint32 count);
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
};

#endif // HUMANPLAYERINPUTMENU_H
