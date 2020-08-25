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

LobbyMenu::LobbyMenu()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Lobby Menue", Console::eDEBUG);

    if (!Settings::getServer())
    {
        m_pTCPClient = new TCPClient();
        emit m_pTCPClient->sig_connect(Settings::getServerAdress(), Settings::getServerPort());
    }

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("Background+1");
    sprite->setResAnim(pBackground);
    sprite->setPosition(0, 0);
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
    pButtonJoin->setVisible(false);
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
        oxygine::getStage()->addChild(new Multiplayermenu(m_pTCPClient));
        addRef();
        oxygine::Actor::detach();
        deleteLater();
        pApp->continueThread();
    }
}

void LobbyMenu::joinGame()
{

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
