#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QCryptographicHash>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "multiplayer/multiplayermenu.h"
#include "multiplayer/lobbymenu.h"
#include "multiplayer/networkcommands.h"

#include "coreengine/mainapp.h"
#include "coreengine/gameconsole.h"
#include "coreengine/settings.h"
#include "coreengine/filesupport.h"
#include "coreengine/globalutils.h"

#include "menue/gamemenue.h"
#include "menue/mainwindow.h"

#include "multiplayer/password.h"

#include "network/tcpclient.h"
#include "network/mainserver.h"
#include "network/JsonKeys.h"

#include "game/gamemap.h"
#include "game/gamerules.h"
#include "game/player.h"

#include "objects/dialogs/filedialog.h"
#include "objects/dialogs/dialogmessagebox.h"
#include "objects/base/moveinbutton.h"

#include "ingamescriptsupport/genericbox.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

Multiplayermenu::Multiplayermenu(const QString & address, const QString & secondaryAddress, quint16 port, const QString & password, NetworkMode networkMode)
    : MapSelectionMapsMenue(MemoryManagement::create<MapSelectionView>(QStringList({".map", ".jsm"})), Settings::getInstance()->getSmallScreenDevice() ? oxygine::Stage::getStage()->getHeight() - 80 : oxygine::Stage::getStage()->getHeight() - 230),
      m_networkMode(networkMode),
      m_local(true),
      m_password(password)
{
    init();
    if (m_networkMode != NetworkMode::Host)
    {
        initClientConnection(address, secondaryAddress, port);
    }
    else
    {
        m_pHostAdresse = ObjectManager::createButton("Show Addresses");
        addChild(m_pHostAdresse);
        m_pHostAdresse->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
        {
            emit sigShowIPs();
        });
        m_pHostAdresse->setPosition(m_pButtonStart->getX() - m_pHostAdresse->getScaledWidth() - 10,
                                    oxygine::Stage::getStage()->getHeight() - m_pHostAdresse->getScaledHeight() - 10);
        m_pHostAdresse->setVisible(false);
        connect(this, &Multiplayermenu::sigShowIPs, this, &Multiplayermenu::showIPs, Qt::QueuedConnection);
    }
}

Multiplayermenu::Multiplayermenu(const QString & address, quint16 port, const Password * password, NetworkMode networkMode)
    : MapSelectionMapsMenue(MemoryManagement::create<MapSelectionView>(QStringList({".map", ".jsm"})), Settings::getInstance()->getSmallScreenDevice() ? oxygine::Stage::getStage()->getHeight() - 80 : oxygine::Stage::getStage()->getHeight() - 230),
      m_networkMode(networkMode),
      m_local(true),
      m_password(*password)
{
    init();
    initClientConnection(address, "", port);
}

Multiplayermenu::Multiplayermenu(spNetworkInterface pNetworkInterface, const QString & password, NetworkMode networkMode)
    : MapSelectionMapsMenue(MemoryManagement::create<MapSelectionView>(QStringList({".map", ".jsm"})), Settings::getInstance()->getSmallScreenDevice() ? oxygine::Stage::getStage()->getHeight() - 80 : oxygine::Stage::getStage()->getHeight() - 230),
      m_networkMode(networkMode),
      m_local(false),
      m_password(password)
{
    m_pNetworkInterface = pNetworkInterface;
    init();
    if (m_networkMode != NetworkMode::Host)
    {
        initClientAndWaitForConnection();
    }
    else
    {
        oxygine::safeSpCast<Label>(m_pButtonStart->getFirstChild())->setHtmlText(tr("Ready"));
        m_pPlayerSelection->setIsServerGame(true);
    }
}

void Multiplayermenu::initClientConnection(const QString & address, const QString & secondaryAddress, quint16 port)
{
    m_pNetworkInterface = MemoryManagement::create<TCPClient>(nullptr);
    m_pNetworkInterface->setIsObserver(m_networkMode == NetworkMode::Observer);
    m_pNetworkInterface->moveToThread(Mainapp::getInstance()->getNetworkThread());
    m_pPlayerSelection->attachNetworkInterface(m_pNetworkInterface);
    initClientAndWaitForConnection();
    emit m_pNetworkInterface->sig_connect(address, port, secondaryAddress);
}

void Multiplayermenu::setSameVersionAsServer(bool newSameVersionAsServer)
{
    m_sameVersionAsServer = newSameVersionAsServer;
}

bool Multiplayermenu::getSameVersionAsServer() const
{
    return m_sameVersionAsServer;
}

void Multiplayermenu::initClientAndWaitForConnection()
{
    createChat();
    hideRuleSelection();
    Multiplayermenu::hideMapSelection();
    m_MapSelectionStep = MapSelectionStep::selectPlayer;
    // change the name of the start button
    if (m_networkMode == NetworkMode::Client)
    {
        m_pReadyAndLeave->setVisible(true);
        Mainapp::getInstance()->pauseRendering();
        oxygine::safeSpCast<Label>(m_pButtonStart->getFirstChild())->setHtmlText(tr("Ready"));
        Mainapp::getInstance()->continueRendering();
    }
    else if (m_networkMode == NetworkMode::Observer)
    {
        m_pButtonStart->setVisible(false);
    }
    // quit on host connection lost
    connectNetworkSlots();

    connect(m_pPlayerSelection.get(), &PlayerSelection::sigDisconnect, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
    // wait 10 minutes till timeout
    spDialogConnecting pDialogConnecting = MemoryManagement::create<DialogConnecting>(tr("Connecting"), 1000 * 60 * 5);
    addChild(pDialogConnecting);
    connect(pDialogConnecting.get(), &DialogConnecting::sigCancel, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
    connect(this, &Multiplayermenu::sigConnected, pDialogConnecting.get(), &DialogConnecting::connected, Qt::QueuedConnection);
}

void Multiplayermenu::init()
{
    CONSOLE_PRINT("Entering Multiplayer Menue", GameConsole::eDEBUG);
    Interpreter::setCppOwnerShip(this);
    m_pButtonLoadSavegame = ObjectManager::createButton(tr("Load Savegame"));
    m_pButtonLoadSavegame->setPosition(oxygine::Stage::getStage()->getWidth() - m_pButtonLoadSavegame->getScaledWidth() - m_pButtonNext->getScaledWidth() - 20,
                                       oxygine::Stage::getStage()->getHeight() - 10 - m_pButtonLoadSavegame->getScaledHeight());
    addChild(m_pButtonLoadSavegame);
    m_pButtonLoadSavegame->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigLoadSaveGame();
    });
    connect(this, &Multiplayermenu::sigLoadSaveGame, this, &Multiplayermenu::showLoadSaveGameDialog, Qt::QueuedConnection);

    m_pReadyAndLeave = ObjectManager::createButton(tr("Ready + Exit"));
    m_pReadyAndLeave->setPosition(oxygine::Stage::getStage()->getWidth() - m_pReadyAndLeave->getScaledWidth() - m_pButtonNext->getScaledWidth() - 20,
                                       oxygine::Stage::getStage()->getHeight() - 10 - m_pReadyAndLeave->getScaledHeight());
    addChild(m_pReadyAndLeave);
    m_pReadyAndLeave->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigReadyAndLeave();
    });
    connect(this, &Multiplayermenu::sigReadyAndLeave, this, &Multiplayermenu::readyAndLeave, Qt::QueuedConnection);
    m_pReadyAndLeave->setVisible(false);

    connect(&m_GameStartTimer, &QTimer::timeout, this, &Multiplayermenu::countdown, Qt::QueuedConnection);
    connect(&m_slaveDespawnTimer, &QTimer::timeout, this, &Multiplayermenu::despawnSlave, Qt::QueuedConnection);
    startDespawnTimer();
}

void Multiplayermenu::despawnSlave()
{
    const auto multiplier = 0.001f;
    std::chrono::milliseconds ms = Settings::getInstance()->getSlaveDespawnTime();
    auto elapsed = m_slaveDespawnElapseTimer.elapsed();
    CONSOLE_PRINT("Multiplayermenu::despawnSlave elapsed seconds " + QString::number(elapsed * multiplier) + " target time " + QString::number(ms.count() * multiplier), GameConsole::eDEBUG);
    if (m_slaveDespawnElapseTimer.hasExpired(ms.count()))
    {
        if (m_despawning)
        {
            CONSOLE_PRINT("Killing slave cause server didn't respond", GameConsole::eERROR);
            QCoreApplication::exit(-10);
        }
        else if (doDespawnSlave(Settings::getInstance()->getSlaveServerName()))
        {
        }
        else
        {
            CONSOLE_PRINT("Killing slave cause no locked players are found.", GameConsole::eDEBUG);
            QCoreApplication::exit(-10);
        }
        CONSOLE_PRINT("Restarting despawn timer", GameConsole::eDEBUG);
        m_slaveDespawnElapseTimer.start();
    }
}

bool Multiplayermenu::doDespawnSlave(const QString & savegame)
{
    if (m_pPlayerSelection->hasLockedPlayersInCaseOfDisconnect())
    {
        m_despawning = true;
        QString command = NetworkCommands::SLAVEINFODESPAWNING;
        QString saveFile = "savegames/" +  savegame + ".lsav";
        auto doc = doSaveLobbyState(saveFile, command);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        spNetworkInterface pSlaveMasterConnection = Mainapp::getInstance()->getSlaveClient();
        emit pSlaveMasterConnection->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
    return m_despawning;
}

QJsonDocument Multiplayermenu::doSaveLobbyState(const QString & saveFile, const QString & command)
{
    saveLobbyState(saveFile);
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    qint32 openPlayerCount = 0;
    if (m_pPlayerSelection.get() != nullptr)
    {
        openPlayerCount = m_pPlayerSelection->getOpenPlayerCount();
    }
    qint32 playerCount = pMap->getPlayerCount();
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_JOINEDPLAYERS, playerCount - openPlayerCount);
    data.insert(JsonKeys::JSONKEY_MAXPLAYERS, playerCount);
    data.insert(JsonKeys::JSONKEY_NAME, pMap->getMapName());
    data.insert(JsonKeys::JSONKEY_DESCRIPTION, pMap->getGameRules()->getDescription());
    data.insert(JsonKeys::JSONKEY_SLAVENAME, Settings::getInstance()->getSlaveServerName());
    data.insert(JsonKeys::JSONKEY_HASPASSWORD, pMap->getGameRules()->getPassword().getIsSet());
    data.insert(JsonKeys::JSONKEY_UUID, 0);
    data.insert(JsonKeys::JSONKEY_SAVEFILE, saveFile);
    data.insert(JsonKeys::JSONKEY_RUNNINGGAME, false);
    data.insert(JsonKeys::JSONKEY_CURRENTPLAYER, "");
    auto activeMods = Settings::getInstance()->getActiveMods();
    QJsonObject mods;
    for (qint32 i = 0; i < activeMods.size(); ++i)
    {
        mods.insert(JsonKeys::JSONKEY_MOD + QString::number(i), activeMods[i]);
    }
    data.insert(JsonKeys::JSONKEY_USEDMODS, mods);
    data.insert(JsonKeys::JSONKEY_USERNAMES, m_pPlayerSelection->getUserNames());
    return QJsonDocument(data);
}

