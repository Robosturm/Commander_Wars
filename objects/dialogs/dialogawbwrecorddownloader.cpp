#include "objects/dialogs/dialogawbwrecorddownloader.h"
#include "objects/dialogs/dialogmessagebox.h"
#include "objects/loadingscreen.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

DialogAwbwRecordDownloader::DialogAwbwRecordDownloader(Basemenu* pBaseMenu)
    : CustomDialog("", "", pBaseMenu, tr("Close"))
{
    m_uiXml = "ui/menu/awbwRecordDownloader.xml";
    loadXmlFile(m_uiXml);

    connect(&m_awbwreplaydownloader, &AwbwReplayDownloader::sigLogin, this, &DialogAwbwRecordDownloader::loginResult, Qt::QueuedConnection);
    connect(&m_awbwreplaydownloader, &AwbwReplayDownloader::sigDownloadResult, this, &DialogAwbwRecordDownloader::downloadResult, Qt::QueuedConnection);
    connect(&m_awbwreplaydownloader, &AwbwReplayDownloader::sigNewProgress, this, &DialogAwbwRecordDownloader::onNewProgress, Qt::QueuedConnection);
}

DialogAwbwRecordDownloader::~DialogAwbwRecordDownloader()
{
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->hide();
}

void DialogAwbwRecordDownloader::downloadRecord()
{
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->show();
    pLoadingScreen->setWorktext(tr("Start downloading record..."));
    m_awbwreplaydownloader.downLoadReplay(m_userName, m_password, m_replay);
}

void DialogAwbwRecordDownloader::loginResult(bool success)
{
    if (!success)
    {
        spDialogMessageBox pExit = MemoryManagement::create<DialogMessageBox>(tr("Failed to login onto Advance Wars by Web!"));
        oxygine::Stage::getStage()->addChild(pExit);
        exit();
    }
}

void DialogAwbwRecordDownloader::downloadResult(bool success)
{
    if (success)
    {
        spDialogMessageBox pExit = MemoryManagement::create<DialogMessageBox>(tr("Successfully downloaded record from Advance Wars by Web!"));
        oxygine::Stage::getStage()->addChild(pExit);
    }
    else
    {
        spDialogMessageBox pExit = MemoryManagement::create<DialogMessageBox>(tr("Failed to download record from Advance Wars by Web!"));
        oxygine::Stage::getStage()->addChild(pExit);
    }
    exit();
}

void DialogAwbwRecordDownloader::onNewProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    constexpr qint64 KB_DIVIDER = (1024);
    qint32 kBytesReceived = bytesReceived / KB_DIVIDER;
    qint32 kBytesTotal = bytesTotal / KB_DIVIDER;
    qint32 progress = 0;
    if (kBytesTotal > 0)
    {
        progress = kBytesReceived * 100 / kBytesTotal;
    }
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->setProgress(tr("Downloading record ") + m_replay + " " + QString::number(kBytesReceived) + " KB / " + QString::number(kBytesTotal) + " KB", progress);
}

QString DialogAwbwRecordDownloader::getReplay() const
{
    return m_replay;
}

void DialogAwbwRecordDownloader::setReplay(const QString & newReplay)
{
    m_replay = newReplay;
}

QString DialogAwbwRecordDownloader::getPassword() const
{
    return m_password;
}

void DialogAwbwRecordDownloader::setPassword(const QString & newPassword)
{
    m_password = newPassword;
}

QString DialogAwbwRecordDownloader::getUserName() const
{
    return m_userName;
}

void DialogAwbwRecordDownloader::setUserName(const QString & newUserName)
{
    m_userName = newUserName;
}
