#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/dialogs/gamepadinfo.h"

#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"

#include "ui_reader/uifactory.h"

constexpr const char* const GamepadInfoItem = "gamepadInfo";

GamepadInfo::GamepadInfo()
{
#ifdef GRAPHICSUPPORT
    setObjectName("GamepadInfo");
#endif
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->setGlobal(GamepadInfoItem, pInterpreter->newQObject(this));
    UiFactory::getInstance().createUi("ui/gamepadInfoDialog.xml", this);
}

GamepadInfo::~GamepadInfo()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (pInterpreter != nullptr)
    {
        pInterpreter->deleteObject(GamepadInfoItem);
    }
}

void GamepadInfo::remove()
{
    detach();
}
