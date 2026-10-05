#include <QJsonObject>
#include <QJsonValueRef>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QSqlError>
#include <cmath>
#include <limits>

#include "network/automatchmaker.h"
#include "network/mainserver.h"
#include "network/JsonKeys.h"
#include "network/elocalculator.h"

#include "coreengine/interpreter.h"
#include "coreengine/gameconsole.h"

#include "multiplayer/multiplayermenu.h"

// cases to consider
// handle slaves despawning due to timeout

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
        QString username = data.value(JsonKeys::JSONKEY_PLAYER).toString();
        usernames.append(username);
        results.append(data.value(JsonKeys::JSONKEY_GAMERESULT).toInt());
        qint32 runningGames = getRunningGames(username);
        if (runningGames > 0 && !setRunningGames(username, runningGames - 1))
        {
            GameConsole::autoMatchLog(m_matchId,
                                      "Failed to decrement active games for player " + username,
                                      GameConsole::eERROR);
        }
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->arraytoJSValue(usernames),
                       pInterpreter->arraytoJSValue(results),
                       pInterpreter->newQObject(&m_mainServer)});
    pInterpreter->doFunction(m_matchId, "onNewMatchResultData", args);
    GameConsole::autoMatchLog(m_matchId, "Processed a completed match result.", GameConsole::eINFO);
}

QStringList AutoMatchMaker::getSignedUpPlayers()
{
    QStringList players;
    auto & database = m_mainServer.getDatabase();
    QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
    QSqlQuery query(database);
    query.prepare(QString("SELECT ") + MainServer::SQL_USERNAME +
                  " FROM " + safeTable +
                  " WHERE " + MainServer::SQL_SIGNEDUP + " = true AND " +
                  MainServer::SQL_MAXGAMES + " > " + MainServer::SQL_RUNNINGGAMES +
                  " ORDER BY " + MainServer::SQL_USERNAME + ";");
    if (!query.exec() || MainServer::sqlQueryFailed(query))
    {
        GameConsole::autoMatchLog(m_matchId, "Failed to list signed-up players: " +
                                  query.lastError().text(), GameConsole::eERROR);
        return players;
    }
    while (query.next())
    {
        players.append(query.value(MainServer::SQL_USERNAME).toString());
    }
    return players;
}

void AutoMatchMaker::createGamesPeriodic()
{
    if (m_state != State::ActiveWithSignUp && m_state != State::ActiveWithNoSignUp)
    {
        return;
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(&m_mainServer)});
    pInterpreter->doFunction(m_matchId, "onCreateNewGames", args);
}

