#ifndef AUTOMATCHMAKER_H
#define AUTOMATCHMAKER_H

#include <QObject>
#include <3rd_party/oxygine-framework/oxygine/core/ref_counter.h>
#include "game/GameEnums.h"

class MainServer;
class AutoMatchMaker;
using spAutoMatchMaker = oxygine::intrusive_ptr<AutoMatchMaker>;

class AutoMatchMaker : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit AutoMatchMaker(const QString & matchName, MainServer & mainServer);
    /**
     * @brief onNewMatchResultData
     * @param objData
     */
    void onNewMatchResultData(const QJsonObject & objData);
    void updateInGameInfo(const QJsonObject & objData);
    void createNewGames();
signals:

public slots:
    QString getMatchName() const;
    void updateMmr(const QString & player1, const QString & player2, qint32 maxEloChange, GameEnums::GameResult gameResultForPlayer1);
private:
    QString m_matchName;
    MainServer & m_mainServer;
};

Q_DECLARE_INTERFACE(AutoMatchMaker, "AutoMatchMaker");

#endif // AUTOMATCHMAKER_H
