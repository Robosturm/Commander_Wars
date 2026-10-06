#pragma once

#include <QObject>
#include <QStringList>
#include <QVariantList>

#include "network/tournamentbracket.h"

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
    Q_INVOKABLE QString getLastError() const;

private:
    bool setupImpl(const QStringList &names, const QVariantList &ratings, bool thirdPlace, bool seedOrder);
    TournamentBracket m_bracket;
    QString m_lastError;
};
