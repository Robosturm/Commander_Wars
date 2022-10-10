#include "updater/gameupdater.h"

#include "objects/loadingscreen.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"

#include <QDir>
#include <QProcess>
#include <QDirIterator>

constexpr const char* const LATEST = "latest";
constexpr const char* const UPDATE_URL = "https://github.com/Robosturm/Commander_Wars/releases/";
constexpr const char* const DOWNLOADTARGET = "update.zip";
constexpr const char* const UNPACK_PATH = "update";
const char* const GameUpdater::MODE_FORCE = "force";
const char* const GameUpdater::MODE_INSTALL = "install";

GameUpdater::GameUpdater()
    : m_filedownloader(QString(COW_UPDATETARGET),
                       UPDATE_URL,
                       LATEST,
                       QString(COW_BUILD_TAG),
                       DOWNLOADTARGET),
      m_downloadFile(DOWNLOADTARGET),
      m_zipReader(&m_downloadFile)
{
    QString mode = Settings::getUpdateStep();
    if (mode == MODE_INSTALL)
    {
        install();
    }
    else
    {
        connect(&m_filedownloader, &FileDownloader::sigNewProgress, this, &GameUpdater::onNewProgress);
        connect(&m_filedownloader, &FileDownloader::sigNewState, this, &GameUpdater::onNewState);
        connect(&m_zipReader, &QZipReader::sigExtractProgress, this, &GameUpdater::onExtractProgress);
        spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
        pLoadingScreen->setProgress(tr("Cleaning up old update artifacts ..."), 0);
        Mainapp::getInstance()->redrawUi();
        QCoreApplication::processEvents();
        cleanUpOldArtifacts();
        pLoadingScreen->setProgress(tr("Checking for new version..."), 1);
        Mainapp::getInstance()->redrawUi();
        QCoreApplication::processEvents();
    }
}

void GameUpdater::cleanUpOldArtifacts()
{
    CONSOLE_PRINT("Clean up old artifacts", Console::eDEBUG);
    QFile downloadFile(DOWNLOADTARGET);
    if (downloadFile.exists())
    {
        downloadFile.remove();
    }
    QDir dir(QCoreApplication::applicationDirPath() + "/" + UNPACK_PATH);
    dir.removeRecursively();
}

void GameUpdater::install()
{
    CONSOLE_PRINT("Installing update", Console::eDEBUG);
    QString path = QCoreApplication::applicationDirPath();
    QDirIterator dirIter(path, QDir::Files, QDirIterator::Subdirectories);
    qint32 count = 0;
    while (dirIter.hasNext())
    {
        QCoreApplication::processEvents();
        dirIter.next();
        QString filePath = dirIter.fileInfo().filePath();
        if (!filePath.endsWith("*.ini"))
        {
            QString targetName = filePath;
            QString targetPath = QCoreApplication::applicationDirPath() + "/../../" + targetName.replace(path, "");
            QFile file(targetPath);
            if (file.exists())
            {
                file.remove();
            }
            else
            {
                QFileInfo info(targetPath);
                QDir dir(info.absoluteDir().canonicalPath());
                dir.mkpath(".");
            }
            QFile::copy(filePath, targetPath);
            CONSOLE_PRINT("Copying " + filePath + " to " + targetPath, Console::eDEBUG);
            spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
            pLoadingScreen->setProgress(tr("Copying files to target directory"), count);
            Mainapp::getInstance()->redrawUi();
            ++count;
        }
    }
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->setProgress(tr("Cleaning up"), 99);
    Mainapp::getInstance()->redrawUi();
    QCoreApplication::processEvents();
    QDir dir(QCoreApplication::applicationDirPath() + "/" + UNPACK_PATH);
    dir.removeRecursively();
    emit Mainapp::getInstance()->sigQuit(3);
}

void GameUpdater::onNewState(FileDownloader::State state)
{
    switch (state)
    {
        case FileDownloader::State::DownloadingFailed:
        {
            Console::print("Downloading new version failed.", Console::eINFO);
            continueBooting();
            break;
        }
        case FileDownloader::State::SameVersion:
        {
            continueBooting();
            break;
        }
        case FileDownloader::State::DownloadingNewVersion:
        {
            Console::print("Start downloading new version.", Console::eINFO);
            break;
        }
        case FileDownloader::State::DownloadingFinished:
        {
            finishDownload();
            break;
        }
    }
}

