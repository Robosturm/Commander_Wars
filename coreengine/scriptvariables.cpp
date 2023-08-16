#include "coreengine/scriptvariables.h"
#include "coreengine/interpreter.h"

ScriptVariables::ScriptVariables()
{
#ifdef GRAPHICSUPPORT
    setObjectName("ScriptVariables");
#endif
    Interpreter::setCppOwnerShip(this);
}

void ScriptVariables::serializeObject(QDataStream& pStream) const
{
    serializeObject(pStream, false);
}

void ScriptVariables::serializeObject(QDataStream& pStream, bool forHash) const
{
    pStream << getVersion();
    if (forHash)
    {
        qint32 count = 0;
        for (qint32 i = 0; i < m_Variables.size(); i++)
        {
            if (m_Variables[i]->getModified())
            {
                m_Variables[i]->serializeObject(pStream);
                ++count;
            }
        }
        pStream << count;
    }
    else
    {
        pStream << static_cast<qint32>(m_Variables.size());
        for (qint32 i = 0; i < m_Variables.size(); i++)
        {
            m_Variables[i]->serializeObject(pStream);
        }
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
        spScriptVariable pVar = MemoryManagement::create<ScriptVariable>();
        pVar->deserializeObject(pStream);
        m_Variables.append(pVar);
    }
}

ScriptVariable* ScriptVariables::createVariable(const QString id)
{
    spScriptVariable pVar = spScriptVariable(getVariable(id));
    if (pVar.get() == nullptr)
    {
        pVar = MemoryManagement::create<ScriptVariable>(id);
        m_Variables.append(pVar);
    }
    return pVar.get();
}

ScriptVariable* ScriptVariables::getVariable(const QString id)
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
