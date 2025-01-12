#pragma once

#include <QObject>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "coreengine/settings.h"

class AwbwReplayDownloader : public QObject
{
    Q_OBJECT
public:
    explicit AwbwReplayDownloader(const QString & downloadPath = Settings::userPath() + "data/records/", QObject *parent = nullptr);

    void login(const QString & userName, const QString & password);
    void downLoadReplay(const QString & userName, const QString & password, const QString replay);
    bool downLoadReplay(const QString & replay);

signals:
    void sigDownloadResult(bool success);
    void sigLogin(bool success);
    void sigNewProgress(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    void onResponseFinished(QNetworkReply* pReply);
    void loginErrorOccurred(QNetworkReply::NetworkError code);
    void downloadErrorOccurred(QNetworkReply::NetworkError code);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
private:
    QNetworkAccessManager m_webCtrl;
    QNetworkReply* m_reply{nullptr};
    QFile m_file;
    QVariant m_cookies;
    QString m_downloadPath;
    bool m_downloading{false};
    bool m_downloadFailed{false};
};

Q_DECLARE_INTERFACE(AwbwReplayDownloader, "AwbwReplayDownloader");
