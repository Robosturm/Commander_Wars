#include "qfile.h"
#include "qdiriterator.h"

#include "wiki/wikidatabase.h"
#include "wiki/fieldinfo.h"
#include "wiki/defaultwikipage.h"

#include "resource_management/buildingspritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/terrainmanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/coperkmanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/achievementmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/terrainmanager.h"
#include "resource_management/shoploader.h"

#include "game/co.h"
#include "game/player.h"
#include "game/unit.h"
#include "game/building.h"
#include "game/terrain.h"
#include "game/gamemap.h"

#include "objects/coinfoactor.h"

#include "coreengine/interpreter.h"

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
    Mainapp* pMainapp = Mainapp::getInstance();
    this->moveToThread(pMainapp->getWorkerthread());    
    Interpreter::setCppOwnerShip(this);

    oxygine::Resources::loadXML("resources/images/wiki/res.xml");

    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        if (QFile::exists(Settings::getMods().at(i) + "/images/wiki/res.xml"))
        {
            oxygine::Resources::loadXML(QString(Settings::getMods().at(i) + "/images/wiki/res.xml"));
        }
    }
}

void WikiDatabase::load()
{
    // load database
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    for (qint32 i = 0; i < pCOSpriteManager->getCount(); i++)
    {
        m_Entries.append(pageData(pCOSpriteManager->getName(i), pCOSpriteManager->getID(i), "CO"));
    }
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    QStringList sortedTerrains = pTerrainManager->getTerrainsSorted();
    for (const auto& terrainId : sortedTerrains)
    {
        m_Entries.append(pageData(pTerrainManager->getName(terrainId), terrainId, "Terrain"));
    }
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    for (qint32 i = 0; i < pBuildingSpriteManager->getCount(); i++)
    {
        m_Entries.append(pageData(pBuildingSpriteManager->getName(i), pBuildingSpriteManager->getID(i), "Building"));
    }
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    QStringList sortedUnits = pUnitSpriteManager->getUnitsSorted();
    for (const auto& unitId : sortedUnits)
    {
        m_Entries.append(pageData(pUnitSpriteManager->getName(unitId), unitId, "Unit"));
    }

    COPerkManager* pCOPerkManager = COPerkManager::getInstance();
    QStringList perks = pCOPerkManager->getLoadedRessources();
    for (const auto& perk : perks)
    {
        m_Entries.append(pageData(pCOPerkManager->getName(perk), perk, "Perk"));
    }

    // load general wiki page
    QStringList searchPaths;
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        searchPaths.append(Settings::getMods().at(i) + "/scripts/wiki");
    }
    searchPaths.append("resources/scripts/wiki");
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString path =  QCoreApplication::applicationDirPath() + "/" + searchPaths[i];
        QStringList filter;
        filter << "*.js";
        QDirIterator* dirIter = new QDirIterator(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter->hasNext())
        {
            dirIter->next();
            QString file = dirIter->fileInfo().absoluteFilePath();
            if (!hasEntry(file))
            {
                Interpreter* pInterpreter = Interpreter::getInstance();
                pInterpreter->openScript(file, false);
                QJSValue erg = pInterpreter->doFunction("LOADEDWIKIPAGE", "getName");
                QString name = "";
                if (erg.isString())
                {
                    name = erg.toString();
                }
                QStringList tags;
                erg = pInterpreter->doFunction("LOADEDWIKIPAGE", "getTags");
                tags = erg.toVariant().toStringList();
                m_Entries.append(pageData(name, file, tags));
            }
        }
    }
}

QVector<WikiDatabase::pageData> WikiDatabase::getEntries(QString searchTerm, bool onlyTag)
{
    QVector<pageData> ret;
    for (qint32 i = 0; i < m_Entries.size(); i++)
    {
        if ((std::get<0>(m_Entries[i]).contains(searchTerm, Qt::CaseInsensitive) && !onlyTag) ||
            (tagMatches(std::get<2>(m_Entries[i]), searchTerm)))
        {
            ret.append(m_Entries[i]);
        }
    }
    return ret;
}

bool WikiDatabase::hasEntry(QString file1)
{
    QString entry2 = file1;
    entry2 = entry2.replace(".js", "");
    entry2 = entry2.remove(0, entry2.lastIndexOf("/") + 1);
    entry2 = entry2.remove(0, entry2.lastIndexOf("\\") + 1);
    for (auto & entryInfo : m_Entries)
    {
        QString entry = std::get<1>(entryInfo);
        entry = entry.replace(".js", "");
        entry = entry.remove(0, entry.lastIndexOf("/") + 1);
        entry = entry.remove(0, entry.lastIndexOf("\\") + 1);
        if (entry == entry2)
        {
            return true;
        }
    }
    return false;
}

QVector<QString> WikiDatabase::getTags()
{
    QVector<QString> ret;
    for (qint32 i = 0; i < m_Entries.size(); i++)
    {
        QStringList tags = std::get<2>(m_Entries[i]);
        for (qint32 i2 = 0; i2 < tags.size(); i2++)
        {
            if (!ret.contains(tags[i2]))
            {
               ret.append(tags[i2]);
            }
        }
    }
    return ret;
}

