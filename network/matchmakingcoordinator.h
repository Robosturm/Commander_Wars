
#ifndef MATCHMAKINGCOORDINATOR_H
#define MATCHMAKINGCOORDINATOR_H


#include <QObject>
#include "network/automatchmaker.h"

class MainServer;

class MatchMakingCoordinator : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    explicit MatchMakingCoordinator(MainServer *parent);
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
        return 1;
    }
    void periodicTasks();

public slots:
    /**
     * @brief getAutoMatchMaker
     * @param matchMaker
     * @return
     */
    AutoMatchMaker* getAutoMatchMaker(const QString & matchMaker);
    /**
     * @brief onSlaveInfoGameResult
     * @param socketID
     * @param objData
     */
    void onSlaveInfoGameResult(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief updatePlayerMatchData
     * @param objData
     */
    void updatePlayerMatchData(const QJsonObject & objData);
    /**
     * @brief loadAutomatches
     * @param path
     * @param running
     */
    void loadAutomatches(QString & path, bool running);
    /**
     * @brief getMatchMakingData
     * @param objData
     */
    void getMatchMakingData(const QString & playerId, QJsonObject & objData);
private:
    /**
     * @brief removeMatches
     */
    void removeMatches();
    /**
     * 
    */
    void fixPlayerTable(const QString &player);
private:
    /**
     * @brief m_autoMatchMakers
     */
    QMap<QString, spAutoMatchMaker> m_autoMatchMakers;
    MainServer* m_mainServer{nullptr};
};

#endif // MATCHMAKINGCOORDINATOR_H
