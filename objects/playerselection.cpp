#include <QJsonDocument>
#include <QJsonObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "ui_reader/uifactory.h"

#include "objects/playerselection.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"

#include "resource_management/cospritemanager.h"
#include "resource_management/gamemanager.h"

#include "objects/dialogs/rules/buildlistdialog.h"
#include "objects/dialogs/rules/coselectiondialog.h"
#include "objects/dialogs/rules/perkselectiondialog.h"
#include "objects/dialogs/dialogmessagebox.h"

#include "objects/base/dropdownmenu.h"
#include "objects/base/dropdownmenucolor.h"
#include "objects/base/spriteobject.h"

#include "gameinput/humanplayerinput.h"

#include "multiplayer/networkcommands.h"

#include "coreengine/filesupport.h"
#include "coreengine/userdata.h"

#include "network/tcpserver.h"
#include "network/JsonKeys.h"

constexpr const char* const CO_ARMY = "CO_ARMY";
constexpr const char* const OBJECT_AI_PREFIX = "AI_";
constexpr const char* const OBJECT_COLOR_PREFIX = "PlayerColor_";
constexpr const char* const OBJECT_TEAM_PREFIX = "Team_";
constexpr const char* const OBJECT_READY_PREFIX = "Ready_";
constexpr const char* const OBJECT_CO1_PREFIX = "CO1_";
constexpr const char* const OBJECT_CO2_PREFIX = "CO2_";


PlayerSelection::PlayerSelection(qint32 width, qint32 heigth)
{
#ifdef GRAPHICSUPPORT
    setObjectName("PlayerSelection");
#endif
    Interpreter::setCppOwnerShip(this);
    setSize(width, heigth);
}

void PlayerSelection::attachNetworkInterface(spNetworkInterface pNetworkInterface)
{
    m_pNetworkInterface = pNetworkInterface;
    if (m_pNetworkInterface.get() != nullptr)
    {
        connect(m_pNetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &PlayerSelection::disconnected, Qt::QueuedConnection);
        connect(m_pNetworkInterface.get(), &NetworkInterface::recieveData, this, &PlayerSelection::recieveData, NetworkCommands::UNIQUE_DATA_CONNECTION);
    }
}

void PlayerSelection::attachCampaign(spCampaign campaign)
{
    m_pCampaign = campaign;
}

bool PlayerSelection::isOpenPlayer(qint32 player)
{
    bool ret = false;
    if (m_pNetworkInterface.get() != nullptr &&
        player >= 0 &&
        player < m_pMap->getPlayerCount())
    {
        DropDownmenu* pDropDownmenu = getCastedObject<DropDownmenu>(OBJECT_AI_PREFIX + QString::number(player));
        if (pDropDownmenu != nullptr)
        {
            if (pDropDownmenu->getCurrentItem() == pDropDownmenu->getItemCount() - 1)
            {
                ret = true;
            }
        }
        else if (player >= 0 &&
                 player < m_pMap->getPlayerCount() &&
                 m_pMap->getPlayer(player)->getControlType() == GameEnums::AiTypes_Open)
        {
            ret = true;
        }
    }
    return ret;
}

bool PlayerSelection::isClosedPlayer(qint32 player)
{
    bool ret = false;
    if (!getIsCampaign())
    {
        DropDownmenu* pDropDownmenu = getCastedObject<DropDownmenu>(OBJECT_AI_PREFIX + QString::number(player));
        if (pDropDownmenu != nullptr)
        {
            if (m_pNetworkInterface.get() != nullptr)
            {
                if (m_pNetworkInterface->getIsServer())
                {
                    if (pDropDownmenu->getCurrentItem() == pDropDownmenu->getItemCount() - 2)
                    {
                        ret = true;
                    }
                }
                else if (player >= 0 &&
                         player < m_pMap->getPlayerCount() &&
                         m_pMap->getPlayer(player)->getControlType() == GameEnums::AiTypes_Closed)
                {
                    ret = true;
                }
            }
            else if (pDropDownmenu->getCurrentItem() == pDropDownmenu->getItemCount() - 1 &&
                     pDropDownmenu->getItemCount() > 1)
            {
                ret = true;
            }
        }
        else if (player >= 0 &&
                 player < m_pMap->getPlayerCount() &&
                 m_pMap->getPlayer(player)->getControlType() == GameEnums::AiTypes_Closed)
        {
            ret = true;
        }
    }
    return ret;
}

bool PlayerSelection::hasOpenPlayer()
{
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        if (isOpenPlayer(i))
        {
            return true;
        }
    }
    return false;
}

bool PlayerSelection::hasHumanPlayer()
{
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        if (m_pMap->getPlayer(i) != nullptr &&
            m_pMap->getPlayer(i)->getBaseGameInput() != nullptr &&
            m_pMap->getPlayer(i)->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human)
        {
            return true;
        }
    }
    return false;
}

QString PlayerSelection::getPlayerAiName(qint32 player)
{
    if (player >= 0 &&
        player < m_pMap->getPlayerCount())
    {
        return m_pMap->getPlayer(player)->getPlayerNameId();
    }
    else
    {
        return "";
    }
}

void PlayerSelection::setPlayerAiName(qint32 player, QString name)
{
    if (player >= 0 &&
        player < m_pMap->getPlayerCount())
    {
        m_pMap->getPlayer(player)->setPlayerNameId(name);
        DropDownmenu* pDropDownmenu = getCastedObject<DropDownmenu>(OBJECT_AI_PREFIX + QString::number(player));
        if (pDropDownmenu != nullptr)
        {
            pDropDownmenu->setCurrentItemText(name);
        }
    }
}

GameEnums::AiTypes PlayerSelection::getPlayerAiType(qint32 player)
{
    if (player >= 0 &&
        player < m_pMap->getPlayerCount())
    {
        return m_pMap->getPlayer(player)->getControlType();
    }
    else
    {
        return GameEnums::AiTypes_Open;
    }
}

void PlayerSelection::showSelectCO(qint32 player, quint8 co)
{    
    QString coid = "";
    
    if (m_pMap->getPlayer(player)->getCO(co) != nullptr)
    {
        coid = m_pMap->getPlayer(player)->getCO(co)->getCoID();
    }
    QStringList cos;
    if (m_pCampaign.get() != nullptr)
    {
        cos = m_pCampaign->getSelectableCOs(m_pMap, player, co);
    }
    else
    {
        cos = m_pMap->getGameRules()->getCOBannlist();
        if (cos.size() == 0)
        {
            cos= COSpriteManager::getInstance()->getLoadedRessources();
        }
        Userdata* pUserdata = Userdata::getInstance();
        auto items = pUserdata->getShopItemsList(GameEnums::ShopItemType_CO, false);
        for (const auto & item : items)
        {
            cos.removeAll(item);
        }
        if (cos.size() == 0)
        {
            cos.push_back("");
        }
    }
    if (cos.size() == 0 ||
        cos[0] != "")
    {
        spCOSelectionDialog dialog = spCOSelectionDialog::create(m_pMap, coid, m_pMap->getPlayer(player)->getColor(), player, cos);
        oxygine::Stage::getStage()->addChild(dialog);
        if (co == 0)
        {
            connect(dialog.get(), &COSelectionDialog::editFinished, this , &PlayerSelection::playerCO1Changed, Qt::QueuedConnection);
        }
        else
        {
            connect(dialog.get(), &COSelectionDialog::editFinished, this , &PlayerSelection::playerCO2Changed, Qt::QueuedConnection);
        }
    }
}

bool PlayerSelection::getIsCampaign() const
{
    return (m_pCampaign.get() != nullptr && !GameConsole::getDeveloperMode());
}

bool PlayerSelection::getIsArmyCustomizationAllowed()
{    
    return (m_pCampaign.get() == nullptr ||
            m_pCampaign->getAllowArmyCustomization(m_pMap) ||
            GameConsole::getDeveloperMode());
}

bool PlayerSelection::hasLockedPlayersInCaseOfDisconnect() const
{
    for (auto & locked : m_lockedInCaseOfDisconnect)
    {
        if (locked)
        {
            return true;
        }
    }
    return false;
}

void PlayerSelection::setLockedAiControl(qint32 player, bool value)
{
    if (player >= 0 && player < m_lockedAiControl.size())
    {
        m_lockedAiControl[player] = value;
    }
}

bool PlayerSelection::getLockedAiControl(qint32 player)
{
    if (player >= 0 && player < m_lockedAiControl.size())
    {
        return m_lockedAiControl[player];
    }
    return false;
}

bool PlayerSelection::hasNetworkInterface() const
{
    return m_pNetworkInterface.get() != nullptr;
}

bool PlayerSelection::getIsServerNetworkInterface() const
{
    return m_pNetworkInterface.get() != nullptr && m_pNetworkInterface->getIsServer() || m_isServerGame;
}

bool PlayerSelection::getIsObserverNetworkInterface() const
{
    return m_pNetworkInterface.get() != nullptr && m_pNetworkInterface->getIsObserver();
}

bool PlayerSelection::isNotServerChangeable(Player* pPlayer) const
{
    bool notServerChangeAblePlayer = false;
    if (pPlayer->getBaseGameInput() != nullptr)
    {
        notServerChangeAblePlayer = (m_pNetworkInterface->getIsServer() && pPlayer->getBaseGameInput()->getAiType() != GameEnums::AiTypes_ProxyAi);
    }
    return notServerChangeAblePlayer;
}

qint32 PlayerSelection::getDefaultColorCount() const
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "getDefaultPlayerColors";
    QJSValueList args;
    QJSValue ret = pInterpreter->doFunction("PLAYER", function, args);
    qint32 colorCount = 0;
    if (ret.isNumber())
    {
        colorCount = ret.toInt();
    }
    return colorCount;
}

QColor PlayerSelection::getDefaultColor(qint32 index)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "getDefaultColor";
    QJSValueList args({QJSValue(index)});
    QJSValue ret = pInterpreter->doFunction("PLAYER", function, args);
    QColor color(ret.toString());
    return color;
}