void Multiplayermenu::saveLobbyState(const QString & filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QDataStream stream(&file);
    stream.setVersion(QDataStream::Version::Qt_6_5);
    m_pMapSelectionView->serializeObject(stream);
    m_pPlayerSelection->serializeObject(stream);
    file.close();
}

void Multiplayermenu::closeSlave()
{
    CONSOLE_PRINT("Closing slave.", GameConsole::eDEBUG);
    if (m_pNetworkInterface.get() != nullptr)
    {
        emit m_pNetworkInterface->sigDisconnectTCP();
        QCoreApplication::processEvents(QEventLoop::ProcessEventsFlag::AllEvents, 5);
    }
    QCoreApplication::exit(0);
}

void Multiplayermenu::doReadyAndLeave()
{
    markGameReady(true);
}

void Multiplayermenu::readyAndLeave()
{
    QString message;
    if (m_pNetworkInterface->getIsServer() && !m_local)
    {
        message = tr("Do you want to exit the game lobby and mark yourself as ready to start the game as soon as all players are ready?\nNote: You won't be able to change any rules when reentering the lobby later.");
    }
    else
    {
        message = tr("Do you want to exit the game lobby and mark yourself as ready to start the game as soon as all players are ready?");
    }
    spDialogMessageBox pDialogAccept = MemoryManagement::create<DialogMessageBox>(message, true);
    connect(pDialogAccept.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::doReadyAndLeave, Qt::QueuedConnection);
    addChild(pDialogAccept);
}

void Multiplayermenu::showIPs()
{
    spGenericBox pGenericBox = MemoryManagement::create<GenericBox>();
    QStringList items = NetworkInterface::getIPAdresses();
    QSize size(oxygine::Stage::getStage()->getWidth() - 40, oxygine::Stage::getStage()->getHeight() - 80);
    spPanel pPanel = MemoryManagement::create<Panel>(true, size, size);
    pPanel->setPosition(20, 20);
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    oxygine::spTextField info = MemoryManagement::create<oxygine::TextField>();
    info->setStyle(style);
    info->setHtmlText((tr("Please use one of the following IP-Addresses to connect to this Host. Not all IP-Addresses") +
                       tr(" may work for each client depending on their network settings. Please use CMD and the ping command to verify if an IP-Address will work")));
    info->setSize(oxygine::Stage::getStage()->getWidth() - 80, 500);
    info->setPosition(10, 10);
    pPanel->addItem(info);
    qint32 starty = 10 + info->getTextRect().height();
    for (qint32 i = 0; i < items.size(); i++)
    {
        oxygine::spTextField text = MemoryManagement::create<oxygine::TextField>();
        text->setStyle(style);
        text->setHtmlText((tr("Host Adress: ") + items[i]));
        text->setPosition(10, starty + i * 40);
        pPanel->addItem(text);
    }
    pPanel->setContentHeigth(items.size() * 40 + 40);
    pGenericBox->addItem(pPanel);
    addChild(pGenericBox);
}

void Multiplayermenu::showLoadSaveGameDialog()
{
    QStringList wildcards;
    wildcards.append("*.msav");
    QString path = Settings::getInstance()->getUserPath() + "savegames";
    spFileDialog saveDialog = MemoryManagement::create<FileDialog>(path, wildcards, false, "", false, tr("Load"));
    addChild(saveDialog);
    connect(saveDialog.get(), &FileDialog::sigFileSelected, this, &Multiplayermenu::loadSaveGame, Qt::QueuedConnection);
}

void Multiplayermenu::loadSaveGame(QString filename)
{    
    if (filename.endsWith(".msav"))
    {
        QFile file(filename);
        QFileInfo info(filename);
        if (file.exists())
        {
            m_pMapSelectionView->loadMap(info, false);
            hideMapSelection();
            m_saveGame = true;
            m_pPlayerSelection->setSaveGame(m_saveGame);
            m_MapSelectionStep = MapSelectionStep::selectRules;


            buttonNext();
        }
    }    
}

void Multiplayermenu::hideMapSelection()
{    
    m_pButtonLoadSavegame->setVisible(false);
    MapSelectionMapsMenue::hideMapSelection();    
}

void Multiplayermenu::showMapSelection()
{    
    m_pButtonLoadSavegame->setVisible(true);
    MapSelectionMapsMenue::showMapSelection();    
}

void Multiplayermenu::playerJoined(quint64 socketID)
{
    CONSOLE_PRINT("Multiplayermenu::playerJoined " +QString::number(socketID) , GameConsole::eDEBUG);
    if (m_pNetworkInterface->getIsServer() &&
       (m_local || Mainapp::getSlave()))
    {
        acceptNewConnection(socketID);
        if (m_pPlayerSelection.get() != nullptr)
        {
            m_pPlayerSelection->sendOpenPlayerCount();
        }
    }
}

void Multiplayermenu::acceptNewConnection(quint64 socketID)
{
    CONSOLE_PRINT("Accepting connection for socket " + QString::number(socketID), GameConsole::eDEBUG);
    if (Mainapp::getSlave() &&
        m_hostSocket == 0)
    {
        m_hostSocket = socketID;
    }
    if (Mainapp::getSlave())
    {
        QString command = QString(NetworkCommands::REQUESTLOGINDDATA);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        QJsonDocument doc(data);
        emit m_pNetworkInterface->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
    else
    {
        sendMapInfoUpdate(socketID);
    }
    CONSOLE_PRINT("Stopping despawn timer", GameConsole::eDEBUG);
    m_slaveDespawnTimer.stop();
}

void Multiplayermenu::recieveServerData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service, quint64 senderSocket)
{
    if (service == NetworkInterface::NetworkSerives::ServerHosting)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        QString messageType;
        stream >> messageType;
        CONSOLE_PRINT("Recieving data from Master. Command received: " + messageType + " for socket " + QString::number(socketID), GameConsole::eDEBUG);
        if (messageType == NetworkCommands::LAUNCHGAMEONSERVER)
        {
            launchGameOnServer(stream);
        }
        else
        {
            CONSOLE_PRINT("Unknown master server command " + messageType + " received", GameConsole::eDEBUG);
        }
    }
    else if (service == NetworkInterface::NetworkSerives::ServerHostingJson)
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject objData = doc.object();
        QString messageType = objData.value(JsonKeys::JSONKEY_COMMAND).toString();
        CONSOLE_PRINT("Master server network command received: " + messageType + " for socket " + QString::number(socketID), GameConsole::eDEBUG);
        if (messageType == NetworkCommands::SERVERRELAUNCHSLAVE)
        {
            onServerRelaunchSlave(socketID, objData);
        }
        else if (messageType == NetworkCommands::DESPAWNSLAVE)
        {
            closeSlave();
        }
        else if (messageType == NetworkCommands::SLAVEFORCEDESPAWN)
        {
            doDespawnSlave(Settings::getInstance()->getSlaveServerName());
        }
        else
        {
            CONSOLE_PRINT("Unknown master server command " + messageType + " received", GameConsole::eDEBUG);
        }
    }
    else
    {
        CONSOLE_PRINT("Unknown master service command " + QString::number(static_cast<qint32>(service)) + " received", GameConsole::eDEBUG);
    }
}

void Multiplayermenu::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service, quint64 senderSocket)
{
    // data for us?
    if (service == NetworkInterface::NetworkSerives::Multiplayer)
    {
        if (!m_pPlayerSelection->hasNetworkInterface())
        {
            m_pPlayerSelection->attachNetworkInterface(m_pNetworkInterface);
        }
        QDataStream stream(&data, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        QString messageType;
        stream >> messageType;
        CONSOLE_PRINT("Local Network Command in Multiplayermenu::recieveData received: " + messageType + " for socket " + QString::number(socketID), GameConsole::eDEBUG);
        if (messageType == NetworkCommands::MAPINFO)
        {
            clientMapInfo(stream, socketID);
        }
        else if (messageType == NetworkCommands::REQUESTRULE)
        {
            requestRule(socketID);
        }
        else if (messageType == NetworkCommands::SENDINITUPDATE)
        {
            sendInitUpdate(stream, socketID);
        }
        else if (messageType == NetworkCommands::REQUESTMAP)
        {
            requestMap(socketID);
        }
        else if (messageType == NetworkCommands::MAPDATA)
        {
            recieveMap(stream, socketID);
        }
        else if (messageType == NetworkCommands::INITGAME)
        {
            // initializes the game on the client
            if (!m_pNetworkInterface->getIsServer() ||
                !m_local)
            {
                initClientGame(socketID, stream);
                m_onEnterTimer.stop();
                oxygine::Actor::detach();
            }
        }
        else if (messageType == NetworkCommands::STARTSERVERGAME ||
                 messageType == NetworkCommands::CLIENTREADY)
        {
            if (messageType == NetworkCommands::CLIENTREADY)
            {
                m_pPlayerSelection->recievePlayerReady(socketID, stream);
            }
            if (!m_local)
            {
                CONSOLE_PRINT("Checking if server game should start", GameConsole::eDEBUG);
                startCountdown();
            }
        }
        else if (messageType == NetworkCommands::VERIFYGAMEDATA)
        {
            verifyGameData(stream, socketID);
        }
        else if (messageType == NetworkCommands::REQUESTJOINREASON)
        {
            sendJoinReason(stream, socketID);
        }
        else if (messageType == NetworkCommands::SENDCURRENTGAMESTATE)
        {
            receiveCurrentGameState(stream, socketID);
        }
        else if (messageType == NetworkCommands::RECEIVEPLAYERCONTROLLEDINFO)
        {
            receivePlayerControlledInfo(stream, socketID);
        }
        else if (messageType == NetworkCommands::DISCONNECTINGFROMSERVER)
        {
            exitMenuToLobby();
        }
        else
        {
            CONSOLE_PRINT("Unknown command in Multiplayermenu::recieveData " + messageType + " received", GameConsole::eDEBUG);
        }
    }
    else if (service == NetworkInterface::NetworkSerives::ServerHostingJson)
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject objData = doc.object();
        QString messageType = objData.value(JsonKeys::JSONKEY_COMMAND).toString();
        CONSOLE_PRINT("Server Network Command Multiplayermenu::recieveData: " + messageType + " for socket " + QString::number(socketID), GameConsole::eDEBUG);
        if (messageType == NetworkCommands::SLAVEADDRESSINFO)
        {
            connectToSlave(objData, socketID);
        }        
        else if (messageType == NetworkCommands::VERIFYLOGINDATA)
        {
            verifyLoginData(objData, socketID);
        }
        else if (messageType == NetworkCommands::SERVERINVALIDMODCONFIG)
        {
            spDialogMessageBox pDialogMessageBox;
            pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Server doesn't have the given mods installed."));
            connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
            addChild(pDialogMessageBox);
        }
        else if (messageType == NetworkCommands::SERVERNOGAMESLOTSAVAILABLE)
        {
            spDialogMessageBox pDialogMessageBox;
            pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Server doesn't have any more slots for players."));
            connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
            addChild(pDialogMessageBox);
        }
        else if (messageType == NetworkCommands::REQUESTLOGINDDATA)
        {
            sendLoginData(socketID, objData);
        }
        else if (messageType == NetworkCommands::DISCONNECTINGFROMSERVER)
        {
            showDisconnectReason(socketID, objData);
        }
        else if (messageType == NetworkCommands::REQUESTUSERNAME)
        {
            sendUsername(socketID, objData);
        }
        else if (messageType == NetworkCommands::DESPAWNSLAVE)
        {
            closeSlave();
        }
        else
        {
            CONSOLE_PRINT("Unknown command in Multiplayermenu::recieveData " + messageType + " received", GameConsole::eDEBUG);
        }
    }
}

