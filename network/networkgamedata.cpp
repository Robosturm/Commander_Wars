#include "networkgamedata.h"

#include "coreengine/filesupport.h"

NetworkGameData::NetworkGameData()
    : QObject()
{

}

void NetworkGameData::serializeObject(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << _players;
    pStream << _maxPlayers;
    Filesupport::writeVectorList(pStream, _Mods);
    pStream << description;
    pStream << _mapName;
    pStream << _gamePort;
}

void NetworkGameData::deserializeObject(QDataStream& pStream)
{
    qint32 version;
    pStream >> version;
    pStream >> _players;
    pStream >> _maxPlayers;
    _Mods = Filesupport::readVectorList<QString, QVector>(pStream);
    pStream >> description;
    pStream >> _mapName;
    pStream >> _gamePort;
}

quint16 NetworkGameData::getGamePort() const
{
    return _gamePort;
}

void NetworkGameData::setGamePort(const quint16 &gamePort)
{
    _gamePort = gamePort;
}

QString NetworkGameData::getMapName() const
{
    return _mapName;
}

void NetworkGameData::setMapName(const QString &mapName)
{
    _mapName = mapName;
}

QString NetworkGameData::getDescription() const
{
    return description;
}

void NetworkGameData::setDescription(const QString &value)
{
    description = value;
}

QVector<QString> NetworkGameData::getMods() const
{
    return _Mods;
}

void NetworkGameData::setMods(const QVector<QString> &Mods)
{
    _Mods = Mods;
}

qint32 NetworkGameData::getMaxPlayers() const
{
    return _maxPlayers;
}

void NetworkGameData::setMaxPlayers(const qint32 &maxPlayers)
{
    _maxPlayers = maxPlayers;
}

qint32 NetworkGameData::getPlayers() const
{
    return _players;
}

void NetworkGameData::setPlayers(const qint32 &players)
{
    _players = players;
}