QColor PlayerSelection::getDisplayColor(qint32 index, bool & exists) const
{
    exists = false;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "getDisplayColor";
    QJSValueList args({QJSValue(index)});
    QColor displayColor;
    QJSValue ret = pInterpreter->doFunction("PLAYER", function, args);
    if (ret.isString())
    {
        QString colorName = ret.toString();
        if (!colorName.isEmpty())
        {
            displayColor = QColor(colorName);
            exists = true;
        }
    }
    return displayColor;
}

QColor PlayerSelection::tableColorToDisplayColor(QColor tableColor)
{
    qint32 colorCount = getDefaultColorCount();
    QColor displayColor = tableColor;
    for (qint32 i = 0; i < colorCount; ++i)
    {
        QColor color = getDefaultColor(i);
        if (color == tableColor)
        {
            bool exists = false;
            displayColor = getDisplayColor(i, exists);
            if (!exists)
            {
                displayColor = tableColor;
            }
            break;
        }
    }
    return displayColor;
}

QColor PlayerSelection::displayColorToTableColor(QColor displayColor)
{
    qint32 colorCount = getDefaultColorCount();
    QColor tableColor = displayColor;
    for (qint32 i = 0; i < colorCount; ++i)
    {
        bool exists = false;
        QColor color = getDisplayColor(i, exists);
        if (color == displayColor && exists)
        {
            tableColor = getDefaultColor(i);
            break;
        }
    }
    return tableColor;
}

QString PlayerSelection::getStartColorName(qint32 player)
{
    QColor startColor = tableColorToDisplayColor(m_pMap->getPlayer(player)->getColor());
    return startColor.name();
}

void PlayerSelection::initializeMap(bool relaunchedLobby)
{
    m_playerReadyFlags.clear();
    m_lockedInCaseOfDisconnect.clear();
    m_lockedAiControl.clear();
    m_playerSockets.clear();
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        m_playerReadyFlags.append(false);
        m_lockedInCaseOfDisconnect.append(false);
        m_lockedAiControl.append(false);
        m_playerSockets.append(0);
    }
    if (!m_saveGame && !relaunchedLobby)
    {
        CONSOLE_PRINT("PlayerSelection::initializeMap fixing outdated specs.", GameConsole::eDEBUG);
        bool allPlayer1 = true;
        bool allHuman = true;
        const bool isCampaign = getIsCampaign();
        if (!isCampaign)
        {
            for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
            {
                auto bannlist = Settings::getInstance()->getDefaultBannlist();
                if (!bannlist.isEmpty())
                {
                    auto unitList = Filesupport::readList(bannlist, "");
                    if (unitList.items.size() > 0)
                    {
                        m_pMap->getPlayer(i)->setBuildList(unitList.items);
                    }
                }
                Player* pPlayer = m_pMap->getPlayer(i);
                if (pPlayer->getTeam() != 0)
                {
                    allPlayer1 = false;
                }
                if (pPlayer->getBaseGameInput() != nullptr)
                {
                    auto ai = pPlayer->getBaseGameInput()->getAiType();
                    if (ai > GameEnums::AiTypes_Human)
                    {
                        allHuman = false;
                    }
                    pPlayer->setControlType(ai);
                }
                else if (pPlayer->getControlType() != GameEnums::AiTypes_Open &&
                         pPlayer->getControlType() != GameEnums::AiTypes_Closed)
                {
                    CONSOLE_PRINT("PlayerSelection::initializeMap changing player " + QString::number(i) + " to human", GameConsole::eDEBUG);
                    pPlayer->setBaseGameInput(spHumanPlayerInput::create(m_pMap));
                    pPlayer->setControlType(GameEnums::AiTypes_Human);
                }
                else
                {
                    allHuman = false;
                }
            }
        }
        else
        {
            allHuman = false;
            allPlayer1 = false;
        }

        // assume players had no real team assigned
        // reassign each a unique team
        if (allPlayer1)
        {
            CONSOLE_PRINT("PlayerSelection::initializeMap fixing team's cause all players are team 1", GameConsole::eDEBUG);
            for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
            {
                m_pMap->getPlayer(i)->setTeam(i);
            }
        }
        if (allHuman)
        {
            CONSOLE_PRINT("PlayerSelection::initializeMap fixing ai's cause all players are human", GameConsole::eDEBUG);
            for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
            {
                if (i != 0)
                {
                    const auto ai = GameEnums::AiTypes_Normal;
                    m_pMap->getPlayer(i)->setControlType(ai);
                    m_pMap->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(m_pMap, ai));
                }
            }
        }
    }
}

void PlayerSelection::showPlayerSelection(bool relaunchedLobby)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    resetUi();
    initializeMap(relaunchedLobby);
    UiFactory::getInstance().createUi("ui/game/playerSelection.xml", this);
    updateInitialState(relaunchedLobby);
    pApp->continueRendering();
}

void PlayerSelection::updateInitialState(bool relaunchedLobby)
{
    CONSOLE_PRINT("PlayerSelection::updateInitialState", GameConsole::eDEBUG);
    // add player selection information
    QStringList aiList = getAiNames();
    QStringList defaultAiList = getDefaultAiNames();
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = m_pMap->getPlayer(i);
        DropDownmenu* pPlayerAi = getCastedObject<DropDownmenu>(OBJECT_AI_PREFIX + QString::number(i));
        qint32 ai = static_cast<qint32>(pPlayer->getControlType());
        // update co view
        selectInitialCos(i);
        // update ai view
        selectInitialAi(relaunchedLobby, i, pPlayerAi, ai, aiList, defaultAiList);
        createInitialAi(pPlayerAi, ai, i);
    }
}

void PlayerSelection::selectInitialAi(bool relaunchedLobby, qint32 player, DropDownmenu* pPlayerAi, qint32 & ai, const QStringList & aiList, const QStringList & defaultAiList)
{
    CONSOLE_PRINT("PlayerSelection::selectInitialAi player=" + QString::number(player) + " ai=" + QString::number(ai) + " relaunchedLobby=" + QString::number(relaunchedLobby), GameConsole::eDEBUG);
    Player* pPlayer = m_pMap->getPlayer(player);
    const bool isCampaign = getIsCampaign();
    if (relaunchedLobby)
    {
        if (pPlayerAi != nullptr)
        {
            pPlayerAi->setCurrentItemText(pPlayer->getPlayerNameId());
        }
    }
    else if (isCampaign)
    {
        if (ai == 0)
        {
            ai = 0;
            if (pPlayerAi != nullptr)
            {
                pPlayerAi->setCurrentItem(0);
            }
        }
        else
        {
            if (pPlayerAi != nullptr)
            {
                pPlayerAi->setCurrentItemText(defaultAiList[ai]);
                pPlayerAi->setEnabled(false);
            }
        }
    }
    else if (m_pNetworkInterface.get() != nullptr)
    {
        if (m_pNetworkInterface->getIsServer())
        {
            if (m_saveGame)
            {
                if (ai == GameEnums::AiTypes_ProxyAi ||
                    ai == GameEnums::AiTypes_Open)
                {
                    ai = GameEnums::AiTypes_Open;
                    if (pPlayerAi != nullptr)
                    {
                        pPlayerAi->setCurrentItem(aiList.size() - 1);
                    }
                }
                else
                {
                    if (pPlayerAi != nullptr)
                    {
                        pPlayerAi->setCurrentItem(ai);
                    }
                }
            }
            else if (player > 0)
            {
                ai = GameEnums::AiTypes_Open;
                if (pPlayerAi != nullptr)
                {
                    pPlayerAi->setCurrentItem(aiList.size() - 1);
                }
            }
            else
            {
                ai = GameEnums::AiTypes_Human;
                if (pPlayerAi != nullptr)
                {
                    pPlayerAi->setCurrentItem(0);
                }
            }
        }
        else
        {
            ai = GameEnums::AiTypes_Normal;
            if (pPlayerAi != nullptr)
            {
                pPlayerAi->setCurrentItem(ai);
                pPlayerAi->setEnabled(false);
            }
        }
    }
    else
    {
        auto* input = pPlayer->getBaseGameInput();
        if (input == nullptr)
        {
            ai = GameEnums::AiTypes_Closed;
            if (pPlayerAi != nullptr)
            {
                pPlayerAi->setCurrentItem(ai);
            }
        }
        else
        {
            ai = static_cast<qint32>(input->getAiType());
            if (pPlayerAi != nullptr)
            {
                pPlayerAi->setCurrentItem(ai);
            }
        }
    }
}

void PlayerSelection::createInitialAi(DropDownmenu* pPlayerAi, qint32 ai, qint32 player)
{
    CONSOLE_PRINT("PlayerSelection::createInitialAi for player " + QString::number(player) + " with control type " + QString::number(ai), GameConsole::eDEBUG);
    Player* pPlayer = m_pMap->getPlayer(player);
    // create initial selected ai's
    QString displayName = pPlayer->getPlayerNameId();
    if (pPlayerAi != nullptr)
    {
        displayName = pPlayerAi->getCurrentItemText();
    }

    if (getIsCampaign() && ai > 0)
    {
        createAi(player, static_cast<GameEnums::AiTypes>(ai), displayName);
    }
    else if ((m_pNetworkInterface.get() == nullptr ||
              m_pNetworkInterface->getIsServer() ||
              m_isServerGame))
    {
        selectPlayerAi(player, static_cast<GameEnums::AiTypes>(ai));
    }
    else
    {
        createAi(player, static_cast<GameEnums::AiTypes>(ai), displayName);
    }
}

void PlayerSelection::selectInitialCos(qint32 player)
{
    Player* pPlayer = m_pMap->getPlayer(player);
    if (m_pNetworkInterface.get() == nullptr ||
        m_pNetworkInterface->getIsServer())
    {
        if (pPlayer->getCO(0) != nullptr)
        {
            playerCO1Changed(pPlayer->getCO(0)->getCoID(), player);
        }
        else
        {
            playerCO1Changed("", player);
        }
    }
    if (m_pMap->getGameRules()->getSingleCo())
    {
        playerCO2Changed("", player);
    }
    else if (m_pNetworkInterface.get() == nullptr ||
             m_pNetworkInterface->getIsServer())
    {
        if (pPlayer->getCO(1) != nullptr)
        {
            playerCO2Changed(pPlayer->getCO(1)->getCoID(), player);
        }
        else
        {
            playerCO2Changed("", player);
        }
    }
}

