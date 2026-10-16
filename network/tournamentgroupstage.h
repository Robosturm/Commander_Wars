#pragma once

#include <QJsonObject>
#include <QString>
#include <QVector>

#include <optional>

class TournamentGroupStage final
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
        int group{0};
        int round{0};
        int player1{-1};
        int player2{-1};
        int result1{-1};
        int result2{-1};
        bool completed{false};
    };

    struct Standing
    {
        QString name;
        int rating{0};
        int points{0};
        int wins{0};
        int draws{0};
        int losses{0};
        int place{0};
        int group{0};
    };

    bool setup(const QVector<Entrant> &entrants, int groupSize, int advanceCount, QString &error);
    bool reportResult(int matchId, int result1, int result2, QString &error);
    const QVector<Entrant> &entrants() const { return m_entrants; }
    const QVector<Match> &matches() const { return m_matches; }
    QVector<Match> readyMatches() const;
    QVector<Standing> standings() const;
    QVector<Entrant> advancers() const;
    int groupCount() const { return m_groupCount; }
    int advanceCount() const { return m_advanceCount; }
    bool isComplete() const;

    QJsonObject toJson() const;
    static std::optional<TournamentGroupStage> fromJson(const QJsonObject &json, QString &error);

private:
    QVector<int> orderedGroupEntrants(int group) const;
    QVector<Entrant> m_entrants;
    QVector<int> m_entrantGroups;
    QVector<Match> m_matches;
    int m_groupCount{0};
    int m_advanceCount{0};
};
