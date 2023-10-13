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

#include "resource_management/backgroundmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "objects/base/chat.h"
#include "objects/dialogs/dialogmessagebox.h"
#include "objects/dialogs/customdialog.h"
#include "objects/tableView/stringtableitem.h"
#include "objects/tableView/xofytableitem.h"
#include "objects/tableView/locktableitem.h"

#include "network/mainserver.h"
#include "network/JsonKeys.h"

LobbyMenu::LobbyMenu()
{
    Mainapp *pApp = Mainapp::getInstance();
    CONSOLE_PRINT("Entering Lobby Menue", GameConsole::eDEBUG);
    Interpreter::setCppOwnerShip(this);

    if (!Settings::getInstance()->getServer())
    {
        m_pTCPClient = MemoryManagement::create<TCPClient>(nullptr);
        m_pTCPClient->moveToThread(Mainapp::getInstance()->getNetworkThread());
        connect(m_pTCPClient.get(), &TCPClient::recieveData, this, &LobbyMenu::recieveData, NetworkCommands::UNIQUE_DATA_CONNECTION);
        connect(m_pTCPClient.get(), &TCPClient::sigConnected, this, &LobbyMenu::connected, Qt::QueuedConnection);
        emit m_pTCPClient->sig_connect(Settings::getInstance()->getServerAdress(), Settings::getInstance()->getServerPort(), Settings::getInstance()->getSecondaryServerAdress());
    }

    BackgroundManager *pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = MemoryManagement::create<oxygine::Sprite>();
    addChild(sprite);
    oxygine::ResAnim *pBackground = pBackgroundManager->getResAnim("lobbymenu");
    sprite->setResAnim(pBackground);
    sprite->setPosition(-1, -1);
    // background should be last to draw
    sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
    sprite->setScaleX(static_cast<float>(oxygine::Stage::getStage()->getWidth()) / static_cast<float>(pBackground->getWidth()));
    sprite->setScaleY(static_cast<float>(oxygine::Stage::getStage()->getHeight()) / static_cast<float>(pBackground->getHeight()));

    pApp->getAudioManager()->clearPlayList();
    pApp->getAudioManager()->loadFolder("resources/music/multiplayer");
    pApp->getAudioManager()->playRandom();

    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    addChild(pButtonExit);
    pButtonExit->setPosition(10, oxygine::Stage::getStage()->getHeight() - pButtonExit->getScaledHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *) -> void
                                  { emit sigExitMenue(); });
    connect(this, &LobbyMenu::sigExitMenue, this, &LobbyMenu::exitMenue, Qt::QueuedConnection);

    m_pOtherButton = ObjectManager::createButton(tr("Other"));
    addChild(m_pOtherButton);
    m_pOtherButton->setPosition(10, pButtonExit->getY() - m_pOtherButton->getScaledHeight());
    m_pOtherButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *) -> void
                                     { emit sigOther(); });
    connect(this, &LobbyMenu::sigOther, this, &LobbyMenu::onShowOtherDialog, Qt::QueuedConnection);
    m_pOtherButton->setEnabled(false);

    oxygine::spButton pButtonHost = ObjectManager::createButton(tr("Direct Host"), 220);
    addChild(pButtonHost);
    pButtonHost->setPosition(oxygine::Stage::getStage()->getWidth() - pButtonHost->getScaledWidth() - 10,
                             oxygine::Stage::getStage()->getHeight() - pButtonExit->getScaledHeight() - 10);
    pButtonHost->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *) -> void
                                  { emit sigHostLocal(); });
    connect(this, &LobbyMenu::sigHostLocal, this, &LobbyMenu::hostLocal, Qt::QueuedConnection);

    m_pButtonHostOnServer = ObjectManager::createButton(tr("Server Host"), 220);
    addChild(m_pButtonHostOnServer);
    m_pButtonHostOnServer->setPosition(oxygine::Stage::getStage()->getWidth() - pButtonHost->getScaledWidth() - 10,
                                       oxygine::Stage::getStage()->getHeight() - pButtonExit->getScaledHeight() * 2 - 10);
    m_pButtonHostOnServer->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *) -> void
                                            { emit sigHostServer(); });
    connect(this, &LobbyMenu::sigHostServer, this, &LobbyMenu::hostServer, Qt::QueuedConnection);
    m_pButtonHostOnServer->setEnabled(false);

    m_pButtonGameJoin = ObjectManager::createButton(tr("Join Game"), 220);
    addChild(m_pButtonGameJoin);
    m_pButtonGameJoin->setPosition(oxygine::Stage::getStage()->getWidth() / 2 + 10,
                                   oxygine::Stage::getStage()->getHeight() - pButtonExit->getScaledHeight() - 10);
    m_pButtonGameJoin->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *) -> void
                                        { emit sigJoinGame(); });
    connect(this, &LobbyMenu::sigJoinGame, this, &LobbyMenu::joinGame, Qt::QueuedConnection);
    m_pButtonGameJoin->setEnabled(false);

    m_pButtonGameObserve = ObjectManager::createButton(tr("Observe Game"), 220);
    addChild(m_pButtonGameObserve);
    m_pButtonGameObserve->setPosition(oxygine::Stage::getStage()->getWidth() / 2 + 10, m_pButtonGameJoin->getY() - m_pButtonGameJoin->getScaledHeight());
    m_pButtonGameObserve->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *) -> void
                                           { emit sigObserveGame(); });
    connect(this, &LobbyMenu::sigObserveGame, this, &LobbyMenu::observeGame, Qt::QueuedConnection);
    m_pButtonGameObserve->setEnabled(false);

    oxygine::spButton pButtonJoinAdress = ObjectManager::createButton(tr("Join Address"), 220);
    addChild(pButtonJoinAdress);
    pButtonJoinAdress->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - 10 - pButtonJoinAdress->getScaledWidth(),
                                   oxygine::Stage::getStage()->getHeight() - pButtonExit->getScaledHeight() - 10);
    pButtonJoinAdress->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *) -> void
                                        { emit sigJoinAdress(); });
    connect(this, &LobbyMenu::sigJoinAdress, this, &LobbyMenu::joinAdress, Qt::QueuedConnection);

    oxygine::spButton pButtonObserveAdress = ObjectManager::createButton(tr("Observe Address"), 220);
    addChild(pButtonObserveAdress);
    pButtonObserveAdress->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - 10 - pButtonJoinAdress->getScaledWidth(),
                                      pButtonJoinAdress->getY() - pButtonJoinAdress->getScaledHeight());
    pButtonObserveAdress->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *) -> void
                                           { emit sigObserveAdress(); });
    connect(this, &LobbyMenu::sigObserveAdress, this, &LobbyMenu::observeAdress, Qt::QueuedConnection);

    m_pButtonSwapOwnGamesMode = ObjectManager::createButton(tr("My games"), 200);
    addChild(m_pButtonSwapOwnGamesMode);
    m_pButtonSwapOwnGamesMode->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_pButtonSwapOwnGamesMode->getScaledWidth() * 2, 10);
    m_pButtonSwapOwnGamesMode->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *) -> void
                                                {
        m_mode = GameViewMode::OwnGames;
        emit sigRequestUpdateGames(); });
    m_pButtonSwapOwnGamesMode->setEnabled(false);

    m_pButtonSwapOpenGamesMode = ObjectManager::createButton(tr("Open games"), 200);
    addChild(m_pButtonSwapOpenGamesMode);
    m_pButtonSwapOpenGamesMode->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_pButtonSwapOpenGamesMode->getScaledWidth(), 10);
    m_pButtonSwapOpenGamesMode->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *) -> void
                                                 {
        m_mode = GameViewMode::OpenGames;
        emit sigRequestUpdateGames(); });
    m_pButtonSwapOpenGamesMode->setEnabled(false);

    m_pButtonSwapObserveGamesMode = ObjectManager::createButton(tr("Observable"), 200);
    addChild(m_pButtonSwapObserveGamesMode);
    m_pButtonSwapObserveGamesMode->setPosition(oxygine::Stage::getStage()->getWidth() / 2, 10);
    m_pButtonSwapObserveGamesMode->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *) -> void
                                                    {
        m_mode = GameViewMode::ObserveGames;
        emit sigRequestUpdateGames(); });
    m_pButtonSwapObserveGamesMode->setEnabled(false);

    m_pButtonUpdateGamesMode = ObjectManager::createButton(tr("Refresh games"), 200);
    addChild(m_pButtonUpdateGamesMode);
    m_pButtonUpdateGamesMode->setPosition(oxygine::Stage::getStage()->getWidth() / 2 + m_pButtonUpdateGamesMode->getScaledWidth(), 10);
    m_pButtonUpdateGamesMode->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *) -> void
                                               { emit sigRequestUpdateGames(); });
    m_pButtonUpdateGamesMode->setEnabled(false);
    connect(this, &LobbyMenu::sigRequestUpdateGames, this, &LobbyMenu::requestUpdateGames, Qt::QueuedConnection);

    qint32 height = m_pButtonHostOnServer->getY() - 270 - 10 - m_pButtonSwapOpenGamesMode->getScaledHeight();
    if (Settings::getInstance()->getSmallScreenDevice())
    {
        height = m_pButtonHostOnServer->getY() - 20 - m_pButtonSwapOpenGamesMode->getScaledHeight();
    }

    QStringList header = {tr("Map"), tr("Players"), tr("Description"), tr("Mods"), tr("Locked")};
    QVector<qint32> widths = GlobalUtils::calcWidths({oxygine::Stage::getStage()->getWidth(), 200, oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getWidth(), 200},
                                                     {0.25f, 0.125f, 0.25f, 0.25f, 0.125f},
                                                     oxygine::Stage::getStage()->getWidth() - 20 - 80);
    m_gamesview = MemoryManagement::create<ComplexTableView>(widths, header, height);
    m_gamesview->setPosition(10, 10 + 10 + pButtonJoinAdress->getScaledHeight());
    addChild(m_gamesview);

    spNetworkInterface pInterface = m_pTCPClient;
    if (Settings::getInstance()->getServer())
    {
        pInterface = MainServer::getInstance()->getGameServer();
    }
    qint32 y = m_gamesview->getY() + m_gamesview->getScaledHeight() + 10;
    const qint32 infoWidth = 100;
    m_matchViewInfo = MemoryManagement::create<Label>(infoWidth * 2, true);
    auto style = m_matchViewInfo->getStyle();
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    m_matchViewInfo->setStyle(style);
    m_matchViewInfo->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - infoWidth, y);
    m_matchViewInfo->setText("-- / --");
    addChild(m_matchViewInfo);
    m_pNextStepButton = ObjectManager::createIconButton("next_unit", 36);
    m_pNextStepButton->setEnabled(false);
    m_pNextStepButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 + 10 + infoWidth, y);
    m_pNextStepButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *)
                                        { emit sigShowNextStep(); });
    addChild(m_pNextStepButton);
    m_pEndStepButton = ObjectManager::createIconButton("toEnd", 36);
    m_pEndStepButton->setEnabled(false);
    m_pEndStepButton->setPosition(m_pNextStepButton->getX() + m_pNextStepButton->getScaledWidth() + 10, y);
    m_pEndStepButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *)
                                       { emit sigShowEnd(); });
    addChild(m_pEndStepButton);

    m_pPreviousStepButton = ObjectManager::createIconButton("previous_unit", 36);
    m_pPreviousStepButton->setEnabled(false);
    m_pPreviousStepButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - 10 - infoWidth - m_pPreviousStepButton->getScaledWidth(), y);
    m_pPreviousStepButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *)
                                            { emit sigShowPreviousStep(); });
    addChild(m_pPreviousStepButton);

    m_pStartStepButton = ObjectManager::createIconButton("toStart", 36);
    m_pStartStepButton->setEnabled(false);
    m_pStartStepButton->setPosition(m_pPreviousStepButton->getX() - m_pPreviousStepButton->getScaledWidth() - 10, y);
    m_pStartStepButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *)
                                         { emit sigShowStart(); });
    addChild(m_pStartStepButton);

    connect(this, &LobbyMenu::sigShowNextStep, this, &LobbyMenu::showNextStep, Qt::QueuedConnection);
    connect(this, &LobbyMenu::sigShowPreviousStep, this, &LobbyMenu::showPreviousStep, Qt::QueuedConnection);
    connect(this, &LobbyMenu::sigShowStart, this, &LobbyMenu::showStart, Qt::QueuedConnection);
    connect(this, &LobbyMenu::sigShowEnd, this, &LobbyMenu::showEnd, Qt::QueuedConnection);
    y += 50;

    height = m_pButtonHostOnServer->getY() - m_gamesview->getY() - m_gamesview->getScaledHeight() - 70;

    spChat pChat = MemoryManagement::create<Chat>(pInterface, QSize(oxygine::Stage::getStage()->getWidth() - 20, height), NetworkInterface::NetworkSerives::LobbyChat, nullptr);
    pChat->setPosition(10, y);
    if (Settings::getInstance()->getSmallScreenDevice())
    {
        pChat->setVisible(false);
    }
    addChild(pChat);
    connect(this, &LobbyMenu::sigUpdateGamesView, this, &LobbyMenu::updateGamesView, Qt::QueuedConnection);
    connect(m_gamesview.get(), &ComplexTableView::sigItemClicked, this, &LobbyMenu::selectGame, Qt::QueuedConnection);
}

