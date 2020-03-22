#include "multiplayermenu.h"

#include "qcryptographichash.h"

#include "qdir.h"
#include "qdiriterator.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"

#include "multiplayer/lobbymenu.h"
#include "menue/gamemenue.h"

#include "network/tcpclient.h"
#include "network/tcpserver.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"

#include "objects/dialogconnecting.h"
#include "objects/filedialog.h"
#include "objects/dialogmessagebox.h"

#include "ingamescriptsupport/genericbox.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "multiplayer/networkcommands.h"

Multiplayermenu::Multiplayermenu(QString adress, bool host)
    : MapSelectionMapsMenue(Settings::getHeight() - 380),
      m_Host(host)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Multiplayer Menue", Console::eDEBUG);

    m_pButtonLoadSavegame = ObjectManager::createButton(tr("Load Savegame"));
    m_pButtonLoadSavegame->setPosition(pApp->getSettings()->getWidth() - m_pButtonLoadSavegame->getWidth() - m_pButtonNext->getWidth() - 20, pApp->getSettings()->getHeight() - 10 - m_pButtonLoadSavegame->getHeight());
    m_pButtonLoadSavegame->attachTo(this);
    m_pButtonLoadSavegame->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigLoadSaveGame();
    });
    connect(this, &Multiplayermenu::sigLoadSaveGame, this, &Multiplayermenu::showLoadSaveGameDialog, Qt::QueuedConnection);

    if (!host)
    {
        m_NetworkInterface = new TCPClient();
        m_pPlayerSelection->attachNetworkInterface(m_NetworkInterface);
        createChat();
        emit m_NetworkInterface->sig_connect(adress, Settings::getGamePort());
        hideRuleSelection();
        hideMapSelection();
        m_MapSelectionStep = MapSelectionStep::selectPlayer;
        // change the name of the start button
        dynamic_cast<oxygine::TextField*>(m_pButtonStart->getFirstChild().get())->setHtmlText(tr("Ready"));
        // quit on host connection lost
        connect(m_NetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &Multiplayermenu::disconnected, Qt::QueuedConnection);
        connect(m_NetworkInterface.get(), &NetworkInterface::recieveData, this, &Multiplayermenu::recieveData, Qt::QueuedConnection);
        connect(m_pPlayerSelection.get(), &PlayerSelection::sigDisconnect, this, &Multiplayermenu::slotButtonBack, Qt::QueuedConnection);
        // wait 10 minutes till timeout
        spDialogConnecting pDialogConnecting = new DialogConnecting(tr("Connecting"), 1000 * 60 * 10);
        addChild(pDialogConnecting);
        connect(pDialogConnecting.get(), &DialogConnecting::sigCancel, this, &Multiplayermenu::slotButtonBack, Qt::QueuedConnection);
        connect(this, &Multiplayermenu::sigConnected, pDialogConnecting.get(), &DialogConnecting::connected, Qt::QueuedConnection);
    }
    else
    {
        m_pHostAdresse = ObjectManager::createButton("Show Adresses");
        addChild(m_pHostAdresse);
        m_pHostAdresse->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
        {
            emit sigShowIPs();
        });
        m_pHostAdresse->setPosition(pApp->getSettings()->getWidth() / 2 - m_pHostAdresse->getWidth() / 2,
                                         pApp->getSettings()->getHeight() - m_pHostAdresse->getHeight() - 10);
        m_pHostAdresse->setVisible(false);
        connect(this, &Multiplayermenu::sigShowIPs, this, &Multiplayermenu::showIPs, Qt::QueuedConnection);
    }
    connect(&m_GameStartTimer, &QTimer::timeout, this, &Multiplayermenu::countdown, Qt::QueuedConnection);

}

