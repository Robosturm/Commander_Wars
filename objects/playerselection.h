#ifndef PLAYERSELECTION_H
#define PLAYERSELECTION_H

#include <QObject>

#include "ui_reader/createdgui.h"

#include "network/NetworkInterface.h"

#include "game/gamemap.h"
#include "game/campaign.h"

#include "coreengine/fileserializable.h"

class DropDownmenu;
class PlayerSelection;
using spPlayerSelection = std::shared_ptr<PlayerSelection>;

class PlayerSelection final : public CreatedGui, public FileSerializable
{
    Q_OBJECT
public:
    explicit PlayerSelection(qint32 width, qint32 heigth);
    ~PlayerSelection() = default;
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& stream) const;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& stream);
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() const
    {
        return 1;
    }
    void showPlayerSelection(bool relaunchedLobby = false);
    void attachNetworkInterface(spNetworkInterface pNetworkInterface);
    void attachCampaign(spCampaign campaign);

    /**
     * @brief sendPlayerRequest sends a request to play as a human player
     * @param socketID
     * @param player -1 for any player
     */
    void sendPlayerRequest(quint64 socketID, qint32 player, GameEnums::AiTypes aiType);
    void playerDataChanged();
    /**
     * @brief sendPlayerReady
     * @param socketID
     * @param player
     * @param value
     */
    void sendPlayerReady(quint64 socketID);
    /**
     * @brief setSaveGame
     * @param value
     */
    void setSaveGame(bool value);
    void setMap(GameMap *newPMap);
    /**
     * @brief sendOpenPlayerCount
     */
    void sendOpenPlayerCount();
    /**
     * @brief getUserNames
     */
    QJsonArray getUserNames();
    /**
     * @brief PlayerSelection::getOnlineInfo
     * @return
     */
    QJsonArray getOnlineInfo();
signals :
    /**
         * @brief sigDisconnect emitted when we need to leave the game cause we
         * don't own a player
         */
    void sigDisconnect();
