#include "coreengine/scriptvariablefile.h"
#include "coreengine/interpreter.h"

#include <QFile>

ScriptVariableFile::ScriptVariableFile(const QString & filename)
    : m_filename(filename)
{
#ifdef GRAPHICSUPPORT
    setObjectName("ScriptVariableFile");
#endif
    Interpreter::setCppOwnerShip(this);
}

void ScriptVariableFile::writeFile()
{
    QFile file(Settings::getInstance()->getUserPath() + m_filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QDataStream pStream(&file);
    pStream.setVersion(QDataStream::Version::Qt_6_5);
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
