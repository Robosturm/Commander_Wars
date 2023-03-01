#pragma once

#include "basetableitem.h"
#include <QObject>

class XofYTableItem;
using spXofYTableItem = oxygine::intrusive_ptr<XofYTableItem>;

class XofYTableItem final : public BaseTableItem
{
    Q_OBJECT
public:
    explicit XofYTableItem(qint32 currentValue, qint32 maxValue, qint32 itemWidth, QColor textColor, QObject *parent = nullptr);
    virtual bool operator>(const BaseTableItem& rhs) const override
    {
        const auto & rhsItem = static_cast<const XofYTableItem&>(rhs);
        return (m_maxValue - m_currentValue) > (rhsItem.m_maxValue - rhsItem.m_currentValue);
    }
private:
    qint32 m_currentValue;
    qint32 m_maxValue;
};