void Multiplayermenu::showIPs()
{
    spGenericBox pGenericBox = new GenericBox();
    QStringList items = NetworkInterface::getIPAdresses();
    QSize size(Settings::getWidth() - 40, Settings::getHeight() - 80);
    spPanel pPanel = new Panel(true, size, size);
    pPanel->setPosition(20, 20);
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    oxygine::spTextField info = new oxygine::TextField();
    info->setStyle(style);
    info->setHtmlText((tr("Please use one of the following IP-Adresses to connect to this Host. Not all IP-Adresses") +
                      tr(" may work for each client depending on the network settings. Please use cmd and the ping command to verify if an IP-Adress may work")));
    info->setSize(Settings::getWidth() - 80, 500);
    info->setPosition(10, 10);
    pPanel->addItem(info);
    qint32 starty = 10 + info->getTextRect().getHeight();
    for (qint32 i = 0; i < items.size(); i++)
    {
        oxygine::spTextField text = new oxygine::TextField();
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
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    // dummy impl for loading
    QVector<QString> wildcards;
    wildcards.append("*.msav");
    QString path = QCoreApplication::applicationDirPath() + "/savegames";
    spFileDialog saveDialog = new FileDialog(path, wildcards);
    this->addChild(saveDialog);
    connect(saveDialog.get(), &FileDialog::sigFileSelected, this, &Multiplayermenu::loadSaveGame, Qt::QueuedConnection);
    pApp->continueThread();
}

void Multiplayermenu::loadSaveGame(QString filename)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (filename.endsWith(".msav"))
    {
        QFile file(filename);
        QFileInfo info(filename);
        if (file.exists())
        {
            m_pMapSelectionView->loadMap(info);
            hideMapSelection();
            saveGame = true;
            m_pPlayerSelection->setSaveGame(saveGame);
            m_MapSelectionStep = MapSelectionStep::selectRules;
            slotButtonNext();
        }
    }
    pApp->continueThread();
}

void Multiplayermenu::hideMapSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pButtonLoadSavegame->setVisible(false);
    MapSelectionMapsMenue::hideMapSelection();
    pApp->continueThread();
}

void Multiplayermenu::showMapSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pButtonLoadSavegame->setVisible(true);
    MapSelectionMapsMenue::showMapSelection();
    pApp->continueThread();
}

void Multiplayermenu::playerJoined(quint64 socketID)
{
    if (m_NetworkInterface->getIsServer())
    {
        if (m_pPlayerSelection->hasOpenPlayer())
        {
            QCryptographicHash myHash(QCryptographicHash::Sha3_512);
            QString file = m_pMapSelectionView->getCurrentFile().filePath();
            QString fileName = m_pMapSelectionView->getCurrentFile().fileName();
            QString scriptFile = m_pMapSelectionView->getCurrentMap()->getGameScript()->getScriptFile();
            QFile mapFile(file);
            mapFile.open(QIODevice::ReadOnly);
            myHash.addData(&mapFile);
            mapFile.close();
            QByteArray hash = myHash.result();
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream << NetworkCommands::MAPINFO;
            stream << Mainapp::getGameVersion();
            QStringList mods = Settings::getMods();
            stream << static_cast<qint32>(mods.size());
            for (qint32 i = 0; i < mods.size(); i++)
            {
                stream << mods[i];
            }
            stream << saveGame;
            if (saveGame)
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
                    QCryptographicHash myScriptHash(QCryptographicHash::Sha3_512);
                    scriptData.open(QIODevice::ReadOnly);
                    myScriptHash.addData(&scriptData);
                    scriptData.close();
                    QByteArray scriptHash = myScriptHash.result();
                    stream << scriptHash;
                }
            }
            // send map data to client
            m_NetworkInterface->sig_sendData(socketID, data, NetworkInterface::NetworkSerives::Multiplayer, false);
        }
        else
        {
            // reject connection by disconnecting
            emit dynamic_cast<TCPServer*>(m_NetworkInterface.get())->sigDisconnectClient(socketID);
        }
    }
}

