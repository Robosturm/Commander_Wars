#pragma once

#include <QObject>
#include "awbwReplayReader/awbwmapdownloader.h"
#include "awbwReplayReader/awbwreplayerreader.h"

class AwbwReplayPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AwbwReplayPlayer(GameMap* pMap);

private:
    GameMap* m_pMap;
    AwbwMapDownloader m_mapDownloader;
    AwbwReplayerReader m_replayReader;
};

