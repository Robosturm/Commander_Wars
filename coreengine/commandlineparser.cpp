#include <QCoreApplication>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "coreengine/commandlineparser.h"
#include "coreengine/mainapp.h"

#include "game/gameanimation/animationskipper.h"

#include "multiplayer/multiplayermenu.h"

#include "network/tcpserver.h"
#include "network/mainserver.h"

#include "3rd_party/smtpClient/src/smtpclient.h"

const char* const CommandLineParser::ARG_MODS                   = "mods";
const char* const CommandLineParser::ARG_SLAVE                  = "slave";
const char* const CommandLineParser::ARG_SLAVENAME              = "slaveServer";
const char* const CommandLineParser::ARG_NOUI                   = "noUi";
const char* const CommandLineParser::ARG_NOAUDIO                = "noAudio";
const char* const CommandLineParser::ARG_INITSCRIPT             = "initScript";
const char* const CommandLineParser::ARG_CREATESLAVELOGS        = "createSlaveLogs";
const char* const CommandLineParser::ARG_SLAVEADDRESS           = "slaveAdress";
const char* const CommandLineParser::ARG_SLAVESECONDARYADDRESS  = "slaveSecondaryAdress";
const char* const CommandLineParser::ARG_SLAVEPORT              = "slavePort";
const char* const CommandLineParser::ARG_MASTERADDRESS          = "masterAdress";
const char* const CommandLineParser::ARG_MASTERPORT             = "masterPort";
const char* const CommandLineParser::ARG_UPDATE                 = "update";
const char* const CommandLineParser::ARG_SPAWNAIPROCESS         = "spawnAiProcess";
const char* const CommandLineParser::ARG_AISLAVE                = "aiSlave";
const char* const CommandLineParser::ARG_USERPATH               = "userPath";
const char* const CommandLineParser::ARG_DEBUGLEVEL             = "debugLevel";

// options required for hosting a dedicated server
const char* const CommandLineParser::ARG_SERVER                     = "server";
const char* const CommandLineParser::ARG_SERVERSLAVEHOSTOPTIONS         = "slaveOptions";
const char* const CommandLineParser::ARG_SERVERLISTENADDRESS            = "serverListenAddress";
const char* const CommandLineParser::ARG_SERVERSECONDARYLISTENADDRESS   = "serverSecondaryListenAddress";
const char* const CommandLineParser::ARG_SERVERLISTENPORT               = "serverListenPort";
const char* const CommandLineParser::ARG_SERVERSLAVELISTENADDRESS       = "serverSlaveListenAddress";
const char* const CommandLineParser::ARG_SERVERSLAVELISTENPORT          = "serverSlaveListenPort";
const char* const CommandLineParser::ARG_SERVERSLAVEDESPAWNTIME         = "serverSlaveDespawnTime";

const char* const CommandLineParser::ARG_MAILSERVERADDRESS = "mailServerAddress";
const char* const CommandLineParser::ARG_MAILSERVERPORT = "mailServerPort";
const char* const CommandLineParser::ARG_MAILSERVERCONNECTIONTYPE = "mailServerConnectionType";
const char* const CommandLineParser::ARG_MAILSERVERUSERNAME = "mailServerUsername";
const char* const CommandLineParser::ARG_MAILSERVERPASSWORD = "mailServerPassword";
const char* const CommandLineParser::ARG_MAILSERVERSENDADDRESS = "mailServerSendAddress";
const char* const CommandLineParser::ARG_MAILSERVERAUTHMETHOD = "mailServerAuthMethod";

