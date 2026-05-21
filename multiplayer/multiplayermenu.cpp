#include <QCoreApplication>
#include <QCryptographicHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "multiplayer/multiplayermenu.h"
#include "multiplayer/lobbymenu.h"
#include "multiplayer/networkcommands.h"

#include "coreengine/mainapp.h"
#include "coreengine/gameconsole.h"
#include "coreengine/settings.h"
#include "coreengine/filesupport.h"
#include "coreengine/globalutils.h"
#include "coreengine/virtualpaths.h"

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
      m_password(password),
      m_serverAddress(address),
      m_serverPort(port)
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
      m_password(*password),
      m_serverAddress(address),
      m_serverPort(port)
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
    m_pJoinConnectingDialog = MemoryManagement::create<DialogConnecting>(tr("Connecting"), 1000 * 60 * 5);
    addChild(m_pJoinConnectingDialog);
    connect(m_pJoinConnectingDialog.get(), &DialogConnecting::sigCancel, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
    connect(this, &Multiplayermenu::sigConnected, m_pJoinConnectingDialog.get(), &DialogConnecting::connected, Qt::QueuedConnection);
    // Drop the smart-ptr after the dialog's own connected() slot has detached it, so a retained reference can't keep the actor alive past the lobby join.
    connect(this, &Multiplayermenu::sigConnected, this, [this](){ m_pJoinConnectingDialog.reset(); }, Qt::QueuedConnection);
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
    GameVersion gameVersion;
    data.insert(JsonKeys::JSONKEY_VERSION_MAJOR, gameVersion.getMajor());
    data.insert(JsonKeys::JSONKEY_VERSION_MINOR, gameVersion.getMinor());
    data.insert(JsonKeys::JSONKEY_VERSION_REVISION, gameVersion.getRevision());
    data.insert(JsonKeys::JSONKEY_VERSION_SUFIX, gameVersion.getSufix());
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
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    bool gatewayHosting = pMap.get() != nullptr && pMap->getGameRules()->getGatewayHosting();
    if (m_pNetworkInterface->getIsServer() &&
        (m_local || Mainapp::getSlave() || gatewayHosting))
    {
        CONSOLE_PRINT("Multiplayermenu::playerJoined " + QString::number(socketID) , GameConsole::eDEBUG);
        acceptNewConnection(socketID);
        if (m_pPlayerSelection.get() != nullptr)
        {
            m_pPlayerSelection->sendOpenPlayerCount();
        }
        if (gatewayHosting)
        {
            emit sigHostGameLaunched();
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
        else if (messageType == NetworkCommands::REQUESTMODSYNC)
        {
            handleModSyncRequest(stream, socketID);
        }
        else if (messageType == NetworkCommands::MODSYNCMANIFEST)
        {
            handleModSyncManifest(stream, socketID);
        }
        else if (messageType == NetworkCommands::MODSYNCDATA)
        {
            handleModSyncData(stream, socketID);
        }
        else if (messageType == NetworkCommands::MODSYNCMODBEGIN)
        {
            handleModSyncModBegin(stream, socketID);
        }
        else if (messageType == NetworkCommands::MODSYNCMODCHUNK)
        {
            handleModSyncModChunk(stream, socketID);
        }
        else if (messageType == NetworkCommands::MODSYNCMODEND)
        {
            handleModSyncModEnd(stream, socketID);
        }
        else if (messageType == NetworkCommands::MODSYNCREJECT)
        {
            handleModSyncReject(stream, socketID);
        }
        else if (messageType == NetworkCommands::MODSYNCCOMPLETE)
        {
            handleModSyncComplete(stream, socketID);
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
    else
    {
        CONSOLE_PRINT("Unknown service in Multiplayermenu::recieveData " + QString::number(static_cast<qint32>(service)) + " received", GameConsole::eDEBUG);
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
    GameVersion().serializeObject(stream);
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
    quint32 capabilities = 0;
    if (Settings::getInstance()->getModSyncEnabled())
    {
        capabilities |= Filesupport::CapabilityModSync;
    }
    // Stay on parent v1 wire format when no caps advertised so v1 clients still join.
    if (capabilities == 0)
    {
        stream << static_cast<qint32>(Filesupport::LegacyHashPayloadVersion);
    }
    else
    {
        stream << static_cast<qint32>(Filesupport::CurrentHashPayloadVersion);
        stream << capabilities;
    }
    Filesupport::writeMap(stream, Filesupport::getResourceFolderHashes());
    Filesupport::writeMap(stream, Filesupport::getPerModHashes(mods));
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
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    bool isServer = m_pNetworkInterface->getIsServer();
    bool isGateway = pMap->getGameRules()->getGatewayHosting();
    disconnectNetworkSlots();
    m_pNetworkInterface = MemoryManagement::create<TCPClient>(nullptr);
    m_pNetworkInterface->setIsObserver(m_networkMode == NetworkMode::Observer);
    m_pNetworkInterface->moveToThread(Mainapp::getInstance()->getNetworkThread());
    if (isGateway)
    {
        Mainapp::getInstance()->setSlaveClient(m_pNetworkInterface);
        m_pNetworkInterface->setIsServer(isServer);
        Settings::getInstance()->setSlaveServerName(objData.value(JsonKeys::JSONKEY_SLAVENAME).toString());
    }
    m_pPlayerSelection->attachNetworkInterface(m_pNetworkInterface);
    createChat();
    connectNetworkSlots();
    emit m_pNetworkInterface->sig_connect(address, port, secondarySlaveAddress, isGateway);
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
        QStringList mismatchedResourceFolders;
        QStringList mismatchedMods;
        quint32 hostCapabilities = 0;
        bool cosmeticAllowed = false;
        QMap<QString, QByteArray> hostModHashes;
        readHashInfo(stream, socketID, mods, versions, myMods, myVersions, mismatchedResourceFolders, mismatchedMods, hostModHashes, hostCapabilities, sameMods, differentHash, sameVersion, cosmeticAllowed);
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
            handleVersionMissmatch(mods, versions, myMods, myVersions, mismatchedResourceFolders, mismatchedMods, hostModHashes, hostCapabilities, sameMods, differentHash, sameVersion, cosmeticAllowed);
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

void Multiplayermenu::readHashInfo(QDataStream & stream, quint64 socketID, QStringList & mods, QStringList & versions, QStringList & myMods, QStringList & myVersions, QStringList & mismatchedResourceFolders, QStringList & mismatchedMods, QMap<QString, QByteArray> & hostModHashes, quint32 & hostCapabilities, bool & sameMods, bool & differentHash, bool & sameVersion, bool & cosmeticAllowed)
{
    hostCapabilities = 0;
    cosmeticAllowed = false;
    hostModHashes.clear();
    GameVersion version;
    version.deserializeObject(stream);
    sameVersion = (version == GameVersion());
    if (!sameVersion)
    {
        return;
    }
    bool filter = false;
    stream >> filter;
    cosmeticAllowed = filter;
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
    qint32 sentinel = 0;
    stream >> sentinel;

    // Call once per readHashInfo: appends to mismatch lists without clearing.
    auto compareMaps = [&](const QMap<QString, QByteArray> & hostResources, const QMap<QString, QByteArray> & hostMods)
    {
        // Surface host's per-mod hash for the inactive-local-copy verification in handleVersionMissmatch.
        hostModHashes = hostMods;
        auto ownResources = Filesupport::getResourceFolderHashes();
        auto ownMods = Filesupport::getPerModHashes(myMods);
        for (auto iter = hostResources.constBegin(); iter != hostResources.constEnd(); ++iter)
        {
            if (ownResources.value(iter.key()) != iter.value())
            {
                mismatchedResourceFolders.append(iter.key());
            }
        }
        for (const auto & mod : std::as_const(mods))
        {
            // Mods only on one side belong in the membership-mismatch section, not here.
            if (!myMods.contains(mod))
            {
                continue;
            }
            if (hostMods.value(mod) != ownMods.value(mod))
            {
                mismatchedMods.append(mod);
            }
        }
        differentHash = !mismatchedResourceFolders.isEmpty() || !mismatchedMods.isEmpty();
    };

    if (sentinel == Filesupport::LegacyRuntimeHashSize)
    {
        QByteArray hostRuntime;
        for (qint32 i = 0; i < sentinel; ++i)
        {
            qint8 byte = 0;
            stream >> byte;
            hostRuntime.append(byte);
        }
        QByteArray ownRuntime = Filesupport::getLegacyRuntimeHash(mods);
        if (GameConsole::eDEBUG >= GameConsole::getLogLevel())
        {
            CONSOLE_PRINT("Received legacy host hash: " + GlobalUtils::getByteArrayString(hostRuntime), GameConsole::eDEBUG);
            CONSOLE_PRINT("Own legacy hash:           " + GlobalUtils::getByteArrayString(ownRuntime), GameConsole::eDEBUG);
        }
        differentHash = (hostRuntime != ownRuntime);
    }
    else if (sentinel == Filesupport::LegacyHashPayloadVersion)
    {
        // parent named-mod-mismatch wire format: two maps, no capabilities advertised.
        auto hostResources = Filesupport::readMap<QString, QByteArray, QMap>(stream);
        auto hostMods = Filesupport::readMap<QString, QByteArray, QMap>(stream);
        compareMaps(hostResources, hostMods);
    }
    else if (sentinel == Filesupport::CurrentHashPayloadVersion)
    {
        stream >> hostCapabilities;
        auto hostResources = Filesupport::readMap<QString, QByteArray, QMap>(stream);
        auto hostMods = Filesupport::readMap<QString, QByteArray, QMap>(stream);
        compareMaps(hostResources, hostMods);
    }
    else
    {
        CONSOLE_PRINT("Unknown hash payload sentinel " + QString::number(sentinel) + ", failing closed", GameConsole::eERROR);
        differentHash = true;
    }
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
        QStringList mismatchedResourceFolders;
        QStringList mismatchedMods;
        quint32 hostCapabilities = 0;
        bool cosmeticAllowed = false;
        QMap<QString, QByteArray> hostModHashes;
        readHashInfo(stream, socketID, mods, versions, myMods, myVersions, mismatchedResourceFolders, mismatchedMods, hostModHashes, hostCapabilities, sameMods, differentHash, sameVersion, cosmeticAllowed);
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
            handleVersionMissmatch(mods, versions, myMods, myVersions, mismatchedResourceFolders, mismatchedMods, hostModHashes, hostCapabilities, sameMods, differentHash, sameVersion, cosmeticAllowed);
        }
    }
}

void Multiplayermenu::handleVersionMissmatch(const QStringList & mods, const QStringList & versions, const QStringList & myMods, const QStringList & myVersions, const QStringList & mismatchedResourceFolders, const QStringList & mismatchedMods, const QMap<QString, QByteArray> & hostModHashes, quint32 hostCapabilities, bool sameMods, bool differentHash, bool sameVersion, bool cosmeticAllowed)
{
    // Mod/hash fields are stale on version mismatch because readHashInfo early-returns.
    if (!sameVersion)
    {
        spDialogMessageBox pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Host has a different game version. Leaving the game again."));
        connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
        addChild(pDialogMessageBox);
        return;
    }

    constexpr qint32 DISPLAY_LIMIT = 5;
    auto * settings = Settings::getInstance();

    QStringList missingHere;
    QStringList extraHere;
    QStringList versionDiffs;
    QStringList modsToDownloadPaths;
    if (!sameMods)
    {
        for (qint32 i = 0; i < mods.size(); ++i)
        {
            const QString & mod = mods[i];
            const qint32 j = myMods.indexOf(mod);
            if (j < 0)
            {
                missingHere.append(settings->getModName(mod) + " " + versions[i]);
                // Skip download only when local hash and version match host; disk presence alone is unsafe (stale or unrelated folder).
                bool localSatisfies = false;
                const QString resolvedAbs = VirtualPaths::find(mod, false);
                if (!resolvedAbs.isEmpty() && QDir(resolvedAbs).exists())
                {
                    const QByteArray localHash = Filesupport::getPerModHashes(QStringList{mod}).value(mod);
                    const QString localVersion = settings->getModVersion(mod);
                    localSatisfies = (!localHash.isEmpty() && localHash == hostModHashes.value(mod) && localVersion == versions[i]);
                    if (!localSatisfies)
                    {
                        CONSOLE_PRINT("Inactive local copy of " + mod + " differs from host (hash or version); queueing download.", GameConsole::eDEBUG);
                    }
                }
                if (!localSatisfies)
                {
                    modsToDownloadPaths.append(mod);
                }
            }
            else if (versions[i] != myVersions[j])
            {
                versionDiffs.append(tr("%1 (host: %2, you: %3)").arg(settings->getModName(mod), versions[i], myVersions[j]));
                modsToDownloadPaths.append(mod);
            }
        }
        for (qint32 i = 0; i < myMods.size(); ++i)
        {
            if (!mods.contains(myMods[i]))
            {
                extraHere.append(settings->getModName(myMods[i]) + " " + myVersions[i]);
            }
        }
    }

    QStringList contentDiffs;
    for (const auto & mod : std::as_const(mismatchedMods))
    {
        contentDiffs.append(settings->getModName(mod));
        if (!modsToDownloadPaths.contains(mod))
        {
            modsToDownloadPaths.append(mod);
        }
    }

    auto logFullList = [](const QString & label, const QStringList & list)
    {
        if (!list.isEmpty())
        {
            CONSOLE_PRINT(label + ": " + list.join(", "), GameConsole::eINFO);
        }
    };
    logFullList(QStringLiteral("Mods host has that you are missing"), missingHere);
    logFullList(QStringLiteral("Mods you have that host does not"), extraHere);
    logFullList(QStringLiteral("Mods with version-string mismatch"), versionDiffs);
    logFullList(QStringLiteral("Mods with different content"), contentDiffs);
    logFullList(QStringLiteral("Engine resource folders modified"), mismatchedResourceFolders);

    auto appendSection = [&](QString & dst, const QString & header, const QStringList & lines)
    {
        if (lines.isEmpty())
        {
            return;
        }
        dst += header + "\n";
        const qint32 shown = std::min(static_cast<qint32>(lines.size()), DISPLAY_LIMIT);
        for (qint32 i = 0; i < shown; ++i)
        {
            dst += "  " + lines[i] + "\n";
        }
        if (lines.size() > DISPLAY_LIMIT)
        {
            dst += "  " + tr("...and %1 more (see console.log)").arg(lines.size() - DISPLAY_LIMIT) + "\n";
        }
        dst += "\n";
    };

    QString message;
    appendSection(message, tr("Missing mods (host has, you don't):"), missingHere);
    appendSection(message, tr("Extra mods (you have, host doesn't):"), extraHere);
    appendSection(message, tr("Version mismatch (mod.txt):"), versionDiffs);
    appendSection(message, tr("Content mismatch:"), contentDiffs);
    appendSection(message, tr("Engine resources differ:"), mismatchedResourceFolders);

    // Mod-sync is offerable when host advertised CapabilityModSync, no engine resource drift, and any mismatch class is non-empty (downloads OR settings-only activate/deactivate work).
    const bool hostSupportsModSync = (hostCapabilities & Filesupport::CapabilityModSync) != 0;
    const bool resourceDrift = !mismatchedResourceFolders.isEmpty();
    const bool fixableViaSync = hostSupportsModSync && !resourceDrift && (!missingHere.isEmpty() || !versionDiffs.isEmpty() || !contentDiffs.isEmpty() || !extraHere.isEmpty());

    if (message.isEmpty())
    {
        // Legacy and fail-closed payloads have no structured detail.
        if (differentHash)
        {
            message = tr("Host has a different version of a mod or the game resource folder has been modified by one of the games.");
        }
        else
        {
            CONSOLE_PRINT("handleVersionMissmatch reached unreachable branch: !differentHash with no mod-set or hash diff detail. checkMods set !sameMods but our classification found nothing. Investigate.", GameConsole::eERROR);
            message = tr("Failed to join game due to unknown verification failure.");
        }
    }
    else if (fixableViaSync)
    {
        message = tr("Your game data differs from the host:") + "\n\n" + message + tr("Want me to download host's mod set, apply it, and restart automatically?");
    }
    else
    {
        message = tr("Cannot join, your game data differs from the host:") + "\n\n" + message + tr("Leaving the game again.");
    }

    if (fixableViaSync)
    {
        spDialogMessageBox pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(message, true, tr("Apply"), tr("Leave game"));
        // Host's advertised list is already cosmetic-filtered when the rule allows them; re-add client cosmetic-only mods so the user does not silently lose them on next boot.
        QStringList postSyncActiveMods = mods;
        if (cosmeticAllowed)
        {
            const QStringList clientFull = settings->getMods();
            for (const auto & mod : std::as_const(clientFull))
            {
                if (!postSyncActiveMods.contains(mod) && settings->getIsCosmetic(mod))
                {
                    postSyncActiveMods.append(mod);
                }
            }
        }
        connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, [this, modsToDownloadPaths, postSyncActiveMods]()
        {
            confirmModSync(modsToDownloadPaths, postSyncActiveMods);
        }, Qt::QueuedConnection);
        connect(pDialogMessageBox.get(), &DialogMessageBox::sigCancel, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
        addChild(pDialogMessageBox);
    }
    else
    {
        spDialogMessageBox pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(message);
        connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
        addChild(pDialogMessageBox);
    }
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
    if (m_pNetworkInterface.get() != nullptr)
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
    if (m_local)
    {
        ruleMode = RuleSelection::Mode::MultiplayerLocal;
    }
    else if (!m_sameVersionAsServer && !m_local)
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
        !m_local ||
        isGatewayGame())
    {
        CONSOLE_PRINT("Leaving Map Selection Menue button back pressed", GameConsole::eDEBUG);
        disconnectGateway();
        exitMenuToLobby();
    }
    else if (m_networkMode == NetworkMode::Host)
    {
        if (m_pHostAdresse.get() != nullptr)
        {
            m_pHostAdresse->setVisible(false);
        }
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
            if (m_pPlayerSelection.get() != nullptr)
            {
                m_pPlayerSelection->setSaveGame(m_saveGame);
            }
        }
    }
}

void Multiplayermenu::disconnectGateway()
{
    if (!Mainapp::getSlave())
    {
        spNetworkInterface emptyClient;
        Mainapp::getInstance()->setSlaveClient(emptyClient);
        if (isGatewayGame())
        {
            m_local = false;
        }
    }
}

bool Multiplayermenu::isGatewayGame()
{
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    if (pMap.get() != nullptr &&
        pMap->getGameRules()->getGatewayHosting())
    {
        return true;
    }
    return false;
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
                startGatewayGameOnServer();
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
    if (m_pNetworkInterface.get() != nullptr)
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
        gameData.setGameVersion(GameVersion());
        QImage img;
        pApp->saveMapAsImage(m_pMapSelectionView->getMinimap(), &img);
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        img.save(&buffer, "PNG"); // writes image into ba in PNG format
        gameData.setMinimapData(ba);
        data.insert(JsonKeys::JSONKEY_GAMEDATA, gameData.toJson());
        QJsonDocument doc(data);
        m_pNetworkInterface->setIsServer(true);
        m_local = true;
        emit m_pNetworkInterface->sig_sendData(m_pNetworkInterface->getSocketID(), doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
        waitForServerConnection();
    }
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
            emit m_pNetworkInterface->sig_sendData(0, data, NetworkInterface::NetworkSerives::Multiplayer, true);
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

// Pin reject codes to wire literals; filesupport.cpp's anonymous-namespace constants must match.
static_assert(static_cast<qint32>(NetworkCommands::ModSyncNoReason) == 0, "ModSync reject code drift");
static_assert(static_cast<qint32>(NetworkCommands::ModSyncDisabled) == 1, "ModSync reject code drift");
static_assert(static_cast<qint32>(NetworkCommands::ModSyncUnknownMod) == 2, "ModSync reject code drift");
static_assert(static_cast<qint32>(NetworkCommands::ModSyncSizeCapExceeded) == 3, "ModSync reject code drift");
static_assert(static_cast<qint32>(NetworkCommands::ModSyncFileCountCapExceeded) == 4, "ModSync reject code drift");
static_assert(static_cast<qint32>(NetworkCommands::ModSyncInvalidPath) == 5, "ModSync reject code drift");
static_assert(static_cast<qint32>(NetworkCommands::ModSyncInternalError) == 6, "ModSync reject code drift");
static_assert(static_cast<qint32>(NetworkCommands::ModSyncBusy) == 7, "ModSync reject code drift");

namespace
{
    // Cap on number of mods in a single REQUESTMODSYNC; client mod count is bounded by what the user has installed.
    constexpr qint32 MOD_SYNC_REQUEST_COUNT_MAX = 1024;
    // Cap on reject-message char length; anything longer than this is truncated by the host or rejected.
    constexpr qint32 MOD_SYNC_REASON_CHARS_MAX = 4096;
    // Wire contract: host and client must agree on this value; chunkCount and per-chunk size are validated against it. Bump MODSYNCMODBEGIN's protocolVersion if it ever changes.
    constexpr qint32 MOD_SYNC_CHUNK_BYTES = 1 * 1024 * 1024;
    // Hard ceiling on chunkCount declared in MODSYNCMODBEGIN; even at perModCap = 1 GB that's only ~1024 chunks at 1 MiB.
    constexpr qint32 MOD_SYNC_CHUNK_COUNT_MAX = 1024 * 1024;

    // QDataStream::operator>> pre-resizes to the declared length; bounded readers reject the header before allocation.
    constexpr quint32 INT32_MAX_AS_U32 = 0x7FFFFFFFu;

    bool readBoundedQByteArray(QDataStream & stream, QByteArray & out, qint64 maxBytes)
    {
        if (maxBytes < 0)
        {
            return false;
        }
        quint32 declared = 0;
        stream >> declared;
        if (stream.status() != QDataStream::Ok)
        {
            return false;
        }
        if (declared == 0xFFFFFFFFu)
        {
            out.clear();
            return true;
        }
        if (declared > INT32_MAX_AS_U32 || static_cast<qint64>(declared) > maxBytes)
        {
            return false;
        }
        out.resize(static_cast<qint32>(declared));
        if (out.size() > 0 && stream.readRawData(out.data(), out.size()) != out.size())
        {
            return false;
        }
        return true;
    }

    bool readBoundedQString(QDataStream & stream, QString & out, qint32 maxChars)
    {
        if (maxChars < 0)
        {
            return false;
        }
        quint32 declared = 0;
        stream >> declared;
        if (stream.status() != QDataStream::Ok)
        {
            return false;
        }
        if (declared == 0xFFFFFFFFu)
        {
            out.clear();
            return true;
        }
        const qint64 maxBytes = static_cast<qint64>(maxChars) * 2;
        if (declared > INT32_MAX_AS_U32 || static_cast<qint64>(declared) > maxBytes || (declared % 2) != 0)
        {
            return false;
        }
        QByteArray buf;
        buf.resize(static_cast<qint32>(declared));
        if (buf.size() > 0 && stream.readRawData(buf.data(), buf.size()) != buf.size())
        {
            return false;
        }
        const qint32 codeUnits = buf.size() / 2;
        out.resize(codeUnits);
        const uchar * src = reinterpret_cast<const uchar *>(buf.constData());
        // Wire is big-endian; build each code unit explicitly to skip platform-endian conversion in fromUtf16.
        for (qint32 i = 0; i < codeUnits; ++i)
        {
            out[i] = QChar(static_cast<ushort>((src[i * 2] << 8) | src[i * 2 + 1]));
        }
        return true;
    }
}

bool Multiplayermenu::requestModSync(const QStringList & modsToDownload, const QStringList & postSyncActiveMods)
{
    if (m_modSyncActive)
    {
        CONSOLE_PRINT("Mod-sync already in flight; ignoring duplicate requestModSync", GameConsole::eWARNING);
        return false;
    }
    // Network guard runs before the empty-request branch so a host-side caller cannot persist client-side post-sync settings.
    if (m_pNetworkInterface == nullptr || m_pNetworkInterface->getIsServer())
    {
        CONSOLE_PRINT("requestModSync called without a client network interface, ignoring", GameConsole::eWARNING);
        return false;
    }
    if (modsToDownload.size() > MOD_SYNC_REQUEST_COUNT_MAX)
    {
        CONSOLE_PRINT("requestModSync exceeds count cap (" + QString::number(modsToDownload.size()) + " > " + QString::number(MOD_SYNC_REQUEST_COUNT_MAX) + "), ignoring", GameConsole::eWARNING);
        return false;
    }
    if (modsToDownload.isEmpty())
    {
        // No downloads needed; persist the post-sync mod selection only. No manifest, no network round-trip.
        Settings::getInstance()->stageActiveModsForRestart(postSyncActiveMods);
        CONSOLE_PRINT("Mod-sync settings-only: " + QString::number(postSyncActiveMods.size()) + " mods staged for restart", GameConsole::eINFO);
        return true;
    }
    m_modSyncActive = true;
    m_modSyncStagings.clear();
    m_modSyncRequestedSet = QSet<QString>(modsToDownload.cbegin(), modsToDownload.cend());
    m_modSyncReceivedBytes = 0;
    m_modSyncReceivedUncompressedBytes = 0;
    m_modSyncExpectedUncompressedTotal = 0;
    m_modSyncPostSyncActiveMods = postSyncActiveMods;
    m_modSyncCurrentChunkMod = ModSyncChunkAccumulator{};

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Version::Qt_6_5);
    stream << QString(NetworkCommands::REQUESTMODSYNC);
    stream << static_cast<qint32>(1);
    stream << modsToDownload;
    // Trailing optional flag; older hosts read past their known fields and ignore. New hosts try-read and fall back to legacy framing if absent.
    stream << static_cast<qint32>(NetworkCommands::ModSyncClientFlagChunked);
    // socketID=0 routes to the server on a TCP client interface; same convention as other client-originated sends.
    emit m_pNetworkInterface->sig_sendData(0, data, NetworkInterface::NetworkSerives::Multiplayer, false);
    CONSOLE_PRINT("Requested mod-sync for " + QString::number(modsToDownload.size()) + " mods (chunked-capable)", GameConsole::eINFO);
    return true;
}

void Multiplayermenu::handleModSyncRequest(QDataStream & stream, quint64 socketID)
{
    if (m_pNetworkInterface == nullptr || !m_pNetworkInterface->getIsServer())
    {
        CONSOLE_PRINT("REQUESTMODSYNC received on non-server, ignoring", GameConsole::eWARNING);
        return;
    }
    auto * settings = Settings::getInstance();
    if (!settings->getModSyncEnabled())
    {
        sendModSyncReject(socketID, NetworkCommands::ModSyncDisabled, QString(), tr("Mod sync is disabled on this host."));
        return;
    }
    // Host-wide pump state is single-slot; reject concurrent peers rather than clobber an in-flight transfer.
    if (m_modSyncSendState.socketID != 0)
    {
        CONSOLE_PRINT("REQUESTMODSYNC arrived while another peer's send is still pumping; rejecting", GameConsole::eWARNING);
        sendModSyncReject(socketID, NetworkCommands::ModSyncBusy, QString(), tr("Another peer is currently mod-syncing; try again shortly."));
        return;
    }
    qint32 protocolVersion = 0;
    stream >> protocolVersion;
    if (protocolVersion != 1)
    {
        sendModSyncReject(socketID, NetworkCommands::ModSyncInternalError, QString(), tr("Unsupported mod-sync protocol version."));
        return;
    }

    const qint32 relPathMaxLen = settings->getModSyncMaxRelativePathLength();
    qint32 modCount = 0;
    stream >> modCount;
    if (stream.status() != QDataStream::Ok || modCount < 0 || modCount > MOD_SYNC_REQUEST_COUNT_MAX)
    {
        sendModSyncReject(socketID, NetworkCommands::ModSyncInternalError, QString(), tr("Malformed mod-sync request."));
        return;
    }
    QStringList requestedMods;
    requestedMods.reserve(modCount);
    {
        // Dedupe at parse time so a hostile client cannot force repeated package builds and duplicate sends within the count cap.
        QSet<QString> seen;
        for (qint32 i = 0; i < modCount; ++i)
        {
            QString mod;
            if (!readBoundedQString(stream, mod, relPathMaxLen))
            {
                sendModSyncReject(socketID, NetworkCommands::ModSyncInvalidPath, QString(), tr("Malformed mod path in request."));
                return;
            }
            if (!seen.contains(mod))
            {
                seen.insert(mod);
                requestedMods.append(mod);
            }
        }
    }
    // Optional clientFlags trailer; older clients send 0 trailing bytes. Strict shape: nothing, or exactly one qint32. Anything else is malformed.
    qint32 clientFlags = 0;
    if (!stream.atEnd())
    {
        stream >> clientFlags;
        if (stream.status() != QDataStream::Ok || !stream.atEnd())
        {
            sendModSyncReject(socketID, NetworkCommands::ModSyncInternalError, QString(), tr("Malformed mod-sync request trailer."));
            return;
        }
    }
    const bool useChunked = (clientFlags & NetworkCommands::ModSyncClientFlagChunked) != 0;

    QStringList hostMods = settings->getMods();
    QStringList hostVersions = settings->getActiveModVersions();
    bool filter = false;
    auto pMap = m_pMapSelectionView->getCurrentMap();
    if (pMap != nullptr && pMap->getGameRules() != nullptr)
    {
        filter = pMap->getGameRules()->getCosmeticModsAllowed();
    }
    settings->filterCosmeticMods(hostMods, hostVersions, filter);

    Filesupport::ModSyncCaps caps;
    caps.perModBytes = settings->getModSyncMaxPerModBytes();
    caps.fileCountMax = settings->getModSyncMaxFiles();
    caps.relPathMaxLen = relPathMaxLen;
    const qint64 totalCap = settings->getModSyncMaxTotalBytes();
    qint64 totalSent = 0;
    qint64 totalUncompressed = 0;

    auto packageRejectReason = [this](qint32 code, const QString & mod) -> QString
    {
        switch (code)
        {
            case NetworkCommands::ModSyncSizeCapExceeded:
                return tr("Mod %1 exceeds the per-mod size cap on the host.").arg(mod);
            case NetworkCommands::ModSyncFileCountCapExceeded:
                return tr("Mod %1 exceeds the per-mod file-count cap on the host.").arg(mod);
            case NetworkCommands::ModSyncInvalidPath:
                return tr("Mod %1 has an unsafe internal file path.").arg(mod);
            case NetworkCommands::ModSyncUnknownMod:
                return tr("Mod %1 was not found on the host.").arg(mod);
            default:
                return tr("Failed to build mod package for %1.").arg(mod);
        }
    };

    // Pre-build so the manifest carries exact sizes; peak ~totalCap, each blob freed after its MODSYNCDATA send.
    QVector<QPair<QString, Filesupport::ModSyncPackage>> builtPackages;
    builtPackages.reserve(requestedMods.size());
    for (const auto & mod : std::as_const(requestedMods))
    {
        if (!Filesupport::validateModPath(mod))
        {
            sendModSyncReject(socketID, NetworkCommands::ModSyncInvalidPath, mod, tr("Invalid mod path."));
            return;
        }
        if (!hostMods.contains(mod))
        {
            sendModSyncReject(socketID, NetworkCommands::ModSyncUnknownMod, mod, tr("Mod not advertised by host."));
            return;
        }
        // Active mods may resolve from CWD or the install/resource path, not just userPath; ask the VFS where the folder actually lives.
        const QString resolvedAbs = VirtualPaths::find(mod, false);
        if (resolvedAbs.isEmpty())
        {
            sendModSyncReject(socketID, NetworkCommands::ModSyncUnknownMod, mod, tr("Mod folder not found in install search paths."));
            return;
        }
        QString resolvedRoot;
        const QString suffixSlash = QStringLiteral("/") + mod;
        if (resolvedAbs.endsWith(suffixSlash))
        {
            resolvedRoot = resolvedAbs.left(resolvedAbs.size() - suffixSlash.size());
        }
        else if (resolvedAbs == mod)
        {
            resolvedRoot = QString();
        }
        else
        {
            sendModSyncReject(socketID, NetworkCommands::ModSyncInternalError, mod, tr("Mod path resolution shape unexpected."));
            return;
        }
        Filesupport::ModSyncPackage pkg = Filesupport::buildModSyncPackage(resolvedRoot, mod, caps);
        if (pkg.rejectReason != 0)
        {
            sendModSyncReject(socketID, pkg.rejectReason, mod, packageRejectReason(pkg.rejectReason, mod));
            return;
        }
        if (totalSent + pkg.compressedBlob.size() > totalCap || totalUncompressed + pkg.declaredUncompressedSize > totalCap)
        {
            sendModSyncReject(socketID, NetworkCommands::ModSyncSizeCapExceeded, QString(), tr("Total sync size exceeds the host's cap."));
            return;
        }
        totalSent += pkg.compressedBlob.size();
        totalUncompressed += pkg.declaredUncompressedSize;
        builtPackages.append(qMakePair(mod, std::move(pkg)));
    }

    {
        QByteArray manifestData;
        QDataStream manifestStream(&manifestData, QIODevice::WriteOnly);
        manifestStream.setVersion(QDataStream::Version::Qt_6_5);
        manifestStream << QString(NetworkCommands::MODSYNCMANIFEST);
        manifestStream << static_cast<qint32>(1);
        manifestStream << static_cast<qint32>(builtPackages.size());
        for (const auto & entry : std::as_const(builtPackages))
        {
            manifestStream << entry.first;
            manifestStream << entry.second.declaredUncompressedSize;
        }
        emit m_pNetworkInterface->sig_sendData(socketID, manifestData, NetworkInterface::NetworkSerives::Multiplayer, false);
        CONSOLE_PRINT("Sent MODSYNCMANIFEST; " + QString::number(builtPackages.size()) + " mods, " + QString::number(totalUncompressed) + " expected uncompressed bytes", GameConsole::eINFO);
    }

    // Hand the data send off to a singleShot-driven pump so the GUI thread isn't pinned hot-looping over chunks for a multi-hundred-MB mod.
    m_modSyncSendState = ModSyncSendState{};
    m_modSyncSendState.socketID = socketID;
    m_modSyncSendState.packages = std::move(builtPackages);
    m_modSyncSendState.useChunked = useChunked;
    CONSOLE_PRINT("Mod-sync send queued (" + QString(useChunked ? "chunked" : "legacy") + "); " + QString::number(m_modSyncSendState.packages.size()) + " mods, " + QString::number(totalSent) + " compressed bytes, " + QString::number(totalUncompressed) + " uncompressed bytes", GameConsole::eINFO);
    QTimer::singleShot(0, this, &Multiplayermenu::pumpModSyncSend);
}

void Multiplayermenu::pumpModSyncSend()
{
    if (m_pNetworkInterface == nullptr || m_modSyncSendState.socketID == 0)
    {
        return;
    }
    auto & state = m_modSyncSendState;
    if (state.currentMod >= state.packages.size())
    {
        QByteArray data;
        QDataStream sendStream(&data, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        sendStream << QString(NetworkCommands::MODSYNCCOMPLETE);
        sendStream << static_cast<qint32>(1);
        emit m_pNetworkInterface->sig_sendData(state.socketID, data, NetworkInterface::NetworkSerives::Multiplayer, false);
        CONSOLE_PRINT("Sent MODSYNCCOMPLETE; " + QString::number(state.packages.size()) + " mods", GameConsole::eINFO);
        state = ModSyncSendState{};
        return;
    }
    auto & entry = state.packages[state.currentMod];
    if (!state.useChunked)
    {
        QByteArray data;
        QDataStream sendStream(&data, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        sendStream << QString(NetworkCommands::MODSYNCDATA);
        sendStream << static_cast<qint32>(1);
        sendStream << entry.first;
        sendStream << entry.second.declaredUncompressedSize;
        sendStream << entry.second.fileCount;
        sendStream << entry.second.compressedBlob;
        emit m_pNetworkInterface->sig_sendData(state.socketID, data, NetworkInterface::NetworkSerives::Multiplayer, false);
        CONSOLE_PRINT("Sent MODSYNCDATA for " + entry.first + " (" + QString::number(entry.second.compressedBlob.size()) + " bytes)", GameConsole::eINFO);
        entry.second.compressedBlob.clear();
        ++state.currentMod;
        QTimer::singleShot(0, this, &Multiplayermenu::pumpModSyncSend);
        return;
    }
    const qint64 compressedTotal = entry.second.compressedBlob.size();
    const qint32 chunkCount = compressedTotal == 0 ? 0 : static_cast<qint32>((compressedTotal + MOD_SYNC_CHUNK_BYTES - 1) / MOD_SYNC_CHUNK_BYTES);
    if (!state.beginEmitted)
    {
        QByteArray data;
        QDataStream sendStream(&data, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        sendStream << QString(NetworkCommands::MODSYNCMODBEGIN);
        sendStream << static_cast<qint32>(1);
        sendStream << entry.first;
        sendStream << entry.second.declaredUncompressedSize;
        sendStream << entry.second.fileCount;
        sendStream << compressedTotal;
        sendStream << chunkCount;
        emit m_pNetworkInterface->sig_sendData(state.socketID, data, NetworkInterface::NetworkSerives::Multiplayer, false);
        CONSOLE_PRINT("Sent MODSYNCMODBEGIN for " + entry.first + " (" + QString::number(compressedTotal) + " bytes, " + QString::number(chunkCount) + " chunks)", GameConsole::eINFO);
        state.beginEmitted = true;
        state.currentChunk = 0;
        QTimer::singleShot(0, this, &Multiplayermenu::pumpModSyncSend);
        return;
    }
    if (state.currentChunk < chunkCount)
    {
        const qint64 offset = static_cast<qint64>(state.currentChunk) * MOD_SYNC_CHUNK_BYTES;
        const qint32 sliceLen = static_cast<qint32>(std::min<qint64>(MOD_SYNC_CHUNK_BYTES, compressedTotal - offset));
        const QByteArray chunkBytes = entry.second.compressedBlob.mid(static_cast<qint32>(offset), sliceLen);
        QByteArray data;
        QDataStream sendStream(&data, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        sendStream << QString(NetworkCommands::MODSYNCMODCHUNK);
        sendStream << static_cast<qint32>(1);
        sendStream << entry.first;
        sendStream << state.currentChunk;
        sendStream << chunkBytes;
        emit m_pNetworkInterface->sig_sendData(state.socketID, data, NetworkInterface::NetworkSerives::Multiplayer, false);
        ++state.currentChunk;
        QTimer::singleShot(0, this, &Multiplayermenu::pumpModSyncSend);
        return;
    }
    {
        QByteArray data;
        QDataStream sendStream(&data, QIODevice::WriteOnly);
        sendStream.setVersion(QDataStream::Version::Qt_6_5);
        sendStream << QString(NetworkCommands::MODSYNCMODEND);
        sendStream << static_cast<qint32>(1);
        sendStream << entry.first;
        emit m_pNetworkInterface->sig_sendData(state.socketID, data, NetworkInterface::NetworkSerives::Multiplayer, false);
        CONSOLE_PRINT("Sent MODSYNCMODEND for " + entry.first, GameConsole::eINFO);
    }
    entry.second.compressedBlob.clear();
    ++state.currentMod;
    state.beginEmitted = false;
    state.currentChunk = 0;
    QTimer::singleShot(0, this, &Multiplayermenu::pumpModSyncSend);
}

void Multiplayermenu::handleModSyncManifest(QDataStream & stream, quint64 socketID)
{
    Q_UNUSED(socketID);
    if (!m_modSyncActive)
    {
        CONSOLE_PRINT("MODSYNCMANIFEST received with no active mod-sync session, ignoring", GameConsole::eWARNING);
        return;
    }
    auto * settings = Settings::getInstance();
    const qint32 relPathMaxLen = settings->getModSyncMaxRelativePathLength();
    const qint64 totalCap = settings->getModSyncMaxTotalBytes();

    qint32 protocolVersion = 0;
    stream >> protocolVersion;
    if (stream.status() != QDataStream::Ok || protocolVersion != 1)
    {
        CONSOLE_PRINT("MODSYNCMANIFEST unsupported protocol version, ignoring", GameConsole::eWARNING);
        return;
    }
    qint32 entryCount = 0;
    stream >> entryCount;
    if (stream.status() != QDataStream::Ok || entryCount < 0 || entryCount > MOD_SYNC_REQUEST_COUNT_MAX)
    {
        CONSOLE_PRINT("MODSYNCMANIFEST malformed entry count, ignoring", GameConsole::eWARNING);
        return;
    }
    qint64 expectedTotal = 0;
    qint32 ignoredEntries = 0;
    for (qint32 i = 0; i < entryCount; ++i)
    {
        QString modPath;
        if (!readBoundedQString(stream, modPath, relPathMaxLen))
        {
            CONSOLE_PRINT("MODSYNCMANIFEST mod path overflow or malformed, ignoring", GameConsole::eWARNING);
            return;
        }
        qint32 declaredSize = 0;
        stream >> declaredSize;
        if (stream.status() != QDataStream::Ok || declaredSize < 0)
        {
            CONSOLE_PRINT("MODSYNCMANIFEST declared size out of range for " + modPath + ", ignoring", GameConsole::eWARNING);
            return;
        }
        // Only count entries the client actually asked for; a hostile or buggy host could otherwise inflate expectedTotal to drive the bar to 100% before any data lands.
        if (!m_modSyncRequestedSet.contains(modPath))
        {
            ++ignoredEntries;
            continue;
        }
        expectedTotal += declaredSize;
        if (expectedTotal > totalCap)
        {
            CONSOLE_PRINT("MODSYNCMANIFEST expected total exceeds host total cap; clamping for display only", GameConsole::eWARNING);
            expectedTotal = totalCap;
            break;
        }
    }
    if (ignoredEntries > 0)
    {
        CONSOLE_PRINT("MODSYNCMANIFEST ignored " + QString::number(ignoredEntries) + " entries not in the client's request set", GameConsole::eWARNING);
    }
    m_modSyncExpectedUncompressedTotal = expectedTotal;
    if (m_modSyncProgressDialog != nullptr)
    {
        m_modSyncProgressDialog->setExpectedTotalBytes(expectedTotal);
    }
    CONSOLE_PRINT("Received MODSYNCMANIFEST; expected uncompressed total " + QString::number(expectedTotal) + " bytes across " + QString::number(entryCount) + " mods", GameConsole::eINFO);
}

void Multiplayermenu::handleModSyncData(QDataStream & stream, quint64 socketID)
{
    Q_UNUSED(socketID);
    if (!m_modSyncActive)
    {
        CONSOLE_PRINT("MODSYNCDATA received with no active mod-sync session, ignoring", GameConsole::eWARNING);
        return;
    }
    auto * settings = Settings::getInstance();
    const qint32 relPathMaxLen = settings->getModSyncMaxRelativePathLength();
    const qint64 perModCap = settings->getModSyncMaxPerModBytes();
    const qint32 fileCountMax = settings->getModSyncMaxFiles();
    const qint64 totalCap = settings->getModSyncMaxTotalBytes();

    auto failData = [this](const QString & uiReason)
    {
        cancelModSyncSession();
        onModSyncFailed(uiReason);
    };

    if (!m_modSyncCurrentChunkMod.modPath.isEmpty())
    {
        CONSOLE_PRINT("MODSYNCDATA arrived while a chunked mod is in flight; protocol violation", GameConsole::eERROR);
        failData(tr("Host mixed chunked and legacy mod-sync framing."));
        return;
    }

    qint32 protocolVersion = 0;
    stream >> protocolVersion;
    if (stream.status() != QDataStream::Ok || protocolVersion != 1)
    {
        CONSOLE_PRINT("MODSYNCDATA unsupported protocol version", GameConsole::eERROR);
        failData(tr("Unsupported mod-sync protocol from host."));
        return;
    }

    QString modPath;
    if (!readBoundedQString(stream, modPath, relPathMaxLen))
    {
        CONSOLE_PRINT("MODSYNCDATA mod path overflow or malformed", GameConsole::eERROR);
        failData(tr("Malformed mod-sync data frame."));
        return;
    }

    qint32 declaredSize = 0;
    qint32 fileCount = 0;
    stream >> declaredSize;
    stream >> fileCount;
    if (stream.status() != QDataStream::Ok || declaredSize < 0 || fileCount < 0 || fileCount > fileCountMax)
    {
        CONSOLE_PRINT("MODSYNCDATA size/count out of range for " + modPath, GameConsole::eERROR);
        failData(tr("Mod %1 exceeds the per-mod file-count cap.").arg(modPath));
        return;
    }

    QByteArray compressedBlob;
    if (!readBoundedQByteArray(stream, compressedBlob, perModCap))
    {
        CONSOLE_PRINT("MODSYNCDATA blob overflow or malformed for " + modPath, GameConsole::eERROR);
        failData(tr("Mod %1 exceeds the per-mod size cap.").arg(modPath));
        return;
    }

    if (!Filesupport::validateModPath(modPath))
    {
        CONSOLE_PRINT("MODSYNCDATA invalid mod path: " + modPath, GameConsole::eERROR);
        failData(tr("Host sent an invalid mod path."));
        return;
    }
    if (!m_modSyncRequestedSet.contains(modPath))
    {
        CONSOLE_PRINT("MODSYNCDATA for unrequested or duplicate mod: " + modPath, GameConsole::eERROR);
        failData(tr("Host sent an unrequested or duplicate mod."));
        return;
    }

    m_modSyncReceivedBytes += compressedBlob.size();
    m_modSyncReceivedUncompressedBytes += declaredSize;
    if (m_modSyncReceivedBytes > totalCap || m_modSyncReceivedUncompressedBytes > totalCap)
    {
        CONSOLE_PRINT("Mod-sync exceeds total bytes cap, aborting", GameConsole::eERROR);
        failData(tr("Mod-sync exceeds the total transfer cap."));
        return;
    }

    Filesupport::ModSyncCaps caps;
    caps.perModBytes = perModCap;
    caps.fileCountMax = fileCountMax;
    caps.relPathMaxLen = relPathMaxLen;

    qint32 rejectReason = 0;
    auto files = Filesupport::extractModSyncPackage(compressedBlob, declaredSize, caps, rejectReason);
    if (rejectReason != 0)
    {
        CONSOLE_PRINT("Mod-sync extract rejected (" + QString::number(rejectReason) + ") for " + modPath, GameConsole::eERROR);
        failData(tr("Failed to unpack mod %1.").arg(modPath));
        return;
    }
    if (files.size() != fileCount)
    {
        CONSOLE_PRINT("Mod-sync file count mismatch for " + modPath + " (got " + QString::number(files.size()) + ", expected " + QString::number(fileCount) + ")", GameConsole::eERROR);
        failData(tr("Mod %1 file count did not match the host's declaration.").arg(modPath));
        return;
    }

    qint32 stageReason = 0;
    QString stagingRel = Filesupport::stageModSync(settings->getUserPath(), modPath, files, caps, stageReason);
    if (stageReason != 0 || stagingRel.isEmpty())
    {
        CONSOLE_PRINT("Mod-sync stage rejected (" + QString::number(stageReason) + ") for " + modPath, GameConsole::eERROR);
        failData(tr("Failed to stage mod %1 to disk.").arg(modPath));
        return;
    }
    m_modSyncStagings.append(qMakePair(stagingRel, modPath));
    m_modSyncRequestedSet.remove(modPath);
    CONSOLE_PRINT("Mod-sync staged " + modPath + " (" + QString::number(files.size()) + " files)", GameConsole::eINFO);
    onModSyncProgress();
}

void Multiplayermenu::handleModSyncModBegin(QDataStream & stream, quint64 socketID)
{
    Q_UNUSED(socketID);
    if (!m_modSyncActive)
    {
        CONSOLE_PRINT("MODSYNCMODBEGIN received with no active mod-sync session, ignoring", GameConsole::eWARNING);
        return;
    }
    auto * settings = Settings::getInstance();
    const qint32 relPathMaxLen = settings->getModSyncMaxRelativePathLength();
    const qint64 perModCap = settings->getModSyncMaxPerModBytes();
    const qint32 fileCountMax = settings->getModSyncMaxFiles();
    const qint64 totalCap = settings->getModSyncMaxTotalBytes();

    auto failBegin = [this](const QString & uiReason)
    {
        cancelModSyncSession();
        onModSyncFailed(uiReason);
    };

    if (!m_modSyncCurrentChunkMod.modPath.isEmpty())
    {
        CONSOLE_PRINT("MODSYNCMODBEGIN while a chunked mod is already in flight; protocol violation", GameConsole::eERROR);
        failBegin(tr("Host opened a second chunked mod before finishing the first."));
        return;
    }

    qint32 protocolVersion = 0;
    stream >> protocolVersion;
    if (stream.status() != QDataStream::Ok || protocolVersion != 1)
    {
        CONSOLE_PRINT("MODSYNCMODBEGIN unsupported protocol version", GameConsole::eERROR);
        failBegin(tr("Unsupported mod-sync protocol from host."));
        return;
    }
    QString modPath;
    if (!readBoundedQString(stream, modPath, relPathMaxLen))
    {
        CONSOLE_PRINT("MODSYNCMODBEGIN mod path overflow or malformed", GameConsole::eERROR);
        failBegin(tr("Malformed mod-sync begin frame."));
        return;
    }
    qint32 declaredUncompressedSize = 0;
    qint32 fileCount = 0;
    qint64 compressedTotal = 0;
    qint32 chunkCount = 0;
    stream >> declaredUncompressedSize;
    stream >> fileCount;
    stream >> compressedTotal;
    stream >> chunkCount;
    if (stream.status() != QDataStream::Ok)
    {
        CONSOLE_PRINT("MODSYNCMODBEGIN truncated header for " + modPath, GameConsole::eERROR);
        failBegin(tr("Malformed mod-sync begin frame."));
        return;
    }
    if (declaredUncompressedSize < 0 || declaredUncompressedSize > perModCap)
    {
        CONSOLE_PRINT("MODSYNCMODBEGIN declaredUncompressedSize out of range for " + modPath, GameConsole::eERROR);
        failBegin(tr("Mod %1 exceeds the per-mod size cap.").arg(modPath));
        return;
    }
    if (compressedTotal < 0 || compressedTotal > perModCap)
    {
        CONSOLE_PRINT("MODSYNCMODBEGIN compressedTotal out of range for " + modPath, GameConsole::eERROR);
        failBegin(tr("Mod %1 exceeds the per-mod size cap.").arg(modPath));
        return;
    }
    // Defensive: QByteArray and downstream offsets use qint32 sizes throughout the slice 5 pipeline. perModCap is qint32 today, but guard explicitly so a future widening cannot silently overflow the casts below.
    if (compressedTotal > std::numeric_limits<qint32>::max())
    {
        CONSOLE_PRINT("MODSYNCMODBEGIN compressedTotal exceeds qint32 range for " + modPath, GameConsole::eERROR);
        failBegin(tr("Mod %1 is too large to receive.").arg(modPath));
        return;
    }
    if (fileCount < 0 || fileCount > fileCountMax)
    {
        CONSOLE_PRINT("MODSYNCMODBEGIN fileCount out of range for " + modPath, GameConsole::eERROR);
        failBegin(tr("Mod %1 exceeds the per-mod file-count cap.").arg(modPath));
        return;
    }
    if (chunkCount < 0 || chunkCount > MOD_SYNC_CHUNK_COUNT_MAX)
    {
        CONSOLE_PRINT("MODSYNCMODBEGIN chunkCount out of range for " + modPath, GameConsole::eERROR);
        failBegin(tr("Mod %1 has too many chunks.").arg(modPath));
        return;
    }
    const qint32 expectedChunkCount = compressedTotal == 0 ? 0 : static_cast<qint32>((compressedTotal + MOD_SYNC_CHUNK_BYTES - 1) / MOD_SYNC_CHUNK_BYTES);
    if (chunkCount != expectedChunkCount)
    {
        CONSOLE_PRINT("MODSYNCMODBEGIN chunkCount inconsistent with compressedTotal for " + modPath, GameConsole::eERROR);
        failBegin(tr("Mod %1 chunk count is inconsistent.").arg(modPath));
        return;
    }
    if (m_modSyncReceivedBytes + compressedTotal > totalCap || m_modSyncReceivedUncompressedBytes + declaredUncompressedSize > totalCap)
    {
        CONSOLE_PRINT("MODSYNCMODBEGIN would exceed total cap for " + modPath, GameConsole::eERROR);
        failBegin(tr("Mod-sync exceeds the total transfer cap."));
        return;
    }
    if (!Filesupport::validateModPath(modPath))
    {
        CONSOLE_PRINT("MODSYNCMODBEGIN invalid mod path: " + modPath, GameConsole::eERROR);
        failBegin(tr("Host sent an invalid mod path."));
        return;
    }
    if (!m_modSyncRequestedSet.contains(modPath))
    {
        CONSOLE_PRINT("MODSYNCMODBEGIN for unrequested or duplicate mod: " + modPath, GameConsole::eERROR);
        failBegin(tr("Host sent an unrequested or duplicate mod."));
        return;
    }

    m_modSyncCurrentChunkMod = ModSyncChunkAccumulator{};
    m_modSyncCurrentChunkMod.modPath = modPath;
    m_modSyncCurrentChunkMod.declaredUncompressedSize = declaredUncompressedSize;
    m_modSyncCurrentChunkMod.fileCount = fileCount;
    m_modSyncCurrentChunkMod.compressedTotal = compressedTotal;
    m_modSyncCurrentChunkMod.expectedChunkCount = chunkCount;
    if (compressedTotal > 0)
    {
        m_modSyncCurrentChunkMod.blob.reserve(static_cast<qint32>(compressedTotal));
    }
    CONSOLE_PRINT("Received MODSYNCMODBEGIN for " + modPath + " (" + QString::number(compressedTotal) + " bytes, " + QString::number(chunkCount) + " chunks)", GameConsole::eINFO);
}

void Multiplayermenu::handleModSyncModChunk(QDataStream & stream, quint64 socketID)
{
    Q_UNUSED(socketID);
    if (!m_modSyncActive)
    {
        CONSOLE_PRINT("MODSYNCMODCHUNK received with no active mod-sync session, ignoring", GameConsole::eWARNING);
        return;
    }
    if (m_modSyncCurrentChunkMod.modPath.isEmpty())
    {
        CONSOLE_PRINT("MODSYNCMODCHUNK without prior MODSYNCMODBEGIN; protocol violation", GameConsole::eERROR);
        cancelModSyncSession();
        onModSyncFailed(tr("Host sent chunk without begin."));
        return;
    }
    auto * settings = Settings::getInstance();
    const qint32 relPathMaxLen = settings->getModSyncMaxRelativePathLength();
    const qint64 totalCap = settings->getModSyncMaxTotalBytes();

    auto failChunk = [this](const QString & uiReason)
    {
        cancelModSyncSession();
        onModSyncFailed(uiReason);
    };

    qint32 protocolVersion = 0;
    stream >> protocolVersion;
    if (stream.status() != QDataStream::Ok || protocolVersion != 1)
    {
        CONSOLE_PRINT("MODSYNCMODCHUNK unsupported protocol version", GameConsole::eERROR);
        failChunk(tr("Unsupported mod-sync protocol from host."));
        return;
    }
    QString modPath;
    if (!readBoundedQString(stream, modPath, relPathMaxLen))
    {
        CONSOLE_PRINT("MODSYNCMODCHUNK mod path overflow or malformed", GameConsole::eERROR);
        failChunk(tr("Malformed mod-sync chunk frame."));
        return;
    }
    if (modPath != m_modSyncCurrentChunkMod.modPath)
    {
        CONSOLE_PRINT("MODSYNCMODCHUNK modPath mismatch; expected " + m_modSyncCurrentChunkMod.modPath + " got " + modPath, GameConsole::eERROR);
        failChunk(tr("Host interleaved chunks across mods."));
        return;
    }
    qint32 chunkIndex = 0;
    stream >> chunkIndex;
    if (stream.status() != QDataStream::Ok || chunkIndex != m_modSyncCurrentChunkMod.receivedChunkCount)
    {
        CONSOLE_PRINT("MODSYNCMODCHUNK out-of-order chunkIndex for " + modPath, GameConsole::eERROR);
        failChunk(tr("Host sent chunks out of order."));
        return;
    }
    QByteArray chunkBytes;
    if (!readBoundedQByteArray(stream, chunkBytes, MOD_SYNC_CHUNK_BYTES))
    {
        CONSOLE_PRINT("MODSYNCMODCHUNK chunk overflow or malformed for " + modPath, GameConsole::eERROR);
        failChunk(tr("Mod-sync chunk exceeds size limit."));
        return;
    }
    const qint64 newAccumulated = static_cast<qint64>(m_modSyncCurrentChunkMod.blob.size()) + chunkBytes.size();
    if (newAccumulated > m_modSyncCurrentChunkMod.compressedTotal)
    {
        CONSOLE_PRINT("MODSYNCMODCHUNK accumulated exceeds compressedTotal for " + modPath, GameConsole::eERROR);
        failChunk(tr("Host sent more bytes than declared for mod %1.").arg(modPath));
        return;
    }
    const bool isFinalChunk = (m_modSyncCurrentChunkMod.receivedChunkCount + 1 == m_modSyncCurrentChunkMod.expectedChunkCount);
    const qint64 expectedThisChunk = isFinalChunk
        ? (m_modSyncCurrentChunkMod.compressedTotal - static_cast<qint64>(m_modSyncCurrentChunkMod.blob.size()))
        : MOD_SYNC_CHUNK_BYTES;
    if (chunkBytes.size() != expectedThisChunk)
    {
        CONSOLE_PRINT("MODSYNCMODCHUNK size " + QString::number(chunkBytes.size()) + " differs from expected " + QString::number(expectedThisChunk) + " for " + modPath, GameConsole::eERROR);
        failChunk(tr("Host sent a chunk of unexpected size."));
        return;
    }

    m_modSyncCurrentChunkMod.blob.append(chunkBytes);
    ++m_modSyncCurrentChunkMod.receivedChunkCount;
    m_modSyncReceivedBytes += chunkBytes.size();
    // Display-side proportional uncompressed advance so the bar fraction can move during a single mod; snap-corrected at MODSYNCMODEND to absorb integer-division drift.
    qint64 perChunkUncompressedDelta = 0;
    if (m_modSyncCurrentChunkMod.compressedTotal > 0 && m_modSyncCurrentChunkMod.declaredUncompressedSize > 0)
    {
        perChunkUncompressedDelta = static_cast<qint64>(chunkBytes.size()) * m_modSyncCurrentChunkMod.declaredUncompressedSize / m_modSyncCurrentChunkMod.compressedTotal;
    }
    m_modSyncCurrentChunkMod.uncompressedAdvanced += perChunkUncompressedDelta;
    m_modSyncReceivedUncompressedBytes += perChunkUncompressedDelta;
    if (m_modSyncReceivedBytes > totalCap || m_modSyncReceivedUncompressedBytes > totalCap)
    {
        CONSOLE_PRINT("Mod-sync exceeds total bytes cap mid-chunk, aborting", GameConsole::eERROR);
        failChunk(tr("Mod-sync exceeds the total transfer cap."));
        return;
    }
    onModSyncProgress();
}

void Multiplayermenu::handleModSyncModEnd(QDataStream & stream, quint64 socketID)
{
    Q_UNUSED(socketID);
    if (!m_modSyncActive)
    {
        CONSOLE_PRINT("MODSYNCMODEND received with no active mod-sync session, ignoring", GameConsole::eWARNING);
        return;
    }
    if (m_modSyncCurrentChunkMod.modPath.isEmpty())
    {
        CONSOLE_PRINT("MODSYNCMODEND without prior MODSYNCMODBEGIN; protocol violation", GameConsole::eERROR);
        cancelModSyncSession();
        onModSyncFailed(tr("Host sent end without begin."));
        return;
    }
    auto * settings = Settings::getInstance();
    const qint32 relPathMaxLen = settings->getModSyncMaxRelativePathLength();
    const qint64 perModCap = settings->getModSyncMaxPerModBytes();
    const qint32 fileCountMax = settings->getModSyncMaxFiles();

    auto failEnd = [this](const QString & uiReason)
    {
        cancelModSyncSession();
        onModSyncFailed(uiReason);
    };

    qint32 protocolVersion = 0;
    stream >> protocolVersion;
    if (stream.status() != QDataStream::Ok || protocolVersion != 1)
    {
        CONSOLE_PRINT("MODSYNCMODEND unsupported protocol version", GameConsole::eERROR);
        failEnd(tr("Unsupported mod-sync protocol from host."));
        return;
    }
    QString modPath;
    if (!readBoundedQString(stream, modPath, relPathMaxLen))
    {
        CONSOLE_PRINT("MODSYNCMODEND mod path overflow or malformed", GameConsole::eERROR);
        failEnd(tr("Malformed mod-sync end frame."));
        return;
    }
    if (modPath != m_modSyncCurrentChunkMod.modPath)
    {
        CONSOLE_PRINT("MODSYNCMODEND modPath mismatch; expected " + m_modSyncCurrentChunkMod.modPath + " got " + modPath, GameConsole::eERROR);
        failEnd(tr("Host ended a different mod than the one in flight."));
        return;
    }
    if (m_modSyncCurrentChunkMod.receivedChunkCount != m_modSyncCurrentChunkMod.expectedChunkCount)
    {
        CONSOLE_PRINT("MODSYNCMODEND chunk count mismatch for " + modPath, GameConsole::eERROR);
        failEnd(tr("Host ended mod %1 before delivering all chunks.").arg(modPath));
        return;
    }
    if (m_modSyncCurrentChunkMod.blob.size() != m_modSyncCurrentChunkMod.compressedTotal)
    {
        CONSOLE_PRINT("MODSYNCMODEND blob size mismatch for " + modPath, GameConsole::eERROR);
        failEnd(tr("Host blob size for mod %1 did not match its declared total.").arg(modPath));
        return;
    }

    Filesupport::ModSyncCaps caps;
    caps.perModBytes = perModCap;
    caps.fileCountMax = fileCountMax;
    caps.relPathMaxLen = relPathMaxLen;

    qint32 rejectReason = 0;
    auto files = Filesupport::extractModSyncPackage(m_modSyncCurrentChunkMod.blob, m_modSyncCurrentChunkMod.declaredUncompressedSize, caps, rejectReason);
    if (rejectReason != 0)
    {
        CONSOLE_PRINT("MODSYNCMODEND extract rejected (" + QString::number(rejectReason) + ") for " + modPath, GameConsole::eERROR);
        failEnd(tr("Failed to unpack mod %1.").arg(modPath));
        return;
    }
    if (files.size() != m_modSyncCurrentChunkMod.fileCount)
    {
        CONSOLE_PRINT("MODSYNCMODEND file count mismatch for " + modPath + " (got " + QString::number(files.size()) + ", expected " + QString::number(m_modSyncCurrentChunkMod.fileCount) + ")", GameConsole::eERROR);
        failEnd(tr("Mod %1 file count did not match the host's declaration.").arg(modPath));
        return;
    }
    qint32 stageReason = 0;
    QString stagingRel = Filesupport::stageModSync(settings->getUserPath(), modPath, files, caps, stageReason);
    if (stageReason != 0 || stagingRel.isEmpty())
    {
        CONSOLE_PRINT("MODSYNCMODEND stage rejected (" + QString::number(stageReason) + ") for " + modPath, GameConsole::eERROR);
        failEnd(tr("Failed to stage mod %1 to disk.").arg(modPath));
        return;
    }

    // Snap-correct the proportional uncompressed advance so the global counter equals the exact sum of declaredUncompressedSize values.
    const qint64 snapDelta = static_cast<qint64>(m_modSyncCurrentChunkMod.declaredUncompressedSize) - m_modSyncCurrentChunkMod.uncompressedAdvanced;
    if (snapDelta != 0)
    {
        m_modSyncReceivedUncompressedBytes += snapDelta;
    }

    m_modSyncStagings.append(qMakePair(stagingRel, modPath));
    m_modSyncRequestedSet.remove(modPath);
    CONSOLE_PRINT("Mod-sync staged " + modPath + " via chunked path (" + QString::number(files.size()) + " files)", GameConsole::eINFO);
    m_modSyncCurrentChunkMod = ModSyncChunkAccumulator{};
    onModSyncProgress();
}

void Multiplayermenu::handleModSyncReject(QDataStream & stream, quint64 socketID)
{
    Q_UNUSED(socketID);
    if (!m_modSyncActive)
    {
        // Drop late or unsolicited rejects after cancel/success so we do not stack a second failure dialog.
        CONSOLE_PRINT("MODSYNCREJECT received with no active mod-sync session, ignoring", GameConsole::eWARNING);
        return;
    }
    auto * settings = Settings::getInstance();
    const qint32 relPathMaxLen = settings->getModSyncMaxRelativePathLength();

    qint32 protocolVersion = 0;
    stream >> protocolVersion;
    if (stream.status() != QDataStream::Ok || protocolVersion != 1)
    {
        CONSOLE_PRINT("MODSYNCREJECT unsupported protocol version", GameConsole::eERROR);
        cancelModSyncSession();
        onModSyncFailed(tr("Unsupported mod-sync protocol from host."));
        return;
    }
    QString modPath;
    if (!readBoundedQString(stream, modPath, relPathMaxLen))
    {
        CONSOLE_PRINT("MODSYNCREJECT mod path overflow or malformed", GameConsole::eERROR);
        cancelModSyncSession();
        onModSyncFailed(tr("Malformed mod-sync reject frame."));
        return;
    }
    qint32 reasonCode = 0;
    stream >> reasonCode;
    QString reasonMessage;
    if (!readBoundedQString(stream, reasonMessage, MOD_SYNC_REASON_CHARS_MAX))
    {
        CONSOLE_PRINT("MODSYNCREJECT reason message overflow or malformed (code=" + QString::number(reasonCode) + " mod=" + modPath + ")", GameConsole::eERROR);
        cancelModSyncSession();
        onModSyncFailed(tr("Malformed reject reason from host."));
        return;
    }
    CONSOLE_PRINT("Mod-sync rejected by host: code=" + QString::number(reasonCode) + " mod=" + modPath + " msg=" + reasonMessage, GameConsole::eERROR);
    const QString uiReason = reasonMessage.isEmpty()
        ? tr("Host rejected the request (code %1).").arg(reasonCode)
        : reasonMessage;
    cancelModSyncSession();
    onModSyncFailed(uiReason);
}

void Multiplayermenu::handleModSyncComplete(QDataStream & stream, quint64 socketID)
{
    Q_UNUSED(socketID);
    if (!m_modSyncActive)
    {
        // Drop late or unsolicited completes after cancel/success before parsing so a malformed stale frame does not stack a second failure dialog.
        CONSOLE_PRINT("MODSYNCCOMPLETE received with no active mod-sync session, ignoring", GameConsole::eWARNING);
        return;
    }
    qint32 protocolVersion = 0;
    stream >> protocolVersion;
    if (stream.status() != QDataStream::Ok || protocolVersion != 1)
    {
        CONSOLE_PRINT("MODSYNCCOMPLETE unsupported protocol version", GameConsole::eERROR);
        cancelModSyncSession();
        onModSyncFailed(tr("Unsupported mod-sync protocol from host."));
        return;
    }
    if (!m_modSyncCurrentChunkMod.modPath.isEmpty())
    {
        CONSOLE_PRINT("MODSYNCCOMPLETE arrived with chunked mod " + m_modSyncCurrentChunkMod.modPath + " still in flight; aborting", GameConsole::eERROR);
        cancelModSyncSession();
        onModSyncFailed(tr("Host completed before finishing the chunked mod transfer."));
        return;
    }
    if (!m_modSyncRequestedSet.isEmpty())
    {
        CONSOLE_PRINT("MODSYNCCOMPLETE arrived with " + QString::number(m_modSyncRequestedSet.size()) + " requested mods unsent; aborting", GameConsole::eERROR);
        cancelModSyncSession();
        onModSyncFailed(tr("Host did not deliver every requested mod."));
        return;
    }
    if (m_modSyncStagings.isEmpty())
    {
        CONSOLE_PRINT("MODSYNCCOMPLETE received with no stagings; nothing to apply", GameConsole::eWARNING);
        m_modSyncActive = false;
        m_modSyncReceivedBytes = 0;
        m_modSyncReceivedUncompressedBytes = 0;
        m_modSyncExpectedUncompressedTotal = 0;
        m_modSyncCurrentChunkMod = ModSyncChunkAccumulator{};
        m_modSyncPostSyncActiveMods.clear();
        return;
    }
    auto * settings = Settings::getInstance();
    // Settings first; the manifest is the commit point so it must be the last thing written.
    const QString priorActiveModsRaw = settings->stageActiveModsForRestart(m_modSyncPostSyncActiveMods);
    QList<QPair<QString, QString>> manifestSwaps;
    for (const auto & p : std::as_const(m_modSyncStagings))
    {
        manifestSwaps.append(qMakePair(p.first, p.second));
    }
    if (!Filesupport::writePendingModSyncManifest(settings->getUserPath(), manifestSwaps))
    {
        CONSOLE_PRINT("Failed to write pending mod-sync manifest; restoring prior active-mod list", GameConsole::eERROR);
        settings->restoreActiveModsRaw(priorActiveModsRaw);
        cancelModSyncSession();
        onModSyncFailed(tr("Failed to write the pending mod-sync manifest."));
        return;
    }
    CONSOLE_PRINT("Mod-sync complete: " + QString::number(m_modSyncStagings.size()) + " mods staged. Restarting to apply.", GameConsole::eINFO);
    m_modSyncActive = false;
    m_modSyncStagings.clear();
    m_modSyncRequestedSet.clear();
    m_modSyncReceivedBytes = 0;
    m_modSyncReceivedUncompressedBytes = 0;
    m_modSyncExpectedUncompressedTotal = 0;
    m_modSyncCurrentChunkMod = ModSyncChunkAccumulator{};
    m_modSyncPostSyncActiveMods.clear();
    // Hold so the progress dialog gets a frame to paint at 100% before the success+restart sequence tears it down.
    QTimer::singleShot(500, this, &Multiplayermenu::onModSyncSucceeded);
}

void Multiplayermenu::sendModSyncReject(quint64 socketID, qint32 reasonCode, const QString & modPath, const QString & message)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Version::Qt_6_5);
    stream << QString(NetworkCommands::MODSYNCREJECT);
    stream << static_cast<qint32>(1);
    stream << modPath;
    stream << reasonCode;
    stream << message;
    emit m_pNetworkInterface->sig_sendData(socketID, data, NetworkInterface::NetworkSerives::Multiplayer, false);
    CONSOLE_PRINT("Sent MODSYNCREJECT code=" + QString::number(reasonCode) + " mod=" + modPath + " msg=" + message, GameConsole::eINFO);
}

void Multiplayermenu::cancelModSyncSession()
{
    // Dialog teardown ahead of the active-session guard so a request that bailed before arming still tears down its progress UI.
    if (m_modSyncProgressDialog != nullptr)
    {
        m_modSyncProgressDialog->detach();
        m_modSyncProgressDialog.reset();
    }
    // Send-state clearing is host-side and runs even when m_modSyncActive (client-side flag) is false; the next pump tick short-circuits on socketID==0.
    m_modSyncSendState = ModSyncSendState{};
    if (!m_modSyncActive)
    {
        return;
    }
    auto * settings = Settings::getInstance();
    const QString installRoot = settings->getUserPath();
    for (const auto & p : std::as_const(m_modSyncStagings))
    {
        QString stagingAbs;
        if (installRoot.isEmpty())
        {
            stagingAbs = p.first;
        }
        else if (installRoot.endsWith(QChar('/')))
        {
            stagingAbs = installRoot + p.first;
        }
        else
        {
            stagingAbs = installRoot + QChar('/') + p.first;
        }
        QDir(stagingAbs).removeRecursively();
    }
    m_modSyncStagings.clear();
    m_modSyncRequestedSet.clear();
    m_modSyncReceivedBytes = 0;
    m_modSyncReceivedUncompressedBytes = 0;
    m_modSyncExpectedUncompressedTotal = 0;
    m_modSyncActive = false;
    m_modSyncCurrentChunkMod = ModSyncChunkAccumulator{};
    m_modSyncPostSyncActiveMods.clear();
}

void Multiplayermenu::confirmModSync(const QStringList & modsToDownload, const QStringList & postSyncActiveMods)
{
    if (modsToDownload.isEmpty())
    {
        // Settings-only branch: no untrusted host content downloaded, skip the trust prompt.
        const bool ok = requestModSync(modsToDownload, postSyncActiveMods);
        if (ok)
        {
            onModSyncSucceeded();
        }
        else
        {
            onModSyncFailed(tr("Could not start mod sync."));
        }
        return;
    }
    // Trust prompt before any host-supplied mod content is downloaded; mod scripts execute under the QJSEngine in this process.
    spDialogMessageBox pTrust = MemoryManagement::create<DialogMessageBox>(
        tr("You are about to install unverified mods from this host. These mods may include scripts that run in your game. Only continue if you trust this host."),
        true, tr("Install"), tr("Cancel"));
    connect(pTrust.get(), &DialogMessageBox::sigOk, this, [this, modsToDownload, postSyncActiveMods]()
    {
        startModSyncDownload(modsToDownload, postSyncActiveMods);
    }, Qt::QueuedConnection);
    connect(pTrust.get(), &DialogMessageBox::sigCancel, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
    addChild(pTrust);
}

void Multiplayermenu::startModSyncDownload(const QStringList & modsToDownload, const QStringList & postSyncActiveMods)
{
    const bool ok = requestModSync(modsToDownload, postSyncActiveMods);
    if (!ok)
    {
        onModSyncFailed(tr("Could not start mod sync."));
        return;
    }
    if (m_pJoinConnectingDialog != nullptr)
    {
        m_pJoinConnectingDialog->detach();
        m_pJoinConnectingDialog.reset();
    }
    if (m_modSyncProgressDialog != nullptr)
    {
        m_modSyncProgressDialog->detach();
        m_modSyncProgressDialog.reset();
    }
    m_modSyncProgressDialog = MemoryManagement::create<DialogModSyncProgress>(static_cast<qint32>(modsToDownload.size()));
    // Defensive seed in case MODSYNCMANIFEST raced ahead of dialog construction.
    m_modSyncProgressDialog->setExpectedTotalBytes(m_modSyncExpectedUncompressedTotal);
    connect(m_modSyncProgressDialog.get(), &DialogModSyncProgress::sigCancel, this, [this]()
    {
        if (!m_modSyncActive)
        {
            return;
        }
        cancelModSyncSession();
        onModSyncFailed(tr("Mod sync canceled."));
    }, Qt::QueuedConnection);
    addChild(m_modSyncProgressDialog);
}

void Multiplayermenu::onModSyncProgress()
{
    if (m_modSyncProgressDialog == nullptr)
    {
        return;
    }
    // Compressed drives the EMA network-rate display; uncompressed drives bar fraction and ETA.
    m_modSyncProgressDialog->setProgress(static_cast<qint32>(m_modSyncStagings.size()), m_modSyncReceivedBytes, m_modSyncReceivedUncompressedBytes);
}

void Multiplayermenu::onModSyncSucceeded()
{
    if (m_modSyncProgressDialog != nullptr)
    {
        m_modSyncProgressDialog->detach();
        m_modSyncProgressDialog.reset();
    }
    auto * settings = Settings::getInstance();
    QString rejoinHost = m_serverAddress;
    quint16 rejoinPort = m_serverPort;
    // Prefer the actually-connected endpoint so secondary-fallback joins rejoin to the working address.
    if (m_pNetworkInterface != nullptr)
    {
        const QString connected = m_pNetworkInterface->getConnectedAdress();
        const quint16 connectedPort = m_pNetworkInterface->getConnectedPort();
        if (!connected.isEmpty())
        {
            rejoinHost = connected;
        }
        if (connectedPort != 0)
        {
            rejoinPort = connectedPort;
        }
    }
    QStringList argv;
    // Forward --userPath only when the parent had it on cmdline; passing it otherwise flips CWD-ini boot mode.
    QString restartUserPath;
    if (Mainapp::getInstance()->getParser().getUserPath(restartUserPath))
    {
        argv << QStringLiteral("--userPath=") + restartUserPath;
    }
    const bool haveRejoinTarget = !rejoinHost.isEmpty() && rejoinPort != 0;
    if (haveRejoinTarget && Filesupport::writeRejoinManifest(settings->getUserPath(), rejoinHost, rejoinPort))
    {
        CONSOLE_PRINT("Wrote .rejoin.json for " + rejoinHost + ":" + QString::number(rejoinPort), GameConsole::eINFO);
        // Password only after manifest succeeds; do not leak it on the no-rejoin restart.
        argv << QStringLiteral("--rejoin-password=") + m_password.getPasswordText();
    }
    else if (haveRejoinTarget)
    {
        CONSOLE_PRINT("Failed to write .rejoin.json; user will return to main menu after restart", GameConsole::eERROR);
    }
    Mainapp::setRestartArgv(argv);
    QCoreApplication::exit(1);
}

void Multiplayermenu::onModSyncFailed(const QString & reason)
{
    if (m_modSyncProgressDialog != nullptr)
    {
        m_modSyncProgressDialog->detach();
        m_modSyncProgressDialog.reset();
    }
    // Escape because reason can include host-supplied text and DialogMessageBox renders via setHtmlText.
    const QString safe = reason.toHtmlEscaped();
    spDialogMessageBox pDialog = MemoryManagement::create<DialogMessageBox>(tr("Mod sync failed: %1\n\nLeaving the game.").arg(safe));
    connect(pDialog.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
    addChild(pDialog);
}
