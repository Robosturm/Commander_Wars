#include "coreengine/scriptvariables.h"
#include "coreengine/interpreter.h"
#include "coreengine/mainapp.h"

ScriptVariables::ScriptVariables()
    : QObject()
{
    setObjectName("ScriptVariables");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void ScriptVariables::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << static_cast<qint32>(m_Variables.size());
    for (qint32 i = 0; i < m_Variables.size(); i++)
    {
        m_Variables[i]->serializeObject(pStream);
    }
}

void ScriptVariables::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    qint32 size = 0;
    pStream >> size;
    m_Variables.clear();
    for (qint32 i = 0; i < size; i++)
    {
        spScriptVariable pVar = spScriptVariable::create();
        pVar->deserializeObject(pStream);
        m_Variables.append(pVar);
    }
}

ScriptVariable* ScriptVariables::createVariable(QString id)
{
    spScriptVariable pVar = getVariable(id);
    if (pVar.get() == nullptr)
    {
        pVar = spScriptVariable::create(id);
        m_Variables.append(pVar);
    }
    return pVar.get();
}

ScriptVariable* ScriptVariables::getVariable(QString id)
{
    for (qint32 i = 0; i < m_Variables.size(); i++)
    {
        if (m_Variables[i]->getId() == id)
        {
            return m_Variables[i].get();
        }
    }
    return nullptr;
}

void ScriptVariables::clear()
{
    m_Variables.clear();
}
