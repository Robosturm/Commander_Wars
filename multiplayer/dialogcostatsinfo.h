#pragma once

#include <QObject>
#include <QJsonObject>

#include "objects/dialogs/customdialog.h"

class LobbyMenu;
class DialogCoStatsInfo;
using spDialogCoStatsInfo = std::shared_ptr<DialogCoStatsInfo>;

class DialogCoStatsInfo final : public CustomDialog
{
    Q_OBJECT
    struct CoData
    {
        QString coid;
        qint32 playedGames;
    };
public:
    explicit DialogCoStatsInfo(LobbyMenu *pBaseMenu, const QJsonObject &objData);
    virtual ~DialogCoStatsInfo() = default;

    Q_INVOKABLE qint32 getGamesWon(const QString &coid);
    Q_INVOKABLE qint32 getGamesLost(const QString &coid);
    Q_INVOKABLE qint32 getGamesMade(const QString &coid, bool zeroBased = false);
    Q_INVOKABLE qint32 getGamesDraw(const QString &coid);
    Q_INVOKABLE qint32 getCoCount();
    Q_INVOKABLE QString getCoId(qint32 index);

private:
    QJsonObject getCoStats(const QString &coid);

private:
    QJsonObject m_coStats;
    std::vector<CoData> m_sortedCoids;
};

Q_DECLARE_INTERFACE(DialogCoStatsInfo, "DialogCoStatsInfo");