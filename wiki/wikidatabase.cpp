#include <QDirIterator>

#include "wiki/wikidatabase.h"
#include "wiki/fieldinfo.h"
#include "wiki/defaultwikipage.h"
#include "wiki/damagetablepage.h"
#include "wiki/actionwikipage.h"

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
#ifdef GRAPHICSUPPORT
    setObjectName("WikiDatabase");
#endif
    Interpreter::setCppOwnerShip(this);
}

void WikiDatabase::load()
{
    // load database
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    for (qint32 i = 0; i < pCOSpriteManager->getCount(); i++)
    {
        m_Entries.append(spPageData::create(pCOSpriteManager->getName(i), pCOSpriteManager->getID(i), QStringList({tr("CO")})));
    }
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    QStringList sortedTerrains = pTerrainManager->getTerrainsSorted();
    for (const auto& terrainId : sortedTerrains)
    {
        m_Entries.append(spPageData::create(pTerrainManager->getName(terrainId), terrainId, QStringList({tr("Terrain")})));
    }
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    for (qint32 i = 0; i < pBuildingSpriteManager->getCount(); i++)
    {
        m_Entries.append(spPageData::create(pBuildingSpriteManager->getName(i), pBuildingSpriteManager->getID(i), QStringList({tr("Building")})));
    }
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    QStringList sortedUnits = pUnitSpriteManager->getUnitsSorted();
    for (const auto& unitId : sortedUnits)
    {
        m_Entries.append(spPageData::create(pUnitSpriteManager->getName(unitId), unitId, QStringList({tr("Unit")})));
    }

    Interpreter* pInterpreter = Interpreter::getInstance();
    GameManager* pGameManager = GameManager::getInstance();
    QStringList  actions = pGameManager->getLoadedRessources();
    for (const auto& action : actions)
    {
        m_Entries.append(spPageData::create(pGameManager->getName(action), action, QStringList({tr("Action")})));
        QJSValue count = pInterpreter->doFunction(action, "getSubWikiInfoCount");
        if (count.isNumber())
        {
            qint32 items = count.toInt();
            for (qint32 i = 0; i < items; ++i)
            {
                QJSValueList args;
                args.append(i);
                m_Entries.append(spPageData::create(pInterpreter->doFunction(action, "getSubWikiInfoName", args).toString(),
                                          pInterpreter->doFunction(action, "getSubWikiInfoId", args).toString(),
                                          QStringList({tr("Action")}),
                                          action,
                                          i));
            }
        }
    }

    COPerkManager* pCOPerkManager = COPerkManager::getInstance();
    QStringList perks = pCOPerkManager->getLoadedRessources();
    for (const auto& perk : perks)
    {
        m_Entries.append(spPageData::create(pCOPerkManager->getName(perk), perk, QStringList({tr("Perk")})));
    }

    m_Entries.append(spPageData::create(tr("Damage Table"), DAMAGE_TABLE_NAME, QStringList({tr("Others")})));

    // load general wiki page
    QStringList searchPaths;
    for (qint32 i = 0; i < Settings::getInstance()->getMods().size(); i++)
    {
        searchPaths.append(Settings::getInstance()->getUserPath() + Settings::getInstance()->getMods().at(i) + "/scripts/wiki");
    }
    searchPaths.append(Settings::getInstance()->getUserPath() + "resources/scripts/wiki");
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
                m_Entries.append(spPageData::create(name, file, tags));
            }
        }
    }
}

