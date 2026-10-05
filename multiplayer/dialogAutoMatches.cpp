#include "multiplayer/dialogAutoMatches.h"
#include "multiplayer/lobbymenu.h"

#include "network/JsonKeys.h"

#include "resource_management/cospritemanager.h"
#include "objects/base/spinbox.h"
#include "multiplayer/networkcommands.h"

#include <QJsonDocument>

DialogAutoMatches::DialogAutoMatches(LobbyMenu *pBaseMenu, const QJsonObject &objData)
    : CustomDialog("", "", pBaseMenu, tr("Close"))
    , m_pLobbyMenu(pBaseMenu)
    , m_objData(objData)
{
    connect(pBaseMenu, &LobbyMenu::sigRequestShowAutoMatches,
            this, &DialogAutoMatches::receivedAutoMatchInfo);
    connect(pBaseMenu, &LobbyMenu::sigAutoMatchActionResult,
            this, &DialogAutoMatches::receivedActionResult);
    m_uiXml = "ui/multiplayer/dialogAutoMatches.xml";
    loadXmlFile(m_uiXml);
}

QJsonObject DialogAutoMatches::getMatch(qint32 index) const
{
    QJsonArray matches = m_objData.value(JsonKeys::JSONKEY_PREPARINGAUTOMATCHES).toArray();
    matches.append(m_objData.value(JsonKeys::JSONKEY_RUNNINGAUTOMATCHES).toArray());
    if (index >= 0 && index < matches.size())
    {
        return matches[index].toObject();
    }
    return {};
}

qint32 DialogAutoMatches::getMatchCount() const
{
    return m_objData.value(JsonKeys::JSONKEY_PREPARINGAUTOMATCHES).toArray().size() +
           m_objData.value(JsonKeys::JSONKEY_RUNNINGAUTOMATCHES).toArray().size();
}

QString DialogAutoMatches::getMatchName(qint32 index) const
{
    return getMatch(index).value(JsonKeys::JSONKEY_NAME).toString();
}

QString DialogAutoMatches::getMatchDescription(qint32 index) const
{
    return getMatch(index).value(JsonKeys::JSONKEY_DESCRIPTION).toString();
}

QString DialogAutoMatches::getMatchState(qint32 index) const
{
    return getMatch(index).value(JsonKeys::JSONKEY_AUTOMATCHSTATE).toString();
}

QString DialogAutoMatches::getMatchMmr(qint32 index) const
{
    qint32 mmr = getMatch(index).value(JsonKeys::JSONKEY_MMR).toInt(-1);
    return mmr < 0 ? tr("Unrated") : QString::number(mmr);
}

QString DialogAutoMatches::getMatchBracketInfo(qint32 index) const
{
    QJsonValue value = getMatch(index).value(JsonKeys::JSONKEY_BRACKETGRAPHINFO);
    return value.isObject() && !value.toObject().isEmpty() ?
               QString::fromUtf8(QJsonDocument(value.toObject()).toJson(QJsonDocument::Compact)) : QString();
}

bool DialogAutoMatches::isMatchSignedUp(qint32 index) const
{
    return getMatch(index).value(JsonKeys::JSONKEY_SIGNEDUP).toBool();
}

bool DialogAutoMatches::isMatchActionAllowed(qint32 index) const
{
    QString state = getMatchState(index);
    return state == "SignUp" || state == "ActiveWithSignUp";
}

QString DialogAutoMatches::getMatchId(qint32 index) const
{
    return getMatch(index).value(JsonKeys::JSONKEY_AUTOMATCHID).toString();
}

void DialogAutoMatches::signUp(qint32 index)
{
    QJsonObject match = getMatch(index);
    auto *minGames = qobject_cast<SpinBox*>(getObject("MinGames"));
    auto *maxGames = qobject_cast<SpinBox*>(getObject("MaxGames"));
    if (match.isEmpty() || minGames == nullptr || maxGames == nullptr)
    {
        showMessageBox(tr("Unable to read the match settings."));
        return;
    }
    m_pLobbyMenu->requestAutoMatchSignUp(getMatchId(index),
                                        static_cast<qint32>(minGames->getCurrentValue()),
                                        static_cast<qint32>(maxGames->getCurrentValue()));
}

void DialogAutoMatches::withdraw(qint32 index)
{
    QString matchId = getMatchId(index);
    if (!matchId.isEmpty())
    {
        m_pLobbyMenu->requestAutoMatchWithdraw(matchId);
    }
}

void DialogAutoMatches::receivedAutoMatchInfo(const QJsonObject &objData)
{
    m_objData = objData;
    refreshUi();
}

void DialogAutoMatches::receivedActionResult(const QJsonObject &objData)
{
    showMessageBox(objData.value(JsonKeys::JSONKEY_DESCRIPTION).toString());
    if (objData.value(JsonKeys::JSONKEY_RESULT).toBool())
    {
        m_pLobbyMenu->requestShowAutoMatches();
    }
}