void Multiplayermenu::showDisconnectReason(quint64 socketID, const QJsonObject & objData)
{
    CONSOLE_PRINT("Showing disconnect reason and exiting menu", GameConsole::eDEBUG);
    QStringList reasons =
    {
        tr("Connection failed. Reason: Invalid username password."),
        tr("Connection failed. Reason: No more observers available."),
        tr("Connection failed. Reason: No more players available."),
        tr("Connection failed. Reason: Invalid connection."),
        tr("Connection failed. Reason: Invalid username."),
        tr("Connection failed. Reason: Password outdated."),
        tr("Connection failed. Reason: Server failed to access database."),
        tr("Connection failed. Reason: Player with same username already connected to the game."),
    };
    NetworkCommands::DisconnectReason type = static_cast<NetworkCommands::DisconnectReason>(objData.value(JsonKeys::JSONKEY_DISCONNECTREASON).toInt());
    spDialogMessageBox pDialog = MemoryManagement::create<DialogMessageBox>(reasons[type]);
    addChild(pDialog);
    if (m_pDialogConnecting.get() != nullptr)
    {
        m_pDialogConnecting->detach();
        m_pDialogConnecting.reset();
    }
    emit m_pNetworkInterface->sigDisconnectClient(socketID);
    buttonBack();
}

