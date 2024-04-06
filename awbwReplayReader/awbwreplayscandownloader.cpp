#include "awbwReplayReader/awbwreplayscandownloader.h"
#include "coreengine/gameconsole.h"

#include <QDomDocument>
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
    const char* const MAP_START = "<a class=\"anchor\" name=\"game_";
    QString result(pReply->readAll());
    auto items = result.split("\n");
    for (qint32 i = 0; i < items.size(); ++i)
    {
        auto & item = items[i];
        if (m_itemCount < 0)
        {
            if (item.contains("gamescompleted.php?start=") &&
                item.startsWith("<span class=small_text style=\"float:right;\">"))
            {
                auto subItems = item.split("gamescompleted.php?start=");
                m_itemCount = subItems[subItems.length() - 1].split("&league=Y\">")[0].toInt();
            }
        }
        else if (item.startsWith(MAP_START))
        {
            qint32 recordId = item.split(MAP_START)[1].split("\">")[0].toInt();
            qint32 player1Points = -1;
            qint32 player2Points = -1;
            for (; i < items.size() && (player1Points < 0 || player2Points < 0); ++i)
            {

            }
        }
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
