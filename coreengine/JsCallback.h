#ifndef JSCALLBACK_H
#define JSCALLBACK_H


#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "coreengine/interpreter.h"
#include "coreengine/interpreter.h"

template<class TClass>
class JsCallback : public oxygine::ref_counter
{
public:
    JsCallback(TClass* pOwner, QString objectID, QString callback)
        : m_pOwner(pOwner),
          m_objectID(objectID),
          m_callback(callback)
    {
    }
    void call()
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args;
        QJSValue objArg = pInterpreter->newQObject(m_pOwner);
        args << objArg;
        pInterpreter->doFunction(m_objectID, m_callback, args);
    }

private:
    TClass* m_pOwner;
    QString m_objectID;
    QString m_callback;
};

#endif // JSCALLBACK_H
