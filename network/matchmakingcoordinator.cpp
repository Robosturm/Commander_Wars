#include "network/matchmakingcoordinator.h"
#include "network/mainserver.h"
#include "network/JsonKeys.h"

#include "coreengine/interpreter.h"

#include <QDirIterator>
#include <QJsonArray>
#include <QDateTime>
#include <QJsonDocument>

MatchMakingCoordinator::MatchMakingCoordinator(MainServer *parent)
    : QObject{parent},
      m_mainServer(parent)
{
}

void MatchMakingCoordinator::serializeObject(QDataStream &stream) const
{
    stream << getVersion();
    stream << static_cast<qint32>(m_autoMatchMakers.size());
    for (auto &autoMatchMakers : m_autoMatchMakers)
    {
        autoMatchMakers->serializeObject(stream);
    }
}

void MatchMakingCoordinator::deserializeObject(QDataStream &stream)
{
    qint32 version = 0;
    stream >> version;
    qint32 size = 0;
    stream >> size;
    for (qint32 i = 0; i < size; ++i)
    {
        spAutoMatchMaker matchMaker = MemoryManagement::create<AutoMatchMaker>("", m_mainServer);
        matchMaker->deserializeObject(stream);
        m_autoMatchMakers.insert(matchMaker->getMatchId(), matchMaker);
    }
}

AutoMatchMaker *MatchMakingCoordinator::getAutoMatchMaker(const QString &matchMaker)
{
    if (m_autoMatchMakers.contains(matchMaker))
    {
        return m_autoMatchMakers[matchMaker].get();
    }
    return nullptr;
}

void MatchMakingCoordinator::onSlaveInfoGameResult(quint64 socketID, const QJsonObject &objData)
{
    auto matchType = objData.value(JsonKeys::JSONKEY_MATCHTYPE).toString();
    updatePlayerMatchData(objData);
    if (!matchType.isEmpty())
    {
        if (!storeMatchResult(matchType, objData))
        {
            GameConsole::autoMatchLog(matchType, "Failed to persist completed match result",
                                      GameConsole::eERROR);
        }
        if (m_autoMatchMakers.contains(matchType))
        {
            m_autoMatchMakers[matchType]->onNewMatchResultData(objData);
        }
        else
        {
            CONSOLE_PRINT("Unknown match type result received, maybe the auto match script got deleted while games were runnig: " + matchType, GameConsole::eERROR);
        }
    }
    m_mainServer->despawnSlave(socketID);
}

bool MatchMakingCoordinator::storeMatchResult(const QString &matchId, const QJsonObject &objData)
{
    QSqlQuery query(m_mainServer->getDatabase());
    query.prepare("INSERT INTO autoMatchResults (matchId, gameId, createdAt, resultJson) "
                  "VALUES (?, ?, ?, ?);");
    query.addBindValue(matchId);
    query.addBindValue(objData.value(JsonKeys::JSONKEY_REPLAYFILE).toString());
    query.addBindValue(QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs));
    query.addBindValue(QString::fromUtf8(QJsonDocument(objData).toJson(QJsonDocument::Compact)));
    return query.exec() && !MainServer::sqlQueryFailed(query);
}

void MatchMakingCoordinator::fixPlayerTable(const QString &player)
{
    auto &database = m_mainServer->getDatabase();
    QString safeTable = MainServer::SQL_TABLE_PLAYERDATA + GlobalUtils::toHash512(player);
    QString command = QString("SELECT ") +
                      MainServer::SQL_METADATA +
                      " from " + safeTable +
                      " WHERE " + MainServer::SQL_COID +
                      " = ?;";
    QSqlQuery query(database);
    query.prepare(command);
    query.addBindValue("%");
    query.exec();
    if (MainServer::sqlQueryFailed(query))
    {
         CONSOLE_PRINT("Fixing user table for player " + player, GameConsole::eDEBUG);
        command = QString("DROP TABLE ") + safeTable;
        query.exec(command);
        if (!MainServer::sqlQueryFailed(query))
        {
            m_mainServer->createUserTable(player);
        }
    }
}

