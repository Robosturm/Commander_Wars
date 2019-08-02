#include "wikidatabase.h"

#include "resource_management/buildingspritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/terrainmanager.h"
#include "resource_management/cospritemanager.h"

WikiDatabase* WikiDatabase::m_pInstance = nullptr;

WikiDatabase* WikiDatabase::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new WikiDatabase();
    }
    return m_pInstance;
}

WikiDatabase::WikiDatabase()
    : QObject()
{
    // load database
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    for (qint32 i = 0; i < pCOSpriteManager->getCOCount(); i++)
    {
        m_Entries.append(pageData(pCOSpriteManager->getCOName(i), pCOSpriteManager->getCOID(i), "CO"));
    }
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    for (qint32 i = 0; i < pTerrainManager->getTerrainCount(); i++)
    {
        m_Entries.append(pageData(pTerrainManager->getTerrainName(i), pTerrainManager->getTerrainID(i), "TERRAIN"));
    }
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    for (qint32 i = 0; i < pBuildingSpriteManager->getBuildingCount(); i++)
    {
        m_Entries.append(pageData(pBuildingSpriteManager->getBuildingName(i), pBuildingSpriteManager->getBuildingID(i), "BUILDING"));
    }
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    for (qint32 i = 0; i < pUnitSpriteManager->getUnitCount(); i++)
    {
        m_Entries.append(pageData(pUnitSpriteManager->getUnitName(i), pUnitSpriteManager->getUnitID(i), "UNIT"));
    }
    // load general wiki page
}

QVector<WikiDatabase::pageData> WikiDatabase::getEntries(QString searchTerm)
{
    QVector<pageData> ret;
    for (qint32 i = 0; i < m_Entries.size(); i++)
    {
        if ((std::get<0>(m_Entries[i]).contains(searchTerm, Qt::CaseInsensitive)) ||
            (tagMatches(std::get<2>(m_Entries[i]), searchTerm)))
        {
            ret.append(m_Entries[i]);
        }
    }
    return ret;
}

bool WikiDatabase::tagMatches(QStringList tags, QString searchTerm)
{
    for (qint32 i = 0; i < tags.size(); i++)
    {
        if (tags[i].contains(searchTerm, Qt::CaseInsensitive))
        {
            return true;
        }
    }
    return false;
}

oxygine::spActor WikiDatabase::getPage(pageData& data)
{
    oxygine::spActor ret;

    return ret;
}