void Multiplayermenu::sendUsername(quint64 socketID, const QJsonObject & objData)
{
    QString command = QString(NetworkCommands::SENDUSERNAME);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getInstance()->getUsername());
    QJsonDocument doc(data);
    emit m_pNetworkInterface->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void Multiplayermenu::sendLoginData(quint64 socketID, const QJsonObject & objData)
{
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, NetworkCommands::VERIFYLOGINDATA);
    Password serverPassword;
    QString password = Settings::getInstance()->getServerPassword();
    serverPassword.setPassword(password);
    data.insert(JsonKeys::JSONKEY_PASSWORD, GlobalUtils::toJsonArray(serverPassword.getHash()));
    data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getInstance()->getUsername());
    // send map data to client and make sure password message is crypted
    QString publicKey = objData.value(JsonKeys::JSONKEY_PUBLICKEY).toString();
    QJsonDocument doc(data);
    CONSOLE_PRINT("Sending login data to slave", GameConsole::eDEBUG);
    emit m_pNetworkInterface->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void Multiplayermenu::verifyLoginData(const QJsonObject & objData, quint64 socketID)
{
    QString username =  objData.value(JsonKeys::JSONKEY_USERNAME).toString();
    QByteArray password = GlobalUtils::toByteArray(objData.value(JsonKeys::JSONKEY_PASSWORD).toArray());
    GameEnums::LoginError valid = MainServer::verifyLoginData(username, password);
    if (valid == GameEnums::LoginError_None)
    {
        sendMapInfoUpdate(socketID);
    }
    else
    {
        CONSOLE_PRINT("Client login data are invalid. Closing connection.", GameConsole::eDEBUG);
        QString command = QString(NetworkCommands::DISCONNECTINGFROMSERVER);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        auto reason = NetworkCommands::DisconnectReason::InvalidPassword;
        switch (valid)
        {
            case GameEnums::LoginError_WrongPassword:
            {
                reason = NetworkCommands::DisconnectReason::InvalidPassword;
                break;
            }
            case GameEnums::LoginError_DatabaseNotAccesible:
            {
                reason = NetworkCommands::DisconnectReason::DatabaseAccessError;
                break;
            }
            case GameEnums::LoginError_AccountDoesntExist:
            {
                reason = NetworkCommands::DisconnectReason::InvalidUsername;
                break;
            }
            case GameEnums::LoginError_PasswordOutdated:
            {
                reason = NetworkCommands::DisconnectReason::PasswordOutdated;
                break;
            }
            default:
            {
                reason = NetworkCommands::DisconnectReason::DatabaseAccessError;
            }
        }
        CONSOLE_PRINT("Login error: " + QString::number(valid) + " reported reason: " + QString::number(reason), GameConsole::eDEBUG);
        data.insert(JsonKeys::JSONKEY_DISCONNECTREASON, reason);
        QJsonDocument doc(data);
        emit m_pNetworkInterface->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

void Multiplayermenu::sendMapInfoUpdate(quint64 socketID)
{
    QCryptographicHash myHash(QCryptographicHash::Sha512);
    QString file = m_pMapSelectionView->getCurrentFile().filePath();
    QString fileName = m_pMapSelectionView->getCurrentFile().fileName();
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    auto* pGameScript = pMap->getGameScript();
    auto* pGameRules = pMap->getGameRules();
    QString scriptFile;
    if (pGameScript != nullptr)
    {
        scriptFile = pGameScript->getScriptFile();
    }
    QFile mapFile(file);
    if (mapFile.exists())
    {
        mapFile.open(QIODevice::ReadOnly);
        myHash.addData(mapFile.readAll());
        mapFile.close();
    }
    QString command = QString(NetworkCommands::MAPINFO);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QByteArray hash = myHash.result();
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Version::Qt_6_5);
    stream << command;
    stream << Mainapp::getGameVersion();
    QStringList mods = Settings::getInstance()->getMods();
    QStringList versions = Settings::getInstance()->getActiveModVersions();
    bool filter = false;
    if (pGameRules != nullptr)
    {
        filter = pGameRules->getCosmeticModsAllowed();
    }
    Settings::getInstance()->filterCosmeticMods(mods, versions, filter);
    stream << filter;
    stream << static_cast<qint32>(mods.size());
    for (qint32 i = 0; i < mods.size(); i++)
    {
        stream << mods[i];
        stream << versions[i];
    }
    auto hostHash = Filesupport::getRuntimeHash(mods);
    if (GameConsole::eDEBUG >= GameConsole::getLogLevel())
    {
        QString hostString = GlobalUtils::getByteArrayString(hostHash);
        CONSOLE_PRINT("Sending host hash: " + hostString, GameConsole::eDEBUG);
    }
    Filesupport::writeByteArray(stream, hostHash);
    stream << m_saveGame;
    if (m_saveGame)
    {
        m_pMapSelectionView->getCurrentMap()->serializeObject(stream);
    }
    else
    {
        stream << fileName;
        stream << hash;
        stream << scriptFile;
        if (QFile::exists(scriptFile))
        {
            // create hash for script file
            QFile scriptData(scriptFile);
            QCryptographicHash myScriptHash(QCryptographicHash::Sha512);
            scriptData.open(QIODevice::ReadOnly);
            while (!scriptData.atEnd())
            {
                myHash.addData(scriptData.readLine().trimmed());
            }
            scriptData.close();
            QByteArray scriptHash = myScriptHash.result();
            stream << scriptHash;
        }
    }
    // send map data to client and make sure password message is crypted
    emit m_pNetworkInterface->sig_sendData(socketID, data, NetworkInterface::NetworkSerives::Multiplayer, false);
}

void Multiplayermenu::connectToSlave(const QJsonObject & objData, quint64 socketID)
{
    CONSOLE_PRINT("Connected to slave", GameConsole::eDEBUG);
    QString address = objData.value(JsonKeys::JSONKEY_ADDRESS).toString();
    QString secondarySlaveAddress = objData.value(JsonKeys::JSONKEY_SECONDARYADDRESS).toString();
    quint16 port = objData.value(JsonKeys::JSONKEY_PORT).toInteger();
    disconnectNetworkSlots();
    m_pNetworkInterface = MemoryManagement::create<TCPClient>(nullptr);
    m_pNetworkInterface->setIsObserver(m_networkMode == NetworkMode::Observer);
    m_pNetworkInterface->moveToThread(Mainapp::getInstance()->getNetworkThread());
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    if (pMap->getGameRules()->getGatewayHosting())
    {
        Mainapp::getInstance()->setSlaveClient(m_pNetworkInterface);
    }
    m_pPlayerSelection->attachNetworkInterface(m_pNetworkInterface);
    createChat();
    connectNetworkSlots();
    emit m_pNetworkInterface->sig_connect(address, port, secondarySlaveAddress);
}

void Multiplayermenu::onSlaveConnectedToMaster(quint64 socketID)
{
    CONSOLE_PRINT("Connected to master", GameConsole::eDEBUG);
    spNetworkInterface pSlaveMasterConnection = Mainapp::getInstance()->getSlaveClient();
    QString command = NetworkCommands::SLAVEREADY;
    QJsonObject data;
    QString slavename = Settings::getInstance()->getSlaveServerName();
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_SLAVENAME, slavename);
    QJsonDocument doc(data);
    CONSOLE_PRINT("Sending command " + command + " for slave " + slavename, GameConsole::eDEBUG);
    emit pSlaveMasterConnection->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void Multiplayermenu::onServerRelaunchSlave(quint64 socketID, const QJsonObject & objData)
{
    QString savefile = objData.value(JsonKeys::JSONKEY_NAME).toString();
    if (QFile::exists(savefile))
    {
        bool runningGame = objData.value(JsonKeys::JSONKEY_RUNNINGGAME).toBool();
        if (runningGame)
        {
            relaunchRunningGame(socketID, savefile);
        }
        else
        {
            relaunchRunningLobby(socketID, savefile);
        }
        CONSOLE_PRINT("Deleting relaunched savefile " + savefile, GameConsole::eDEBUG);
        QFile::remove(savefile);
    }
    else
    {
        CONSOLE_PRINT("Failed to relaunch slave with savefile " + savefile, GameConsole::eERROR);
        QCoreApplication::exit(0);
    }
}

void Multiplayermenu::relaunchRunningGame(quint64 socketID, const QString & savefile)
{
    CONSOLE_PRINT("Relaunching running game on slave with savefile " + savefile, GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    spGameMap pMap = MemoryManagement::create<GameMap>(savefile, false, false, true);
    pMap->initProxyAis();
    spGameMenue pMenu = MemoryManagement::create<GameMenue>(pMap, true, m_pNetworkInterface, false, true);
    oxygine::Stage::getStage()->addChild(pMenu);
    sendSlaveRelaunched(socketID);
    oxygine::Actor::detach();
}

void Multiplayermenu::relaunchRunningLobby(quint64 socketID, const QString & savefile)
{
    CONSOLE_PRINT("Relaunching running lobby on slave with savefile " + savefile, GameConsole::eDEBUG);
    QFile file(savefile);
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        m_pPlayerSelection->setIsServerGame(true);
        m_pPlayerSelection->attachNetworkInterface(m_pNetworkInterface);
        m_pMapSelectionView->deserializeObject(stream);
        m_pMapSelectionView->setCurrentFile(NetworkCommands::SERVERMAPIDENTIFIER);
        m_pPlayerSelection->deserializeObject(stream);
        m_pMapSelectionView->getCurrentMap()->setSavegame(m_pPlayerSelection->getSaveGame());
        loadMultiplayerMap(true);
        createChat();
        m_slaveGameReady = true;
        sendSlaveRelaunched(socketID);
        startDespawnTimer();
    }
    else
    {
        CONSOLE_PRINT("Failed to open savefile " + savefile, GameConsole::eERROR);
        QCoreApplication::exit(0);
    }
}

void Multiplayermenu::sendSlaveRelaunched(quint64 socketID)
{
    QString command = NetworkCommands::SLAVERELAUNCHED;
    QJsonObject data;
    QString slavename = Settings::getInstance()->getSlaveServerName();
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_SLAVENAME, slavename);
    QJsonDocument doc(data);
    CONSOLE_PRINT("Sending command " + command + " for slave " + slavename, GameConsole::eDEBUG);
    spNetworkInterface pSlaveMasterConnection = Mainapp::getInstance()->getSlaveClient();
    emit pSlaveMasterConnection->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void Multiplayermenu::receiveCurrentGameState(QDataStream & stream, quint64 socketID)
{
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    if (pMap.get() != nullptr)
    {
        pMap->detach();
    }
    m_pMapSelectionView->setCurrentMap(spGameMap());
    pMap = MemoryManagement::create<GameMap, QDataStream &, bool>(stream, true);
    if (m_password.areEqualPassword(pMap->getGameRules()->getPassword()))
    {
        pMap->updateSprites(-1, -1, false, true);
        m_pMapSelectionView->setCurrentMap(pMap);
        if (m_networkMode == NetworkMode::Observer)
        {
            for (qint32 i = 0; i < pMap->getPlayerCount(); ++i)
            {
                Player* pPlayer = pMap->getPlayer(i);
                BaseGameInputIF* pInput = pPlayer->getBaseGameInput();
                if (pInput != nullptr &&
                    pInput->getAiType() != GameEnums::AiTypes_ProxyAi &&
                    pInput->getAiType() != GameEnums::AiTypes_Closed &&
                    pInput->getAiType() != GameEnums::AiTypes_Open)
                {
                    pPlayer->setBaseGameInput(BaseGameInputIF::createAi(pMap.get(), GameEnums::AiTypes_ProxyAi));
                }
            }
            qint64 syncCounter = 0;
            stream >> syncCounter;            
            startRejoinedGame(syncCounter);
        }
        else
        {
            QString command = NetworkCommands::REQUESTPLAYERCONTROLLEDINFO;
            QByteArray sendData;
            QDataStream sendStream(&sendData, QIODevice::WriteOnly);
            sendStream.setVersion(QDataStream::Version::Qt_6_5);
            sendStream << command;
            sendStream << Settings::getInstance()->getUsername();
            emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
        }
    }
    else
    {
        CONSOLE_PRINT("Incorrect Password found.", GameConsole::eDEBUG);
        CONSOLE_PRINT("Entered password hash: " + GlobalUtils::getByteArrayString(m_password.getHash()), GameConsole::eDEBUG);
        CONSOLE_PRINT("Host    password hash: " + GlobalUtils::getByteArrayString(m_pMapSelectionView->getCurrentMap()->getGameRules()->getPassword().getHash()), GameConsole::eDEBUG);
        spDialogMessageBox pDialogMessageBox;
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Wrong password entered for joining the game."));
        connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
        addChild(pDialogMessageBox);
    }
}

void Multiplayermenu::receivePlayerControlledInfo(QDataStream & stream, quint64 socketID)
{
    QVector<qint32> playerAis;
    QVector<GameEnums::AiTypes> aiTypes;
    qint32 dataSize;
    stream >> dataSize;
    for (qint32 i = 0; i < dataSize; ++i)
    {
        qint32 player;
        stream >> player;
        playerAis.append(player);
        qint32 ai;
        stream >> ai;
        aiTypes.append(static_cast<GameEnums::AiTypes>(ai));
    }
    qint64 syncCounter = 0;
    stream >> syncCounter;
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    for (qint32 i = 0; i < pMap->getPlayerCount(); ++i)
    {
        Player* pPlayer = pMap->getPlayer(i);
        BaseGameInputIF* pInput = pPlayer->getBaseGameInput();
        if (playerAis.contains(i))
        {
            CONSOLE_PRINT("updating ai for player " + QString::number(i) + " to " + QString::number(aiTypes[playerAis.indexOf(i)]), GameConsole::eDEBUG);
            pPlayer->setBaseGameInput(BaseGameInputIF::createAi(pMap.get(), aiTypes[playerAis.indexOf(i)]));
        }
        else if (pInput != nullptr &&
                 pInput->getAiType() != GameEnums::AiTypes_ProxyAi &&
                 pInput->getAiType() != GameEnums::AiTypes_Closed &&
                 pInput->getAiType() != GameEnums::AiTypes_Open &&
                 !Mainapp::getTrainingSession())
        {
            pPlayer->setBaseGameInput(BaseGameInputIF::createAi(pMap.get(), GameEnums::AiTypes_ProxyAi));
        }
    }
    startRejoinedGame(syncCounter);
}

void Multiplayermenu::startRejoinedGame(qint64 syncCounter)
{
    // start game
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    CONSOLE_PRINT("Leaving Map Selection Menue and rejoining game with sync counter " + QString::number(syncCounter), GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    auto window = MemoryManagement::create<GameMenue>(pMap, true, m_pNetworkInterface, true);
    window->getActionPerformer()->setSyncCounter(syncCounter);
    oxygine::Stage::getStage()->addChild(window);

    QString command = NetworkCommands::RECEIVEDCURRENTGAMESTATE;
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream.setVersion(QDataStream::Version::Qt_6_5);
    sendStream << command;
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
    oxygine::Actor::detach();
}

void Multiplayermenu::sendJoinReason(QDataStream & stream, quint64 socketID)
{
    if (m_networkMode == NetworkMode::Observer)
    {
        QString command = NetworkCommands::JOINASOBSERVER;
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        sendStream << command;
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
    }
    else
    {
        QString command = NetworkCommands::JOINASPLAYER;
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        sendStream << command;
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
    }
}

void Multiplayermenu::requestRule(quint64 socketID)
{
    // a client requested the current map rules set by the server
    if (m_pNetworkInterface->getIsServer())
    {
        QString command = QString(NetworkCommands::SENDINITUPDATE);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        sendStream << command;
        spGameMap pMap = m_pMapSelectionView->getCurrentMap();
        pMap->getGameRules()->serializeObject(sendStream);
        if (pMap->getCampaign() != nullptr)
        {
            sendStream << true;
            pMap->getCampaign()->serializeObject(sendStream);
        }
        else
        {
            sendStream << false;
        }
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            GameEnums::AiTypes aiType = m_pPlayerSelection->getPlayerAiType(i);
            const QString playerId = pMap->getPlayer(i)->getPlayerNameId();
            qint32 intAiType = 0;
            if (aiType == GameEnums::AiTypes_Human && !m_saveGame)
            {
                intAiType = static_cast<qint32>(GameEnums::AiTypes_ProxyAi);
            }
            else
            {
                if (m_pPlayerSelection->isOpenPlayer(i))
                {
                    intAiType = static_cast<qint32>(GameEnums::AiTypes_Open);
                }
                else if (m_pPlayerSelection->isClosedPlayer(i))
                {
                    intAiType = static_cast<qint32>(GameEnums::AiTypes_Closed);
                }
                else
                {
                    intAiType = static_cast<qint32>(GameEnums::AiTypes_ProxyAi);
                }
            }
            sendStream << playerId;
            sendStream << intAiType;
            CONSOLE_PRINT("Sending " + playerId + " with ai type " + QString::number(aiType) + " for player " + QString::number(i), GameConsole::eDEBUG);
            m_pMapSelectionView->getCurrentMap()->getPlayer(i)->serializeObject(sendStream);
        }
        emit m_pNetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
    }
}

void Multiplayermenu::sendInitUpdate(QDataStream & stream, quint64 socketID)
{
    // the client recieved the initial map data read it and make it visible
    if (!m_pNetworkInterface->getIsServer() || !m_local)
    {
        m_pMapSelectionView->getCurrentMap()->getGameRules()->deserializeObject(stream);
        if (!m_password.areEqualPassword(m_pMapSelectionView->getCurrentMap()->getGameRules()->getPassword()))
        {
            CONSOLE_PRINT("Incorrect Password found.", GameConsole::eDEBUG);
            CONSOLE_PRINT("Entered password hash: " + GlobalUtils::getByteArrayString(m_password.getHash()), GameConsole::eDEBUG);
            CONSOLE_PRINT("Host    password hash: " + GlobalUtils::getByteArrayString(m_pMapSelectionView->getCurrentMap()->getGameRules()->getPassword().getHash()), GameConsole::eDEBUG);
            spDialogMessageBox pDialogMessageBox;
            pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Wrong password entered for joining the game."));
            connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
            addChild(pDialogMessageBox);
        }
        else
        {
            bool campaign = false;
            stream >> campaign;
            if (campaign)
            {
                m_pMapSelectionView->getCurrentMap()->setCampaign(MemoryManagement::create<Campaign>());
                m_pMapSelectionView->getCurrentMap()->getCampaign()->deserializeObject(stream);
            }
            CONSOLE_PRINT("Reading players count: " + QString::number(m_pMapSelectionView->getCurrentMap()->getPlayerCount()), GameConsole::eDEBUG);
            for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getPlayerCount(); i++)
            {
                QString name;
                qint32 aiType;
                stream >> name;
                stream >> aiType;
                CONSOLE_PRINT("Read " + name + " with ai type " + QString::number(aiType) + " for player " + QString::number(i), GameConsole::eDEBUG);
                m_pPlayerSelection->setPlayerAiName(i, name);
                spGameMap pMap = m_pMapSelectionView->getCurrentMap();
                Player* pPlayer = pMap->getPlayer(i);
                pPlayer->deserializeObject(stream);
                pPlayer->setControlType(static_cast<GameEnums::AiTypes>(aiType));
                if (aiType != GameEnums::AiTypes::AiTypes_Open &&
                    aiType != GameEnums::AiTypes::AiTypes_Closed)
                {
                    aiType = GameEnums::AiTypes::AiTypes_ProxyAi;
                }
                pPlayer->setBaseGameInput(BaseGameInputIF::createAi(pMap.get(), static_cast<GameEnums::AiTypes>(aiType)));
                pPlayer->setPlayerNameId(name);
                m_pPlayerSelection->updatePlayerData(i);
            }
            if (m_networkMode == NetworkMode::Observer)
            {
                QString command = NetworkCommands::JOINASOBSERVER;
                QByteArray sendData;
                QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                sendStream.setVersion(QDataStream::Version::Qt_6_5);
                sendStream << command;
                CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
                emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
            }
            else
            {
                CONSOLE_PRINT("Multiplayermenu::sendInitUpdate requesting player", GameConsole::eDEBUG);
                m_pPlayerSelection->sendPlayerRequest(socketID, -1, GameEnums::AiTypes_Human);
            }
            emit sigConnected();
            emit sigHostGameLaunched();
        }
    }
}

