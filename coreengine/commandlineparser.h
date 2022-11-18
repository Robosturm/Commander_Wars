#pragma once

#include <QCommandLineParser>
#include <QObject>
#include <QCoreApplication>

class CommandLineParser final : public QObject
{
    Q_OBJECT
public:
    static const char* const ARG_MODS;
    static const char* const ARG_SLAVE;
    static const char* const ARG_SLAVENAME;
    static const char* const ARG_NOUI;
    static const char* const ARG_NOAUDIO;
    static const char* const ARG_INITSCRIPT;
    static const char* const ARG_CREATESLAVELOGS;
    static const char* const ARG_SLAVEADDRESS;
    static const char* const ARG_SLAVESECONDARYADDRESS;
    static const char* const ARG_SLAVEPORT;
    static const char* const ARG_MASTERADDRESS;
    static const char* const ARG_MASTERPORT;
    static const char* const ARG_UPDATE;
    static const char* const ARG_SPAWNAIPROCESS;
    static const char* const ARG_AISLAVE;

    static const char* const ARG_SERVER;
    static const char* const ARG_SERVERSLAVEHOSTOPTIONS;
    static const char* const ARG_SERVERLISTENADDRESS;
    static const char* const ARG_SERVERSECONDARYLISTENADDRESS;
    static const char* const ARG_SERVERLISTENPORT;
    static const char* const ARG_SERVERSLAVELISTENADDRESS;
    static const char* const ARG_SERVERSLAVELISTENPORT;
    static const char* const ARG_SERVERSLAVEDESPAWNTIME;

    static const char* const ARG_MAILSERVERADDRESS;
    static const char* const ARG_MAILSERVERPORT;
    static const char* const ARG_MAILSERVERCONNECTIONTYPE;
    static const char* const ARG_MAILSERVERUSERNAME;
    static const char* const ARG_MAILSERVERPASSWORD;
    static const char* const ARG_MAILSERVERSENDADDRESS;
    static const char* const ARG_MAILSERVERAUTHMETHOD;

    CommandLineParser();

    void parseArgsPhaseOne(QCoreApplication & app);
    void parseArgsPhaseTwo();

    void startSlaveGame() const;
private:
    void disableUi();
private:
    QCommandLineOption m_aiSlave;
    QCommandLineOption m_spawnAiProcess;
    QCommandLineOption m_mods;
    QCommandLineOption m_update;
    QCommandLineOption m_slave;
    QCommandLineOption m_noUi;
    QCommandLineOption m_noAudio;
    QCommandLineOption m_iniScript;
    QCommandLineOption m_createSlaveLogs;
    QCommandLineOption m_slaveAddress;
    QCommandLineOption m_secondarySlaveAddress;
    QCommandLineOption m_slavePort;
    QCommandLineOption m_masterAddress;
    QCommandLineOption m_masterPort;
    QCommandLineOption m_slaveName;
    QCommandLineOption m_server;
    QCommandLineOption m_serverSlaveHostOptions;
    QCommandLineOption m_serverListenAddress;
    QCommandLineOption m_serverSecondaryListenAddress;
    QCommandLineOption m_serverListenPort;
    QCommandLineOption m_serverSlaveListenAddress;
    QCommandLineOption m_serverSlaveListenPort;
    QCommandLineOption m_serverSlaveDespawnTime;
    QCommandLineOption m_mailServerAddress;
    QCommandLineOption m_mailServerPort;
    QCommandLineOption m_mailServerConnectionType;
    QCommandLineOption m_mailServerUsername;
    QCommandLineOption m_mailServerPassword;
    QCommandLineOption m_mailServerSendAddress;
    QCommandLineOption m_mailServerAuthMethod;

    QCommandLineParser m_parser;
};

