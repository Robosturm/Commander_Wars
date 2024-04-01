#include "awbwReplayReader/awbwreplayscandownloader.h"

#include <QFile>

AwbwReplayScanDownloader::AwbwReplayScanDownloader(QObject *parent)
    : QObject{parent}
{
    connect(&m_webCtrl, &QNetworkAccessManager::finished, this, &AwbwReplayScanDownloader::onResponseFinished, Qt::QueuedConnection);
}

void AwbwReplayScanDownloader::scan(qint32 stdRankPoints, qint32 fogRankPoints)
{

    QString urlRequestStart = "https://awbw.amarriner.com/gamescompleted.php?start=";
    QString urlEnd =   "&league=Y";
    QUrl requestUrl(urlRequestStart + QString::number(1) + urlEnd);
    QNetworkRequest request(requestUrl);
    m_reply = m_webCtrl.get(request);
}

void AwbwReplayScanDownloader::onResponseFinished(QNetworkReply* pReply)
{
    auto result = pReply->readAll();
    QFile file("test.xml");
    file.open(QIODevice::WriteOnly);
    file.write(result);
    pReply->deleteLater();
}
