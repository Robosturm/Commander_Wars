#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QRegularExpression>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "multiplayer/lobbymenu.h"
#include "multiplayer/networkcommands.h"
#include "multiplayer/dialogpassword.h"
#include "multiplayer/dialogpasswordandadress.h"
#include "multiplayer/multiplayermenu.h"
#include "multiplayer/networkgamedataview.h"
#include "multiplayer/dialogotherlobbyinfo.h"

#include "coreengine/mainapp.h"
#include "coreengine/gameconsole.h"
#include "coreengine/settings.h"
#include "coreengine/audiomanager.h"

#include "menue/mainwindow.h"

#include "game/gamemap.h"

#include "resource_management/fontmanager.h"

#include "objects/minimap.h"
#include "objects/base/chat.h"
#include "objects/dialogs/dialogmessagebox.h"
#include "objects/dialogs/customdialog.h"
#include "objects/tableView/stringtableitem.h"
#include "objects/tableView/xofytableitem.h"
#include "objects/tableView/locktableitem.h"

#include "network/mainserver.h"
#include "network/JsonKeys.h"

#include "ui_reader/uifactory.h"

namespace
{
const char* const GAMES_VIEW_ID = "GamesView";
const char* const MATCH_VIEW_INFO_ID = "MatchViewInfo";
const char* const CHAT_ITEM_PREFIX = "CHAT:";
const QStringList SERVER_CONTROL_IDS = {
    "ObserveGameButton",
    "JoinGameButton",
    "ServerHostButton",
    "OpenGamesButton",
    "OwnGamesButton",
    "ObserveGamesButton",
    "RefreshGamesButton",
    "EndPageButton",
    "NextPageButton",
    "PreviousPageButton",
    "StartPageButton",
    "OtherButton",
};
}

LobbyMenu::LobbyMenu()
{
    Mainapp *pApp = Mainapp::getInstance();
    CONSOLE_PRINT("Entering Lobby Menu", GameConsole::eDEBUG);
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("LobbyMenu");
#endif

    if (!Settings::getInstance()->getServer())
    {
        m_pTCPClient = MemoryManagement::create<TCPClient>(nullptr);        
        m_pTCPClient->moveToThread(Mainapp::getInstance()->getNetworkThread());
        connect(m_pTCPClient.get(), &TCPClient::recieveData, this, &LobbyMenu::recieveData, NetworkCommands::UNIQUE_DATA_CONNECTION);
        connect(m_pTCPClient.get(), &TCPClient::sigConnected, this, &LobbyMenu::connected, Qt::QueuedConnection);
        connect(m_pTCPClient.get(), &TCPClient::sigDisconnected, this, &LobbyMenu::disconnected, Qt::QueuedConnection);
        emit m_pTCPClient->sig_connect(Settings::getInstance()->getServerAdress(), Settings::getInstance()->getServerPort(), Settings::getInstance()->getSecondaryServerAdress());
    }

    changeBackground("lobbymenu");

    pApp->getAudioManager()->clearPlayList();
    pApp->getAudioManager()->loadFolder("resources/music/multiplayer");
    pApp->getAudioManager()->playRandom();

    const QString uiFile = Settings::getInstance()->getSmallScreenDevice()
                               ? "ui/multiplayer/lobbymenuSmall.xml"
                               : "ui/multiplayer/lobbymenu.xml";
    UiFactory::getInstance().createUi(uiFile, this);
    m_gamesview = getCastedObject<ComplexTableView>(GAMES_VIEW_ID);
    m_matchViewInfo = getCastedObject<Label>(MATCH_VIEW_INFO_ID);

    connect(this, &LobbyMenu::sigRequestUpdateGames, this, &LobbyMenu::requestUpdateGames, Qt::QueuedConnection);
    connect(this, &LobbyMenu::sigUpdateGamesView, this, &LobbyMenu::updateGamesView, Qt::QueuedConnection);
}

oxygine::spActor LobbyMenu::loadCustomId(const QString & item, qint32 x, qint32 y, bool enabled, bool visible, float scale,
                                         const QString & id, const QString & tooltip, const QString & onEvent,
                                         UiFactory* pFactory, CreatedGui* pMenu, qint32 loopIdx, qint32 & scaledWidth, qint32 & scaledHeight)
{
    if (!item.startsWith(CHAT_ITEM_PREFIX))
    {
        return oxygine::spActor();
    }

    const QStringList dimensions = item.mid(QString(CHAT_ITEM_PREFIX).size()).split(":");
    if (dimensions.size() != 2)
    {
        CONSOLE_PRINT("Invalid lobby chat dimensions: " + item, GameConsole::eERROR);
        return oxygine::spActor();
    }

    const QSize size(dimensions[0].toInt(), dimensions[1].toInt());
    spNetworkInterface pInterface = m_pTCPClient;
    if (Settings::getInstance()->getServer())
    {
        pInterface = MainServer::getInstance()->getGameServer();
    }
    spChat pChat = MemoryManagement::create<Chat>(pInterface, size, NetworkInterface::NetworkSerives::LobbyChat, nullptr);
    pChat->setObjectName(id);
    pChat->setPosition(x, y);
    pChat->setEnabled(enabled);
    pChat->setVisible(visible);
    pChat->setScale(scale);
    scaledWidth = pChat->getScaledWidth();
    scaledHeight = pChat->getScaledHeight();
    return pChat;
}

