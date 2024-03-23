#include "network/sslserver.h"
#include "coreengine/gameconsole.h"

SslServer::SslServer(QObject *parent)
    : QSslServer{parent}
{}

void SslServer::incomingConnection(qintptr socket)
{
    CONSOLE_PRINT(QString("SslServer::incomingConnection as socket: ") + QString::number(socket), GameConsole::eDEBUG);
    QSslServer::incomingConnection(socket);
}
