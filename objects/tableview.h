#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QObject>

#include "qvector.h"

#include "qstringlist.h"

#include "oxygine-framework.h"

class TableView;
typedef oxygine::intrusive_ptr<TableView> spTableView;

class TableView : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit TableView(qint32 width, QVector<QStringList> data, QStringList header, bool selectable = false);





signals:
    void sigItemClicked();
public slots:
    void setCurrentItem(qint32 i);
    qint32 getCurrentItem() const;
    QStringList getItem(qint32 i);
private:
    QStringList m_Header;
    QVector<QStringList> m_data;
    QVector<oxygine::spColorRectSprite> m_VLines;
    QVector<oxygine::spColorRectSprite> m_HLines;
    qint32 currentItem;
};

#endif // TABLEVIEW_H
