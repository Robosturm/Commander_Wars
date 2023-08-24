#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "network/automatchmaker.h"
#include "network/mainserver.h"
#include "network/JsonKeys.h"

#include "coreengine/interpreter.h"
#include "coreengine/gameconsole.h"

#include "multiplayer/multiplayermenu.h"

AutoMatchMaker::AutoMatchMaker(const QString & matchId, MainServer * mainServer)
    : m_matchId(matchId),
      m_mainServer(*mainServer)
{
    Interpreter::setCppOwnerShip(this);
}

QString AutoMatchMaker::getName()
{
    QString name;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(&m_mainServer)});
    auto erg =  pInterpreter->doFunction(m_matchId, "getName", args);
    if (erg.isString())
    {
        name = erg.toString();
    }
    return name;
}

QString AutoMatchMaker::getDescription()
{
    QString description;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(&m_mainServer)});
    auto erg =  pInterpreter->doFunction(m_matchId, "getDescription", args);
    if (erg.isString())
    {
        description = erg.toString();
    }
    return description;
}

qint32 AutoMatchMaker::getNotActiveCounter() const
{
    return m_notActiveCounter;
}

void AutoMatchMaker::increaseNotActiveCounter()
{
    ++m_notActiveCounter;
}

QString AutoMatchMaker::getMatchId() const
{
    return m_matchId;
}

void AutoMatchMaker::onNewMatchResultData(const QJsonObject & objData)
{
    QJsonArray winnerInfo  = objData.value(JsonKeys::JSONKEY_GAMERESULTARRAY).toArray();
    QStringList usernames;
    QVector<qint32> results;
    for (const auto & player : winnerInfo)
    {
        auto data = player.toObject();
        usernames.append(data.value(JsonKeys::JSONKEY_PLAYER).toString());
        results.append(data.value(JsonKeys::JSONKEY_GAMERESULT).toInt());
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->arraytoJSValue(usernames),
                       pInterpreter->arraytoJSValue(results),
                       pInterpreter->newQObject(&m_mainServer)});
    pInterpreter->doFunction(m_matchId, "onNewMatchResultData", args);
}

void AutoMatchMaker::createNewGame(const QString players)
{
    spNetworkInterface dummy;
    Multiplayermenu multiplayermenu(dummy, "", Multiplayermenu::NetworkMode::Host);
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(&multiplayermenu),
                       players,
                       pInterpreter->newQObject(&m_mainServer)});
    pInterpreter->doFunction(m_matchId, "onCreateNewGame", args);
    QString saveFile = "savegames/" + m_matchId + QString::number(m_matchCounter) + ".lsav";
    auto doc = multiplayermenu.doSaveLobbyState(saveFile, "");
    QJsonObject objData = doc.object();
    m_mainServer.onSlaveInfoDespawning(0, objData);
    ++m_matchCounter;
}

QStringList AutoMatchMaker::getOpponentsForPlayer(const QString player, qint32 playerCount, qint32 mmrSearchRange)
{
    qint32 mmr = getMmr(player);
    QStringList players;
    if (mmr >= 0)
    {
        auto & database = m_mainServer.getDatabase();
        QSqlQuery query = database.exec(QString("SELECT ") + MainServer::SQL_USERNAME +
                                        " from " + MainServer::SQL_TABLE_MATCH_DATA + m_matchId +
                                        " WHERE " +
                                        MainServer::SQL_MINGAMES + " > 0 AND " +
                                        MainServer::SQL_MAXGAMES + " <= " + MainServer::SQL_RUNNINGGAMES + " AND " +
                                        MainServer::SQL_MMR + " >= '" + QString::number(mmr - mmrSearchRange) + "' AND " +
                                        MainServer::SQL_MMR + " <= '" + QString::number(mmr + mmrSearchRange) + "';");
        if (!MainServer::sqlQueryFailed(query) &&
            query.first())
        {
            do
            {
                QString sqlPlayer = query.value(MainServer::SQL_USERNAME).toString();
                if (sqlPlayer != player)
                {
                    players.append(player);
                }
            }
            while (query.next());
        }
    }
    return players;
}

