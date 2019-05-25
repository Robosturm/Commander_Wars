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
    m_pButtonLoadSavegame->setPosition(pApp->getSettings()->getWidth() / 2 - m_pButtonLoadSavegame->getWidth() / 2, pApp->getSettings()->getHeight() - 10 - m_pButtonLoadSavegame->getHeight());
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
        dynamic_cast<oxygine::TextField*>(m_pButtonStart->getFirstChild().get())->setText(tr("Ready").toStdString().c_str());
        // quit on host connection lost
        connect(m_NetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &Multiplayermenu::disconnected, Qt::QueuedConnection);
        connect(m_NetworkInterface.get(), &NetworkInterface::recieveData, this, &Multiplayermenu::recieveData, Qt::QueuedConnection);
        connect(m_pPlayerSelection.get(), &PlayerSelection::sigDisconnect, this, &Multiplayermenu::slotButtonBack, Qt::QueuedConnection);

        spDialogConnecting pDialogConnecting = new DialogConnecting(tr("Connecting"));
        addChild(pDialogConnecting);
        connect(pDialogConnecting.get(), &DialogConnecting::sigCancel, this, &Multiplayermenu::slotButtonBack, Qt::QueuedConnection);
        connect(this, &Multiplayermenu::sigConnected, pDialogConnecting.get(), &DialogConnecting::connected, Qt::QueuedConnection);
    }
    else
    {
        oxygine::TextStyle style = FontManager::getMainFont();
        style.color = oxygine::Color(255, 255, 255, 255);
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = true;

        m_pHostAdresse = new oxygine::TextField();
        addChild(m_pHostAdresse);
        m_pHostAdresse->setStyle(style);
        m_pHostAdresse->setText((tr("Host Adress: ") + NetworkInterface::getIPAdresse()).toStdString().c_str());
        m_pHostAdresse->setPosition(pApp->getSettings()->getWidth() / 2 - m_pHostAdresse->getTextRect().getWidth() / 2,
                                 pApp->getSettings()->getHeight() - m_pHostAdresse->getTextRect().getHeight() - 10);

        m_pHostAdresse->setVisible(false);
    }
    connect(&m_GameStartTimer, &QTimer::timeout, this, &Multiplayermenu::countdown, Qt::QueuedConnection);

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
            loadMap(info);
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
            QString file = m_currentMapFile.filePath();
            QString fileName = m_currentMapFile.fileName();
            QString scriptFile = m_pCurrentMap->getGameScript()->getScriptFile();
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
            stream << saveGame;
            for (qint32 i = 0; i < mods.size(); i++)
            {
                stream << mods[i];
            }
            if (saveGame)
            {
                m_pCurrentMap->serializeObject(stream);
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
                    for (qint32 i = 0; i < myMods.size(); i++)
                    {
                        if (!mods.contains(myMods[i]))
                        {
                            sameMods = false;
                        }
                    }
                }
                if (version == Mainapp::getGameVersion() && sameMods)
                {
                    stream >> saveGame;
                    if (saveGame)
                    {
                        m_pCurrentMap = new GameMap(stream);
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
                        if (existsMap(fileName, hash, scriptFile, scriptHash))
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
                    slotButtonBack();
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
                m_pCurrentMap->getGameRules()->serializeObject(sendStream);
                for (qint32 i = 0; i < m_pCurrentMap->getPlayerCount(); i++)
                {
                    BaseGameInputIF::AiTypes aiType = m_pPlayerSelection->getPlayerAiType(i);
                    if (aiType == BaseGameInputIF::AiTypes::Human)
                    {
                        sendStream << Settings::getUsername();
                        sendStream << static_cast<qint32>(BaseGameInputIF::AiTypes::ProxyAi);
                    }
                    else
                    {
                        sendStream << m_pPlayerSelection->getPlayerAiName(i);
                        if (m_pPlayerSelection->isOpenPlayer(i))
                        {
                            sendStream << static_cast<qint32>(BaseGameInputIF::AiTypes::Open);
                        }
                        else
                        {
                            sendStream << static_cast<qint32>(BaseGameInputIF::AiTypes::ProxyAi);
                        }
                    }
                    m_pCurrentMap->getPlayer(i)->serializeObject(sendStream);
                }
                m_NetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
            }
        }
        else if (messageType == NetworkCommands::SENDINITUPDATE)
        {
            // the client recieved the initial map data read it and make it visible
            if (!m_NetworkInterface->getIsServer())
            {
                m_pCurrentMap->getGameRules()->deserializeObject(stream);
                for (qint32 i = 0; i < m_pCurrentMap->getPlayerCount(); i++)
                {
                    QString name;
                    qint32 aiType;
                    stream >> name;
                    stream >> aiType;
                    m_pPlayerSelection->setPlayerAiName(i, name);
                    m_pCurrentMap->getPlayer(i)->deserializeObject(stream);
                    m_pCurrentMap->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(static_cast<BaseGameInputIF::AiTypes>(aiType)));
                    m_pPlayerSelection->updatePlayerData(i);
                }
                m_pPlayerSelection->sendPlayerRequest(socketID, -1, BaseGameInputIF::AiTypes::Human);
                emit sigConnected();
            }
        }
        else if (messageType == NetworkCommands::REQUESTMAP)
        {
            // someone requested the current map data from the server
            if (m_NetworkInterface->getIsServer())
            {
                QString file = m_currentMapFile.filePath().replace(QCoreApplication::applicationDirPath() + "/", "");
                QString scriptFile = m_pCurrentMap->getGameScript()->getScriptFile();
                QByteArray sendData;
                QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                sendStream << NetworkCommands::MAPDATA;
                sendStream << file;
                QFile mapFile(m_currentMapFile.filePath());
                mapFile.open(QIODevice::ReadOnly);
                sendStream << mapFile.readAll();
                mapFile.close();
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
                            slotButtonBack();
                            return;
                        }
                    }
                    m_pCurrentMap = new GameMap(mapFile, true);
                    loadMultiplayerMap();
                    QByteArray sendData;
                    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                    sendStream << NetworkCommands::REQUESTRULE;
                    m_NetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
                }
                else
                {
                    slotButtonBack();
                    return;
                }
            }
        }
        else if (messageType == NetworkCommands::INITGAME)
        {
            // initializes the game on the client
            if (!m_NetworkInterface->getIsServer())
            {
                initClientGame(socketID, stream);
            }
        }
    }
}