bool AutoMatchMaker::createNewGame(const QStringList players, const QStringList modList)
{
    if ((m_state != State::ActiveWithSignUp && m_state != State::ActiveWithNoSignUp) ||
        players.isEmpty())
    {
        GameConsole::autoMatchLog(m_matchId, "Rejected game creation request due to inactive state or insufficient players",
                                  GameConsole::eWARNING);
        return false;
    }
    for (qint32 i = 0; i < players.size(); ++i)
    {
        const QString &player = players[i];
        if (player.isEmpty() || players.indexOf(player) != i ||
            !getSignedUp(player) || getRunningGames(player) < 0 ||
            getMaxGames(player) <= getRunningGames(player))
        {
            GameConsole::autoMatchLog(m_matchId,
                                      "Rejected game creation request for unavailable or duplicate player " + player,
                                      GameConsole::eWARNING);
            return false;
        }
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue scriptObject = pInterpreter->globalObject().property(m_matchId);
    if (!scriptObject.property("onCreateNewGame").isCallable())
    {
        GameConsole::autoMatchLog(m_matchId, "Script does not define onCreateNewGame", GameConsole::eERROR);
        return false;
    }
    spNetworkInterface dummy;
    Multiplayermenu multiplayermenu(dummy, "", Multiplayermenu::NetworkMode::Host);
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(&multiplayermenu),
                       pInterpreter->arraytoJSValue(players),
                       pInterpreter->newQObject(&m_mainServer)});
    QJSValue result = pInterpreter->doFunction(m_matchId, "onCreateNewGame", args);
    if (result.isError())
    {
        GameConsole::autoMatchLog(m_matchId, "Game configuration script failed; no lobby was created",
                                  GameConsole::eERROR);
        return false;
    }
    GameMap *map = multiplayermenu.getCurrentMap();
    if (map == nullptr)
    {
        GameConsole::autoMatchLog(m_matchId, "Game setup script did not select a map", GameConsole::eERROR);
        return false;
    }
    if (map->getPlayerCount() < 2)
    {
        GameConsole::autoMatchLog(m_matchId, "Selected map has fewer than two player slots",
                                  GameConsole::eERROR);
        return false;
    }
    PlayerSelection *playerSelection = multiplayermenu.getPlayerSelection();
    playerSelection->setMap(map);
    for (const QString &player : players)
    {
        bool assigned = false;
        for (qint32 playerIndex = 0; playerIndex < map->getPlayerCount(); ++playerIndex)
        {
            Player *mapPlayer = map->getPlayer(playerIndex);
            if (mapPlayer != nullptr &&
                mapPlayer->getPlayerNameId() == player &&
                mapPlayer->getControlType() == GameEnums::AiTypes_Human)
            {
                assigned = true;
                break;
            }
        }
        if (!assigned)
        {
            GameConsole::autoMatchLog(m_matchId,
                                      "Game setup did not assign signed-up player " + player +
                                      " to a human slot", GameConsole::eERROR);
            return false;
        }
    }
    map->getGameRules()->setMatchType(m_matchId);
    map->getGameRules()->setAutoMatch(true);
    QString saveFile = "savegames/" + m_matchId + QString::number(m_matchCounter) + ".lsav";
    auto doc = multiplayermenu.doSaveLobbyState(saveFile, "");
    QJsonObject objData = doc.object();
    if (objData.isEmpty())
    {
        GameConsole::autoMatchLog(m_matchId, "Game lobby save returned empty data; no lobby was registered",
                                  GameConsole::eERROR);
        QFile::remove(saveFile);
        return false;
    }
    objData.remove(JsonKeys::JSONKEY_USEDMODS);
    QJsonObject mods;
    for (qint32 i = 0; i < modList.size(); ++i)
    {
        mods.insert(JsonKeys::JSONKEY_MOD + QString::number(i), modList[i]);
    }
    objData.insert(JsonKeys::JSONKEY_USEDMODS, mods);
    auto &database = m_mainServer.getDatabase();
    if (!database.transaction())
    {
        GameConsole::autoMatchLog(m_matchId,
                                  "Could not start transaction to reserve players for a new game",
                                  GameConsole::eERROR);
        QFile::remove(saveFile);
        return false;
    }
    for (const QString &player : players)
    {
        if (!setRunningGames(player, getRunningGames(player) + 1))
        {
            database.rollback();
            QFile::remove(saveFile);
            GameConsole::autoMatchLog(m_matchId,
                                      "Could not reserve player " + player + " for the new game",
                                      GameConsole::eERROR);
            return false;
        }
    }
    if (!database.commit())
    {
        database.rollback();
        QFile::remove(saveFile);
        GameConsole::autoMatchLog(m_matchId, "Could not commit player reservations for the new game",
                                  GameConsole::eERROR);
        return false;
    }
    m_mainServer.onSlaveInfoDespawning(0, objData);
    ++m_matchCounter;
    GameConsole::autoMatchLog(m_matchId, "Created suspended game lobby for " + players.join(", "),
                              GameConsole::eINFO);
    return true;
}

