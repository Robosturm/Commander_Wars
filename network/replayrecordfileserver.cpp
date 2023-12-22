#include "network/replayrecordfileserver.h"
#include "multiplayer/networkcommands.h"
#include "network/JsonKeys.h"
#include "network/mainserver.h"
#include "coreengine/settings.h"
#include "game/gamerecording/replayrecorder.h"

#include <QJsonObject>
#include <QJsonDocument>

class MainServer;

ReplayRecordFileserver::ReplayRecordFileserver(MainServer *parent)
    : m_mainServer(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &ReplayRecordFileserver::onDeleteOldReplays, Qt::QueuedConnection);
    connect(m_mainServer, &MainServer::sigOnRequestFilePacket, this, &ReplayRecordFileserver::onRequestFilePacket, Qt::QueuedConnection);

    m_timer.setSingleShot(false);
    m_timer.start(60 * 1000 * 60);
}

void ReplayRecordFileserver::onSlaveInfoGameResult(quint64 socketID, const QJsonObject &objData)
{
    addRecordToDatabase(objData);
}

void ReplayRecordFileserver::addRecordToDatabase(const QString & filePath)
{
    if (QFile::exists(filePath))
    {
        QJsonObject objData;
        objData.insert(JsonKeys::JSONKEY_REPLAYFILE, filePath);
        QFile file(filePath);
        file.open(QFile::ReadOnly);
        QDataStream stream(&file);
        QJsonObject recordInfo;
        if (ReplayRecorder::readRecordInfo(stream, recordInfo))
        {
            objData.insert(JsonKeys::JSONKEY_MAPNAME, recordInfo.value(JsonKeys::JSONKEY_MAPNAME).toString());
            objData.insert(JsonKeys::JSONKEY_MAPAUTHOR, recordInfo.value(JsonKeys::JSONKEY_MAPAUTHOR).toString());
            objData.insert(JsonKeys::JSONKEY_MAPPLAYERS, recordInfo.value(JsonKeys::JSONKEY_PLAYERDATA).toArray().size());
            objData.insert(JsonKeys::JSONKEY_MAPWIDTH, recordInfo.value(JsonKeys::JSONKEY_MAPWIDTH).toInt());
            objData.insert(JsonKeys::JSONKEY_MAPHEIGHT, recordInfo.value(JsonKeys::JSONKEY_MAPHEIGHT).toInt());
            objData.insert(JsonKeys::JSONKEY_MAPFLAGS, recordInfo.value(JsonKeys::JSONKEY_MAPFLAGS).toInteger());
            addRecordToDatabase(objData);
        }
    }
}

void ReplayRecordFileserver::addRecordToDatabase(const QJsonObject &objData)
{
    QString recordFile = objData.value(JsonKeys::JSONKEY_REPLAYFILE).toString();
    if (!recordFile.isEmpty())
    {
        QString command =  QString("INSERT INTO ") + MainServer::SQL_TABLE_REPLAYINFO + "(" +
                          MainServer::SQL_MAPNAME + ", " +
                          MainServer::SQL_MAPAUTHOR + ", " +
                          MainServer::SQL_REPLAYPATH + ", " +
                          MainServer::SQL_MAPPLAYERS + ", " +
                          MainServer::SQL_MAPWIDTH + ", " +
                          MainServer::SQL_MAPHEIGHT + ", " +
                          MainServer::SQL_MAPFLAGS + "," +
                          MainServer::SQL_REPLAYCREATIONTIME +
                          ") VALUES (" +
                          "'" + objData.value(JsonKeys::JSONKEY_MAPNAME).toString() + "', " +
                          "'" + objData.value(JsonKeys::JSONKEY_MAPAUTHOR).toString() + "', " +
                          "'" + recordFile + "', " +
                          QString::number(objData.value(JsonKeys::JSONKEY_MAPPLAYERS).toInt()) + ", " +
                          QString::number(objData.value(JsonKeys::JSONKEY_MAPWIDTH).toInt()) + ", " +
                          QString::number(objData.value(JsonKeys::JSONKEY_MAPHEIGHT).toInt()) + ", " +
                          QString::number(objData.value(JsonKeys::JSONKEY_MAPFLAGS).toInteger()) + ", " +
                          QString::number(QDateTime::currentSecsSinceEpoch()) + ")";
        QSqlQuery query = m_mainServer->getDatabase().exec(command);
        if (MainServer::sqlQueryFailed(query))
        {
            CONSOLE_PRINT("Unable to create record entry for record " + recordFile, GameConsole::eERROR);
        }
    }
}

