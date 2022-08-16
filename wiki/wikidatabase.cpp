#include "qfile.h"
#include "qdiriterator.h"

#include "wiki/wikidatabase.h"
#include "wiki/fieldinfo.h"
#include "wiki/defaultwikipage.h"
#include "wiki/damagetablepage.h"

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

const char* const DAMAGE_TABLE_NAME = "Damage Table";

WikiDatabase::WikiDatabase()
    : RessourceManagement<WikiDatabase>("/resources/images/wiki/res.xml",
      "")
{
    setObjectName("WikiDatabase");
    Mainapp* pMainapp = Mainapp::getInstance();
    moveToThread(pMainapp->getWorkerthread());    
    Interpreter::setCppOwnerShip(this);
}

void WikiDatabase::load()
{
    // load database
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    for (qint32 i = 0; i < pCOSpriteManager->getCount(); i++)
    {
        m_Entries.append(PageData(pCOSpriteManager->getName(i), pCOSpriteManager->getID(i), {"CO"}));
    }
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    QStringList sortedTerrains = pTerrainManager->getTerrainsSorted();
    for (const auto& terrainId : sortedTerrains)
    {
        m_Entries.append(PageData(pTerrainManager->getName(terrainId), terrainId, {"Terrain"}));
    }
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    for (qint32 i = 0; i < pBuildingSpriteManager->getCount(); i++)
    {
        m_Entries.append(PageData(pBuildingSpriteManager->getName(i), pBuildingSpriteManager->getID(i), {"Building"}));
    }
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    QStringList sortedUnits = pUnitSpriteManager->getUnitsSorted();
    for (const auto& unitId : sortedUnits)
    {
        m_Entries.append(PageData(pUnitSpriteManager->getName(unitId), unitId, {"Unit"}));
    }

    COPerkManager* pCOPerkManager = COPerkManager::getInstance();
    QStringList perks = pCOPerkManager->getLoadedRessources();
    for (const auto& perk : perks)
    {
        m_Entries.append(PageData(pCOPerkManager->getName(perk), perk, {"Perk"}));
    }

    m_Entries.append(PageData(DAMAGE_TABLE_NAME, DAMAGE_TABLE_NAME, {"Others"}));

    // load general wiki page
    QStringList searchPaths;
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        searchPaths.append(Settings::getUserPath() + Settings::getMods().at(i) + "/scripts/wiki");
    }
    searchPaths.append(Settings::getUserPath() + "resources/scripts/wiki");
    searchPaths.append(QString(oxygine::Resource::RCC_PREFIX_PATH) + "resources/scripts/wiki");
    for (auto & path : searchPaths)
    {
        QStringList filter;
        filter << "*.js";
        QDirIterator dirIter = QDirIterator(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter.hasNext())
        {
            dirIter.next();
            QString file = dirIter.fileInfo().canonicalFilePath();
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
                m_Entries.append(PageData(name, file, tags));
            }
        }
    }
}