QStringList AutoMatchMaker::getOpponentsForPlayer(const QString player, qint32 mmrSearchRange)
{
    qint32 mmr = getMmr(player);
    QStringList players;
    if (mmr >= 0)
    {
        auto & database = m_mainServer.getDatabase();
        QString safeTable =MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
        QSqlQuery query(database);
        query.prepare(QString("SELECT ") + MainServer::SQL_USERNAME +
                   " from " + safeTable +
                   " WHERE " +
                   MainServer::SQL_MAXGAMES + " > 0 AND " +
                   MainServer::SQL_MAXGAMES + " > " + MainServer::SQL_RUNNINGGAMES + " AND " +
                   MainServer::SQL_SIGNEDUP + " = true AND " +
                   MainServer::SQL_MMR + " >= ? AND " + MainServer::SQL_MMR + " <= ?;");
        query.addBindValue(mmr - mmrSearchRange);
        query.addBindValue(mmr + mmrSearchRange);
        query.exec();
        if (!MainServer::sqlQueryFailed(query) &&
            query.first())
        {
            do
            {
                QString sqlPlayer = query.value(MainServer::SQL_USERNAME).toString();
                if (sqlPlayer != player)
                {
                    players.append(sqlPlayer);
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
    QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
    QSqlQuery changeQuery(database);
    changeQuery.prepare(QString("UPDATE ") + safeTable + " SET " +
                     MainServer::SQL_MATCHHISTORY + " = ? WHERE " +
                     MainServer::SQL_USERNAME + " = ?;");
    changeQuery.addBindValue(historyData);
    changeQuery.addBindValue(player);
    return changeQuery.exec() && changeQuery.numRowsAffected() == 1 &&
           !MainServer::sqlQueryFailed(changeQuery);
}

QString AutoMatchMaker::getMatchHistoryData(const QString player)
{
    auto & database = m_mainServer.getDatabase();
    QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
    QSqlQuery query(database);
    query.prepare(QString("SELECT ") + MainServer::SQL_MATCHHISTORY +
               " from " + safeTable +
               " WHERE " + MainServer::SQL_USERNAME + " = ?;");
    query.addBindValue(player);
    query.exec();
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
    QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
    QSqlQuery query(database);
    query.prepare(QString("SELECT ") + MainServer::SQL_METADATA +
               " from " + safeTable +
               " WHERE " + MainServer::SQL_USERNAME + " = ?;");
    query.addBindValue(player);
    query.exec();
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
    QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
    QSqlQuery changeQuery(database);
    changeQuery.prepare(QString("UPDATE ") + safeTable + " SET " +
                                      MainServer::SQL_METADATA + " = ? WHERE " +
                                      MainServer::SQL_USERNAME + " = ?;");
    changeQuery.addBindValue(metaData);
    changeQuery.addBindValue(player);
    return changeQuery.exec() && changeQuery.numRowsAffected() == 1 &&
           !MainServer::sqlQueryFailed(changeQuery);
}

void AutoMatchMaker::updateMmr(const QString player1, const QString player2, qint32 maxEloChange, GameEnums::GameResult gameResultForPlayer1)
{
    if (maxEloChange < 0)
    {
        GameConsole::autoMatchLog(m_matchId, "Rejected Elo update with a negative K-factor",
                                  GameConsole::eERROR);
        return;
    }
    qint32 mmr1 = getMmr(player1);
    qint32 mmr2 = getMmr(player2);
    if (mmr1 >= 0 &&
        mmr2 >= 0)
    {
        double score1 = 0.0;
        switch (gameResultForPlayer1)
        {
            case GameEnums::GameResult_Won:
            {
                score1 = 1.0;
                break;
            }
            case GameEnums::GameResult_Lost:
            {
                score1 = 0.0;
                break;
            }
            case GameEnums::GameResult_Draw:
            {
                score1 = 0.5;
                break;
            }
            default:
            {
                GameConsole::autoMatchLog(m_matchId, "Rejected Elo update with an unknown game result",
                                          GameConsole::eERROR);
                return;
            }
        }
        auto updatedMmr1 = EloCalculator::updatedRating(mmr1, mmr2, maxEloChange, score1);
        auto updatedMmr2 = EloCalculator::updatedRating(mmr2, mmr1, maxEloChange, 1.0 - score1);
        if (!updatedMmr1 || !updatedMmr2)
        {
            GameConsole::autoMatchLog(m_matchId, "Elo calculator rejected the match rating update",
                                      GameConsole::eERROR);
            return;
        }
        mmr1 = *updatedMmr1;
        mmr2 = *updatedMmr2;
        bool player1Updated = setMmr(player1, mmr1);
        bool player2Updated = setMmr(player2, mmr2);
        if (!player1Updated || !player2Updated)
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

void AutoMatchMaker::updateMmrAgainstRating(const QString &player, qint32 opponentMmr,
                                            qint32 maxEloChange, GameEnums::GameResult result)
{
    qint32 playerMmr = getMmr(player);
    if (playerMmr < 0 || opponentMmr < 0 || maxEloChange < 0)
    {
        GameConsole::autoMatchLog(m_matchId, "Rejected invalid human-versus-AI Elo update for player " + player,
                                  GameConsole::eERROR);
        return;
    }
    float score = 0.0f;
    switch (result)
    {
    case GameEnums::GameResult_Won:
        score = 1.0f;
        break;
    case GameEnums::GameResult_Draw:
        score = 0.5f;
        break;
    case GameEnums::GameResult_Lost:
        score = 0.0f;
        break;
    default:
        GameConsole::autoMatchLog(m_matchId, "Rejected human-versus-AI Elo update with unknown result",
                                  GameConsole::eERROR);
        return;
    }
    auto updatedMmr = EloCalculator::updatedRating(playerMmr, opponentMmr, maxEloChange, score);
    if (!updatedMmr || !setMmr(player, *updatedMmr))
    {
        GameConsole::autoMatchLog(m_matchId, "Failed human-versus-AI Elo update for player " + player,
                                  GameConsole::eERROR);
    }
}

qint32 AutoMatchMaker::getMmr(const QString player)
{
    auto & database = m_mainServer.getDatabase();
    QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
    QSqlQuery query(database);
    query.prepare(QString("SELECT ") + MainServer::SQL_MMR +
               " from " + safeTable +
               " WHERE " + MainServer::SQL_USERNAME + " = ?;");
    query.addBindValue(player);
    query.exec();
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
    QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
    QSqlQuery changeQuery(database);
    changeQuery.prepare(QString("UPDATE ") + safeTable + " SET " +
                     MainServer::SQL_MMR + " = ? WHERE " +
                     MainServer::SQL_USERNAME + " = ?;");
    changeQuery.addBindValue(mmr);
    changeQuery.addBindValue(player);
    return changeQuery.exec() && changeQuery.numRowsAffected() == 1 &&
           !MainServer::sqlQueryFailed(changeQuery);
}

qint32 AutoMatchMaker::getMinGames(const QString player)
{
    auto & database = m_mainServer.getDatabase();
    QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
    QSqlQuery query(database);
    query.prepare(QString("SELECT ") + MainServer::SQL_MINGAMES +
               " from " + safeTable +
               " WHERE " + MainServer::SQL_USERNAME + " = ?;");
    query.addBindValue(player);
    query.exec();
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
    QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
    QSqlQuery query(database);
    query.prepare(QString("SELECT ") + MainServer::SQL_RUNNINGGAMES +
               " from " + safeTable +
               " WHERE " + MainServer::SQL_USERNAME + " = ?;");
    query.addBindValue(player);
    query.exec();
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
    QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
    QSqlQuery changeQuery(database);
    changeQuery.prepare(QString("UPDATE ") + safeTable + " SET " +
                     MainServer::SQL_RUNNINGGAMES + " = ? WHERE " +
                     MainServer::SQL_USERNAME + " = ?;");
    changeQuery.addBindValue(count);
    changeQuery.addBindValue(player);
    return count >= 0 && changeQuery.exec() && changeQuery.numRowsAffected() == 1 &&
           !MainServer::sqlQueryFailed(changeQuery);
}

qint32 AutoMatchMaker::getMaxGames(const QString player)
{
    auto & database = m_mainServer.getDatabase();
    QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
    QSqlQuery query(database);
    query.prepare(QString("SELECT ") + MainServer::SQL_MAXGAMES +
               " from " + safeTable +
               " WHERE " + MainServer::SQL_USERNAME + " = ?;");
    query.addBindValue(player);
    query.exec();
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
    stream << static_cast<qint32>(m_state);
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
    if (version > 2)
    {
        qint32 state = 0;
        stream >> state;
        if (state >= static_cast<qint32>(State::InCreation) &&
            state <= static_cast<qint32>(State::ActiveWithNoSignUp))
        {
            m_state = static_cast<State>(state);
        }
    }
    else
    {
        m_state = m_running ? State::ActiveWithSignUp : State::SignUp;
    }
}

bool AutoMatchMaker::onNewPlayerData(const QJsonObject & objData)
{
    QString player = objData.value(JsonKeys::JSONKEY_USERNAME).toString();
    QJsonValue minValue = objData.value(JsonKeys::JSONKEY_MINMATCHGAMES);
    QJsonValue maxValue = objData.value(JsonKeys::JSONKEY_MAXMATCHGAMES);
    qint32 minGames = minValue.toInt(-1);
    qint32 maxGames = maxValue.toInt(-1);
    if (player.isEmpty() || !minValue.isDouble() || !maxValue.isDouble() ||
        minValue.toDouble() != minGames || maxValue.toDouble() != maxGames ||
        minGames < 0 || maxGames < minGames)
    {
        CONSOLE_PRINT("Invalid auto match sign-up data for matchmaker " + m_matchId +
                      " and player " + player, GameConsole::eWARNING);
        GameConsole::autoMatchLog(m_matchId, "Rejected invalid sign-up data for player " + player,
                                  GameConsole::eWARNING);
        return false;
    }
    if (!getIsSignUpChangeAllowed())
    {
        GameConsole::autoMatchLog(m_matchId, "Rejected sign-up because sign-ups are closed for player " + player,
                                  GameConsole::eINFO);
        return false;
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(&m_mainServer)});
    QJSValue startMmrValue = pInterpreter->doFunction(m_matchId, "getStartMmr", args);
    const double startMmr = startMmrValue.toNumber();
    if (!startMmrValue.isNumber() || !std::isfinite(startMmr) || startMmr < 0 ||
        startMmr > std::numeric_limits<qint32>::max())
    {
        GameConsole::autoMatchLog(m_matchId, "Script returned an invalid starting MMR for player " + player,
                                  GameConsole::eERROR);
        return false;
    }
    if (!doNewPlayerData(player, minGames, maxGames, "", qRound(startMmr)))
    {
        CONSOLE_PRINT("Failed to save auto match sign-up data for matchmaker " + m_matchId +
                      " and player " + player, GameConsole::eERROR);
        GameConsole::autoMatchLog(m_matchId, "Failed to save sign-up data for player " + player,
                                  GameConsole::eERROR);
        return false;
    }
    auto & database = m_mainServer.getDatabase();
    QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
    QSqlQuery signupQuery(database);
    signupQuery.prepare(QString("UPDATE ") + safeTable + " SET " +
                        MainServer::SQL_SIGNEDUP + " = true WHERE " +
                        MainServer::SQL_USERNAME + " = ?;");
    signupQuery.addBindValue(player);
    signupQuery.exec();
    if (MainServer::sqlQueryFailed(signupQuery))
    {
        CONSOLE_PRINT("Failed to mark player " + player + " signed up for matchmaker " + m_matchId,
                      GameConsole::eERROR);
        GameConsole::autoMatchLog(m_matchId, "Failed to mark player " + player + " as signed up",
                                  GameConsole::eERROR);
        return false;
    }
    GameConsole::autoMatchLog(m_matchId, "Player " + player + " signed up for " +
                              QString::number(minGames) + "-" + QString::number(maxGames) +
                              " concurrent games.", GameConsole::eINFO);
    QJSValueList args1({pInterpreter->newQObject(this),
                       player,
                       minGames,
                       maxGames,
                       pInterpreter->newQObject(&m_mainServer)});

    pInterpreter->doFunction(m_matchId, "onNewPlayerData", args1);
    return true;
}

bool AutoMatchMaker::withdrawPlayer(const QString &playerId)
{
    if (playerId.isEmpty() || !getSignedUp(playerId) || !getIsSignUpChangeAllowed())
    {
        return false;
    }
    if (getRunningGames(playerId) > 0)
    {
        GameConsole::autoMatchLog(m_matchId,
                                  "Rejected withdrawal for player " + playerId +
                                  " because they have running games", GameConsole::eWARNING);
        return false;
    }
    auto &database = m_mainServer.getDatabase();
    QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
    QSqlQuery query(database);
    query.prepare(QString("UPDATE ") + safeTable + " SET " +
                  MainServer::SQL_SIGNEDUP + " = false WHERE " +
                  MainServer::SQL_USERNAME + " = ?;");
    query.addBindValue(playerId);
    if (!query.exec() || MainServer::sqlQueryFailed(query) || query.numRowsAffected() != 1)
    {
        GameConsole::autoMatchLog(m_matchId, "Failed to withdraw player " + playerId,
                                  GameConsole::eERROR);
        return false;
    }
    GameConsole::autoMatchLog(m_matchId, "Player " + playerId + " withdrew from the matchmaker",
                              GameConsole::eINFO);
    return true;
}

bool AutoMatchMaker::getSignedUp(const QString playerId)
{
    auto & database = m_mainServer.getDatabase();
    QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
    QSqlQuery query(database);
    query.prepare(QString("SELECT ") + MainServer::SQL_SIGNEDUP +
               " from " + safeTable +
               " WHERE " + MainServer::SQL_USERNAME + " = ?;");
    query.addBindValue(playerId);
    query.exec();
    if (!MainServer::sqlQueryFailed(query) &&
        query.first())
    {
        return query.value(MainServer::SQL_SIGNEDUP).toBool();
    }
    return false;
}

bool AutoMatchMaker::doNewPlayerData(const QString & player, qint32 minGames, qint32 maxGames, const QString & metaData, qint32 startMmr)
{
    if (player.isEmpty() || minGames < 0 || maxGames < minGames)
    {
        CONSOLE_PRINT("Invalid player data for matchmaker " + m_matchId + " and player " + player,
                      GameConsole::eWARNING);
        return false;
    }
    bool result = false;
    auto & database = m_mainServer.getDatabase();
    if (getMmr(player) < 0)
    {
        auto & database = m_mainServer.getDatabase();
        QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
        QSqlQuery query(database);
        query.prepare(QString("INSERT INTO ") + safeTable + " (" +
                          MainServer::SQL_USERNAME + ", " +
                          MainServer::SQL_MMR + " , " +
                          MainServer::SQL_MINGAMES + ", " +
                          MainServer::SQL_MAXGAMES + ", " +
                          MainServer::SQL_RUNNINGGAMES + "," +
                          MainServer::SQL_METADATA + "," +
                          MainServer::SQL_SIGNEDUP + "," +
                          MainServer::SQL_MATCHHISTORY +
                          ") VALUES(?, ?, ?, ?, 0, ?, false, '');");
        query.addBindValue(player);
        query.addBindValue(startMmr);
        query.addBindValue(minGames);
        query.addBindValue(maxGames);
        query.addBindValue(metaData);
        query.exec();
        result = !m_mainServer.sqlQueryFailed(query);
    }
    else
    {
        auto & database = m_mainServer.getDatabase();
        QString safeTable = MainServer::SQL_TABLE_MATCH_DATA + GlobalUtils::toHash512(m_matchId);
        QSqlQuery changeQuery(database);
        changeQuery.prepare(QString("UPDATE ") + safeTable + " SET " +
                         MainServer::SQL_MINGAMES + " = ?, " +
                         MainServer::SQL_METADATA + " = ? , " +
                         MainServer::SQL_MAXGAMES + " = ? WHERE " +
                         MainServer::SQL_USERNAME + " = ?;");
        changeQuery.addBindValue(minGames);
        changeQuery.addBindValue(metaData);
        changeQuery.addBindValue(maxGames);
        changeQuery.addBindValue(player);
        changeQuery.exec();
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
    if (m_state != State::SignUp && m_state != State::ActiveWithSignUp)
    {
        return false;
    }
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

QString AutoMatchMaker::readDataFromJson(const QString & filePath)
{
    if (QFile::exists(filePath))
    {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            return stream.readAll();
        }
        else
        {
            CONSOLE_PRINT("Failed to open file " + file.fileName(), GameConsole::eERROR);
        }
    }
    return "";
}

bool AutoMatchMaker::getRunning() const
{
    return m_running;
}

void AutoMatchMaker::setRunning(bool newRunning)
{
    m_running = newRunning;
    m_state = m_running ? State::ActiveWithSignUp : State::SignUp;
}

QString AutoMatchMaker::getState() const
{
    switch (m_state)
    {
    case State::InCreation:
        return "InCreation";
    case State::SignUp:
        return "SignUp";
    case State::ActiveWithSignUp:
        return "ActiveWithSignUp";
    case State::ActiveWithNoSignUp:
        return "ActiveWithNoSignUp";
    }
    return "InCreation";
}

void AutoMatchMaker::updateStateFromScript()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(&m_mainServer)});
    QJSValue result = pInterpreter->doFunction(m_matchId, "getState", args);
    if (!result.isString())
    {
        return;
    }

    const QString state = result.toString();
    if (state == "InCreation")
    {
        m_state = State::InCreation;
    }
    else if (state == "SignUp")
    {
        m_state = State::SignUp;
    }
    else if (state == "ActiveWithSignUp")
    {
        m_state = State::ActiveWithSignUp;
    }
    else if (state == "ActiveWithNoSignUp")
    {
        m_state = State::ActiveWithNoSignUp;
    }
    else
    {
        CONSOLE_PRINT("Auto match script " + m_matchId + " returned an unknown state: " + state,
                      GameConsole::eWARNING);
    }
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
