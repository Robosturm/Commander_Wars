#pragma once

#include <QObject>
#include <QTimer>
#include "updater/filedownloader.h"
#include "updater/zipSupport/qzipreader.h"

class GameUpdater;
using spGameUpdater = std::shared_ptr<GameUpdater>;

class GameUpdater final : public QObject
{
    Q_OBJECT
public:
    static const char* const MODE_FORCE;
    static const char* const MODE_INSTALL;

    explicit GameUpdater();
    virtual ~GameUpdater() = default;
    static void launchPatcher();
    static void launchApplication();
    static void cleanUpOldArtifacts();
public slots:
    void onNewState(FileDownloader::State state);
    void onNewProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onExtractProgress(qint32 progress);
private slots:
    void continueBooting();
    void updateUi();
private:
    void finishDownload();
    void install();
private:
    FileDownloader m_filedownloader;
    QFile m_downloadFile;
    QZipReader m_zipReader;
    bool m_continued{false};
    QTimer m_updateTimer;
};