void GameUpdater::finishDownload()
{
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->setProgress(tr("Preparing extraction..."), 0);
    Mainapp::getInstance()->redrawUi();
    QCoreApplication::processEvents();
    m_downloadFile.open(QIODevice::ReadOnly);
    QDir dir(COW_INSTALLDIR);
    dir.removeRecursively();
    bool success = m_zipReader.extractAll(UNPACK_PATH);
    pLoadingScreen->setProgress(tr("Removing zip..."), 50);
    Mainapp::getInstance()->redrawUi();
    QCoreApplication::processEvents();
    m_downloadFile.close();
    m_downloadFile.remove();
    if (success)
    {
        pLoadingScreen->setProgress(tr("Launching patcher..."), 75);
        Mainapp::getInstance()->redrawUi();
        QCoreApplication::processEvents();
        emit Mainapp::getInstance()->sigQuit(2);
    }
    else
    {
        Console::print("Failed to extract new version.", Console::eINFO);
        continueBooting();
    }

}

void GameUpdater::launchPatcher()
{
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->setProgress(tr("Preparing update installation..."), 50);
    Mainapp::getInstance()->redrawUi();
    QCoreApplication::processEvents();
    QThread::currentThread()->msleep(100);
    QFileInfo info(QCoreApplication::applicationFilePath());
    QString program = info.fileName();
    QString patcherProgram =  QCoreApplication::applicationDirPath() + "/" + UNPACK_PATH + "/" + COW_INSTALLDIR + "/" + program;
    QThread::currentThread()->msleep(350);
    const char* const prefix = "--";
    QStringList args({QString(prefix) + CommandLineParser::ARG_UPDATE,
                      MODE_INSTALL});
    CONSOLE_PRINT("Starting patcher application " + patcherProgram, Console::eDEBUG);
    if (!QProcess::startDetached(patcherProgram, args))
    {
        CONSOLE_PRINT("Failed to start patcher application " + patcherProgram, Console::eERROR);
    }
}

void GameUpdater::launchApplication()
{
    QThread::currentThread()->msleep(100);
    QFileInfo info(QCoreApplication::applicationFilePath());
    QString program = info.fileName();
    QString workingDir = QCoreApplication::applicationDirPath() + "/../../";
    QString appPath = workingDir + program;
    QThread::currentThread()->msleep(350);
    CONSOLE_PRINT("Starting application " + appPath, Console::eDEBUG);
    QProcess process;
    process.setProgram(appPath);
    process.setWorkingDirectory(workingDir);
    process.startDetached();
}

void GameUpdater::continueBooting()
{
    Mainapp* pApp = Mainapp::getInstance();
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->setProgress(tr("Loading Object Textures ..."), 8);
    pApp->redrawUi();
    QCoreApplication::processEvents();
    m_failed = true;
    emit pApp->sigNextStartUpStep(static_cast<Mainapp::StartupPhase>(static_cast<qint8>(pApp->getStartUpStep()) + 1));
}

void GameUpdater::onNewProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    constexpr qint64 MB_DIVIDER = (1024 * 1024);
    qint32 kBytesReceived = bytesReceived / MB_DIVIDER;
    qint32 kBytesTotal = bytesTotal / MB_DIVIDER;
    qint32 progress = 0;
    if (kBytesTotal > 0)
    {
        progress = kBytesReceived * 100 / kBytesTotal;
    }
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->setProgress(tr("Downloading new version ") + QString::number(kBytesReceived) + " MB / " + QString::number(kBytesTotal) + " MB", progress);
    Mainapp::getInstance()->redrawUi();
    QCoreApplication::processEvents();
}

void GameUpdater::onExtractProgress(qint32 progress)
{
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->setProgress(tr("Extracting new version..."), progress);
    Mainapp::getInstance()->redrawUi();
    QCoreApplication::processEvents();
}
