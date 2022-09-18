#include "objects/tableView/locktableitem.h"
#include "objects/base/label.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

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

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spSprite pSprite = oxygine::spSprite::create();
    if (locked)
    {
        pSprite->setResAnim(pObjectManager->getResAnim("lock"));
    }
    else
    {
        pSprite->setResAnim(pObjectManager->getResAnim("unlock"));
    }
    addChild(pSprite);
}