QStringList PlayerSelection::getSelectableArmies() const
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue erg = pInterpreter->doFunction("PLAYER", "getArmies");
    QStringList ret = erg.toVariant().toStringList();
    ret.push_front(CO_ARMY);
    return ret;
}

void PlayerSelection::selectedArmyChanged(qint32 player, QString army)
{
    Player* pPlayer = m_pMap->getPlayer(player);
    if (army == CO_ARMY)
    {
        pPlayer->setPlayerArmySelected(false);
    }
    else
    {
        pPlayer->setPlayerArmy(army);
        pPlayer->setPlayerArmySelected(true);
    }
    if (m_pNetworkInterface.get() != nullptr)
    {
        QString command = QString(NetworkCommands::PLAYERARMY);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        sendStream << command;
        sendStream << player;
        sendStream << army;
        emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, true);
    }
}

void PlayerSelection::allPlayerIncomeChanged(float value)
{        
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        m_pMap->getPlayer(i)->setFundsModifier(value);
    }
    playerDataChanged();
}

void PlayerSelection::allPlayerStartFundsChanged(float value)
{
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        m_pMap->getPlayer(i)->setFunds(static_cast<qint32>(value));
    }
    playerDataChanged();
}

void PlayerSelection::playerIncomeChanged(float value, qint32 playerIdx)
{
    m_pMap->getPlayer(playerIdx)->setFundsModifier(value);
    playerDataChanged();
}

void PlayerSelection::slotShowAllBuildList()
{
    // use player 0 as default for showing all
    spBuildListDialog dialog = spBuildListDialog::create(m_pMap, 0, m_pMap->getPlayer(0)->getBuildList());
    oxygine::Stage::getStage()->addChild(dialog);
    connect(dialog.get(), &BuildListDialog::editFinished, this , &PlayerSelection::slotChangeAllBuildList, Qt::QueuedConnection);
}

void PlayerSelection::slotShowPlayerBuildList(qint32 player)
{
    spBuildListDialog dialog = spBuildListDialog::create(m_pMap, player, m_pMap->getPlayer(player)->getBuildList());
    oxygine::Stage::getStage()->addChild(dialog);
    connect(dialog.get(), &BuildListDialog::editFinished, this , &PlayerSelection::slotChangePlayerBuildList, Qt::QueuedConnection);
}

void PlayerSelection::slotChangeAllBuildList(qint32, QStringList buildList)
{
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        m_pMap->getPlayer(i)->setBuildList(buildList);
    }
    playerDataChanged();
}

void PlayerSelection::slotChangePlayerBuildList(qint32 player, QStringList buildList)
{    
    if (player >= 0 && player < m_pMap->getPlayerCount())
    {
        m_pMap->getPlayer(player)->setBuildList(buildList);
    }
    playerDataChanged();
}

void PlayerSelection::playerStartFundsChanged(float value, qint32 playerIdx)
{
    m_pMap->getPlayer(playerIdx)->setFunds(static_cast<qint32>(value));
    playerDataChanged();
}

void PlayerSelection::playerTeamChanged(qint32 value, qint32 playerIdx)
{
    m_pMap->getPlayer(playerIdx)->setTeam(value);
    playerDataChanged();
}

void PlayerSelection::playerDataChanged()
{
    if (m_pNetworkInterface.get() != nullptr &&
        m_pNetworkInterface->getIsServer())
    {
        QString command = QString(NetworkCommands::PLAYERDATA);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        sendStream << command;
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            sendStream << pPlayer->getFunds();
            sendStream << pPlayer->getFundsModifier();
            sendStream << pPlayer->getTeam();
            QStringList buildList = pPlayer->getBuildList();
            sendStream << static_cast<qint32>(buildList.size());
            for (qint32 i2 = 0; i2 < buildList.size(); i2++)
            {
                sendStream << buildList[i2];
            }
        }
        emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, true);
    }
}

void PlayerSelection::playerColorChanged(QColor displayColor, qint32 playerIdx, qint32 item)
{
    QColor tableColor = displayColorToTableColor(displayColor);
    m_pMap->getPlayer(playerIdx)->setColor(tableColor, item);
    if (m_pNetworkInterface.get() != nullptr)
    {
        QString command = QString(NetworkCommands::COLORDATA);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        sendStream << command;
        sendStream << playerIdx;
        sendStream << displayColor;
        emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, true);
    }
}

void PlayerSelection::playerCO1Changed(QString coid, qint32 playerIdx)
{    
    if (!m_saveGame)
    {
        CO* pCO = m_pMap->getPlayer(playerIdx)->getCO(1);
        if (coid == "" ||
            coid == CO::CO_RANDOM ||
            pCO == nullptr ||
            pCO->getCoID() != coid)
        {
            QStringList perks;
            CO* pCurrentCO = m_pMap->getPlayer(playerIdx)->getCO(0);
            if (pCurrentCO != nullptr)
            {
                perks = pCurrentCO->getPerkList();
            }
            m_pMap->getPlayer(playerIdx)->setCO(coid, 0);
            pCurrentCO = m_pMap->getPlayer(playerIdx)->getCO(0);
            if (pCurrentCO != nullptr)
            {
                pCurrentCO->setPerkList(perks);
                pCurrentCO->setCoStyleFromUserdata();
            }
            updateCOData(playerIdx);
        }
    }
    updateCO1Sprite(coid, playerIdx);
    if (getIsCampaign() && m_pCampaign->getAutoSelectPlayerColors(m_pMap))
    {
        autoSelectPlayerColors();
    }
}

void PlayerSelection::autoSelectPlayerColors()
{
    QVector<qint32> usedColors;
    QVector<qint32> openPlayers;
    qint32 playerCount = m_pMap->getPlayerCount();
    DropDownmenuColor* pPlayer0Color= getCastedObject<DropDownmenuColor>(OBJECT_COLOR_PREFIX + QString::number(0));
    if (pPlayer0Color != nullptr)
    {
        for (qint32 i = 0; i < playerCount; ++i)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            CO* pCO = pPlayer->getCO(0);
            if (pCO != nullptr)
            {
                Interpreter* pInterpreter = Interpreter::getInstance();
                QJSValueList args({pCO->getCOArmy().toLower()});
                QJSValue erg = pInterpreter->doFunction("PLAYER", "getDisplayColorFromArmy", args);
                if (erg.isNumber())
                {
                    qint32 index = erg.toInt();
                    if (index >= 0 && !usedColors.contains(index))
                    {
                        usedColors.append(index);
                        DropDownmenuColor* pDropDownmenuColor = getCastedObject<DropDownmenuColor>(OBJECT_COLOR_PREFIX + QString::number(i));
                        if (pDropDownmenuColor != nullptr)
                        {
                            pDropDownmenuColor->setCurrentItem(index);
                            playerColorChanged(pDropDownmenuColor->getCurrentItemColor(), i, index);
                        }
                    }
                    else
                    {
                        openPlayers.append(i);
                    }
                }
            }
        }
        qint32 colors = pPlayer0Color->getItemCount();
        for (const auto & openPlayer : qAsConst(openPlayers))
        {
            for (qint32 i = 0; i < colors; ++i)
            {
                if (!usedColors.contains(i))
                {
                    usedColors.append(i);
                    DropDownmenuColor* pDropDownmenuColor = getCastedObject<DropDownmenuColor>(OBJECT_COLOR_PREFIX + QString::number(i));
                    if (pDropDownmenuColor != nullptr)
                    {
                        pDropDownmenuColor->setCurrentItem(i);
                        playerColorChanged(pDropDownmenuColor->getCurrentItemColor(), openPlayer, i);
                    }
                    break;
                }
            }
        }
    }
}

GameMap *PlayerSelection::getMap() const
{
    return m_pMap;
}

void PlayerSelection::setMap(GameMap *newPMap)
{
    m_pMap = newPMap;
}

void PlayerSelection::updateCO1Sprite(QString coid, qint32 playerIdx)
{    
    if (m_pMap)
    {
        CO* pCurrentCO = m_pMap->getPlayer(playerIdx)->getCO(0);
        oxygine::ResAnim* pAnim = nullptr;
        if (coid.isEmpty() || pCurrentCO == nullptr)
        {
            pAnim = COSpriteManager::getInstance()->getResAnim("no_co+info");
        }
        else
        {
            pAnim = pCurrentCO->getResAnim((coid + "+info"));
        }

        SpriteObject* pSprite = getCastedObject<SpriteObject>(OBJECT_CO1_PREFIX + QString::number(playerIdx));
        if (pSprite != nullptr)
        {
            pSprite->setResAnim(pAnim);
            if (pAnim != nullptr)
            {
                pSprite->setScale(2.0f * pAnim->getWidth() / 32.0f);
            }
            else
            {
                pSprite->setScale(2.0f);
            }
        }
    }
}

void PlayerSelection::playerCO2Changed(QString coid, qint32 playerIdx)
{    
    if (!m_saveGame)
    {
        CO* pCO = m_pMap->getPlayer(playerIdx)->getCO(0);
        if (coid == "" ||
            coid == CO::CO_RANDOM ||
            pCO == nullptr ||
            pCO->getCoID() != coid)
        {
            QStringList perks;
            CO* pCurrentCO = m_pMap->getPlayer(playerIdx)->getCO(1);
            if (pCurrentCO != nullptr)
            {
                perks = pCurrentCO->getPerkList();
            }
            m_pMap->getPlayer(playerIdx)->setCO(coid, 1);
            pCurrentCO = m_pMap->getPlayer(playerIdx)->getCO(1);
            if (pCurrentCO != nullptr)
            {
                pCurrentCO->setPerkList(perks);
                pCurrentCO->setCoStyleFromUserdata();
            }
            updateCOData(playerIdx);
        }
    }
    updateCO2Sprite(coid, playerIdx);
}

