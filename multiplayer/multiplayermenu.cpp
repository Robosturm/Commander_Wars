#include <QJsonObject>
#include <QJsonDocument>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "multiplayer/multiplayermenu.h"
#include "multiplayer/lobbymenu.h"
#include "multiplayer/networkcommands.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"
#include "coreengine/filesupport.h"
#include "coreengine/globalutils.h"
#include "coreengine/sha256hash.h"

#include "menue/gamemenue.h"
#include "menue/mainwindow.h"

#include "network/tcpclient.h"
#include "network/tcpserver.h"
#include "network/mainserver.h"
#include "network/JsonKeys.h"

#include "game/gamemap.h"
#include "game/player.h"

#include "objects/dialogs/filedialog.h"
#include "objects/dialogs/dialogmessagebox.h"
#include "objects/base/moveinbutton.h"

#include "ingamescriptsupport/genericbox.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

Multiplayermenu::Multiplayermenu(QString address, QString secondaryAddress, quint16 port, QString password, NetworkMode networkMode)
    : MapSelectionMapsMenue(spMapSelectionView::create(QStringList({".map", ".jsm"})), Settings::getSmallScreenDevice() ? Settings::getHeight() - 80 : Settings::getHeight() - 380),
      m_networkMode(networkMode),
      m_local(true),
      m_password(password)
{
    init();
    if (m_networkMode != NetworkMode::Host)
    {
        m_pNetworkInterface = spTCPClient::create(nullptr);
        m_pNetworkInterface->setIsObserver(m_networkMode == NetworkMode::Observer);
        m_pNetworkInterface->moveToThread(Mainapp::getInstance()->getNetworkThread());
        m_pPlayerSelection->attachNetworkInterface(m_pNetworkInterface);
        initClientAndWaitForConnection();
        emit m_pNetworkInterface->sig_connect(address, port, secondaryAddress);
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
                                    Settings::getHeight() - m_pHostAdresse->getScaledHeight() - 10);
        m_pHostAdresse->setVisible(false);
        connect(this, &Multiplayermenu::sigShowIPs, this, &Multiplayermenu::showIPs, Qt::QueuedConnection);
    }
}

Multiplayermenu::Multiplayermenu(spNetworkInterface pNetworkInterface, QString password, NetworkMode networkMode)
    : MapSelectionMapsMenue(spMapSelectionView::create(QStringList({".map", ".jsm"})), Settings::getHeight() - 380),
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
        dynamic_cast<Label*>(m_pButtonStart->getFirstChild().get())->setHtmlText(tr("Ready"));
        m_pPlayerSelection->setIsServerGame(true);
    }
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
        dynamic_cast<Label*>(m_pButtonStart->getFirstChild().get())->setHtmlText(tr("Ready"));
    }
    else if (m_networkMode == NetworkMode::Observer)
    {
        m_pButtonStart->setVisible(false);
    }
    // quit on host connection lost
    connect(m_pNetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &Multiplayermenu::disconnected, Qt::QueuedConnection);
    connect(m_pNetworkInterface.get(), &NetworkInterface::recieveData, this, &Multiplayermenu::recieveData, Qt::QueuedConnection);
    connect(m_pPlayerSelection.get(), &PlayerSelection::sigDisconnect, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
    // wait 10 minutes till timeout
    spDialogConnecting pDialogConnecting = spDialogConnecting::create(tr("Connecting"), 1000 * 60 * 5);
    addChild(pDialogConnecting);
    connect(pDialogConnecting.get(), &DialogConnecting::sigCancel, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
    connect(this, &Multiplayermenu::sigConnected, pDialogConnecting.get(), &DialogConnecting::connected, Qt::QueuedConnection);
}

void Multiplayermenu::init()
{
    CONSOLE_PRINT("Entering Multiplayer Menue", Console::eDEBUG);
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_pButtonLoadSavegame = ObjectManager::createButton(tr("Load Savegame"));
    m_pButtonLoadSavegame->setPosition(Settings::getWidth() - m_pButtonLoadSavegame->getScaledWidth() - m_pButtonNext->getScaledWidth() - 20,
                                       Settings::getHeight() - 10 - m_pButtonLoadSavegame->getScaledHeight());
    addChild(m_pButtonLoadSavegame);
    m_pButtonLoadSavegame->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigLoadSaveGame();
    });
    connect(this, &Multiplayermenu::sigLoadSaveGame, this, &Multiplayermenu::showLoadSaveGameDialog, Qt::QueuedConnection);
    connect(&m_GameStartTimer, &QTimer::timeout, this, &Multiplayermenu::countdown, Qt::QueuedConnection);
}

