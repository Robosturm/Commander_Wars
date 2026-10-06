#include "network/tournamentbracket.h"

#include <QJsonArray>
#include <QSet>

#include <algorithm>
#include <cmath>
#include <limits>

namespace
{
bool readInteger(const QJsonValue &value, int &result)
{
    if (!value.isDouble())
    {
        return false;
    }
    const double number = value.toDouble();
    if (!std::isfinite(number) || std::floor(number) != number ||
        number < std::numeric_limits<int>::min() || number > std::numeric_limits<int>::max())
    {
        return false;
    }
    result = static_cast<int>(number);
    return true;
}
}

bool TournamentBracket::setup(const QVector<Entrant> &entrants, bool thirdPlace, QString &error)
{
    return setupInternal(entrants, thirdPlace, true, error);
}

bool TournamentBracket::setupSeededOrder(const QVector<Entrant> &entrants, bool thirdPlace, QString &error)
{
    return setupInternal(entrants, thirdPlace, false, error);
}

bool TournamentBracket::setupInternal(const QVector<Entrant> &entrants, bool thirdPlace,
                                      bool sortByRating, QString &error)
{
    error.clear();
    if (entrants.size() < 2 || entrants.size() > std::numeric_limits<int>::max() / 2)
    {
        error = QStringLiteral("A knockout bracket requires at least two entrants and a representable bracket size.");
        return false;
    }
    if (thirdPlace && entrants.size() < 4)
    {
        error = QStringLiteral("A third-place match requires at least four entrants.");
        return false;
    }
    QSet<QString> names;
    for (const auto &entrant : entrants)
    {
        if (entrant.name.trimmed().isEmpty() || names.contains(entrant.name) || entrant.rating < 0)
        {
            error = QStringLiteral("Entrant names must be unique and nonblank, and ratings must be nonnegative.");
            return false;
        }
        names.insert(entrant.name);
    }

    TournamentBracket bracket;
    bracket.m_entrants = entrants;
    if (sortByRating)
    {
        std::stable_sort(bracket.m_entrants.begin(), bracket.m_entrants.end(),
                         [](const Entrant &left, const Entrant &right) { return left.rating > right.rating; });
    }
    bracket.m_thirdPlace = thirdPlace;
    bracket.m_seedOrderProvided = !sortByRating;
    bracket.m_bracketSize = 1;
    while (bracket.m_bracketSize < entrants.size())
    {
        bracket.m_bracketSize *= 2;
        ++bracket.m_roundCount;
    }

    // Reflect seeds at each expansion: 1,8,4,5,2,7,3,6 for eight slots.
    QVector<int> seeds{1};
    for (int size = 2; ; size *= 2)
    {
        QVector<int> expanded;
        expanded.reserve(size);
        for (int seed : seeds)
        {
            expanded.append(seed);
            expanded.append(size + 1 - seed);
        }
        seeds = std::move(expanded);
        if (size == bracket.m_bracketSize)
        {
            break;
        }
    }
    bracket.m_matches.reserve(bracket.m_bracketSize - 1 + (thirdPlace ? 1 : 0));
    for (int round = 0, count = bracket.m_bracketSize / 2; count > 0; ++round, count /= 2)
    {
        for (int slot = 0; slot < count; ++slot)
        {
            Match match;
            match.id = static_cast<int>(bracket.m_matches.size());
            match.round = round;
            match.slot = slot;
            if (round == 0)
            {
                const int first = seeds[slot * 2] - 1;
                const int second = seeds[slot * 2 + 1] - 1;
                match.player1 = first < entrants.size() ? first : -1;
                match.player2 = second < entrants.size() ? second : -1;
            }
            bracket.m_matches.append(match);
        }
    }
    if (thirdPlace)
    {
        Match match;
        match.id = static_cast<int>(bracket.m_matches.size());
        match.round = bracket.m_roundCount - 1;
        match.thirdPlace = true;
        bracket.m_matches.append(match);
    }
    bracket.progress();
    *this = std::move(bracket);
    return true;
}

