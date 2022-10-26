#include <QJsonArray>

#include "network/networkgamedata.h"
#include "network/JsonKeys.h"

#include "coreengine/filesupport.h"

bool NetworkGameData::hasOpenPlayers()
{
    return (m_players < m_maxPlayers);
}

const QString &NetworkGameData::getSlaveAddress() const
{
    return m_slaveAddress;
}

void NetworkGameData::setSlaveAddress(const QString &newAddress)
{
    m_slaveAddress = newAddress;
}

quint16 NetworkGameData::getSlavePort() const
{
    return m_slavePort;
}

void NetworkGameData::setSlavePort(quint16 newPort)
{
    m_slavePort = newPort;
}

QJsonObject NetworkGameData::toJson() const
{
    QJsonObject obj;
    obj.insert(JsonKeys::JSONKEY_JOINEDPLAYERS, static_cast<qint64>(m_players));
    obj.insert(JsonKeys::JSONKEY_MAXPLAYERS, static_cast<qint64>(m_maxPlayers));
    obj.insert(JsonKeys::JSONKEY_GAMEDESCRIPTION, m_description);
    obj.insert(JsonKeys::JSONKEY_MAPNAME, m_mapName);
    obj.insert(JsonKeys::JSONKEY_SLAVENAME, m_slaveName);
    obj.insert(JsonKeys::JSONKEY_HASPASSWORD, m_locked);
    obj.insert(JsonKeys::JSONKEY_UUID, m_uuid);
    QJsonObject mods;
    for (qint32 i = 0; i < m_Mods.size(); ++i)
    {
        mods.insert(JsonKeys::JSONKEY_MOD + QString::number(i), m_Mods[i]);
    }
    obj.insert(JsonKeys::JSONKEY_USEDMODS, mods);

    QJsonArray usernames;
    for (qint32 i = 0; i < m_playerNames.size(); ++i)
    {
        usernames.push_back(m_playerNames[i]);
    }
    obj.insert(JsonKeys::JSONKEY_USERNAMES, usernames);
    return obj;
}

void NetworkGameData::fromJson(const QJsonObject & obj)
{
    m_players = obj.value(JsonKeys::JSONKEY_JOINEDPLAYERS).toInteger();
    m_maxPlayers = obj.value(JsonKeys::JSONKEY_MAXPLAYERS).toInteger();
    QJsonObject mods = obj.value(JsonKeys::JSONKEY_USEDMODS).toObject();
    for (const auto & mod : mods)
    {
        m_Mods.append(mod.toString());
    }
    m_description = obj.value(JsonKeys::JSONKEY_GAMEDESCRIPTION).toString();
    m_mapName = obj.value(JsonKeys::JSONKEY_MAPNAME).toString();
    m_slaveName = obj.value(JsonKeys::JSONKEY_SLAVENAME).toString();
    m_locked = obj.value(JsonKeys::JSONKEY_HASPASSWORD).toBool();
    m_uuid = obj.value(JsonKeys::JSONKEY_UUID).toInteger();
    QJsonArray usernames = obj.value(JsonKeys::JSONKEY_USERNAMES).toArray();
    for (const auto & username : usernames)
    {
        m_playerNames.append(username.toString());
    }
}

QString NetworkGameData::getMapName() const
{
    return m_mapName;
}

void NetworkGameData::setMapName(const QString &mapName)
{
    m_mapName = mapName;
}

QString NetworkGameData::getDescription() const
{
    return m_description;
}

void NetworkGameData::setDescription(const QString &value)
{
    m_description = value;
}

const QStringList &  NetworkGameData::getMods() const
{
    return m_Mods;
}

void NetworkGameData::setMods(const QStringList &Mods)
{
    m_Mods = Mods;
}

qint32 NetworkGameData::getMaxPlayers() const
{
    return m_maxPlayers;
}

void NetworkGameData::setMaxPlayers(const qint32 &maxPlayers)
{
    m_maxPlayers = maxPlayers;
}

qint32 NetworkGameData::getPlayers() const
{
    return m_players;
}

void NetworkGameData::setPlayers(const qint32 &players)
{
    m_players = players;
}

QString NetworkGameData::getSlaveName() const
{
    return m_slaveName;
}

void NetworkGameData::setSlaveName(const QString &slaveName)
{
    m_slaveName = slaveName;
}

bool NetworkGameData::getLaunched() const
{
    return m_launched;
}

void NetworkGameData::setLaunched(bool launched)
{
    m_launched = launched;
}

bool NetworkGameData::getLocked() const
{
    return m_locked;
}

void NetworkGameData::setLocked(bool locked)
{
    m_locked = locked;
}

const QStringList &  NetworkGameData::getPlayerNames() const
{
    return m_playerNames;
}

void NetworkGameData::setPlayerNames(const QStringList &playerNames)
{
    m_playerNames = playerNames;
}

qint64 NetworkGameData::getUuid() const
{
    return m_uuid;
}

void NetworkGameData::setUuid(qint64 newUuid)
{
    m_uuid = newUuid;
}

const QString &NetworkGameData::getSlaveSecondaryAddress() const
{
    return m_slaveSecondaryAddress;
}

void NetworkGameData::setSlaveSecondaryAddress(const QString &newSlaveSecondaryAddress)
{
    m_slaveSecondaryAddress = newSlaveSecondaryAddress;
}
