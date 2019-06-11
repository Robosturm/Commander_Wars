#include "mapselectionview.h"

#include "game/gamemap.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/buildingspritemanager.h"

#include "coreengine/mainapp.h"

#include "game/gamemap.h"

MapSelectionView::MapSelectionView()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 0;
    if (pApp->getSettings()->getWidth() / 2 > 400)
    {
        width = 400;
    }
    else if (pApp->getSettings()->getWidth() / 2 < 300)
    {
        width = 300;
    }
    else
    {
        width = pApp->getSettings()->getWidth() / 2;
    }

    m_pMapSelection = new MapSelection(pApp->getSettings()->getHeight() - 70, width, "");
    m_pMapSelection->setPosition(10, 10);
    this->addChild(m_pMapSelection);
    m_pMinimap = new Minimap();
    m_pMinimap->setPosition(0, 0);
    m_pMinimap->setScale(2.0f);

    m_MinimapSlider = new oxygine::SlidingActor();

    m_MinimapSlider->setPosition(10, 10);
    m_MinimapSlider->setSize(pApp->getSettings()->getWidth() - width - 100 - 20,
                             pApp->getSettings()->getHeight() / 2 - 235);
    m_MinimapSlider->setContent(m_pMinimap);

    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    m_pMiniMapBox = new oxygine::Box9Sprite();
    m_pMiniMapBox->setResAnim(pAnim);
    m_pMiniMapBox->setPosition(width + 50, 50);
    m_pMiniMapBox->setSize(pApp->getSettings()->getWidth() - width - 100,
                           pApp->getSettings()->getHeight() / 2 - 215);
    m_pMiniMapBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pMiniMapBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);


    m_pMiniMapBox->addChild(m_MinimapSlider);
    addChild(m_pMiniMapBox);

    // map info text
    m_MapInfo = new Panel(true, QSize(pApp->getSettings()->getWidth() - width - 100, pApp->getSettings()->getHeight() / 2 - 60),
                          QSize(pApp->getSettings()->getWidth() - width - 100, pApp->getSettings()->getHeight() / 2 - 60));
    m_MapInfo->setPosition(width + 50, pApp->getSettings()->getHeight() / 2 - 140);
    this->addChild(m_MapInfo);
    oxygine::spTextField pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setPosition(10, 10);
    pTextfield->setText(tr("Map Name: ").toStdString().c_str());
    m_MapInfo->addItem(pTextfield);
    m_MapName = new oxygine::TextField();
    m_MapName->setStyle(style);
    m_MapName->setPosition(150, 10);
    m_MapInfo->addItem(m_MapName);

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setPosition(10, 50);
    pTextfield->setText(tr("Map Author: ").toStdString().c_str());
    m_MapInfo->addItem(pTextfield);
    m_MapAuthor = new oxygine::TextField();
    m_MapAuthor->setStyle(style);
    m_MapAuthor->setPosition(150, 50);
    m_MapInfo->addItem(m_MapAuthor);

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setPosition(10, 90);
    pTextfield->setText(tr("Map Description ").toStdString().c_str());
    m_MapInfo->addItem(pTextfield);

    style.multiline = true;
    m_MapDescription = new oxygine::TextField();
    m_MapDescription->setStyle(style);
    m_MapDescription->setWidth(m_MapInfo->getContentWidth() - 80);
    m_MapDescription->setPosition(10, 130);
    m_MapInfo->addItem(m_MapDescription);

    // building count
    pAnim = pObjectManager->getResAnim("mapSelectionBuildingInfo");
    m_pBuildingBackground = new oxygine::Box9Sprite();
    m_pBuildingBackground->setResAnim(pAnim);
    m_pBuildingBackground->setSize(pApp->getSettings()->getWidth() - width - 100, 60);
    m_pBuildingBackground->setPosition(m_MapInfo->getX(),
                                     m_MapInfo->getY() + m_MapInfo->getHeight() + 20);
    m_pBuildingBackground->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pBuildingBackground->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    oxygine::TextStyle styleTimes10 = FontManager::getTimesFont10();
    styleTimes10.color = oxygine::Color(255, 255, 255, 255);
    styleTimes10.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    styleTimes10.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    styleTimes10.multiline = false;

    oxygine::spSlidingActor slider = new oxygine::SlidingActor();
    slider->setSize(m_pBuildingBackground->getWidth() - 20, 100);
    oxygine::spActor content = new oxygine::Actor();
    content->setSize(pBuildingSpriteManager->getBuildingCount()* (GameMap::Imagesize + 12), 100);
    for (qint32 i = 0; i < pBuildingSpriteManager->getBuildingCount(); i++)
    {
        spBuilding building = new Building(pBuildingSpriteManager->getBuildingID(i));
        building->updateBuildingSprites();
        qint32 width = building->getBuildingWidth();
        qint32 heigth = building->getBuildingHeigth();
        building->setScaleX(1.0f / static_cast<float>(width));
        building->setScaleY(1.0f / static_cast<float>(heigth));
        building->setPosition(i * (GameMap::Imagesize + 12) + GameMap::Imagesize * (width - 1) / (width),
                              5 + GameMap::Imagesize / 2 + GameMap::Imagesize * (heigth - 1) / (heigth));
        content->addChild(building);
        oxygine::spTextField pText = new oxygine::TextField();
        pText->setText("0");
        pText->setPosition(2 + i * (GameMap::Imagesize + 12), 10 + GameMap::Imagesize * 1.2f);
        pText->setStyle(styleTimes10);
        content->addChild(pText);
        m_BuildingCountTexts.push_back(pText);
    }
    slider->setX(10);
    slider->setContent(content);
    m_pBuildingBackground->addChild(slider);
    addChild(m_pBuildingBackground);
}

