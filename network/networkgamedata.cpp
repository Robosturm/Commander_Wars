#include <QJsonArray>

#include "network/networkgamedata.h"
#include "network/JsonKeys.h"
#include "coreengine/gameconsole.h"

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
    CONSOLE_PRINT("NetworkGameData::toJson", GameConsole::eDEBUG);
    QJsonObject obj;
    obj.insert(JsonKeys::JSONKEY_JOINEDPLAYERS, m_players);
    obj.insert(JsonKeys::JSONKEY_MAXPLAYERS, m_maxPlayers);
    obj.insert(JsonKeys::JSONKEY_DESCRIPTION, m_description);
    obj.insert(JsonKeys::JSONKEY_NAME, m_mapName);
    obj.insert(JsonKeys::JSONKEY_SLAVENAME, m_slaveName);
    obj.insert(JsonKeys::JSONKEY_HASPASSWORD, m_locked);
    obj.insert(JsonKeys::JSONKEY_UUID, QString::number(m_uuid));
    obj.insert(JsonKeys::JSONKEY_CURRENTPLAYER, m_currentPlayer);
    obj.insert(JsonKeys::JSONKEY_VERSION_MAJOR, m_gameVersion.getMajor());
    obj.insert(JsonKeys::JSONKEY_VERSION_MINOR, m_gameVersion.getMinor());
    obj.insert(JsonKeys::JSONKEY_VERSION_REVISION, m_gameVersion.getRevision());
    obj.insert(JsonKeys::JSONKEY_VERSION_SUFIX, m_gameVersion.getSufix());
    QJsonObject mods;
    for (qint32 i = 0; i < m_Mods.size(); ++i)
    {
        mods.insert(JsonKeys::JSONKEY_MOD + QString::number(i), m_Mods[i]);
    }
    obj.insert(JsonKeys::JSONKEY_USEDMODS, mods);
    obj.insert(JsonKeys::JSONKEY_RUNNINGGAME, m_runningGame || m_launched);

    CONSOLE_PRINT("Adding players to slavegame " + m_slaveName, GameConsole::eDEBUG);
    QJsonArray usernames;
    for (qint32 i = 0; i < m_playerNames.size(); ++i)
    {
        CONSOLE_PRINT("user: " + m_playerNames[i], GameConsole::eDEBUG);
        usernames.push_back(m_playerNames[i]);
    }
    obj.insert(JsonKeys::JSONKEY_USERNAMES, usernames);
    QJsonArray onlineData;
    for (qint32 i = 0; i < m_onlineData.size(); ++i)
    {
        onlineData.push_back(m_onlineData[i]);
    }
    obj.insert(JsonKeys::JSONKEY_ONLINEINFO, onlineData);
    obj.insert(JsonKeys::JSONKEY_MINIMAPDATA, QString::fromLocal8Bit(m_minimapData));
    obj.insert(JsonKeys::JSONKEY_MATCHOBSERVERCOUNT, m_observers);
    obj.insert(JsonKeys::JSONKEY_MATCHMAXOBSERVERCOUNT, m_maxObservers);
    return obj;
}

void NetworkGameData::fromJson(const QJsonObject & obj)
{
    m_Mods.clear();
    m_playerNames.clear();
    m_onlineData.clear();
    m_gameVersion.setMajor(obj.value(JsonKeys::JSONKEY_VERSION_MAJOR).toInt());
    m_gameVersion.setMinor(obj.value(JsonKeys::JSONKEY_VERSION_MINOR).toInt());
    m_gameVersion.setRevision(obj.value(JsonKeys::JSONKEY_VERSION_REVISION).toInt());
    m_gameVersion.setSufix(obj.value(JsonKeys::JSONKEY_VERSION_SUFIX).toString());
    m_players = obj.value(JsonKeys::JSONKEY_JOINEDPLAYERS).toInt();
    m_maxPlayers = obj.value(JsonKeys::JSONKEY_MAXPLAYERS).toInt();
    QJsonObject mods = obj.value(JsonKeys::JSONKEY_USEDMODS).toObject();
    for (const auto & mod : mods)
    {
        m_Mods.append(mod.toString());
    }
    m_description = obj.value(JsonKeys::JSONKEY_DESCRIPTION).toString();
    m_mapName = obj.value(JsonKeys::JSONKEY_NAME).toString();
    m_slaveName = obj.value(JsonKeys::JSONKEY_SLAVENAME).toString();
    m_locked = obj.value(JsonKeys::JSONKEY_HASPASSWORD).toBool();
    m_uuid = obj.value(JsonKeys::JSONKEY_UUID).toString().toLongLong();
    m_currentPlayer = obj.value(JsonKeys::JSONKEY_CURRENTPLAYER).toString();
    m_runningGame = obj.value(JsonKeys::JSONKEY_RUNNINGGAME).toBool();
    QJsonArray usernames = obj.value(JsonKeys::JSONKEY_USERNAMES).toArray();
    for (const auto & username : usernames)
    {
        m_playerNames.append(username.toString());
    }
    QJsonArray onlineData = obj.value(JsonKeys::JSONKEY_ONLINEINFO).toArray();
    for (const auto & data : onlineData)
    {
        m_onlineData.append(data.toBool());
    }
    m_minimapData = obj.value(JsonKeys::JSONKEY_MINIMAPDATA).toString().toLocal8Bit();
    m_observers = obj.value(JsonKeys::JSONKEY_MATCHOBSERVERCOUNT).toInt();
    m_maxObservers = obj.value(JsonKeys::JSONKEY_MATCHMAXOBSERVERCOUNT).toInt();
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

bool NetworkGameData::getRunningGame() const
{
    return m_runningGame;
}

void NetworkGameData::setRunningGame(bool newRunningGame)
{
    m_runningGame = newRunningGame;
}

QString NetworkGameData::getCurrentPlayer() const
{
    return m_currentPlayer;
}

void NetworkGameData::setCurrentPlayer(const QString & newCurrentPlayer)
{
    m_currentPlayer = newCurrentPlayer;
}

QVector<bool> NetworkGameData::getOnlineData() const
{
    return m_onlineData;
}

void NetworkGameData::setOnlineData(const QVector<bool> & newOnlineData)
{
    m_onlineData = newOnlineData;
}

QByteArray & NetworkGameData::getMinimapData()
{
    return m_minimapData;
}

void NetworkGameData::setMinimapData(const QByteArray & newMinimapData)
{
    m_minimapData = newMinimapData;
}

qint32 NetworkGameData::getObservers() const
{
    return m_observers;
}

void NetworkGameData::setObservers(qint32 newObservers)
{
    m_observers = newObservers;
}

qint32 NetworkGameData::getMaxObservers() const
{
    return m_maxObservers;
}

void NetworkGameData::setMaxObservers(qint32 newMaxObservers)
{
    m_maxObservers = newMaxObservers;
}

const GameVersion &  NetworkGameData::getGameVersion() const
{
    return m_gameVersion;
}

void NetworkGameData::setGameVersion(const GameVersion & newGameVersion)
{
    m_gameVersion = newGameVersion;
}

const QString &NetworkGameData::getSlaveSecondaryAddress() const
{
    return m_slaveSecondaryAddress;
}

void NetworkGameData::setSlaveSecondaryAddress(const QString &newSlaveSecondaryAddress)
{
    m_slaveSecondaryAddress = newSlaveSecondaryAddress;
}
