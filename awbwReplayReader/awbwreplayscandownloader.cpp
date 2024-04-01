#include "awbwReplayReader/awbwreplayscandownloader.h"
#include "coreengine/gameconsole.h"

#include <QDomDocument>

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
    QDomDocument document;
    bool loaded = document.setContent(result);
    if (loaded)
    {
        auto rootElement = document.documentElement();
        auto node = rootElement.firstChild();
        scanNode(node);
    }
    else
    {
        CONSOLE_PRINT("Unable to parse received scan content", GameConsole::eERROR);
    }
    pReply->deleteLater();
}

void AwbwReplayScanDownloader::scanNode(QDomNode node)
{
    while (!node.isNull())
    {
        while (node.isComment())
        {
            node = node.nextSibling();
        }
        if (!node.isNull())
        {
            scanNode(node);
        }
        node = node.nextSibling();
    }
}