void PlayerSelection::updateCO2Sprite(QString coid, qint32 playerIdx)
{
    if (m_pMap)
    {
        CO* pCurrentCO = m_pMap->getPlayer(playerIdx)->getCO(1);
        oxygine::ResAnim* pAnim = nullptr;
        if (coid.isEmpty() || pCurrentCO == nullptr)
        {
            pAnim = COSpriteManager::getInstance()->getResAnim("no_co+info");
        }
        else
        {
            pAnim = pCurrentCO->getResAnim((coid + "+info"));
        }
        SpriteObject* pSprite = getCastedObject<SpriteObject>(OBJECT_CO2_PREFIX + QString::number(playerIdx));
        if (pSprite != nullptr)
        {
            pSprite->setResAnim(pAnim);
            if (pAnim != nullptr)
            {
                pSprite->setScale(2.0f * pAnim->getWidth() / 32.0f);
            }
            else
            {
                pSprite->setScale(2.0f);
            }
        }
    }
}

void PlayerSelection::updateCOData(qint32 playerIdx)
{
    if (m_pNetworkInterface.get() != nullptr)
    {
        QString command = QString(NetworkCommands::CODATA);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        
        Player* pPlayer = m_pMap->getPlayer(playerIdx);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        sendStream << command;
        sendStream << playerIdx;
        CO* pCO = pPlayer->getCO(0);
        QString coid = "";
        QStringList perks;
        if (pCO != nullptr)
        {
            coid = pCO->getCoID();
            perks = pCO->getPerkList();
        }
        sendStream << coid;
        Filesupport::writeVectorList(sendStream, perks);
        if (pCO != nullptr)
        {
            pCO->writeCoStyleToStream(sendStream);
        }
        pCO = pPlayer->getCO(1);
        coid = "";
        perks.clear();
        if (pCO != nullptr)
        {
            coid = pCO->getCoID();
            perks = pCO->getPerkList();
        }
        sendStream << coid;
        Filesupport::writeVectorList(sendStream, perks);
        if (pCO != nullptr)
        {
            pCO->writeCoStyleToStream(sendStream);
        }
        emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, true);
    }
}

void PlayerSelection::slotCOsRandom(qint32 mode)
{
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        if ((mode == 0) || mode < 0)
        {
            playerCO1Changed(CO::CO_RANDOM, i);
        }
        if ((mode == 1) || mode < 0)
        {
            playerCO2Changed(CO::CO_RANDOM, i);
        }
    }
}

void PlayerSelection::slotCOsDelete(qint32 mode)
{
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        if ((mode == 0) || mode < 0)
        {
            playerCO1Changed("", i);
        }
        if ((mode == 1) || mode < 0)
        {
            playerCO2Changed("", i);
        }
    }
}

void PlayerSelection::showSelectCOPerks(qint32 player)
{
    Player* pPlayer = m_pMap->getPlayer(player);
    if (pPlayer->getCO(0) != nullptr || pPlayer->getCO(1) != nullptr)
    {
        Userdata* pUserdata = Userdata::getInstance();
        auto hiddenList = pUserdata->getShopItemsList(GameEnums::ShopItemType_Perk, false);
        spPerkSelectionDialog pPerkSelectionDialog = spPerkSelectionDialog::create(m_pMap, pPlayer, false, hiddenList);
        oxygine::Stage::getStage()->addChild(pPerkSelectionDialog);
        connect(pPerkSelectionDialog.get(), &PerkSelectionDialog::sigFinished, this, [this, player]()
        {
            updateCOData(player);
        });
    }
}

void PlayerSelection::selectPlayerAi(qint32 player, GameEnums::AiTypes eAiType)
{
    if (player >= 0 && player < m_pMap->getPlayerCount())
    {
        CONSOLE_PRINT("PlayerSelection::selectPlayerAi for player " + QString::number(player) + " with control type " + QString::number(eAiType), GameConsole::eDEBUG);
        m_pMap->getPlayer(player)->setControlType(eAiType);
        DropDownmenu* pDropDownmenu = getCastedObject<DropDownmenu>(OBJECT_AI_PREFIX + QString::number(player));
        if (pDropDownmenu != nullptr)
        {
            pDropDownmenu->setCurrentItem(static_cast<qint32>(eAiType));
        }
        selectAI(player);
    }
}

void PlayerSelection::forcePlayerAi(qint32 player, GameEnums::AiTypes eAiType)
{
    CONSOLE_PRINT("PlayerSelection::forcePlayerAi for player " + QString::number(player) + " with control type " + QString::number(eAiType), GameConsole::eDEBUG);
    m_pMap->getPlayer(player)->setControlType(eAiType);
    createAi(player, eAiType, "ForcedAi");
}


void PlayerSelection::selectAI(qint32 player)
{
    CONSOLE_PRINT("PlayerSelection::selectAI for player " + QString::number(player), GameConsole::eDEBUG);
    GameEnums::AiTypes type = m_pMap->getPlayer(player)->getControlType();
    DropDownmenu* pDropDownmenu = getCastedObject<DropDownmenu>(OBJECT_AI_PREFIX + QString::number(player));
    if (pDropDownmenu != nullptr)
    {
        type = static_cast<GameEnums::AiTypes>(pDropDownmenu->getCurrentItem());
    }
    if (isOpenPlayer(player))
    {
        CONSOLE_PRINT("PlayerSelection::selectAI player " + QString::number(player) + " is open.", GameConsole::eDEBUG);
        type = GameEnums::AiTypes_Open;
    }
    else if (isClosedPlayer(player))
    {
        CONSOLE_PRINT("PlayerSelection::selectAI player " + QString::number(player) + " is closed.", GameConsole::eDEBUG);
        type = GameEnums::AiTypes_Closed;
    }
    QString name = m_pMap->getPlayer(player)->getPlayerNameId();
    if (type == GameEnums::AiTypes_Human)
    {
        if (!Mainapp::getSlave())
        {
            name = Settings::getInstance()->getUsername();
        }
    }
    else
    {
        QStringList aiTypes = getDefaultAiNames();
        if (type >= 0 && type < aiTypes.size())
        {
            name = aiTypes[type];
        }
    }
    CONSOLE_PRINT("Selecting ai type " + QString::number(type) + " with name " + name + " for payer " + QString::number(player), GameConsole::eDEBUG);
    if (m_pNetworkInterface.get() != nullptr)
    {
        if (m_pNetworkInterface->getIsServer())
        {
            quint64 socket = m_pNetworkInterface->getSocketID();
            createAi(player, type, name);
            qint32 ai = type;
            m_playerSockets[player] = socket;
            if (type == GameEnums::AiTypes_Closed)
            {
                ai = static_cast<qint32>(GameEnums::AiTypes_Closed);
                m_playerSockets[player] = 0;
            }
            else if (type == GameEnums::AiTypes_Open)
            {
                ai = static_cast<qint32>(GameEnums::AiTypes_Open);
                m_playerSockets[player] = 0;
            }
            else if (m_isServerGame &&
                     Mainapp::getSlave() &&
                     type != GameEnums::AiTypes_Human &&
                     !Mainapp::getTrainingSession())
            {
                CONSOLE_PRINT("Remapping ai " + QString::number(ai) + " to proxy ai on slave", GameConsole::eDEBUG);
                ai = type;
                createAi(player, GameEnums::AiTypes_ProxyAi, name);
                m_pMap->getPlayer(player)->setControlType(type);
            }
            else if (Mainapp::getSlave())
            {
                ai = static_cast<qint32>(GameEnums::AiTypes_ProxyAi);
            }
            QByteArray data;
            createPlayerChangedData(data, socket, name, ai, player, false);
            // update data for all clients
            CONSOLE_PRINT(name + " AI " + QString::number(type) + " selected for player " + QString::number(player) + " and socket " + QString::number(socket) + " sending data.", GameConsole::eDEBUG);
            emit m_pNetworkInterface->sig_sendData(0, data, NetworkInterface::NetworkSerives::Multiplayer, false);
            updatePlayerData(player);
        }
        else
        {
            CONSOLE_PRINT("PlayerSelection::selectAI requesting player " + QString::number(player) + " with ai type " + QString::number(type), GameConsole::eDEBUG);
            sendPlayerRequest(0, player, type);
        }
    }
    else
    {
        if (isClosedPlayer(player))
        {
            createAi(player, GameEnums::AiTypes_Closed, name);
        }
        else
        {
            createAi(player, type, name);
        }
    }
}

void PlayerSelection::createAi(qint32 player, GameEnums::AiTypes type, QString displayName)
{    
    if(m_pMap != nullptr)
    {
        CONSOLE_PRINT("PlayerSelection::createAi for player " + QString::number(player) + " with control type " + QString::number(type) + " with display name " + displayName, GameConsole::eDEBUG);
        Player* pPlayer = m_pMap->getPlayer(player);
        pPlayer->setBaseGameInput(BaseGameInputIF::createAi(m_pMap, type));
        pPlayer->setPlayerNameId(displayName);
        pPlayer->setControlType(type);
        if (pPlayer->getBaseGameInput() != nullptr)
        {
            pPlayer->getBaseGameInput()->setEnableNeutralTerrainAttack(m_pMap->getGameRules()->getAiAttackTerrain());
        }
    }
}

QStringList PlayerSelection::getDefaultAiNames() const
{
    QStringList defaultAiList = {tr("Human"), tr("Very Easy"), tr("Normal"), tr("Normal Off."), tr("Normal Def.")}; // heavy ai disabled cause it's not finished
#if HEAVY_AI
    Interpreter* pInterpreter = Interpreter::getInstance();
    GameManager* pGameManager = GameManager::getInstance();
    // heavy ai enable code
    for (qint32 i = 0; i < pGameManager->getHeavyAiCount(); ++i)
    {
        QString id = pGameManager->getHeavyAiID(i);
        QJSValue aiName = pInterpreter->doFunction(id, "getName");
        defaultAiList.append(aiName.toString());
    }
#endif
    defaultAiList.append(tr("Closed"));
    return defaultAiList;
}

QStringList PlayerSelection::getAiNames() const
{
    QStringList aiList = getDefaultAiNames();
    if (getIsCampaign())
    {
        aiList = {tr("Human")};
        if (m_pNetworkInterface.get() != nullptr)
        {
            aiList.append(tr("Open"));
        }
    }
    else if (m_pNetworkInterface.get() != nullptr)
    {
        if (m_pNetworkInterface->getIsServer())
        {
            aiList.append(tr("Open"));
        }
        else
        {
            aiList = {tr("Human"), tr("Open")};
        }
    }
    return aiList;
}