void MatchMakingCoordinator::updatePlayerMatchData(const QJsonObject &objData)
{
    auto &database = m_mainServer->getDatabase();
    QJsonArray resultInfo = objData.value(JsonKeys::JSONKEY_GAMERESULTARRAY).toArray();
    for (const auto & entry : std::as_const(resultInfo))
    {
        QJsonObject data = entry.toObject();
        QString player = data.value(JsonKeys::JSONKEY_PLAYER).toString();
        if (!player.isEmpty())
        {
            fixPlayerTable(player);
            GameEnums::GameResult result = static_cast<GameEnums::GameResult>(data.value(JsonKeys::JSONKEY_GAMERESULT).toInt());
            QJsonArray coInfo = data.value(JsonKeys::JSONKEY_COS).toArray();
            for (const auto & co : std::as_const(coInfo))
            {
                QString coId = co.toString();
                if (!coId.isEmpty())
                {
                    QString entryKey = "";
                    switch (result)
                    {
                    case GameEnums::GameResult::GameResult_Lost:
                    {
                        entryKey = MainServer::SQL_GAMESLOST;
                        break;
                    }
                    case GameEnums::GameResult::GameResult_Draw:
                    {
                        entryKey = MainServer::SQL_GAMESDRAW;
                        break;
                    }
                    case GameEnums::GameResult::GameResult_Won:
                    {
                        entryKey = MainServer::SQL_GAMESWON;
                        break;
                    }
                    }
                    if (!entryKey.isEmpty())
                    {
                        QString safeTable = MainServer::SQL_TABLE_PLAYERDATA + GlobalUtils::toHash512(player);
                        QSqlQuery selectQuery(database);
                        selectQuery.prepare(QString("SELECT ") +
                                                MainServer::SQL_GAMESLOST + ", " +
                                                MainServer::SQL_GAMESWON + ", " +
                                                MainServer::SQL_GAMESMADE + ", " +
                                                MainServer::SQL_GAMESDRAW +
                                                " from " + safeTable +
                                                " WHERE " + MainServer::SQL_COID + " = ?;");
                        selectQuery.addBindValue(coId);
                        selectQuery.exec();
                        if (MainServer::sqlQueryFailed(selectQuery) || !selectQuery.first())
                        {

                            QSqlQuery insertQuery(database);
                            insertQuery.prepare(QString("INSERT INTO ") + safeTable + "(" +
                                              MainServer::SQL_COID + ", " +
                                              MainServer::SQL_GAMESMADE + ", " +
                                              MainServer::SQL_GAMESLOST + ", " +
                                              MainServer::SQL_GAMESWON + ", " +
                                              MainServer::SQL_GAMESDRAW + ", " +
                                              MainServer::SQL_METADATA +
                                              ") VALUES(?, 0, 0, 0, 0, '');");
                            insertQuery.addBindValue(coId);
                            insertQuery.exec();
                            MainServer::sqlQueryFailed(insertQuery);
                        }
                        selectQuery.exec();
                        if (!MainServer::sqlQueryFailed(selectQuery) && selectQuery.first())
                        {
                            QSqlQuery updateQuery(database);
                            updateQuery.prepare(QString("UPDATE ") + safeTable + " SET " +
                                       entryKey + " = ? WHERE " +
                                       MainServer::SQL_COID + " = ?;");
                            updateQuery.addBindValue(selectQuery.value(entryKey).toInt() + 1);
                            updateQuery.addBindValue(coId);
                            updateQuery.exec();
                        }
                    }
                }
            }
        }
    }
}

void MatchMakingCoordinator::getMatchMakingData(const QString &playerId, QJsonObject &objData)
{
    QJsonArray preparingAutoMatches;
    QJsonArray runningAutoMatches;
    for (auto &match : m_autoMatchMakers)
    {
        bool isInMatch = match->getSignedUp(playerId);
        QJsonObject matchInfo;
        matchInfo.insert(JsonKeys::JSONKEY_NAME, match->getName());
        matchInfo.insert(JsonKeys::JSONKEY_DESCRIPTION, match->getDescription());
        matchInfo.insert(JsonKeys::JSONKEY_AUTOMATCHID, match->getMatchId());
        matchInfo.insert(JsonKeys::JSONKEY_SIGNEDUP, isInMatch);
        matchInfo.insert(JsonKeys::JSONKEY_SIGNUPCHANGEALLOWED, match->getIsSignUpChangeAllowed());
        matchInfo.insert(JsonKeys::JSONKEY_AUTOMATCHSTATE, match->getState());
        matchInfo.insert(JsonKeys::JSONKEY_MMR, match->getMmr(playerId));
        matchInfo.insert(JsonKeys::JSONKEY_BRACKETGRAPHINFO, match->getBracketGraphInfo());

        if (match->getRunning())
        {
            runningAutoMatches.append(matchInfo);
        }
        else
        {
            preparingAutoMatches.append(matchInfo);
        }
    }
    objData.insert(JsonKeys::JSONKEY_PREPARINGAUTOMATCHES, preparingAutoMatches);
    objData.insert(JsonKeys::JSONKEY_RUNNINGAUTOMATCHES, runningAutoMatches);
}

