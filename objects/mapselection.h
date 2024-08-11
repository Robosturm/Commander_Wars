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

#include "objects/base/panel.h"
#include "objects/base/label.h"

class MapSelection;
using spMapSelection = std::shared_ptr<MapSelection>;

class MapSelection final : public QObject, public oxygine::Actor
{
    Q_OBJECT
    static const qint32 m_itemHeigth{35};
public:
    explicit MapSelection(qint32 heigth, qint32 width, QString folder, const QStringList & filter);
    virtual ~MapSelection() = default;

    QString getCurrentFolder()
    {
        return m_currentFolder;
    }
    QString getCurrentFile()
    {
        return m_currentItem;
    }
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
    void itemClicked(QString item);
    void sigStartItemChangeTimer();
public slots:
    void startItemChangeTimer();
    void changeFolder(QString folder);
    void updateSelection();
    void itemChangeTimerExpired();
    void createItemContainer(qint32 y, qint32 width, qint32 height);
    void filterChanged();
    void refresh();
private:
    void addNewSelectionItem(qint32 i, qint32 & y);
    void addFiles(const QString & newFolder, const QStringList & searchPaths, QStringList filterList, QDir::Filter filter);
private:
    QStringList m_filter;
    QString m_currentFolder;
    QString m_currentItem;
    QString m_lastItem;
    qint32 m_currentIdx{0};


    QVector<spLabel> m_Items;
    qint32 m_itemCount{0};
    QStringList m_Files;
    qint32 m_currentStartIndex{0};
    oxygine::spBox9Sprite m_SelectedItem;
    QTimer m_itemChangedTimer;
    bool m_itemClicked{false};
    spPanel m_ItemContainer;
    bool m_wasMoveScrolling{false};
    QPoint m_lastScrollPoint;
    MapFilter m_mapFilter;
};

#endif // MAPSELECTION_H