void TournamentBracket::progress()
{
    int roundStart = 0;
    int previousStart = 0;
    int count = m_bracketSize / 2;
    for (int round = 0; round < m_roundCount; ++round)
    {
        for (int slot = 0; slot < count; ++slot)
        {
            auto &match = m_matches[roundStart + slot];
            if (match.completed)
            {
                continue;
            }
            if (round > 0)
            {
                const auto &first = m_matches[previousStart + slot * 2];
                const auto &second = m_matches[previousStart + slot * 2 + 1];
                match.player1 = first.completed ? first.winner : -1;
                match.player2 = second.completed ? second.winner : -1;
                // A pending feeder is not a bye.
                if (!first.completed || !second.completed)
                {
                    continue;
                }
            }
            if (match.player1 < 0 || match.player2 < 0)
            {
                match.winner = match.player1 >= 0 ? match.player1 : match.player2;
                match.completed = true;
                match.automatic = true;
            }
        }
        previousStart = roundStart;
        roundStart += count;
        count /= 2;
    }
    if (m_thirdPlace)
    {
        auto &match = m_matches.last();
        const auto &first = m_matches[m_bracketSize - 4];
        const auto &second = m_matches[m_bracketSize - 3];
        match.player1 = first.completed ? loser(first) : -1;
        match.player2 = second.completed ? loser(second) : -1;
    }
}

int TournamentBracket::loser(const Match &match) const
{
    if (!match.completed || match.automatic)
    {
        return -1;
    }
    return match.winner == match.player1 ? match.player2 : match.player1;
}

QVector<TournamentBracket::Match> TournamentBracket::readyMatches() const
{
    QVector<Match> ready;
    for (const auto &match : m_matches)
    {
        if (!match.completed && match.player1 >= 0 && match.player2 >= 0)
        {
            ready.append(match);
        }
    }
    return ready;
}

bool TournamentBracket::reportResult(int matchId, const QString &winnerName, QString &error)
{
    error.clear();
    if (matchId < 0 || matchId >= m_matches.size())
    {
        error = QStringLiteral("Unknown match ID.");
        return false;
    }
    auto &match = m_matches[matchId];
    if (match.completed || match.player1 < 0 || match.player2 < 0)
    {
        error = QStringLiteral("The match is completed or not ready.");
        return false;
    }
    int winner = -1;
    if (m_entrants[match.player1].name == winnerName)
    {
        winner = match.player1;
    }
    else if (m_entrants[match.player2].name == winnerName)
    {
        winner = match.player2;
    }
    if (winner < 0)
    {
        error = QStringLiteral("The winner must be a participant in the match.");
        return false;
    }
    match.winner = winner;
    match.completed = true;
    progress();
    return true;
}

bool TournamentBracket::isComplete() const
{
    return !m_matches.isEmpty() &&
           std::all_of(m_matches.cbegin(), m_matches.cend(),
                       [](const Match &match) { return match.completed; });
}

QVector<TournamentBracket::Placement> TournamentBracket::placements() const
{
    QVector<Placement> result;
    for (const auto &match : m_matches)
    {
        const int eliminated = loser(match);
        if (eliminated < 0)
        {
            continue;
        }
        if (!match.thirdPlace && m_thirdPlace && match.round == m_roundCount - 2)
        {
            continue;
        }
        const auto &entrant = m_entrants[eliminated];
        result.append({entrant.name, entrant.rating,
                       match.thirdPlace ? 4 : (1 << (m_roundCount - match.round - 1)) + 1});
        if (match.thirdPlace || match.round == m_roundCount - 1)
        {
            const auto &winner = m_entrants[match.winner];
            result.append({winner.name, winner.rating, match.thirdPlace ? 3 : 1});
        }
    }
    std::stable_sort(result.begin(), result.end(),
                     [](const Placement &left, const Placement &right) { return left.place < right.place; });
    return result;
}