QVector<PageData *> WikiDatabase::getEntries(QString searchTerm, bool onlyTag)
{
    QVector<PageData *> ret;
    for (auto & entry : m_Entries)
    {
        if ((entry->m_name.contains(searchTerm, Qt::CaseInsensitive) && !onlyTag) ||
            (tagMatches(entry->m_tags, searchTerm)))
        {
            ret.append(entry.get());
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
        QString entry = entryInfo->m_id;
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
        QStringList tags = entry->m_tags;
        for (auto & tag : tags)
        {
            if (!ret.contains(tag))
            {
               ret.append(tag);
            }
        }
    }
    std::sort(ret.begin(), ret.end(), [](const QString& lhs, const QString& rhs)
    {
        return lhs < rhs;
    });
    return ret;
}

const PageData * WikiDatabase::getEntry(qint32 entry)
{
    if (entry >= 0 && entry < m_Entries.size())
    {
        return m_Entries[entry].get();
    }
    return &m_emptyPage;
}

const PageData * WikiDatabase::getEntry(QString id)
{
    for (auto & entryPage : m_Entries)
    {
        QString entry = entryPage->m_id;
        entry = entry.replace(".js", "");
        entry = entry.remove(0, entry.lastIndexOf("/") + 1);
        entry = entry.remove(0, entry.lastIndexOf("\\") + 1);
        if (entry == id)
        {
            return entryPage.get();
        }
        else if (entry.toUpper() == id.toUpper())
        {
            return entryPage.get();
        }
    }
    return &m_emptyPage;
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

spWikipage WikiDatabase::getPage(const PageData * data)
{
    spWikipage ret;
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    GameManager* pGameManager = GameManager::getInstance();
    // select page loader and create wiki page
    if (pCOSpriteManager->exists(data->m_id))
    {
        spPlayer pPlayer = spPlayer::create(nullptr);
        pPlayer->init();
        spCO pCO = spCO::create(data->m_id, pPlayer.get(), nullptr);
        ret = spWikipage::create(data->m_id);
        spCOInfoActor pInfo = spCOInfoActor::create(nullptr, ret->getPanel()->getScaledWidth());
        pInfo->showCO(pCO, pPlayer);
        ret->getPanel()->addItem(pInfo);
        ret->getPanel()->setContentHeigth(pInfo->getScaledHeight());
    }
    else if (pTerrainManager->exists(data->m_id))
    {
        spTerrain pTerrain = Terrain::createTerrain(data->m_id, -1, -1, "", nullptr);
        ret = spFieldInfo::create(pTerrain.get(), nullptr);
    }
    else if (pBuildingSpriteManager->exists(data->m_id))
    {
        spTerrain pTerrain = Terrain::createTerrain(GameMap::PLAINS, -1, -1, "", nullptr);
        spBuilding pBuilding = spBuilding::create(data->m_id, nullptr);
        pTerrain->setBuilding(pBuilding);
        ret = spFieldInfo::create(pTerrain.get(), nullptr);
    }
    else if (pUnitSpriteManager->exists(data->m_id))
    {
        spPlayer pPlayer = spPlayer::create(nullptr);
        pPlayer->init();
        spUnit pUnit = spUnit::create(data->m_id, pPlayer.get(), false, nullptr);
        ret = spFieldInfo::create(nullptr, pUnit.get());
    }
    else if (data->m_id == DAMAGE_TABLE_NAME)
    {
        ret = spDamageTablePage::create(DAMAGE_TABLE_NAME);
    }
    else if (!data->m_mainId.isEmpty() || pGameManager->exists(data->m_id))
    {
        ret = spActionWikipage::create<const PageData *>(data);
    }
    else if (QFile::exists(data->m_id))
    {
        // default loader
        ret = spWikipage::create(data->m_id);
        Interpreter* pInterpreter = Interpreter::getInstance();
        pInterpreter->openScript(data->m_id, false);
        QJSValueList args({pInterpreter->newQObject(ret.get())});
        pInterpreter->doFunction("LOADEDWIKIPAGE", "loadPage", args);
    }
    else
    {
        ret = spDefaultWikipage::create(data->m_id);
    }
    ret->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));
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
        pSprite->setScale(static_cast<float>(size) / static_cast<float>(pAnim->getWidth()));
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
            pUnit->setScale(static_cast<float>(size) / static_cast<float>(GameMap::getImageSize()));
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
                    pPlayer = spPlayer(pMap->getCurrentPlayer());
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
            pTerrain->setScale(static_cast<float>(size) / static_cast<float>(GameMap::getImageSize()));
            pTerrain->loadSprites();
            return pTerrain;
        }
    }
    return pSprite;
}
