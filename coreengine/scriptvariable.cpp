#include "coreengine/scriptvariable.h"
#include "coreengine/interpreter.h"
#include "coreengine/mainapp.h"
#include "coreengine/filesupport.h"

ScriptVariable::ScriptVariable(QString id)
    : m_Id(id)
{
    setObjectName("ScriptVariable");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_buffer.open(QIODevice::ReadWrite);
}

ScriptVariable::ScriptVariable()
{
    setObjectName("ScriptVariable");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_buffer.open(QIODevice::ReadWrite);
}

void ScriptVariable::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_Id;
    QByteArray data = m_buffer.data();
    Filesupport::writeByteArray(pStream, data);
}

void ScriptVariable::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_Id;
    auto data = Filesupport::readByteArray(pStream);
    m_buffer.seek(0);
    m_buffer.write(data);
}