void MapSelectionView::loadMap(QFileInfo info)
{
    if (info.isFile() && info.fileName().endsWith(".map"))
    {
        if (m_pCurrentMap != nullptr)
        {
            m_pCurrentMap->deleteMap();
            m_pCurrentMap = nullptr;
        }
        m_pCurrentMap = new GameMap(info.absoluteFilePath(), true);
        m_pCurrentMap->getGameScript()->init();
        m_pMinimap->updateMinimap(m_pCurrentMap);
        m_MinimapSlider->setContent(m_pMinimap);
        m_MinimapSlider->snap();
        m_MapName->setText(m_pCurrentMap->getMapName().toStdString().c_str());
        m_MapAuthor->setText(m_pCurrentMap->getMapAuthor().toStdString().c_str());
        m_MapDescription->setText(m_pCurrentMap->getMapDescription().toStdString().c_str());
        m_MapInfo->setContentHeigth(m_MapDescription->getY() + m_MapDescription->getTextRect().getHeight() + 30);
        m_currentMapFile = info;

        BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
        for (qint32 i = 0; i < pBuildingSpriteManager->getBuildingCount(); i++)
        {
            qint32 count = m_pCurrentMap->getBuildingCount(pBuildingSpriteManager->getBuildingID(i));
            m_BuildingCountTexts[i]->setText(QString::number(count).toStdString().c_str());
        }
    }
    else if (info.isFile() && info.fileName().endsWith(".cmp"))
    {
        if (m_pCurrentMap != nullptr)
        {
            m_pCurrentMap->deleteMap();
            m_pCurrentMap = nullptr;
        }
        m_pMinimap->updateMinimap(nullptr);
        m_CurrentCampaign = nullptr;
        m_CurrentCampaign = new Campaign(info.absoluteFilePath());
        m_MapDescription->setText(m_CurrentCampaign->getDescription().toStdString().c_str());
        m_MapAuthor->setText(m_CurrentCampaign->getAuthor().toStdString().c_str());
        m_MapName->setText(m_CurrentCampaign->getName().toStdString().c_str());
    }
}

void MapSelectionView::updateMapData()
{
    m_MapName->setText(m_pCurrentMap->getMapName().toStdString().c_str());
    m_MapAuthor->setText(m_pCurrentMap->getMapAuthor().toStdString().c_str());
    m_MapDescription->setText(m_pCurrentMap->getMapDescription().toStdString().c_str());
}
