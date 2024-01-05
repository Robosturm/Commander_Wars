#include "coreengine/jsthis.h"
#include "coreengine/interpreter.h"
#include "coreengine/gameconsole.h"

QJSValue JsThis::m_null(QJSValue::NullValue);

void JsThis::setupJsThis(QObject * me)
{
    if (me != nullptr)
    {
        m_jsThis = Interpreter::getInstance()->newQJsObject(me, this);
    }
    else
    {
        CONSOLE_PRINT_MODULE("Failed to setupJsThis", GameConsole::eDEBUG, GameConsole::eJavaScript);
    }
}

JsThis::~JsThis()
{
    Interpreter::getInstance()->deleteQJsObject(this);
}
