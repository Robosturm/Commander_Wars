#include "wiki/actionwikipage.h"

#include "coreengine/interpreter.h"

ActionWikipage::ActionWikipage(const PageData * item)
    : Wikipage(item->m_id)
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("ActionWikipage");
#endif
    QString id = item->m_id;
    QJSValue icon;
    QJSValue name;
    QJSValue description;
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (item->m_mainId.isEmpty())
    {
        QJSValueList args({id});
        icon = pInterpreter->doFunction(id, "getIcon");
        name = pInterpreter->doFunction(id, "getName", args);
        description = pInterpreter->doFunction(id, "getDescription");
    }
    else
    {
        QJSValueList args;
        args.append(item->m_item);
        icon = pInterpreter->doFunction(item->m_mainId, "getSubWikiInfoIcon", args);
        name = pInterpreter->doFunction(item->m_mainId, "getSubWikiInfoName", args);
        description = pInterpreter->doFunction(item->m_mainId, "getSubWikiInfoDescription", args);
    }
    loadHeadline(name.toString());
    loadImage(icon.toString(), 2);
    loadText(description.toString());
}
