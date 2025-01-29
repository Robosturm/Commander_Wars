#pragma once

#include "objects/dialogs/customdialog.h"
#include "awbwReplayReader/awbwreplaydownloader.h"

class Basemenu;
class DialogAwbwRecordDownloader;
using spDialogAwbwRecordDownloader = std::shared_ptr<DialogAwbwRecordDownloader>;

class DialogAwbwRecordDownloader : public CustomDialog
{
    Q_OBJECT
public:
    DialogAwbwRecordDownloader(Basemenu* pBaseMenu);
    ~DialogAwbwRecordDownloader();

    Q_INVOKABLE void downloadRecord();
    Q_INVOKABLE QString getUserName() const;
    Q_INVOKABLE void setUserName(const QString & newUserName);

    Q_INVOKABLE QString getPassword() const;
    Q_INVOKABLE void setPassword(const QString & newPassword);

    Q_INVOKABLE QString getReplay() const;
    Q_INVOKABLE void setReplay(const QString & newReplay);

private slots:
    void loginResult(bool success);
    void downloadResult(bool success);
    void onNewProgress(qint64 bytesReceived, qint64 bytesTotal);
private:
    AwbwReplayDownloader m_awbwreplaydownloader{Settings::getInstance()->getUserPath() + "data/records/", this};
    QString m_userName;
    QString m_password;
    QString m_replay;
};

Q_DECLARE_INTERFACE(DialogAwbwRecordDownloader, "DialogAwbwRecordDownloader");
