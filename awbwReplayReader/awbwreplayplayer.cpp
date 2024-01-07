#include "awbwReplayReader/awbwreplayplayer.h"

AwbwReplayPlayer::AwbwReplayPlayer(GameMap* pMap)
    : m_pMap(pMap)
{

}

bool AwbwReplayPlayer::loadRecord(const QString & filename)
{
    bool ret = false;
    m_replayReader.parseReplay(filename);
    auto const & states = m_replayReader.getGameStates();
    if (states.size() > 0)
    {
        m_mapDownloader.startMapDownload(states[0].mapId);
        ret = true;
    }
    return ret;
}

QStringList AwbwReplayPlayer::getMods()
{
    return {"mods/awds_unit", "mods/awds_co"};
}

spGameAction AwbwReplayPlayer::nextAction()
{
    // todo
    return spGameAction();
}

qint32 AwbwReplayPlayer::getRecordSize()
{
    // todo
    return 0;
}

qint32 AwbwReplayPlayer::getProgess()
{
    // todo
    return 0;
}

qint32 AwbwReplayPlayer::getDayFromPosition(qint32 count)
{
    // todo
    return 0;
}

void AwbwReplayPlayer::seekToDay(qint32 day)
{

}

void AwbwReplayPlayer::requestReplayStart()
{
    if (m_startReplay)
    {
        emit startReplay();
    }
    else
    {
        m_startReplay = true;
    }
}

void AwbwReplayPlayer::onDownloadResult(bool success)
{
    if (m_startReplay)
    {
        emit startReplay();
    }
    else
    {
        m_startReplay = true;
    }
}
