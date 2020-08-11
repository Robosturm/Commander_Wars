#ifndef MULTIPLAYERMENU_H
#define MULTIPLAYERMENU_H

#include "memory.h"

#include <QObject>
#include <QTimer>
#include "oxygine-framework.h"

#include "objects/chat.h"

#include "menue/mapselectionmapsmenue.h"
#include "network/NetworkInterface.h"

class Multiplayermenu : public MapSelectionMapsMenue
{
    Q_OBJECT
public:
    explicit Multiplayermenu(QString adress, bool host);
    explicit Multiplayermenu(spNetworkInterface pNetworkInterface);
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
    void requestRule(quint64 socketID);
    void sendInitUpdate(QDataStream & stream, quint64 socketID);
    void requestMap(quint64 socketID);
    void recieveMap(QDataStream & stream, quint64 socketID);
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
private:
    bool m_Host{false};
    spNetworkInterface m_NetworkInterface;
    oxygine::spButton m_pHostAdresse;
    spChat m_Chat;
    QTimer m_GameStartTimer;
    qint32 counter{5};
    oxygine::spButton m_pButtonLoadSavegame;
    bool saveGame{false};
    bool _local{true};
};

#endif // MULTIPLAYERMENU_H
