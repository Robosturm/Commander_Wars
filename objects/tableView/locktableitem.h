#pragma once

#include "basetableitem.h"
#include <QObject>

class LockTableItem : public BaseTableItem
{
    Q_OBJECT
public:
    explicit LockTableItem(bool locked, qint32 itemWidth, QObject *parent = nullptr);
    virtual bool operator>(const BaseTableItem& rhs) const override
    {
        const auto & rhsItem = static_cast<const LockTableItem&>(rhs);
        return m_locked > rhsItem.m_locked;
    }

private:
    bool m_locked;
};