void Multiplayermenu::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    // data for us?
    if (service == NetworkInterface::NetworkSerives::Multiplayer)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        if (messageType == NetworkCommands::MAPINFO)
        {
            // we recieved map info from a server check if we have this map already
            if (!m_NetworkInterface->getIsServer())
            {
                QString version;
                stream >> version;
                qint32 size = 0;
                stream >> size;
                QStringList mods;
                for (qint32 i = 0; i < size; i++)
                {
                    QString mod;
                    stream >> mod;
                    mods.append(mod);
                }
                QStringList myMods = Settings::getMods();
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
                if (version == Mainapp::getGameVersion() && sameMods)
                {
                    stream >> saveGame;
                    if (saveGame)
                    {
                        m_pMapSelectionView->setCurrentMap(new GameMap(stream));
                        loadMultiplayerMap();
                        QByteArray sendData;
                        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                        sendStream << NetworkCommands::REQUESTRULE;
                        m_NetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
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
                        if (!scriptFile.isEmpty())
                        {
                            stream >> scriptHash;
                        }
                        if (!fileName.startsWith(NetworkCommands::RANDOMMAPIDENTIFIER) && existsMap(fileName, hash, scriptFile, scriptHash))
                        {
                            QByteArray sendData;
                            QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                            sendStream << NetworkCommands::REQUESTRULE;
                            m_NetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
                        }
                        else
                        {
                            QByteArray sendData;
                            QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                            sendStream << NetworkCommands::REQUESTMAP;
                            m_NetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
                        }
                    }
                }
                else
                {
                    // quit game with wrong version
                    Mainapp* pApp = Mainapp::getInstance();
                    pApp->suspendThread();
                    spDialogMessageBox pDialogMessageBox = new DialogMessageBox(tr("Host has a different game version or other mods loaded leaving the game again."));
                    connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::slotButtonBack, Qt::QueuedConnection);
                    addChild(pDialogMessageBox);
                    pApp->continueThread();
                }
            }
        }
        else if (messageType == NetworkCommands::REQUESTRULE)
        {
            // a client requested the current map rules set by the server
            if (m_NetworkInterface->getIsServer())
            {
                QByteArray sendData;
                QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                sendStream << NetworkCommands::SENDINITUPDATE;
                m_pMapSelectionView->getCurrentMap()->getGameRules()->serializeObject(sendStream);
                if (m_pMapSelectionView->getCurrentMap()->getCampaign() != nullptr)
                {
                    sendStream << true;
                    m_pMapSelectionView->getCurrentMap()->getCampaign()->serializeObject(sendStream);
                }
                else
                {
                    sendStream << false;
                }
                for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getPlayerCount(); i++)
                {
                    GameEnums::AiTypes aiType = m_pPlayerSelection->getPlayerAiType(i);
                    if (aiType == GameEnums::AiTypes_Human)
                    {
                        sendStream << Settings::getUsername();
                        sendStream << static_cast<qint32>(GameEnums::AiTypes_ProxyAi);
                    }
                    else
                    {
                        sendStream << m_pPlayerSelection->getPlayerAiName(i);
                        if (m_pPlayerSelection->isOpenPlayer(i))
                        {
                            sendStream << static_cast<qint32>(GameEnums::AiTypes_Open);
                        }
                        else
                        {
                            sendStream << static_cast<qint32>(GameEnums::AiTypes_ProxyAi);
                        }
                    }
                    m_pMapSelectionView->getCurrentMap()->getPlayer(i)->serializeObject(sendStream);
                }
                m_NetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
            }
        }
        else if (messageType == NetworkCommands::SENDINITUPDATE)
        {
            // the client recieved the initial map data read it and make it visible
            if (!m_NetworkInterface->getIsServer())
            {
                m_pMapSelectionView->getCurrentMap()->getGameRules()->deserializeObject(stream);
                bool campaign = false;
                stream >> campaign;
                if (campaign)
                {
                    m_pMapSelectionView->getCurrentMap()->setCampaign(new Campaign());
                    m_pMapSelectionView->getCurrentMap()->getCampaign()->deserializeObject(stream);
                }
                for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getPlayerCount(); i++)
                {
                    QString name;
                    qint32 aiType;
                    stream >> name;
                    stream >> aiType;
                    m_pPlayerSelection->setPlayerAiName(i, name);
                    m_pMapSelectionView->getCurrentMap()->getPlayer(i)->deserializeObject(stream);
                    m_pMapSelectionView->getCurrentMap()->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(static_cast<GameEnums::AiTypes>(aiType)));
                    m_pPlayerSelection->updatePlayerData(i);
                }
                m_pPlayerSelection->sendPlayerRequest(socketID, -1, GameEnums::AiTypes_Human);
                emit sigConnected();
            }
        }
        else if (messageType == NetworkCommands::REQUESTMAP)
        {
            // someone requested the current map data from the server
            if (m_NetworkInterface->getIsServer())
            {
                QString file = m_pMapSelectionView->getCurrentFile().filePath().replace(QCoreApplication::applicationDirPath() + "/", "");
                QString scriptFile = m_pMapSelectionView->getCurrentMap()->getGameScript()->getScriptFile();
                QByteArray sendData;
                QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                sendStream << NetworkCommands::MAPDATA;
                sendStream << file;
                QFile mapFile(m_pMapSelectionView->getCurrentFile().filePath());
                if (file.startsWith(NetworkCommands::RANDOMMAPIDENTIFIER))
                {
                    GameMap::getInstance()->serializeObject(sendStream);
                }
                else
                {
                    mapFile.open(QIODevice::ReadOnly);
                    sendStream << mapFile.readAll();
                    mapFile.close();
                }
                sendStream << scriptFile;
                if (!scriptFile.isEmpty())
                {
                    QFile script(scriptFile);
                    script.open(QIODevice::ReadOnly);
                    sendStream << script.readAll();
                    script.close();
                }
                m_NetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
            }
        }
        else if (messageType == NetworkCommands::MAPDATA)
        {
            if (!m_NetworkInterface->getIsServer())
            {
                QString mapFile;
                stream >> mapFile;
                if (mapFile.startsWith(NetworkCommands::RANDOMMAPIDENTIFIER))
                {
                    new GameMap(stream);
                }
                else
                {
                    QByteArray mapData;
                    stream >> mapData;
                    QFile map(mapFile);
                    if (!map.exists())
                    {
                        QFileInfo mapInfo(mapFile);
                        QDir dir;
                        QString fileDir = mapInfo.filePath().replace(mapInfo.fileName(), "");
                        dir.mkdir(fileDir);
                        map.open(QIODevice::WriteOnly);
                        QDataStream mapFilestream(&map);
                        for (qint32 i = 0; i < mapData.size(); i++)
                        {
                            mapFilestream << static_cast<quint8>(mapData[i]);
                        }
                        map.close();
                        QString scriptFile;
                        stream >> scriptFile;
                        QByteArray scriptData;
                        if (!scriptFile.isEmpty())
                        {
                            stream >> scriptData;
                            QFile script(scriptFile);
                            if (!script.exists())
                            {
                                QFileInfo scriptInfo(scriptFile);
                                fileDir = scriptInfo.filePath().replace(scriptInfo.fileName(), "");
                                dir.mkdir(fileDir);
                                script.open(QIODevice::WriteOnly);
                                QDataStream scriptFilestream(&map);
                                for (qint32 i = 0; i < scriptData.size(); i++)
                                {
                                    scriptFilestream << static_cast<quint8>(scriptData[i]);
                                }
                                script.close();
                            }
                            else
                            {
                                Mainapp* pApp = Mainapp::getInstance();
                                pApp->suspendThread();
                                spDialogMessageBox pDialogMessageBox = new DialogMessageBox(tr("Unable to download map or game script from host a different version of the map or game script with the same name exist! Leaving the game again."));
                                connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::slotButtonBack, Qt::QueuedConnection);
                                addChild(pDialogMessageBox);
                                pApp->continueThread();
                                return;
                            }
                        }
                        m_pMapSelectionView->setCurrentMap(new GameMap(mapFile, true));
                        loadMultiplayerMap();
                        QByteArray sendData;
                        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                        sendStream << NetworkCommands::REQUESTRULE;
                        m_NetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
                    }
                    else
                    {
                        Mainapp* pApp = Mainapp::getInstance();
                        pApp->suspendThread();
                        spDialogMessageBox pDialogMessageBox = new DialogMessageBox(tr("Unable to download map or game script from host a different version of the map or game script with the same name exist! Leaving the game again."));
                        connect(pDialogMessageBox.get(), &DialogMessageBox::sigOk, this, &Multiplayermenu::slotButtonBack, Qt::QueuedConnection);
                        addChild(pDialogMessageBox);
                        pApp->continueThread();
                        return;
                    }
                }
            }
        }
        else if (messageType == NetworkCommands::INITGAME)
        {
            // initializes the game on the client
            if (!m_NetworkInterface->getIsServer())
            {
                initClientGame(socketID, stream);
                addRef();
                oxygine::Actor::detach();
                deleteLater();
            }
        }
    }
}

