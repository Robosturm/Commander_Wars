#ifndef LOBBYMENU_H
#define LOBBYMENU_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/panel.h"

#include "network/tcpclient.h"

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
public slots:
    void exitMenue();    
    void hostLocal();
    void hostServer();
    void joinGame();
    void join(QString adress);
    void joinAdress();
private:
    spPanel m_pGamesPanel;
    spNetworkInterface m_pTCPClient{nullptr};
};

#endif // LOBBYMENU_H