QVector<WikiDatabase::PageData> WikiDatabase::getEntries(QString searchTerm, bool onlyTag)
{
    QVector<PageData> ret;
    for (auto & entry : m_Entries)
    {
        if ((entry.m_name.contains(searchTerm, Qt::CaseInsensitive) && !onlyTag) ||
            (tagMatches(entry.m_tags, searchTerm)))
        {
            ret.append(entry);
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
        QString entry = entryInfo.m_id;
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

QStringList WikiDatabase::getTags()
{
    QStringList ret;
    for (auto & entry : m_Entries)
    {
        QStringList tags = entry.m_tags;
        for (auto & tag : tags)
        {
            if (!ret.contains(tag))
            {
               ret.append(tag);
            }
        }
    }
    return ret;
}

WikiDatabase::PageData WikiDatabase::getEntry(qint32 entry)
{
    if (entry >= 0 && entry < m_Entries.size())
    {
        return m_Entries[entry];
    }
    return PageData("", "", QStringList());
}

WikiDatabase::PageData WikiDatabase::getEntry(QString id)
{
    for (auto & entryPage : m_Entries)
    {
        QString entry = entryPage.m_id;
        entry = entry.replace(".js", "");
        entry = entry.remove(0, entry.lastIndexOf("/") + 1);
        entry = entry.remove(0, entry.lastIndexOf("\\") + 1);
        if (entry == id)
        {
            return entryPage;
        }
        else if (entry.toUpper() == id.toUpper())
        {
            return entryPage;
        }
    }
    return PageData("", id, QStringList());
}

bool WikiDatabase::tagMatches(const QStringList & tags, const QString & searchTerm)
{
    for (auto & tag : tags)
    {
        if (tag.contains(searchTerm, Qt::CaseInsensitive))
        {
            return true;
        }
    }
    return false;
}

spWikipage WikiDatabase::getPage(PageData data)
{
    spWikipage ret;
    QString id = data.m_id;
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    // select page loader and create wiki page
    if (pCOSpriteManager->exists(id))
    {
        spPlayer pPlayer = spPlayer::create(nullptr);
        pPlayer->init();
        spCO pCO = spCO::create(id, pPlayer.get(), nullptr);
        ret = spWikipage::create();
        spCOInfoActor pInfo = spCOInfoActor::create(nullptr, ret->getPanel()->getWidth());
        pInfo->showCO(pCO, pPlayer);
        ret->getPanel()->addItem(pInfo);
        ret->getPanel()->setContentHeigth(pInfo->getHeight());
    }
    else if (pTerrainManager->exists(id))
    {
        spTerrain pTerrain = Terrain::createTerrain(id, -1, -1, "", nullptr);
        ret = spFieldInfo::create(pTerrain.get(), nullptr);
    }
    else if (pBuildingSpriteManager->exists(id))
    {
        spTerrain pTerrain = Terrain::createTerrain(GameMap::PLAINS, -1, -1, "", nullptr);
        spBuilding pBuilding = spBuilding::create(id, nullptr);
        pTerrain->setBuilding(pBuilding);
        ret = spFieldInfo::create(pTerrain.get(), nullptr);
    }
    else if (pUnitSpriteManager->exists(id))
    {
        spPlayer pPlayer = spPlayer::create(nullptr);
        pPlayer->init();
        spUnit pUnit = spUnit::create(id, pPlayer.get(), false, nullptr);
        ret = spFieldInfo::create(nullptr, pUnit.get());
    }
    else if (id == DAMAGE_TABLE_NAME)
    {
        ret = spDamageTablePage::create();
    }
    else if (QFile::exists(id))
    {
        // default loader
        ret = spWikipage::create();
        Interpreter* pInterpreter = Interpreter::getInstance();
        pInterpreter->openScript(id, false);
        QJSValueList args({pInterpreter->newQObject(ret.get())});
        pInterpreter->doFunction("LOADEDWIKIPAGE", "loadPage", args);
    }
    else
    {
        ret = spDefaultWikipage::create(id);
    }
    ret->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    return ret;
}

oxygine::spSprite WikiDatabase::getIcon(GameMap* pMap, QString file, qint32 size, Player* pIconPlayer)
{
    oxygine::spSprite pSprite = oxygine::spSprite::create();
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
        Player* pFinalIconPlayer = pIconPlayer;
        spPlayer pPlayer;
        if (pUnitSpriteManager->exists(file))
        {
            if (pFinalIconPlayer == nullptr)
            {
                pPlayer = spPlayer::create(nullptr);
                pPlayer->init();
                pFinalIconPlayer = pPlayer.get();
            }
            spUnit pUnit = spUnit::create(file, pFinalIconPlayer, false, pMap);
            pUnit->setScale(size / GameMap::getImageSize());
            pUnit->setOwner(nullptr);
            pSprite = pUnit;
        }
        else if (pBuildingSpriteManager->exists(file))
        {
            // check buildings?
            if (pFinalIconPlayer == nullptr)
            {
                if (pMap != nullptr)
                {
                    pPlayer = pMap->getCurrentPlayer();
                }
                pFinalIconPlayer = pPlayer.get();
            }
            spBuilding pBuilding = spBuilding::create(file, pMap);
            pBuilding->setOwner(pFinalIconPlayer);
            pBuilding->scaleAndShowOnSingleTile();
            return pBuilding;
        }
        else if (pTerrainManager->exists(file))
        {
            spTerrain pTerrain = Terrain::createTerrain(file, -10, -10, "", pMap);
            pTerrain->loadSprites();
            return pTerrain;
        }
    }
    return pSprite;
}
