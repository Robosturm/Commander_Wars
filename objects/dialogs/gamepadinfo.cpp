#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"

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
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
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
    pInterpreter->deleteObject(GamepadInfoItem);
}

void GamepadInfo::remove()
{
    detach();
}