void LobbyMenu::setGameViewMode(qint32 mode)
{
    if (mode >= static_cast<qint32>(GameEnums::GameViewMode_OpenGames) &&
        mode <= static_cast<qint32>(GameEnums::GameViewMode_ObserveGames))
    {
        m_mode = static_cast<GameEnums::GameViewMode>(mode);
        emit sigRequestUpdateGames();
    }
}

void LobbyMenu::requestUpdateGames()
{
    switch (m_mode)
    {
    case GameEnums::GameViewMode_OpenGames:
    {
        requestServerGames();
        break;
    }
    case GameEnums::GameViewMode_OwnGames:
    {
        requestUserUpdateGames();
        break;
    }
    case GameEnums::GameViewMode_ObserveGames:
    {
        requestObserverUpdateGames();
        break;
    }
    }
}

void LobbyMenu::requestServerGames()
{
    if (m_pTCPClient.get() != nullptr)
    {
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, NetworkCommands::SERVERREQUESTGAMES);
        data.insert(JsonKeys::JSONKEY_MATCHSTARTINDEX, m_gameIndex);
        data.insert(JsonKeys::JSONKEY_MATCHCOUNT, REQUEST_COUNT);
        QJsonDocument doc(data);
        emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

void LobbyMenu::requestUserUpdateGames()
{
    if (m_pTCPClient.get() != nullptr)
    {
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, NetworkCommands::SERVERREQUESTUSERGAMES);
        data.insert(JsonKeys::JSONKEY_MATCHSTARTINDEX, m_gameIndex);
        data.insert(JsonKeys::JSONKEY_MATCHCOUNT, REQUEST_COUNT);
        data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getInstance()->getUsername());
        QJsonDocument doc(data);
        emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

void LobbyMenu::requestObserverUpdateGames()
{
    if (m_pTCPClient.get() != nullptr)
    {
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, NetworkCommands::SERVERREQUESTOBSERVEGAMES);
        data.insert(JsonKeys::JSONKEY_MATCHSTARTINDEX, m_gameIndex);
        data.insert(JsonKeys::JSONKEY_MATCHCOUNT, REQUEST_COUNT);
        data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getInstance()->getUsername());
        QJsonDocument doc(data);
        emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

void LobbyMenu::enableServerButtons(bool enable)
{
    for (const auto & id : SERVER_CONTROL_IDS)
    {
        setObjectEnabled(id, enable);
    }
}

void LobbyMenu::leaveServer()
{
    m_pTCPClient.reset();
    enableServerButtons(false);
}

void LobbyMenu::exitMenue()
{
    CONSOLE_PRINT("Leaving Lobby Menue", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    auto window = MemoryManagement::create<Mainwindow>("ui/menu/mainmenu.xml");
    oxygine::Stage::getStage()->addChild(window);
    detachAndRemove();
}

void LobbyMenu::hostLocal()
{
    CONSOLE_PRINT("Leaving Lobby Menue", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    oxygine::Stage::getStage()->addChild(MemoryManagement::create<Multiplayermenu>("", "", Settings::getInstance()->getGamePort(), "", Multiplayermenu::NetworkMode::Host));
    detachAndRemove();
}

void LobbyMenu::hostServer()
{
    if (m_pTCPClient.get() != nullptr &&
        m_pTCPClient->getIsConnected())
    {
        CONSOLE_PRINT("Leaving Lobby Menue", GameConsole::eDEBUG);
        m_onEnterTimer.stop();
        auto pMenu = MemoryManagement::create<Multiplayermenu>(m_pTCPClient, "", Multiplayermenu::NetworkMode::Host);
        pMenu->setSameVersionAsServer(m_sameVersionAsServer);
        oxygine::Stage::getStage()->addChild(pMenu);
        detachAndRemove();
    }
}

void LobbyMenu::joinGame()
{
    if (!checkGameVersion(m_currentGame))
    {
        return;
    }
    if (m_currentGame.getUuid() != 0)
    {
        if ((m_mode == GameEnums::GameViewMode_OwnGames || m_currentGame.hasOpenPlayers()))
        {
            if (m_currentGame.getLocked())
            {
                spDialogPassword pDialogTextInput = MemoryManagement::create<DialogPassword>(tr("Enter Password"), true, "");
                addChild(pDialogTextInput);
                connect(pDialogTextInput.get(), &DialogPassword::sigTextChanged, this, &LobbyMenu::joinGamePassword, Qt::QueuedConnection);
            }
            else
            {
                joinGamePassword("");
            }
        }
        else
        {
            CONSOLE_PRINT("Joining game " + m_currentGame.getSlaveName() + " not possible.", GameConsole::eDEBUG);
        }
    }
    else
    {
        CONSOLE_PRINT("Server error no valid uuid found for game " + m_currentGame.getSlaveName(), GameConsole::eDEBUG);
    }
}

void LobbyMenu::joinGamePassword(QString password)
{
    if (!checkGameVersion(m_currentGame))
    {
        return;
    }

    bool exists = false;
    if (m_currentGame.getUuid() != 0)
    {
        for (const auto &game : std::as_const(m_games))
        {
            if (m_currentGame.getUuid() == game.getUuid())
            {
                exists = true;
                break;
            }
        }
    }
    if (exists)
    {
        QString command = QString(NetworkCommands::SERVERJOINGAME);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        showContactingServer();
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        data.insert(JsonKeys::JSONKEY_SLAVENAME, m_currentGame.getSlaveName());
        QJsonDocument doc(data);
        emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
        m_password = password;
    }
}

bool LobbyMenu::checkGameVersion(NetworkGameData & game)
{
    bool ret = true;
    if (game.getGameVersion() != GameVersion())
    {
        spDialogMessageBox pDialogMessageBox;
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Game has a different version. Game version: ") + game.getGameVersion().toString());
        addChild(pDialogMessageBox);
        ret = false;
    }
    return ret;
}

void LobbyMenu::showContactingServer()
{
    spDialogConnecting pDialogConnecting = MemoryManagement::create<DialogConnecting>(tr("Contacting server"), 1000 * 60 * 5);
    addChild(pDialogConnecting);
    connect(pDialogConnecting.get(), &DialogConnecting::sigCancel, this, &LobbyMenu::cancelWaitingForServer, Qt::QueuedConnection);
    connect(this, &LobbyMenu::sigServerResponded, pDialogConnecting.get(), &DialogConnecting::connected, Qt::QueuedConnection);
}

void LobbyMenu::cancelWaitingForServer()
{
    CONSOLE_PRINT("Leaving Lobby Menue", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    spLobbyMenu newMenu = MemoryManagement::create<LobbyMenu>();
    oxygine::Stage::getStage()->addChild(newMenu);
    detachAndRemove();
}

void LobbyMenu::joinAdress()
{
    spDialogPasswordAndAdress pDialogTextInput = MemoryManagement::create<DialogPasswordAndAdress>(tr("Enter Host address"));
    addChild(pDialogTextInput);
    connect(pDialogTextInput.get(), &DialogPasswordAndAdress::sigTextChanged, this, &LobbyMenu::join, Qt::QueuedConnection);
}

void LobbyMenu::join(QString adress, QString password)
{
    CONSOLE_PRINT("Leaving Lobby Menue to join game by adress", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    oxygine::Stage::getStage()->addChild(MemoryManagement::create<Multiplayermenu>(adress.trimmed(), "", Settings::getInstance()->getGamePort(), password, Multiplayermenu::NetworkMode::Client));
    detachAndRemove();
}

void LobbyMenu::observeAdress()
{
    spDialogPasswordAndAdress pDialogTextInput = MemoryManagement::create<DialogPasswordAndAdress>(tr("Enter Host address"));
    addChild(pDialogTextInput);
    connect(pDialogTextInput.get(), &DialogPasswordAndAdress::sigTextChanged, this, &LobbyMenu::observe, Qt::QueuedConnection);
}

void LobbyMenu::observe(QString adress, QString password)
{
    CONSOLE_PRINT("Leaving Lobby Menue to observe game by adress", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    oxygine::Stage::getStage()->addChild(MemoryManagement::create<Multiplayermenu>(adress.trimmed(), "", Settings::getInstance()->getGamePort(), password, Multiplayermenu::NetworkMode::Observer));
    detachAndRemove();
}

void LobbyMenu::observeGame()
{
    if (!checkGameVersion(m_currentGame))
    {
        return;
    }
    if (m_currentGame.getUuid() != 0)
    {
        if (m_currentGame.getLocked())
        {
            spDialogPassword pDialogTextInput = MemoryManagement::create<DialogPassword>(tr("Enter Password"), true, "");
            addChild(pDialogTextInput);
            connect(pDialogTextInput.get(), &DialogPassword::sigTextChanged, this, &LobbyMenu::observeGamePassword, Qt::QueuedConnection);
        }
        else
        {
            observeGamePassword("");
        }
    }
}

void LobbyMenu::observeGamePassword(QString password)
{
    if (!checkGameVersion(m_currentGame))
    {
        return;
    }
    bool exists = false;
    if (m_currentGame.getUuid() != 0)
    {
        for (const auto &game : std::as_const(m_games))
        {
            if (m_currentGame.getUuid() == game.getUuid())
            {
                exists = true;
                break;
            }
        }
    }
    if (exists)
    {
        CONSOLE_PRINT("Leaving Lobby Menue to observe server game", GameConsole::eDEBUG);
        m_onEnterTimer.stop();
        m_pTCPClient->setIsObserver(true);
        oxygine::Stage::getStage()->addChild(MemoryManagement::create<Multiplayermenu>(m_pTCPClient, password, Multiplayermenu::NetworkMode::Observer));
        QString command = QString(NetworkCommands::SERVERJOINGAME);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        data.insert(JsonKeys::JSONKEY_SLAVENAME, m_currentGame.getSlaveName());
        QJsonDocument doc(data);
        emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
        detachAndRemove();
    }
}

void LobbyMenu::disconnected(quint64 socketID)
{
    if (m_loggedIn)
    {
        spDialogMessageBox pDialogMessageBox;
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Connection to server lost leaving server lobby."));
        addChild(pDialogMessageBox);
        connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &LobbyMenu::exitMenue, Qt::QueuedConnection);
    }
}

void LobbyMenu::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service, quint64 senderSocket)
{
    if (service == NetworkInterface::NetworkSerives::ServerHostingJson)
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject objData = doc.object();
        QString messageType = objData.value(JsonKeys::JSONKEY_COMMAND).toString();
        CONSOLE_PRINT("LobbyMenu Command received: " + messageType + " from " + QString::number(socketID), GameConsole::eDEBUG);
        if (messageType == NetworkCommands::SERVERVERSION)
        {
            checkVersionAndShowInfo(objData);
        }
        else if (messageType == NetworkCommands::SERVERGAMEDATA)
        {
            if (m_loggedIn &&
                (m_mode ==  GameEnums::GameViewMode_OpenGames || m_mode == GameEnums::GameViewMode_ObserveGames))
            {
                updateGameData(objData);
            }
        }
        else if (messageType == NetworkCommands::SERVERUSERGAMEDATA)
        {
            if (m_loggedIn && m_mode == GameEnums::GameViewMode_OwnGames)
            {
                updateGameData(objData);
            }
        }
        else if (messageType == NetworkCommands::SLAVEADDRESSINFO)
        {
            joinSlaveGame(objData);
        }
        else if (messageType == NetworkCommands::SERVERNOGAMESLOTSAVAILABLE)
        {
            spDialogMessageBox pDialogMessageBox;
            pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Failed to launch game on server cause no more game slots are available."));
            addChild(pDialogMessageBox);
        }
        else if (messageType == NetworkCommands::SERVERGAMENOLONGERAVAILABLE)
        {
            emit sigServerResponded();
            spDialogMessageBox pDialogMessageBox;
            pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Host game is no longer available or is currently relaunched."));
            addChild(pDialogMessageBox);
        }
        else if (messageType == NetworkCommands::SERVERUSERNAMES)
        {
            onSearchedPlayersReceived(objData);
        }
        else if (messageType == NetworkCommands::SERVERPLAYERSTATS)
        {
            onReceivedPlayerStats(objData);
        }
        else if (messageType == NetworkCommands::SERVERRESPONSCREATEACCOUNT)
        {
            handleAccountMessage(socketID, "CreateAccountDialog", objData);
        }
        else if (messageType == NetworkCommands::SERVERRESPONSLOGINACCOUNT)
        {
            handleAccountMessage(socketID, "UserLoginDialog", objData);
        }
        else if (messageType == NetworkCommands::SERVERRESPONSRESETPASSWORD)
        {
            handleAccountMessage(socketID, "ForgotPasswordDialog", objData);
        }
        else if (messageType == NetworkCommands::SERVERRESPONSCHANGEPASSWORD)
        {
            handleAccountMessage(socketID, "ChangePasswordDialog", objData);
        }
        else if (messageType == NetworkCommands::SERVERRESPONSDELETEACCOUNT)
        {
            handleAccountMessage(socketID, "DeleteAccountDialog", objData);
        }
        else if (messageType == NetworkCommands::SERVERRESPONSSETUP2FA)
        {
            handle2faMessage(socketID, "Setup2faDialog", objData);
        }
        else if (messageType == NetworkCommands::SERVERRESPONSCONFIRM2FA)
        {
            handle2faMessage(socketID, "Setup2faDialog", objData);
        }
        else if (messageType == NetworkCommands::SERVERRESPONSRESETPASSWORD2FA)
        {
            handle2faMessage(socketID, "ForgotPasswordDialog", objData);
        }
        else if (messageType == NetworkCommands::SERVERSENDAUTOMATCHINFO)
        {
            receivedShowAutoMatches(objData);
        }
        else if (messageType == NetworkCommands::MAPUPLOADRESPONSE)
        {
            onMapUploadResponse(objData);
        }
        else if (messageType == NetworkCommands::RECEIVEAVAILABLEMAPS)
        {
            emit sigReceivedAvailableMaps(objData);
        }
        else if (messageType == NetworkCommands::RECEIVEAVAILABLERECORDS)
        {
            emit sigReceivedAvailableRecords(objData);
        }
        else if (messageType == NetworkCommands::FILEDOWNLOAD)
        {
            onDownloadResponse(objData);
        }
        else if (messageType == NetworkCommands::SERVERDELETERESPONSE)
        {
            onMapDeleteResponse(objData);
        }
        else if (messageType == NetworkCommands::FILEPACKET)
        {
            emit sigReceivedFilePacket(objData);
        }
        else
        {
            CONSOLE_PRINT("Unknown command in LobbyMenu::recieveData " + messageType + " received", GameConsole::eDEBUG);
        }
    }
    else
    {
        CONSOLE_PRINT("Unknown service in LobbyMenu::recieveData " + QString::number(static_cast<qint32>(service)) + " received", GameConsole::eDEBUG);
    }
}

void LobbyMenu::requestPlayersFromServer(const QString &searchFilter)
{
    QString command = QString(NetworkCommands::SERVERREQUESTUSERNAMES);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_FILTER, searchFilter);
    QJsonDocument doc(data);
    emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void LobbyMenu::requestPlayerStats(const QString &player)
{
    QString command = QString(NetworkCommands::SERVERREQUESTPLAYERSTATS);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_USERNAME, player);
    QJsonDocument doc(data);
    emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void LobbyMenu::onReceivedPlayerStats(const QJsonObject &objData)
{
    emit sigReceivedPlayerStats(objData);
}

void LobbyMenu::onSearchedPlayersReceived(const QJsonObject &objData)
{
    QStringList players;
    QJsonArray usernames = objData.value(JsonKeys::JSONKEY_USERNAMES).toArray();
    for (const auto &username : std::as_const(usernames))
    {
        QString user = username.toString();
        players.append(user);
        CONSOLE_PRINT("user: " + user, GameConsole::eDEBUG);
    }
    emit sigSearchedPlayersReceived(players);
}

void LobbyMenu::updateGameData(const QJsonObject &objData)
{
    m_games.clear();
    QJsonObject games = objData.value(JsonKeys::JSONKEY_GAMES).toObject();
    m_serverCurrentMatchCount = objData.value(JsonKeys::JSONKEY_MATCHCOUNT).toInt();
    for (const auto &game : std::as_const(games))
    {
        NetworkGameData gameData;
        gameData.fromJson(game.toObject());
        m_games.append(gameData);
    }
    if (m_matchViewInfo != nullptr)
    {
        m_matchViewInfo->setText(QString::number(m_gameIndex) + " / " + QString::number(m_gameIndex + games.size()));
    }
    emit sigUpdateGamesView();
}

void LobbyMenu::joinSlaveGame(const QJsonObject &objData)
{
    emit sigServerResponded();
    QString slaveAddress = objData.value(JsonKeys::JSONKEY_ADDRESS).toString();
    QString secondarySlaveAddress = objData.value(JsonKeys::JSONKEY_SECONDARYADDRESS).toString();
    quint16 slavePort = objData.value(JsonKeys::JSONKEY_PORT).toInteger();
    CONSOLE_PRINT("Leaving Lobby Menue to join game by adress", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    oxygine::Stage::getStage()->addChild(MemoryManagement::create<Multiplayermenu>(slaveAddress.trimmed(), secondarySlaveAddress.trimmed(), slavePort, m_password, Multiplayermenu::NetworkMode::Client));
    detachAndRemove();
}

void LobbyMenu::updateGamesView()
{
    if (m_gamesview == nullptr)
    {
        return;
    }
    const auto &widths = m_gamesview->getWidths();
    ComplexTableView::Items items;
    qint64 uuid = m_currentGame.getUuid();
    bool hasGame = (uuid != 0);
    qint32 itemCount = 0;
    qint32 currentItem = -1;
    for (auto &game : m_games)
    {
        if (hasGame && game.getUuid() == uuid)
        {
            currentItem = itemCount;
            m_currentGame = game;
        }
        QColor textColor = FontManager::getFontColor();
        if (game.getRunningGame())
        {
            if (Settings::getInstance()->getUsername() == game.getCurrentPlayer() ||
                game.getCurrentPlayer().isEmpty())
            {
                textColor = QColor(0, 255, 0);
            }
        }
        ComplexTableView::Item item;
        item.pData = &game;
        item.items.append(std::static_pointer_cast<BaseTableItem>(MemoryManagement::create<StringTableItem>(game.getMapName(), widths[0], textColor)));
        item.items.append(std::static_pointer_cast<BaseTableItem>(MemoryManagement::create<XofYTableItem>(game.getPlayers(), game.getMaxPlayers(), widths[1], textColor)));
        item.items.append(std::static_pointer_cast<BaseTableItem>(MemoryManagement::create<StringTableItem>(game.getDescription(), widths[2], textColor)));
        QStringList mods = game.getMods();
        QString modString;
        for (const auto &mod : mods)
        {
            modString.append(Settings::getInstance()->getModName(mod) + "; ");
        }
        item.items.append(std::static_pointer_cast<BaseTableItem>(MemoryManagement::create<StringTableItem>(modString, widths[3], textColor)));
        item.items.append(std::static_pointer_cast<BaseTableItem>(MemoryManagement::create<LockTableItem>(game.getLocked(), widths[4])));
        items.append(item);
        ++itemCount;
    }
    m_gamesview->setItems(items);
    if (hasGame && currentItem >= 0)
    {
        m_gamesview->setCurrentItem(currentItem);
    }
    else
    {
        m_currentGame = NetworkGameData();
    }
}

void LobbyMenu::selectGame()
{
    if (m_gamesview != nullptr && m_gamesview->getCurrentItem() >= 0)
    {
        m_currentGame = *m_gamesview->getDataItem<NetworkGameData>(m_gamesview->getCurrentItem());
        if (m_lastSelectedItem == m_gamesview->getCurrentItem())
        {
            spNetworkGameDataView pView = MemoryManagement::create<NetworkGameDataView>(m_currentGame);
            addChild(pView);
        }
        m_lastSelectedItem = m_gamesview->getCurrentItem();
    }
}

void LobbyMenu::connected(quint64 socket)
{
    QString command = QString(NetworkCommands::SERVERREQUESTVERSION);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    QJsonDocument doc(data);
    emit m_pTCPClient->sig_sendData(socket, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void LobbyMenu::checkVersionAndShowInfo(const QJsonObject &objData)
{
    GameVersion gatewayVersion(0, 36, 0, "main");
    GameVersion version;
    version.setMajor(objData.value(JsonKeys::JSONKEY_VERSION_MAJOR).toInt());
    version.setMinor(objData.value(JsonKeys::JSONKEY_VERSION_MINOR).toInt());
    version.setRevision(objData.value(JsonKeys::JSONKEY_VERSION_REVISION).toInt());
    version.setSufix(objData.value(JsonKeys::JSONKEY_VERSION_SUFIX).toString());
    if (version == GameVersion())
    {
        m_sameVersionAsServer = true;
        requestPassword();
    }
    else if (version >= gatewayVersion && GameVersion() >= gatewayVersion)
    {
        m_sameVersionAsServer = false;
        spDialogMessageBox pDialogMessageBox;
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Server has a different version of the game. Server ") + version.toString() + " Some features are not accessible.");
        addChild(pDialogMessageBox);
        connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &LobbyMenu::requestPassword, Qt::QueuedConnection);
    }
    else
    {
        spDialogMessageBox pDialogMessageBox;
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Connection refused. Server has a different version of the game. Server ") + version.toString());
        addChild(pDialogMessageBox);
        m_pTCPClient.reset();
    }
}

void LobbyMenu::requestPassword()
{
    CONSOLE_PRINT("LobbyMenu::connected", GameConsole::eDEBUG);
    QString password = Settings::getInstance()->getServerPassword();
    spCustomDialog pDialog = MemoryManagement::create<CustomDialog>("userLogin", "ui/serverLogin/userLoginDialog.xml", this);
    addChild(pDialog);
    if (!password.isEmpty())
    {
        loginToServerAccount(password);
    }
}

void LobbyMenu::onLogin()
{
    enableServerButtons(true);
    m_loggedIn = true;
    requestServerGames();
}

void LobbyMenu::onLogout()
{
    enableServerButtons(false);
    m_loggedIn = false;
    m_games.clear();
    m_currentGame = NetworkGameData();
    if (m_gamesview != nullptr)
    {
        m_gamesview->setItems(ComplexTableView::Items());
    }
}

void LobbyMenu::onEnter()
{
    Interpreter *pInterpreter = Interpreter::getInstance();
    QString object = "Init";
    QString func = "lobbyMenu";
    if (pInterpreter->exists(object, func))
    {
        CONSOLE_PRINT("Executing:" + object + "." + func, GameConsole::eDEBUG);
        QJSValueList args({pInterpreter->newQObject(this)});
        pInterpreter->doFunction(object, func, args);
    }
}

void LobbyMenu::handleAccountMessage(quint64 socketID, const QString object, const QJsonObject &objData)
{
    auto accountError = objData.value(JsonKeys::JSONKEY_ACCOUNT_ERROR).toInt();
    if (!object.isEmpty())
    {
        CONSOLE_PRINT("Calling function " + object + ".onAccountMessage(" + QString::number(accountError) + ")", GameConsole::eDEBUG);
        Interpreter *pInterpreter = Interpreter::getInstance();
        QJSValueList args;
        args.append(accountError);
        args.append(objData.value(JsonKeys::JSONKEY_HAS2FA).toBool());
        pInterpreter->doFunction(object, "onAccountMessage", args);
    }
    else
    {
        CONSOLE_PRINT("Account message is empty.", GameConsole::eDEBUG);
    }
}

void LobbyMenu::handle2faMessage(quint64 socketID, const QString object, const QJsonObject &objData)
{
    auto accountError = objData.value(JsonKeys::JSONKEY_ACCOUNT_ERROR).toInt();
    if (!object.isEmpty())
    {
        CONSOLE_PRINT("Calling function " + object + ".on2faMessage(" + QString::number(accountError) + ")", GameConsole::eDEBUG);
        Interpreter *pInterpreter = Interpreter::getInstance();
        QJSValueList args;
        args.append(accountError);
        args.append(objData.value(JsonKeys::JSONKEY_TOTPSECRET).toString());
        args.append(objData.value(JsonKeys::JSONKEY_TOTPURL).toString());
        args.append(objData.value(JsonKeys::JSONKEY_NEWPASSWORD).toString());
        pInterpreter->doFunction(object, "on2faMessage", args);
    }
    else
    {
        CONSOLE_PRINT("2fa message is empty.", GameConsole::eDEBUG);
    }
}

bool LobbyMenu::getServerRequestNewPassword() const
{
    return m_serverRequestNewPassword;
}

void LobbyMenu::setServerRequestNewPassword(bool newServerRequestNewPassword)
{
    m_serverRequestNewPassword = newServerRequestNewPassword;
}

bool LobbyMenu::isValidEmailAdress(const QString emailAdress)
{
    static const QRegularExpression regex("(?:[a-zA-Z0-9!#$%&'*+\\/=?^_`{|}~-]+(?:\\.[a-zA-Z0-9!#$%&'*+\\/=?^_`{|}~-]+)*|\""
                                          "(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21\\x23-\\x5b\\x5d-\\x7f]"
                                          "|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])*\")@(?:(?:[a-zA-Z0-9]"
                                          "(?:[a-zA-Z0-9-]*[a-zA-Z0-9])?\\.)+[a-zA-Z0-9](?:[a-zA-Z0-9-]*[a-zA-Z0-9])?|"
                                          "\\[(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]"
                                          "|2[0-4][0-9]|[01]?[0-9][0-9]?|[a-zA-Z0-9-]*[a-zA-Z0-9]:(?:"
                                          "[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21-\\x5a\\x53-\\x7f]|\\\\"
                                          "[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])+)\\])");
    auto match = regex.match(emailAdress);
    return match.hasMatch() && match.capturedLength() == emailAdress.length();
}

bool LobbyMenu::isValidPassword(const QString password)
{
    static const QRegularExpression regex("^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[#?!@$%^&*-]).{8,}$");
    auto match = regex.match(password);
    return match.hasMatch();
}

void LobbyMenu::createServerAccount(const QString passwordString, const QString emailAdress)
{
    QString command = NetworkCommands::CREATEACCOUNT;
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    Password password;
    password.setPassword(passwordString);
    data.insert(JsonKeys::JSONKEY_PASSWORD, GlobalUtils::toJsonArray(password.getHash()));
    data.insert(JsonKeys::JSONKEY_EMAILADRESS, emailAdress);
    data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getInstance()->getUsername());
    QJsonDocument doc(data);
    emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::JsonFormat::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void LobbyMenu::deleteServerAccount(const QString passwordString, const QString emailAdress)
{
    QString command = NetworkCommands::DELETEACCOUNT;
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    Password password;
    password.setPassword(passwordString);
    data.insert(JsonKeys::JSONKEY_PASSWORD, GlobalUtils::toJsonArray(password.getHash()));
    data.insert(JsonKeys::JSONKEY_EMAILADRESS, emailAdress);
    data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getInstance()->getUsername());
    QJsonDocument doc(data);
    emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::JsonFormat::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void LobbyMenu::loginToServerAccount(const QString passwordString)
{
    QString command = NetworkCommands::LOGINACCOUNT;
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    Password password;
    password.setPassword(passwordString);
    data.insert(JsonKeys::JSONKEY_PASSWORD, GlobalUtils::toJsonArray(password.getHash()));
    data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getInstance()->getUsername());
    QJsonDocument doc(data);
    emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::JsonFormat::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void LobbyMenu::resetPasswordOnServerAccount(const QString emailAdress)
{
    QString command = NetworkCommands::RESETPASSWORD;
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_EMAILADRESS, emailAdress);
    data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getInstance()->getUsername());
    QJsonDocument doc(data);
    emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::JsonFormat::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void LobbyMenu::start2faResetPassword(const QString emailAdress)
{
    QString command = NetworkCommands::START2FACTORRESETPASSWORD;
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_EMAILADRESS, emailAdress);
    data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getInstance()->getUsername());
    QJsonDocument doc(data);
    emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::JsonFormat::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void LobbyMenu::changePasswordOnServerAccount(const QString oldServerPassword, const QString newServerPassword)
{
    QString command = NetworkCommands::CHANGEPASSWORD;
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    Password password;
    password.setPassword(newServerPassword);
    data.insert(JsonKeys::JSONKEY_PASSWORD, GlobalUtils::toJsonArray(password.getHash()));
    password.setPassword(oldServerPassword);
    data.insert(JsonKeys::JSONKEY_OLDPASSWORD, GlobalUtils::toJsonArray(password.getHash()));
    data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getInstance()->getUsername());
    QJsonDocument doc(data);
    emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::JsonFormat::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void LobbyMenu::requestServer2faSetup()
{
    QString command = NetworkCommands::SETUP2FA;
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getInstance()->getUsername());
    QJsonDocument doc(data);
    emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::JsonFormat::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void LobbyMenu::confirmServer2fa(const QString code)
{
    QString command = NetworkCommands::CONFIRM2FA;
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_TOTPCODE, code);
    data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getInstance()->getUsername());
    QJsonDocument doc(data);
    emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::JsonFormat::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void LobbyMenu::cancelServer2fa()
{
    QString command = NetworkCommands::CANCEL2FA;
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getInstance()->getUsername());
    QJsonDocument doc(data);
    emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::JsonFormat::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void LobbyMenu::submitPasswordResetCode(const QString code)
{
    QString command = NetworkCommands::SUBMITPASSWORDRESET2FACODE;
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_TOTPCODE, code);
    data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getInstance()->getUsername());
    QJsonDocument doc(data);
    emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::JsonFormat::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void LobbyMenu::showNextStep()
{
    m_gameIndex += REQUEST_COUNT;
    if (m_gameIndex >= m_serverCurrentMatchCount)
    {
        showEnd();
    }
    else
    {
        requestUserUpdateGames();
    }
}

void LobbyMenu::showPreviousStep()
{
    m_gameIndex -= REQUEST_COUNT;
    if (m_gameIndex < 0)
    {
        m_gameIndex = 0;
    }
    requestUserUpdateGames();
}

void LobbyMenu::showStart()
{
    m_gameIndex = 0;
    requestUserUpdateGames();
}

void LobbyMenu::showEnd()
{
    m_gameIndex = m_serverCurrentMatchCount - (m_serverCurrentMatchCount % REQUEST_COUNT);
    requestUserUpdateGames();
}

void LobbyMenu::requestShowAutoMatches()
{
    if (m_pTCPClient.get() != nullptr)
    {
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, NetworkCommands::SERVERREQUESTAUTOMATCHINFO);
        QJsonDocument doc(data);
        emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

void LobbyMenu::onShowOtherDialog()
{
    spDialogOtherLobbyInfo pDialog = MemoryManagement::create<DialogOtherLobbyInfo>(this);
    addChild(pDialog);
}

void LobbyMenu::receivedShowAutoMatches(const QJsonObject & objData)
{
    emit sigRequestShowAutoMatches(objData);
}

void LobbyMenu::uploadMap(const  QString & selectedFilePath)
{
    QString filePath = selectedFilePath;
    QString userPath = Settings::getInstance()->getUserPath();
    if (!userPath.isEmpty())
    {
        filePath = filePath.replace(userPath, "");
    }
    if (m_loggedIn && QFile::exists(filePath))
    {
        GameMap map(filePath, true, false, false);
        if (map.validMap())
        {
            Minimap minimap;
            minimap.updateMinimap(&map);
            QImage image;
            emit Mainapp::getInstance()->getRenderer().sigSaveMapAsImage(&minimap, &image);
            QByteArray imageArray;
            QBuffer buffer(&imageArray);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "PNG");
            buffer.close();
            QByteArray mapArray;            
            buffer.setBuffer(&mapArray);
            buffer.open(QIODevice::WriteOnly);
            QDataStream stream(&buffer);
            map.serializeObject(stream);
            QString command = NetworkCommands::MAPUPLOAD;
            CONSOLE_PRINT("Sending command " + command + " map data=" + QString::number(mapArray.size()) + " image data=" + QString::number(imageArray.size()), GameConsole::eDEBUG);
            QJsonObject data;
            data.insert(JsonKeys::JSONKEY_COMMAND, command);
            data.insert(JsonKeys::JSONKEY_MAPPATH, filePath);
            data.insert(JsonKeys::JSONKEY_MINIMAPDATA, GlobalUtils::toJsonArray(imageArray));
            data.insert(JsonKeys::JSONKEY_MAPDATA, GlobalUtils::toJsonArray(mapArray));
            data.insert(JsonKeys::JSONKEY_MAPNAME, map.getMapName());
            data.insert(JsonKeys::JSONKEY_MAPAUTHOR, map.getMapAuthor());
            data.insert(JsonKeys::JSONKEY_MAPPLAYERS, map.getPlayerCount());
            data.insert(JsonKeys::JSONKEY_MAPWIDTH, map.getMapWidth());
            data.insert(JsonKeys::JSONKEY_MAPHEIGHT, map.getMapHeight());
            data.insert(JsonKeys::JSONKEY_MAPFLAGS, static_cast<qint64>(map.getMapFlags()));
            data.insert(JsonKeys::JSONKEY_MAPUPLOADER, Settings::getInstance()->getUsername());
            QJsonDocument doc(data);
            emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::JsonFormat::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
        }
    }
}

void LobbyMenu::onMapUploadResponse(const QJsonObject & objData)
{
    if (objData.value(JsonKeys::JSONKEY_RESULT).toBool())
    {
        spDialogMessageBox pDialogMessageBox;
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Map upload was successful."));
        addChild(pDialogMessageBox);
    }
    else
    {
        spDialogMessageBox pDialogMessageBox;
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Map upload failed. Please rename the map file to a unique name."));
        addChild(pDialogMessageBox);
    }
}

NetworkInterface* LobbyMenu::getTcpClient() const
{
    return m_pTCPClient.get();
}

void LobbyMenu::onDownloadResponse(const QJsonObject & objData)
{
    bool success = objData.value(JsonKeys::JSONKEY_DOWNLOADRESULT).toBool();
    if (success)
    {
        QByteArray mapArray = GlobalUtils::toByteArray(objData.value(JsonKeys::JSONKEY_MAPDATA).toArray());
        QString filePath = objData.value(JsonKeys::JSONKEY_MAPPATH).toString();
        QFile::remove(filePath);
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly))
        {
            QDataStream stream(&file);
            qint32 contentSize = mapArray.size();
            stream.writeRawData(mapArray.constData(), contentSize);
            file.close();
        }
        else
        {
            CONSOLE_PRINT("Failed to open file " + file.fileName(), GameConsole::eERROR);
        }
    }
    emit sigOnDownloadedResponse(success);
}

void LobbyMenu::onMapDeleteResponse(const QJsonObject & objData)
{
    bool success = objData.value(JsonKeys::JSONKEY_RESULT).toBool();
    emit sigOnMapDeleteResponse(success);
}

void LobbyMenu::sendCommandToServer(const QJsonObject & objData)
{
    QJsonDocument doc(objData);
    emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::JsonFormat::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}
