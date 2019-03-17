#include "scriptvariable.h"

#include "interpreter.h"

ScriptVariable::ScriptVariable(QString id)
    : m_Id(id)
{
    Interpreter::setCppOwnerShip(this);
    buffer.open(QIODevice::ReadWrite);
}

ScriptVariable::ScriptVariable()
{
    Interpreter::setCppOwnerShip(this);
    buffer.open(QIODevice::ReadWrite);
}

void ScriptVariable::serialize(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << m_Id;
    QByteArray data = buffer.data();
    pStream << data;
}

void ScriptVariable::deserialize(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_Id;
    QByteArray data;
    pStream >> data;
    buffer.setData(data);
}