void Multiplayermenu::verifyGameData(QDataStream & stream, quint64 socketID)
{
    if (!m_pNetworkInterface->getIsServer() || !m_local)
    {
        bool sameMods = false;
        bool differentHash = false;
        bool sameVersion = false;
        QStringList mods;
        QStringList versions;
        QStringList myMods;
        QStringList myVersions;
        readHashInfo(stream, socketID, mods, versions, myMods, myVersions, sameMods, differentHash, sameVersion);
        if (sameVersion && sameMods && !differentHash)
        {
            QString command = QString(NetworkCommands::GAMEDATAVERIFIED);
            CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Version::Qt_6_5);
            stream << command;
            emit m_pNetworkInterface->sig_sendData(socketID, data, NetworkInterface::NetworkSerives::Multiplayer, false);
        }
        else
        {
            handleVersionMissmatch(mods, versions, myMods, myVersions, sameMods, differentHash, sameVersion);
        }
    }
}

bool Multiplayermenu::checkMods(const QStringList & mods, const QStringList & versions, QStringList & myMods, QStringList & myVersions, bool filter)
{
    myVersions = Settings::getInstance()->getActiveModVersions();
    myMods = Settings::getInstance()->getMods();
    Settings::getInstance()->filterCosmeticMods(myMods, myVersions, filter);
    bool sameMods = true;
    if (myMods.size() != mods.size())
    {
        sameMods = false;
    }
    else
    {
        // check mods in both directions
        for (qint32 i = 0; i < myMods.size(); i++)
        {
            if (!mods.contains(myMods[i]))
            {
                sameMods = false;
                break;
            }
            else
            {
                for (qint32 i2 = 0; i2 < mods.size(); i2++)
                {
                    if (mods[i2] == myMods[i])
                    {
                        if (versions[i2] != myVersions[i])
                        {
                            sameMods = false;
                            break;
                        }
                    }
                }
            }
        }
        for (qint32 i = 0; i < mods.size(); i++)
        {
            if (!myMods.contains(mods[i]))
            {
                sameMods = false;
                break;
            }
        }
    }
    return sameMods;
}

void Multiplayermenu::readHashInfo(QDataStream & stream, quint64 socketID, QStringList & mods, QStringList & versions, QStringList & myMods, QStringList & myVersions, bool & sameMods, bool & differentHash, bool & sameVersion)
{
    QString version;
    stream >> version;
    bool filter = false;
    stream >> filter;
    qint32 size = 0;
    stream >> size;
    for (qint32 i = 0; i < size; i++)
    {
        QString mod;
        stream >> mod;
        mods.append(mod);
        QString version;
        stream >> version;
        versions.append(version);
    }
    sameMods = checkMods(mods, versions, myMods, myVersions, filter);
    QByteArray hostRuntime = Filesupport::readByteArray(stream);
    QByteArray ownRuntime = Filesupport::getRuntimeHash(mods);
    if (GameConsole::eDEBUG >= GameConsole::getLogLevel())
    {
        QString hostString = GlobalUtils::getByteArrayString(hostRuntime);
        QString ownString = GlobalUtils::getByteArrayString(ownRuntime);
        CONSOLE_PRINT("Received host hash: " + hostString, GameConsole::eDEBUG);
        CONSOLE_PRINT("Own hash:           " + ownString, GameConsole::eDEBUG);
    }
    differentHash = (hostRuntime != ownRuntime);
    sameVersion = version == Mainapp::getGameVersion();
}

void Multiplayermenu::clientMapInfo(QDataStream & stream, quint64 socketID)
{
    // we recieved map info from a server check if we have this map already
    if (!m_pNetworkInterface->getIsServer() || !m_local)
    {
        bool sameMods = false;
        bool differentHash = false;
        bool sameVersion = false;
        QStringList mods;
        QStringList versions;
        QStringList myMods;
        QStringList myVersions;
        readHashInfo(stream, socketID, mods, versions, myMods, myVersions, sameMods, differentHash, sameVersion);
        if (sameVersion && sameMods && !differentHash)
        {
            stream >> m_saveGame;
            m_pPlayerSelection->setSaveGame(m_saveGame);
            if (m_saveGame)
            {
                QString command = QString(NetworkCommands::REQUESTRULE);
                CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
                spGameMap pMap = MemoryManagement::create<GameMap, QDataStream &, bool>(stream, m_saveGame);
                m_pMapSelectionView->setCurrentMap(pMap);
                loadMultiplayerMap();
                QByteArray sendData;
                QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                sendStream.setVersion(QDataStream::Version::Qt_6_5);
                sendStream << command;
                emit m_pNetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
            }
            else
            {
                QString fileName;
                stream >> fileName;
                QByteArray hash;
                stream >> hash;
                QString scriptFile;
                stream >> scriptFile;
                QByteArray scriptHash;
                CONSOLE_PRINT("Checking for map " + fileName + " and script " + scriptFile , GameConsole::eDEBUG);
                if (!scriptFile.isEmpty())
                {
                    stream >> scriptHash;
                }
                if (!fileName.startsWith(NetworkCommands::RANDOMMAPIDENTIFIER) &&
                    !fileName.startsWith(NetworkCommands::SERVERMAPIDENTIFIER) &&
                    existsMap(fileName, hash, scriptFile, scriptHash))
                {
                    QString command = QString(NetworkCommands::REQUESTRULE);
                    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
                    QByteArray sendData;
                    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                    sendStream.setVersion(QDataStream::Version::Qt_6_5);
                    sendStream << command;
                    emit m_pNetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
                }
                else
                {
                    QString command = QString(NetworkCommands::REQUESTMAP);
                    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
                    QByteArray sendData;
                    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                    sendStream.setVersion(QDataStream::Version::Qt_6_5);
                    sendStream << command;
                    emit m_pNetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
                }
            }
        }
        else
        {
            handleVersionMissmatch(mods, versions, myMods, myVersions, sameMods, differentHash, sameVersion);
        }
    }
}

void Multiplayermenu::handleVersionMissmatch(const QStringList & mods, const QStringList & versions, const QStringList & myMods, const QStringList & myVersions, bool sameMods, bool differentHash, bool sameVersion)
{
    // quit game with wrong version
    spDialogMessageBox pDialogMessageBox;
    if (differentHash)
    {
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Host has a different version of a mod or the game resource folder has been modified by one of the games."));
    }
    else  if (!sameVersion)
    {
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Host has a different game version. Leaving the game again."));
    }
    else if (!sameMods)
    {
        QString hostModsInfo;
        for (qint32 i = 0; i < mods.size(); ++i)
        {
            hostModsInfo += Settings::getInstance()->getModName(mods[i]) + " " + versions[i] + "\n";
        }
        QString myModsInfo;
        for (qint32 i = 0; i < myMods.size(); ++i)
        {
            myModsInfo += Settings::getInstance()->getModName(myMods[i]) + " " + myVersions[i]  + "\n";
        }
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Host has different mods. Leaving the game again.\nHost mods:\n") + hostModsInfo + "\nYour Mods:\n" + myModsInfo);
    }
    else
    {
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Failed to join game due to unknown verification failure."));
    }
    connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
    addChild(pDialogMessageBox);
}

void Multiplayermenu::requestMap(quint64 socketID)
{
    // someone requested the current map data from the server
    if (m_pNetworkInterface->getIsServer())
    {
        QString command = QString(NetworkCommands::MAPDATA);
        QString file = GlobalUtils::makePathRelative(m_pMapSelectionView->getCurrentFile().filePath(), false);
        QString scriptFile = m_pMapSelectionView->getCurrentMap()->getGameScript()->getScriptFile();
        CONSOLE_PRINT("Sending command " + command + " with map file " + file + " and script file " + scriptFile, GameConsole::eDEBUG);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        sendStream << command;
        sendStream << file;
        sendStream << scriptFile;
        QFile mapFile(m_pMapSelectionView->getCurrentFile().filePath());
        if (file.startsWith(NetworkCommands::RANDOMMAPIDENTIFIER) ||
            file.startsWith(NetworkCommands::SERVERMAPIDENTIFIER))
        {
            spGameMap pMap = m_pMapSelectionView->getCurrentMap();
            pMap->serializeObject(sendStream);
        }
        else
        {
            mapFile.open(QIODevice::ReadOnly);
            QByteArray data = mapFile.readAll();
            mapFile.close();
            Filesupport::writeByteArray(sendStream, data);
        }
        if (!scriptFile.isEmpty())
        {
            QFile script(scriptFile);
            script.open(QIODevice::ReadOnly);
            QByteArray data = script.readAll();
            Filesupport::writeByteArray(sendStream, data);
            script.close();
        }
        emit m_pNetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
    }
}