QStringList PlayerSelection::getTeamNames() const
{
    QStringList teamList;
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        teamList.append(tr("Team") + " " + QString::number(i + 1));
    }
    return teamList;
}

QStringList PlayerSelection::getDropDownColorNames() const
{
    qint32 colorCount = getDefaultColorCount();
    QStringList playerColors;
    for (qint32 i = 0; i < colorCount; i++)
    {
        bool exists = false;
        QColor color = getDisplayColor(i, exists);
        playerColors.append(color.name());
    }
    return playerColors;
}

void PlayerSelection::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    // data for us?
    if (service == NetworkInterface::NetworkSerives::Multiplayer)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        QString messageType;
        stream >> messageType;
        CONSOLE_PRINT("Network Command PlayerSelection::recieveData: " + messageType + " for socket " + QString::number(socketID), GameConsole::eDEBUG);
        if (messageType == NetworkCommands::REQUESTPLAYER)
        {
            requestPlayer(socketID, stream);
        }
        else if (messageType == NetworkCommands::PLAYERCHANGED)
        {
            changePlayer(socketID, stream);
            sendOpenPlayerCount();
        }
        else if (messageType == NetworkCommands::PLAYERDATA)
        {
            recievedPlayerData(socketID, stream);
        }
        else if (messageType == NetworkCommands::CODATA)
        {
            recievedCOData(socketID, stream);
        }
        else if (messageType == NetworkCommands::COLORDATA)
        {
            recievedColorData(socketID, stream);
        }
        else if (messageType == NetworkCommands::PLAYERARMY)
        {
            recievePlayerArmy(socketID, stream);
        }
        else if (messageType == NetworkCommands::PLAYERACCESSDENIED)
        {
            playerAccessDenied();
        }
        else if (messageType == NetworkCommands::JOINASOBSERVER)
        {
            joinObserver(socketID);
        }
        else if (messageType == NetworkCommands::SERVERREADY)
        {
            recievePlayerServerReady(socketID, stream);
        }
        else
        {
            CONSOLE_PRINT("Command not handled in playerselection", GameConsole::eDEBUG);
        }
    }
    else if (service == NetworkInterface::NetworkSerives::ServerHostingJson)
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject objData = doc.object();
        QString messageType = objData.value(JsonKeys::JSONKEY_COMMAND).toString();
        CONSOLE_PRINT("Server Command received: " + messageType + " for socket " + QString::number(socketID), GameConsole::eDEBUG);
        if (messageType == NetworkCommands::SERVERREQUESTOPENPLAYERCOUNT)
        {
            sendOpenPlayerCount();
        }
        else
        {
            CONSOLE_PRINT("Command not handled in playerselection", GameConsole::eDEBUG);
        }
    }
}

void PlayerSelection::joinObserver(quint64 socketID)
{
    if (m_pNetworkInterface->getIsServer())
    {
        auto* gameRules = m_pMap->getGameRules();
        auto & observer = gameRules->getObserverList();
        if (observer.size() < gameRules->getMultiplayerObserver())
        {
            observer.append(socketID);
            auto server = oxygine::dynamic_pointer_cast<TCPServer>(m_pNetworkInterface);
            if (server.get())
            {
                auto client = server->getClient(socketID);
                client->setIsObserver(true);
            }
            sendPlayerReady(0);
            sendOpenPlayerCount();
        }
        else
        {
            QString command = QString(NetworkCommands::DISCONNECTINGFROMSERVER);
            CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
            QJsonObject data;
            data.insert(JsonKeys::JSONKEY_COMMAND, command);
            data.insert(JsonKeys::JSONKEY_DISCONNECTREASON, NetworkCommands::DisconnectReason::NoMoreObservers);
            QJsonDocument doc(data);
            emit m_pNetworkInterface->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
        }
    }
}

void PlayerSelection::playerAccessDenied()
{
    if (!m_pNetworkInterface->getIsServer() &&
        !m_pNetworkInterface->getIsObserver() &&
        !hasHumanPlayer())
    {
        CONSOLE_PRINT("No remaining human players found as client", GameConsole::eDEBUG);
        QString message = tr("Connection failed.Reason: No more players available or user is already in the game.");
        spDialogMessageBox pDialog = spDialogMessageBox::create(message);
        oxygine::Stage::getStage()->addChild(pDialog);
        emit m_pNetworkInterface->sigDisconnectClient(0);
    }
}

qint32 PlayerSelection::getOpenPlayerCount()
{
    qint32 openPlayerCount = 0;
    if (m_pMap != nullptr)
    {
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); ++i)
        {
            if (m_pMap->getPlayer(i)->getControlType() == GameEnums::AiTypes_Open)
            {
                ++openPlayerCount;
            }
        }
    }
    return openPlayerCount;
}

void PlayerSelection::sendOpenPlayerCount()
{
    if (Mainapp::getSlaveClient().get() != nullptr)
    {
        QString command = QString(NetworkCommands::SERVEROPENPLAYERCOUNT);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        data.insert(JsonKeys::JSONKEY_SLAVENAME, Settings::getInstance()->getSlaveServerName());
        data.insert(JsonKeys::JSONKEY_OPENPLAYERCOUNT, getOpenPlayerCount());
        data.insert(JsonKeys::JSONKEY_USERNAMES, getUserNames());
        data.insert(JsonKeys::JSONKEY_ONLINEINFO, getOnlineInfo());
        if (m_pMap != nullptr)
        {
            data.insert(JsonKeys::JSONKEY_MATCHOBSERVERCOUNT, m_pMap->getGameRules()->getObserverList().size());
            data.insert(JsonKeys::JSONKEY_MATCHMAXOBSERVERCOUNT, m_pMap->getGameRules()->getMultiplayerObserver());
        }
        QJsonDocument doc(data);
        emit Mainapp::getSlaveClient()->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

QJsonArray PlayerSelection::getOnlineInfo()
{
    QJsonArray onlineInfo;
    if (m_pMap != nullptr)
    {
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); ++i)
        {
            auto* pPlayer = m_pMap->getPlayer(i);
            bool isOnline = pPlayer->getSocketId() != 0;
            pPlayer->setIsOnline(isOnline);
            onlineInfo.append(isOnline);
        }
    }
    return onlineInfo;
}

QJsonArray PlayerSelection::getUserNames()
{
    QJsonArray usernames;
    if (m_pMap != nullptr)
    {
        qint32 count = m_pMap->getPlayerCount();
        for (qint32 i = 0; i < count; ++i)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            if (pPlayer->getControlType() == GameEnums::AiTypes_Human)
            {
                CONSOLE_PRINT("Adding human player " + pPlayer->getPlayerNameId() + " to usernames for player " + QString::number(i), GameConsole::eDEBUG);
                usernames.append(pPlayer->getPlayerNameId());
            }
            else
            {
                CONSOLE_PRINT("Player is ai controlled " + QString::number(pPlayer->getControlType()) + " to usernames for player " + QString::number(i), GameConsole::eDEBUG);
                usernames.append(pPlayer->getPlayerNameId());
            }
        }
    }
    return usernames;
}

bool PlayerSelection::getIsServerGame() const
{
    return m_isServerGame;
}

void PlayerSelection::setIsServerGame(bool isServerGame)
{
    m_isServerGame = isServerGame;
}

void PlayerSelection::recievePlayerReady(quint64 socketID, QDataStream& stream)
{
    if (m_pNetworkInterface->getIsServer())
    {
        bool fixed = false;
        bool value = false;
        stream >> fixed;
        stream >> value;
        CONSOLE_PRINT("PlayerSelection::recievePlayerReady for socket " + QString::number(socketID) + " is " + (value ? "ready" : "not ready"), GameConsole::eDEBUG);
        for  (qint32 i = 0; i < m_playerSockets.size(); i++)
        {
            if (m_playerSockets[i] == socketID)
            {
                CONSOLE_PRINT("Changing player " + QString::number(i), GameConsole::eDEBUG);
                m_playerReadyFlags[i] = value;
                m_lockedInCaseOfDisconnect[i] = value;
                Checkbox* pCheckbox = getCastedObject<Checkbox>(OBJECT_READY_PREFIX + QString::number(i));
                if (pCheckbox != nullptr)
                {
                    pCheckbox->setChecked(value);
                }
            }
        }
        sendPlayerReady(0);
        if (fixed)
        {
            QString command = QString(NetworkCommands::DISCONNECTINGFROMSERVER);
            CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Version::Qt_6_5);
            stream << command;
            emit m_pNetworkInterface->sig_sendData(socketID, data, NetworkInterface::NetworkSerives::Multiplayer, false);
        }
    }
}

void PlayerSelection::sendPlayerReady(quint64 socketID)
{
    if (m_pNetworkInterface->getIsServer())
    {
        QString command = QString(NetworkCommands::SERVERREADY);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        auto playerCount = m_pMap->getPlayerCount();
        sendStream << command;
        sendStream << static_cast<qint32>(m_pMap->getPlayerCount());
        for  (qint32 i = 0; i < playerCount; i++)
        {
            CONSOLE_PRINT("Player " + QString::number(i) + " is " + (m_playerReadyFlags[i] ? "ready" :  "not ready"), GameConsole::eDEBUG);
            sendStream << m_playerReadyFlags[i];
        }
        emit m_pNetworkInterface.get()->sigForwardData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer);
    }
}

void PlayerSelection::recievePlayerServerReady(quint64, QDataStream& stream)
{
    qint32 size = 0;
    stream >> size;
    for  (qint32 i = 0; i < size; i++)
    {
        bool value = false;
        stream >> value;
        m_playerReadyFlags[i] = value;
        Checkbox* pCheckbox = getCastedObject<Checkbox>(OBJECT_READY_PREFIX + QString::number(i));
        if (pCheckbox != nullptr)
        {
            pCheckbox->setChecked(value);
        }
    }
}

bool PlayerSelection::getSaveGame() const
{
    return m_saveGame;
}

