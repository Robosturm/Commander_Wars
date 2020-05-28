#include "defaultwikipage.h"

#include "coreengine/interpreter.h"

DefaultWikipage::DefaultWikipage(QString item)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue icon = pInterpreter->doFunction(item, "getIcon");
    QJSValue name = pInterpreter->doFunction(item, "getName");
    QJSValue description = pInterpreter->doFunction(item, "getDescription");

    loadHeadline(name.toString());
    loadImage(icon.toString(), 2);
    loadText(description.toString());
}
