#include "game/ui/humanquickbuttons.h"
#include "coreengine/mainapp.h"

#include "ui_reader/uifactory.h"

#include "menue/gamemenue.h"
#include "menue/movementplanner.h"

constexpr const char* const quickButtonsMenu = "quickButtonsMenu";

HumanQuickButtons::HumanQuickButtons(GameMenue* pMenu)
{
    setObjectName("MapSelectionFilterDialog");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));

    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->setGlobal(quickButtonsMenu, pInterpreter->newQObject(pMenu));
    UiFactory::getInstance().createUi("ui/humanQuickButtons.xml", this);
}

HumanQuickButtons::~HumanQuickButtons()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->deleteObject(quickButtonsMenu);
}
