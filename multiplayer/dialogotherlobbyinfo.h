#ifndef DIALOGOTHERLOBBYINFO_H
#define DIALOGOTHERLOBBYINFO_H

#include <QJsonObject>

#include "objects/dialogs/customdialog.h"

class LobbyMenu;
class DialogOtherLobbyInfo;
using spDialogOtherLobbyInfo = std::shared_ptr<DialogOtherLobbyInfo>;

class DialogOtherLobbyInfo final : public CustomDialog
{
    Q_OBJECT
public:
    explicit DialogOtherLobbyInfo(LobbyMenu* pBaseMenu);
    virtual ~DialogOtherLobbyInfo() = default;

    Q_INVOKABLE void requestPlayersFromServer(const QString & searchFilter);    
    Q_INVOKABLE QStringList getFoundPlayers();
    Q_INVOKABLE qint32 getFoundPlayerSize();
    Q_INVOKABLE QString getUserName(qint32 idx);
    Q_INVOKABLE void showPlayerStats(const QString & player);
    Q_INVOKABLE void showAutoMatches();
    Q_INVOKABLE LobbyMenu* getLobbyMenu();
    Q_INVOKABLE void showDownloadMap();
    Q_INVOKABLE void showDownloadReplay();
private slots:
    void onSearchedPlayersReceived(const QStringList & foundPlayers);
    void receivedPlayerStats(const QJsonObject & objData);
    void receivedShowAutoMatches(const QJsonObject & objData);
private:
    LobbyMenu* m_pLobbyMenu{nullptr};
    QStringList m_foundPlayers;
};

Q_DECLARE_INTERFACE(DialogOtherLobbyInfo, "DialogOtherLobbyInfo");

#endif // DIALOGOTHERLOBBYINFO_H