void Multiplayermenu::recieveMap(QDataStream & stream, quint64 socketID)
{
    if (!m_pNetworkInterface->getIsServer())
    {
        QString mapFile;
        stream >> mapFile;
        QString scriptFile;
        stream >> scriptFile;
        CONSOLE_PRINT("Multiplayermenu::recieveMap with map file " + mapFile + " and script file " + scriptFile, GameConsole::eDEBUG);
        spGameMap pMap = m_pMapSelectionView->getCurrentMap();
        if (pMap.get() != nullptr)
        {
            pMap->detach();
        }
        if (mapFile.startsWith(NetworkCommands::RANDOMMAPIDENTIFIER) ||
            mapFile.startsWith(NetworkCommands::SERVERMAPIDENTIFIER))
        {
            pMap = MemoryManagement::create<GameMap, QDataStream &, bool>(stream, m_saveGame);
        }
        else
        {
            QFile map(mapFile);
            QFile script(scriptFile);
            if (!map.exists() &&
                (scriptFile.isEmpty() ||
                 !script.exists()))
            {
                QByteArray mapData;
                mapData = Filesupport::readByteArray(stream);
                QFileInfo mapInfo(mapFile);
                QDir dir = mapInfo.absoluteDir();
                dir.mkpath(".");
                if (map.open(QIODevice::WriteOnly))
                {
                    QDataStream mapFilestream(&map);
                    mapFilestream.setVersion(QDataStream::Version::Qt_6_5);
                    Filesupport::writeBytes(mapFilestream, mapData);
                    map.close();
                    if (!scriptFile.isEmpty())
                    {
                        QByteArray scriptData;
                        scriptData = Filesupport::readByteArray(stream);
                        QFileInfo scriptInfo(scriptFile);
                        dir = scriptInfo.absoluteDir();
                        dir.mkpath(".");
                        if (script.open(QIODevice::WriteOnly))
                        {
                            QDataStream scriptFilestream(&script);
                            scriptFilestream.setVersion(QDataStream::Version::Qt_6_5);
                            Filesupport::writeBytes(scriptFilestream, scriptData);
                            script.close();
                        }
                        else
                        {
                            CONSOLE_PRINT("Unable to write downloaded script-file", GameConsole::eDEBUG);
                            spDialogMessageBox pDialogMessageBox;
                            pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Unable to download script file from host."));
                            connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
                            addChild(pDialogMessageBox);
                        }
                    }
                    pMap = MemoryManagement::create<GameMap>(mapFile, true, false, m_saveGame);
                }
                else
                {
                    CONSOLE_PRINT("Unable to write downloaded map-file", GameConsole::eDEBUG);
                    spDialogMessageBox pDialogMessageBox;
                    pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Unable to download map file from host."));
                    connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
                    addChild(pDialogMessageBox);
                }
            }
            else
            {
                pMap = createMapFromStream(mapFile, scriptFile, stream);
            }
        }
        m_pMapSelectionView->setCurrentMap(pMap);
        QString command = QString(NetworkCommands::REQUESTRULE);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        loadMultiplayerMap();
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        sendStream << command;
        emit m_pNetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
    }
}

void Multiplayermenu::launchGameOnServer(QDataStream & stream)
{
    CONSOLE_PRINT("Launching Game on Slave", GameConsole::eDEBUG);
    hideMapSelection();
    QStringList mods;
    mods = Filesupport::readVectorList<QString, QList>(stream);
    Filesupport::readByteArray(stream); // minimap data
    spGameMap pMap = MemoryManagement::create<GameMap, QDataStream &, bool>(stream, m_saveGame);
    stream >> m_saveGame;
    CONSOLE_PRINT("Is save game " + QString::number(m_saveGame), GameConsole::eDEBUG);
    m_pPlayerSelection->setSaveGame(m_saveGame);
    m_pMapSelectionView->setCurrentMap(pMap);
    m_pMapSelectionView->setCurrentFile(NetworkCommands::SERVERMAPIDENTIFIER);
    m_pPlayerSelection->attachNetworkInterface(m_pNetworkInterface);
    m_pPlayerSelection->setIsServerGame(true);
    loadMultiplayerMap(m_saveGame);
    createChat();
    // open all players
    if (m_saveGame)
    {
        CONSOLE_PRINT("Reopening all human players", GameConsole::eDEBUG);
        for (qint32 i = 0; i < pMap->getPlayerCount(); ++i)
        {
            if (pMap->getPlayer(i)->getControlType() == GameEnums::AiTypes::AiTypes_Human)
            {
                m_pPlayerSelection->setPlayerAi(i, GameEnums::AiTypes::AiTypes_Open, "");
            }
        }
    }
    else
    {

        CONSOLE_PRINT("Reopening player 1", GameConsole::eDEBUG);
        m_pPlayerSelection->setPlayerAi(0, GameEnums::AiTypes::AiTypes_Open, "");
    }
    sendSlaveReady();
    m_slaveGameReady = true;
}

void Multiplayermenu::sendSlaveReady()
{
    QString command = QString(NetworkCommands::GAMERUNNINGONSERVER);
    QString slavename = Settings::getInstance()->getSlaveServerName();
    CONSOLE_PRINT("Sending command " + command + " for slave " + slavename, GameConsole::eDEBUG);
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_SLAVENAME, slavename);
    data.insert(JsonKeys::JSONKEY_DESCRIPTION, pMap->getGameRules()->getDescription());
    if (pMap->getGameRules()->getPassword().isValidPassword(""))
    {
        data.insert(JsonKeys::JSONKEY_HASPASSWORD, false);
    }
    else
    {
        data.insert(JsonKeys::JSONKEY_HASPASSWORD, true);
    }
    QJsonDocument doc(data);
    emit Mainapp::getInstance()->getSlaveClient()->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, true);
}

void Multiplayermenu::slotCancelHostConnection()
{
    CONSOLE_PRINT("Canceled host connection", GameConsole::eDEBUG);
    buttonBack();
}

void Multiplayermenu::slotHostGameLaunched()
{
    if (m_pNetworkInterface.get() != nullptr)
    {
        m_pDialogConnecting.reset();
        // we're hosting a game so we get the same rights as a local host
        m_pNetworkInterface->setIsServer(true);
        createChat();
        MapSelectionMapsMenue::buttonNext();
    }
}

spGameMap Multiplayermenu::createMapFromStream(QString mapFile, QString scriptFile, QDataStream &stream)
{
    spGameMap pNewMap;
    mapFile = getNewFileName(mapFile);
    QFile map(mapFile);
    QFileInfo mapInfo(mapFile);
    QDir dir;
    QString fileDir = mapInfo.filePath().replace(mapInfo.fileName(), "");
    dir.mkdir(fileDir);
    map.open(QIODevice::WriteOnly);
    QDataStream mapFilestream(&map);
    mapFilestream.setVersion(QDataStream::Version::Qt_6_5);
    QByteArray mapData;
    mapData = Filesupport::readByteArray(stream);
    Filesupport::writeBytes(mapFilestream, mapData);
    map.close();
    pNewMap = MemoryManagement::create<GameMap>(mapFile, true, false, m_saveGame);
    if (!scriptFile.isEmpty())
    {
        QFile script(scriptFile);
        scriptFile = getNewFileName(scriptFile);
        QByteArray scriptData;
        scriptData = Filesupport::readByteArray(stream);
        QFileInfo scriptInfo(scriptFile);
        fileDir = scriptInfo.filePath().replace(scriptInfo.fileName(), "");
        dir.mkdir(fileDir);
        script.open(QIODevice::WriteOnly);
        QDataStream scriptFilestream(&script);
        scriptFilestream.setVersion(QDataStream::Version::Qt_6_5);
        Filesupport::writeBytes(scriptFilestream, scriptData);
        script.close();
        scriptFile = GlobalUtils::makePathRelative(scriptFile, true);
        // save script file
        pNewMap->getGameScript()->setScriptFile(scriptFile);
        map.open(QIODevice::WriteOnly);
        QDataStream stream(&map);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        pNewMap->serializeObject(stream);
        map.close();
    }
    return pNewMap;
}

QString Multiplayermenu::getNewFileName(QString filename)
{
    bool found = false;
    qint32 index = 0;
    QString newName;
    filename = filename.replace(".map", "");
    while (!found)
    {
        newName = filename + "+" + QString::number(index) + ".map";
        found = !QFile::exists(newName);
        index++;
    }
    return newName;
}

void Multiplayermenu::loadMultiplayerMap(bool relaunchedLobby)
{
    CONSOLE_PRINT("Multiplayermenu::loadMultiplayerMap", GameConsole::eDEBUG);
    m_pMapSelectionView->getCurrentMap()->getGameScript()->init();
    m_pMapSelectionView->updateMapData();
    showPlayerSelection(relaunchedLobby);
}

void Multiplayermenu::initClientGame(quint64, QDataStream &stream)
{
    CONSOLE_PRINT("Multiplayermenu::initClientGame", GameConsole::eDEBUG);
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    pMap->setVisible(false);
    quint32 seed;
    stream >> seed;
    GlobalUtils::seed(seed);
    GlobalUtils::setUseSeed(true);
    if (!m_saveGame)
    {
        pMap->initPlayers();
    }
    for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getPlayerCount(); i++)
    {
        GameEnums::AiTypes aiType = GameEnums::AiTypes::AiTypes_Closed;
        auto* baseGameInput = m_pMapSelectionView->getCurrentMap()->getPlayer(i)->getBaseGameInput();        
        if (baseGameInput != nullptr)
        {
            aiType = baseGameInput->getAiType();
        }
        CONSOLE_PRINT("Creating AI for player " + QString::number(i) + " of type " + QString::number(aiType), GameConsole::eDEBUG);
        spGameMap pMap = m_pMapSelectionView->getCurrentMap();
        pMap->getPlayer(i)->deserializeObject(stream);
        pMap->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(pMap.get(), aiType));
    }
    if (!m_saveGame)
    {
        pMap->getGameScript()->gameStart();
    }
    bool applyRulesPalette = pMap->getGameRules()->getMapPalette() > 0;
    pMap->updateSprites(-1, -1, false, false, applyRulesPalette);
    // start game
    m_pNetworkInterface->setIsServer(false);
    CONSOLE_PRINT("Leaving Map Selection Menue and init client game", GameConsole::eDEBUG);
    auto window = MemoryManagement::create<GameMenue>(pMap, m_saveGame, m_pNetworkInterface, false);
    oxygine::Stage::getStage()->addChild(window);
    // send game started
    QString command = QString(NetworkCommands::CLIENTINITGAME);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream.setVersion(QDataStream::Version::Qt_6_5);
    sendStream << command;
    sendStream << m_pNetworkInterface->getSocketID();
    emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
}

