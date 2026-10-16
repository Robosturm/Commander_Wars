#pragma once

#include <QObject>
#include <QStringList>
#include <QVariantList>

#include "network/tournamentbracket.h"
#include "network/tournamentgroupstage.h"

class TournamentBracketController final : public QObject
{
    Q_OBJECT
public:
    explicit TournamentBracketController(QObject *parent = nullptr);

    Q_INVOKABLE bool setup(const QStringList &names, const QVariantList &ratings, bool thirdPlace);
    Q_INVOKABLE bool setupSeededOrder(const QStringList &names, const QVariantList &ratings, bool thirdPlace);
    Q_INVOKABLE bool reportResult(int matchId, const QString &winnerName);
    Q_INVOKABLE bool load(const QString &json);
    Q_INVOKABLE QString save() const;
    Q_INVOKABLE QString readyMatchesJson() const;
    Q_INVOKABLE QString placementsJson() const;
    Q_INVOKABLE bool isComplete() const;
    Q_INVOKABLE bool setupGroupStage(const QStringList &names, const QVariantList &ratings,
                                     int groupSize, int advanceCount);
    Q_INVOKABLE bool reportGroupResult(int matchId, int result1, int result2);
    Q_INVOKABLE bool loadGroupStage(const QString &json);
    Q_INVOKABLE QString saveGroupStage() const;
    Q_INVOKABLE QString readyGroupMatchesJson() const;
    Q_INVOKABLE QString groupStageStandingsJson() const;
    Q_INVOKABLE QString groupStageAdvancersJson() const;
    Q_INVOKABLE bool isGroupStageComplete() const;
    Q_INVOKABLE QString getLastError() const;

private:
    bool setupImpl(const QStringList &names, const QVariantList &ratings, bool thirdPlace, bool seedOrder);
    TournamentBracket m_bracket;
    TournamentGroupStage m_groupStage;
    QString m_lastError;
};