CommandLineParser::CommandLineParser()
    : m_debugLevel(ARG_DEBUGLEVEL, tr("Debug level for the next sessions"), tr("level"), "1"),
      m_userPath(ARG_USERPATH, tr("Userpath for the game to use for user files to be stored"), tr("path"), ""),
      m_aiSlave(ARG_AISLAVE, tr("Acts as ai slave process")),
      m_spawnAiProcess(ARG_SPAWNAIPROCESS, tr("mode for starting the sub ai process. Off=0 Spawn=1"), tr("mode"), "1"),
      m_mods(ARG_MODS, tr("mods that should be loaded. As a string list separated by ';'"), tr("mod list"), ""),
      m_update(ARG_UPDATE, tr("Only used internal to tell the game that an update is in progresss"), tr("update step"), ""),
      m_slave(ARG_SLAVE, tr("If the exe is started as a slave process.")),
      m_noUi(ARG_NOUI, tr("If the exe is started in headless mode")),
      m_noAudio(ARG_NOAUDIO, tr("If the exe is started muted and sound can't be turned on")),
      m_iniScript(ARG_INITSCRIPT, tr("Path to a js script that gets triggered by the game to automate or test things"), tr("script"), ""),
      m_createSlaveLogs(ARG_CREATESLAVELOGS, tr("If the game should create logs for spawned slave processes")),
      m_slaveAddress(ARG_SLAVEADDRESS, tr("Address on which the game will listen for new clients"), tr("ip-adress"), ""),
      m_secondarySlaveAddress(ARG_SLAVESECONDARYADDRESS, tr("Secondary address on which the game will listen for new clients"), tr("ip-address"), ""),
      m_slavePort(ARG_SLAVEPORT, tr("Port on which the game will listen for new clients"), tr("port"), "0"),
      m_masterAddress(ARG_MASTERADDRESS, tr("Address on which the game will connect to the hosting server to exchange data"), "ip-address", "::1"),
      m_masterPort(ARG_MASTERPORT, tr("Port on which the game will connect to the hosting server to exchange data"), tr("port"), ""),
      m_slaveName(ARG_SLAVENAME, tr("Unique name to identify the slave on the server side"), tr("name"), "SlaveLog"),
      m_server(ARG_SERVER, tr("If set the game launches the dedicated server."), tr("server"), "0"),
      m_serverSlaveHostOptions(ARG_SERVERSLAVEHOSTOPTIONS, tr("Ip-Address and Port range separated by '&' for the 3 parts and ';' for gaps or different addresses on which slave games will be spawned to listen. E.g. ::1&10000&20000;::1&50000&65535. Note the Ip-Address needs to be accessible by connecting clients."), tr("options"), ""),
      m_serverListenAddress(ARG_SERVERLISTENADDRESS, tr("The address on which the server will listen for clients. Empty for all addresses."), tr("ip-address"), ""),
      m_serverSecondaryListenAddress(ARG_SERVERSECONDARYLISTENADDRESS, tr("The secondary address on which the server will listen for clients. Empty for all addresses."), tr("ip-address"), ""),
      m_serverListenPort(ARG_SERVERLISTENPORT, tr("Port on which the server will initially listen for clients."), tr("port"), ""),
      m_serverSlaveListenAddress(ARG_SERVERSLAVELISTENADDRESS, tr("The address on which the server will listen for slave games. Empty for all addresses."), "slaveListenAddress", "::1"),
      m_serverSlaveListenPort(ARG_SERVERSLAVELISTENPORT, tr("Port on which the server will listen for slave games."), tr("port"), ""),
      m_serverSlaveDespawnTime(ARG_SERVERSLAVEDESPAWNTIME, tr("Time in seconds till a slave game with no connected clients get despawned in seconds."), tr("time"), "60000"),
      m_mailServerAddress(ARG_MAILSERVERADDRESS, tr("Mail server address for the server for sending mails to accounts."), tr("address"), ""),
      m_mailServerPort(ARG_MAILSERVERPORT, tr("Mail server port for the server for sending mails to accounts."), tr("port"), ""),
      m_mailServerConnectionType(ARG_MAILSERVERCONNECTIONTYPE, tr("Mail server connection type (TLS, TCP, SSL) for the server for sending mails to accounts."), tr("connection"), ""),
      m_mailServerUsername(ARG_MAILSERVERUSERNAME, tr("Username on the mail server for the server for sending mails to accounts."), tr("username"), ""),
      m_mailServerPassword(ARG_MAILSERVERPASSWORD, tr("Password on the mail server for the server for sending mails to accounts. Must be set cause the password is never stored in the game."), tr("password"), ""),
      m_mailServerSendAddress(ARG_MAILSERVERSENDADDRESS, tr("E-Mail address used on the mail server for the server for sending mails to accounts."), tr("address"), ""),
      m_mailServerAuthMethod(ARG_MAILSERVERAUTHMETHOD, tr("Mail server authentication type (Plain, Login) for the server for sending mails to accounts."), tr("method"), "")
{
    m_parser.setApplicationDescription("Commander Wars game");
    m_parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    m_parser.addHelpOption();
    m_parser.addVersionOption();    
    m_parser.addOption(m_debugLevel);
    m_parser.addOption(m_userPath);
    m_parser.addOption(m_aiSlave);
    m_parser.addOption(m_spawnAiProcess);
    m_parser.addOption(m_mods);
    m_parser.addOption(m_update);
    m_parser.addOption(m_slave);
    m_parser.addOption(m_noUi);
    m_parser.addOption(m_noAudio);
    m_parser.addOption(m_iniScript);
    m_parser.addOption(m_createSlaveLogs);
    m_parser.addOption(m_slaveAddress);
    m_parser.addOption(m_secondarySlaveAddress);
    m_parser.addOption(m_slavePort);
    m_parser.addOption(m_masterAddress);
    m_parser.addOption(m_masterPort);
    m_parser.addOption(m_slaveName);
    m_parser.addOption(m_server);
    m_parser.addOption(m_serverSlaveHostOptions);
    m_parser.addOption(m_serverListenAddress);
    m_parser.addOption(m_serverSecondaryListenAddress);
    m_parser.addOption(m_serverListenPort);
    m_parser.addOption(m_serverSlaveListenAddress);
    m_parser.addOption(m_serverSlaveListenPort);
    m_parser.addOption(m_serverSlaveDespawnTime);
    m_parser.addOption(m_mailServerAddress);
    m_parser.addOption(m_mailServerPort);
    m_parser.addOption(m_mailServerConnectionType);
    m_parser.addOption(m_mailServerUsername);
    m_parser.addOption(m_mailServerPassword);
    m_parser.addOption(m_mailServerSendAddress);
    m_parser.addOption(m_mailServerAuthMethod);
}

