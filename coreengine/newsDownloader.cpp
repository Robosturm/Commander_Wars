#include "coreengine/newsDownloader.h"
#include "coreengine/settings.h"
#include "coreengine/gameconsole.h"

NewsDownloader::NewsDownloader(QObject *parent)
    : QObject(parent),
    m_webCtrl(this)
{
    connect(&m_webCtrl, &QNetworkAccessManager::finished, this, &NewsDownloader::onResponseFinished, Qt::QueuedConnection);
}

void NewsDownloader::startDownloadNews()
{
    if (!Settings::getInstance()->getNewsDownloaded())
    {
        auto url = Settings::getInstance()->getNewsUrl();
        QUrl requestUrl("https://raw.githubusercontent.com/Robosturm/Commander_Wars/master/news.json");
        QNetworkRequest request(requestUrl);
        m_reply = m_webCtrl.get(request);
        connect(m_reply, &QNetworkReply::downloadProgress, this, &NewsDownloader::downloadProgress);
        connect(m_reply, &QNetworkReply::errorOccurred, this, &NewsDownloader::downloadErrorOccurred, Qt::QueuedConnection);
        m_downloadingNews = true;        
    }
}

void NewsDownloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0 && bytesReceived > 0)
    {
        m_newsData.append(m_reply->readAll());
    }
}

void NewsDownloader::downloadErrorOccurred(QNetworkReply::NetworkError code)
{
    CONSOLE_PRINT("NewsDownloader::downloadErrorOccurred error: " + QString::number(code), GameConsole::eERROR);
}

void NewsDownloader::onResponseFinished(QNetworkReply* pReply)
{
    auto result = pReply->readAll();
    if (m_downloadingNews && pReply->error() == QNetworkReply::NoError)
    {
        m_downloadingNews = false;
        m_newsData.append(result);
        QString newsHash = QString::fromUtf8(QCryptographicHash::hash(m_newsData, QCryptographicHash::Sha256).toHex());
        bool newNews = false;
        if (newsHash != Settings::getInstance()->getLastNewsHash())
        {
            Settings::getInstance()->setLastNewsHash(newsHash);
            newNews = true;
        }
        Settings::getInstance()->setLastNews(QString::fromUtf8(m_newsData));
        Settings::getInstance()->setNewsDownloaded(true);
        emit sigNewsDownloaded(newNews);
    }
    else
    {
        CONSOLE_PRINT("NewsDownloader::onResponseFinished error: " + pReply->errorString(), GameConsole::eERROR);
    }
    pReply->deleteLater();
}