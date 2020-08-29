#include "scriptvariable.h"

#include "interpreter.h"

#include "coreengine/mainapp.h"

#include "coreengine/filesupport.h"

ScriptVariable::ScriptVariable(QString id)
    : m_Id(id)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    buffer.open(QIODevice::ReadWrite);
}

ScriptVariable::ScriptVariable()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    buffer.open(QIODevice::ReadWrite);
}

void ScriptVariable::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_Id;
    QByteArray data = buffer.data();
    Filesupport::writeByteArray(pStream, data);
}

void ScriptVariable::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_Id;
    auto data = Filesupport::readByteArray(pStream);
    buffer.seek(0);
    buffer.write(data);
}
