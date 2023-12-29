#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QFile>

class FileDownloader final : public QObject
{
    Q_OBJECT
public:
    enum class State
    {
        SameVersion,
        NewVersion,
        DownloadingNewVersion,
        DownloadingFailed,
        DownloadingFinished,
    };

    FileDownloader(const QString & targetFile,
                   const QString & baseUrl,
                   const QString & resolveEnd,
                   const QString & currentTag,
                   const QString & downloadFile);
    virtual ~FileDownloader() = default;

signals:
    void sigNewProgress(qint64 bytesReceived, qint64 bytesTotal);
    void sigNewState(State state);
public slots:
    void startDownloading();
private slots:
    void onResponseFinished(QNetworkReply* pReply);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void errorOccurred(QNetworkReply::NetworkError code);
private:
    void downloadFile(const QUrl & fileUrl);
private:
    QNetworkAccessManager m_webCtrl;
    QNetworkReply* m_reply{nullptr};
    QFile m_file;
    QString m_targetFile;
    QString m_baseUrl;
    QString m_resolveEnd;
    QString m_currentTag;
    QString m_latestTag;
    QUrl m_requestUrl;

    bool m_downloading{false};
    bool m_downloadFailed{false};
};

