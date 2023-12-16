#include <QFile>
#include <QJsonDocument>

#include "network/mapfileserver.h"
#include "network/mainserver.h"
#include "network/JsonKeys.h"
#include "coreengine/settings.h"
#include "coreengine/globalutils.h"

MapFileServer::MapFileServer(MainServer *parent)
    : QObject{parent},
    m_mainServer(parent)
{
}

void MapFileServer::onMapUpload(quint64 socketID, const QJsonObject & objData)
{
    bool success = false;
    spTCPServer pServer = m_mainServer->getGameServer();
    if (pServer->getClient(socketID)->getIsActive() &&
        Settings::getInstance()->getAllowMapUpload())
    {
        QString filePath = objData.value(JsonKeys::JSONKEY_MAPPATH).toString();
        QString imagePath = filePath;
        imagePath.replace(".map", ".png");
        bool uploadAllowed = !QFile::exists(filePath) || sameUploader(objData);
        if ((filePath.startsWith("maps/") || filePath.startsWith("/maps")) &&
            filePath.endsWith(".map") &&
            uploadAllowed)
        {
            QByteArray mapArray = GlobalUtils::toByteArray(objData.value(JsonKeys::JSONKEY_MAPDATA).toArray());
            QBuffer buffer(&mapArray);
            buffer.open(QIODevice::ReadOnly);
            QDataStream stream(&buffer);
            qint32 version = 0;
            stream >> version;
            QByteArray mapMagic;
            stream >> mapMagic;
            if (mapMagic == GlobalUtils::MAP_MAGIC)
            {
                QFile::remove(filePath);
                QFile::remove(imagePath);
                QFile file(filePath);
                file.open(QIODevice::WriteOnly);
                QDataStream stream(&file);
                qint32 contentSize = mapArray.size();
                stream.writeRawData(mapArray.constData(), contentSize);
                file.close();
                QByteArray imageArray = GlobalUtils::toByteArray(objData.value(JsonKeys::JSONKEY_MINIMAPDATA).toArray());
                QImage image;
                QBuffer buffer(&imageArray);
                buffer.open(QIODevice::ReadOnly);
                image.load(&buffer, "PNG");
                image.save(imagePath);
                QString command =  QString("INSERT INTO ") + MainServer::SQL_TABLE_DOWNLOADMAPINFO + "(" +
                                  MainServer::SQL_MAPNAME + ", " +
                                  MainServer::SQL_MAPAUTHOR + ", " +
                                  MainServer::SQL_MAPPATH + ", " +
                                  MainServer::SQL_MAPIMAGEPATH + ", " +
                                  MainServer::SQL_MAPPLAYERS + ", " +
                                  MainServer::SQL_MAPWIDTH + ", " +
                                  MainServer::SQL_MAPHEIGHT + ", " +
                                  MainServer::SQL_MAPFLAGS + "," +
                                  MainServer::SQL_MAPUPLOADER + ", " +
                                  MainServer::SQL_MAPDOWNLOADCOUNT + ", " +
                                  MainServer::SQL_MAPUPLOADDATE + ", " +
                                  MainServer::SQL_MAPLASTDOWNLOADDATE +
                                  ") VALUES(" +
                                  "'" + objData.value(JsonKeys::JSONKEY_MAPNAME).toString() + "', " +
                                  "'" + objData.value(JsonKeys::JSONKEY_MAPAUTHOR).toString() + "', " +
                                  "'" + filePath + "', " +
                                  "'" + imagePath + "', " +
                                  QString::number(objData.value(JsonKeys::JSONKEY_MAPPLAYERS).toInt()) + ", " +
                                  QString::number(objData.value(JsonKeys::JSONKEY_MAPWIDTH).toInt()) + ", " +
                                  QString::number(objData.value(JsonKeys::JSONKEY_MAPHEIGHT).toInt()) + ", " +
                                  QString::number(objData.value(JsonKeys::JSONKEY_MAPFLAGS).toInteger()) + "," +
                                  "'" + objData.value(JsonKeys::JSONKEY_MAPUPLOADER).toString() + "', " +
                                  "0, " +
                                  "'" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "', " +
                                  "'" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "')";
                QSqlQuery query = m_mainServer->getDatabase().exec(command);
                if (MainServer::sqlQueryFailed(query))
                {
                    QFile::remove(filePath);
                    QFile::remove(imagePath);
                }
                else
                {
                    success = true;
                }
            }
        }
    }
    QString command = QString(NetworkCommands::MAPUPLOADRESPONSE);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_RESULT, success);
    QJsonDocument doc(data);
    CONSOLE_PRINT("Sending command " + doc.object().value(JsonKeys::JSONKEY_COMMAND).toString() + " to socket " + QString::number(socketID), GameConsole::eDEBUG);
    emit pServer->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);

}