void CommandLineParser::parseArgsPhaseOne(QCoreApplication & app)
{
    m_parser.process(app);
    Mainapp* pApp = Mainapp::getInstance();
    if (m_parser.isSet(m_createSlaveLogs))
    {
        pApp->setCreateSlaveLogs(true);
    }
    if (m_parser.isSet(m_userPath))
    {
        Settings::setUserPath(m_parser.value(m_userPath));
    }
    if (m_parser.isSet(m_slaveName))
    {
        QString value = m_parser.value(m_slaveName);
        Settings::setSlaveServerName(value);
    }
    if (m_parser.isSet(m_slave))
    {
        pApp->actAsSlave();
    }
    if (m_parser.isSet(m_spawnAiProcess))
    {
        Settings::setSpawnAiProcess(m_parser.value(m_spawnAiProcess) == "1");
    }
    if (m_parser.isSet(m_aiSlave) && !Settings::getSpawnAiProcess())
    {
        Settings::setAiSlave(true);
    }
    if (m_parser.isSet(m_update))
    {
        QString value = m_parser.value(m_update);
        Settings::setUpdateStep(value);
    }
}

bool CommandLineParser::getUserPath(QString & path)
{
    if (m_parser.isSet(m_userPath))
    {
        path = m_parser.value(m_userPath);
        return true;
    }
    else
    {
        return false;
    }
}

void CommandLineParser::parseArgsPhaseTwo()
{
    Mainapp* pApp = Mainapp::getInstance();
    if (m_parser.isSet(m_slaveName))
    {
        QString value = m_parser.value(m_slaveName);
        Settings::setSlaveServerName(value);
    }
    if (m_parser.isSet(m_update))
    {
        QString value = m_parser.value(m_update);
        Settings::setUpdateStep(value);
    }
#ifdef GRAPHICSUPPORT
    if (m_parser.isSet(m_noUi))
    {
        disableUi();
    }
#else
    disableUi();
#endif
    if (m_parser.isSet(m_noAudio))
    {
        pApp->setNoAudio();
    }
    if (m_parser.isSet(m_mods))
    {
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
            CONSOLE_PRINT("Using injected mod list: " + mods, GameConsole::eDEBUG);
            Settings::setActiveMods(modList);
        }
        else
        {
            CONSOLE_PRINT("Using no mods", GameConsole::eDEBUG);
            Settings::setActiveMods(QStringList());
        }
    }
    if (m_parser.isSet(m_iniScript))
    {
        pApp->setInitScript(m_parser.value(m_iniScript));
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
    if (m_parser.isSet(m_serverSecondaryListenAddress))
    {
        Settings::setServerSecondaryListenAdress(m_parser.value(m_serverSecondaryListenAddress));
    }
    if (m_parser.isSet(m_serverListenPort))
    {
        bool ok = false;
        Settings::setServerPort(m_parser.value(m_serverListenPort).toInt(&ok));
    }
    if (m_parser.isSet(m_serverSlaveListenAddress))
    {
        QString value = m_parser.value(m_serverSlaveListenAddress);
        CONSOLE_PRINT("Using slave listen adress " + value, GameConsole::eDEBUG);
        Settings::setSlaveListenAdress(value);
    }
    if (m_parser.isSet(m_serverSlaveListenPort))
    {
        QString value = m_parser.value(m_serverSlaveListenPort);
        CONSOLE_PRINT("Using slave listen port " + value, GameConsole::eDEBUG);
        bool ok = false;
        Settings::setSlaveServerPort(value.toInt(&ok));
    }
    if (m_parser.isSet(m_serverSlaveDespawnTime))
    {
        QString value = m_parser.value(m_serverSlaveDespawnTime);
        CONSOLE_PRINT("Using slave despawn time adress " + value, GameConsole::eDEBUG);
        Settings::setSlaveDespawnTime(std::chrono::seconds(value.toInt()));
    }
    if (m_parser.isSet(m_mailServerAddress))
    {
        Settings::setMailServerAddress(m_parser.value(m_mailServerAddress));
    }
    if (m_parser.isSet(m_mailServerPort))
    {
        bool ok = false;
        Settings::setMailServerPort(m_parser.value(m_mailServerPort).toInt(&ok));
    }
    if (m_parser.isSet(m_mailServerConnectionType))
    {
        auto type = m_parser.value(m_mailServerConnectionType);
        SmtpClient::ConnectionType value = SmtpClient::TlsConnection;
        if (type == "TLS")
        {
            value = SmtpClient::TlsConnection;
        }
        else if (type == "SSL")
        {
            value = SmtpClient::SslConnection;
        }
        else if (type == "TCP")
        {
            value = SmtpClient::TcpConnection;
        }
        Settings::setMailServerAuthMethod(static_cast<qint32>(value));
    }
    if (m_parser.isSet(m_mailServerUsername))
    {
        Settings::setMailServerUsername(m_parser.value(m_mailServerUsername));
    }
    if (m_parser.isSet(m_mailServerPassword))
    {
        Settings::setMailServerPassword(m_parser.value(m_mailServerPassword));
    }
    if (m_parser.isSet(m_mailServerSendAddress))
    {
        Settings::setMailServerSendAddress(m_parser.value(m_mailServerSendAddress));
    }
    if (m_parser.isSet(m_mailServerAuthMethod))
    {
        auto type = m_parser.value(m_mailServerAuthMethod);
        SmtpClient::AuthMethod value = SmtpClient::AuthLogin;
        if (type == "Login")
        {
            value = SmtpClient::AuthLogin;
        }
        else if (type == "Plain")
        {
            value = SmtpClient::AuthPlain;
        }
        Settings::setMailServerAuthMethod(static_cast<qint32>(value));
    }
    if (m_parser.isSet(m_debugLevel))
    {
        QString level = m_parser.value(m_debugLevel);
        GameConsole::setLogLevel(static_cast<GameConsole::eLogLevels>(level.toInt()));
    }
}

