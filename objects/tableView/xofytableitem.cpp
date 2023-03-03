#include "objects/tableView/xofytableitem.h"
#include "objects/base/label.h"

#include "coreengine/interpreter.h"

XofYTableItem::XofYTableItem(qint32 currentValue, qint32 maxValue, qint32 itemWidth, QColor textColor, QObject *parent)
    : BaseTableItem{parent},
      m_currentValue(currentValue),
      m_maxValue(maxValue)
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("XofYTableItem");
#endif
    spLabel pTextfield = spLabel::create(itemWidth);
    auto style = pTextfield->getStyle();
    style.color = textColor;
    pTextfield->setStyle(style);
    QString text = QString::number(m_currentValue) + " / " + QString::number(m_maxValue);
    pTextfield->setText(text);
    pTextfield->setTooltipText(text);
    addChild(pTextfield);
}
