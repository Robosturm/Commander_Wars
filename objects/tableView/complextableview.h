#pragma once

#include <QObject>
#include <QVector>
#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "objects/tableView/basetableitem.h"

class ComplexTableView;
using spComplexTableView = oxygine::intrusive_ptr<ComplexTableView>;

class ComplexTableView : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    using Items = QVector<QVector<spBaseTableItem>>;

    explicit ComplexTableView(QObject *parent = nullptr);

    void setItems(const Items & items);

private:
    Items m_items;
};
