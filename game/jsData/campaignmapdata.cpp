#include "game/jsData/campaignmapdata.h"

#include "coreengine/interpreter.h"

CampaignMapData::CampaignMapData()
{
    setObjectName("CampaignMapData");
    Interpreter::setCppOwnerShip(this);
}

const QString CampaignMapData::getMapBackground() const
{
    return m_mapBackground;
}

void CampaignMapData::setMapBackground(const QString &newMapBackground)
{
    m_mapBackground = newMapBackground;
}

const QString CampaignMapData::getFolder() const
{
    return m_folder;
}

void CampaignMapData::setFolder(const QString &newFolder)
{
    m_folder = newFolder;
}

qint32 CampaignMapData::getMapWidth() const
{
    return m_mapWidth;
}

void CampaignMapData::setMapWidth(qint32 newMapWidth)
{
    m_mapWidth = newMapWidth;
}

qint32 CampaignMapData::getMapHeight() const
{
    return m_mapHeight;
}

void CampaignMapData::setMapHeight(qint32 newMapHeight)
{
    m_mapHeight = newMapHeight;
}

const QStringList CampaignMapData::getMapFilenames() const
{
    return m_mapFilenames;
}

void CampaignMapData::setMapFilenames(const QStringList &newMapFilenames)
{
    m_mapFilenames = newMapFilenames;
}

const QVector<QPointF> CampaignMapData::getOpenMapPositions() const
{
    return m_openMapPositions;
}

void CampaignMapData::setOpenMapPositions(const QVector<QPointF> &newOpenMapPositions)
{
    m_openMapPositions = newOpenMapPositions;
}

const QVector<qint32> CampaignMapData::getNewMapPosition() const
{
    return m_newMapPosition;
}

void CampaignMapData::setNewMapPosition(const QVector<qint32> &newNewMapPosition)
{
    m_newMapPosition = newNewMapPosition;
}

qint32 CampaignMapData::getNewlyWonMap() const
{
    return m_newlyWonMap;
}

void CampaignMapData::setNewlyWonMap(qint32 newNewlyWonMap)
{
    m_newlyWonMap = newNewlyWonMap;
}

const QVector<QPointF> CampaignMapData::getWonMapPositions() const
{
    return m_wonMapPositions;
}

void CampaignMapData::setWonMapPositions(const QVector<QPointF> &newWonMapPositions)
{
    m_wonMapPositions = newWonMapPositions;
}
