#ifndef MULTIPLAYERMENU_H
#define MULTIPLAYERMENU_H

#include "memory.h"
#include <QObject>
#include <QTimer>
#include "oxygine-framework.h"

#include "objects/base/chat.h"

#include "menue/mapselectionmapsmenue.h"

#include "network/NetworkInterface.h"

#include "multiplayer/password.h"

class Multiplayermenu : public MapSelectionMapsMenue
{
    Q_OBJECT
public:
    explicit Multiplayermenu(QString adress, QString password, bool host);
    explicit Multiplayermenu(spNetworkInterface pNetworkInterface, QString password, bool host);
    virtual ~Multiplayermenu() = default;

    /**
     * @brief existsMap
     * @param fileName
     * @param hash
     */
    bool existsMap(QString& fileName, QByteArray& hash, QString& scriptFileName, QByteArray& scriptHash);
    /**
     * @brief createChat
     */
    void createChat();
    /**
     * @brief disconnectNetwork
     */
    void disconnectNetwork();
    /**
     * @brief connectNetworkSlots
     */
    void connectNetworkSlots();
    /**
     * @brief showRuleSelection
     */
    virtual void showRuleSelection() override;
signals:
    void sigConnected();
    void sigHostGameLaunched();
    void sigLoadSaveGame();
    void sigShowIPs();
public slots:

    // general slots
    virtual void slotButtonBack() override;
    virtual void slotButtonNext() override;
    virtual void startGame() override;

    // network slots
    void playerJoined(quint64 socketID);
    void disconnected(quint64 socketID);
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);

    virtual void hideMapSelection() override;
    virtual void showMapSelection() override;

    void showLoadSaveGameDialog();
    void loadSaveGame(QString filename);

    void slotCancelHostConnection();
    void slotHostGameLaunched();
protected slots:
    void countdown();
protected:
    bool getGameReady();
    void sendServerReady(bool value);
    void initClientGame(quint64 socketID, QDataStream &stream);
    void loadMultiplayerMap();
    void showIPs();
    GameMap* createMapFromStream(QString mapFile, QString scriptFile, QDataStream &stream);
    QString getNewFileName(QString filename);
    void clientMapInfo(QDataStream & stream, quint64 socketID);
    /**
     * @brief filterCosmeticMods
     * @param mods
     * @param versions
     */
    void filterCosmeticMods(QStringList & mods, QStringList & versions, bool filter);
    bool checkMods(const QStringList & mods, const QStringList & versions, bool filter);
    void requestRule(quint64 socketID);
    void sendInitUpdate(QDataStream & stream, quint64 socketID);
    void requestMap(quint64 socketID);
    void recieveMap(QDataStream & stream, quint64 socketID);
    void playerJoinedServer(QDataStream & stream, quint64 socketID);
private:
    /**
     * @brief init
     */
    void init();
    /**
     * @brief startGameOnServer
     */
    void startGameOnServer();
    /**
     * @brief launchGameOnServer
     * @param stream
     */
    void launchGameOnServer(QDataStream & stream);
    /**
     * @brief sendSlaveReady
     */
    void sendSlaveReady();
    /**
     * @brief acceptNewConnection
     * @param socketID
     */
    void acceptNewConnection(quint64 socketID);
    /**
     * @brief initClientAndWaitForConnection
     */
    void initClientAndWaitForConnection();
    /**
     * @brief startCountdown
     */
    void startCountdown();
    /**
     * @brief markGameReady
     */
    void markGameReady();
    /**
     * @brief changeButtonText
     */
    void changeButtonText();
private:
    bool m_Host{false};
    spNetworkInterface m_NetworkInterface;
    oxygine::spButton m_pHostAdresse;
    spChat m_Chat;
    QTimer m_GameStartTimer;
    qint32 counter{5};
    oxygine::spButton m_pButtonLoadSavegame;
    bool m_saveGame{false};
    bool m_local{true};
    bool m_slaveGameReady{false};
    Password m_password;
};

#endif // MULTIPLAYERMENU_H
