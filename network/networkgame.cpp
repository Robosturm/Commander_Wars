#include "networkgame.h"

NetworkGame::NetworkGame()
    : QObject()
{

}

void NetworkGame::disconnected(quint64 socketID)
{
    for (qint32 i = 0; i < m_pClientIDs.size(); i++)
    {
        if (m_pClientIDs[i] == socketID)
        {
            m_pClientIDs.removeAt(i);
            m_pClients.removeAt(i);
            // host left stop game
            if (m_HostId == socketID)
            {

            }
            // todo inform other clients of disconnection
            break;
        }
    }
}

void NetworkGame::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service, quint64 targetSocket)
{
    switch (service)
    {
        case NetworkInterface::NetworkSerives::Game:
        case NetworkInterface::NetworkSerives::GameChat:
        case NetworkInterface::NetworkSerives::Multiplayer:
        {
            for (qint64 i = 0; i < m_pClientIDs.size(); i++)
            {
                if (m_pClientIDs[i] != socketID &&
                    (targetSocket == 0 || targetSocket == m_pClientIDs[i]))
                {
                    emit m_pClients[i]->sig_sendData(0, data, service, false);
                }
            }
            break;
        }
        case NetworkInterface::NetworkSerives::Lobby:
        case NetworkInterface::NetworkSerives::LobbyChat:
        case NetworkInterface::NetworkSerives::None:
        case NetworkInterface::NetworkSerives::Max:
        {
            // do nothing
            break;
        }
    }
}