void PlayerSelection::setSaveGame(bool value)
{
    m_saveGame = value;
}

void PlayerSelection::sendPlayerRequest(quint64 socketID, qint32 player, GameEnums::AiTypes aiType)
{
    QString command = QString(NetworkCommands::REQUESTPLAYER);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream.setVersion(QDataStream::Version::Qt_6_5);
    sendStream << command;
    // request player smaller 0 for any (the first avaible on the server :)
    sendStream << static_cast<qint32>(player);
    sendStream << Settings::getInstance()->getUsername();
    sendStream << static_cast<qint32>(aiType);
    emit m_pNetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
}

void PlayerSelection::requestPlayer(quint64 socketID, QDataStream& stream)
{
    if (m_pNetworkInterface->getIsServer() ||
        m_isServerGame)
    {
        qint32 player = 0;
        QString username;
        stream >> player;
        stream >> username;
        qint32 aiType;
        stream >> aiType;
        GameEnums::AiTypes eAiType = static_cast<GameEnums::AiTypes>(aiType);
        bool rejoin = false;
        if (player < 0)
        {
            for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
            {
                auto* pPlayer = m_pMap->getPlayer(i);
                if (pPlayer->getPlayerNameId() == username &&
                    pPlayer->getSocketId() == 0)
                {
                    CONSOLE_PRINT("Player " + QString::number(i) + " username " + username + " rejoined lobby game.", GameConsole::eDEBUG);
                    remoteChangePlayerOwner(socketID, username, i, eAiType);
                    rejoin = true;
                }
                else if (Mainapp::getSlave() &&
                         pPlayer->getSocketId() == 0 &&
                         pPlayer->getBaseGameInput() != nullptr &&
                         pPlayer->getControlType() > GameEnums::AiTypes::AiTypes_Human)
                {
                    CONSOLE_PRINT("Slave player " + QString::number(i) + " ai " + pPlayer->getPlayerNameId() + " control type " + QString::number(pPlayer->getControlType()) + " transferred control to socket " + QString::number(socketID), GameConsole::eDEBUG);
                    remoteChangePlayerOwner(socketID, pPlayer->getPlayerNameId(), i, pPlayer->getControlType(), true, true);
                }
            }
        }
        if (!rejoin)
        {
            // the client wants any player?
            if (player < 0)
            {
                for (qint32 i = 0; i < m_pMap->getPlayerCount(); ++i)
                {
                    if (isOpenPlayer(i))
                    {
                        player = i;
                        break;
                    }
                }
            }
            bool allowed = joinAllowed(socketID, username, eAiType);
            bool isOpen = isOpenPlayer(player);
            // opening a player is always valid changing to an human with an open player is also valid
            if (allowed &&
                (isOpen || eAiType == GameEnums::AiTypes_Open))
            {
                remoteChangePlayerOwner(socketID, username, player, eAiType);
            }
            else
            {
                CONSOLE_PRINT("Player change for username " + username + " rejected. Cause player " + QString::number(player) + " for ai " + QString::number(aiType) + " allowed=" + QString::number(allowed) + " open=" + QString::number(isOpen), GameConsole::eDEBUG);
                QString command = NetworkCommands::PLAYERACCESSDENIED;
                QByteArray accessDenied;
                QDataStream sendStream(&accessDenied, QIODevice::WriteOnly);
                sendStream.setVersion(QDataStream::Version::Qt_6_5);
                sendStream << command;
                emit m_pNetworkInterface->sig_sendData(socketID, accessDenied, NetworkInterface::NetworkSerives::Multiplayer, false);
            }
        }
        else
        {
            CONSOLE_PRINT("Finished rejoin for username " + username + " to lobby game.", GameConsole::eDEBUG);
        }
        sendOpenPlayerCount();
    }
}

void PlayerSelection::remoteChangePlayerOwner(quint64 socketID, const QString & username, qint32 player, GameEnums::AiTypes eAiType, bool forceAiType, bool inSetup)
{
    // valid request
    // change data locally and send remote update
    Player* pPlayer = m_pMap->getPlayer(player);
    m_lockedInCaseOfDisconnect[player] = false;
    // we need to handle opening a player slightly different here...
    DropDownmenu* pDropDownmenu = getCastedObject<DropDownmenu>(OBJECT_AI_PREFIX + QString::number(player));
    if (eAiType == GameEnums::AiTypes_Open)
    {
        pPlayer->setControlType(eAiType);
        pPlayer->setBaseGameInput(spBaseGameInputIF());
        pPlayer->setPlayerNameId(getNameFromAiType(eAiType));
        pPlayer->setSocketId(0);
        if (pDropDownmenu != nullptr)
        {
            pDropDownmenu->setCurrentItem(pDropDownmenu->getItemCount() - 1);
        }
        m_playerSockets[player] = 0;
    }
    else
    {
        pPlayer->setControlType(eAiType);
        pPlayer->setBaseGameInput(BaseGameInputIF::createAi(m_pMap, GameEnums::AiTypes_ProxyAi));
        if (eAiType == GameEnums::AiTypes_Closed)
        {
            pPlayer->setPlayerNameId(getNameFromAiType(eAiType));
            m_playerSockets[player] = 0;
        }
        else
        {
            pPlayer->setPlayerNameId(username);
            m_playerSockets[player] = socketID;
        }
        pPlayer->setSocketId(socketID);
        if (pDropDownmenu != nullptr)
        {
            pDropDownmenu->setCurrentItemText(username);
        }
    }
    CONSOLE_PRINT("Player " + username + " change for " + QString::number(player) + " changed locally to ai-type " + QString::number(eAiType) + " for socket " + QString::number(m_playerSockets[player]), GameConsole::eDEBUG);
    updatePlayerData(player);

    qint32 aiType = eAiType;
    if (!forceAiType)
    {
        if (eAiType == GameEnums::AiTypes_Open)
        {
            aiType = static_cast<qint32>(GameEnums::AiTypes_Open);
        }
        else
        {
            aiType = static_cast<qint32>(GameEnums::AiTypes_Human);
        }
    }
    CONSOLE_PRINT("Player change " + QString::number(player) + " changing remote to ai-type " + QString::number(aiType), GameConsole::eDEBUG);
    QByteArray sendDataRequester;
    createPlayerChangedData(sendDataRequester, socketID, username, aiType, player, true, inSetup);
    // create data block for other clients
    if (eAiType == GameEnums::AiTypes_Open)
    {
        aiType = static_cast<qint32>(GameEnums::AiTypes_Open);
    }
    else
    {
        aiType = static_cast<qint32>(GameEnums::AiTypes_ProxyAi);
    }
    QByteArray sendDataOtherClients;
    createPlayerChangedData(sendDataOtherClients, socketID, username, aiType, player, false, inSetup);
    // send player update
    emit m_pNetworkInterface->sig_sendData(socketID, sendDataRequester, NetworkInterface::NetworkSerives::Multiplayer, false);
    emit m_pNetworkInterface.get()->sigForwardData(socketID, sendDataOtherClients, NetworkInterface::NetworkSerives::Multiplayer);
    sendPlayerReady(0);
}

bool PlayerSelection::joinAllowed(quint64 socketId, QString username, GameEnums::AiTypes eAiType)
{
    bool joinAllowed = (Settings::getInstance()->getUsername() != username) || getIsServerGame() || eAiType == GameEnums::AiTypes_Open;
    if (eAiType != GameEnums::AiTypes_Open && joinAllowed)
    {
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); ++i)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            if (pPlayer->getPlayerNameId() == username &&
                socketId != pPlayer->getSocketId())
            {
                joinAllowed = false;
                break;
            }
        }
    }
    return joinAllowed;
}

QString PlayerSelection::getNameFromAiType(GameEnums::AiTypes aiType) const
{
    QString name;
    QStringList aiList = getAiNames();
    if (aiType == GameEnums::AiTypes::AiTypes_Open)
    {
        name = aiList[aiList.size() - 1];
    }
    else if (aiType == GameEnums::AiTypes::AiTypes_Closed)
    {
        name = aiList[aiList.size() - 2];
    }
    else
    {
        if (aiType < aiList.size() - 2)
        {
            name = aiList[aiType];
        }
    }
    return name;
}

