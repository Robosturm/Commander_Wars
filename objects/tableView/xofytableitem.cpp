#include "objects/tableView/xofytableitem.h"
#include "objects/base/label.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

XofYTableItem::XofYTableItem(qint32 currentValue, qint32 maxValue, qint32 itemWidth, QObject *parent)
    : BaseTableItem{parent},
      m_currentValue(currentValue),
      m_maxValue(maxValue)
{
    Interpreter::setCppOwnerShip(this);
    setObjectName("LockTableItem");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());

    spLabel pTextfield = spLabel::create(itemWidth);
    QString text = QString::number(m_currentValue) + " / " + QString::number(m_maxValue);
    pTextfield->setText(text);
    pTextfield->setTooltipText(text);
    addChild(pTextfield);
}
