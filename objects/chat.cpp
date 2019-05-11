#include "chat.h"

Chat::Chat(NetworkInterface* pInterface)
    : QObject(),
      m_pInterface(pInterface)
{

    connect(m_pInterface, &NetworkInterface::recieveData, this, &Chat::dataRecieved, Qt::QueuedConnection);
}

void Chat::dataRecieved(QByteArray data, Mainapp::NetworkSerives service)
{
    if (service == Mainapp::NetworkSerives::Chat)
    {
        QString message(data);
        messages.append(message);
        if (messages.size() > bufferSize)
        {
            messages.pop_front();
        }
    }
}