void PlayerSelection::changePlayer(quint64 socketId, QDataStream& stream)
{
    if (!m_pNetworkInterface->getIsServer() ||
        m_isServerGame)
    {
        quint64 socket;
        QString name;
        qint32 aiType;
        qint32 player;
        bool clientRequest = false;
        bool setup = false;
        stream >> clientRequest;
        stream >> socket;
        stream >> name;
        stream >> player;
        stream >> aiType;        
        stream >> setup;
        CONSOLE_PRINT("Remote change of Player " + QString::number(player) + " with name " + name + " for socket " + QString::number(socket) + " and ai " + QString::number(aiType) + " in setup " + QString::number(setup), GameConsole::eDEBUG);
        if (socket != m_pNetworkInterface->getSocketID() ||
            aiType != GameEnums::AiTypes::AiTypes_ProxyAi)
        {
            GameEnums::AiTypes originalAiType = static_cast<GameEnums::AiTypes>(aiType);
            if (player < m_playerSockets.size() && m_pMap != nullptr)
            {
                if (aiType != GameEnums::AiTypes::AiTypes_Open &&
                    aiType != GameEnums::AiTypes::AiTypes_Closed)
                {
                    m_playerSockets[player] = socket;
                }
                else
                {
                    m_playerSockets[player] = 0;
                    name = getNameFromAiType(static_cast<GameEnums::AiTypes>(aiType));
                }
                if (Mainapp::getSlave())
                {
                    if (aiType != GameEnums::AiTypes::AiTypes_Open &&
                        aiType != GameEnums::AiTypes::AiTypes_Closed)
                    {
                        aiType = GameEnums::AiTypes::AiTypes_ProxyAi;
                    }
                    CONSOLE_PRINT("Slave remapped change of Player " + QString::number(player) + " with name " + name + " for socket " + QString::number(socket) + " and ai " + QString::number(aiType) + " after validation. Orignal ai " + QString::number(originalAiType), GameConsole::eDEBUG);
                }
                else if (m_isServerGame)
                {
                    if (aiType != GameEnums::AiTypes::AiTypes_Open &&
                        aiType != GameEnums::AiTypes::AiTypes_Human)
                    {
                        aiType = GameEnums::AiTypes::AiTypes_ProxyAi;
                    }
                    CONSOLE_PRINT("Server remapped change of Player " + QString::number(player) + " with name " + name + " for socket " + QString::number(socket) + " and ai " + QString::number(aiType) + " after validation. Orignal ai " + QString::number(originalAiType), GameConsole::eDEBUG);
                }
                else if (!clientRequest)
                {
                    if (aiType != GameEnums::AiTypes::AiTypes_Open &&
                        aiType != GameEnums::AiTypes::AiTypes_Closed)
                    {
                        aiType = GameEnums::AiTypes::AiTypes_ProxyAi;
                    }
                    CONSOLE_PRINT("Remapped change of Player " + QString::number(player) + " with name " + name + " for socket " + QString::number(socket) + " and ai " + QString::number(aiType) + " after validation. Orignal ai " + QString::number(originalAiType), GameConsole::eDEBUG);
                }
                GameEnums::AiTypes eAiType = static_cast<GameEnums::AiTypes>(aiType);
                setPlayerAi(player, eAiType, name);
                m_pMap->getPlayer(player)->deserializeObject(stream);
                m_pMap->getPlayer(player)->setBaseGameInput(BaseGameInputIF::createAi(m_pMap, eAiType));
                m_pMap->getPlayer(player)->setPlayerNameId(name);
                m_pMap->getPlayer(player)->setControlType(originalAiType);
                m_pMap->getPlayer(player)->setSocketId(socketId);

                bool humanFound = false;
                for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
                {
                    if (m_pMap->getPlayer(i)->getControlType() == GameEnums::AiTypes_Human)
                    {
                        humanFound = true;
                        break;
                    }
                }
                updatePlayerData(player);
                if (!humanFound &&
                    !m_pNetworkInterface->getIsObserver() &&
                    !setup)
                {
                    CONSOLE_PRINT("Disconnecting cause controlling no player and isn't an observer", GameConsole::eDEBUG);
                    emit sigDisconnect();
                }
                if (m_isServerGame)
                {
                    if (aiType != GameEnums::AiTypes::AiTypes_Open)
                    {
                        aiType = GameEnums::AiTypes::AiTypes_ProxyAi;
                    }
                    QByteArray data;
                    createPlayerChangedData(data, socket, name, aiType, player, false);
                    emit m_pNetworkInterface->sigForwardData(socketId, data, NetworkInterface::NetworkSerives::Multiplayer);
                }
            }
            else
            {
                CONSOLE_PRINT("Disconnecting cause connection seems faulty", GameConsole::eDEBUG);
                emit sigDisconnect();
            }
        }
        else
        {
            CONSOLE_PRINT("Update ignored", GameConsole::eDEBUG);
        }
        sendOpenPlayerCount();
    }
    else
    {
        CONSOLE_PRINT("Illegal access to PlayerSelection::changePlayer ignored request", GameConsole::eDEBUG);
    }
}

void PlayerSelection::createPlayerChangedData(QByteArray & data, quint64 socketId, QString name, qint32 aiType, qint32 player, bool clientRequest, bool inSetup)
{
    QString command = QString(NetworkCommands::PLAYERCHANGED);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QDataStream sendStream(&data, QIODevice::WriteOnly);
    sendStream.setVersion(QDataStream::Version::Qt_6_5);
    sendStream << command;
    sendStream << clientRequest;
    sendStream << socketId;
    sendStream << name;
    sendStream << player;
    sendStream << aiType;
    sendStream << inSetup;
    m_pMap->getPlayer(player)->serializeObject(sendStream);
}

void PlayerSelection::setPlayerAi(qint32 player, GameEnums::AiTypes eAiType, QString name)
{
    DropDownmenu* pDropDownmenu = getCastedObject<DropDownmenu>(OBJECT_AI_PREFIX + QString::number(player));
    if (eAiType != GameEnums::AiTypes_Human)
    {
        if (eAiType == GameEnums::AiTypes_Open)
        {
            if (pDropDownmenu != nullptr)
            {
                pDropDownmenu->setCurrentItem(pDropDownmenu->getItemCount() - 1);
            }
        }
        else
        {
            setPlayerAiName(player, name);
        }
    }
    else
    {
        if (pDropDownmenu != nullptr)
        {
            pDropDownmenu->setCurrentItem(0);
        }
    }
}

void PlayerSelection::recievedPlayerData(quint64, QDataStream& stream)
{
    if (!m_pNetworkInterface->getIsServer() ||
        m_isServerGame)
    {
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            qint32 funds = 0;
            float fundsModifier = 0.0f;
            qint32 team = 0;
            stream >> funds;
            stream >> fundsModifier;
            stream >> team;
            pPlayer->setFunds(funds);
            pPlayer->setFundsModifier(fundsModifier);
            pPlayer->setTeam(team);
            QStringList buildList;
            qint32 size = 0;
            stream >> size;
            for (qint32 i2 = 0; i2 < size; i2++)
            {
                QString item;
                stream >> item;
                buildList.append(item);
            }
            pPlayer->setBuildList(buildList);
            updatePlayerData(i);
        }
    }
}

void PlayerSelection::recievedCOData(quint64, QDataStream& stream)
{    
    qint32 playerIdx;
    QString coid;
    stream >> playerIdx;
    stream >> coid;
    QStringList perks = Filesupport::readVectorList<QString, QList>(stream);
    if (!m_saveGame)
    {
        m_pMap->getPlayer(playerIdx)->setCO(coid, 0);
        CO* pCO = m_pMap->getPlayer(playerIdx)->getCO(0);
        if (pCO != nullptr)
        {
            pCO->setPerkList(perks);
            pCO->readCoStyleFromStream(stream);
        }
    }
    updateCO1Sprite(coid, playerIdx);
    stream >> coid;
    perks = Filesupport::readVectorList<QString, QList>(stream);
    if (!m_saveGame)
    {
        m_pMap->getPlayer(playerIdx)->setCO(coid, 1);
        CO* pCO = m_pMap->getPlayer(playerIdx)->getCO(1);
        if (pCO != nullptr)
        {
            pCO->setPerkList(perks);
            pCO->readCoStyleFromStream(stream);
        }
    }
    updateCO2Sprite(coid, playerIdx);
}

void PlayerSelection::recievedColorData(quint64, QDataStream& stream)
{    
    qint32 playerIdx = 0;
    QColor displayColor;
    stream >> playerIdx;
    stream >> displayColor;
    Player* pPlayer = m_pMap->getPlayer(playerIdx);
    QColor tableColor = displayColorToTableColor(displayColor);
    pPlayer->setColor(tableColor);
    DropDownmenuColor* pDropDownmenuColor = getCastedObject<DropDownmenuColor>(OBJECT_COLOR_PREFIX + QString::number(playerIdx));
    if (pDropDownmenuColor != nullptr)
    {
        pDropDownmenuColor->setCurrentItem(displayColor);
    }
}

void PlayerSelection::recievePlayerArmy(quint64, QDataStream& stream)
{    
    qint32 playerIdx = 0;
    QString army;
    stream >> playerIdx;
    stream >> army;
    Player* pPlayer = m_pMap->getPlayer(playerIdx);
    if (army == CO_ARMY)
    {
        pPlayer->setPlayerArmySelected(false);
    }
    else
    {
        pPlayer->setPlayerArmy(army);
        pPlayer->setPlayerArmySelected(true);
    }
}

void PlayerSelection::disconnected(quint64 socketID)
{
    if (m_pNetworkInterface.get() != nullptr &&
        m_pNetworkInterface->getIsServer())
    {
        if (m_pMap != nullptr)
        {
            CONSOLE_PRINT("Reopening players for socket " + QString::number(socketID) + " after disconnecting", GameConsole::eLogLevels::eDEBUG);
            // handle disconnect of clients here
            for (qint32 i = 0; i < m_playerSockets.size(); i++)
            {
                // this player has disconnect reopen him
                if (m_playerSockets[i] == socketID &&
                    m_pMap->getPlayer(i)->getControlType() != GameEnums::AiTypes_Open)
                {
                    if (m_lockedInCaseOfDisconnect[i])
                    {
                        m_pMap->getPlayer(i)->setSocketId(0);
                    }
                    else
                    {
                        // reopen all players
                        m_pMap->getPlayer(i)->setControlType(GameEnums::AiTypes_Open);
                        m_pMap->getPlayer(i)->setPlayerNameId("");
                        DropDownmenu* pDropDownmenu = getCastedObject<DropDownmenu>(OBJECT_AI_PREFIX + QString::number(i));
                        if (pDropDownmenu != nullptr)
                        {
                            pDropDownmenu->setCurrentItem(pDropDownmenu->getItemCount() - 1);
                        }
                        selectAI(i);
                    }
                }
            }
            CONSOLE_PRINT("Removing socket " + QString::number(socketID) + " from observer list", GameConsole::eLogLevels::eDEBUG);

            auto* gameRules = m_pMap->getGameRules();
            auto & observer = gameRules->getObserverList();
            observer.removeAll(socketID);
        }
        sendOpenPlayerCount();
    }
}

