#include "awbwReplayReader/awbwreplayplayer.h"

#include "game/gamemap.h"

AwbwReplayPlayer::AwbwReplayPlayer(GameMap* pMap)
    : m_pMap(pMap)
{
    connect(&m_mapDownloader, &AwbwMapDownloader::sigDownloadResult, this, &AwbwReplayPlayer::onDownloadResult, Qt::QueuedConnection);
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
    ++m_currentActionPos;
    // todo
    return spGameAction();
}

qint32 AwbwReplayPlayer::getRecordSize()
{
    qint32 count = 0;
    const auto & actions = m_replayReader.getActions();
    for (const auto & action : actions)
    {
        count += action.actionData.size();
    }
    return count;
}

qint32 AwbwReplayPlayer::getProgess()
{
    return m_currentActionPos;
}

qint32 AwbwReplayPlayer::getDayFromPosition(qint32 count)
{
    qint32 day = 0;
    qint32 currentCount = 0;
    const auto & actions = m_replayReader.getActions();
    while (day < actions.size() && currentCount < count)
    {
        currentCount += actions[day].actionData.size();
        ++day;
    }
    return day;
}

void AwbwReplayPlayer::seekToDay(qint32 day)
{
    const auto & actions = m_replayReader.getActions();
    m_currentActionPos = 0;
    for (qint32 i = 0; i < day; ++i)
    {
        m_currentActionPos += actions[i].actionData.size();
    }
    loadMap(true);
}

void AwbwReplayPlayer::requestReplayStart()
{
    if (m_startReplay)
    {
        startReplayInternal();
    }
    else
    {
        m_startReplay = true;
    }
}

void AwbwReplayPlayer::onDownloadResult(bool success)
{
    if (m_startReplay && success)
    {
        startReplayInternal();
    }
    else
    {
        m_startReplay = true;
    }
}

void AwbwReplayPlayer::startReplayInternal()
{
    loadMap(false);
    emit startReplay();
}

void AwbwReplayPlayer::loadMap(bool withOutUnits)
{
    m_pMap->setIsHumanMatch(false);
    m_mapDownloader.loadMap(m_pMap, withOutUnits);
    // swap out all ai's / or players with a proxy ai.
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        m_pMap->getPlayer(i)->setBaseGameInput(BaseGameInputIF::createAi(m_pMap, GameEnums::AiTypes::AiTypes_ProxyAi));
    }
}