bool AutoMatchMaker::setMatchHistoryData(const QString player, QString historyData)
{
    auto & database = m_mainServer.getDatabase();
    auto changeQuery = database.exec(QString("UPDATE ") + MainServer::SQL_TABLE_MATCH_DATA + m_matchId + " SET " +
                                      MainServer::SQL_MATCHHISTORY + " = " + historyData + "  WHERE " +
                                      MainServer::SQL_USERNAME + " = '" + player + "';");
    return MainServer::sqlQueryFailed(changeQuery);
}

QString AutoMatchMaker::getMatchHistoryData(const QString player)
{
    auto & database = m_mainServer.getDatabase();
    QSqlQuery query = database.exec(QString("SELECT ") + MainServer::SQL_MATCHHISTORY +
                                    " from " + MainServer::SQL_TABLE_MATCH_DATA + m_matchId +
                                    " WHERE " + MainServer::SQL_USERNAME +
                                    " = '" + player + "';");
    if (!MainServer::sqlQueryFailed(query) &&
        query.first())
    {
        return query.value(MainServer::SQL_MATCHHISTORY).toString();
    }
    return "";
}

QString AutoMatchMaker::getMatchMetaData(const QString player)
{
    auto & database = m_mainServer.getDatabase();
    QSqlQuery query = database.exec(QString("SELECT ") + MainServer::SQL_METADATA +
                                    " from " + MainServer::SQL_TABLE_MATCH_DATA + m_matchId +
                                    " WHERE " + MainServer::SQL_USERNAME +
                                    " = '" + player + "';");
    if (!MainServer::sqlQueryFailed(query) &&
        query.first())
    {
        return query.value(MainServer::SQL_METADATA).toString();
    }
    return "";
}

bool AutoMatchMaker::setMatchMetaData(const QString player, QString metaData)
{
    auto & database = m_mainServer.getDatabase();
    auto changeQuery = database.exec(QString("UPDATE ") + MainServer::SQL_TABLE_MATCH_DATA + m_matchId + " SET " +
                                      MainServer::SQL_METADATA + " = " + metaData + "  WHERE " +
                                      MainServer::SQL_USERNAME + " = '" + player + "';");
    return MainServer::sqlQueryFailed(changeQuery);
}

void AutoMatchMaker::updateMmr(const QString player1, const QString player2, qint32 maxEloChange, GameEnums::GameResult gameResultForPlayer1)
{
    qint32 mmr1 = getMmr(player1);
    qint32 mmr2 = getMmr(player2);
    if (mmr1 >= 0 &&
        mmr2 >= 0)
    {
        float diff = static_cast<float>(mmr1 - mmr2);
        float ea1 = 1.0f / (1.0f + qPow(10.0f, diff / 400.0f));
        float ea2 = (1.0f - ea1);
        switch (gameResultForPlayer1)
        {
            case GameEnums::GameResult_Won:
            {
                mmr1 = mmr1 + maxEloChange * (1.0f - ea1);
                mmr2 = mmr2 + maxEloChange * (0.0f - ea2);
                break;
            }
            case GameEnums::GameResult_Lost:
            {
                mmr1 = mmr1 + maxEloChange * (0.0f - ea1);
                mmr2 = mmr2 + maxEloChange * (1.0f - ea2);
                break;
            }
            case GameEnums::GameResult_Draw:
            {
                mmr1 = mmr1 + maxEloChange * (0.5f - ea1);
                mmr2 = mmr2 + maxEloChange * (0.5f - ea2);
                break;
            }
        }
        if (setMmr(player1, mmr1) ||
            setMmr(player2, mmr2))
        {
                CONSOLE_PRINT("Failed to update mmr's for match rounds " + m_matchId +
                          " for player " + player1 + " to " + QString::number(mmr1) +
                          " and for player " + player2 + " to " + QString::number(mmr2), GameConsole::eERROR);
        }
    }
    else
    {
        CONSOLE_PRINT("Failed to read mmr's for match rounds " + m_matchId +
                      " for player " + player1 +
                      " and for player " + player2, GameConsole::eERROR);
    }
}

qint32 AutoMatchMaker::getMmr(const QString player)
{
    auto & database = m_mainServer.getDatabase();
    QSqlQuery query = database.exec(QString("SELECT ") + MainServer::SQL_MMR +
                                    " from " + MainServer::SQL_TABLE_MATCH_DATA + m_matchId +
                                    " WHERE " + MainServer::SQL_USERNAME +
                                    " = '" + player + "';");
    if (!MainServer::sqlQueryFailed(query) &&
        query.first())
    {
        return query.value(MainServer::SQL_MMR).toInt();
    }
    return -1;
}

