#pragma once

#include <QJsonObject>
#include <QString>
#include <QVector>

#include <optional>

// Qt Core only; no QObject, matchmaking, UI or transport dependencies.
class TournamentBracket final
{
public:
    struct Entrant
    {
        QString name;
        int rating{0};
    };

    struct Match
    {
        int id{-1};
        int round{0};
        int slot{0};
        bool thirdPlace{false};
        int player1{-1};
        int player2{-1};
        int winner{-1};
        bool completed{false};
        bool automatic{false};
    };

    struct Placement
    {
        QString name;
        int rating{0};
        int place{0};
    };

    // At least two unique, nonblank names and nonnegative ratings are required.
    // Seeds are descending by rating, preserving input order for ties.
    // Third place requires at least four entrants. Failure leaves state unchanged.
    bool setup(const QVector<Entrant> &entrants, bool thirdPlace, QString &error);
    bool reportResult(int matchId, const QString &winnerName, QString &error);

    const QVector<Entrant> &entrants() const { return m_entrants; }
    const QVector<Match> &matches() const { return m_matches; }
    int roundCount() const { return m_roundCount; }
    int bracketSize() const { return m_bracketSize; }
    bool hasThirdPlaceMatch() const { return m_thirdPlace; }
    bool isComplete() const;
    QVector<Match> readyMatches() const;
    // Only decided placements are returned; eliminated players share places
    // unless the third-place match separates the semifinal losers.
    QVector<Placement> placements() const;

    // Versioned inputs + played results; derived matches/byes are reconstructed.
    QJsonObject toJson() const;
    static std::optional<TournamentBracket> fromJson(const QJsonObject &json, QString &error);

private:
    void progress();
    int loser(const Match &match) const;

    QVector<Entrant> m_entrants;
    QVector<Match> m_matches;
    int m_roundCount{0};
    int m_bracketSize{0};
    bool m_thirdPlace{false};
};
