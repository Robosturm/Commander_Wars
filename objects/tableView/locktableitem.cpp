#include "objects/tableView/locktableitem.h"
#include "objects/base/label.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

LockTableItem::LockTableItem(bool locked, qint32 itemWidth, QObject *parent)
    : BaseTableItem{parent},
      m_locked(locked)
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("LockTableItem");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());

    spLabel pTextfield = spLabel::create(itemWidth);
    QString text;
    if (locked)
    {
        text = QString(static_cast<char>(FontManager::SpecialChars::lockChar));
    }
    else
    {
        text = QString(static_cast<char>(FontManager::SpecialChars::unlockChar));
    }
    pTextfield->setText(text);
    pTextfield->setTooltipText(text);
    addChild(pTextfield);
}