void Multiplayermenu::loadMultiplayerMap()
{    
    m_pMapSelectionView->getCurrentMap()->getGameScript()->init();
    m_pMapSelectionView->updateMapData();
    showPlayerSelection();
}

void Multiplayermenu::initClientGame(quint64, QDataStream &stream)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    quint32 seed;
    stream >> seed;
    Mainapp::seed(seed);
    Mainapp::setUseSeed(true);
    for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getPlayerCount(); i++)
    {
        GameEnums::AiTypes aiType = m_pMapSelectionView->getCurrentMap()->getPlayer(i)->getBaseGameInput()->getAiType();
        m_pMapSelectionView->getCurrentMap()->getPlayer(i)->deserializeObject(stream);
        m_pMapSelectionView->getCurrentMap()->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(aiType));
    }
    GameMap* pMap = GameMap::getInstance();
    pMap->initPlayers();
    pMap->getGameScript()->gameStart();
    pMap->updateSprites();
    // start game
    Console::print("Leaving Map Selection Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new GameMenue(m_NetworkInterface, saveGame));
    pApp->continueThread();
}

bool Multiplayermenu::existsMap(QString& fileName, QByteArray& hash, QString& scriptFileName, QByteArray& scriptHash)
{
    QString path = QCoreApplication::applicationDirPath() + "/maps";
    if (saveGame)
    {
        path = QCoreApplication::applicationDirPath() + "/savegames";
    }
    QStringList filter;
    filter << "*" + fileName;
    QDirIterator dirIter(path, filter, QDir::Files, QDirIterator::Subdirectories);
    bool found = false;
    while (dirIter.hasNext() && !found)
    {
        dirIter.next();
        QString file = dirIter.fileInfo().absoluteFilePath();
        QFile mapFile(file);
        mapFile.open(QIODevice::ReadOnly);
        QCryptographicHash myHash(QCryptographicHash::Sha3_512);
        myHash.addData(&mapFile);
        mapFile.close();
        QByteArray myHashArray = myHash.result();
        if (hash == myHashArray)
        {
            m_pMapSelectionView->setCurrentMap(new GameMap(file, true));
            loadMultiplayerMap();
            found = true;
        }
    }
    if (found  && !saveGame)
    {
        if (!scriptFileName.isEmpty())
        {
            QFile scriptFile(scriptFileName);
            scriptFile.open(QIODevice::ReadOnly);
            QCryptographicHash myHash(QCryptographicHash::Sha3_512);
            myHash.addData(&scriptFile);
            scriptFile.close();
            QByteArray myHashArray = myHash.result();
            if (myHashArray != scriptHash)
            {
                found = false;
            }
        }
    }

    return found;

}

