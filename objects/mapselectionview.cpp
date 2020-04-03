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
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 0;
    if (Settings::getWidth() / 2 > 400)
    {
        width = 400;
    }
    else if (Settings::getWidth() / 2 < 300)
    {
        width = 300;
    }
    else
    {
        width = Settings::getWidth() / 2;
    }

    m_pMapSelection = new MapSelection(Settings::getHeight() - 70, width, "");
    m_pMapSelection->setPosition(10, 10);
    this->addChild(m_pMapSelection);
    m_pMinimap = new Minimap();
    m_pMinimap->setPosition(0, 0);
    m_pMinimap->setScale(2.0f);

    QSize size(Settings::getWidth() - width - 80,
               Settings::getHeight() / 2 - 135);
    m_MinimapPanel = new Panel(true, size, size);
    m_MinimapPanel->setPosition(width + 50, 10);
    m_MinimapPanel->addItem(m_pMinimap);
    addChild(m_MinimapPanel);

    // map info text
    m_MapInfo = new Panel(true, QSize(Settings::getWidth() - width - 100, Settings::getHeight() / 2 - 60),
                          QSize(Settings::getWidth() - width - 100, Settings::getHeight() / 2 - 60));
    m_MapInfo->setPosition(width + 50, Settings::getHeight() / 2 - 100);
    this->addChild(m_MapInfo);
    oxygine::spTextField pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setPosition(10, 10);
    pTextfield->setHtmlText(tr("Map Name: "));
    m_MapInfo->addItem(pTextfield);
    m_MapName = new oxygine::TextField();
    m_MapName->setStyle(style);
    m_MapName->setPosition(150, 10);
    m_MapInfo->addItem(m_MapName);

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setPosition(10, 50);
    pTextfield->setHtmlText(tr("Map Author: "));
    m_MapInfo->addItem(pTextfield);
    m_MapAuthor = new oxygine::TextField();
    m_MapAuthor->setStyle(style);
    m_MapAuthor->setPosition(150, 50);
    m_MapInfo->addItem(m_MapAuthor);

    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setPosition(10, 90);
    pTextfield->setHtmlText(tr("Map Description "));
    m_MapInfo->addItem(pTextfield);

    style.multiline = true;
    m_MapDescription = new oxygine::TextField();
    m_MapDescription->setStyle(style);
    m_MapDescription->setWidth(m_MapInfo->getContentWidth() - 80);
    m_MapDescription->setPosition(10, 130);
    m_MapInfo->addItem(m_MapDescription);

    // building count
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("mapSelectionBuildingInfo");
    m_pBuildingBackground = new oxygine::Box9Sprite();
    m_pBuildingBackground->setResAnim(pAnim);
    m_pBuildingBackground->setSize(Settings::getWidth() - width - 100, 60);
    m_pBuildingBackground->setPosition(m_MapInfo->getX(),
                                       m_MapInfo->getY() + m_MapInfo->getHeight() + 20);
    m_pBuildingBackground->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pBuildingBackground->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    oxygine::TextStyle styleMain16 = FontManager::getMainFont16();
    styleMain16.color = FontManager::getFontColor();
    styleMain16.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    styleMain16.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    styleMain16.multiline = false;

    oxygine::spSlidingActor slider = new oxygine::SlidingActor();
    slider->setSize(m_pBuildingBackground->getWidth() - 20, 100);
    oxygine::spActor content = new oxygine::Actor();
    content->setSize(pBuildingSpriteManager->getCount()* (GameMap::Imagesize + 12), 100);
    for (qint32 i = 0; i < pBuildingSpriteManager->getCount(); i++)
    {
        spBuilding building = new Building(pBuildingSpriteManager->getID(i));
        building->updateBuildingSprites(false);
        qint32 width = building->getBuildingWidth();
        qint32 heigth = building->getBuildingHeigth();
        building->setScaleX(1.0f / static_cast<float>(width));
        building->setScaleY(1.0f / static_cast<float>(heigth));
        building->setPosition(i * (GameMap::Imagesize + 12) + GameMap::Imagesize * (width - 1) / (width),
                              5 + GameMap::Imagesize / 2 + GameMap::Imagesize * (heigth - 1) / (heigth));
        content->addChild(building);
        oxygine::spTextField pText = new oxygine::TextField();
        pText->setHtmlText("0");
        pText->setPosition(2 + i * (GameMap::Imagesize + 12), 12 + GameMap::Imagesize * 1.2f);
        pText->setStyle(styleMain16);
        content->addChild(pText);
        m_BuildingCountTexts.push_back(pText);
    }
    slider->setX(10);
    slider->setContent(content);
    m_pBuildingBackground->addChild(slider);
    addChild(m_pBuildingBackground);

    oxygine::spButton pButtonTop = new oxygine::Button();
    pButtonTop->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+right"));
    pButtonTop->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    oxygine::Sprite* ptr = pButtonTop.get();
    pButtonTop->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    pButtonTop->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButtonTop->setFlippedX(true);
    pButtonTop->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        if (content->getWidth() > slider->getWidth())
        {
            content->setX(content->getX() - GameMap::Imagesize);
            if (content->getX() + content->getWidth() < slider->getWidth())
            {
                content->setX(slider->getWidth() - content->getWidth());
            }
        }
    });
    pButtonTop->setPosition(m_MapInfo->getX() - 25, m_MapInfo->getY() + m_MapInfo->getHeight() + 30);
    addChild(pButtonTop);

    pButtonTop = new oxygine::Button();
    pButtonTop->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+right"));
    pButtonTop->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    ptr = pButtonTop.get();
    pButtonTop->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    pButtonTop->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
    {
        ptr->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pButtonTop->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        content->setX(content->getX() + GameMap::Imagesize);
        if (content->getX() > 0)
        {
            content->setX(0);
        }
    });
    pButtonTop->setPosition(m_MapInfo->getX() + slider->getWidth() + 25, m_MapInfo->getY() + m_MapInfo->getHeight() + 30);
    addChild(pButtonTop);
}

