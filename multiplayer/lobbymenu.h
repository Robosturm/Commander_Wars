#ifndef LOBBYMENU_H
#define LOBBYMENU_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/panel.h"
#include "objects/base/tableview.h"

#include "network/tcpclient.h"
#include "network/networkgamedata.h"

class LobbyMenu;
using spLobbyMenu = oxygine::intrusive_ptr<LobbyMenu>;

class LobbyMenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit LobbyMenu();
    virtual ~LobbyMenu();

signals:    
    void sigExitMenue();
    void sigHostServer();
    void sigHostLocal();
    void sigJoinGame();
    void sigJoinAdress();
    void sigUpdateGamesView();
public slots:
    void exitMenue();    
    void hostLocal();
    void hostServer();
    void joinGame();
    void joinGamePassword(QString password);
    void join(QString adress, QString password);
    void joinAdress();
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
    void updateGamesView();
    void selectGame();
private:
    spPanel m_pGamesPanel;
    spNetworkInterface m_pTCPClient{nullptr};
    QVector<spNetworkGameData> m_games;
    spNetworkGameData m_currentGame;
    spTableView m_Gamesview;
    bool m_usedForHosting{false};
};

#endif // LOBBYMENU_H