void Multiplayermenu::disconnected(quint64)
{
    if (m_Host)
    {
        // handled in player selection
    }
    else
    {
        disconnectNetwork();
        Console::print("Leaving Map Selection Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new LobbyMenu());
        addRef();
        oxygine::Actor::detach();
        deleteLater();
    }
}

void Multiplayermenu::slotButtonBack()
{
    if (!m_Host || m_MapSelectionStep == MapSelectionStep::selectMap)
    {
        disconnectNetwork();
        Console::print("Leaving Map Selection Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new LobbyMenu());
        addRef();
        oxygine::Actor::detach();
        deleteLater();
    }
    else if (m_Host)
    {
        disconnectNetwork();
        m_pHostAdresse->setVisible(false);
        MapSelectionMapsMenue::slotButtonBack();
        if (saveGame)
        {
            MapSelectionMapsMenue::slotButtonBack();
            saveGame = false;
            m_pPlayerSelection->setSaveGame(saveGame);
        }
    }
}

void Multiplayermenu::slotButtonNext()
{
    if (m_Host && m_MapSelectionStep == MapSelectionStep::selectRules)
    {
        m_pHostAdresse->setVisible(true);
        m_NetworkInterface = new TCPServer();
        m_pPlayerSelection->attachNetworkInterface(m_NetworkInterface);
        createChat();
        emit m_NetworkInterface->sig_connect("", Settings::getGamePort());
        connect(m_NetworkInterface.get(), &NetworkInterface::sigConnected, this, &Multiplayermenu::playerJoined, Qt::QueuedConnection);
        connect(m_NetworkInterface.get(), &NetworkInterface::recieveData, this, &Multiplayermenu::recieveData, Qt::QueuedConnection);
        connect(m_NetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &Multiplayermenu::disconnected, Qt::QueuedConnection);
    }
    MapSelectionMapsMenue::slotButtonNext();
}

void Multiplayermenu::createChat()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Chat = new Chat(m_NetworkInterface, QSize(pApp->getSettings()->getWidth() - 20,
                                                300));
    m_Chat->setPosition(10, Settings::getHeight() - 360);
    addChild(m_Chat);
    pApp->continueThread();
}

void Multiplayermenu::disconnectNetwork()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_GameStartTimer.stop();
    if (m_NetworkInterface.get() != nullptr)
    {
        m_Chat->detach();
        m_Chat = nullptr;
        emit m_NetworkInterface->sig_close();
        m_pPlayerSelection->attachNetworkInterface(nullptr);
        m_NetworkInterface = nullptr;
    }
    pApp->continueThread();
}