public:
    /**
     * @brief getOpenPlayerCount
     * @return
     */
    Q_INVOKABLE qint32 getOpenPlayerCount();
    /**
     * @brief hasLockedPlayersInCaseOfDisconnect
     * @return
     */
    Q_INVOKABLE bool hasLockedPlayersInCaseOfDisconnect() const;
    /**
     * @brief setLockedAiControl
     * @param player
     * @param value
     */
    Q_INVOKABLE void setLockedAiControl(qint32 player, bool value);
    /**
     * @brief getLockedAiControl
     * @param player
     * @return
     */
    Q_INVOKABLE bool getLockedAiControl(qint32 player);
    Q_INVOKABLE bool hasNetworkInterface() const;
    Q_INVOKABLE bool getIsServerNetworkInterface() const;
    Q_INVOKABLE bool getIsObserverNetworkInterface() const;
    Q_INVOKABLE bool isNotServerChangeable(Player* pPlayer) const;
    /**
     * @brief getSaveGame
     * @return
     */
    Q_INVOKABLE bool getSaveGame() const;
    Q_INVOKABLE bool isOpenPlayer(qint32 player);
    Q_INVOKABLE bool isClosedPlayer(qint32 player);
    Q_INVOKABLE bool hasOpenPlayer();
    Q_INVOKABLE bool hasHumanPlayer();
    Q_INVOKABLE QString getPlayerAiName(qint32 player);
    Q_INVOKABLE void setPlayerAiName(qint32 player, QString name);
    Q_INVOKABLE GameEnums::AiTypes getPlayerAiType(qint32 player);
    Q_INVOKABLE bool getIsServerGame() const;
    Q_INVOKABLE void setIsServerGame(bool isServerGame);
    Q_INVOKABLE bool getIsCampaign() const;
    Q_INVOKABLE bool getIsArmyCustomizationAllowed();
    Q_INVOKABLE void updateCOData(qint32 playerIdx);
    Q_INVOKABLE bool getReady(qint32 playerIdx);
    Q_INVOKABLE bool getPlayerReady();
    Q_INVOKABLE void setPlayerReady(bool value);
    Q_INVOKABLE QStringList getDefaultAiNames() const;
    Q_INVOKABLE QStringList getAiNames() const;
    Q_INVOKABLE QString getNameFromAiType(GameEnums::AiTypes aiType) const;
    Q_INVOKABLE QStringList getTeamNames() const;
    Q_INVOKABLE QStringList getDropDownColorNames() const;
    /**
     * @brief getSelectableArmies
     * @return
     */
    Q_INVOKABLE QStringList getSelectableArmies() const;
    /**
     * @brief getStartColorName
     * @param player
     * @return
     */
    Q_INVOKABLE QString getStartColorName(qint32 player);
    // slots for automation
    /**
     * @brief selectPlayerAi
     * @param player
     * @param eAiType
     */
    Q_INVOKABLE void selectPlayerAi(qint32 player, GameEnums::AiTypes eAiType);
    /**
     * @brief forcePlayerAi forces player ai only for
     * @param player
     * @param eAiType
     */
    Q_INVOKABLE void forcePlayerAi(qint32 player, GameEnums::AiTypes eAiType);
    // slots for changing player data
    Q_INVOKABLE void allPlayerIncomeChanged(float value);
    Q_INVOKABLE void playerIncomeChanged(float value, qint32 playerIdx);
    Q_INVOKABLE void allPlayerStartFundsChanged(float value);
    Q_INVOKABLE void playerStartFundsChanged(float value, qint32 playerIdx);
    Q_INVOKABLE void playerTeamChanged(qint32 value, qint32 playerIdx);
    Q_INVOKABLE void playerColorChanged(QColor displayColor, qint32 playerIdx, qint32 item);
    Q_INVOKABLE void selectedArmyChanged(qint32 player, QString army);
    Q_INVOKABLE void showSelectCO(qint32 player, quint8 co);
    Q_INVOKABLE void playerCO1Changed(QString coid, qint32 playerIdx);
    Q_INVOKABLE void updateCO1Sprite(QString coid, qint32 playerIdx);
    Q_INVOKABLE void playerCO2Changed(QString coid, qint32 playerIdx);
    Q_INVOKABLE void updateCO2Sprite(QString coid, qint32 playerIdx);
    Q_INVOKABLE void slotCOsRandom(qint32 mode);
    Q_INVOKABLE void slotCOsDelete(qint32 mode);
    Q_INVOKABLE void slotShowAllBuildList();
    Q_INVOKABLE void slotShowPlayerBuildList(qint32 player);
    Q_INVOKABLE void slotChangeAllBuildList(qint32, QStringList buildList);
    Q_INVOKABLE void slotChangePlayerBuildList(qint32 player, QStringList buildList);
    Q_INVOKABLE void selectAI(qint32 player);
    Q_INVOKABLE GameMap *getMap() const;
    /**
     * @brief changeTeams
     */
    Q_INVOKABLE void changeAllTeams(qint32 value);
    /**
     * @brief showSelectCOPerks
     * @param player
     */
    Q_INVOKABLE void showSelectCOPerks(qint32 player);
    /**
     * @brief updatePlayerData updates the visual player data when data has been changed via network
     * @param player the player that has changed
     */
    Q_INVOKABLE void updatePlayerData(qint32 player);
    Q_INVOKABLE void setPlayerAi(qint32 player, GameEnums::AiTypes eAiType, QString name);
public slots:
    /**
     * @brief recievePlayerReady
     * @param socketID
     * @param stream
     */
    void recievePlayerReady(quint64 socketID, QDataStream& stream);
