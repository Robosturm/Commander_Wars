#include "network/automatchmaker.h"
#include "network/mainserver.h"

AutoMatchMaker::AutoMatchMaker(const QString & matchName, MainServer & mainServer)
    : m_matchName(matchName),
      m_mainServer(mainServer)
{

}

QString AutoMatchMaker::getMatchName() const
{
    return m_matchName;
}

void AutoMatchMaker::onNewMatchResultData(const QJsonObject & objData)
{

}
