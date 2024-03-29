#include <QUrlQuery>

#include "awbwReplayReader/awbwreplaydownloader.h"

AwbwReplayDownloader::AwbwReplayDownloader(QObject *parent)
    : QObject{parent}
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

void AwbwReplayDownloader::downLoadReplay(const QString & userName, const QString & password, const QString & replay)
{
    login(userName, password);
    downLoadReplay(replay);
}

void AwbwReplayDownloader::downLoadReplay(const QString & replay)
{
    m_file.setFileName("data/records/" + replay + ".zip");
    m_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QString urlReuqest = "https://awbw.amarriner.com/replay_download.php?games_id={" + replay + "}";
    QUrl requestUrl(urlReuqest);
    QNetworkRequest request(requestUrl);
    m_reply = m_webCtrl.get(request);
    connect(m_reply, &QNetworkReply::downloadProgress, this, &AwbwReplayDownloader::downloadProgress);
    connect(m_reply, &QNetworkReply::errorOccurred, this, &AwbwReplayDownloader::downloadErrorOccurred, Qt::QueuedConnection);
}

void AwbwReplayDownloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0 && bytesReceived > 0)
    {
        auto data = m_reply->readAll();
        m_file.write(data);
        emit sigNewProgress(bytesReceived, bytesTotal);
    }
}

void AwbwReplayDownloader::onResponseFinished(QNetworkReply* pReply)
{
    auto result = pReply->readAll();
    if (result == "1")
    {
        emit sigLogin(true);
    }
    else if (result == "0")
    {
        emit sigLogin(false);
    }
    else
    {

    }
    pReply->deleteLater();
}
