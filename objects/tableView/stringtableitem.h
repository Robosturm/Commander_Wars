#pragma once

#include "objects/tableView/basetableitem.h"
#include <QObject>

class StringTableItem;
using spStringTableItem = oxygine::intrusive_ptr<StringTableItem>;

class StringTableItem final : public BaseTableItem
{
    Q_OBJECT
public:
    explicit StringTableItem(const QString& value, qint32 itemWidth, QColor textColor, QObject *parent = nullptr);

    virtual bool operator>(const BaseTableItem& rhs) const override
    {
        return m_value > static_cast<const StringTableItem&>(rhs).m_value;
    }
private:
    QString m_value;
};

