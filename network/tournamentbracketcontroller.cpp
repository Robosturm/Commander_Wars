#include "network/tournamentbracketcontroller.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include <cmath>
#include <limits>

TournamentBracketController::TournamentBracketController(QObject *parent)
    : QObject(parent)
{
}

bool TournamentBracketController::setup(const QStringList &names, const QVariantList &ratings,
                                        bool thirdPlace)
{
    return setupImpl(names, ratings, thirdPlace, false);
}

bool TournamentBracketController::setupSeededOrder(const QStringList &names, const QVariantList &ratings,
                                                   bool thirdPlace)
{
    return setupImpl(names, ratings, thirdPlace, true);
}

bool TournamentBracketController::setupImpl(const QStringList &names, const QVariantList &ratings,
                                            bool thirdPlace, bool seedOrder)
{
    m_lastError.clear();
    if (names.size() != ratings.size())
    {
        m_lastError = QStringLiteral("Entrant names and ratings must have the same length.");
        return false;
    }
    QVector<TournamentBracket::Entrant> entrants;
    entrants.reserve(names.size());
    for (qsizetype i = 0; i < names.size(); ++i)
    {
        bool ok = false;
        const double rating = ratings[i].toDouble(&ok);
        if (!ok || !std::isfinite(rating) || std::floor(rating) != rating ||
            rating < 0 || rating > std::numeric_limits<int>::max())
        {
            m_lastError = QStringLiteral("Every rating must be a nonnegative integer.");
            return false;
        }
        entrants.append({names[i], static_cast<int>(rating)});
    }
    return seedOrder ? m_bracket.setupSeededOrder(entrants, thirdPlace, m_lastError)
                     : m_bracket.setup(entrants, thirdPlace, m_lastError);
}

bool TournamentBracketController::reportResult(int matchId, const QString &winnerName)
{
    return m_bracket.reportResult(matchId, winnerName, m_lastError);
}

bool TournamentBracketController::load(const QString &json)
{
    m_lastError.clear();
    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(json.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject())
    {
        m_lastError = parseError.error == QJsonParseError::NoError
                          ? QStringLiteral("Bracket JSON must contain an object.")
                          : parseError.errorString();
        return false;
    }
    auto bracket = TournamentBracket::fromJson(document.object(), m_lastError);
    if (!bracket.has_value())
    {
        return false;
    }
    m_bracket = std::move(*bracket);
    return true;
}

QString TournamentBracketController::save() const
{
    return QString::fromUtf8(QJsonDocument(m_bracket.toJson()).toJson(QJsonDocument::Compact));
}

QString TournamentBracketController::readyMatchesJson() const
{
    QJsonArray matches;
    for (const auto &match : m_bracket.readyMatches())
    {
        const auto &entrants = m_bracket.entrants();
        matches.append(QJsonObject{
            {QStringLiteral("matchId"), match.id},
            {QStringLiteral("round"), match.round},
            {QStringLiteral("slot"), match.slot},
            {QStringLiteral("player1"), entrants[match.player1].name},
            {QStringLiteral("player2"), entrants[match.player2].name},
            {QStringLiteral("thirdPlace"), match.thirdPlace}});
    }
    return QString::fromUtf8(QJsonDocument(matches).toJson(QJsonDocument::Compact));
}

QString TournamentBracketController::placementsJson() const
{
    QJsonArray placements;
    for (const auto &placement : m_bracket.placements())
    {
        placements.append(QJsonObject{
            {QStringLiteral("name"), placement.name},
            {QStringLiteral("rating"), placement.rating},
            {QStringLiteral("place"), placement.place}});
    }
    return QString::fromUtf8(QJsonDocument(placements).toJson(QJsonDocument::Compact));
}

bool TournamentBracketController::isComplete() const
{
    return m_bracket.isComplete();
}

bool TournamentBracketController::setupGroupStage(const QStringList &names, const QVariantList &ratings,
                                                   int groupSize, int advanceCount)
{
    m_lastError.clear();
    if (names.size() != ratings.size())
    {
        m_lastError = QStringLiteral("Entrant names and ratings must have the same length.");
        return false;
    }
    QVector<TournamentGroupStage::Entrant> entrants;
    entrants.reserve(names.size());
    for (qsizetype i = 0; i < names.size(); ++i)
    {
        bool ok = false;
        const double rating = ratings[i].toDouble(&ok);
        if (!ok || !std::isfinite(rating) || std::floor(rating) != rating ||
            rating < 0 || rating > std::numeric_limits<int>::max())
        {
            m_lastError = QStringLiteral("Every rating must be a nonnegative integer.");
            return false;
        }
        entrants.append({names[i], static_cast<int>(rating)});
    }
    return m_groupStage.setup(entrants, groupSize, advanceCount, m_lastError);
}

bool TournamentBracketController::reportGroupResult(int matchId, int result1, int result2)
{
    return m_groupStage.reportResult(matchId, result1, result2, m_lastError);
}

bool TournamentBracketController::loadGroupStage(const QString &json)
{
    m_lastError.clear();
    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(json.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject())
    {
        m_lastError = parseError.error == QJsonParseError::NoError
                          ? QStringLiteral("Group-stage JSON must contain an object.")
                          : parseError.errorString();
        return false;
    }
    auto groupStage = TournamentGroupStage::fromJson(document.object(), m_lastError);
    if (!groupStage.has_value())
    {
        return false;
    }
    m_groupStage = std::move(*groupStage);
    return true;
}

QString TournamentBracketController::saveGroupStage() const
{
    return QString::fromUtf8(QJsonDocument(m_groupStage.toJson()).toJson(QJsonDocument::Compact));
}

QString TournamentBracketController::readyGroupMatchesJson() const
{
    QJsonArray matches;
    const auto &entrants = m_groupStage.entrants();
    for (const auto &match : m_groupStage.readyMatches())
    {
        matches.append(QJsonObject{
            {QStringLiteral("matchId"), match.id},
            {QStringLiteral("group"), match.group},
            {QStringLiteral("round"), match.round},
            {QStringLiteral("player1"), entrants[match.player1].name},
            {QStringLiteral("player2"), entrants[match.player2].name}});
    }
    return QString::fromUtf8(QJsonDocument(matches).toJson(QJsonDocument::Compact));
}

QString TournamentBracketController::groupStageStandingsJson() const
{
    QJsonArray standings;
    for (const auto &standing : m_groupStage.standings())
    {
        standings.append(QJsonObject{
            {QStringLiteral("name"), standing.name},
            {QStringLiteral("rating"), standing.rating},
            {QStringLiteral("group"), standing.group},
            {QStringLiteral("place"), standing.place},
            {QStringLiteral("points"), standing.points},
            {QStringLiteral("wins"), standing.wins},
            {QStringLiteral("draws"), standing.draws},
            {QStringLiteral("losses"), standing.losses}});
    }
    return QString::fromUtf8(QJsonDocument(standings).toJson(QJsonDocument::Compact));
}

QString TournamentBracketController::groupStageAdvancersJson() const
{
    QJsonArray entrants;
    for (const auto &entrant : m_groupStage.advancers())
    {
        entrants.append(QJsonObject{
            {QStringLiteral("name"), entrant.name},
            {QStringLiteral("rating"), entrant.rating}});
    }
    return QString::fromUtf8(QJsonDocument(entrants).toJson(QJsonDocument::Compact));
}

bool TournamentBracketController::isGroupStageComplete() const
{
    return m_groupStage.isComplete();
}

QString TournamentBracketController::getLastError() const
{
    return m_lastError;
}
