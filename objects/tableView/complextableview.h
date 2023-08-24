#pragma once

#include <QColor>
#include <QObject>
#include <QVector>
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "objects/tableView/basetableitem.h"
#include "objects/base/label.h"
#include "objects/base/panel.h"

class ComplexTableView;
using spComplexTableView = std::shared_ptr<ComplexTableView>;

class ComplexTableView final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    struct Item
    {
        using ViewItems = QVector<spBaseTableItem>;
        ViewItems items;
        void * pData{nullptr};
    };
    using Items = QVector<Item>;

    explicit ComplexTableView(const QVector<qint32> & widths, const QStringList & header, qint32 heigth, QObject *parent = nullptr);


    void setItems(const Items & items)
    {
        m_pPanel->clearContent();
        m_VLines.clear();
        m_HLines.clear();
        m_columns.clear();
        m_items = items;
        m_currentItem = -1;
        sortItems(1, false);
        createItems();
    }

    template<typename TData>
    TData* getDataItem(qint32 i) const
    {
        Q_ASSERT(i >= 0 && i < m_items.size());
        return static_cast<TData*>(m_items[i].pData);
    }

    const QVector<qint32> & getWidths() const;
    const Item & getItem(qint32 i) const;
    Q_INVOKABLE void setCurrentItem(qint32 i);
    Q_INVOKABLE qint32 getCurrentItem() const;
    Q_INVOKABLE void sortItems(qint32 column, bool up);
signals:
    void sigItemClicked();
    void sigSortItems(qint32 column, bool up);

private:
    void createItems();
    void updateItemPositions();
private:
    Items m_items;
    QVector<qint32> m_widths;
    QVector<bool> m_sortUp;
    QVector<oxygine::spBox9Sprite> m_VLines;
    QVector<oxygine::spBox9Sprite> m_HLines;
    qint32 m_currentItem{-1};
    QVector<oxygine::spActor> m_columns;
    QVector<spLabel> m_Labels;
    spPanel m_pPanel;
    QColor m_selectColor{32, 200, 32, 0};
};

