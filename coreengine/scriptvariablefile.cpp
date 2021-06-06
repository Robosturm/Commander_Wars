#include "coreengine/scriptvariablefile.h"
#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

ScriptVariableFile::ScriptVariableFile(QString filename)
    : QObject(),
      m_filename(filename)
{
    setObjectName("ScriptVariableFile");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void ScriptVariableFile::writeFile()
{
    QFile file(m_filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QDataStream pStream(&file);
    serializeObject(pStream);
}

void ScriptVariableFile::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    m_Variables.serializeObject(pStream);
}

void ScriptVariableFile::deserializeObject(QDataStream& pStream)
{
    qint32 version;
    pStream >> version;
    m_Variables.deserializeObject(pStream);
}

const QString &ScriptVariableFile::getFilename() const
{
    return m_filename;
}
