#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class QDomNode;

class AwbwReplayScanDownloader : public QObject
{
    Q_OBJECT
public:
    explicit AwbwReplayScanDownloader(QObject *parent = nullptr);

    Q_INVOKABLE void scan(qint32 stdRankPoints, qint32 fogRankPoints);
signals:

private slots:
    void onResponseFinished(QNetworkReply* pReply);
private:
    void scanNode(QDomNode node);
private:
    QNetworkAccessManager m_webCtrl;
    QNetworkReply* m_reply{nullptr};
    qint32 m_itemCount{-1};
};

Q_DECLARE_INTERFACE(AwbwReplayScanDownloader, "AwbwReplayScanDownloader");
