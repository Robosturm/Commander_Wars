#include "scriptvariables.h"

#include "interpreter.h"

#include "coreengine/mainapp.h"

ScriptVariables::ScriptVariables()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void ScriptVariables::serialize(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << static_cast<qint32>(m_Variables.size());
    for (qint32 i = 0; i < m_Variables.size(); i++)
    {
        m_Variables[i]->serialize(pStream);
    }
}

void ScriptVariables::deserialize(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    qint32 size = 0;
    pStream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        ScriptVariable* pVar = new ScriptVariable();
        pVar->deserialize(pStream);
        m_Variables.append(pVar);
    }
}

ScriptVariable* ScriptVariables::createVariable(QString id)
{
    ScriptVariable* pVar = getVariable(id);
    if (pVar == nullptr)
    {
        pVar = new ScriptVariable(id);
        m_Variables.append(pVar);
    }
    return pVar;
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
