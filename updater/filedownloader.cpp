#include "updater/filedownloader.h"

FileDownloader::FileDownloader(const QString & targetFile,
                               const QString & baseUrl,
                               const QString & resolveEnd,
                               const QString & currentTag,
                               const QString & downloadFile)
    : m_webCtrl(this),
      m_file(downloadFile),
      m_targetFile(targetFile),
      m_baseUrl(baseUrl),
      m_resolveEnd(resolveEnd),
      m_currentTag(currentTag)
{
    connect(&m_webCtrl, QNetworkAccessManager::finished, this, FileDownloader::onResponseFinished);

    m_requestUrl = QUrl(m_baseUrl + m_resolveEnd);
    QNetworkRequest request(m_requestUrl);
    m_reply = m_webCtrl.get(request);
    connect(m_reply, QNetworkReply::errorOccurred, this, FileDownloader::errorOccurred);
}

void FileDownloader::onResponseFinished(QNetworkReply* pReply)
{
    if (!m_downloading)
    {
        m_downloading = true;
        QUrl url = m_reply->url();
        QString latestTag = url.toString();
        latestTag = latestTag.replace(m_baseUrl + "tag/", "");
        if (latestTag != m_currentTag)
        {
            QUrl targetUrl(m_baseUrl + "download/" + latestTag + "/" + m_targetFile);
            downloadFile(targetUrl);
            emit sigNewState(State::DownloadingNewVersion);
        }
        else
        {
            emit sigNewState(State::SameVersion);
        }
    }
    else
    {
        m_file.write(pReply->readAll());
        m_file.close();
        emit sigNewState(State::DownloadingFinished);
    }
    pReply->deleteLater();
}

void FileDownloader::downloadFile(const QUrl & fileUrl)
{
    m_requestUrl = fileUrl;
    m_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QNetworkRequest request(m_requestUrl);
    m_reply = m_webCtrl.get(request);
    connect(m_reply, QNetworkReply::downloadProgress, this, FileDownloader::downloadProgress);
    connect(m_reply, QNetworkReply::errorOccurred, this, FileDownloader::errorOccurred);
}

void FileDownloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_file.write(m_reply->readAll());
    emit sigNewProgress(bytesReceived, bytesTotal);
}

void FileDownloader::errorOccurred(QNetworkReply::NetworkError code)
{
    emit sigNewState(State::DownloadingFailed);
}
