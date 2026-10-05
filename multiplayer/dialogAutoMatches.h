#pragma once

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

#include "objects/dialogs/customdialog.h"

class LobbyMenu;
class DialogAutoMatches;
using spDialogAutoMatches = std::shared_ptr<DialogAutoMatches>;

class DialogAutoMatches final : public CustomDialog
{
public:
    explicit DialogAutoMatches(LobbyMenu *pBaseMenu, const QJsonObject &objData);
    virtual ~DialogAutoMatches() = default;
    Q_INVOKABLE qint32 getMatchCount() const;
    Q_INVOKABLE QString getMatchName(qint32 index) const;
    Q_INVOKABLE QString getMatchDescription(qint32 index) const;
    Q_INVOKABLE QString getMatchState(qint32 index) const;
    Q_INVOKABLE QString getMatchMmr(qint32 index) const;
    Q_INVOKABLE QString getMatchBracketInfo(qint32 index) const;
    Q_INVOKABLE bool isMatchSignedUp(qint32 index) const;
    Q_INVOKABLE bool isMatchActionAllowed(qint32 index) const;
    Q_INVOKABLE QString getMatchId(qint32 index) const;
    Q_INVOKABLE void signUp(qint32 index);
    Q_INVOKABLE void withdraw(qint32 index);
    void receivedAutoMatchInfo(const QJsonObject &objData);
    void receivedActionResult(const QJsonObject &objData);

private:
    QJsonObject getMatch(qint32 index) const;
    LobbyMenu *m_pLobbyMenu{nullptr};
    QJsonObject m_objData;
};

Q_DECLARE_INTERFACE(DialogAutoMatches, "DialogAutoMatches");