bool Multiplayermenu::getGameReady()
{
    bool gameReady = true;
    for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getPlayerCount(); i++)
    {
        GameEnums::AiTypes aiType = m_pPlayerSelection->getPlayerAiType(i);
        if (aiType == GameEnums::AiTypes_Open)
        {
            gameReady = false;
            break;
        }
        else if (aiType == GameEnums::AiTypes_ProxyAi)
        {
            if (m_pPlayerSelection->getReady(i) == false)
            {
                gameReady = false;
                break;
            }
        }
    }
    return gameReady;
}

void Multiplayermenu::startGame()
{
    if (!m_Host)
    {
        m_pPlayerSelection->setPlayerReady(!m_pPlayerSelection->getPlayerReady());
        if (m_pPlayerSelection->getPlayerReady())
        {
            dynamic_cast<oxygine::TextField*>(m_pButtonStart->getFirstChild().get())->setHtmlText(tr("Not Ready"));
        }
        else
        {
            dynamic_cast<oxygine::TextField*>(m_pButtonStart->getFirstChild().get())->setHtmlText(tr("Ready"));
        }
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << NetworkCommands::CLIENTREADY;
        sendStream << m_pPlayerSelection->getPlayerReady();
        m_NetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
    }
    else
    {
        if (m_pPlayerSelection->getPlayerReady())
        {
            counter = 5;
            m_GameStartTimer.stop();
            sendServerReady(false);
        }
        // can we start the game?
        else if (getGameReady())
        {
            sendServerReady(true);
            counter = 5;
            m_GameStartTimer.setInterval(std::chrono::seconds(1));
            m_GameStartTimer.setSingleShot(false);
            m_GameStartTimer.start();
            emit m_Chat->sigSendText(QString::number(counter) + "...");
        }
    }
}

void Multiplayermenu::sendServerReady(bool value)
{
    if (m_NetworkInterface->getIsServer())
    {
        if (value)
        {
            emit dynamic_cast<TCPServer*>(m_NetworkInterface.get())->pauseListening();
        }
        else
        {
            emit dynamic_cast<TCPServer*>(m_NetworkInterface.get())->continueListening();
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
        m_pPlayerSelection->setPlayerReady(value);
        m_pPlayerSelection->sendPlayerReady(0, player, value);
    }
}

void Multiplayermenu::countdown()
{
    counter--;
    if (getGameReady())
    {
        emit m_Chat->sigSendText(QString::number(counter) + "...");
        if (counter == 0)
        {
            Mainapp* pApp = Mainapp::getInstance();
            pApp->suspendThread();
            GameMap* pMap = GameMap::getInstance();
            pMap->initPlayers();
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream << NetworkCommands::INITGAME;
            quint32 seed = QRandomGenerator::global()->bounded(std::numeric_limits<quint32>::max());
            Mainapp::seed(seed);
            Mainapp::setUseSeed(true);
            stream << seed;
            for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getPlayerCount(); i++)
            {
                m_pMapSelectionView->getCurrentMap()->getPlayer(i)->serializeObject(stream);
            }            
            pMap->getGameScript()->gameStart();
            pMap->updateSprites();
            // start game
            Console::print("Leaving Map Selection Menue", Console::eDEBUG);
            oxygine::getStage()->addChild(new GameMenue(m_NetworkInterface, saveGame));
            emit m_NetworkInterface->sig_sendData(0, data, NetworkInterface::NetworkSerives::Multiplayer, false);
            addRef();
            oxygine::Actor::detach();
            deleteLater();
            pApp->continueThread();
        }
    }
    else
    {
        counter = 5;
        m_GameStartTimer.stop();
        sendServerReady(false);
    }
}
