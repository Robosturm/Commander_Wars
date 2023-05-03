#include "coreengine/scriptvariable.h"
#include "coreengine/interpreter.h"
#include "coreengine/filesupport.h"

ScriptVariable::ScriptVariable(QString id)
    : m_Id(id)
{
#ifdef GRAPHICSUPPORT
    setObjectName("ScriptVariable");
#endif
    Interpreter::setCppOwnerShip(this);
    m_buffer.open(QIODevice::ReadWrite);
}

ScriptVariable::ScriptVariable()
{
#ifdef GRAPHICSUPPORT
    setObjectName("ScriptVariable");
#endif
    Interpreter::setCppOwnerShip(this);
    m_buffer.open(QIODevice::ReadWrite);
}

void ScriptVariable::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_Id;
    QByteArray data = m_buffer.data();
    Filesupport::writeByteArray(pStream, data);
    pStream << m_modified;
}

void ScriptVariable::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_Id;
    auto data = Filesupport::readByteArray(pStream);
    m_buffer.seek(0);
    m_buffer.write(data);
    if (version > 1)
    {
        pStream >> m_modified;
    }
}

bool ScriptVariable::getModified() const
{
    return m_modified;
}