bool Multiplayermenu::existsMap(QString& fileName, QByteArray& hash, QString& scriptFileName, QByteArray& scriptHash)
{
    QString path = "maps";
    QStringList filter;
    filter << "*" + fileName;
    QDirIterator dirIter(Settings::getInstance()->getUserPath() + path, filter, QDir::Files, QDirIterator::Subdirectories);
    bool found = findAndLoadMap(dirIter, hash, m_saveGame);
    if (!found)
    {
        QDirIterator virtDirIter(oxygine::Resource::RCC_PREFIX_PATH + path, filter, QDir::Files, QDirIterator::Subdirectories);
        found = findAndLoadMap(virtDirIter, hash, m_saveGame);
    }
    if (found  && !m_saveGame)
    {
        if (!scriptFileName.isEmpty())
        {
            QFile scriptFile;
            QByteArray myHashArray;
            // check real drive and virt drive for script
            if (QFile::exists(Settings::getInstance()->getUserPath() + scriptFileName))
            {
                scriptFile.setFileName(Settings::getInstance()->getUserPath() + scriptFileName);
                scriptFile.open(QIODevice::ReadOnly);
                QCryptographicHash myHash(QCryptographicHash::Sha512);
                while (!scriptFile.atEnd())
                {
                    myHash.addData(scriptFile.readLine().trimmed());
                }
                scriptFile.close();
                myHashArray = myHash.result();

            }
            if (myHashArray != scriptHash && QFile::exists(oxygine::Resource::RCC_PREFIX_PATH + scriptFileName))
            {
                scriptFile.setFileName(oxygine::Resource::RCC_PREFIX_PATH + scriptFileName);
                scriptFile.open(QIODevice::ReadOnly);
                QCryptographicHash myHash(QCryptographicHash::Sha512);
                while (!scriptFile.atEnd())
                {
                    myHash.addData(scriptFile.readLine().trimmed());
                }
                scriptFile.close();
                QByteArray myHashArray = myHash.result();
                if (myHashArray != scriptHash)
                {
                    found = false;
                }
            }
            else
            {
                found = false;
            }
        }
    }
    return found;
}

bool Multiplayermenu::findAndLoadMap(QDirIterator & dirIter, QByteArray& hash, bool m_saveGame)
{
    bool found = false;
    while (dirIter.hasNext() && !found)
    {
        dirIter.next();
        QString file = dirIter.fileInfo().canonicalFilePath();
        QFile mapFile(file);
        mapFile.open(QIODevice::ReadOnly);
        QCryptographicHash myHash(QCryptographicHash::Sha512);
        myHash.addData(&mapFile);
        mapFile.close();
        QByteArray myHashArray = myHash.result();
        if (hash == myHashArray)
        {
            spGameMap pMap = MemoryManagement::create<GameMap>(file, true, false, m_saveGame);
            m_pMapSelectionView->setCurrentMap(pMap);
            loadMultiplayerMap();
            found = true;
        }
    }
    return found;
}

void Multiplayermenu::showRuleSelection()
{    
    m_pRuleSelection->setVisible(true);
    m_pButtonSaveRules->setVisible(true);
    m_pButtonLoadRules->setVisible(true);
    m_pRuleSelection->clearContent();
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    RuleSelection::Mode ruleMode = RuleSelection::Mode::Multiplayer;
    if (!m_sameVersionAsServer)
    {
        ruleMode = RuleSelection::Mode::MultiplayerOnlyGateway;
    }
    m_pRuleSelectionView = MemoryManagement::create<RuleSelection>(pMap.get(), oxygine::Stage::getStage()->getWidth() - 80, ruleMode);
    connect(m_pRuleSelectionView.get(), &RuleSelection::sigSizeChanged, this, &Multiplayermenu::ruleSelectionSizeChanged, Qt::QueuedConnection);
    m_pRuleSelection->addItem(m_pRuleSelectionView);
    m_pRuleSelection->setContentHeigth(m_pRuleSelectionView->getScaledHeight() + 40);
    m_pRuleSelection->setContentWidth(m_pRuleSelectionView->getScaledWidth());
}

void Multiplayermenu::showPlayerSelection(bool relaunchedLobby)
{
    MapSelectionMapsMenue::showPlayerSelection(relaunchedLobby);
    if (m_networkMode == NetworkMode::Observer)
    {
        m_pButtonStart->setVisible(false);
    }
}

void Multiplayermenu::exitMenu()
{
    CONSOLE_PRINT("Leaving Map Selection Menue and going back to main menu", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    spMainwindow window = MemoryManagement::create<Mainwindow>("ui/menu/mainmenu.xml");
    oxygine::Stage::getStage()->addChild(window);
    oxygine::Actor::detach();
}

void Multiplayermenu::exitMenuToLobby()
{
    CONSOLE_PRINT("Leaving Map Selection Menue and going back to lobby menu", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    disconnectNetwork();
    oxygine::Stage::getStage()->addChild(MemoryManagement::create<LobbyMenu>());
    oxygine::Actor::detach();
}

void Multiplayermenu::disconnected(quint64 socket)
{
    CONSOLE_PRINT("Multiplayermenu::disconnected", GameConsole::eDEBUG);
    if (Mainapp::getSlave())
    {
        if (m_pPlayerSelection.get() != nullptr)
        {
            m_pPlayerSelection->sendOpenPlayerCount();
        }
        if (m_pNetworkInterface->getConnectedSockets().size() == 0)
        {
            startDespawnTimer();
        }
    }
    else if (m_networkMode == NetworkMode::Host && m_local)
    {
        // do nothing
    }
    else
    {
        emit sigServerResponded();
        CONSOLE_PRINT("Leaving Map Selection Menue", GameConsole::eDEBUG);
        m_onEnterTimer.stop();
        disconnectNetwork();
        oxygine::Stage::getStage()->addChild(MemoryManagement::create<LobbyMenu>());
        oxygine::Actor::detach();
    }
}

void Multiplayermenu::startDespawnTimer()
{
    if (Mainapp::getSlave())
    {
        CONSOLE_PRINT("Multiplayermenu::startDespawnTimer", GameConsole::eDEBUG);
        m_slaveDespawnElapseTimer.start();
        m_slaveDespawnTimer.setSingleShot(false);
        m_despawning = false;
        constexpr qint32 MS_PER_SECOND = 1000;
        m_slaveDespawnTimer.start(MS_PER_SECOND);
    }
}

void Multiplayermenu::buttonBack()
{
    if (m_networkMode == NetworkMode::Host && !m_local && m_MapSelectionStep == MapSelectionStep::selectPlayer)
    {
        QString command = NetworkCommands::DESPAWNSLAVE;
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        QJsonDocument doc(data);
        CONSOLE_PRINT("Sending command " + command + " to slave", GameConsole::eDEBUG);
        showInformingServer();
        emit m_pNetworkInterface->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);        
    }
    else if (m_networkMode != NetworkMode::Host ||
        m_MapSelectionStep == MapSelectionStep::selectMap ||
        !m_local)
    {
        CONSOLE_PRINT("Leaving Map Selection Menue button back pressed", GameConsole::eDEBUG);
        exitMenuToLobby();
    }
    else if (m_networkMode == NetworkMode::Host)
    {
        m_pHostAdresse->setVisible(false);
        if (m_Chat.get() != nullptr)
        {
            m_Chat->detach();
            m_Chat.reset();
        }
        disconnectNetwork();
        MapSelectionMapsMenue::buttonBack();
        if (m_saveGame)
        {
            MapSelectionMapsMenue::buttonBack();
            m_saveGame = false;
            m_pPlayerSelection->setSaveGame(m_saveGame);
        }
    }
}

void Multiplayermenu::showInformingServer()
{
    spDialogConnecting pDialogConnecting = MemoryManagement::create<DialogConnecting>(tr("Informing server"), 1000 * 60 * 5, false);
    addChild(pDialogConnecting);
    connect(this, &Multiplayermenu::sigServerResponded, pDialogConnecting.get(), &DialogConnecting::connected, Qt::QueuedConnection);
}

void Multiplayermenu::buttonNext()
{
    if (m_networkMode == NetworkMode::Host &&
        m_MapSelectionStep == MapSelectionStep::selectRules)
    {
        spGameMap pMap = m_pMapSelectionView->getCurrentMap();
        m_password.setPassword(pMap->getGameRules()->getPassword());
        if (m_local)
        {
            m_pNetworkInterface = MemoryManagement::create<TCPServer>(nullptr);
            m_pNetworkInterface->moveToThread(Mainapp::getInstance()->getNetworkThread());
            m_pPlayerSelection->attachNetworkInterface(m_pNetworkInterface);
            createChat();
            connectNetworkSlots();
            emit m_pNetworkInterface->sig_connect("", Settings::getInstance()->getGamePort(), "");
            MapSelectionMapsMenue::buttonNext();
        }
        else
        {
            // m_MapSelectionStep = MapSelectionStep::selectPlayer;
            MapSelectionMapsMenue::hideRuleSelection();
            if (pMap->getGameRules()->getGatewayHosting())
            {
                connectNetworkSlots();
            }
            else
            {
                m_pReadyAndLeave->setVisible(true);
                connectNetworkSlots();
                startGameOnServer();
            }
        }
    }
    else
    {
        MapSelectionMapsMenue::buttonNext();
        if (m_MapSelectionStep == MapSelectionStep::selectRules)
        {
            m_pMapSelectionView->getCurrentMap()->getGameRules()->setPassword("");
        }
    }
}

void Multiplayermenu::connectNetworkSlots()
{
    CONSOLE_PRINT("Multiplayermenu::connectNetworkSlots", GameConsole::eDEBUG);
    disconnectNetworkSlots();
    connect(m_pNetworkInterface.get(), &NetworkInterface::sigConnected, this, &Multiplayermenu::playerJoined, Qt::QueuedConnection);
    connect(m_pNetworkInterface.get(), &NetworkInterface::recieveData, this, &Multiplayermenu::recieveData, NetworkCommands::UNIQUE_DATA_CONNECTION);
    connect(m_pNetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &Multiplayermenu::disconnected, Qt::QueuedConnection);
}

void Multiplayermenu::disconnectNetworkSlots()
{
    CONSOLE_PRINT("Multiplayermenu::disconnectNetworkSlots", GameConsole::eDEBUG);
    disconnect(m_pNetworkInterface.get(), &NetworkInterface::sigConnected, this, &Multiplayermenu::playerJoined);
    disconnect(m_pNetworkInterface.get(), &NetworkInterface::recieveData, this, &Multiplayermenu::recieveData);
    disconnect(m_pNetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &Multiplayermenu::disconnected);
}

void Multiplayermenu::startGameOnServer()
{
    Mainapp* pApp = Mainapp::getInstance();
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    QString command = QString(NetworkCommands::LAUNCHGAMEONSERVER);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream.setVersion(QDataStream::Version::Qt_6_5);
    sendStream << command;
    QStringList myVersions = Settings::getInstance()->getActiveModVersions();
    QStringList myMods = Settings::getInstance()->getMods();
    Settings::getInstance()->filterCosmeticMods(myMods, myVersions, pMap->getGameRules()->getCosmeticModsAllowed());
    Filesupport::writeVectorList(sendStream, myMods);
    QImage img;
    pApp->saveMapAsImage(m_pMapSelectionView->getMinimap(), &img);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "PNG"); // writes image into ba in PNG format
    Filesupport::writeByteArray(sendStream, ba);

    pMap->serializeObject(sendStream);
    sendStream << m_saveGame;
    emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::ServerHosting, false);
    waitForServerConnection();
}