bool AutoMatchMaker::setMmr(const QString player, qint32 mmr)
{
    auto & database = m_mainServer.getDatabase();
    auto changeQuery = database.exec(QString("UPDATE ") + MainServer::SQL_TABLE_MATCH_DATA + m_matchId + " SET " +
                                      MainServer::SQL_MMR + " = " + QString::number(mmr) + "  WHERE " +
                                      MainServer::SQL_USERNAME + " = '" + player + "';");
    return MainServer::sqlQueryFailed(changeQuery);
}

qint32 AutoMatchMaker::getMinGames(const QString player)
{
    auto & database = m_mainServer.getDatabase();
    QSqlQuery query = database.exec(QString("SELECT ") + MainServer::SQL_MINGAMES +
                                    " from " + MainServer::SQL_TABLE_MATCH_DATA + m_matchId +
                                    " WHERE " + MainServer::SQL_USERNAME +
                                    " = '" + player + "';");
    if (!MainServer::sqlQueryFailed(query) &&
        query.first())
    {
        return query.value(MainServer::SQL_MINGAMES).toInt();
    }
    return -1;
}

qint32 AutoMatchMaker::getRunningGames(const QString player)
{
    auto & database = m_mainServer.getDatabase();
    QSqlQuery query = database.exec(QString("SELECT ") + MainServer::SQL_RUNNINGGAMES +
                                    " from " + MainServer::SQL_TABLE_MATCH_DATA + m_matchId +
                                    " WHERE " + MainServer::SQL_USERNAME +
                                    " = '" + player + "';");
    if (!MainServer::sqlQueryFailed(query) &&
        query.first())
    {
        return query.value(MainServer::SQL_RUNNINGGAMES).toInt();
    }
    return -1;
}

bool AutoMatchMaker::setRunningGames(const QString player, qint32 count)
{
    auto & database = m_mainServer.getDatabase();
    auto changeQuery = database.exec(QString("UPDATE ") + MainServer::SQL_TABLE_MATCH_DATA + m_matchId + " SET " +
                                      MainServer::SQL_RUNNINGGAMES + " = " + QString::number(count) + "  WHERE " +
                                      MainServer::SQL_USERNAME + " = '" + player + "';");
    return MainServer::sqlQueryFailed(changeQuery);
}

qint32 AutoMatchMaker::getMaxGames(const QString player)
{
    auto & database = m_mainServer.getDatabase();
    QSqlQuery query = database.exec(QString("SELECT ") + MainServer::SQL_MAXGAMES +
                                    " from " + MainServer::SQL_TABLE_MATCH_DATA + m_matchId +
                                    " WHERE " + MainServer::SQL_USERNAME +
                                    " = '" + player + "';");
    if (!MainServer::sqlQueryFailed(query) &&
        query.first())
    {
        return query.value(MainServer::SQL_MAXGAMES).toInt();
    }
    return -1;
}

void AutoMatchMaker::serializeObject(QDataStream& stream) const
{
    stream << getVersion();
    stream << m_matchId;
    stream << m_matchCounter;
    m_Variables.serializeObject(stream);
    stream << m_running;
}

void AutoMatchMaker::deserializeObject(QDataStream& stream)
{
    qint32 version;
    stream >> version;
    stream >> m_matchId;
    stream >> m_matchCounter;
    m_Variables.deserializeObject(stream);
    if (version > 1)
    {
        stream >> m_running;
    }
}

void AutoMatchMaker::onNewPlayerData(const QJsonObject & objData)
{
    QString player = objData.value(JsonKeys::JSONKEY_USERNAME).toString();
    qint32 minGames = objData.value(JsonKeys::JSONKEY_MINMATCHGAMES).toInt();
    qint32 maxGames = objData.value(JsonKeys::JSONKEY_MAXMATCHGAMES).toInt();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       player,
                       minGames,
                       maxGames,
                       pInterpreter->newQObject(&m_mainServer)});
    pInterpreter->doFunction(m_matchId, "onNewPlayerData", args);
}

