#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QObject>
#include "qvector.h"
#include "qstringlist.h"

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "objects/base/label.h"

class TableView;
typedef oxygine::intrusive_ptr<TableView> spTableView;

class TableView : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit TableView(QVector<qint32> widths, QVector<QStringList> data, QStringList header, bool selectable = false);

signals:
    void sigItemClicked();
public slots:
    void setCurrentItem(qint32 i);
    qint32 getCurrentItem() const;
    QStringList getItem(qint32 i);
private:
    void addRow(qint32 i, qint32 i2, qint32 x, bool selectable);
private:
    QStringList m_Header;
    QVector<QStringList> m_data;
    QVector<oxygine::spColorRectSprite> m_VLines;
    QVector<oxygine::spColorRectSprite> m_HLines;
    QVector<spLabel> m_Labels;
    QVector<qint32> m_widths;
    qint32 m_currentItem{-1};
};

#endif // TABLEVIEW_H
