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

#include "network/mainserver.h"


LobbyMenu::LobbyMenu()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Lobby Menue", Console::eDEBUG);

    if (!Settings::getServer())
    {
        m_pTCPClient = spTCPClient::create();
        connect(m_pTCPClient.get(), &TCPClient::recieveData, this, &LobbyMenu::recieveData, Qt::QueuedConnection);
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
    pButtonExit->attachTo(this);
    pButtonExit->setPosition(10, Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &LobbyMenu::sigExitMenue, this, &LobbyMenu::exitMenue, Qt::QueuedConnection);

    oxygine::spButton pButtonHost = ObjectManager::createButton(tr("Host Local"));
    pButtonHost->attachTo(this);
    pButtonHost->setPosition(Settings::getWidth() - pButtonHost->getWidth() - 10, Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonHost->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigHostLocal();
    });
    connect(this, &LobbyMenu::sigHostLocal, this, &LobbyMenu::hostLocal, Qt::QueuedConnection);

    oxygine::spButton pButtonHostOnServer = ObjectManager::createButton(tr("Host"));
    pButtonHostOnServer->attachTo(this);
    pButtonHostOnServer->setPosition(Settings::getWidth() - pButtonHost->getWidth() - 10, Settings::getHeight() - pButtonExit->getHeight() * 2 - 10);
    pButtonHostOnServer->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigHostServer();
    });
    connect(this, &LobbyMenu::sigHostServer, this, &LobbyMenu::hostServer, Qt::QueuedConnection);

    oxygine::spButton pButtonJoin = ObjectManager::createButton(tr("Join Game"));
    pButtonJoin->attachTo(this);
    pButtonJoin->setPosition(Settings::getWidth() / 2 + 10, Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonJoin->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigJoinGame();
    });
    connect(this, &LobbyMenu::sigJoinGame, this, &LobbyMenu::joinGame, Qt::QueuedConnection);

    oxygine::spButton pButtonJoinAdress = ObjectManager::createButton(tr("Join Adress"));
    pButtonJoinAdress->attachTo(this);
    pButtonJoinAdress->setPosition(Settings::getWidth() / 2 - 10 - pButtonJoinAdress->getWidth(), Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonJoinAdress->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigJoinAdress();
    });
    connect(this, &LobbyMenu::sigJoinAdress, this, &LobbyMenu::joinAdress, Qt::QueuedConnection);

    m_pGamesPanel = spPanel::create(true, QSize(Settings::getWidth() - 20, Settings::getHeight() - 420),
                              QSize(Settings::getWidth() - 20, Settings::getHeight() - 420));
    m_pGamesPanel->setPosition(10, 10);
    addChild(m_pGamesPanel);

    spNetworkInterface pInterface = m_pTCPClient;
    if (Settings::getServer())
    {
        pInterface = MainServer::getInstance()->getGameServer();
    }

    spChat pChat = spChat::create(pInterface, QSize(Settings::getWidth() - 20, 300), NetworkInterface::NetworkSerives::LobbyChat);
    pChat->setPosition(10, m_pGamesPanel->getHeight() + 20);
    addChild(pChat);

    connect(this, &LobbyMenu::sigUpdateGamesView, this, &LobbyMenu::updateGamesView, Qt::QueuedConnection);
}

LobbyMenu::~LobbyMenu()
{
    if (!Settings::getServer() && !m_usedForHosting)
    {
        emit m_pTCPClient->sig_close();
        m_pTCPClient = nullptr;
    }
}

void LobbyMenu::exitMenue()
{    
    Console::print("Leaving Lobby Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(spMainwindow::create());
    oxygine::Actor::detach();    
}

void LobbyMenu::hostLocal()
{    
    Console::print("Leaving Lobby Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(spMultiplayermenu::create("", "", true));
    oxygine::Actor::detach();    
}

void LobbyMenu::hostServer()
{
    if (m_pTCPClient.get() != nullptr &&
        m_pTCPClient->getIsConnected())
    {
        m_usedForHosting = true;
        Console::print("Leaving Lobby Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(spMultiplayermenu::create(m_pTCPClient, "", true));
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
        Console::print("Leaving Lobby Menue", Console::eDEBUG);
        QString command = QString(NetworkCommands::SERVERJOINGAME);
        Console::print("Sending command " + command, Console::eDEBUG);
        m_usedForHosting = true;
        oxygine::getStage()->addChild(spMultiplayermenu::create(m_pTCPClient, password, false));
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << command;
        stream << m_currentGame->getSlaveName();
        emit m_pTCPClient->sig_sendData(0, data, NetworkInterface::NetworkSerives::ServerHosting, false);
        oxygine::Actor::detach();        
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
    
    Console::print("Leaving Lobby Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(spMultiplayermenu::create(adress, password, false));
    oxygine::Actor::detach();
    
}

void LobbyMenu::recieveData(quint64, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == NetworkInterface::NetworkSerives::ServerHosting)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        if (messageType == NetworkCommands::SERVERGAMEDATA)
        {
            m_games.clear();
            qint32 size = 0;
            stream >> size;
            for (qint32 i = 0; i < size; i++)
            {
                m_games.append(spNetworkGameData::create());
                m_games[i]->deserializeObject(stream);
            }
            emit sigUpdateGamesView();
        }
    }
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
