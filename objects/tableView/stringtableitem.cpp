#include "objects/tableView/stringtableitem.h"
#include "objects/base/label.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

StringTableItem::StringTableItem(const QString& value, qint32 itemWidth, QObject *parent)
    : BaseTableItem{parent},
      m_value(value)
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("StringTableItem");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());

    spLabel pTextfield = spLabel::create(itemWidth);
    pTextfield->setText(m_value);
    pTextfield->setTooltipText(m_value);
    addChild(pTextfield);
}
