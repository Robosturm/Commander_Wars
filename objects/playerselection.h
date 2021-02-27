#ifndef PLAYERSELECTION_H
#define PLAYERSELECTION_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/base/panel.h"

#include "objects/base/spinbox.h"

#include "objects/base/dropdownmenu.h"
#include "objects/base/dropdownmenucolor.h"
#include "objects/base/checkbox.h"
#include "objects/base/dropdownmenusprite.h"

#include "network/NetworkInterface.h"

#include "gameinput/basegameinputif.h"

#include "game/campaign.h"

class PlayerSelection;
typedef oxygine::intrusive_ptr<PlayerSelection> spPlayerSelection;

class PlayerSelection : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit PlayerSelection(qint32 width, qint32 heigth);
    virtual ~PlayerSelection() = default;

    void showPlayerSelection();
    void resetPlayerSelection();
    void attachNetworkInterface(spNetworkInterface pNetworkInterface);
    bool hasNetworkInterface()
    {
        return m_pNetworkInterface.get() != nullptr;
    }
    void attachCampaign(spCampaign campaign);
    bool isOpenPlayer(qint32 player);
    bool hasOpenPlayer();
    QString getPlayerAiName(qint32 player);
    void setPlayerAiName(qint32 player, QString name);
    GameEnums::AiTypes getPlayerAiType(qint32 player);
    /**
     * @brief sendPlayerRequest sends a request to play as a human player
     * @param socketID
     * @param player -1 for any player
     */
    void sendPlayerRequest(quint64 socketID, qint32 player, GameEnums::AiTypes aiType);
    void playerDataChanged();
    void updateCOData(qint32 playerIdx);
    bool getReady(qint32 playerIdx);
    bool getPlayerReady();
    void setPlayerReady(bool value);
    /**
     * @brief sendPlayerReady
     * @param socketID
     * @param player
     * @param value
     */
    void sendPlayerReady(quint64 socketID, const QVector<qint32> & player, bool value);
    /**
     * @brief getSaveGame
     * @return
     */
    bool getSaveGame() const;
    /**
     * @brief setSaveGame
     * @param value
     */
    void setSaveGame(bool value);

    bool getIsServerGame() const;
    void setIsServerGame(bool isServerGame);

signals:
    void sigCOsRandom(qint32 mode);
    void sigShowSelectCO(qint32 player, quint8 co);
    void sigShowSelectCOPerks(qint32 player);
    void buttonShowAllBuildList();
    void buttonShowPlayerBuildList(qint32 player);
    void sigAiChanged(qint32 player);
    void sigSelectedArmyChanged(qint32 player, QString army);
    /**
     * @brief sigDisconnect emitted when we need to leave the game cause we don't own a player
     */
    void sigDisconnect();
public slots:
    // slots for automation
    /**
     * @brief selectPlayerAi
     * @param player
     * @param eAiType
     */
    void selectPlayerAi(qint32 player, GameEnums::AiTypes eAiType);

public slots:
    // slots for changing player data
    void allPlayerIncomeChanged(float value);
    void playerIncomeChanged(float value, qint32 playerIdx);
    void allPlayerStartFundsChanged(float value);
    void playerStartFundsChanged(float value, qint32 playerIdx);
    void playerTeamChanged(qint32 value, qint32 playerIdx);
    void playerColorChanged(QColor value, qint32 playerIdx, qint32 item);
    void selectedArmyChanged(qint32 player, QString army);
    void showSelectCO(qint32 player, quint8 co);
    void playerCO1Changed(QString coid, qint32 playerIdx);
    void updateCO1Sprite(QString coid, qint32 playerIdx);
    void playerCO2Changed(QString coid, qint32 playerIdx);
    void updateCO2Sprite(QString coid, qint32 playerIdx);
    void playerCOCanceled();
    void slotCOsRandom(qint32 mode);
    void slotShowAllBuildList();
    void slotShowPlayerBuildList(qint32 player);
    void slotChangeAllBuildList(qint32, QStringList buildList);
    void slotChangePlayerBuildList(qint32 player, QStringList buildList);
    void selectAI(qint32 player);

    /**
     * @brief showSelectCOPerks
     * @param player
     */
    void showSelectCOPerks(qint32 player);
    // network stuff
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
    void disconnected(quint64 socketID);
    /**
     * @brief updatePlayerData updates the visual player data when data has been changed via network
     * @param player the player that has changed
     */
    void updatePlayerData(qint32 player);
    void setPlayerAi(qint32 player, GameEnums::AiTypes eAiType, QString name);
    /**
     * @brief recievePlayerReady
     * @param socketID
     * @param stream
     */
    void recievePlayerReady(quint64 socketID, QDataStream& stream);
protected:
    /**
     * @brief createArmySelection
     * @param ai
     * @param xPositions
     * @param y
     * @param itemIndex
     */
    void createArmySelection(qint32 ai, QVector<qint32> & xPositions, qint32 y, qint32 itemIndex, qint32 player);
    /**
     * @brief getSelectableArmies
     * @return
     */
    QStringList getSelectableArmies();
    /**
     * @brief createAi
     * @param player
     * @param type
     */
    void createAi(qint32 player, GameEnums::AiTypes type);
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
     * @brief sendOpenPlayerCount
     */
    void sendOpenPlayerCount();

    void createPlayerChangedData(QByteArray & data, quint64 socketId, QString name, qint32 aiType, qint32 player, bool clientRequest);
private:
    // player selection
    spPanel m_pPlayerSelection;

    QVector<oxygine::spSprite> m_playerCO1;
    QVector<oxygine::spSprite> m_playerCO2;
    QVector<oxygine::spButton> m_playerPerks;
    QVector<spDropDownmenuSprite> m_playerArmy;
    QVector<spDropDownmenuColor> m_playerColors;
    QVector<spSpinBox> m_playerIncomes;
    QVector<spSpinBox> m_playerStartFunds;
    QVector<spDropDownmenu> m_playerAIs;
    QVector<spCheckbox> m_pReadyBoxes;
    /**
     * @brief m_PlayerSockets holds which socket owns which player
     * For clients and local games this always contains 0
     * For the host this contains 0 for owned by the server or the socket id for client owned
     */
    QVector<quint64> m_PlayerSockets;
    QVector<spDropDownmenu> m_playerTeams;
    QVector<oxygine::spButton> m_playerBuildlist;

    spNetworkInterface m_pNetworkInterface{nullptr};
    spCampaign m_pCampaign;

    bool saveGame{false};
    bool m_PlayerReady{false};
    bool m_isServerGame{false};
};

#endif // PLAYERSELECTION_H
