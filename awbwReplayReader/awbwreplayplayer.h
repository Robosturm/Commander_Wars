#pragma once

#include <QObject>
#include "awbwReplayReader/awbwmapdownloader.h"
#include "awbwReplayReader/awbwreplayerreader.h"
#include "game/gamerecording/iReplayReader.h"

class AwbwReplayPlayer : public IReplayReader
{
    Q_OBJECT
public:
    explicit AwbwReplayPlayer(GameMap* pMap);

    virtual bool loadRecord(const QString & filename) override;
    virtual QStringList getMods() override;
    virtual spGameAction nextAction() override;
    virtual qint32 getRecordSize() override;
    virtual qint32 getProgess() override;
    virtual qint32 getDayFromPosition(qint32 count) override;
    virtual void seekToDay(qint32 day) override;
    virtual void requestReplayStart() override;
private slots:
    void onDownloadResult(bool success);
private:
    void startReplayInternal();
    void loadMap(bool withOutUnits);
private:
    GameMap* m_pMap;
    AwbwMapDownloader m_mapDownloader;
    AwbwReplayerReader m_replayReader;
    bool m_startReplay{false};
    qint32 m_currentActionPos{0};
};