void MapSelectionView::loadMap(QFileInfo info)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (info.isFile() &&
        (info.fileName().endsWith(".map") ||
         info.fileName().endsWith(".msav")))
    {
        m_MinimapPanel->clearContent();
        if (m_pCurrentMap != nullptr)
        {
            m_pCurrentMap->deleteMap();
            m_pCurrentMap = nullptr;
        }
        m_pCurrentMap = new GameMap(info.absoluteFilePath(), true);
        m_pCurrentMap->getGameScript()->init();
        m_pMinimap->updateMinimap(m_pCurrentMap);
        m_MinimapPanel->addItem(m_pMinimap);
        m_MinimapPanel->setContentWidth(m_pMinimap->getScaledWidth() + 50);
        m_MinimapPanel->setContentHeigth(m_pMinimap->getScaledHeight() + 50);
        m_MapName->setHtmlText(m_pCurrentMap->getMapName());
        m_MapAuthor->setHtmlText(m_pCurrentMap->getMapAuthor());
        m_MapDescription->setHtmlText(m_pCurrentMap->getMapDescription());
        m_currentMapFile = info;

        BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
        for (qint32 i = 0; i < pBuildingSpriteManager->getCount(); i++)
        {
            qint32 count = m_pCurrentMap->getBuildingCount(pBuildingSpriteManager->getID(i));
            m_BuildingCountTexts[i]->setHtmlText(QString::number(count));
        }
    }
    else if (info.isFile() && info.fileName().endsWith(".jsm"))
    {
        m_MinimapPanel->clearContent();
        if (m_pCurrentMap != nullptr)
        {
            m_pCurrentMap->deleteMap();
            m_pCurrentMap = nullptr;
        }
        m_pMinimap->updateMinimap(nullptr);
        m_CurrentCampaign = nullptr;
        m_CurrentCampaign = new Campaign(info.absoluteFilePath());
        m_MapDescription->setHtmlText(m_CurrentCampaign->getDescription());
        m_MapAuthor->setHtmlText(m_CurrentCampaign->getAuthor());
        m_MapName->setHtmlText(m_CurrentCampaign->getName());
    }
    qint32 maxWidth = m_MapDescription->getX() + m_MapDescription->getTextRect().getWidth();
    if (maxWidth < m_MapAuthor->getX() + m_MapAuthor->getTextRect().getWidth())
    {
        maxWidth = m_MapAuthor->getX() + m_MapAuthor->getTextRect().getWidth();
    }
    if (maxWidth < m_MapName->getX() + m_MapName->getTextRect().getWidth())
    {
        maxWidth = m_MapName->getX() + m_MapName->getTextRect().getWidth();
    }
    m_MapInfo->setContentWidth(maxWidth + 30);
    m_MapInfo->setContentHeigth(m_MapDescription->getY() + m_MapDescription->getTextRect().getHeight() + 30);
    pApp->continueThread();
}

void MapSelectionView::updateMapData()
{
    m_MapName->setHtmlText(m_pCurrentMap->getMapName());
    m_MapAuthor->setHtmlText(m_pCurrentMap->getMapAuthor());
    m_MapDescription->setHtmlText(m_pCurrentMap->getMapDescription());
}
