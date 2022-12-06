#include "game/ui/humanquickbuttons.h"
#include "coreengine/interpreter.h"

#include "ui_reader/uifactory.h"

#include "menue/gamemenue.h"

constexpr const char* const quickButtonsMenu = "quickButtonsMenu";

HumanQuickButtons::HumanQuickButtons(GameMenue* pMenu)
{
#ifdef GRAPHICSUPPORT
    setObjectName("MapSelectionFilterDialog");
#endif
    Interpreter::setCppOwnerShip(this);
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));

    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->setGlobal(quickButtonsMenu, pInterpreter->newQObject(pMenu));
    UiFactory::getInstance().createUi("ui/humanQuickButtons.xml", this);
}

HumanQuickButtons::~HumanQuickButtons()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (pInterpreter != nullptr)
    {
        pInterpreter->deleteObject(quickButtonsMenu);
    }
}