void LobbyMenu::requestUpdateGames()
{
    switch (m_mode)
    {
    case GameViewMode::OpenGames:
    {
        requestServerGames();
        break;
    }
    case GameViewMode::OwnGames:
    {
        requestUserUpdateGames();
        break;
    }
    case GameViewMode::ObserveGames:
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
    m_pButtonGameObserve->setEnabled(enable);
    m_pButtonGameJoin->setEnabled(enable);
    m_pButtonHostOnServer->setEnabled(enable);
    m_pButtonSwapOpenGamesMode->setEnabled(enable);
    m_pButtonSwapOwnGamesMode->setEnabled(enable);
    m_pButtonSwapObserveGamesMode->setEnabled(enable);
    m_pButtonUpdateGamesMode->setEnabled(enable);
    m_pEndStepButton->setEnabled(enable);
    m_pNextStepButton->setEnabled(enable);
    m_pPreviousStepButton->setEnabled(enable);
    m_pStartStepButton->setEnabled(enable);
    m_pOtherButton->setEnabled(enable);
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
    oxygine::Actor::detach();
}

void LobbyMenu::hostLocal()
{
    CONSOLE_PRINT("Leaving Lobby Menue", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    oxygine::Stage::getStage()->addChild(MemoryManagement::create<Multiplayermenu>("", "", Settings::getInstance()->getGamePort(), "", Multiplayermenu::NetworkMode::Host));
    oxygine::Actor::detach();
}

void LobbyMenu::hostServer()
{
    if (m_pTCPClient.get() != nullptr &&
        m_pTCPClient->getIsConnected())
    {
        CONSOLE_PRINT("Leaving Lobby Menue", GameConsole::eDEBUG);
        m_onEnterTimer.stop();
        oxygine::Stage::getStage()->addChild(MemoryManagement::create<Multiplayermenu>(m_pTCPClient, "", Multiplayermenu::NetworkMode::Host));
        oxygine::Actor::detach();
    }
}

void LobbyMenu::joinGame()
{
    if (m_currentGame.getUuid() != 0)
    {
        if ((m_mode == GameViewMode::OwnGames || m_currentGame.hasOpenPlayers()))
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
    oxygine::Actor::detach();
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
    oxygine::Actor::detach();
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
    oxygine::Actor::detach();
}

void LobbyMenu::observeGame()
{
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
        oxygine::Actor::detach();
    }
}

void LobbyMenu::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == NetworkInterface::NetworkSerives::ServerHostingJson)
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject objData = doc.object();
        QString messageType = objData.value(JsonKeys::JSONKEY_COMMAND).toString();
        CONSOLE_PRINT("LobbyMenu Command received: " + messageType, GameConsole::eDEBUG);
        if (messageType == NetworkCommands::SERVERVERSION)
        {
            checkVersionAndShowInfo(objData);
        }
        else if (messageType == NetworkCommands::SERVERGAMEDATA)
        {
            if (m_loggedIn &&
                (m_mode == GameViewMode::OpenGames || m_mode == GameViewMode::ObserveGames))
            {
                updateGameData(objData);
            }
        }
        else if (messageType == NetworkCommands::SERVERUSERGAMEDATA)
        {
            if (m_loggedIn && m_mode == GameViewMode::OwnGames)
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
        else if (messageType == NetworkCommands::SERVERSENDAUTOMATCHINFO)
        {
            receivedShowAutoMatches(objData);
        }
        else
        {
            CONSOLE_PRINT("Unknown command in LobbyMenu::recieveData " + messageType + " received", GameConsole::eDEBUG);
        }
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
    for (const auto &username : usernames)
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
    for (const auto &game : games)
    {
        NetworkGameData gameData;
        gameData.fromJson(game.toObject());
        m_games.append(gameData);
    }
    m_matchViewInfo->setText(QString::number(m_gameIndex) + " / " + QString::number(m_gameIndex + games.size()));
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
    oxygine::Actor::detach();
}

void LobbyMenu::updateGamesView()
{
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
    if (m_gamesview->getCurrentItem() >= 0)
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
    emit m_pTCPClient->sig_sendData(0, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void LobbyMenu::checkVersionAndShowInfo(const QJsonObject &objData)
{
    QString version = objData.value(JsonKeys::JSONKEY_VERSION).toString();
    if (version == Mainapp::getGameVersion())
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
    else
    {
        spDialogMessageBox pDialogMessageBox;
        pDialogMessageBox = MemoryManagement::create<DialogMessageBox>(tr("Connection refused. Server has a different version of the game. Server ") + version);
        addChild(pDialogMessageBox);
        m_pTCPClient.reset();
    }
}

void LobbyMenu::onLogin()
{
    enableServerButtons(true);
    m_loggedIn = true;
    requestServerGames();
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
        pInterpreter->doFunction(object, "onAccountMessage", args);
    }
    else
    {
        CONSOLE_PRINT("Account message is empty.", GameConsole::eDEBUG);
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