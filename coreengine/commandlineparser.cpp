#include <QApplication>

#include "coreengine/commandlineparser.h"
#include "coreengine/mainapp.h"

#include "multiplayer/multiplayermenu.h"

#include "network/tcpserver.h"

const char* const CommandLineParser::ARG_MODS = "mods";
const char* const CommandLineParser::ARG_SLAVE = "slave";
const char* const CommandLineParser::ARG_SLAVENAME = "slaveServer";
const char* const CommandLineParser::ARG_NOUI = "noUi";
const char* const CommandLineParser::ARG_NOAUDIO = "noAudio";
const char* const CommandLineParser::ARG_INITSCRIPT = "initScript";
const char* const CommandLineParser::ARG_CREATESLAVELOGS = "createSlaveLogs";
const char* const CommandLineParser::ARG_SLAVEADDRESS = "slaveAdress";
const char* const CommandLineParser::ARG_SLAVEPORT = "slavePort";
const char* const CommandLineParser::ARG_MASTERADDRESS = "masterAdress";
const char* const CommandLineParser::ARG_MASTERPORT= "masterPort";

// options required for hosting a dedicated server
const char* const CommandLineParser::ARG_SERVER = "server";
const char* const CommandLineParser::ARG_SERVERSLAVEHOSTOPTIONS = "slaveOptions";
const char* const CommandLineParser::ARG_SERVERLISTENADDRESS    = "serverListenAddress";
const char* const CommandLineParser::ARG_SERVERLISTENPORT       = "serverListenPort";
const char* const CommandLineParser::ARG_SERVERSLAVELISTENADDRESS    = "serverSlaveListenAddress";
const char* const CommandLineParser::ARG_SERVERSLAVELISTENPORT       = "serverSlaveListenPort";
const char* const CommandLineParser::ARG_SERVERSLAVEDESPAWNTIME      = "serverSlaveDespawnTime";

CommandLineParser::CommandLineParser()
    : m_mods(ARG_MODS, tr("mods that should be loaded. As a string list separated by ';'")),
      m_slave(ARG_SLAVE, tr("If the exe is started as a slave process.")),
      m_noUi(ARG_NOUI, tr("If the exe is started in headless mode")),
      m_noAudio(ARG_NOAUDIO, tr("If the exe is started muted and sound can't be turned on")),
      m_iniScript(ARG_INITSCRIPT, tr("Path to a js script that gets triggered by the game to automate or test things")),
      m_createSlaveLogs(ARG_CREATESLAVELOGS, tr("If the game should create logs for spawned slave processes")),
      m_slaveAddress(ARG_SLAVEADDRESS, tr("Address on which the game will listen for new clients")),
      m_slavePort(ARG_SLAVEPORT, tr("Port on which the game will listen for new clients")),
      m_masterAddress(ARG_MASTERADDRESS, tr("Address on which the game will connect to the hosting server to exchange data"), "ip-address", "::1"),
      m_masterPort(ARG_MASTERPORT, tr("Port on which the game will connect to the hosting server to exchange data")),
      m_slaveName(ARG_SLAVENAME, tr("Unique name to identify the slave on the server side")),
      m_server(ARG_SERVER, tr("If set the game launches the dedicated server."), "0"),
      m_serverSlaveHostOptions(ARG_SERVERSLAVEHOSTOPTIONS, tr("Ip-Address and Port range separated by '&' for the 3 parts and ';' for gaps or different addresses on which slave games will be spawned to listen. E.g. ::1&10000&20000;::1&50000&65535. Not the Ip-Address needs to be accessible by connecting clients.")),
      m_serverListenAddress(ARG_SERVERLISTENADDRESS, tr("The address on which the server will listen for clients. Empty for all addresses.")),
      m_serverListenPort(ARG_SERVERLISTENPORT, tr("Port on which the server will initially listen for clients.")),
      m_serverSlaveListenAddress(ARG_SERVERSLAVELISTENADDRESS, tr("The address on which the server will listen for slave games. Empty for all addresses."), "slaveListenAddress", "::1"),
      m_serverSlaveListenPort(ARG_SERVERSLAVELISTENPORT, tr("Port on which the server will listen for slave games.")),
      m_serverSlaveDespawnTime(ARG_SERVERSLAVEDESPAWNTIME, tr("Time in seconds till a slave game with no connected clients get despawned in seconds."))
{
    m_parser.setApplicationDescription("Commander Wars game");
    m_parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    m_parser.addHelpOption();
    m_parser.addVersionOption();
    m_parser.addOption(m_mods);
    m_parser.addOption(m_slave);
    m_parser.addOption(m_noUi);
    m_parser.addOption(m_noAudio);
    m_parser.addOption(m_iniScript);
    m_parser.addOption(m_createSlaveLogs);
    m_parser.addOption(m_slaveAddress);
    m_parser.addOption(m_slavePort);
    m_parser.addOption(m_masterAddress);
    m_parser.addOption(m_masterPort);
    m_parser.addOption(m_slaveName);
    m_parser.addOption(m_server);
    m_parser.addOption(m_serverSlaveHostOptions);
    m_parser.addOption(m_serverListenAddress);
    m_parser.addOption(m_serverListenPort);
    m_parser.addOption(m_serverSlaveListenAddress);
    m_parser.addOption(m_serverSlaveListenPort);
    m_parser.addOption(m_serverSlaveDespawnTime);
}