QJsonObject TournamentBracket::toJson() const
{
    QJsonArray entrants;
    for (const auto &entrant : m_entrants)
    {
        entrants.append(QJsonObject{{QStringLiteral("name"), entrant.name},
                                   {QStringLiteral("rating"), entrant.rating}});
    }
    QJsonArray results;
    for (const auto &match : m_matches)
    {
        if (match.completed && !match.automatic)
        {
            results.append(QJsonObject{{QStringLiteral("matchId"), match.id},
                                      {QStringLiteral("winner"), m_entrants[match.winner].name}});
        }
    }
    return {{QStringLiteral("version"), 2},
            {QStringLiteral("entrants"), entrants},
            {QStringLiteral("thirdPlace"), m_thirdPlace},
            {QStringLiteral("seedOrderProvided"), m_seedOrderProvided},
            {QStringLiteral("results"), results}};
}

std::optional<TournamentBracket> TournamentBracket::fromJson(const QJsonObject &json, QString &error)
{
    error.clear();
    int version = 0;
    if (!readInteger(json.value(QStringLiteral("version")), version) || (version != 1 && version != 2) ||
        !json.value(QStringLiteral("entrants")).isArray() ||
        !json.value(QStringLiteral("thirdPlace")).isBool() ||
        !json.value(QStringLiteral("results")).isArray() ||
        (version == 2 && !json.value(QStringLiteral("seedOrderProvided")).isBool()))
    {
        error = QStringLiteral("Invalid bracket schema or unsupported version.");
        return std::nullopt;
    }
    QVector<Entrant> entrants;
    const auto entrantArray = json.value(QStringLiteral("entrants")).toArray();
    for (const auto &value : entrantArray)
    {
        const auto object = value.toObject();
        int rating = 0;
        if (!value.isObject() || !object.value(QStringLiteral("name")).isString() ||
            !readInteger(object.value(QStringLiteral("rating")), rating))
        {
            error = QStringLiteral("Invalid entrant JSON.");
            return std::nullopt;
        }
        entrants.append({object.value(QStringLiteral("name")).toString(), rating});
    }
    TournamentBracket bracket;
    const bool seedOrderProvided = version == 2 &&
                                   json.value(QStringLiteral("seedOrderProvided")).toBool();
    const bool setupSucceeded = seedOrderProvided
                                    ? bracket.setupSeededOrder(entrants,
                                                              json.value(QStringLiteral("thirdPlace")).toBool(),
                                                              error)
                                    : bracket.setup(entrants, json.value(QStringLiteral("thirdPlace")).toBool(),
                                                    error);
    if (!setupSucceeded)
    {
        return std::nullopt;
    }
    struct Result
    {
        int matchId;
        QString winner;
    };
    QVector<Result> results;
    QSet<int> ids;
    const auto resultArray = json.value(QStringLiteral("results")).toArray();
    for (const auto &value : resultArray)
    {
        const auto object = value.toObject();
        int matchId = -1;
        if (!value.isObject() || !readInteger(object.value(QStringLiteral("matchId")), matchId) ||
            !object.value(QStringLiteral("winner")).isString() || ids.contains(matchId))
        {
            error = QStringLiteral("Invalid or duplicate match result JSON.");
            return std::nullopt;
        }
        ids.insert(matchId);
        results.append({matchId, object.value(QStringLiteral("winner")).toString()});
    }
    // Match IDs are topological, so input array order does not affect loading.
    std::sort(results.begin(), results.end(),
              [](const Result &left, const Result &right) { return left.matchId < right.matchId; });
    for (const auto &result : results)
    {
        if (!bracket.reportResult(result.matchId, result.winner, error))
        {
            return std::nullopt;
        }
    }
    return bracket;
}
