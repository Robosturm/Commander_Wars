#include "objects/tableView/stringtableitem.h"
#include "objects/base/label.h"

#include "coreengine/interpreter.h"

StringTableItem::StringTableItem(const QString& value, qint32 itemWidth, QColor textColor, QObject *parent)
    : BaseTableItem{parent},
      m_value(value)
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("StringTableItem");
#endif

    spLabel pTextfield = spLabel::create(itemWidth);
    auto style = pTextfield->getStyle();
    style.color = textColor;
    pTextfield->setStyle(style);
    pTextfield->setText(m_value);
    pTextfield->setTooltipText(m_value);
    addChild(pTextfield);
}