void PlayerSelection::updatePlayerData(qint32 player)
{    
    if (player < 0)
    {
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            updatePlayerData(i);
        }
    }
    else
    {
        Player* pPlayer = m_pMap->getPlayer(player);
        CO* pCO = pPlayer->getCO(0);
        QString coid = "no_co";
        if (pCO != nullptr)
        {
            coid = pCO->getCoID();
        }
        updateCO1Sprite(coid, player);
        pCO = pPlayer->getCO(1);
        coid = "no_co";
        if (pCO != nullptr)
        {
            coid = pCO->getCoID();
        }
        updateCO2Sprite(coid, player);

        DropDownmenuColor* pDropDownmenuColor = getCastedObject<DropDownmenuColor>(OBJECT_COLOR_PREFIX + QString::number(player));
        DropDownmenu* pDropDownmenuTeam = getCastedObject<DropDownmenu>(OBJECT_TEAM_PREFIX + QString::number(player));
        DropDownmenu* pDropDownmenuAI = getCastedObject<DropDownmenu>(OBJECT_AI_PREFIX + QString::number(player));
        SpriteObject* pCo1 = getCastedObject<SpriteObject>(OBJECT_CO1_PREFIX + QString::number(player));
        SpriteObject* pCo2 = getCastedObject<SpriteObject>(OBJECT_CO2_PREFIX + QString::number(player));
        Checkbox* pReadyBox = getCastedObject<Checkbox>(OBJECT_READY_PREFIX + QString::number(player));
        bool singleCo = m_pMap->getGameRules()->getSingleCo();
        if (pDropDownmenuColor != nullptr)
        {
            QColor itemColor = tableColorToDisplayColor(pPlayer->getColor());
            pDropDownmenuColor->setCurrentItem(itemColor);
        }
        if (pDropDownmenuTeam != nullptr)
        {
            pDropDownmenuTeam->setCurrentItem(pPlayer->getTeam());
        }
        bool notServerChangeAblePlayer = isNotServerChangeable(pPlayer);
        // check for open player
        if (m_pNetworkInterface->getIsObserver())
        {
            if (pDropDownmenuAI != nullptr)
            {
                pDropDownmenuAI->setEnabled(false);
            }
            if (pDropDownmenuColor != nullptr)
            {
                pDropDownmenuColor->setEnabled(false);
            }
            if (pCo1 != nullptr)
            {
                pCo1->setEnabled(false);
            }
            if (pCo2 != nullptr)
            {
                pCo2->setEnabled(false);
            }
        }
        else if (pPlayer->getBaseGameInput() == nullptr ||
                 pPlayer->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human ||
                 notServerChangeAblePlayer)
        {
            if (((pPlayer->getBaseGameInput() != nullptr &&
                  pPlayer->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human) ||
                 notServerChangeAblePlayer) &&
                !m_saveGame)
            {
                if (pDropDownmenuAI != nullptr)
                {
                    pDropDownmenuAI->setEnabled(true);
                }
                if (pDropDownmenuColor != nullptr)
                {
                    pDropDownmenuColor->setEnabled(true);
                }
                if (pCo1 != nullptr)
                {
                    pCo1->setEnabled(true);
                }
                if (pCo2 != nullptr)
                {
                    pCo2->setEnabled(!singleCo);
                }
            }
            else
            {
                if (m_pNetworkInterface->getIsServer() || m_isServerGame ||
                    isOpenPlayer(player))
                {
                    if (pDropDownmenuAI != nullptr)
                    {
                        pDropDownmenuAI->setEnabled(true);
                    }
                }
                else
                {
                    if (pDropDownmenuAI != nullptr)
                    {
                        pDropDownmenuAI->setEnabled(false);
                    }
                }
                if (pDropDownmenuColor != nullptr)
                {
                    pDropDownmenuColor->setEnabled(false);
                }
                if (pCo1 != nullptr)
                {
                    pCo1->setEnabled(false);
                }
                if (pCo2 != nullptr)
                {
                    pCo2->setEnabled(false);
                }
            }
            if (pReadyBox != nullptr)
            {
                if (pPlayer->getBaseGameInput() != nullptr &&
                    pPlayer->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human)
                {
                    pReadyBox->setChecked(m_playerReady);
                }
                else
                {
                    pReadyBox->setChecked(false);
                }
            }
        }
        else
        {
            if ((m_pNetworkInterface->getIsServer() || m_isServerGame) &&
                !m_saveGame)
            {
                if (pDropDownmenuAI != nullptr)
                {
                    pDropDownmenuAI->setEnabled(true);
                    if (pDropDownmenuAI->getCurrentItem() >= 0)
                    {
                        if (pDropDownmenuColor != nullptr)
                        {
                            pDropDownmenuColor->setEnabled(true);
                        }
                        if (pCo1 != nullptr)
                        {
                            pCo1->setEnabled(true);
                        }
                        if (pCo2 != nullptr)
                        {
                            pCo2->setEnabled(!singleCo);
                        }
                    }
                    else
                    {
                        if (pDropDownmenuColor != nullptr)
                        {
                            pDropDownmenuColor->setEnabled(false);
                        }
                        if (pCo1 != nullptr)
                        {
                            pCo1->setEnabled(false);
                        }
                        if (pCo2 != nullptr)
                        {
                            pCo2->setEnabled(false);
                        }
                    }
                }
            }
            else
            {
                if (pDropDownmenuAI != nullptr)
                {
                    pDropDownmenuAI->setEnabled(false);
                }
                if (pDropDownmenuColor != nullptr)
                {
                    pDropDownmenuColor->setEnabled(false);
                }
                if (pCo1 != nullptr)
                {
                    pCo1->setEnabled(false);
                }
                if (pCo2 != nullptr)
                {
                    pCo2->setEnabled(false);
                }
            }
        }
        if (getIsCampaign())
        {
            auto* input = pPlayer->getBaseGameInput();
            if (input != nullptr &&
                input->getAiType() != GameEnums::AiTypes_Human)
            {
                if (pDropDownmenuAI != nullptr)
                {
                    pDropDownmenuAI->setEnabled(false);
                }
                if (pCo1 != nullptr)
                {
                    pCo1->setEnabled(false);
                }
                if (pCo2 != nullptr)
                {
                    pCo2->setEnabled(false);
                }
            }
            if (pDropDownmenuColor != nullptr)
            {
                pDropDownmenuColor->setEnabled(false);
            }
        }
        if (m_lockedAiControl[player])
        {
            pDropDownmenuAI->setEnabled(false);
        }
    }
}

bool PlayerSelection::getReady(qint32 playerIdx)
{
    if (playerIdx >= 0 && playerIdx < m_playerReadyFlags.size())
    {
        return m_playerReadyFlags[playerIdx];
    }
    return false;
}

void PlayerSelection::setPlayerReady(bool value)
{
    m_playerReady = value;
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        if (m_pMap->getPlayer(i)->getControlType() >= GameEnums::AiTypes_Human &&
            m_pMap->getPlayer(i)->getControlType() != GameEnums::AiTypes_Open)
        {
            m_playerReadyFlags[i] = value;
            Checkbox* pCheckbox = getCastedObject<Checkbox>(OBJECT_READY_PREFIX + QString::number(i));
            if (pCheckbox != nullptr)
            {
                pCheckbox->setChecked(value);
            }
        }
    }
}

bool PlayerSelection::getPlayerReady()
{
    return m_playerReady;
}

void PlayerSelection::changeAllTeams(qint32 value)
{
    if (value > 0)
    {
        qint32 playersPerTeam = m_pMap->getPlayerCount() / value;
        qint32 freePlayer = m_pMap->getPlayerCount() - value * playersPerTeam;
        qint32 upCountPlayers = playersPerTeam / 2;
        qint32 player = 0;
        for (qint32 team = 0; team < value; ++team)
        {
            for (qint32 i = 0; i < playersPerTeam - upCountPlayers; ++i)
            {
                DropDownmenu* pDropDownmenu = getCastedObject<DropDownmenu>(OBJECT_TEAM_PREFIX + QString::number(player));
                if (pDropDownmenu != nullptr)
                {
                    pDropDownmenu->setCurrentItem(team);
                }
                playerTeamChanged(team, player);
                ++player;
            }
        }
        for (qint32 team = value - 1; team >= 0; --team)
        {
            for (qint32 i = 0; i < upCountPlayers; ++i)
            {
                DropDownmenu* pDropDownmenu = getCastedObject<DropDownmenu>(OBJECT_TEAM_PREFIX + QString::number(player));
                if (pDropDownmenu != nullptr)
                {
                    pDropDownmenu->setCurrentItem(team);
                }
                playerTeamChanged(team, player);
                ++player;
            }
            if (freePlayer > 0)
            {
                --freePlayer;
                DropDownmenu* pDropDownmenu = getCastedObject<DropDownmenu>(OBJECT_TEAM_PREFIX + QString::number(player));
                if (pDropDownmenu != nullptr)
                {
                    pDropDownmenu->setCurrentItem(team);
                }
                playerTeamChanged(team, player);
                ++player;
            }
        }
    }
}

void PlayerSelection::serializeObject(QDataStream& stream) const
{
    CONSOLE_PRINT("PlayerSelection::serializeObject", GameConsole::eDEBUG);
    stream << getVersion();
    stream << static_cast<qint32>(m_playerReadyFlags.size());
    for (auto & item : m_playerReadyFlags)
    {
        stream << item;
    }
    for (auto & item : m_lockedInCaseOfDisconnect)
    {
        stream << item;
    }
    for (auto & item : m_lockedAiControl)
    {
        stream << item;
    }
    if (m_pCampaign.get() != nullptr)
    {
        stream << true;
        m_pCampaign->serializeObject(stream);
    }
    else
    {
        stream << false;
    }
    stream << m_saveGame;
    stream << m_playerReady;
    stream << m_isServerGame;
}

void PlayerSelection::deserializeObject(QDataStream& stream)
{
    CONSOLE_PRINT("PlayerSelection::deserializeObject", GameConsole::eDEBUG);
    qint32 version = 0;
    stream >> version;
    qint32 size = 0;
    stream >> size;
    m_playerReadyFlags.clear();
    for (qint32 i = 0; i < size; ++i)
    {
        bool value = false;
        stream >> value;
        m_playerReadyFlags.append(value);
    }
    m_lockedInCaseOfDisconnect.clear();
    for (qint32 i = 0; i < size; ++i)
    {
        bool value = false;
        stream >> value;
        m_lockedInCaseOfDisconnect.append(value);
    }
    m_lockedAiControl.clear();
    for (qint32 i = 0; i < size; ++i)
    {
        bool value = false;
        stream >> value;
        m_lockedAiControl.append(value);
    }
    bool hasCampaign = false;
    stream >> hasCampaign;
    if (hasCampaign)
    {
        m_pCampaign = spCampaign::create();
        m_pCampaign->deserializeObject(stream);
    }
    stream >> m_saveGame;
    stream >> m_playerReady;
    stream >> m_isServerGame;
}

