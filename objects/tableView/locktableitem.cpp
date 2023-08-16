#include "objects/tableView/locktableitem.h"
#include "resource_management/objectmanager.h"

#include "coreengine/interpreter.h"

LockTableItem::LockTableItem(bool locked, qint32 itemWidth, QObject *parent)
    : BaseTableItem{parent},
      m_locked(locked)
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("LockTableItem");
#endif

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
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