void Multiplayermenu::startGatewayGameOnServer()
{
    Mainapp* pApp = Mainapp::getInstance();
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    QString command = QString(NetworkCommands::LAUNCHGATEWAYGAMEONSERVER);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    NetworkGameData gameData;
    gameData.setMapName(pMap->getMapName());
    gameData.setDescription(pMap->getMapDescription());
    gameData.setMods(Settings::getInstance()->getMods());
    gameData.setMaxPlayers(pMap->getPlayerCount());
    gameData.setMaxObservers(pMap->getGameRules()->getMultiplayerObserver());
    gameData.setLocked(pMap->getGameRules()->getPassword().getIsSet());
    gameData.setGameVersion(Mainapp::getGameVersion());
    QImage img;
    pApp->saveMapAsImage(m_pMapSelectionView->getMinimap(), &img);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "PNG"); // writes image into ba in PNG format
    gameData.setMinimapData(ba);
    data.insert(JsonKeys::JSONKEY_GAMEDATA, gameData.toJson());
    QJsonDocument doc(data);
    emit m_pNetworkInterface->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    waitForServerConnection();
}

void Multiplayermenu::waitForServerConnection()
{
    m_pDialogConnecting = MemoryManagement::create<DialogConnecting>(tr("Launching game on server"), 1000 * 60 * 5);
    addChild(m_pDialogConnecting);
    connect(m_pDialogConnecting.get(), &DialogConnecting::sigCancel, this, &Multiplayermenu::slotCancelHostConnection, Qt::QueuedConnection);
    connect(m_pNetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &Multiplayermenu::slotCancelHostConnection, Qt::QueuedConnection);
    connect(this, &Multiplayermenu::sigHostGameLaunched, m_pDialogConnecting.get(), &DialogConnecting::connected, Qt::QueuedConnection);
    connect(m_pDialogConnecting.get(), &DialogConnecting::sigConnected, this, &Multiplayermenu::slotHostGameLaunched);
}

void Multiplayermenu::createChat()
{
    if (m_Chat.get() != nullptr)
    {
        m_Chat->detach();
    }
    if (Settings::getInstance()->getSmallScreenDevice())
    {
        m_Chat = MemoryManagement::create<Chat>(m_pNetworkInterface,
                                QSize(oxygine::Stage::getStage()->getWidth() - 60, oxygine::Stage::getStage()->getHeight() - 90),
                                NetworkInterface::NetworkSerives::GameChat, nullptr);
        m_Chat->setPosition(-m_Chat->getScaledWidth() + 1, 10);
        auto moveButton = MemoryManagement::create<MoveInButton>(m_Chat.get(), m_Chat->getScaledWidth(), 1, -1, 1.0f);
        m_Chat->addChild(moveButton);
    }
    else
    {
        m_Chat = MemoryManagement::create<Chat>(m_pNetworkInterface,
                                QSize(oxygine::Stage::getStage()->getWidth() - 20, 150),
                                NetworkInterface::NetworkSerives::GameChat, nullptr);
        m_Chat->setPosition(10, oxygine::Stage::getStage()->getHeight() - 210);
    }
    addChild(m_Chat);    
}

void Multiplayermenu::disconnectNetwork()
{    
    m_GameStartTimer.stop();
    if (m_pNetworkInterface.get() != nullptr)
    {
        if (m_Chat.get())
        {
            m_Chat->detach();
            m_Chat.reset();
        }
        m_pPlayerSelection->attachNetworkInterface(spNetworkInterface());
        m_pNetworkInterface.reset();
    }    
}

bool Multiplayermenu::getGameReady()
{
    bool gameReady = true;
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    bool hasRemotePlayer = false;
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        auto* pInput = pMap->getPlayer(i)->getBaseGameInput();
        GameEnums::AiTypes aiType = m_pPlayerSelection->getPlayerAiType(i);
        if (aiType == GameEnums::AiTypes_Open)
        {
            CONSOLE_PRINT("Game not ready cause player " + QString::number(i) + " is open", GameConsole::eDEBUG);
            gameReady = false;
            break;
        }
        else if (aiType == GameEnums::AiTypes_ProxyAi ||
                 (pInput != nullptr &&
                  pInput->getAiType() == GameEnums::AiTypes_ProxyAi))
        {
            if (m_pPlayerSelection->getReady(i) == false)
            {
                CONSOLE_PRINT("Game not ready cause proxy player " + QString::number(i) + " is not ready", GameConsole::eDEBUG);
                gameReady = false;
                break;
            }
        }
        if (aiType == GameEnums::AiTypes_ProxyAi ||
            (pInput != nullptr &&
             pInput->getAiType() == GameEnums::AiTypes_ProxyAi))
        {
            hasRemotePlayer = true;
        }
    }
    CONSOLE_PRINT("Game ready " + QString::number(gameReady) + " and remote player found " + QString::number(hasRemotePlayer), GameConsole::eDEBUG);
    return gameReady && hasRemotePlayer;
}

void Multiplayermenu::startGame()
{
    if (m_networkMode != NetworkMode::Host)
    {
        markGameReady();
        changeButtonText();
    }
    else if (m_local)
    {
        if (getGameReady())
        {
            markGameReady();
            startCountdown();
        }
    }
    else
    {
        markGameReady();
        changeButtonText();        
    }
}

void Multiplayermenu::markGameReady(bool fixed)
{
    QString command = QString(NetworkCommands::CLIENTREADY);
    m_pPlayerSelection->setPlayerReady(!m_pPlayerSelection->getPlayerReady());
    bool ready = m_pPlayerSelection->getPlayerReady();
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream.setVersion(QDataStream::Version::Qt_6_5);
    sendStream << command;
    sendStream << fixed;
    sendStream << ready;
    CONSOLE_PRINT("Sending command " + command + " with fixed=" + QString::number(fixed) + " and ready=" + QString::number(ready), GameConsole::eDEBUG);
    emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
}

void Multiplayermenu::changeButtonText()
{
    Mainapp::getInstance()->pauseRendering();
    if (m_pPlayerSelection->getPlayerReady())
    {
        oxygine::safeSpCast<Label>(m_pButtonStart->getFirstChild())->setHtmlText(tr("Not Ready"));
    }
    else
    {
        oxygine::safeSpCast<Label>(m_pButtonStart->getFirstChild())->setHtmlText(tr("Ready"));
    }
    Mainapp::getInstance()->continueRendering();
}

void Multiplayermenu::startCountdown()
{
    m_counter = 5;
    // can we start the game?
    if (getGameReady())
    {
        if (!m_GameStartTimer.isActive())
        {
            CONSOLE_PRINT("Starting countdown", GameConsole::eDEBUG);
            sendServerReady(true);
            m_GameStartTimer.setInterval(std::chrono::seconds(1));
            m_GameStartTimer.setSingleShot(false);
            m_GameStartTimer.start();
            emit m_Chat->sigSendText(QString::number(m_counter) + "...");
        }
    }
    else
    {
        CONSOLE_PRINT("Stopping countdown", GameConsole::eDEBUG);
        m_GameStartTimer.stop();
        if (m_local)
        {
            sendServerReady(false);
        }
    }
}

void Multiplayermenu::sendServerReady(bool value)
{
    if (m_pNetworkInterface->getIsServer())
    {
        if (value)
        {
            emit m_pNetworkInterface->sigPauseListening();
        }
        else
        {
            emit m_pNetworkInterface->sigContinueListening();
        }
        CONSOLE_PRINT("Setting player ready information to local players with value "  + QString::number(value), GameConsole::eDEBUG);
        m_pPlayerSelection->setPlayerReady(value);
        CONSOLE_PRINT("Sending ready information to all players with value " + QString::number(value), GameConsole::eDEBUG);
        m_pPlayerSelection->sendPlayerReady(0);
    }
}

void Multiplayermenu::countdown()
{
    if (getGameReady())
    {
        --m_counter;
        if (m_Chat.get() != nullptr)
        {
            CONSOLE_PRINT("Sending game counter..." + QString::number(m_counter), GameConsole::eDEBUG);
            emit m_Chat->sigSendText(QString::number(m_counter) + "...");
        }
        if (m_counter == 0 && m_pNetworkInterface.get() != nullptr)
        {
            CONSOLE_PRINT("Starting game on server", GameConsole::eDEBUG);
            defeatClosedPlayers();

            spGameMap pMap = m_pMapSelectionView->getCurrentMap();
            pMap->setVisible(false);
            if (!m_saveGame)
            {
                pMap->initPlayersAndSelectCOs();
            }
            QString command = QString(NetworkCommands::INITGAME);
            CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Version::Qt_6_5);
            stream << command;
            quint32 seed = QRandomGenerator::global()->bounded(std::numeric_limits<quint32>::max());
            GlobalUtils::seed(seed);
            GlobalUtils::setUseSeed(true);
            stream << seed;
            for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getPlayerCount(); i++)
            {
                CONSOLE_PRINT("AI on server for player " + QString::number(i) + " is " + QString::number(pMap->getPlayer(i)->getBaseGameInput()->getAiType()), GameConsole::eDEBUG);
                pMap->getPlayer(i)->serializeObject(stream);
            }
            if (!m_saveGame)
            {
                pMap->getGameScript()->gameStart();
            }
            bool applyRulesPalette = pMap->getGameRules()->getMapPalette() > 0;
            pMap->updateSprites(-1, -1, false, false, applyRulesPalette);
            // start game
            CONSOLE_PRINT("Leaving Map Selection Menue after countdown", GameConsole::eDEBUG);
            m_onEnterTimer.stop();
            auto window = MemoryManagement::create<GameMenue>(pMap, m_saveGame, m_pNetworkInterface, false);
            oxygine::Stage::getStage()->addChild(window);
            CONSOLE_PRINT("Suspending thread shortly before informing slaves to launch game.", GameConsole::eDEBUG);
            QThread::msleep(200);
            CONSOLE_PRINT("Sending init game to clients", GameConsole::eDEBUG);
            emit m_pNetworkInterface->sig_sendData(0, data, NetworkInterface::NetworkSerives::Multiplayer, false);
            oxygine::Actor::detach();
        }
    }
    else
    {
        m_counter = 5;
        m_GameStartTimer.stop();
        sendServerReady(false);
    }
}