void Multiplayermenu::showIPs()
{
    spGenericBox pGenericBox = spGenericBox::create();
    QStringList items = NetworkInterface::getIPAdresses();
    QSize size(Settings::getWidth() - 40, Settings::getHeight() - 80);
    spPanel pPanel = spPanel::create(true, size, size);
    pPanel->setPosition(20, 20);
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    oxygine::spTextField info = oxygine::spTextField::create();
    info->setStyle(style);
    info->setHtmlText((tr("Please use one of the following IP-Addresses to connect to this Host. Not all IP-Addresses") +
                       tr(" may work for each client depending on their network settings. Please use CMD and the ping command to verify if an IP-Address will work")));
    info->setSize(Settings::getWidth() - 80, 500);
    info->setPosition(10, 10);
    pPanel->addItem(info);
    qint32 starty = 10 + info->getTextRect().getHeight();
    for (qint32 i = 0; i < items.size(); i++)
    {
        oxygine::spTextField text = oxygine::spTextField::create();
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
    // dummy impl for loading
    QStringList wildcards;
    wildcards.append("*.msav");
    QString path = Settings::getUserPath() + "savegames";
    spFileDialog saveDialog = spFileDialog::create(path, wildcards, "", false, tr("Load"));
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
    CONSOLE_PRINT("Multiplayermenu::playerJoined", Console::eDEBUG);
    if (m_pNetworkInterface->getIsServer() &&
       (m_local || Mainapp::getSlave()))
    {
        acceptNewConnection(socketID);
    }
}

void Multiplayermenu::acceptNewConnection(quint64 socketID)
{
    CONSOLE_PRINT("Accepting connection for socket " + QString::number(socketID), Console::eDEBUG);
    auto & cypher = Mainapp::getInstance()->getCypher();
    if (Mainapp::getSlave() &&
        m_hostSocket == 0)
    {
        m_hostSocket = socketID;
    }
    if (Mainapp::getSlave())
    {
        CONSOLE_PRINT("Slave requesting login data", Console::eDEBUG);
        emit m_pNetworkInterface->sig_sendData(socketID, cypher.getPublicKeyMessage(NetworkCommands::PublicKeyActions::RequestLoginData), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
    else
    {
        CONSOLE_PRINT("Requesting public key for initial map update", Console::eDEBUG);
        emit m_pNetworkInterface->sig_sendData(socketID, cypher.getRequestKeyMessage(NetworkCommands::PublicKeyActions::SendInitialMapUpdate), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

void Multiplayermenu::recieveServerData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == NetworkInterface::NetworkSerives::ServerHosting)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        CONSOLE_PRINT("Recieving data from Master. Command received: " + messageType + " for socket " + QString::number(socketID), Console::eDEBUG);
        if (messageType == NetworkCommands::LAUNCHGAMEONSERVER)
        {
            launchGameOnServer(stream);
        }
    }
    else if (service == NetworkInterface::NetworkSerives::ServerHostingJson)
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject objData = doc.object();
        QString messageType = objData.value(JsonKeys::JSONKEY_COMMAND).toString();
        CONSOLE_PRINT("Server Network Command received: " + messageType + " for socket " + QString::number(socketID), Console::eDEBUG);
        if (messageType == NetworkCommands::SLAVEADDRESSINFO)
        {

        }
        else if (messageType == NetworkCommands::SERVERRELAUNCHSLAVE)
        {
            onServerRelaunchSlave(socketID, objData);
        }
        else
        {
            CONSOLE_PRINT("Unknown command " + messageType + " received", Console::eDEBUG);
        }
    }
}

void Multiplayermenu::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    // data for us?
    if (service == NetworkInterface::NetworkSerives::Multiplayer)
    {
        if (!m_pPlayerSelection->hasNetworkInterface())
        {
            m_pPlayerSelection->attachNetworkInterface(m_pNetworkInterface);
        }
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        CONSOLE_PRINT("Local Network Command received: " + messageType + " for socket " + QString::number(socketID), Console::eDEBUG);
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
                CONSOLE_PRINT("Checking if server game should start", Console::eDEBUG);
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
        else
        {
            CONSOLE_PRINT("Unknown command " + messageType + " received", Console::eDEBUG);
        }
    }
    else if (service == NetworkInterface::NetworkSerives::ServerHostingJson)
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject objData = doc.object();
        QString messageType = objData.value(JsonKeys::JSONKEY_COMMAND).toString();
        CONSOLE_PRINT("Server Network Command received: " + messageType + " for socket " + QString::number(socketID), Console::eDEBUG);
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
            pDialogMessageBox = spDialogMessageBox::create(tr("Server doesn't have the given mods installed."));
            connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
            addChild(pDialogMessageBox);
        }
        else if (messageType == NetworkCommands::SERVERNOGAMESLOTSAVAILABLE)
        {
            spDialogMessageBox pDialogMessageBox;
            pDialogMessageBox = spDialogMessageBox::create(tr("Server doesn't have any more slots for players."));
            connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
            addChild(pDialogMessageBox);
        }
        else if (messageType == NetworkCommands::REQUESTPUBLICKEY)
        {
            auto & cypher = Mainapp::getInstance()->getCypher();
            auto action = static_cast<NetworkCommands::PublicKeyActions>(objData.value(JsonKeys::JSONKEY_RECEIVEACTION).toInt());
            emit m_pNetworkInterface->sig_sendData(socketID, cypher.getPublicKeyMessage(action), NetworkInterface::NetworkSerives::ServerHostingJson, false);
        }
        else if (messageType == NetworkCommands::SENDPUBLICKEY)
        {
            auto action = static_cast<NetworkCommands::PublicKeyActions>(objData.value(JsonKeys::JSONKEY_RECEIVEACTION).toInt());
            if (action == NetworkCommands::PublicKeyActions::SendInitialMapUpdate)
            {
                sendMapInfoUpdate(socketID, objData, action);
            }
            else if (action == NetworkCommands::PublicKeyActions::RequestLoginData)
            {
                sendLoginData(socketID, objData, action);
            }
            else
            {
                CONSOLE_PRINT("Unknown public key action " + QString::number(static_cast<qint32>(action)) + " received", Console::eDEBUG);
            }
        }
        else if (messageType == NetworkCommands::CRYPTEDMESSAGE)
        {
            auto action = static_cast<NetworkCommands::PublicKeyActions>(objData.value(JsonKeys::JSONKEY_RECEIVEACTION).toInt());
            if (action == NetworkCommands::PublicKeyActions::SendInitialMapUpdate)
            {
                auto & cypher = Mainapp::getInstance()->getCypher();
                recieveData(socketID, cypher.getDecryptedMessage(doc), NetworkInterface::NetworkSerives::Multiplayer);
            }
            else if (action == NetworkCommands::PublicKeyActions::RequestLoginData)
            {
                auto & cypher = Mainapp::getInstance()->getCypher();
                recieveData(socketID, cypher.getDecryptedMessage(doc), NetworkInterface::NetworkSerives::ServerHostingJson);
            }
            else
            {
                CONSOLE_PRINT("Unknown crypted message action " + QString::number(static_cast<qint32>(action)) + " received", Console::eDEBUG);
            }
        }
        else if (messageType == NetworkCommands::DISCONNECTINGFOFROMSERVER)
        {
            showDisconnectReason(socketID, objData);
        }
        else if (messageType == NetworkCommands::REQUESTUSERNAME)
        {
            sendUsername(socketID, objData);
        }
        else
        {
            CONSOLE_PRINT("Unknown command " + messageType + " received", Console::eDEBUG);
        }
    }
}