bool MapFileServer::sameUploader(const QJsonObject & objData)
{
    bool success = false;
    QString filePath = objData.value(JsonKeys::JSONKEY_MAPPATH).toString();
    QSqlQuery query = m_mainServer->getDatabase().exec(QString("SELECT ") + MainServer::SQL_MAPUPLOADER +
                                                       " from " + MainServer::SQL_TABLE_DOWNLOADMAPINFO +
                                                       " WHERE " + MainServer::SQL_MAPPATH +
                                                       " = '" + filePath + "';");
    if (!MainServer::sqlQueryFailed(query) && query.first())
    {
        QString currentUploader = objData.value(JsonKeys::JSONKEY_MAPUPLOADER).toString();
        QString oldUploader = query.value(MainServer::SQL_MAPUPLOADER).toString();
        success = oldUploader == currentUploader;
        if (success)
        {
            QString command = QString("DELETE FROM ") + MainServer::SQL_TABLE_DOWNLOADMAPINFO + " WHERE " +
                              MainServer::SQL_MAPPATH + " = '" + filePath + "';";
            query = m_mainServer->getDatabase().exec(command);
            MainServer::sqlQueryFailed(query);
        }
    }
    return success;
}

void MapFileServer::onRequestFilteredMaps(quint64 socketID, const QJsonObject & objData)
{
    spTCPServer pServer = m_mainServer->getGameServer();
    QString command = QString(NetworkCommands::RECEIVEAVAILABLEMAPS);
    QJsonObject response;
    response.insert(JsonKeys::JSONKEY_COMMAND, command);
    QString filterCommand = QString("SELECT ") + MainServer::SQL_MAPPATH + ", " +
                            MainServer::SQL_MAPNAME + ", " +
                            MainServer::SQL_MAPAUTHOR + ", " +
                            MainServer::SQL_MAPPLAYERS + ", " +
                            MainServer::SQL_MAPWIDTH + ", " +
                            MainServer::SQL_MAPHEIGHT + ", " +
                            MainServer::SQL_MAPFLAGS + ", " +
                            MainServer::SQL_MAPUPLOADER + ", " +
                            MainServer::SQL_MAPIMAGEPATH +
                            " from " + MainServer::SQL_TABLE_DOWNLOADMAPINFO +
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
        filterCommand += QString(MainServer::SQL_MAPPATH) + " LIKE '%';";
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
        QJsonArray foundMaps;
        if (query.seek(startItem))
        {
            itemCount = startItem;
            do
            {
                ++itemCount;
                if (foundMaps.size() < itemsPerPage)
                {
                    QJsonObject mapData;
                    mapData.insert(JsonKeys::JSONKEY_MAPPATH, query.value(MainServer::SQL_MAPPATH).toString());
                    mapData.insert(JsonKeys::JSONKEY_MAPNAME, query.value(MainServer::SQL_MAPNAME).toString());
                    mapData.insert(JsonKeys::JSONKEY_MAPAUTHOR, query.value(MainServer::SQL_MAPAUTHOR).toString());
                    mapData.insert(JsonKeys::JSONKEY_MAPUPLOADER, query.value(MainServer::SQL_MAPUPLOADER).toString());
                    mapData.insert(JsonKeys::JSONKEY_MAPPLAYERS, query.value(MainServer::SQL_MAPPLAYERS).toInt());
                    mapData.insert(JsonKeys::JSONKEY_MAPWIDTH, query.value(MainServer::SQL_MAPWIDTH).toInt());
                    mapData.insert(JsonKeys::JSONKEY_MAPHEIGHT, query.value(MainServer::SQL_MAPHEIGHT).toInt());
                    mapData.insert(JsonKeys::JSONKEY_MAPFLAGS, query.value(MainServer::SQL_MAPFLAGS).toLongLong());
                    QImage image;
                    image.load(query.value(MainServer::SQL_MAPIMAGEPATH).toString());
                    QByteArray imageArray;
                    QBuffer buffer(&imageArray);
                    buffer.open(QIODevice::WriteOnly);
                    image.save(&buffer, "PNG");
                    buffer.close();
                    mapData.insert(JsonKeys::JSONKEY_MINIMAPDATA, GlobalUtils::toJsonArray(imageArray));
                    foundMaps.append(mapData);
                }
            } while (query.next());
        }
        else
        {
            itemCount = startItem - 1;
        }
        response.insert(JsonKeys::JSONKEY_MAPLIST, foundMaps);
    }
    response.insert(JsonKeys::JSONKEY_FOUNDITEMS, itemCount);
    response.insert(JsonKeys::JSONKEY_STARTINDEX, startItem);
    QJsonDocument doc(response);
    CONSOLE_PRINT("Sending command " + doc.object().value(JsonKeys::JSONKEY_COMMAND).toString() + " to socket " + QString::number(socketID), GameConsole::eDEBUG);
    emit pServer->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MapFileServer::addFlagFilterOption(QString & filterCommand, qint32 & filterCount, const QVector<MapFilter::FlagFilter> & filters)
{
    bool requiresOptional = true;
    for (const auto & filter : filters)
    {
        if (filter.isActive && !filter.isOptional)
        {
            requiresOptional = false;
            if (filterCount > 0)
            {
                filterCommand += " AND ";
            }
            filterCommand += QString(MainServer::SQL_MAPFLAGS) + " & " + QString::number(filter.flag) + " > 0";
            ++filterCount;
        }
    }
    bool initialAnd = false;
    if (requiresOptional)
    {
        for (const auto & filter : filters)
        {
            if (filter.isActive && filter.isOptional)
            {
                if (filterCount > 0 && !initialAnd)
                {
                    filterCommand += " AND ( ";
                    initialAnd = true;
                }
                else if (!initialAnd)
                {
                    filterCommand += " ( ";
                    initialAnd = true;
                }
                else
                {
                    filterCommand += " OR ";
                }
                filterCommand += QString(MainServer::SQL_MAPFLAGS) + " & " + QString::number(filter.flag) + " > 0";
                ++filterCount;
            }
        }
    }
    if (initialAnd)
    {
        filterCommand += ")";
    }
}

