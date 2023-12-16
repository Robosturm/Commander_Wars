#include "network/replayrecordfileserver.h"

class MainServer;

ReplayRecordFileserver::ReplayRecordFileserver(MainServer *parent)
    : m_mainServer(parent)
{

}

void ReplayRecordFileserver::onSlaveInfoGameResult(quint64 socketID, const QJsonObject &objData)
{

}
