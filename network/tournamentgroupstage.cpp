#include "network/tournamentgroupstage.h"

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

bool TournamentGroupStage::setup(const QVector<Entrant> &entrants, int groupSize,
                                 int advanceCount, QString &error)
{
    error.clear();
    if (entrants.size() < 2 || groupSize < 2 || advanceCount < 1 ||
        advanceCount >= groupSize)
    {
        error = QStringLiteral("Group size must be at least two and advancement must be smaller than the group size.");
        return false;
    }
    QSet<QString> names;
    for (const auto &entrant : entrants)
    {
        if (entrant.name.trimmed().isEmpty() || entrant.rating < 0 || names.contains(entrant.name))
        {
            error = QStringLiteral("Entrant names must be unique and nonblank, and ratings must be nonnegative.");
            return false;
        }
        names.insert(entrant.name);
    }

    TournamentGroupStage stage;
    stage.m_entrants = entrants;
    std::stable_sort(stage.m_entrants.begin(), stage.m_entrants.end(),
                     [](const Entrant &left, const Entrant &right)
                     {
                         return left.rating > right.rating;
                     });
    stage.m_groupCount = (stage.m_entrants.size() - 1) / groupSize + 1;
    stage.m_advanceCount = advanceCount;
    stage.m_entrantGroups.resize(stage.m_entrants.size());

    // Snake seeding spreads adjacent MMR seeds across groups.
    for (int seed = 0; seed < stage.m_entrants.size(); ++seed)
    {
        const int band = seed / stage.m_groupCount;
        const int offset = seed % stage.m_groupCount;
        const int group = band % 2 == 0 ? offset : stage.m_groupCount - offset - 1;
        stage.m_entrantGroups[seed] = group;
    }

    for (int group = 0; group < stage.m_groupCount; ++group)
    {
        QVector<int> players = stage.orderedGroupEntrants(group);
        if (players.size() < 2 || players.size() <= advanceCount)
        {
            error = QStringLiteral("Every group must contain at least two entrants and more entrants than the advancement count.");
            return false;
        }
        if (players.size() % 2 != 0)
        {
            players.append(-1);
        }
        const int roundCount = players.size() - 1;
        const int half = players.size() / 2;
        for (int round = 0; round < roundCount; ++round)
        {
            for (int slot = 0; slot < half; ++slot)
            {
                const int first = players[slot];
                const int second = players[players.size() - slot - 1];
                if (first >= 0 && second >= 0)
                {
                    Match match;
                    match.id = stage.m_matches.size();
                    match.group = group;
                    match.round = round;
                    match.player1 = first;
                    match.player2 = second;
                    stage.m_matches.append(match);
                }
            }
            const int last = players.takeLast();
            players.insert(1, last);
        }
    }

    *this = std::move(stage);
    return true;
}

QVector<int> TournamentGroupStage::orderedGroupEntrants(int group) const
{
    QVector<int> indices;
    for (int index = 0; index < m_entrantGroups.size(); ++index)
    {
        if (m_entrantGroups[index] == group)
        {
            indices.append(index);
        }
    }
    return indices;
}

bool TournamentGroupStage::reportResult(int matchId, int result1, int result2, QString &error)
{
    error.clear();
    if (matchId < 0 || matchId >= m_matches.size() ||
        result1 < 0 || result1 > 2 || result2 < 0 || result2 > 2 ||
        (result1 == 1 && result2 == 1) ||
        (result1 == result2 && result1 != 0))
    {
        error = QStringLiteral("Invalid group match result.");
        return false;
    }
    auto &match = m_matches[matchId];
    if (match.completed)
    {
        error = QStringLiteral("The group match has already been completed.");
        return false;
    }
    if (!((result1 == 2 && result2 == 1) || (result1 == 1 && result2 == 2) ||
          (result1 == 0 && result2 == 0)))
    {
        error = QStringLiteral("Results must be a win/loss pair or a draw.");
        return false;
    }
    match.result1 = result1;
    match.result2 = result2;
    match.completed = true;
    return true;
}

QVector<TournamentGroupStage::Match> TournamentGroupStage::readyMatches() const
{
    QVector<Match> result;
    for (const auto &match : m_matches)
    {
        if (!match.completed)
        {
            const bool earlierFixturePending = std::any_of(
                m_matches.cbegin(), m_matches.cend(), [&match](const Match &earlier)
                {
                    return !earlier.completed && earlier.group == match.group &&
                           earlier.round < match.round &&
                           (earlier.player1 == match.player1 || earlier.player1 == match.player2 ||
                            earlier.player2 == match.player1 || earlier.player2 == match.player2);
                });
            if (!earlierFixturePending)
            {
                result.append(match);
            }
        }
    }
    return result;
}

