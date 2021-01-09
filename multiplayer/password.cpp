#include "qcryptographichash.h"

#include "multiplayer/password.h"
#include "coreengine/filesupport.h"

Password::Password()
    : QObject()
{
    setPassword("");
}

Password::Password(QString password)
{
    setPassword(password);
}

void Password::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    Filesupport::writeByteArray(pStream, m_passwordHash);
}

void Password::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    m_passwordHash = Filesupport::readByteArray(pStream);
}

void Password::setPassword(QString password)
{
    QCryptographicHash myHash(QCryptographicHash::Sha3_512);
    myHash.addData(password.toStdString().c_str(), password.size());
    m_passwordHash = myHash.result();
}

void Password::setPassword(const Password & password)
{
    m_passwordHash = password.m_passwordHash;
}

bool Password::isValidPassword(QString password) const
{
    QCryptographicHash myHash(QCryptographicHash::Sha3_512);
    myHash.addData(password.toStdString().c_str(), password.size());
    return (m_passwordHash == myHash.result());
}

bool Password::areEqualPassword(const Password & password) const
{
    return (m_passwordHash == password.m_passwordHash);
}
