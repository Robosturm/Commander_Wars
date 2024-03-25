#include <QUrlQuery>

#include "awbwReplayReader/awbwreplaydownloader.h"

static const char* const LOGIN_REQUEST = "https://awbw.amarriner.com/logincheck.php";
static const char* const DOWNLOAD_REQUEST = "https://awbw.amarriner.com/game.php?games_id=";

AwbwReplayDownloader::AwbwReplayDownloader(QObject *parent)
    : QObject{parent}
{
    connect(&m_webCtrl, &QNetworkAccessManager::finished, this, &AwbwReplayDownloader::onResponseFinished, Qt::QueuedConnection);
}

void AwbwReplayDownloader::login(const QString & userName, const QString & password)
{
    QUrl requestUrl(LOGIN_REQUEST);
    QUrlQuery query;

    query.addQueryItem("username", userName);
    query.addQueryItem("password", password);

    requestUrl.setQuery(query.query());
    QNetworkRequest request(requestUrl);
    m_reply = m_webCtrl.get(request);
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
    QString urlReuqest = replay;
    if (!urlReuqest.startsWith(DOWNLOAD_REQUEST))
    {
        urlReuqest = DOWNLOAD_REQUEST + replay;
    }
    QUrl requestUrl(urlReuqest);
    QNetworkRequest request(requestUrl);
    m_reply = m_webCtrl.get(request);
    connect(m_reply, &QNetworkReply::errorOccurred, this, &AwbwReplayDownloader::downloadErrorOccurred, Qt::QueuedConnection);
}

void AwbwReplayDownloader::onResponseFinished(QNetworkReply* pReply)
{

}
