#include "wiki/actionwikipage.h"

#include "coreengine/interpreter.h"

ActionWikipage::ActionWikipage(QString item)
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("ActionWikipage");
#endif
    QJSValueList args({item});
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue icon = pInterpreter->doFunction(item, "getIcon");
    QJSValue name = pInterpreter->doFunction(item, "getName", args);
    QJSValue description = pInterpreter->doFunction(item, "getDescription");
    loadHeadline(name.toString());
    loadImage(icon.toString(), 2);
    loadText(description.toString());
}
