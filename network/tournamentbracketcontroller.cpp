#include "network/tournamentbracketcontroller.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

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

QString TournamentBracketController::getLastError() const
{
    return m_lastError;
}