void Multiplayermenu::showDisconnectReason(quint64 socketID, const QJsonObject & objData)
{
    CONSOLE_PRINT("Showing disconnect reason and exiting menu", Console::eDEBUG);
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
    spDialogMessageBox pDialog = spDialogMessageBox::create(reasons[type]);
    addChild(pDialog);
    if (m_pDialogConnecting.get() != nullptr)
    {
        m_pDialogConnecting->detach();
        m_pDialogConnecting = nullptr;
    }
    emit m_pNetworkInterface->sigDisconnectClient(0);
    buttonBack();
}

void Multiplayermenu::sendUsername(quint64 socketID, const QJsonObject & objData)
{
    QString command = QString(NetworkCommands::SENDUSERNAME);
    CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getUsername());
    QJsonDocument doc(data);
    emit m_pNetworkInterface->sig_sendData(socketID, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void Multiplayermenu::sendLoginData(quint64 socketID, const QJsonObject & objData, NetworkCommands::PublicKeyActions action)
{
    auto & cypher = Mainapp::getInstance()->getCypher();
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, NetworkCommands::VERIFYLOGINDATA);
    Password serverPassword;
    QString password = Settings::getServerPassword();
    serverPassword.setPassword(password);
    data.insert(JsonKeys::JSONKEY_PASSWORD, cypher.toJsonArray(serverPassword.getHash()));
    data.insert(JsonKeys::JSONKEY_USERNAME, Settings::getUsername());
    // send map data to client and make sure password message is crypted
    QString publicKey = objData.value(JsonKeys::JSONKEY_PUBLICKEY).toString();
    QJsonDocument doc(data);
    CONSOLE_PRINT("Sending login data to slave", Console::eDEBUG);
    emit m_pNetworkInterface->sig_sendData(socketID, cypher.getEncryptedMessage(publicKey, action, doc.toJson()).toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void Multiplayermenu::verifyLoginData(const QJsonObject & objData, quint64 socketID)
{
    auto & cypher = Mainapp::getInstance()->getCypher();
    QString username =  objData.value(JsonKeys::JSONKEY_USERNAME).toString();
    QByteArray password = cypher.toByteArray(objData.value(JsonKeys::JSONKEY_PASSWORD).toArray());
    GameEnums::LoginError valid = MainServer::verifyLoginData(username, password);
    if (valid == GameEnums::LoginError_None)
    {
        CONSOLE_PRINT("Client login data are valid. Requesting public key for initial map update", Console::eDEBUG);
        emit m_pNetworkInterface->sig_sendData(socketID, cypher.getRequestKeyMessage(NetworkCommands::PublicKeyActions::SendInitialMapUpdate), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
    else
    {
        CONSOLE_PRINT("Client login data are invalid. Closing connection.", Console::eDEBUG);
        QString command = QString(NetworkCommands::DISCONNECTINGFOFROMSERVER);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
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
        CONSOLE_PRINT("Login error: " + QString::number(valid) + " reported reason: " + QString::number(reason), Console::eDEBUG);
        data.insert(JsonKeys::JSONKEY_DISCONNECTREASON, reason);
        QJsonDocument doc(data);
        emit m_pNetworkInterface->sig_sendData(0, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

void Multiplayermenu::sendMapInfoUpdate(quint64 socketID, const QJsonObject & objData, NetworkCommands::PublicKeyActions action)
{
    Sha256Hash myHash;
    QString file = m_pMapSelectionView->getCurrentFile().filePath();
    QString fileName = m_pMapSelectionView->getCurrentFile().fileName();
    QString scriptFile = m_pMapSelectionView->getCurrentMap()->getGameScript()->getScriptFile();
    QFile mapFile(file);
    if (mapFile.exists())
    {
        mapFile.open(QIODevice::ReadOnly);
        myHash.addData(mapFile.readAll());
        mapFile.close();
    }
    QString command = QString(NetworkCommands::MAPINFO);
    CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
    QByteArray hash = myHash.result();
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << command;
    stream << Mainapp::getGameVersion();
    QStringList mods = Settings::getMods();
    QStringList versions = Settings::getActiveModVersions();
    bool filter = m_pMapSelectionView->getCurrentMap()->getGameRules()->getCosmeticModsAllowed();
    Settings::filterCosmeticMods(mods, versions, filter);
    stream << filter;
    stream << static_cast<qint32>(mods.size());
    for (qint32 i = 0; i < mods.size(); i++)
    {
        stream << mods[i];
        stream << versions[i];
    }
    auto hostHash = Filesupport::getRuntimeHash(mods);
    if (Console::eDEBUG >= Console::getLogLevel())
    {
        QString hostString = GlobalUtils::getByteArrayString(hostHash);
        CONSOLE_PRINT("Sending host hash: " + hostString, Console::eDEBUG);
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
            Sha256Hash myScriptHash;
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
    auto & cypher = Mainapp::getInstance()->getCypher();
    QString publicKey = objData.value(JsonKeys::JSONKEY_PUBLICKEY).toString();
    emit m_pNetworkInterface->sig_sendData(socketID, cypher.getEncryptedMessage(publicKey, action, data).toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void Multiplayermenu::connectToSlave(const QJsonObject & objData, quint64 socketID)
{
    CONSOLE_PRINT("Connected to slave", Console::eDEBUG);
    QString address = objData.value(JsonKeys::JSONKEY_ADDRESS).toString();
    QString secondarySlaveAddress = objData.value(JsonKeys::JSONKEY_SECONDARYADDRESS).toString();
    quint16 port = objData.value(JsonKeys::JSONKEY_PORT).toInteger();
    disconnectNetworkSlots();
    m_pNetworkInterface = spTCPClient::create(nullptr);
    m_pNetworkInterface->setIsObserver(m_networkMode == NetworkMode::Observer);
    m_pNetworkInterface->moveToThread(Mainapp::getInstance()->getNetworkThread());
    m_pPlayerSelection->attachNetworkInterface(m_pNetworkInterface);
    createChat();
    connectNetworkSlots();
    emit m_pNetworkInterface->sig_connect(address, port, secondarySlaveAddress);
}

void Multiplayermenu::onSlaveConnectedToMaster(quint64 socketID)
{
    CONSOLE_PRINT("Connected to master", Console::eDEBUG);
    spTCPClient pSlaveMasterConnection = Mainapp::getSlaveClient();
    QString command = NetworkCommands::SLAVEREADY;
    QJsonObject data;
    QString slavename = Settings::getSlaveServerName();
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_SLAVENAME, slavename);
    QJsonDocument doc(data);
    CONSOLE_PRINT("Sending command " + command + " for slave " + slavename, Console::eDEBUG);
    emit pSlaveMasterConnection->sig_sendData(socketID, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void Multiplayermenu::onServerRelaunchSlave(quint64 socketID, const QJsonObject & objData)
{
    QString savefile = objData.value(JsonKeys::JSONKEY_MAPNAME).toString();
    if (QFile::exists(savefile))
    {
        CONSOLE_PRINT("Relaunching slave with savefile " + savefile, Console::eERROR);
        spGameMap pMap = spGameMap::create(savefile, false, false, true);
        spGameMenue pMenu = spGameMenue::create(pMap, true, m_pNetworkInterface, false, true);
        oxygine::Stage::getStage()->addChild(pMenu);
        QString command = NetworkCommands::SLAVERELAUNCHED;
        QJsonObject data;
        QString slavename = Settings::getSlaveServerName();
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        data.insert(JsonKeys::JSONKEY_SLAVENAME, slavename);
        QJsonDocument doc(data);
        CONSOLE_PRINT("Sending command " + command + " for slave " + slavename, Console::eDEBUG);
        spTCPClient pSlaveMasterConnection = Mainapp::getSlaveClient();
        emit pSlaveMasterConnection->sig_sendData(socketID, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
        oxygine::Actor::detach();
    }
    else
    {
        CONSOLE_PRINT("Failed to relaunch slave with savefile " + savefile, Console::eERROR);
        QCoreApplication::exit(0);
    }
}

void Multiplayermenu::receiveCurrentGameState(QDataStream & stream, quint64 socketID)
{
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    if (pMap.get() != nullptr)
    {
        pMap->detach();
    }
    m_pMapSelectionView->setCurrentMap(spGameMap());
    pMap = spGameMap::create<QDataStream &, bool>(stream, true);
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
            sendStream << command;
            sendStream << Settings::getUsername();
            emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
        }
    }
    else
    {
        CONSOLE_PRINT("Incorrect Password found.", Console::eDEBUG);
        CONSOLE_PRINT("Entered password hash: " + GlobalUtils::getByteArrayString(m_password.getHash()), Console::eDEBUG);
        CONSOLE_PRINT("Host    password hash: " + GlobalUtils::getByteArrayString(m_pMapSelectionView->getCurrentMap()->getGameRules()->getPassword().getHash()), Console::eDEBUG);
        spDialogMessageBox pDialogMessageBox;
        pDialogMessageBox = spDialogMessageBox::create(tr("Wrong password entered for joining the game."));
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
            pPlayer->setBaseGameInput(BaseGameInputIF::createAi(pMap.get(), aiTypes[playerAis.indexOf(i)]));
        }
        else if (pInput != nullptr &&
                 pInput->getAiType() != GameEnums::AiTypes_ProxyAi &&
                 pInput->getAiType() != GameEnums::AiTypes_Closed &&
                 pInput->getAiType() != GameEnums::AiTypes_Open)
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
    CONSOLE_PRINT("Leaving Map Selection Menue and rejoining game", Console::eDEBUG);
    auto window = spGameMenue::create(pMap, true, m_pNetworkInterface, true);
    window->getActionPerformer().setSyncCounter(syncCounter);
    oxygine::Stage::getStage()->addChild(window);

    QString command = NetworkCommands::RECEIVEDCURRENTGAMESTATE;
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream << command;
    CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
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
        sendStream << command;
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
    }
    else
    {
        QString command = NetworkCommands::JOINASPLAYER;
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << command;
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
    }
}

void Multiplayermenu::requestRule(quint64 socketID)
{
    // a client requested the current map rules set by the server
    if (m_pNetworkInterface->getIsServer())
    {
        QString command = QString(NetworkCommands::SENDINITUPDATE);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
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
            if (aiType == GameEnums::AiTypes_Human && !m_saveGame)
            {
                sendStream << pMap->getPlayer(i)->getPlayerNameId();
                sendStream << static_cast<qint32>(GameEnums::AiTypes_ProxyAi);
            }
            else
            {
                sendStream << pMap->getPlayer(i)->getPlayerNameId();
                if (m_pPlayerSelection->isOpenPlayer(i))
                {
                    sendStream << static_cast<qint32>(GameEnums::AiTypes_Open);
                }
                else if (m_pPlayerSelection->isClosedPlayer(i))
                {
                    sendStream << static_cast<qint32>(GameEnums::AiTypes_Closed);
                }
                else
                {
                    sendStream << static_cast<qint32>(GameEnums::AiTypes_ProxyAi);
                }
            }
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
            CONSOLE_PRINT("Incorrect Password found.", Console::eDEBUG);
            CONSOLE_PRINT("Entered password hash: " + GlobalUtils::getByteArrayString(m_password.getHash()), Console::eDEBUG);
            CONSOLE_PRINT("Host    password hash: " + GlobalUtils::getByteArrayString(m_pMapSelectionView->getCurrentMap()->getGameRules()->getPassword().getHash()), Console::eDEBUG);
            spDialogMessageBox pDialogMessageBox;
            pDialogMessageBox = spDialogMessageBox::create(tr("Wrong password entered for joining the game."));
            connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
            addChild(pDialogMessageBox);
        }
        else
        {
            bool campaign = false;
            stream >> campaign;
            if (campaign)
            {
                m_pMapSelectionView->getCurrentMap()->setCampaign(spCampaign::create());
                m_pMapSelectionView->getCurrentMap()->getCampaign()->deserializeObject(stream);
            }
            CONSOLE_PRINT("Reading players count: " + QString::number(m_pMapSelectionView->getCurrentMap()->getPlayerCount()), Console::eDEBUG);
            for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getPlayerCount(); i++)
            {
                QString name;
                qint32 aiType;
                stream >> name;
                stream >> aiType;
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
                sendStream << command;
                CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
                emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
            }
            else
            {
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
        readHashInfo(stream, socketID, mods, sameMods, differentHash, sameVersion);
        if (sameVersion && sameMods && !differentHash)
        {
            QString command = QString(NetworkCommands::GAMEDATAVERIFIED);
            CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream << command;
            emit m_pNetworkInterface->sig_sendData(socketID, data, NetworkInterface::NetworkSerives::Multiplayer, false);
        }
        else
        {
            handleVersionMissmatch(mods, sameMods, differentHash, sameVersion);
        }
    }
}

bool Multiplayermenu::checkMods(const QStringList & mods, const QStringList & versions, bool filter)
{
    QStringList myVersions = Settings::getActiveModVersions();
    QStringList myMods = Settings::getMods();
    Settings::filterCosmeticMods(myMods, myVersions, filter);
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

void Multiplayermenu::readHashInfo(QDataStream & stream, quint64 socketID, QStringList & mods, bool & sameMods, bool & differentHash, bool & sameVersion)
{
    QString version;
    stream >> version;
    bool filter = false;
    stream >> filter;
    qint32 size = 0;
    stream >> size;
    QStringList versions;
    for (qint32 i = 0; i < size; i++)
    {
        QString mod;
        stream >> mod;
        mods.append(mod);
        QString version;
        stream >> version;
        versions.append(version);
    }
    sameMods = checkMods(mods, versions, filter);
    QByteArray hostRuntime = Filesupport::readByteArray(stream);
    QByteArray ownRuntime = Filesupport::getRuntimeHash(mods);
    if (Console::eDEBUG >= Console::getLogLevel())
    {
        QString hostString = GlobalUtils::getByteArrayString(hostRuntime);
        QString ownString = GlobalUtils::getByteArrayString(ownRuntime);
        CONSOLE_PRINT("Received host hash: " + hostString, Console::eDEBUG);
        CONSOLE_PRINT("Own hash:           " + ownString, Console::eDEBUG);
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
        readHashInfo(stream, socketID, mods, sameMods, differentHash, sameVersion);
        if (sameVersion && sameMods && !differentHash)
        {
            stream >> m_saveGame;
            m_pPlayerSelection->setSaveGame(m_saveGame);
            if (m_saveGame)
            {
                QString command = QString(NetworkCommands::REQUESTRULE);
                CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
                spGameMap pMap = spGameMap::create<QDataStream &, bool>(stream, m_saveGame);
                m_pMapSelectionView->setCurrentMap(pMap);
                loadMultiplayerMap();
                QByteArray sendData;
                QDataStream sendStream(&sendData, QIODevice::WriteOnly);
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
                CONSOLE_PRINT("Checking for map " + fileName + " and script " + scriptFile , Console::eDEBUG);
                if (!scriptFile.isEmpty())
                {
                    stream >> scriptHash;
                }
                if (!fileName.startsWith(NetworkCommands::RANDOMMAPIDENTIFIER) &&
                    !fileName.startsWith(NetworkCommands::SERVERMAPIDENTIFIER) &&
                    existsMap(fileName, hash, scriptFile, scriptHash))
                {
                    QString command = QString(NetworkCommands::REQUESTRULE);
                    CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
                    QByteArray sendData;
                    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                    sendStream << command;
                    emit m_pNetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
                }
                else
                {
                    QString command = QString(NetworkCommands::REQUESTMAP);
                    CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
                    QByteArray sendData;
                    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                    sendStream << command;
                    emit m_pNetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
                }
            }
        }
        else
        {
            handleVersionMissmatch(mods, sameMods, differentHash, sameVersion);
        }
    }
}

void Multiplayermenu::handleVersionMissmatch(const QStringList & mods, bool sameMods, bool differentHash, bool sameVersion)
{
    // quit game with wrong version
    spDialogMessageBox pDialogMessageBox;
    if (differentHash)
    {
        pDialogMessageBox = spDialogMessageBox::create(tr("Host has a different version of a mod or the game resource folder has been modified by one of the games."));
    }
    else  if (!sameVersion)
    {
        pDialogMessageBox = spDialogMessageBox::create(tr("Host has a different game version. Leaving the game again."));
    }
    else if (!sameMods)
    {
        QString hostMods;
        for (auto & mod : mods)
        {
            hostMods += Settings::getModName(mod) + "\n";
        }
        QStringList myModsList = Settings::getMods();
        QString myMods;
        for (auto & mod : myModsList)
        {
            myMods += Settings::getModName(mod) + "\n";
        }
        pDialogMessageBox = spDialogMessageBox::create(tr("Host has  different mods. Leaving the game again.\nHost mods: ") + hostMods + "\nYour Mods: " + myMods);
    }
    else
    {
        pDialogMessageBox = spDialogMessageBox::create(tr("Failed to join game due to unknown verification failure."));
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
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        QString file = GlobalUtils::makePathRelative(m_pMapSelectionView->getCurrentFile().filePath(), false);
        QString scriptFile = m_pMapSelectionView->getCurrentMap()->getGameScript()->getScriptFile();
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
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

        spGameMap pMap = m_pMapSelectionView->getCurrentMap();
        if (pMap.get() != nullptr)
        {
            pMap->detach();
        }
        if (mapFile.startsWith(NetworkCommands::RANDOMMAPIDENTIFIER) ||
            mapFile.startsWith(NetworkCommands::SERVERMAPIDENTIFIER))
        {
            pMap = spGameMap::create<QDataStream &, bool>(stream, m_saveGame);
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
                            Filesupport::writeBytes(scriptFilestream, scriptData);
                            script.close();
                        }
                        else
                        {
                            CONSOLE_PRINT("Unable to write downloaded script-file", Console::eDEBUG);
                            spDialogMessageBox pDialogMessageBox;
                            pDialogMessageBox = spDialogMessageBox::create(tr("Unable to download script file from host."));
                            connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::buttonBack, Qt::QueuedConnection);
                            addChild(pDialogMessageBox);
                        }
                    }
                    pMap = spGameMap::create(mapFile, true, false, m_saveGame);
                }
                else
                {
                    CONSOLE_PRINT("Unable to write downloaded map-file", Console::eDEBUG);
                    spDialogMessageBox pDialogMessageBox;
                    pDialogMessageBox = spDialogMessageBox::create(tr("Unable to download map file from host."));
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
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        loadMultiplayerMap();
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << command;
        emit m_pNetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
    }
}

void Multiplayermenu::launchGameOnServer(QDataStream & stream)
{
    CONSOLE_PRINT("Launching Game on Slave", Console::eDEBUG);
    hideMapSelection();
    QStringList mods;
    mods = Filesupport::readVectorList<QString, QList>(stream);
    spGameMap pMap = spGameMap::create<QDataStream &, bool>(stream, m_saveGame);
    stream >> m_saveGame;    
    CONSOLE_PRINT("Is save game" + QString::number(m_saveGame), Console::eDEBUG);
    m_pPlayerSelection->setSaveGame(m_saveGame);
    m_pMapSelectionView->setCurrentMap(pMap);
    m_pMapSelectionView->setCurrentFile(NetworkCommands::SERVERMAPIDENTIFIER);
    m_pPlayerSelection->attachNetworkInterface(m_pNetworkInterface);
    m_pPlayerSelection->setIsServerGame(true);
    loadMultiplayerMap();
    createChat();
    // open all players
    if (m_saveGame)
    {
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
        m_pPlayerSelection->setPlayerAi(0, GameEnums::AiTypes::AiTypes_Open, "");
    }
    sendSlaveReady();
    m_slaveGameReady = true;
}

void Multiplayermenu::sendSlaveReady()
{
    QString command = QString(NetworkCommands::GAMERUNNINGONSERVER);
    QString slavename = Settings::getSlaveServerName();
    CONSOLE_PRINT("Sending command " + command + " for slave " + slavename, Console::eDEBUG);
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_SLAVENAME, slavename);
    data.insert(JsonKeys::JSONKEY_GAMEDESCRIPTION, pMap->getGameRules()->getDescription());
    if (pMap->getGameRules()->getPassword().isValidPassword(""))
    {
        data.insert(JsonKeys::JSONKEY_HASPASSWORD, false);
    }
    else
    {
        data.insert(JsonKeys::JSONKEY_HASPASSWORD, true);
    }
    QJsonDocument doc(data);
    emit Mainapp::getSlaveClient()->sig_sendData(0, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, true);
}

void Multiplayermenu::slotCancelHostConnection()
{
    CONSOLE_PRINT("Canceled host connection", Console::eDEBUG);
    buttonBack();
}

void Multiplayermenu::slotHostGameLaunched()
{
    m_pDialogConnecting = nullptr;
    // we're hosting a game so we get the same rights as a local host
    m_pNetworkInterface->setIsServer(true);
    createChat();
    MapSelectionMapsMenue::buttonNext();
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
    QByteArray mapData;
    mapData = Filesupport::readByteArray(stream);
    Filesupport::writeBytes(mapFilestream, mapData);
    map.close();
    pNewMap = spGameMap::create(mapFile, true, false, m_saveGame);
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
        Filesupport::writeBytes(scriptFilestream, scriptData);
        script.close();
        scriptFile = GlobalUtils::makePathRelative(scriptFile, true);
        // save script file
        pNewMap->getGameScript()->setScriptFile(scriptFile);
        map.open(QIODevice::WriteOnly);
        QDataStream stream(&map);
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

void Multiplayermenu::loadMultiplayerMap()
{    
    m_pMapSelectionView->getCurrentMap()->getGameScript()->init();
    m_pMapSelectionView->updateMapData();
    showPlayerSelection();
}

void Multiplayermenu::initClientGame(quint64, QDataStream &stream)
{
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    pMap->setVisible(false);
    if (!m_saveGame)
    {
        pMap->initPlayers();
    }
    quint32 seed;
    stream >> seed;
    for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getPlayerCount(); i++)
    {
        GameEnums::AiTypes aiType = GameEnums::AiTypes::AiTypes_Closed;
        auto* baseGameInput = m_pMapSelectionView->getCurrentMap()->getPlayer(i)->getBaseGameInput();        
        if (baseGameInput != nullptr)
        {
            aiType = baseGameInput->getAiType();
        }
        CONSOLE_PRINT("Creating AI for player " + QString::number(i) + " of type " + QString::number(aiType), Console::eDEBUG);
        spGameMap pMap = m_pMapSelectionView->getCurrentMap();
        pMap->getPlayer(i)->deserializeObject(stream);
        pMap->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(pMap.get(), aiType));
    }
    GlobalUtils::seed(seed);
    GlobalUtils::setUseSeed(true);

    if (!m_saveGame)
    {
        pMap->getGameScript()->gameStart();
    }
    pMap->updateSprites();
    // start game
    m_pNetworkInterface->setIsServer(false);
    CONSOLE_PRINT("Leaving Map Selection Menue and init client game", Console::eDEBUG);
    auto window = spGameMenue::create(pMap, m_saveGame, m_pNetworkInterface, false);
    oxygine::Stage::getStage()->addChild(window);
    // send game started
    QString command = QString(NetworkCommands::CLIENTINITGAME);
    CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream << command;
    sendStream << m_pNetworkInterface->getSocketID();
    emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
}

bool Multiplayermenu::existsMap(QString& fileName, QByteArray& hash, QString& scriptFileName, QByteArray& scriptHash)
{
    QString path = "maps";
    QStringList filter;
    filter << "*" + fileName;
    QDirIterator dirIter(Settings::getUserPath() + path, filter, QDir::Files, QDirIterator::Subdirectories);
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
            if (QFile::exists(Settings::getUserPath() + scriptFileName))
            {
                scriptFile.setFileName(Settings::getUserPath() + scriptFileName);
                scriptFile.open(QIODevice::ReadOnly);
                Sha256Hash myHash;
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
                Sha256Hash myHash;
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
        Sha256Hash myHash;
        myHash.addData(&mapFile);
        mapFile.close();
        QByteArray myHashArray = myHash.result();
        if (hash == myHashArray)
        {
            spGameMap pMap = spGameMap::create(file, true, false, m_saveGame);
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
    m_pRuleSelectionView = spRuleSelection::create(pMap.get(), Settings::getWidth() - 80, RuleSelection::Mode::Multiplayer);
    connect(m_pRuleSelectionView.get(), &RuleSelection::sigSizeChanged, this, &Multiplayermenu::ruleSelectionSizeChanged, Qt::QueuedConnection);
    m_pRuleSelection->addItem(m_pRuleSelectionView);
    m_pRuleSelection->setContentHeigth(m_pRuleSelectionView->getScaledHeight() + 40);
    m_pRuleSelection->setContentWidth(m_pRuleSelectionView->getScaledWidth());
}

void Multiplayermenu::showPlayerSelection()
{
    MapSelectionMapsMenue::showPlayerSelection();
    if (m_networkMode == NetworkMode::Observer)
    {
        m_pButtonStart->setVisible(false);
    }
}

void Multiplayermenu::exitMenu()
{
    CONSOLE_PRINT("Leaving Map Selection Menue and going back to main menu", Console::eDEBUG);
    spMainwindow window = spMainwindow::create("ui/menu/mainmenu.xml");
    oxygine::Stage::getStage()->addChild(window);
    oxygine::Actor::detach();
}

void Multiplayermenu::disconnected(quint64 socket)
{
    CONSOLE_PRINT("Multiplayermenu::disconnected", Console::eDEBUG);
    if (m_networkMode == NetworkMode::Host)
    {
        // handled in player selection
        if (Mainapp::getSlave() && m_hostSocket == socket)
        {
            CONSOLE_PRINT("Closing slave cause the host has disconnected.", Console::eDEBUG);
            QCoreApplication::exit(0);
        }
    }
    else
    {
        CONSOLE_PRINT("Leaving Map Selection Menue", Console::eDEBUG);
        disconnectNetwork();
        oxygine::Stage::getStage()->addChild(spLobbyMenu::create());
        oxygine::Actor::detach();
    }
}

void Multiplayermenu::buttonBack()
{
    if (m_networkMode != NetworkMode::Host ||
        m_MapSelectionStep == MapSelectionStep::selectMap ||
        !m_local)
    {
        CONSOLE_PRINT("Leaving Map Selection Menue button back pressed", Console::eDEBUG);
        disconnectNetwork();
        oxygine::Stage::getStage()->addChild(spLobbyMenu::create());
        oxygine::Actor::detach();
    }
    else if (m_networkMode == NetworkMode::Host)
    {
        m_pHostAdresse->setVisible(false);
        if (m_Chat.get() != nullptr)
        {
            m_Chat->detach();
            m_Chat = nullptr;
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

void Multiplayermenu::buttonNext()
{
    if (m_networkMode == NetworkMode::Host &&
        m_MapSelectionStep == MapSelectionStep::selectRules)
    {
        spGameMap pMap = m_pMapSelectionView->getCurrentMap();
        m_password.setPassword(pMap->getGameRules()->getPassword());
        if (m_local)
        {
            m_pHostAdresse->setVisible(true);
            m_pNetworkInterface = spTCPServer::create(nullptr);
            m_pNetworkInterface->moveToThread(Mainapp::getInstance()->getNetworkThread());
            m_pPlayerSelection->attachNetworkInterface(m_pNetworkInterface);
            createChat();
            connectNetworkSlots();
            emit m_pNetworkInterface->sig_connect("", Settings::getGamePort(), "");
            MapSelectionMapsMenue::buttonNext();
        }
        else
        {
            MapSelectionMapsMenue::hideRuleSelection();
            connectNetworkSlots();
            startGameOnServer();
        }
    }
    else
    {
        MapSelectionMapsMenue::buttonNext();
    }
}

void Multiplayermenu::connectNetworkSlots()
{
    connect(m_pNetworkInterface.get(), &NetworkInterface::sigConnected, this, &Multiplayermenu::playerJoined, Qt::QueuedConnection);
    connect(m_pNetworkInterface.get(), &NetworkInterface::recieveData, this, &Multiplayermenu::recieveData, Qt::QueuedConnection);
    connect(m_pNetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &Multiplayermenu::disconnected, Qt::QueuedConnection);
}

void Multiplayermenu::disconnectNetworkSlots()
{
    disconnect(m_pNetworkInterface.get(), &NetworkInterface::sigConnected, this, &Multiplayermenu::playerJoined);
    disconnect(m_pNetworkInterface.get(), &NetworkInterface::recieveData, this, &Multiplayermenu::recieveData);
    disconnect(m_pNetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &Multiplayermenu::disconnected);
}

void Multiplayermenu::startGameOnServer()
{
    QString command = QString(NetworkCommands::LAUNCHGAMEONSERVER);
    CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream << command;
    Filesupport::writeVectorList(sendStream, Settings::getMods());

    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    pMap->serializeObject(sendStream);
    sendStream << m_saveGame;
    emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::ServerHosting, false);

    m_pDialogConnecting = spDialogConnecting::create(tr("Launching game on server"), 1000 * 60 * 5);
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
    if (Settings::getSmallScreenDevice())
    {
        m_Chat = spChat::create(m_pNetworkInterface,
                                QSize(Settings::getWidth() - 60, Settings::getHeight() - 90),
                                NetworkInterface::NetworkSerives::GameChat, nullptr);
        m_Chat->setPosition(-m_Chat->getScaledWidth() + 1, 10);
        auto moveButton = spMoveInButton::create(m_Chat.get(), m_Chat->getScaledWidth(), 1, -1, 1.0f);
        m_Chat->addChild(moveButton);
    }
    else
    {
        m_Chat = spChat::create(m_pNetworkInterface,
                                QSize(Settings::getWidth() - 20, 300),
                                NetworkInterface::NetworkSerives::GameChat, nullptr);
        m_Chat->setPosition(10, Settings::getHeight() - 360);
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
            m_Chat = nullptr;
        }
        m_pPlayerSelection->attachNetworkInterface(spNetworkInterface());
        m_pNetworkInterface = nullptr;
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
            CONSOLE_PRINT("Game not ready cause player " + QString::number(i) + " is open", Console::eDEBUG);
            gameReady = false;
            break;
        }
        else if (aiType == GameEnums::AiTypes_ProxyAi ||
                 (pInput != nullptr &&
                  pInput->getAiType() == GameEnums::AiTypes_ProxyAi))
        {
            if (m_pPlayerSelection->getReady(i) == false)
            {
                CONSOLE_PRINT("Game not ready cause proxy player " + QString::number(i) + " is not ready", Console::eDEBUG);
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
    CONSOLE_PRINT("Game ready " + QString::number(gameReady) + " and remote player found " + QString::number(hasRemotePlayer), Console::eDEBUG);
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
        QString command = QString(NetworkCommands::STARTSERVERGAME);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        markGameReady();
        changeButtonText();
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << command;
        emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
    }
}

void Multiplayermenu::markGameReady()
{
    QString command = QString(NetworkCommands::CLIENTREADY);
    CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
    m_pPlayerSelection->setPlayerReady(!m_pPlayerSelection->getPlayerReady());
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream << command;
    sendStream << m_pPlayerSelection->getPlayerReady();
    emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
}

void Multiplayermenu::changeButtonText()
{
    if (m_pPlayerSelection->getPlayerReady())
    {
        dynamic_cast<Label*>(m_pButtonStart->getFirstChild().get())->setHtmlText(tr("Not Ready"));
    }
    else
    {
        dynamic_cast<Label*>(m_pButtonStart->getFirstChild().get())->setHtmlText(tr("Ready"));
    }
}

void Multiplayermenu::startCountdown()
{
    m_counter = 5;
    // can we start the game?
    if (getGameReady())
    {
        if (!m_GameStartTimer.isActive())
        {
            CONSOLE_PRINT("Starting countdown", Console::eDEBUG);
            sendServerReady(true);
            m_GameStartTimer.setInterval(std::chrono::seconds(1));
            m_GameStartTimer.setSingleShot(false);
            m_GameStartTimer.start();
            emit m_Chat->sigSendText(QString::number(m_counter) + "...");
        }
    }
    else
    {
        CONSOLE_PRINT("Stoping countdown", Console::eDEBUG);
        m_GameStartTimer.stop();
        sendServerReady(false);
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
        QVector<qint32> player;
        for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getPlayerCount(); i++)
        {
            GameEnums::AiTypes aiType = m_pPlayerSelection->getPlayerAiType(i);
            if (aiType != GameEnums::AiTypes_Open &&
                aiType != GameEnums::AiTypes_ProxyAi)
            {
                player.append(i);
            }
        }
        CONSOLE_PRINT("Setting player ready information to local players with value "  + QString::number(value), Console::eDEBUG);
        m_pPlayerSelection->setPlayerReady(value);
        CONSOLE_PRINT("Sending ready information to all players with value " + QString::number(value), Console::eDEBUG);
        m_pPlayerSelection->sendPlayerReady(0, player, value);
    }
}

void Multiplayermenu::countdown()
{
    if (getGameReady())
    {
        m_counter--;
        if (m_Chat.get() != nullptr)
        {
            CONSOLE_PRINT("Sending game counter..." + QString::number(m_counter), Console::eDEBUG);
            emit m_Chat->sigSendText(QString::number(m_counter) + "...");
        }
        if (m_counter == 0 && m_pNetworkInterface.get() != nullptr)
        {
            CONSOLE_PRINT("Starting game on server", Console::eDEBUG);
            defeatClosedPlayers();

            spGameMap pMap = m_pMapSelectionView->getCurrentMap();
            pMap->setVisible(false);
            if (!m_saveGame)
            {
                pMap->initPlayersAndSelectCOs();
            }
            QString command = QString(NetworkCommands::INITGAME);
            CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream << command;
            quint32 seed = QRandomGenerator::global()->bounded(std::numeric_limits<quint32>::max());
            stream << seed;
            for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getPlayerCount(); i++)
            {
                CONSOLE_PRINT("AI on server for player " + QString::number(i) + " is " + QString::number(pMap->getPlayer(i)->getBaseGameInput()->getAiType()), Console::eDEBUG);
                pMap->getPlayer(i)->serializeObject(stream);
            }
            GlobalUtils::seed(seed);
            GlobalUtils::setUseSeed(true);
            if (!m_saveGame)
            {
                pMap->getGameScript()->gameStart();
            }
            pMap->updateSprites(-1, -1, false, true);
            // start game
            CONSOLE_PRINT("Leaving Map Selection Menue after countdown", Console::eDEBUG);
            auto window = spGameMenue::create(pMap, m_saveGame, m_pNetworkInterface, false);
            oxygine::Stage::getStage()->addChild(window);
            QThread::msleep(200);
            CONSOLE_PRINT("Sending init game to clients", Console::eDEBUG);
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
