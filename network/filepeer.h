#pragma once

#include <QObject>
#include <QFile>
class MainServer;

class FilePeer : public QObject
{
    Q_OBJECT
public:
    explicit FilePeer(MainServer* pOwner, const QString & filePath, qint64 socketId);

    void startUpload();
    void startDownload();
signals:

public slots:
    //void onRequestNextData();

private:
    MainServer* m_pOwner;
    QFile m_file;
    QDataStream m_fileStream{&m_file};
    qint64 m_connectSocket;
};