void CommandLineParser::parseArgs(QApplication & app)
{
    m_parser.process(app);
    Mainapp* pApp = Mainapp::getInstance();
    if (m_parser.isSet(m_noUi))
    {
        Settings::setOverworldAnimations(false);
        Settings::setBattleAnimationType(GameEnums::BattleAnimationType_Overworld);
        Settings::setBattleAnimationMode(GameEnums::BattleAnimationMode_None);
        Settings::setAnimationSpeed(100);
        Settings::setWalkAnimationSpeed(100);
        Settings::setBattleAnimationSpeed(100);
        Settings::setDialogAnimation(false);
        Settings::setDialogAnimationSpeed(100);
        pApp->setNoUi();
    }
    if (m_parser.isSet(m_noAudio))
    {
        pApp->setNoAudio();
    }
    QString mods = m_parser.value(m_mods);
    if (!mods.isEmpty())
    {
        QStringList modList = mods.split(",");
        qint32 i= 0;
        while (i < modList.size())
        {
            if (modList[i].isEmpty())
            {
                modList.removeAt(i);
            }
            else
            {
                ++i;
            }
        }
        CONSOLE_PRINT("Using injected mod list: " + mods, Console::eDEBUG);
        Settings::setActiveMods(modList);
    }
    if (m_parser.isSet(m_slave))
    {
        pApp->actAsSlave();
    }
    if (m_parser.isSet(m_slaveName))
    {
        Settings::setSlaveServerName(m_parser.value(m_slaveName));
    }
    if (m_parser.isSet(m_iniScript))
    {
        pApp->setInitScript(m_parser.value(m_iniScript));
    }
    if (m_parser.isSet(m_createSlaveLogs))
    {
        pApp->setCreateSlaveLogs(true);
    }
    if (m_parser.isSet(m_server))
    {
        Settings::setServer(m_parser.value(m_server) == "1");
    }
    if (m_parser.isSet(m_serverSlaveHostOptions))
    {
        Settings::setSlaveHostOptions(m_parser.value(m_serverSlaveHostOptions));
    }
    if (m_parser.isSet(m_serverListenAddress))
    {
        Settings::setServerListenAdress(m_parser.value(m_serverListenAddress));
    }
    if (m_parser.isSet(m_serverListenPort))
    {
        bool ok = false;
        Settings::setServerPort(m_parser.value(m_serverListenPort).toInt(&ok));
    }
    if (m_parser.isSet(m_serverSlaveListenAddress))
    {
        Settings::setSlaveListenAdress(m_parser.value(m_serverSlaveListenAddress));
    }
    if (m_parser.isSet(m_serverSlaveListenPort))
    {
        bool ok = false;
        Settings::setSlaveServerPort(m_parser.value(m_serverSlaveListenPort).toInt(&ok));
    }
    if (m_parser.isSet(m_serverSlaveDespawnTime))
    {
        Settings::setSlaveDespawnTime(std::chrono::seconds(m_parser.value(m_serverSlaveDespawnTime).toInt()));
    }
}

void CommandLineParser::startSlaveGame() const
{
    QString slaveAddress;
    quint16 slavePort = 0;
    if (m_parser.isSet(m_slaveAddress) && m_parser.isSet(m_slavePort))
    {
        bool ok = false;
        slaveAddress = m_parser.value(m_slaveAddress);
        slavePort = m_parser.value(m_slavePort).toInt(&ok);
        if (!ok)
        {
            slaveAddress = "";
        }
    }
    QString masterAddress;
    quint16 masterPort = 0;
    if (m_parser.isSet(m_masterPort))
    {
        bool ok = false;
        masterAddress = m_parser.value(m_masterAddress);
        masterPort = m_parser.value(m_masterPort).toInt(&ok);
        if (!ok)
        {
            masterPort = 0;
        }
        if (masterAddress.isEmpty())
        {
            masterAddress = "::1";
        }
    }
    if (!slaveAddress.isEmpty() && masterPort > 0 && slavePort > 0 && !masterAddress.isEmpty())
    {
        // init multiplayer menu
        spTCPServer pServer = spTCPServer::create(nullptr);
        pServer->moveToThread(Mainapp::getInstance()->getNetworkThread());
        spMultiplayermenu pMenu = spMultiplayermenu::create(pServer, "", Multiplayermenu::NetworkMode::Host);
        pMenu->connectNetworkSlots();
        oxygine::Stage::getStage()->addChild(pMenu);
        emit pServer->sig_connect(slaveAddress, slavePort);
        // connect to server
        spTCPClient pSlaveMasterConnection = Mainapp::getSlaveClient();
        connect(pSlaveMasterConnection.get(), &TCPClient::sigConnected, pMenu.get(), &Multiplayermenu::onSlaveConnectedToMaster, Qt::QueuedConnection);
        connect(pSlaveMasterConnection.get(), &TCPClient::recieveData, pMenu.get(), &Multiplayermenu::recieveServerData, Qt::QueuedConnection);
        emit pSlaveMasterConnection->sig_connect(masterAddress, masterPort);

    }
    else
    {
        QApplication::exit(-3);
    }
}