void ReplayRecordFileserver::onDeleteOldReplays()
{
    QString sqlFilter = QString(MainServer::SQL_REPLAYCREATIONTIME) + " < " + QString::number(QDateTime::currentSecsSinceEpoch() - Settings::getInstance()->getReplayDeleteTime().count());
    QString filterCommand = QString("SELECT ") + MainServer::SQL_REPLAYPATH +
                            " from " + MainServer::SQL_TABLE_REPLAYINFO +
                            " WHERE " + sqlFilter + ";";
    QSqlQuery query = m_mainServer->getDatabase().exec(filterCommand);
    bool success = !MainServer::sqlQueryFailed(query);
    if (success && query.first())
    {
        do
        {
            QFile::remove(query.value(MainServer::SQL_REPLAYPATH).toString());
        } while (query.next());
        QString command = QString("DELETE FROM ") + MainServer::SQL_TABLE_REPLAYINFO + " WHERE " + sqlFilter + ";";
        query = m_mainServer->getDatabase().exec(command);
        MainServer::sqlQueryFailed(query);
    }
}

void ReplayRecordFileserver::onRequestFilteredRecords(quint64 socketID, const QJsonObject & objData)
{
    spTCPServer pServer = m_mainServer->getGameServer();
    QString command = QString(NetworkCommands::RECEIVEAVAILABLERECORDS);
    QJsonObject response;
    response.insert(JsonKeys::JSONKEY_COMMAND, command);
    QString filterCommand = QString("SELECT ") + MainServer::SQL_REPLAYPATH + ", " +
                            MainServer::SQL_MAPNAME + ", " +
                            MainServer::SQL_MAPAUTHOR + ", " +
                            MainServer::SQL_MAPPLAYERS + ", " +
                            MainServer::SQL_MAPWIDTH + ", " +
                            MainServer::SQL_MAPHEIGHT + ", " +
                            MainServer::SQL_MAPFLAGS + ", " +
                            MainServer::SQL_REPLAYCREATIONTIME +
                            " from " + MainServer::SQL_TABLE_REPLAYINFO +
                            " WHERE ";
    qint32 filterCount = 0;
    MapFilter mapFilter;
    mapFilter.fromJson(objData);
    addFilterOption(filterCommand, mapFilter.getMinHeight(), filterCount, MainServer::SQL_MAPHEIGHT, ">=");
    addFilterOption(filterCommand, mapFilter.getMaxHeight(), filterCount, MainServer::SQL_MAPHEIGHT, "<=");
    addFilterOption(filterCommand, mapFilter.getMinWidth(), filterCount, MainServer::SQL_MAPWIDTH, ">=");
    addFilterOption(filterCommand, mapFilter.getMaxWidth(), filterCount, MainServer::SQL_MAPWIDTH, "<=");
    addFilterOption(filterCommand, mapFilter.getMinPlayer(), filterCount, MainServer::SQL_MAPPLAYERS, ">=");
    addFilterOption(filterCommand, mapFilter.getMaxPlayer(), filterCount, MainServer::SQL_MAPPLAYERS, "<=");
    addFilterOption(filterCommand, mapFilter.getMapName(), filterCount, MainServer::SQL_MAPNAME);
    addFilterOption(filterCommand, mapFilter.getMapName(), filterCount, MainServer::SQL_MAPAUTHOR);
    addFlagFilterOption(filterCommand, filterCount, mapFilter.getFlagFilter());
    if (filterCount == 0)
    {
        filterCommand += QString(MainServer::SQL_REPLAYPATH) + " LIKE '%';";
    }
    else
    {
        filterCommand +=  ";";
    }
    QSqlQuery query = m_mainServer->getDatabase().exec(filterCommand);
    bool success = !MainServer::sqlQueryFailed(query);
    success = success && query.first();
    bool active = query.isActive() && query.isSelect();
    qint32 startItem = objData.value(JsonKeys::JSONKEY_STARTINDEX).toInt();
    qint32 itemsPerPage = objData.value(JsonKeys::JSONKEY_ITEMSPERPAGE).toInt();
    qint32 itemCount = 0;
    if (success && active)
    {
        QJsonArray foundRecords;
        if (query.seek(startItem))
        {
            itemCount = startItem;
            do
            {
                ++itemCount;
                if (foundRecords.size() < itemsPerPage)
                {
                    auto path = query.value(MainServer::SQL_REPLAYPATH).toString();
                    if (QFile::exists(path))
                    {
                        QJsonObject mapData;
                        mapData.insert(JsonKeys::JSONKEY_REPLAYFILE, path);
                        mapData.insert(JsonKeys::JSONKEY_MAPNAME, query.value(MainServer::SQL_MAPNAME).toString());
                        mapData.insert(JsonKeys::JSONKEY_MAPAUTHOR, query.value(MainServer::SQL_MAPAUTHOR).toString());
                        mapData.insert(JsonKeys::JSONKEY_MAPPLAYERS, query.value(MainServer::SQL_MAPPLAYERS).toInt());
                        mapData.insert(JsonKeys::JSONKEY_MAPWIDTH, query.value(MainServer::SQL_MAPWIDTH).toInt());
                        mapData.insert(JsonKeys::JSONKEY_MAPHEIGHT, query.value(MainServer::SQL_MAPHEIGHT).toInt());
                        mapData.insert(JsonKeys::JSONKEY_MAPFLAGS, query.value(MainServer::SQL_MAPFLAGS).toLongLong());
                        QFile file(path);
                        file.open(QFile::ReadOnly);
                        QDataStream stream(&file);
                        QJsonObject jsonRecordInfo;
                        ReplayRecorder::readRecordInfo(stream, jsonRecordInfo);
                        mapData.insert(JsonKeys::JSONKEY_REPLAYRECORDHEADER, jsonRecordInfo);
                        foundRecords.append(mapData);
                    }
                }
            } while (query.next());
        }
        else
        {
            itemCount = startItem - 1;
        }
        response.insert(JsonKeys::JSONKEY_RECORDLIST, foundRecords);
    }
    response.insert(JsonKeys::JSONKEY_FOUNDITEMS, itemCount);
    response.insert(JsonKeys::JSONKEY_STARTINDEX, startItem);
    QJsonDocument doc(response);
    CONSOLE_PRINT("Sending command " + doc.object().value(JsonKeys::JSONKEY_COMMAND).toString() + " to socket " + QString::number(socketID), GameConsole::eDEBUG);
    emit pServer->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void ReplayRecordFileserver::onRequestDownloadRecord(quint64 socketID, const QJsonObject & objData)
{
    spTCPServer pServer = m_mainServer->getGameServer();
    spFilePeer filePeer = MemoryManagement::create<FilePeer>(pServer.get(), objData.value(JsonKeys::JSONKEY_REPLAYFILE).toString(), socketID);
    m_filePeers[socketID] = filePeer;
    filePeer->startUpload();
}

void ReplayRecordFileserver::onRequestFilePacket(quint64 socketID, const QJsonObject & objData)
{
    if (m_filePeers.contains(socketID))
    {
        if (m_filePeers[socketID]->sendNextPacket())
        {
            m_filePeers.erase(socketID);
        }
    }
}
