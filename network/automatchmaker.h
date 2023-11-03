#ifndef AUTOMATCHMAKER_H
#define AUTOMATCHMAKER_H

#include <QObject>
#include <QSqlQuery>

#include "game/GameEnums.h"

#include "coreengine/fileserializable.h"
#include "coreengine/scriptvariables.h"

class MainServer;
class AutoMatchMaker;
using spAutoMatchMaker = std::shared_ptr<AutoMatchMaker>;

class AutoMatchMaker : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    explicit AutoMatchMaker(const QString & matchId, MainServer * mainServer);
    /**
     * @brief onNewMatchResultData
     * @param objData
     */
    void onNewMatchResultData(const QJsonObject & objData);
    /**
     * @brief onNewPlayerData
     * @param objData
     */
    void onNewPlayerData(const QJsonObject & objData);
    /**
     * @brief serializeObject
     * @param stream
     */
    virtual void serializeObject(QDataStream& stream) const override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& stream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 2;
    }
    /**
     * @brief playerJoined
     * @param player
     */
    void playerJoined(const QString & player);
    /**
     * @brief getRunning
     * @return
     */
    bool getRunning() const;
    /**
     * @brief setRunning
     * @param newRunning
     */
    void setRunning(bool newRunning);
    /**
     * @brief getActiveMatch
     * @return
     */
    bool getActiveMatch() const;
    /**
     * @brief setActiveMatch
     * @param newActiveMatch
     */
    void setActiveMatch(bool newActiveMatch);

    qint32 getNotActiveCounter() const;
    void increaseNotActiveCounter();
    /**
     * @brief getBracketGraphInfo
     * @return
     */
    QJsonObject getBracketGraphInfo();
    Q_INVOKABLE bool getSignedUp(const QString  playerId);
    Q_INVOKABLE void createNewGame(const QStringList players, const QStringList modList);
    Q_INVOKABLE QString getMatchId() const;
    Q_INVOKABLE void updateMmr(const QString player1, const QString player2, qint32 maxEloChange, GameEnums::GameResult gameResultForPlayer1);
    Q_INVOKABLE bool setMmr(const QString player, qint32 mmr);
    Q_INVOKABLE qint32 getMmr(const QString player);
    Q_INVOKABLE bool setMatchHistoryData(const QString player, QString historyData);
    Q_INVOKABLE QString getMatchHistoryData(const QString player);
    Q_INVOKABLE bool setMatchMetaData(const QString player, QString metaData);
    Q_INVOKABLE QString getMatchMetaData(const QString player);
    Q_INVOKABLE QStringList getOpponentsForPlayer(const QString player, qint32 mmrSearchStepRange);
    Q_INVOKABLE qint32 getRunningGames(const QString player);
    Q_INVOKABLE bool setRunningGames(const QString player, qint32 count);
    Q_INVOKABLE qint32 getMinGames(const QString player);
    Q_INVOKABLE qint32 getMaxGames(const QString player);
    /**
     * @brief getVariables
     * @return
     */
    Q_INVOKABLE inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    /**
     * @brief doNewPlayerData creates new player data for this match set if no exists else updates minGames, maxGames and metaData to the given values
     * @param player
     * @param minGames
     * @param maxGames
     * @param metaData
     * @param startMmr
     */
    Q_INVOKABLE bool doNewPlayerData(const QString & player, qint32 minGames, qint32 maxGames, const QString & metaData, qint32 startMmr = 750);
    /**
     * @brief getName
     * @return
     */
    Q_INVOKABLE QString getName();
    /**
     * @brief getDescription
     * @return
     */
    Q_INVOKABLE QString getDescription();
    /**
     * @brief getIsSignUpChangeAllowed
     * @return
     */
    Q_INVOKABLE bool getIsSignUpChangeAllowed();
    Q_INVOKABLE static QString getBracketGraphInfoId();
    Q_INVOKABLE static QString getBracketGraphPreviousMatchId();
    Q_INVOKABLE static QString getBracketGraphPreviousWinnersId();
    Q_INVOKABLE static QString getBracketGraphPreviousPlayersId();
    Q_INVOKABLE QString readDataFromJson(const QString & filePath);
private:
    QString m_matchId;
    MainServer & m_mainServer;
    quint64 m_matchCounter{0};
    ScriptVariables m_Variables;
    bool m_running{false};
    bool m_activeMatch{false};
    qint32 m_notActiveCounter{0};
};

Q_DECLARE_INTERFACE(AutoMatchMaker, "AutoMatchMaker");

#endif // AUTOMATCHMAKER_H
