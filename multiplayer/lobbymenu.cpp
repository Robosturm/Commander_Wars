#include <QJsonObject>
#include <QJsonDocument>

#include "multiplayer/lobbymenu.h"
#include "multiplayer/networkcommands.h"
#include "multiplayer/dialogpassword.h"
#include "multiplayer/dialogpasswordandadress.h"
#include "multiplayer/multiplayermenu.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"
#include "coreengine/audiothread.h"

#include "menue/mainwindow.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "objects/base/chat.h"
#include "objects/dialogs/dialogmessagebox.h"

#include "network/mainserver.h"
#include "network/JsonKeys.h"

LobbyMenu::LobbyMenu()
{
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    CONSOLE_PRINT("Entering Lobby Menue", Console::eDEBUG);
    Interpreter::setCppOwnerShip(this);

    if (!Settings::getServer())
    {
        m_pTCPClient = spTCPClient::create(nullptr);
        m_pTCPClient->moveToThread(Mainapp::getInstance()->getNetworkThread());
        connect(m_pTCPClient.get(), &TCPClient::recieveData, this, &LobbyMenu::recieveData, Qt::QueuedConnection);
        connect(m_pTCPClient.get(), &TCPClient::sigConnected, this, &LobbyMenu::connected, Qt::QueuedConnection);
        emit m_pTCPClient->sig_connect(Settings::getServerAdress(), Settings::getServerPort());
    }

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = oxygine::spSprite::create();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("lobbymenu");
    sprite->setResAnim(pBackground);
    sprite->setPosition(-1, -1);
    // background should be last to draw
    sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
    sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
    sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/multiplayer");
    pApp->getAudioThread()->playRandom();

    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    addChild(pButtonExit);
    pButtonExit->setPosition(10, Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &LobbyMenu::sigExitMenue, this, &LobbyMenu::exitMenue, Qt::QueuedConnection);

    oxygine::spButton pButtonHost = ObjectManager::createButton(tr("Direct Host"));
    addChild(pButtonHost);
    pButtonHost->setPosition(Settings::getWidth() - pButtonHost->getWidth() - 10, Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonHost->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigHostLocal();
    });
    connect(this, &LobbyMenu::sigHostLocal, this, &LobbyMenu::hostLocal, Qt::QueuedConnection);

    m_pButtonHostOnServer = ObjectManager::createButton(tr("Server Host"));
    addChild(m_pButtonHostOnServer);
    m_pButtonHostOnServer->setPosition(Settings::getWidth() - pButtonHost->getWidth() - 10, Settings::getHeight() - pButtonExit->getHeight() * 2 - 10);
    m_pButtonHostOnServer->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigHostServer();
    });
    connect(this, &LobbyMenu::sigHostServer, this, &LobbyMenu::hostServer, Qt::QueuedConnection);
    if (m_pTCPClient.get() == nullptr ||
        !m_pTCPClient->getIsConnected())
    {
        m_pButtonHostOnServer ->setEnabled(false);
    }

    oxygine::spButton pButtonJoin = ObjectManager::createButton(tr("Join Game"));
    addChild(pButtonJoin);
    pButtonJoin->setPosition(Settings::getWidth() / 2 + 10, Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonJoin->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigJoinGame();
    });
    connect(this, &LobbyMenu::sigJoinGame, this, &LobbyMenu::joinGame, Qt::QueuedConnection);

    oxygine::spButton pButtonObserve = ObjectManager::createButton(tr("Observe Game"));
    addChild(pButtonObserve);
    pButtonObserve->setPosition(Settings::getWidth() / 2 + 10, pButtonJoin->getY() - pButtonJoin->getHeight());
    pButtonObserve->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigObserveGame();
    });
    connect(this, &LobbyMenu::sigObserveGame, this, &LobbyMenu::observeGame, Qt::QueuedConnection);

    oxygine::spButton pButtonJoinAdress = ObjectManager::createButton(tr("Join Adress"));
    addChild(pButtonJoinAdress);
    pButtonJoinAdress->setPosition(Settings::getWidth() / 2 - 10 - pButtonJoinAdress->getWidth(), Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonJoinAdress->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigJoinAdress();
    });
    connect(this, &LobbyMenu::sigJoinAdress, this, &LobbyMenu::joinAdress, Qt::QueuedConnection);

    oxygine::spButton pButtonObserveAdress = ObjectManager::createButton(tr("Observe Adress"));
    addChild(pButtonObserveAdress);
    pButtonObserveAdress->setPosition(Settings::getWidth() / 2 - 10 - pButtonJoinAdress->getWidth(), pButtonJoinAdress->getY() - pButtonJoinAdress->getHeight());
    pButtonObserveAdress->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigObserveAdress();
    });
    connect(this, &LobbyMenu::sigObserveAdress, this, &LobbyMenu::observeAdress, Qt::QueuedConnection);


    qint32 height = Settings::getHeight() - 420;
    if (Settings::getSmallScreenDevice())
    {
        height = Settings::getHeight() - 120;
    }
    m_pGamesPanel = spPanel::create(true, QSize(Settings::getWidth() - 20, height),
                              QSize(Settings::getWidth() - 20, height));
    m_pGamesPanel->setPosition(10, 10);
    addChild(m_pGamesPanel);

    spNetworkInterface pInterface = m_pTCPClient;
    if (Settings::getServer())
    {
        pInterface = MainServer::getInstance()->getGameServer();
    }
    if (!Settings::getSmallScreenDevice())
    {
        height = 300;
    }
    spChat pChat = spChat::create(pInterface, QSize(Settings::getWidth() - 20, height), NetworkInterface::NetworkSerives::LobbyChat);
    pChat->setPosition(10, m_pGamesPanel->getHeight() + 20);
    if (Settings::getSmallScreenDevice())
    {
        pChat->setVisible(false);
    }
    addChild(pChat);

    connect(this, &LobbyMenu::sigUpdateGamesView, this, &LobbyMenu::updateGamesView, Qt::QueuedConnection);
}

