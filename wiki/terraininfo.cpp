#include "wiki/terraininfo.h"
#include "wiki/wikidatabase.h"

#include "resource_management/fontmanager.h"
#include "resource_management/movementtablemanager.h"
#include "resource_management/unitspritemanager.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"

#include "ai/coreai.h"

TerrainInfo::TerrainInfo(Terrain* pTerrain, qint32 width)
    : QObject()
{
    setObjectName("TerrainInfo");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);

    setWidth(width);

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;

    oxygine::TextStyle headerStyle = FontManager::getMainFont48();
    headerStyle.color = FontManager::getFontColor();
    headerStyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = true;

    QString name = pTerrain->getTerrainName();
    QString description = pTerrain->getDescription();
    Building* pBuilding = pTerrain->getBuilding();
    if (pBuilding != nullptr)
    {
        name = pBuilding->getName();
        description = pBuilding->getDescription();
    }
    // no the fun begins create checkboxes and stuff and a panel down here
    qint32 y = 0;
    oxygine::spTextField pLabel = oxygine::spTextField::create();
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText((tr("Terrain Information ") + name));
    pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth() / 2, 0);
    addChild(pLabel);
    y += 60;

    pLabel = oxygine::spTextField::create();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(description);
    pLabel->setPosition(0, y);
    addChild(pLabel);
    y += 20 + pLabel->getTextRect().getHeight();

    qint32 xOffset = 200;
    // Income
    pLabel = oxygine::spTextField::create();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Defense:"));
    pLabel->setPosition(0, y);
    addChild(pLabel);
    pLabel = oxygine::spTextField::create();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText((QString::number(pTerrain->getDefense(nullptr))));
    pLabel->setPosition(xOffset, y);
    addChild(pLabel);
    y += 40;

    if (pBuilding != nullptr)
    {
        // Income
        pLabel = oxygine::spTextField::create();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Income:"));
        pLabel->setPosition(0, y);
        addChild(pLabel);
        pLabel = oxygine::spTextField::create();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText((QString::number(pBuilding->getIncome())));
        pLabel->setPosition(xOffset, y);
        addChild(pLabel);
        y += 40;

        QStringList productionList = pBuilding->getConstructionList();
        if (productionList.size() > 0)
        {
            pLabel = oxygine::spTextField::create();
            pLabel->setStyle(headerStyle);
            if (pBuilding->getActionList().contains(CoreAI::ACTION_BUILD_UNITS))
            {
                pLabel->setHtmlText(tr("Builds"));
                pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth() / 2, y);
                addChild(pLabel);
                y += 80;
                showUnitList(productionList, y, width);
            }
        }
        auto repairList = pBuilding->getRepairTypes();
        if (productionList.size() > 0 ||
            repairList.size() > 0)
        {
            pLabel = oxygine::spTextField::create();
            pLabel->setStyle(headerStyle);
            pLabel->setHtmlText(tr("Supplies"));
            pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth() / 2, y);
            addChild(pLabel);
            y += 80;
            UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
            for (qint32 i = 0; i < pUnitSpriteManager->getCount(); i++)
            {
                QString unitID = pUnitSpriteManager->getID(i);
                if (repairList.contains(static_cast<qint32>(pUnitSpriteManager->getUnitType(i))) &&
                    !productionList.contains(unitID))
                {
                    productionList.append(unitID);
                }
            }
            showUnitList(productionList, y, width);
        }
    }

    pLabel = oxygine::spTextField::create();
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText(tr("Movement Costs"));
    pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth() / 2, y);
    addChild(pLabel);
    y += 60;

    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
    qint32 x = 0;
    for (qint32 i = 0; i < pMovementTableManager->getCount(); i++)
    {
        QString name = pMovementTableManager->getName(pMovementTableManager->getID(i));
        qint32 costs = pMovementTableManager->getBaseMovementPoints(pMovementTableManager->getID(i), pTerrain, pTerrain, nullptr);

        pLabel = oxygine::spTextField::create();
        pLabel->setWidth(width);
        pLabel->setStyle(style);
        pLabel->setHtmlText((name + ":"));
        pLabel->setPosition(x, y);
        addChild(pLabel);
        pLabel = oxygine::spTextField::create();
        pLabel->setWidth(width);
        pLabel->setStyle(style);
        if (costs >= 0)
        {
            pLabel->setHtmlText(QString::number(costs));
        }
        else
        {
            pLabel->setHtmlText("-");
        }
        pLabel->setPosition(x + 160, y);
        addChild(pLabel);
        x += 210;
        if (x + 250 > width && i < pMovementTableManager->getCount() - 1)
        {
            x = 0;
            y += 40;
        }
    }
    y += 40;
    setHeight(y);
    connect(this, &TerrainInfo::sigShowLink, this, &TerrainInfo::showLink, Qt::QueuedConnection);
}

void TerrainInfo::showLink(QString pageID)
{
    WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
    oxygine::getStage()->addChild(pWikiDatabase->getPage(pWikiDatabase->getEntry(pageID)));
    
}

void TerrainInfo::showUnitList(QStringList productionList, qint32& y, qint32 width)
{
    qint32 x = 0;
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        m_pPlayer = pMap->getSpCurrentPlayer();
    }
    if (m_pPlayer.get() == nullptr)
    {
        m_pPlayer = spPlayer::create();
        m_pPlayer->init();
    }
    for (qint32 i = 0; i < productionList.size(); i++)
    {
        spUnit pDummy = spUnit::create(productionList[i], m_pPlayer.get(), false);
        pDummy->setPosition(x, y);
        pDummy->addClickListener([=](oxygine::Event*)
        {
           emit sigShowLink(productionList[i]);
        });
        addChild(pDummy);
        x += GameMap::getImageSize() * 2;
        if (x + GameMap::getImageSize() * 2 > width && i < productionList.size() - 1)
        {
            x = 0;
            y += 40;
        }
    }
    y += 40;
}