bool AutoMatchMaker::getSignedUp(const QString playerId)
{
    auto & database = m_mainServer.getDatabase();
    QSqlQuery query = database.exec(QString("SELECT ") + MainServer::SQL_SIGNEDUP +
                                    " from " + MainServer::SQL_TABLE_MATCH_DATA + m_matchId +
                                    " WHERE " + MainServer::SQL_USERNAME +
                                    " = '" + playerId + "';");
    if (!MainServer::sqlQueryFailed(query) &&
        query.first())
    {
        return query.value(MainServer::SQL_SIGNEDUP).toBool();
    }
    return false;
}

bool AutoMatchMaker::doNewPlayerData(const QString & player, qint32 minGames, qint32 maxGames, const QString & metaData, qint32 startMmr)
{
    bool result = false;
    auto & database = m_mainServer.getDatabase();
    if (getMmr(player) < 0)
    {
        QString command = QString("INSERT INTO ") + MainServer::SQL_TABLE_MATCH_DATA + m_matchId + " (" +
                          MainServer::SQL_USERNAME + " TEXT PRIMARY KEY, " +
                          MainServer::SQL_MMR + ", " +
                          MainServer::SQL_MINGAMES + ", " +
                          MainServer::SQL_MAXGAMES + ", " +
                          MainServer::SQL_RUNNINGGAMES + "," +
                          MainServer::SQL_METADATA + "," +
                          MainServer::SQL_MATCHHISTORY +
                          ") VALUES(" +
                          "'" + player + "'," +
                          QString::number(startMmr) + "," +
                          QString::number(minGames) + "," +
                          QString::number(maxGames) + "," +
                          "'" + metaData + "'," +
                          "''" +
                          ")";
        auto query = database.exec(command);
        result = !m_mainServer.sqlQueryFailed(query);
    }
    else
    {
        auto changeQuery = database.exec(QString("UPDATE ") + MainServer::SQL_TABLE_MATCH_DATA + m_matchId + " SET " +
                                        MainServer::SQL_MINGAMES + " = " + QString::number(minGames) + ", " +
                                        MainServer::SQL_MINGAMES + " = '" + metaData + "', " +
                                        MainServer::SQL_MAXGAMES + " = " + QString::number(maxGames) + " WHERE " +
                                        MainServer::SQL_USERNAME + " = '" + player + "';");
        result = !m_mainServer.sqlQueryFailed(changeQuery);
    }
    return result;
}

bool AutoMatchMaker::getActiveMatch() const
{
    return m_activeMatch;
}

void AutoMatchMaker::setActiveMatch(bool newActiveMatch)
{
    m_activeMatch = newActiveMatch;
    if (m_activeMatch)
    {
        m_notActiveCounter = 0;
    }
}

bool AutoMatchMaker::getIsSignUpChangeAllowed()
{
    bool isSignUpChangeAllowed = false;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(&m_mainServer)});
    auto erg =  pInterpreter->doFunction(m_matchId, "getIsSignUpChangeAllowed", args);
    if (erg.isBool())
    {
        isSignUpChangeAllowed = erg.toBool();
    }
    return isSignUpChangeAllowed;
}

QJsonObject AutoMatchMaker::getBracketGraphInfo()
{
    QJsonObject graphInfo;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(&m_mainServer)});
    auto erg =  pInterpreter->doFunction(m_matchId, "getBracketGraphInfo", args);
    if (erg.isString())
    {
        graphInfo = QJsonDocument::fromJson(erg.toString().toLocal8Bit()).object();
    }
    return graphInfo;
}

bool AutoMatchMaker::getRunning() const
{
    return m_running;
}

void AutoMatchMaker::setRunning(bool newRunning)
{
    m_running = newRunning;
}

QString AutoMatchMaker::getBracketGraphInfoId()
{
    return JsonKeys::JSONKEY_BRACKETGRAPHINFO;
}

QString AutoMatchMaker::getBracketGraphPreviousMatchId()
{
    return JsonKeys::JSONKEY_BRACKETGRAPHPREVIOUSMATCH;
}

QString AutoMatchMaker::getBracketGraphPreviousWinnersId()
{
    return JsonKeys::JSONKEY_BRACKETGRAPHWINNERS;
}

QString AutoMatchMaker::getBracketGraphPreviousPlayersId()
{
    return JsonKeys::JSONKEY_BRACKETGRAPHPLAYERS;
}
