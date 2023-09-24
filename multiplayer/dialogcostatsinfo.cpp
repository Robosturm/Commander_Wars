#include "multiplayer/dialogcostatsinfo.h"
#include "multiplayer/lobbymenu.h"

#include "network/JsonKeys.h"

#include "resource_management/cospritemanager.h"

DialogCoStatsInfo::DialogCoStatsInfo(LobbyMenu *pBaseMenu, const QJsonObject &objData)
    : CustomDialog("", "", pBaseMenu, tr("Close")),
      m_coStats(objData)
{
    COSpriteManager *pCOSpriteManager = COSpriteManager::getInstance();
    auto coids = pCOSpriteManager->getCoIds();
    auto sortedCos = pCOSpriteManager->getCoGroups(coids);
    for (const auto &group : sortedCos)
    {
        for (const auto &co : group.cos)
        {
            CoData data;
            data.coid = co;
            data.playedGames = getGamesMade(co, true);
            m_sortedCoids.push_back(data);
        }
    }
    std::sort(m_sortedCoids.begin(), m_sortedCoids.end(), [](const CoData &lhs, const CoData &rhs)
    {
       return lhs.playedGames > rhs.playedGames; 
    });
    m_uiXml = "ui/multiplayer/coStatsInfo.xml";
    loadXmlFile(m_uiXml);
}

qint32 DialogCoStatsInfo::getGamesWon(const QString &coid)
{
    auto stats = getCoStats(coid);
    if (stats.isEmpty())
    {
        return 0;
    }
    else
    {
        return stats.value(JsonKeys::JSONKEY_COSTATSWON).toInt();
    }
}

qint32 DialogCoStatsInfo::getGamesLost(const QString &coid)
{
    auto stats = getCoStats(coid);
    if (stats.isEmpty())
    {
        return 0;
    }
    else
    {
        return stats.value(JsonKeys::JSONKEY_COSTATSLOST).toInt();
    }
}

qint32 DialogCoStatsInfo::getGamesMade(const QString &coid, bool zeroBased)
{

    auto stats = getCoStats(coid);
    if (stats.isEmpty())
    {
        if (zeroBased)
        {
            return 0;
        }
        return 1;
    }
    else
    {
        return stats.value(JsonKeys::JSONKEY_COSTATSWON).toInt() +
               stats.value(JsonKeys::JSONKEY_COSTATSLOST).toInt() +
               stats.value(JsonKeys::JSONKEY_COSTATSDRAW).toInt();
    }
}

qint32 DialogCoStatsInfo::getGamesDraw(const QString &coid)
{
    auto stats = getCoStats(coid);
    if (stats.isEmpty())
    {
        return 0;
    }
    else
    {
        return stats.value(JsonKeys::JSONKEY_COSTATSDRAW).toInt();
    }
}

QJsonObject DialogCoStatsInfo::getCoStats(const QString &coid)
{
    return m_coStats.value(coid).toObject();
}

qint32 DialogCoStatsInfo::getCoCount()
{
    return m_sortedCoids.size();
}
    
QString DialogCoStatsInfo::getCoId(qint32 index)
{
    if (index >= 0 && index < m_sortedCoids.size())
    {
        return m_sortedCoids[index].coid;
    }
    return "";
}
