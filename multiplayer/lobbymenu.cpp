#include "lobbymenu.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"
#include "coreengine/audiothread.h"

#include "menue/mainwindow.h"

#include "multiplayer/multiplayermenu.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"

#include "objects/chat.h"

#include "objects/dialogtextinput.h"
#include "network/mainserver.h"

#include "multiplayer/networkcommands.h"

LobbyMenu::LobbyMenu()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Lobby Menue", Console::eDEBUG);

    if (!Settings::getServer())
    {
        m_pTCPClient = new TCPClient();
        connect(m_pTCPClient.get(), &TCPClient::recieveData, this, &LobbyMenu::recieveData, Qt::QueuedConnection);
        emit m_pTCPClient->sig_connect(Settings::getServerAdress(), Settings::getServerPort());
    }

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("Background+1");
    sprite->setResAnim(pBackground);
    sprite->setPosition(-1, -1);
    // background should be last to draw
    sprite->setPriority(static_cast<short>(Mainapp::ZOrder::Background));
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

    m_pGamesPanel = new Panel(true, QSize(Settings::getWidth() - 20, Settings::getHeight() - 420),
                              QSize(Settings::getWidth() - 20, Settings::getHeight() - 420));
    m_pGamesPanel->setPosition(10, 10);
    addChild(m_pGamesPanel);

    spNetworkInterface pInterface = m_pTCPClient;
    if (Settings::getServer())
    {
        pInterface = MainServer::getInstance()->getGameServer();
    }

    spChat pChat = new Chat(pInterface, QSize(Settings::getWidth() - 20, 300), NetworkInterface::NetworkSerives::LobbyChat);
    pChat->setPosition(10, m_pGamesPanel->getHeight() + 20);
    addChild(pChat);

    connect(this, &LobbyMenu::sigUpdateGamesView, this, &LobbyMenu::updateGamesView, Qt::QueuedConnection);
}

LobbyMenu::~LobbyMenu()
{
    if (!Settings::getServer())
    {
        emit m_pTCPClient->sig_close();
        m_pTCPClient = nullptr;
    }
}

void LobbyMenu::exitMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    Console::print("Leaving Lobby Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new Mainwindow());
    addRef();
    oxygine::Actor::detach();
    deleteLater();
    pApp->continueThread();
}

void LobbyMenu::hostLocal()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    Console::print("Leaving Lobby Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new Multiplayermenu("", true));
    addRef();
    oxygine::Actor::detach();
    deleteLater();
    pApp->continueThread();
}

void LobbyMenu::hostServer()
{
    if (m_pTCPClient.get() != nullptr)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        Console::print("Leaving Lobby Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new Multiplayermenu(m_pTCPClient, true));
        addRef();
        oxygine::Actor::detach();
        deleteLater();
        pApp->continueThread();
    }
}

void LobbyMenu::joinGame()
{
    bool exists = false;
    if (m_currentGame.get() != nullptr)
    {
        for (const auto & game : m_games)
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
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        Console::print("Leaving Lobby Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new Multiplayermenu(m_pTCPClient, false));
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << NetworkCommands::SERVERJOINGAME;
        stream << m_currentGame->getSlaveName();
        emit m_pTCPClient->sig_sendData(0, data, NetworkInterface::NetworkSerives::ServerHosting, false);
        addRef();
        oxygine::Actor::detach();
        deleteLater();
        pApp->continueThread();
    }
}

void LobbyMenu::joinAdress()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();

    spDialogTextInput pDialogTextInput = new DialogTextInput(tr("Enter Host Adress"), true, "");
    addChild(pDialogTextInput);
    connect(pDialogTextInput.get(), &DialogTextInput::sigTextChanged, this, &LobbyMenu::join, Qt::QueuedConnection);

    pApp->continueThread();
}

void LobbyMenu::join(QString adress)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    Console::print("Leaving Lobby Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new Multiplayermenu(adress, false));
    addRef();
    oxygine::Actor::detach();
    deleteLater();
    pApp->continueThread();
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
                m_games.append(new NetworkGameData());
                m_games[i]->deserializeObject(stream);
            }
            emit sigUpdateGamesView();
        }
    }
}

void LobbyMenu::updateGamesView()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (m_Gamesview.get() != nullptr)
    {
        m_Gamesview->detach();
        m_Gamesview = nullptr;
    }
    QStringList header = {tr("Map"), tr("Players"), tr("Description"), tr("Mods")};
    qint32 itemWidth = (m_pGamesPanel->getWidth() - 80 - 100) / 3;
    QVector<qint32> widths = {itemWidth, 100, itemWidth, itemWidth};
    QVector<QStringList> items;
    for (const auto & game : m_games)
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
            modString.append(mod + "; ");
        }
        data.append(modString);
        items.append(data);
    }
    m_Gamesview = new TableView(widths, items, header, true);
    connect(m_Gamesview.get(), &TableView::sigItemClicked, this, &LobbyMenu::selectGame, Qt::QueuedConnection);
    m_pGamesPanel->addItem(m_Gamesview);
    m_pGamesPanel->setContentHeigth(m_Gamesview->getHeight() + 40);
    pApp->continueThread();
}

void LobbyMenu::selectGame()
{
    qint32 game = m_Gamesview->getCurrentItem();
    if (game >= 0 && game < m_games.size())
    {
        m_currentGame = m_games[game];
    }
}
