#ifndef MAPSELECTION_H
#define MAPSELECTION_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QElapsedTimer>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"

#include "mapsupport/mapfilter.h"

class MapSelection;
using spMapSelection = oxygine::intrusive_ptr<MapSelection>;

class MapSelection final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit MapSelection(qint32 heigth, qint32 width, QString folder, const QStringList & filter);
    ~MapSelection() = default;

    QString getCurrentFolder()
    {
        return m_currentFolder;
    }
    QString getCurrentFile()
    {
        return m_currentItem;
    }
    virtual void update(const oxygine::UpdateState& us) override;
    /**
     * @brief setSelection
     * @param folder
     * @param files
     */
    void setSelection(QString folder, QStringList files);
    /**
     * @brief setCurrentItem
     * @param item
     */
    void setCurrentItem(QString item);
    /**
     * @brief getMapFilter
     * @return
     */
    MapFilter* getMapFilter()
    {
        return &m_mapFilter;
    }

signals:    
    void itemChanged(QString item);
    void changeSelection(qint32 index);
    void itemClicked(QString item);
    void sigStartItemChangeTimer();
public slots:
    void startItemChangeTimer();
    void changeFolder(QString folder);
    void updateSelection(qint32 startIndex);
    void itemChangeTimerExpired();
    void createItemContainer(qint32 y, qint32 width, qint32 height);
    void filterChanged();
    void refresh();
private:
    QStringList m_filter;
    QString m_currentFolder;
    QString m_currentItem;
    QString m_lastItem;
    qint32 m_currentIdx{0};
    QVector<oxygine::spTextField> m_Items;
    qint32 m_itemCount{0};
    static const qint32 m_itemHeigth{35};
    QStringList m_Files;
    qint32 m_currentStartIndex{0};
    oxygine::spBox9Sprite m_SelectedItem;
    qint32 m_spin = 0;
    QElapsedTimer m_timer;
    QTimer m_itemChangedTimer;
    bool m_itemClicked{false};
    oxygine::spActor m_ItemContainer;
    bool m_moveScrolling{false};
    bool m_wasMoveScrolling{false};
    QPoint m_lastScrollPoint;
    MapFilter m_mapFilter;
    QVector<qint32> m_mapMapping;
};

#endif // MAPSELECTION_H