WikiDatabase::pageData WikiDatabase::getEntry(qint32 entry)
{
    if (entry >= 0 && entry < m_Entries.size())
    {
        return m_Entries[entry];
    }
    return pageData("", "", QStringList());
}

WikiDatabase::pageData WikiDatabase::getEntry(QString id)
{
    for (qint32 i = 0; i < m_Entries.size(); i++)
    {
        QString entry = std::get<1>(m_Entries[i]);
        entry = entry.replace(".js", "");
        entry = entry.remove(0, entry.lastIndexOf("/") + 1);
        entry = entry.remove(0, entry.lastIndexOf("\\") + 1);
        if (entry == id)
        {
            return m_Entries[i];
        }
        else if (entry.toUpper() == id.toUpper())
        {
            return m_Entries[i];
        }
    }
    return pageData("", "", QStringList());
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

spWikipage WikiDatabase::getPage(pageData data)
{
    spWikipage ret;
    QString id = std::get<1>(data);
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    // select page loader and create wiki page
    if (pCOSpriteManager->exists(id))
    {
        spPlayer pPlayer = new Player();
        pPlayer->init();
        spCO pCO = new CO(id, pPlayer.get());
        ret = new Wikipage();
        spCOInfoActor pInfo = new COInfoActor(ret->getPanel()->getWidth());
        pInfo->showCO(pCO, pPlayer);
        ret->getPanel()->addItem(pInfo);
        ret->getPanel()->setContentHeigth(pInfo->getHeight());
    }
    else if (pTerrainManager->exists(id))
    {
        spTerrain pTerrain = Terrain::createTerrain(id, -1, -1, "");
        ret = new FieldInfo(pTerrain.get(), nullptr);
    }
    else if (pBuildingSpriteManager->exists(id))
    {
        spTerrain pTerrain = Terrain::createTerrain("PLAINS", -1, -1, "");
        pTerrain->setBuilding(new Building(id));
        ret = new FieldInfo(pTerrain.get(), nullptr);
    }
    else if (pUnitSpriteManager->exists(id))
    {
        spPlayer pPlayer = new Player();
        pPlayer->init();
        spUnit pUnit = new Unit(id, pPlayer.get(), false);
        ret = new FieldInfo(nullptr, pUnit.get());
    }
    else if (QFile::exists(id))
    {
        // default loader
        ret = new Wikipage();
        Interpreter* pInterpreter = Interpreter::getInstance();
        pInterpreter->openScript(id, false);
        QJSValueList args;
        QJSValue obj1 = pInterpreter->newQObject(ret.get());
        args << obj1;
        QJSValue erg = pInterpreter->doFunction("LOADEDWIKIPAGE", "loadPage", args);
    }
    else
    {
        ret = new DefaultWikipage(id);
    }
    
    return ret;
}


oxygine::spSprite WikiDatabase::getIcon(QString file, qint32 size)
{
    oxygine::spSprite pSprite = new oxygine::Sprite();
    oxygine::ResAnim* pAnim = WikiDatabase::getInstance()->getResAnim(file, oxygine::error_policy::ep_ignore_error);
    if (pAnim == nullptr)
    {
        pAnim = COSpriteManager::getInstance()->getResAnim(file, oxygine::error_policy::ep_ignore_error);
    }
    if (pAnim == nullptr)
    {
        pAnim = GameManager::getInstance()->getResAnim(file, oxygine::error_policy::ep_ignore_error);
    }
    if (pAnim == nullptr)
    {
        pAnim = COPerkManager::getInstance()->getResAnim(file, oxygine::error_policy::ep_ignore_error);
    }
    if (pAnim == nullptr)
    {
        pAnim = AchievementManager::getInstance()->getResAnim(file, oxygine::error_policy::ep_ignore_error);
    }
    if (pAnim == nullptr)
    {
        pAnim = ShopLoader::getInstance()->getResAnim(file, oxygine::error_policy::ep_ignore_error);
    }
    if (pAnim != nullptr)
    {
        pSprite->setResAnim(pAnim);
        pSprite->setScale(size / pAnim->getWidth());
    }
    else
    {
        UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
        BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
        TerrainManager* pTerrainManager = TerrainManager::getInstance();
        if (pUnitSpriteManager->exists(file))
        {
            spPlayer pPlayer = new Player();
            pPlayer->init();
            spUnit pUnit = new Unit(file, pPlayer.get(), false);
            pUnit->setScale(size / GameMap::getImageSize());
            pUnit->setOwner(nullptr);
            pSprite = pUnit.get();
        }
        else if (pBuildingSpriteManager->exists(file))
        {
            // check buildings?
            spGameMap pMap = GameMap::getInstance();
            spPlayer pPlayer = nullptr;
            if (pMap.get() != nullptr)
            {
                pPlayer = pMap->getCurrentPlayer();
            }
            Building* pBuilding = new Building(file);
            pBuilding->setOwner(pPlayer.get());
            pBuilding->scaleAndShowOnSingleTile();
            return pBuilding;
        }
        else if (pTerrainManager->exists(file))
        {
            spTerrain pTerrain = Terrain::createTerrain(file, -1, -1, "");
            pTerrain->loadSprites();
            return pTerrain.get();
        }
    }
    return pSprite;
}