QVector<TournamentGroupStage::Standing> TournamentGroupStage::standings() const
{
    QVector<Standing> result;
    result.reserve(m_entrants.size());
    for (int index = 0; index < m_entrants.size(); ++index)
    {
        const auto &entrant = m_entrants[index];
        result.append({entrant.name, entrant.rating, 0, 0, 0, 0, 0, m_entrantGroups[index]});
    }
    for (const auto &match : m_matches)
    {
        if (!match.completed)
        {
            continue;
        }
        auto &first = result[match.player1];
        auto &second = result[match.player2];
        if (match.result1 == 0)
        {
            ++first.draws;
            ++second.draws;
            ++first.points;
            ++second.points;
        }
        else if (match.result1 == 2)
        {
            ++first.wins;
            ++second.losses;
            first.points += 3;
        }
        else
        {
            ++first.losses;
            ++second.wins;
            second.points += 3;
        }
    }

    for (int group = 0; group < m_groupCount; ++group)
    {
        QVector<int> groupIndices = orderedGroupEntrants(group);
        std::stable_sort(groupIndices.begin(), groupIndices.end(),
                         [&result, this](int left, int right)
                         {
                             if (result[left].points != result[right].points)
                             {
                                 return result[left].points > result[right].points;
                             }
                             return false;
                         });
        int tieBegin = 0;
        while (tieBegin < groupIndices.size())
        {
            int tieEnd = tieBegin + 1;
            while (tieEnd < groupIndices.size() &&
                   result[groupIndices[tieEnd]].points == result[groupIndices[tieBegin]].points)
            {
                ++tieEnd;
            }
            QVector<int> tied(groupIndices.cbegin() + tieBegin, groupIndices.cbegin() + tieEnd);
            auto headToHeadPoints = [this, &tied](int player)
            {
                int points = 0;
                for (const auto &match : m_matches)
                {
                    if (!match.completed || match.group != m_entrantGroups[player] ||
                        !tied.contains(match.player1) || !tied.contains(match.player2))
                    {
                        continue;
                    }
                    if (match.player1 == player)
                    {
                        points += match.result1 == 2 ? 3 : (match.result1 == 0 ? 1 : 0);
                    }
                    else if (match.player2 == player)
                    {
                        points += match.result2 == 2 ? 3 : (match.result2 == 0 ? 1 : 0);
                    }
                }
                return points;
            };
            std::stable_sort(groupIndices.begin() + tieBegin, groupIndices.begin() + tieEnd,
                             [&result, &headToHeadPoints](int left, int right)
                             {
                                 const int leftHeadToHead = headToHeadPoints(left);
                                 const int rightHeadToHead = headToHeadPoints(right);
                                 if (leftHeadToHead != rightHeadToHead)
                                 {
                                     return leftHeadToHead > rightHeadToHead;
                                 }
                                 if (result[left].rating != result[right].rating)
                                 {
                                     return result[left].rating > result[right].rating;
                                 }
                                 return result[left].name < result[right].name;
                             });
            tieBegin = tieEnd;
        }
        for (int place = 0; place < groupIndices.size(); ++place)
        {
            result[groupIndices[place]].place = place + 1;
        }
    }
    std::stable_sort(result.begin(), result.end(),
                     [](const Standing &left, const Standing &right)
                     {
                         if (left.group != right.group)
                         {
                             return left.group < right.group;
                         }
                         return left.place < right.place;
                     });
    return result;
}

QVector<TournamentGroupStage::Entrant> TournamentGroupStage::advancers() const
{
    QVector<Entrant> result;
    const auto ranked = standings();
    for (const auto &standing : ranked)
    {
        if (standing.place <= m_advanceCount)
        {
            result.append({standing.name, standing.rating});
        }
    }
    return result;
}

bool TournamentGroupStage::isComplete() const
{
    return !m_matches.isEmpty() &&
           std::all_of(m_matches.cbegin(), m_matches.cend(),
                       [](const Match &match) { return match.completed; });
}

