#include <QUrlQuery>

#include "awbwReplayReader/awbwreplaydownloader.h"

AwbwReplayDownloader::AwbwReplayDownloader(const QString & downloadPath, QObject *parent)
    : QObject{parent},
    m_downloadPath(downloadPath)
{
    connect(&m_webCtrl, &QNetworkAccessManager::finished, this, &AwbwReplayDownloader::onResponseFinished, Qt::QueuedConnection);
}

void AwbwReplayDownloader::login(const QString & userName, const QString & password)
{
    QUrl requestUrl("https://awbw.amarriner.com/logincheck.php");
    QNetworkRequest request(requestUrl);

    QUrlQuery query;
    query.addQueryItem("username", userName);
    query.addQueryItem("password", password);
    QString data = query.toString();
    m_reply = m_webCtrl.post(request, data.toUtf8());
    connect(m_reply, &QNetworkReply::errorOccurred, this, &AwbwReplayDownloader::loginErrorOccurred, Qt::QueuedConnection);
}

void AwbwReplayDownloader::loginErrorOccurred(QNetworkReply::NetworkError code)
{
    emit sigLogin(false);
}

void AwbwReplayDownloader::downloadErrorOccurred(QNetworkReply::NetworkError code)
{
    emit sigDownloadResult(false);
}

void AwbwReplayDownloader::downLoadReplay(const QString & userName, const QString & password, const QString replay)
{
    connect(this, &AwbwReplayDownloader::sigLogin, this, [this, replay](bool success)
    {
        if (success)
        {
            downLoadReplay(replay);
        }
        else
        {
            emit sigDownloadResult(false);
        }
    });
    login(userName, password);
}

bool AwbwReplayDownloader::downLoadReplay(const QString & replay)
{
    bool ret = false;
    if (!m_cookies.isNull())
    {
        m_file.setFileName(m_downloadPath + replay + ".zip");
        m_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QString urlRequest = "https://awbw.amarriner.com/replay_download.php?games_id=" + replay;
        QUrl requestUrl(urlRequest);
        QNetworkRequest request(requestUrl);
        request.setHeader(QNetworkRequest::CookieHeader, m_cookies);
        m_downloading = true;
        ret = true;
        m_reply = m_webCtrl.get(request);
        connect(m_reply, &QNetworkReply::downloadProgress, this, &AwbwReplayDownloader::downloadProgress);
        connect(m_reply, &QNetworkReply::errorOccurred, this, &AwbwReplayDownloader::downloadErrorOccurred, Qt::QueuedConnection);
    }
    return ret;
}

void AwbwReplayDownloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0 && bytesReceived > 0)
    {
        auto data = m_reply->readAll();
        m_file.write(data);
        emit sigNewProgress(bytesReceived, bytesTotal);
    }
    else
    {
        m_downloadFailed = true;
    }
}

void AwbwReplayDownloader::onResponseFinished(QNetworkReply* pReply)
{
    auto result = pReply->readAll();
    if (!m_downloading && result == "1")
    {
        m_cookies = pReply->header(QNetworkRequest::SetCookieHeader);
        emit sigLogin(true);
    }
    else if (!m_downloading && result == "0")
    {
        emit sigLogin(false);
    }
    else if (m_downloading)
    {
        if (!m_downloadFailed)
        {
            m_file.write(pReply->readAll());
            m_file.close();
            emit sigDownloadResult(true);
        }
        else
        {
            m_file.remove();
            emit sigDownloadResult(false);
        }
    }
    pReply->deleteLater();
}
