#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"

#include "objects/dialogs/editor/dialogviewmapstats.h"

#include "ui_reader/uifactory.h"

#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"

constexpr const char* const ViewMapStats = "viewMapStats";

DialogViewMapStats::DialogViewMapStats(GameMap* pMap)
    : m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DialogViewMapStats");
#endif
    Interpreter::setCppOwnerShip(this);

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->setGlobal(ViewMapStats, pInterpreter->newQObject(this));
    UiFactory::getInstance().createUi("ui/viewMapStats.xml", this);
}

void DialogViewMapStats::init()
{
    connect(this, &DialogViewMapStats::sigClosed, this, &DialogViewMapStats::remove, Qt::QueuedConnection);
}

DialogViewMapStats::~DialogViewMapStats()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (pInterpreter != nullptr)
    {
        pInterpreter->deleteObject(ViewMapStats);
    }
}

GameMap* DialogViewMapStats::getMap()
{
    return m_pMap;
}

void DialogViewMapStats::close()
{
    emit sigClosed();
}

void DialogViewMapStats::remove()
{
    detach();
}
