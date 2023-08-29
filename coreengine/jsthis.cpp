#include "coreengine/jsthis.h"
#include "coreengine/interpreter.h"

QJSValue JsThis::m_null(QJSValue::NullValue);

void JsThis::setupJsThis(QObject * me)
{
    m_jsThis = Interpreter::getInstance()->newQObject(me);
}
