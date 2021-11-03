#include "game/ui/humanquickbuttons.h"
#include "coreengine/mainapp.h"

#include "ui_reader/uifactory.h"

HumanQuickButtons::HumanQuickButtons()
{
    setObjectName("MapSelectionFilterDialog");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));

    UiFactory::getInstance().createUi("ui/humanQuickButtons.xml", this);
}