void Multiplayermenu::loadMultiplayerMap()
{    
    m_pCurrentMap->getGameScript()->init();
    m_MapName->setText(m_pCurrentMap->getMapName().toStdString().c_str());
    m_MapAuthor->setText(m_pCurrentMap->getMapAuthor().toStdString().c_str());
    m_MapDescription->setText(m_pCurrentMap->getMapDescription().toStdString().c_str());
    showPlayerSelection();
}

void Multiplayermenu::initClientGame(quint64, QDataStream &stream)
{
    quint32 seed;
    stream >> seed;
    Mainapp::seed(seed);
    Mainapp::setUseSeed(true);
    for (qint32 i = 0; i < m_pCurrentMap->getPlayerCount(); i++)
    {
        BaseGameInputIF::AiTypes aiType = m_pCurrentMap->getPlayer(i)->getBaseGameInput()->getAiType();
        m_pCurrentMap->getPlayer(i)->deserializeObject(stream);
        m_pCurrentMap->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(aiType));
    }
    initPlayers();
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    pMap->getGameScript()->gameStart();
    pMap->updateSprites();
    // start game
    Console::print("Leaving Map Selection Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new GameMenue(m_NetworkInterface));
    oxygine::Actor::detach();
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
            m_pCurrentMap = new GameMap(file, true);
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
        oxygine::Actor::detach();
    }
}

void Multiplayermenu::slotButtonBack()
{
    if (!m_Host || m_MapSelectionStep == MapSelectionStep::selectMap)
    {
        disconnectNetwork();
        Console::print("Leaving Map Selection Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new LobbyMenu());
        oxygine::Actor::detach();
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
    for (qint32 i = 0; i < m_pCurrentMap->getPlayerCount(); i++)
    {
        BaseGameInputIF::AiTypes aiType = m_pPlayerSelection->getPlayerAiType(i);
        if (aiType == BaseGameInputIF::AiTypes::Open)
        {
            gameReady = false;
            break;
        }
        else if (aiType == BaseGameInputIF::AiTypes::ProxyAi)
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
            dynamic_cast<oxygine::TextField*>(m_pButtonStart->getFirstChild().get())->setText(tr("Not Ready").toStdString().c_str());
        }
        else
        {
            dynamic_cast<oxygine::TextField*>(m_pButtonStart->getFirstChild().get())->setText(tr("Ready").toStdString().c_str());
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
            emit m_Chat->sigSendText((QString::number(counter) + "...").toStdString().c_str());
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
        for (qint32 i = 0; i < m_pCurrentMap->getPlayerCount(); i++)
        {
            BaseGameInputIF::AiTypes aiType = m_pPlayerSelection->getPlayerAiType(i);
            if (aiType != BaseGameInputIF::AiTypes::Open &&
                aiType != BaseGameInputIF::AiTypes::ProxyAi)
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
        emit m_Chat->sigSendText((QString::number(counter) + "...").toStdString().c_str());
        if (counter == 0)
        {
            initPlayers();
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream << NetworkCommands::INITGAME;
            quint32 seed = QRandomGenerator::global()->bounded(std::numeric_limits<quint32>::max());
            Mainapp::seed(seed);
            Mainapp::setUseSeed(true);
            stream << seed;
            for (qint32 i = 0; i < m_pCurrentMap->getPlayerCount(); i++)
            {
                m_pCurrentMap->getPlayer(i)->serializeObject(stream);
            }

            Mainapp* pApp = Mainapp::getInstance();
            pApp->suspendThread();
            GameMap* pMap = GameMap::getInstance();
            pMap->getGameScript()->gameStart();
            pMap->updateSprites();
            // start game
            Console::print("Leaving Map Selection Menue", Console::eDEBUG);
            oxygine::getStage()->addChild(new GameMenue(m_NetworkInterface));
            emit m_NetworkInterface->sig_sendData(0, data, NetworkInterface::NetworkSerives::Multiplayer, false);
            oxygine::Actor::detach();
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
