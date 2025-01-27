#include "updater/filedownloader.h"

#include "coreengine/gameconsole.h"

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
    m_currentTag.replace("refs/tags/", "");
    connect(&m_webCtrl, &QNetworkAccessManager::finished, this, &FileDownloader::onResponseFinished);

    m_requestUrl = QUrl(m_baseUrl + m_resolveEnd);
    QNetworkRequest request(m_requestUrl);
    m_reply = m_webCtrl.get(request);
    connect(m_reply, &QNetworkReply::errorOccurred, this, &FileDownloader::errorOccurred);
}

void FileDownloader::onResponseFinished(QNetworkReply* pReply)
{
    if (!m_downloading && !m_downloadFailed)
    {
        m_downloading = true;
        QUrl url = m_reply->url();
        m_latestTag = url.toString();
        m_latestTag = m_latestTag.replace(m_baseUrl + "tag/", "");
        GameConsole::print("Current " + m_currentTag + " latest version tag " + m_latestTag, GameConsole::eDEBUG);
        if (m_latestTag != m_currentTag)
        {
            emit sigNewState(State::NewVersion);
        }
        else
        {
            emit sigNewState(State::SameVersion);
        }
    }
    else
    {
        if (!m_downloadFailed)
        {
            m_file.write(pReply->readAll());
            m_file.close();
            emit sigNewState(State::DownloadingFinished);
        }
        else
        {
            emit sigNewState(State::DownloadingFailed);
        }
    }
    pReply->deleteLater();
}

void FileDownloader::startDownloading()
{
    QString targetFile = m_baseUrl + "download/" + m_latestTag + "/" + m_targetFile + "-" + m_latestTag + ".zip";
    GameConsole::print("Starting download of " + targetFile, GameConsole::eINFO);
    QUrl targetUrl(targetFile);
    downloadFile(targetUrl);
    emit sigNewState(State::DownloadingNewVersion);
}

void FileDownloader::downloadFile(const QUrl & fileUrl)
{
    m_requestUrl = fileUrl;
    m_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QNetworkRequest request(m_requestUrl);
    m_reply = m_webCtrl.get(request);
    connect(m_reply, &QNetworkReply::downloadProgress, this, &FileDownloader::downloadProgress);
    connect(m_reply, &QNetworkReply::errorOccurred, this, &FileDownloader::errorOccurred);
}

QString FileDownloader::getLatestTag() const
{
    return m_latestTag;
}

void FileDownloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal < 0 && bytesReceived < 0)
    {
        m_downloadFailed = true;
    }
    else
    {
        m_file.write(m_reply->readAll());
        emit sigNewProgress(bytesReceived, bytesTotal);
    }
}

void FileDownloader::errorOccurred(QNetworkReply::NetworkError code)
{
    m_downloadFailed = true;
    emit sigNewState(State::DownloadingFailed);
}
