#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class AwbwReplayDownloader : public QObject
{
    Q_OBJECT
public:
    explicit AwbwReplayDownloader(QObject *parent = nullptr);

    void login(const QString & userName, const QString & password);
    void downLoadReplay(const QString & userName, const QString & password, const QString & replay);
    void downLoadReplay(const QString & replay);

signals:
    void sigDownloadResult(bool success);
    void sigLogin(bool success);

private slots:
    void onResponseFinished(QNetworkReply* pReply);
    void loginErrorOccurred(QNetworkReply::NetworkError code);
    void downloadErrorOccurred(QNetworkReply::NetworkError code);
private:
    QNetworkAccessManager m_webCtrl;
    QNetworkReply* m_reply{nullptr};
};