void LobbyMenu::exitMenue()
{    
    CONSOLE_PRINT("Leaving Lobby Menue", Console::eDEBUG);
    auto window = spMainwindow::create();
    oxygine::Stage::getStage()->addChild(window);
    oxygine::Actor::detach();    
}

void LobbyMenu::hostLocal()
{    
    CONSOLE_PRINT("Leaving Lobby Menue", Console::eDEBUG);
    oxygine::Stage::getStage()->addChild(spMultiplayermenu::create("", Settings::getGamePort(), "", Multiplayermenu::NetworkMode::Host));
    oxygine::Actor::detach();    
}

void LobbyMenu::hostServer()
{
    if (m_pTCPClient.get() != nullptr &&
        m_pTCPClient->getIsConnected())
    {
        CONSOLE_PRINT("Leaving Lobby Menue", Console::eDEBUG);
        oxygine::Stage::getStage()->addChild(spMultiplayermenu::create(m_pTCPClient, "",  Multiplayermenu::NetworkMode::Host));
        oxygine::Actor::detach();        
    }
}

void LobbyMenu::joinGame()
{
    if (m_currentGame.get() && m_currentGame->hasOpenPlayers())
    {
        if (m_currentGame->getLocked())
        {
            spDialogPassword pDialogTextInput = spDialogPassword::create(tr("Enter Password"), true, "");
            addChild(pDialogTextInput);
            connect(pDialogTextInput.get(), &DialogPassword::sigTextChanged, this, &LobbyMenu::joinGamePassword, Qt::QueuedConnection);
            
        }
        else
        {
            joinGamePassword("");
        }
    }
}

void LobbyMenu::joinGamePassword(QString password)
{
    bool exists = false;
    if (m_currentGame.get() != nullptr)
    {
        for (const auto & game : qAsConst(m_games))
        {
            if (m_currentGame.get() == game.get())
            {
                exists = true;
                break;
            }
        }
    }
    if (exists)
    {
        CONSOLE_PRINT("Leaving Lobby Menue to join server game", Console::eDEBUG);
        QString command = QString(NetworkCommands::SERVERJOINGAME);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);

        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        data.insert(JsonKeys::JSONKEY_SLAVENAME, m_currentGame->getSlaveName());
        QJsonDocument doc(data);
        emit m_pTCPClient->sig_sendData(0, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
        m_password = password;
    }
}

void LobbyMenu::joinAdress()
{
    spDialogPasswordAndAdress pDialogTextInput = spDialogPasswordAndAdress::create(tr("Enter Host Adress"));
    addChild(pDialogTextInput);
    connect(pDialogTextInput.get(), &DialogPasswordAndAdress::sigTextChanged, this, &LobbyMenu::join, Qt::QueuedConnection);
}

void LobbyMenu::join(QString adress, QString password)
{    
    CONSOLE_PRINT("Leaving Lobby Menue to join game by adress", Console::eDEBUG);
    oxygine::Stage::getStage()->addChild(spMultiplayermenu::create(adress.trimmed(), Settings::getGamePort(), password, Multiplayermenu::NetworkMode::Client));
    oxygine::Actor::detach();
}

void LobbyMenu::observeAdress()
{
    spDialogPasswordAndAdress pDialogTextInput = spDialogPasswordAndAdress::create(tr("Enter Host Adress"));
    addChild(pDialogTextInput);
    connect(pDialogTextInput.get(), &DialogPasswordAndAdress::sigTextChanged, this, &LobbyMenu::observe, Qt::QueuedConnection);
}

void LobbyMenu::observe(QString adress, QString password)
{
    CONSOLE_PRINT("Leaving Lobby Menue to observe game by adress", Console::eDEBUG);
    oxygine::Stage::getStage()->addChild(spMultiplayermenu::create(adress.trimmed(), Settings::getGamePort(), password, Multiplayermenu::NetworkMode::Observer));
    oxygine::Actor::detach();
}

