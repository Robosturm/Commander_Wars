#include "coreengine/gameversion.h"

GameVersion::GameVersion()
    : m_sufix{VERSION_SUFFIX}
{
}

GameVersion::GameVersion(qint32 major, qint32 minor, qint32 revision, QString sufix)
    : m_major(major),
    m_minor(minor),
    m_revision(revision),
    m_sufix(sufix)
{
}

QString GameVersion::toString() const
{
    return QString::number(m_major) + "." + QString::number(m_minor) + "." + QString::number(m_revision)+ "-" + m_sufix;
}

QString GameVersion::getSufix() const
{
    return m_sufix;
}

void GameVersion::serializeObject(QDataStream& stream) const
{
    stream << getVersion();
    stream << m_major;
    stream << m_minor;
    stream << m_revision;
    stream << m_sufix;
}

void GameVersion::deserializeObject(QDataStream& stream)
{
    qint32 version = 0;
    stream >> version;
    stream >> m_major;
    stream >> m_minor;
    stream >> m_revision;
    stream >> m_sufix;
}

qint32 GameVersion::getMajor() const
{
    return m_major;
}

void GameVersion::setMajor(qint32 newMajor)
{
    m_major = newMajor;
}

qint32 GameVersion::getMinor() const
{
    return m_minor;
}

void GameVersion::setMinor(qint32 newMinor)
{
    m_minor = newMinor;
}

qint32 GameVersion::getRevision() const
{
    return m_revision;
}

void GameVersion::setRevision(qint32 newRevision)
{
    m_revision = newRevision;
}

void GameVersion::setSufix(const QString & newSufix)
{
    m_sufix = newSufix;
}

bool GameVersion::operator==(const GameVersion& other)
{
    return m_major == other.m_major &&
           m_minor == other.m_minor &&
           m_revision == other.m_revision &&
           m_sufix == other.m_sufix;
}

bool GameVersion::operator!=(const GameVersion& other)
{
    return !(*this == other);
}

bool GameVersion::operator>=(const GameVersion& other)
{
    if (m_major > other.m_major)
    {
        return true;
    }
    else if (m_major == other.m_major)
    {
        if (m_minor > other.m_minor)
        {
            return true;
        }
        else if (m_minor == other.m_minor)
        {
            if (m_revision > other.m_revision)
            {
                return true;
            }
        }
    }
    return false;
}
