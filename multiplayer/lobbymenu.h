#ifndef LOBBYMENU_H
#define LOBBYMENU_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "objects/tableView/complextableview.h"

#include "network/networkgamedata.h"
#include "network/networkInterface.h"

#include "multiplayer/networkcommands.h"
#include "multiplayer/password.h"

#include "menue/basemenu.h"

class LobbyMenu;
using spLobbyMenu = std::shared_ptr<LobbyMenu>;

class LobbyMenu final : public Basemenu
{
    Q_OBJECT
    static constexpr qint32 REQUEST_COUNT = 50;
public:
    enum class GameViewMode
    {
        OpenGames,
        OwnGames,
        ObserveGames,
    };

    explicit LobbyMenu();
    virtual ~LobbyMenu() = default;
    Q_INVOKABLE bool getServerRequestNewPassword() const;
    Q_INVOKABLE void setServerRequestNewPassword(bool newServerRequestNewPassword);
    Q_INVOKABLE void onLogin();
    Q_INVOKABLE bool isValidEmailAdress(const QString emailAdress);
    Q_INVOKABLE bool isValidPassword(const QString password);
    Q_INVOKABLE void leaveServer();
    Q_INVOKABLE void createServerAccount(const QString password, const QString emailAdress);
    Q_INVOKABLE void deleteServerAccount(const QString password, const QString emailAdress);
    Q_INVOKABLE void loginToServerAccount(const QString password);
    Q_INVOKABLE void resetPasswordOnServerAccount(const QString emailAdress);
    Q_INVOKABLE void changePasswordOnServerAccount(const QString oldServerPassword, const QString newServerPassword);
    Q_INVOKABLE void enableServerButtons(bool enable);
    Q_INVOKABLE void requestObserverUpdateGames();
    Q_INVOKABLE void showContactingServer();
    Q_INVOKABLE void requestPlayersFromServer(const QString & searchFilter);
    Q_INVOKABLE void requestPlayerStats(const QString & player);
    Q_INVOKABLE void uploadMap(const  QString & selectedFilePath);
    Q_INVOKABLE void requestDownloadMap(const QJsonObject & objData);
    void sendCommandToServer(const QJsonObject & objData);
signals:
    void sigExitMenue();
    void sigHostServer();
    void sigHostLocal();
    void sigJoinGame();
    void sigJoinAdress();
    void sigObserveGame();
    void sigObserveAdress();
    void sigUpdateGamesView();
    void sigRequestUpdateGames();
    void sigServerResponded();
    void sigOther();
    void sigShowNextStep();
    void sigShowPreviousStep();
    void sigShowStart();
    void sigShowEnd();
    void sigSearchedPlayersReceived(const QStringList & foundPlayers);
    void sigReceivedPlayerStats(const QJsonObject & objData);
    void sigRequestShowAutoMatches(const QJsonObject & objData);
    void sigReceivedAvailableMaps(const QJsonObject & objData);
    void sigOnDownloadedResponse(bool success);
    void sigOnMapDeleteResponse(bool success);

public slots:
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
    void exitMenue();
    void hostLocal();
    void hostServer();
    void joinGame();
    void joinGamePassword(QString password);
    void join(QString adress, QString password);
    void joinAdress();
    void updateGamesView();
    void selectGame();
    void observe(QString adress, QString password);
    void observeAdress();
    void observeGame();
    void observeGamePassword(QString password);
    void connected(quint64 socket);
    void requestUpdateGames();
    void cancelWaitingForServer();
    void showNextStep();    
    void showPreviousStep();
    void showStart();
    void showEnd();
    void requestShowAutoMatches();
protected slots:
    virtual void onEnter() override;
private:
    void onShowOtherDialog();
    void receivedShowAutoMatches(const QJsonObject & objData);
    void updateGameData(const QJsonObject & objData);
    void joinSlaveGame(const QJsonObject & objData);
    void checkVersionAndShowInfo(const QJsonObject & objData);
    void handleAccountMessage(quint64 socketID, const QString object, const QJsonObject & objData);
    void requestServerGames();
    void requestUserUpdateGames();
    void onDownloadResponse(const QJsonObject & objData);
    void onMapDeleteResponse(const QJsonObject & objData);
    /**
     * @brief getMinimapImage
     * @param img
     * @param data
     */
    void getMinimapImage(QImage & img, NetworkGameData & data);
    /**
     * @brief showNetworkGameData
     * @param data
     */
    void showNetworkGameData(NetworkGameData & data);
    /**
     * 
     */
    void onSearchedPlayersReceived(const QJsonObject &objData);
    /**
     * 
    */
    void onReceivedPlayerStats(const QJsonObject &objData);
    /**
     * @brief onMapUploadResponse
     * @param objData
     */
    void onMapUploadResponse(const QJsonObject & objData);
private:
    spNetworkInterface m_pTCPClient{nullptr};
    QVector<NetworkGameData> m_games;
    NetworkGameData m_currentGame;
    oxygine::spButton m_pButtonHostOnServer;
    oxygine::spButton m_pButtonGameObserve;    
    oxygine::spButton m_pButtonGameJoin;
    oxygine::spButton m_pButtonSwapOpenGamesMode;
    oxygine::spButton m_pButtonSwapOwnGamesMode;
    oxygine::spButton m_pButtonSwapObserveGamesMode;
    oxygine::spButton m_pButtonUpdateGamesMode;
    oxygine::spButton m_pEndStepButton;
    oxygine::spButton m_pNextStepButton;
    oxygine::spButton m_pPreviousStepButton;
    oxygine::spButton m_pStartStepButton;
    oxygine::spButton m_pOtherButton;
    spLabel m_matchViewInfo;
    spComplexTableView m_gamesview;
    QString m_password;
    bool m_loggedIn{false};

    bool m_serverRequestNewPassword;
    GameViewMode m_mode{GameViewMode::OpenGames};
    qint32 m_lastSelectedItem{-1};
    qint32 m_gameIndex{0};
    qint32 m_serverCurrentMatchCount{0};
};

Q_DECLARE_INTERFACE(LobbyMenu, "LobbyMenu");

#endif // LOBBYMENU_H