void MatchMakingCoordinator::periodicTasks()
{
    for (auto &match : m_autoMatchMakers)
    {
        match->setActiveMatch(false);
    }
    QString path = "server/preparingAutoMatches";
    loadAutomatches(path, false);
    path = "server/runningAutoMatches";
    loadAutomatches(path, true);
    for (auto &match : m_autoMatchMakers)
    {
        match->createGamesPeriodic();
    }
    removeMatches();
}

void MatchMakingCoordinator::loadAutomatches(QString &path, bool running)
{
    Interpreter *pInterpreter = Interpreter::getInstance();
    QStringList filter;
    filter << "*.js";
    QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
    while (dirIter.hasNext())
    {
        dirIter.next();
        QString id = dirIter.fileInfo().fileName().split(".").at(0).toUpper();
        if (m_autoMatchMakers.contains(id))
        {
            const QString filePath = dirIter.fileInfo().filePath();
            const qint64 modifiedTime = dirIter.fileInfo().lastModified().toMSecsSinceEpoch();
            if (m_scriptPaths.value(id) != filePath ||
                m_scriptModifiedTimes.value(id, -1) != modifiedTime)
            {
                if (pInterpreter->openScript(filePath, false))
                {
                    m_scriptPaths[id] = filePath;
                    m_scriptModifiedTimes[id] = modifiedTime;
                }
                else
                {
                    CONSOLE_PRINT("Failed to reload auto match script " + filePath +
                                  "; retaining the previous loaded script", GameConsole::eERROR);
                    GameConsole::autoMatchLog(id, "Failed to reload script " + filePath +
                                              "; retaining the previous script", GameConsole::eERROR);
                }
            }
            m_autoMatchMakers[id]->setRunning(running);
            m_autoMatchMakers[id]->setActiveMatch(true);
            m_autoMatchMakers[id]->updateStateFromScript();
        }
        else
        {
            QString filePath = dirIter.fileInfo().filePath();
            if (!pInterpreter->openScript(filePath, true))
            {
                CONSOLE_PRINT("Failed to load auto match script " + filePath, GameConsole::eERROR);
                GameConsole::autoMatchLog(id, "Failed to load script " + filePath, GameConsole::eERROR);
                continue;
            }
            m_autoMatchMakers[id] = MemoryManagement::create<AutoMatchMaker>(id, m_mainServer);
            m_autoMatchMakers[id]->setRunning(running);
            m_autoMatchMakers[id]->setActiveMatch(true);
            m_scriptPaths[id] = filePath;
            m_scriptModifiedTimes[id] = dirIter.fileInfo().lastModified().toMSecsSinceEpoch();
            m_autoMatchMakers[id]->updateStateFromScript();
        }
        m_mainServer->createMatchData(id);
    }
}

void MatchMakingCoordinator::removeMatches()
{
    constexpr qint32 MAX_INACTIVE_COUNT = 2 * 30;
    auto it = m_autoMatchMakers.begin();
    while (it != m_autoMatchMakers.end())
    {
        auto &match = it.value();
        if (!match->getActiveMatch())
        {
            match->increaseNotActiveCounter();
            if (match->getNotActiveCounter() > MAX_INACTIVE_COUNT)
            {
                const QString matchId = it.key();
                it = m_autoMatchMakers.erase(it);
                m_scriptPaths.remove(matchId);
                m_scriptModifiedTimes.remove(matchId);
                continue;
            }
        }
        ++it;
    }
}