QJsonObject TournamentGroupStage::toJson() const
{
    QJsonArray entrants;
    for (int i = 0; i < m_entrants.size(); ++i)
    {
        entrants.append(QJsonObject{
            {QStringLiteral("name"), m_entrants[i].name},
            {QStringLiteral("rating"), m_entrants[i].rating},
            {QStringLiteral("group"), m_entrantGroups[i]}});
    }
    QJsonArray results;
    for (const auto &match : m_matches)
    {
        if (match.completed)
        {
            results.append(QJsonObject{
                {QStringLiteral("matchId"), match.id},
                {QStringLiteral("result1"), match.result1},
                {QStringLiteral("result2"), match.result2}});
        }
    }
    return {{QStringLiteral("version"), 1},
            {QStringLiteral("groupCount"), m_groupCount},
            {QStringLiteral("advanceCount"), m_advanceCount},
            {QStringLiteral("entrants"), entrants},
            {QStringLiteral("results"), results}};
}

std::optional<TournamentGroupStage> TournamentGroupStage::fromJson(const QJsonObject &json, QString &error)
{
    error.clear();
    int version = 0;
    int groupCount = 0;
    int advanceCount = 0;
    if (!readInteger(json.value(QStringLiteral("version")), version) || version != 1 ||
        !readInteger(json.value(QStringLiteral("groupCount")), groupCount) ||
        !readInteger(json.value(QStringLiteral("advanceCount")), advanceCount) ||
        !json.value(QStringLiteral("entrants")).isArray() || !json.value(QStringLiteral("results")).isArray())
    {
        error = QStringLiteral("Invalid group-stage JSON schema.");
        return std::nullopt;
    }
    QVector<Entrant> entrants;
    QVector<int> groups;
    const QJsonArray entrantArray = json.value(QStringLiteral("entrants")).toArray();
    for (const auto &value : entrantArray)
    {
        const QJsonObject object = value.toObject();
        int rating = 0;
        int group = 0;
        if (!value.isObject() || !object.value(QStringLiteral("name")).isString() ||
            !readInteger(object.value(QStringLiteral("rating")), rating) ||
            !readInteger(object.value(QStringLiteral("group")), group) || group < 0 || group >= groupCount)
        {
            error = QStringLiteral("Invalid group-stage entrant.");
            return std::nullopt;
        }
        entrants.append({object.value(QStringLiteral("name")).toString(), rating});
        groups.append(group);
    }
    int maxGroupSize = 0;
    for (int group = 0; group < groupCount; ++group)
    {
        const int count = static_cast<int>(std::count(groups.cbegin(), groups.cend(), group));
        if (count < 2)
        {
            error = QStringLiteral("Every persisted group must contain at least two entrants.");
            return std::nullopt;
        }
        maxGroupSize = std::max(maxGroupSize, count);
    }
    TournamentGroupStage stage;
    if (!stage.setup(entrants, maxGroupSize, advanceCount, error))
    {
        return std::nullopt;
    }
    if (stage.m_groupCount != groupCount)
    {
        error = QStringLiteral("Group-stage group count does not match its entrants.");
        return std::nullopt;
    }
    stage.m_entrantGroups = groups;
    stage.m_matches.clear();
    for (int group = 0; group < groupCount; ++group)
    {
        QVector<int> players = stage.orderedGroupEntrants(group);
        if (players.size() % 2 != 0)
        {
            players.append(-1);
        }
        const int roundCount = players.size() - 1;
        const int half = players.size() / 2;
        for (int round = 0; round < roundCount; ++round)
        {
            for (int slot = 0; slot < half; ++slot)
            {
                const int first = players[slot];
                const int second = players[players.size() - slot - 1];
                if (first >= 0 && second >= 0)
                {
                    Match match;
                    match.id = stage.m_matches.size();
                    match.group = group;
                    match.round = round;
                    match.player1 = first;
                    match.player2 = second;
                    stage.m_matches.append(match);
                }
            }
            const int last = players.takeLast();
            players.insert(1, last);
        }
    }
    QSet<int> seen;
    for (const auto &value : json.value(QStringLiteral("results")).toArray())
    {
        int id = -1;
        int result1 = -1;
        int result2 = -1;
        const QJsonObject object = value.toObject();
        if (!value.isObject() || !readInteger(object.value(QStringLiteral("matchId")), id) ||
            !readInteger(object.value(QStringLiteral("result1")), result1) ||
            !readInteger(object.value(QStringLiteral("result2")), result2) ||
            seen.contains(id) || !stage.reportResult(id, result1, result2, error))
        {
            if (error.isEmpty())
            {
                error = QStringLiteral("Invalid or duplicate group-stage result.");
            }
            return std::nullopt;
        }
        seen.insert(id);
    }
    return stage;
}
