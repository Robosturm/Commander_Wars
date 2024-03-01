#include "network/matchmakingcoordinator.h"
#include "network/mainserver.h"
#include "network/JsonKeys.h"

#include "coreengine/interpreter.h"

#include <QDirIterator>
#include <QJsonArray>

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

void MatchMakingCoordinator::fixPlayerTable(const QString &player)
{
    auto &database = m_mainServer->getDatabase();
    QString command = QString("SELECT ") +
                      MainServer::SQL_METADATA +
                      " from " + MainServer::SQL_TABLE_PLAYERDATA + player +
                      " WHERE " + MainServer::SQL_COID +
                      " = '%';";
    QSqlQuery query(database);
    query.exec(command);
    if (MainServer::sqlQueryFailed(query))
    {
         CONSOLE_PRINT("Fixing user table for player " + player, GameConsole::eDEBUG);
        command = QString("DROP TABLE ") + MainServer::SQL_TABLE_PLAYERDATA + player;
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
    for (const auto &entry : resultInfo)
    {
        QJsonObject data = entry.toObject();
        QString player = data.value(JsonKeys::JSONKEY_PLAYER).toString();
        if (!player.isEmpty())
        {
            fixPlayerTable(player);
            GameEnums::GameResult result = static_cast<GameEnums::GameResult>(data.value(JsonKeys::JSONKEY_GAMERESULT).toInt());
            QJsonArray coInfo = data.value(JsonKeys::JSONKEY_COS).toArray();
            for (auto co : coInfo)
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
                        QString selectCommand = QString("SELECT ") +
                                                MainServer::SQL_GAMESLOST + ", " +
                                                MainServer::SQL_GAMESWON + ", " +
                                                MainServer::SQL_GAMESMADE + ", " +
                                                MainServer::SQL_GAMESDRAW +
                                                " from " + MainServer::SQL_TABLE_PLAYERDATA + player +
                                                " WHERE " + MainServer::SQL_COID +
                                                " = '" + coId + "';";
                        QSqlQuery query(database);
                        query.exec(selectCommand);
                        if (MainServer::sqlQueryFailed(query) || !query.first())
                        {

                            QString command = QString("INSERT INTO ") + MainServer::SQL_TABLE_PLAYERDATA + player + "(" +
                                              MainServer::SQL_COID + ", " +
                                              MainServer::SQL_GAMESMADE + ", " +
                                              MainServer::SQL_GAMESLOST + ", " +
                                              MainServer::SQL_GAMESWON + ", " +
                                              MainServer::SQL_GAMESDRAW + ", " +
                                              MainServer::SQL_METADATA +
                                              ") VALUES(" +
                                              "'" + coId + "'," +
                                              "0," +
                                              "0," +
                                              "0," +
                                              "0," +
                                              "''" +
                                              ")";
                            query.exec(command);
                            MainServer::sqlQueryFailed(query);
                        }
                        query.exec(selectCommand);
                        if (!MainServer::sqlQueryFailed(query) && query.first())
                        {

                            query.exec(QString("UPDATE ") + MainServer::SQL_TABLE_PLAYERDATA + player + " SET " +
                                       entryKey + " = " + QString::number(query.value(entryKey).toInt() + 1) + "  WHERE " +
                                       MainServer::SQL_COID + " = '" + coId + "';");
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
        matchInfo.insert(JsonKeys::JSONKEY_NAME, match->getDescription());
        matchInfo.insert(JsonKeys::JSONKEY_AUTOMATCHID, match->getMatchId());
        matchInfo.insert(JsonKeys::JSONKEY_SIGNEDUP, isInMatch);
        matchInfo.insert(JsonKeys::JSONKEY_SIGNUPCHANGEALLOWED, match->getIsSignUpChangeAllowed());
        matchInfo.insert(JsonKeys::JSONKEY_MMR, match->getMmr(playerId));
        matchInfo.insert(JsonKeys::JSONKEY_BRACKETGRAPHINFO, match->getBracketGraphInfo());

        if (match->getRunning())
        {
            preparingAutoMatches.append(matchInfo);
        }
        else
        {
            runningAutoMatches.append(matchInfo);
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
            m_autoMatchMakers[id]->setRunning(running);
            m_autoMatchMakers[id]->setActiveMatch(true);
        }
        else
        {
            QString filePath = dirIter.fileInfo().filePath();
            pInterpreter->openScript(filePath, true);
            m_autoMatchMakers[id] = MemoryManagement::create<AutoMatchMaker>(id, m_mainServer);
            m_autoMatchMakers[id]->setRunning(running);
            m_autoMatchMakers[id]->setActiveMatch(true);
        }
    }
}

void MatchMakingCoordinator::removeMatches()
{
    bool removed = true;
    while (removed)
    {
        removed = false;
        for (auto &match : m_autoMatchMakers)
        {
            if (!match->getActiveMatch())
            {
                constexpr qint32 MAX_INACTIVE_COUNT = 2 * 30;
                match->increaseNotActiveCounter();
                if (match->getNotActiveCounter() > MAX_INACTIVE_COUNT)
                {
                    m_autoMatchMakers.remove(match->getMatchId());
                }
            }
        }
    }
}
