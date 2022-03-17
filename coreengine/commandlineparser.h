#pragma once

#include <QCommandLineParser>
#include <QObject>
#include <QApplication>

class CommandLineParser : public QObject
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
    static const char* const ARG_SLAVEPORT;
    static const char* const ARG_MASTERADDRESS;
    static const char* const ARG_MASTERPORT;

    static const char* const ARG_SERVER;
    static const char* const ARG_SERVERSLAVEHOSTOPTIONS;
    static const char* const ARG_SERVERLISTENADDRESS;
    static const char* const ARG_SERVERLISTENPORT;
    static const char* const ARG_SERVERSLAVELISTENADDRESS;
    static const char* const ARG_SERVERSLAVELISTENPORT;

    CommandLineParser();

    void parseArgs(QApplication & app);
    void startSlaveGame() const;
private:
    QCommandLineOption m_mods;
    QCommandLineOption m_slave;
    QCommandLineOption m_noUi;
    QCommandLineOption m_noAudio;
    QCommandLineOption m_iniScript;
    QCommandLineOption m_createSlaveLogs;
    QCommandLineOption m_slaveAddress;
    QCommandLineOption m_slavePort;
    QCommandLineOption m_masterAddress;
    QCommandLineOption m_masterPort;
    QCommandLineOption m_slaveName;
    QCommandLineOption m_server;
    QCommandLineOption m_serverSlaveHostOptions;
    QCommandLineOption m_serverListenAddress;
    QCommandLineOption m_serverListenPort;
    QCommandLineOption m_serverSlaveListenAddress;
    QCommandLineOption m_serverSlaveListenPort;
    QCommandLineParser m_parser;
};

