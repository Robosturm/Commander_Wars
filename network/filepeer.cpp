#include "network/filepeer.h"

FilePeer::FilePeer(MainServer* pOwner, const QString & filePath, qint64 socketId)
    : m_pOwner{pOwner},
    m_file{filePath},
    m_connectSocket{socketId}
{
}


void FilePeer::startUpload()
{
    m_file.open(QIODevice::ReadOnly);

}

void FilePeer::startDownload()
{
    m_file.remove();
    m_file.open(QIODevice::WriteOnly);

}