void CommandLineParser::disableUi()
{
    CONSOLE_PRINT("Running without ui", GameConsole::eDEBUG);
    AnimationSkipper::disableAllAnimations();
    Mainapp* pApp = Mainapp::getInstance();
    pApp->setNoUi();
}

void CommandLineParser::startSlaveGame() const
{
    QString slaveAddress;
    QString secondarySlaveAddress;
    quint16 slavePort = 0;
    if (m_parser.isSet(m_slaveAddress) && m_parser.isSet(m_slavePort))
    {
        bool ok = false;
        slaveAddress = m_parser.value(m_slaveAddress);
        if (m_parser.isSet(m_secondarySlaveAddress))
        {
            secondarySlaveAddress = m_parser.value(m_secondarySlaveAddress);
        }
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
    CONSOLE_PRINT("Slave address " + slaveAddress + " port " + QString::number(slavePort) + " secondary slave address " + secondarySlaveAddress +
                  " master address " + masterAddress + " port " + QString::number(masterPort), GameConsole::eDEBUG);
    if (!slaveAddress.isEmpty() && masterPort > 0 && slavePort > 0 && !masterAddress.isEmpty())
    {
        MainServer::initDatabase();
        // init multiplayer menu
        spTCPServer pServer = spTCPServer::create(nullptr);
        pServer->moveToThread(Mainapp::getInstance()->getNetworkThread());
        spMultiplayermenu pMenu = spMultiplayermenu::create(pServer, "", Multiplayermenu::NetworkMode::Host);
        pMenu->connectNetworkSlots();
        oxygine::Stage::getStage()->addChild(pMenu);
        emit pServer->sig_connect(slaveAddress, slavePort, secondarySlaveAddress);
        // connect to server
        spTCPClient pSlaveMasterConnection = Mainapp::getSlaveClient();
        connect(pSlaveMasterConnection.get(), &TCPClient::sigConnected, pMenu.get(), &Multiplayermenu::onSlaveConnectedToMaster, Qt::QueuedConnection);
        connect(pSlaveMasterConnection.get(), &TCPClient::recieveData, pMenu.get(), &Multiplayermenu::recieveServerData, Qt::QueuedConnection);
        emit pSlaveMasterConnection->sig_connect(masterAddress, masterPort, "");
    }
    else
    {
        CONSOLE_PRINT("Despawning game cause slave game configuration is invalid", GameConsole::eDEBUG);
        QCoreApplication::exit(-3);
    }
}