protected slots:
    void disconnected(quint64 socketID);
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
protected:
    /**
     * @brief createAi
     * @param player
     * @param type
     */
    void createAi(qint32 player, GameEnums::AiTypes type, QString displayName);
    /**
     * @brief requestPlayer a client requested to get control of a player. We check the request and execute it if it's valid
     * @param socketID
     * @param stream
     */
    void requestPlayer(quint64 socketID, QDataStream& stream);
    /**
     * @brief changePlayer changes the player data on client sites
     * @param socketID
     * @param stream
     */
    void changePlayer(quint64 socketID, QDataStream& stream);
    /**
     * @brief recievedPlayerData
     * @param socketID
     * @param stream
     */
    void recievedPlayerData(quint64 socketID, QDataStream& stream);
    /**
     * @brief PlayerSelection::recievedCOData
     * @param stream
     */
    void recievedCOData(quint64, QDataStream& stream);
    /**
     * @brief recievedColorData
     * @param stream
     */
    void recievedColorData(quint64, QDataStream& stream);

    /**
     * @brief recievePlayerServerReady
     * @param socketID
     * @param stream
     */
    void recievePlayerServerReady(quint64 socketID, QDataStream& stream);
    /**
     * @brief recievePlayerArmy
     * @param socketID
     * @param stream
     */
    void recievePlayerArmy(quint64 socketID, QDataStream& stream);
    /**
     * @brief joinAllowed
     * @param socketId
     * @param username
     * @param eAiType
     * @return
     */
    bool joinAllowed(quint64 socketId, QString username, GameEnums::AiTypes eAiType);
    /**
     * @brief remoteChangePlayerOwner
     * @param username
     * @param player
     * @param eAiType
     */
    void remoteChangePlayerOwner(quint64 socketID, const QString & username, qint32 player, GameEnums::AiTypes eAiType, bool forceAiType = false, bool inSetup = false);
    /**
     * @brief createPlayerChangedData
     * @param data
     * @param socketId
     * @param name
     * @param aiType
     * @param player
     * @param clientRequest
     */
    void createPlayerChangedData(QByteArray & data, quint64 socketId, QString name, qint32 aiType, qint32 player, bool clientRequest, bool inSetup = false);
    /**
     * @brief getDefaultColorCount
     * @return
     */
    qint32 getDefaultColorCount() const;
    /**
     * @brief getDefaultColor
     * @param index
     * @return
     */
    QColor getDefaultColor(qint32 index);
    /**
     * @brief getDisplayColor
     * @param index
     * @param exists
     * @return
     */
    QColor getDisplayColor(qint32 index, bool & exists) const;
    /**
     * @brief tableColorToDisplayColor
     * @param tableColor
     * @return
     */
    QColor tableColorToDisplayColor(QColor tableColor);
    /**
     * @brief displayColorToTableColor
     * @param displayColor
     * @return
     */
    QColor displayColorToTableColor(QColor displayColor);
    /**
     * @brief autoSelectPlayerColors
     */
    void autoSelectPlayerColors();
    /**
     * @brief playerAccessDenied
     */
    void playerAccessDenied();
    /**
     * @brief joinObserver
     */
    void joinObserver(quint64 socketID);
    /**
     * @brief initializeMap sets predefined stuff and fixes old maps
     */
    void initializeMap(bool relaunchedLobby);
    /**
     * @brief updateInitialState
     */
    void updateInitialState(bool relaunchedLobby);
    /**
     * @brief selectInitialCos
     * @param player
     */
    void selectInitialCos(qint32 player);
    /**
     * @brief selectInitialAi
     * @param pPlayerAi
     * @param ai
     * @param aiList
     */
    void selectInitialAi(bool relaunchedLobby, qint32 player, DropDownmenu* pPlayerAi, qint32 & ai, const QStringList & aiList, const QStringList & defaultAiList);
    /**
     * @brief createInitialAi
     * @param pPlayerAi
     * @param ai
     * @param player
     */
    void createInitialAi(DropDownmenu* pPlayerAi, qint32 ai, qint32 player);
private:
    /**
     * @brief m_PlayerSockets holds which socket owns which player
     * For clients and local games this always contains 0
     * For the host this contains 0 for owned by the server or the socket id for client owned
     */
    QVector<quint64> m_playerSockets;
    QVector<bool> m_playerReadyFlags;
    QVector<bool> m_lockedInCaseOfDisconnect;
    QVector<bool> m_lockedAiControl;

    spNetworkInterface m_pNetworkInterface{nullptr};
    spCampaign m_pCampaign;

    bool m_saveGame{false};
    bool m_playerReady{false};
    bool m_isServerGame{false};
    GameMap* m_pMap{nullptr};
};

Q_DECLARE_INTERFACE(PlayerSelection, "PlayerSelection");

#endif // PLAYERSELECTION_H
