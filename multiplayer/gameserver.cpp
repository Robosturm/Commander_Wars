#include "gameserver.h"

GameServer* GameServer::m_pInstance = nullptr;

GameServer::GameServer()
    : QObject()
{

}

GameServer* GameServer::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new GameServer();
    }
    return m_pInstance;
}