void MapFileServer::addFilterOption(QString & filterCommand, qint32 value, qint32 & filterCount, const char* const item, const char* const opCommand)
{
    if (value > 0)
    {
        if (filterCount > 0)
        {
            filterCommand += " AND ";
        }
        filterCommand += QString(item) + " " + opCommand + " "  + QString::number(value);
        ++filterCount;
    }
}

void MapFileServer::addFilterOption(QString & filterCommand, QString value, qint32 & filterCount, const char* const item)
{
    if (!value.isEmpty())
    {
        if (filterCount > 0)
        {
            filterCommand += " AND ";
        }
        filterCommand += QString(item) + " LIKE '"  + value + "%'";
        ++filterCount;
    }
}

void MapFileServer::onRequestDeleteMap(quint64 socketID, const QJsonObject & objData)
{
    spTCPServer pServer = m_mainServer->getGameServer();
    QString filter = QString(MainServer::SQL_MAPPATH) + " = '" + objData.value(JsonKeys::JSONKEY_MAPPATH).toString() + "' AND " +
                     MainServer::SQL_MAPUPLOADER + " = '" + objData.value(JsonKeys::JSONKEY_USERNAME).toString() + "'";
    bool success = removeMapFromServer(filter);
    QString command = QString(NetworkCommands::SERVERDELETERESPONSE);
    QJsonObject response;
    response.insert(JsonKeys::JSONKEY_COMMAND, command);
    response.insert(JsonKeys::JSONKEY_RESULT, success);
    QJsonDocument doc(response);
    CONSOLE_PRINT("Sending command " + doc.object().value(JsonKeys::JSONKEY_COMMAND).toString() + " to socket " + QString::number(socketID), GameConsole::eDEBUG);
    emit pServer->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MapFileServer::onRequestDownloadMap(quint64 socketID, const QJsonObject & objData)
{
    spTCPServer pServer = m_mainServer->getGameServer();
    bool success = false;
    QString command = QString(NetworkCommands::FILEDOWNLOAD);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    QString mapPath = objData.value(JsonKeys::JSONKEY_MAPPATH).toString();
    if (QFile::exists(mapPath))
    {
        QSqlQuery query = m_mainServer->getDatabase().exec(QString("SELECT ") +
                                                           MainServer::SQL_MAPPATH + ", " +
                                                           MainServer::SQL_MAPDOWNLOADCOUNT + ", " +
                                                           MainServer::SQL_MAPLASTDOWNLOADDATE +
                                                           " from " + MainServer::SQL_TABLE_DOWNLOADMAPINFO +
                                                           " WHERE " + MainServer::SQL_MAPPATH +
                                                           " = '" + mapPath + "';");
        if (!MainServer::sqlQueryFailed(query) && query.first())
        {
            // update internal data
            qint32 downloadCount = query.value(MainServer::SQL_MAPDOWNLOADCOUNT).toInt();
            auto changeQuery = m_mainServer->getDatabase().exec(QString("UPDATE ") + MainServer::SQL_TABLE_DOWNLOADMAPINFO + " SET " +
                                                                MainServer::SQL_MAPDOWNLOADCOUNT + " = '" + QString::number(downloadCount + 1) + ", " +
                                                                MainServer::SQL_MAPLASTDOWNLOADDATE + " = '" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "' WHERE " +
                                                                MainServer::SQL_MAPPATH + " = '" + objData.value(JsonKeys::JSONKEY_MAPPATH).toString() + "';");
            MainServer::sqlQueryFailed(changeQuery);
            // load map data and send it to client
            QFile file(mapPath);
            file.open(QIODevice::ReadOnly);
            data.insert(JsonKeys::JSONKEY_MAPDATA, GlobalUtils::toJsonArray(file.readAll()));
            success = true;
        }
    }
    data.insert(JsonKeys::JSONKEY_DOWNLOADRESULT, success);
    data.insert(JsonKeys::JSONKEY_MAPPATH, mapPath);        
    QJsonDocument doc(data);
    CONSOLE_PRINT("Sending command " + doc.object().value(JsonKeys::JSONKEY_COMMAND).toString() + " to socket " + QString::number(socketID), GameConsole::eDEBUG);
    emit pServer->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

bool MapFileServer::removeMapFromServer(const QString & sqlFilter)
{
    QString filterCommand = QString("SELECT ") + MainServer::SQL_MAPPATH + ", " +
                            MainServer::SQL_MAPIMAGEPATH +
                            " from " + MainServer::SQL_TABLE_DOWNLOADMAPINFO +
                            " WHERE " + sqlFilter + ";";
    QSqlQuery query = m_mainServer->getDatabase().exec(filterCommand);
    bool success = !MainServer::sqlQueryFailed(query);
    if (success)
    {
        do
        {
            QFile::remove(query.value(MainServer::SQL_MAPPATH).toString());
            QFile::remove(query.value(MainServer::SQL_MAPIMAGEPATH).toString());
        } while (query.next());
        QString command = QString("DELETE FROM ") + MainServer::SQL_TABLE_PLAYERS + " WHERE " + sqlFilter + ";";
        query = m_mainServer->getDatabase().exec(command);
        MainServer::sqlQueryFailed(query);
    }
    return success;
}