void LobbyMenu::observeGame()
{
    if (m_currentGame.get())
    {
        if (m_currentGame->getLocked())
        {
            spDialogPassword pDialogTextInput = spDialogPassword::create(tr("Enter Password"), true, "");
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
    bool exists = false;
    if (m_currentGame.get() != nullptr)
    {
        for (const auto & game : qAsConst(m_games))
        {
            if (m_currentGame.get() == game.get())
            {
                exists = true;
                break;
            }
        }
    }
    if (exists)
    {
        CONSOLE_PRINT("Leaving Lobby Menue to observe server game", Console::eDEBUG);
        m_pTCPClient->setIsObserver(true);
        oxygine::Stage::getStage()->addChild(spMultiplayermenu::create(m_pTCPClient, password, Multiplayermenu::NetworkMode::Observer));
        QString command = QString(NetworkCommands::SERVERJOINGAME);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        data.insert(JsonKeys::JSONKEY_SLAVENAME, m_currentGame->getSlaveName());
        QJsonDocument doc(data);
        emit m_pTCPClient->sig_sendData(0, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
        oxygine::Actor::detach();
    }
}

void LobbyMenu::recieveData(quint64, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == NetworkInterface::NetworkSerives::ServerHostingJson)
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject objData = doc.object();
        QString messageType = objData.value(JsonKeys::JSONKEY_COMMAND).toString();
        CONSOLE_PRINT("LobbyMenu Command received: " + messageType, Console::eDEBUG);
        if (messageType == NetworkCommands::SERVERGAMEDATA)
        {
            updateGameData(objData);
        }
        else if (messageType == NetworkCommands::SLAVEADDRESSINFO)
        {
            joinSlaveGame(objData);
        }
        else if (messageType == NetworkCommands::SERVERGAMENOLONGERAVAILABLE)
        {
             spDialogMessageBox pDialogMessageBox;
             pDialogMessageBox = spDialogMessageBox::create(tr("Host game is no longer available."));
             addChild(pDialogMessageBox);
        }
    }
}

void LobbyMenu::updateGameData(const QJsonObject & objData)
{
    m_games.clear();
    QJsonObject games = objData.value(JsonKeys::JSONKEY_GAMES).toObject();
    for (const auto & game : games)
    {
        auto gameData = spNetworkGameData::create();
        gameData->fromJson(game.toObject());
        m_games.append(gameData);
    }
    emit sigUpdateGamesView();
}

void LobbyMenu::joinSlaveGame(const QJsonObject & objData)
{
    QString slaveAddress = objData.value(JsonKeys::JSONKEY_ADDRESS).toString();
    quint16 slavePort = objData.value(JsonKeys::JSONKEY_PORT).toInteger();
    CONSOLE_PRINT("Leaving Lobby Menue to join game by adress", Console::eDEBUG);
    oxygine::Stage::getStage()->addChild(spMultiplayermenu::create(slaveAddress.trimmed(), slavePort, m_password, Multiplayermenu::NetworkMode::Client));
    oxygine::Actor::detach();
}

void LobbyMenu::updateGamesView()
{    
    if (m_Gamesview.get() != nullptr)
    {
        m_Gamesview->detach();
        m_Gamesview = nullptr;
    }
    QStringList header = {tr("Map"), tr("Players"), tr("Description"), tr("Mods"), tr("Locked")};
    qint32 itemWidth = (m_pGamesPanel->getWidth() - 80 - 100 - 90) / 3;
    QVector<qint32> widths = {itemWidth, 100, itemWidth, itemWidth, 90};
    QVector<QStringList> items;
    for (const auto & game : qAsConst(m_games))
    {
        QStringList data;
        data.append(game->getMapName());
        data.append(QString::number(game->getPlayers()) + "/" +
                    QString::number(game->getMaxPlayers()));
        data.append(game->getDescription());
        QStringList mods = game->getMods();
        QString modString;
        for (const auto & mod : mods)
        {
            modString.append(Settings::getModName(mod) + "; ");
        }
        data.append(modString);
        char lockChar = FontManager::SpecialChars::unlockChar;
        if (game->getLocked())
        {
            lockChar = FontManager::SpecialChars::lockChar;
        }
        data.append(QString(lockChar));
        items.append(data);
    }
    m_Gamesview = spTableView::create(widths, items, header, true);
    connect(m_Gamesview.get(), &TableView::sigItemClicked, this, &LobbyMenu::selectGame, Qt::QueuedConnection);
    m_pGamesPanel->addItem(m_Gamesview);
    m_pGamesPanel->setContentHeigth(m_Gamesview->getHeight() + 40);
    
}

void LobbyMenu::selectGame()
{
    qint32 game = m_Gamesview->getCurrentItem();
    if (game >= 0 && game < m_games.size())
    {
        m_currentGame = m_games[game];
    }
}

void LobbyMenu::connected(quint64 socket)
{
    m_pButtonHostOnServer->setEnabled(true);
}
