#include "mapselectionmapsmenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/buildingspritemanager.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/co.h"

#include "menue/mainwindow.h"

#include "objects/dropdownmenucolor.h"

#include "QFileInfo"

MapSelectionMapsMenue::MapSelectionMapsMenue()
    : QObject()
{
    Console::print("Entering Map Selection Menue", Console::eDEBUG);
    Mainapp* pApp = Mainapp::getInstance();
    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    pBuildingSpriteManager->loadAll();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("Background+1");
    sprite->setResAnim(pBackground);
    sprite->setPosition(0, 0);
    // background should be last to draw
    sprite->setPriority(static_cast<short>(Mainapp::ZOrder::Background));
    sprite->setScaleX(pApp->getSettings()->getWidth() / pBackground->getWidth());
    sprite->setScaleY(pApp->getSettings()->getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->loadFolder("resources/music/mapselectionmenue");
    pApp->getAudioThread()->playRandom();

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
                             pApp->getSettings()->getHeight() - 210 - 20);
    m_MinimapSlider->setContent(m_pMinimap);

    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    m_pMiniMapBox = new oxygine::Box9Sprite();
    m_pMiniMapBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pMiniMapBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_pMiniMapBox->setResAnim(pAnim);
    m_pMiniMapBox->setPosition(width + 50, 50);
    m_pMiniMapBox->setSize(pApp->getSettings()->getWidth() - width - 100,
                           pApp->getSettings()->getHeight() - 210);

    m_pMiniMapBox->addChild(m_MinimapSlider);
    addChild(m_pMiniMapBox);

    // building count
    pAnim = pObjectManager->getResAnim("mapSelectionBuildingInfo");
    m_pBuildingBackground = new oxygine::Box9Sprite();
    m_pBuildingBackground->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pBuildingBackground->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_pBuildingBackground->setResAnim(pAnim);
    m_pBuildingBackground->setSize(pApp->getSettings()->getWidth() - width - 100, 60);
    m_pBuildingBackground->setPosition(m_pMiniMapBox->getX(),
                                     m_pMiniMapBox->getY() + m_pMiniMapBox->getHeight() + 20);
    oxygine::TextStyle style = FontManager::getTimesFont10();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::spSlidingActor slider = new oxygine::SlidingActor();
    slider->setSize(m_pBuildingBackground->getWidth() - 20, 100);
    oxygine::spActor content = new oxygine::Actor();
    content->setSize(pBuildingSpriteManager->getBuildingCount()* (GameMap::Imagesize + 12), 100);
    for (qint32 i = 0; i < pBuildingSpriteManager->getBuildingCount(); i++)
    {
        spBuilding building = new Building(pBuildingSpriteManager->getBuildingID(i));
        building->updateBuildingSprites();
        building->setPosition(i * (GameMap::Imagesize + 12), 5 + GameMap::Imagesize / 2);
        content->addChild(building);
        oxygine::spTextField pText = new oxygine::TextField();
        pText->setText("0");
        pText->setPosition(2 + i * (GameMap::Imagesize + 12), 10 + GameMap::Imagesize * 1.2f);
        pText->setStyle(style);
        content->addChild(pText);
        m_BuildingCountTexts.push_back(pText);
    }
    slider->setX(10);
    slider->setContent(content);
    m_pBuildingBackground->addChild(slider);
    addChild(m_pBuildingBackground);

    connect(m_pMapSelection.get(), SIGNAL(itemChanged(QString)), this, SLOT(mapSelectionItemChanged(QString)), Qt::QueuedConnection);
    connect(m_pMapSelection.get(), SIGNAL(itemClicked(QString)), this, SLOT(mapSelectionItemClicked(QString)), Qt::QueuedConnection);

    oxygine::spButton pButtonBack = ObjectManager::createButton(tr("Back"));
    pButtonBack->setPosition(10, pApp->getSettings()->getHeight() - 10 - pButtonBack->getHeight());
    pButtonBack->attachTo(this);
    pButtonBack->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonBack();
    });
    connect(this, SIGNAL(buttonBack()), this, SLOT(slotButtonBack()), Qt::QueuedConnection);
    oxygine::spButton pButtonNext = ObjectManager::createButton(tr("Next"));
    pButtonNext->setPosition(pApp->getSettings()->getWidth() - 10 - pButtonBack->getWidth(), pApp->getSettings()->getHeight() - 10 - pButtonBack->getHeight());
    pButtonNext->attachTo(this);
    pButtonNext->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonNext();
    });
    connect(this, SIGNAL(buttonNext()), this, SLOT(slotButtonNext()), Qt::QueuedConnection);

    qint32 yPos = 10;
    m_pPlayerSelection = new Panel(true,
                                   QSize(pApp->getSettings()->getWidth() - 20,
                                         pApp->getSettings()->getHeight() - yPos - 20 - pButtonBack->getHeight()),
                                   QSize(pApp->getSettings()->getWidth() - 70, 100));
    m_pPlayerSelection->setPosition(10, yPos);
    this->addChild(m_pPlayerSelection);
    hideCOSelection();
}

MapSelectionMapsMenue::~MapSelectionMapsMenue()
{

}

void MapSelectionMapsMenue::slotButtonBack()
{
    switch (m_MapSelectionStep)
    {
        case MapSelectionStep::selectMap:
        {
            Console::print("Leaving Map Selection Menue", Console::eDEBUG);
            oxygine::getStage()->addChild(new Mainwindow());
            oxygine::Actor::detach();
            break;
        }
        case MapSelectionStep::selectRules:
        {
            showMapSelection();
            m_MapSelectionStep = MapSelectionStep::selectMap;
            break;
        }
        case MapSelectionStep::selectPlayer:
        {
            hideCOSelection();
            m_MapSelectionStep = MapSelectionStep::selectRules;
            break;
        }
    }
}

void MapSelectionMapsMenue::slotButtonNext()
{
    switch (m_MapSelectionStep)
    {
        case MapSelectionStep::selectMap:
        {
            if (m_pCurrentMap != nullptr)
            {
                hideMapSelection();
                m_MapSelectionStep = MapSelectionStep::selectRules;
            }
            break;
        }
        case MapSelectionStep::selectRules:
        {
            showCOSelection();
            m_MapSelectionStep = MapSelectionStep::selectPlayer;
            break;
        }
        case MapSelectionStep::selectPlayer:
        {
            break;
        }
    }
}

void MapSelectionMapsMenue::mapSelectionItemClicked(QString item)
{
    QFileInfo info = m_pMapSelection->getCurrentFolder() + item;
    if (info.isFile())
    {
        emit buttonNext();
    }
}

void MapSelectionMapsMenue::mapSelectionItemChanged(QString item)
{
    QFileInfo info = m_pMapSelection->getCurrentFolder() + item;
    if (info.isFile())
    {
        if (m_pCurrentMap != nullptr)
        {
            delete m_pCurrentMap;
            m_pCurrentMap = nullptr;
        }
        m_pCurrentMap = new GameMap(info.absoluteFilePath(), false, true);
        m_pMinimap->updateMinimap(m_pCurrentMap);
        m_MinimapSlider->setContent(m_pMinimap);
        m_MinimapSlider->snap();
        BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
        for (qint32 i = 0; i < pBuildingSpriteManager->getBuildingCount(); i++)
        {
            qint32 count = m_pCurrentMap->getBuildingCount(pBuildingSpriteManager->getBuildingID(i));
            m_BuildingCountTexts[i]->setText(QString::number(count).toStdString().c_str());
        }
    }
}

void MapSelectionMapsMenue::hideMapSelection()
{
    m_pMapSelection->setVisible(false);
    m_pMiniMapBox->setVisible(false);
    m_pBuildingBackground->setVisible(false);
}
void MapSelectionMapsMenue::showMapSelection()
{
    m_pMapSelection->setVisible(true);
    m_pMiniMapBox->setVisible(true);
    m_pBuildingBackground->setVisible(true);
}

void MapSelectionMapsMenue::hideCOSelection()
{
    m_pPlayerSelection->setVisible(false);
    m_pPlayerSelection->clearContent();
}
void MapSelectionMapsMenue::showCOSelection()
{
    m_pPlayerSelection->setVisible(true);
    // font style
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    // add player labels at top
    QStringList items = {tr("Username"), tr("CO's"), tr("Color"), tr("AI Strength"), tr("Startfonds"), tr("Income Modifier"), tr("Team")};
    QVector<qint32> xPositions;
    qint32 labelminStepSize = 150;
    qint32 curPos = 5;

    oxygine::spTextField pLabel;
    for (qint32 i = 0; i < items.size(); i++)
    {
        xPositions.append(curPos);
        pLabel = new oxygine::TextField();
        pLabel->setStyle(style);
        pLabel->setText(items[i].toStdString().c_str());
        qint32 width = pLabel->getTextRect().getWidth() + 10;
        pLabel->setPosition(curPos, 5);
        m_pPlayerSelection->addItem(pLabel);
        if (width < labelminStepSize)
        {
            width = labelminStepSize;
        }
        curPos += width;
    }
    xPositions.append(curPos);
    m_pPlayerSelection->setContentWidth(curPos);
    qint32 y = pLabel->getTextRect().getHeight() + 10 + 50; // 50 is dummy value
    // all player
    pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setText(tr("All").toStdString().c_str());
    pLabel->setPosition(xPositions[0], y);
    m_pPlayerSelection->addItem(pLabel);
    qint32 itemIndex = 4;

    spSpinBox allStartFondsSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 100000);
    allStartFondsSpinBox->setPosition(xPositions[itemIndex], y);
    m_pPlayerSelection->addItem(allStartFondsSpinBox);
    connect(allStartFondsSpinBox.get(), SIGNAL(sigValueChanged(float)), this, SLOT(allPlayerStartFondsChanged(float)), Qt::QueuedConnection);
    itemIndex = 5;
    spSpinBox allIncomeSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 10, SpinBox::Mode::Float);
    allIncomeSpinBox->setPosition(xPositions[itemIndex], y);
    allIncomeSpinBox->setCurrentValue(1.0f);
    allIncomeSpinBox->setSpinSpeed(0.1f);
    m_pPlayerSelection->addItem(allIncomeSpinBox);
    connect(allIncomeSpinBox.get(), SIGNAL(sigValueChanged(float)), this, SLOT(allPlayerIncomeChanged(float)), Qt::QueuedConnection);

    y += 10 + allIncomeSpinBox->getHeight();
    QVector<QString> teamList;
    for (qint32 i = 0; i < m_pCurrentMap->getPlayerCount(); i++)
    {
        teamList.append(tr("Team") + " " + QString::number(i + 1));
    }

    Mainapp* pApp = Mainapp::getInstance();
    QString function = "getDefaultPlayerColors";
    QJSValueList args;
    QJSValue ret = pApp->getInterpreter()->doFunction("PLAYER", function, args);
    qint32 colorCount = ret.toInt();
    QVector<QColor> playerColors;

    for (qint32 i = 0; i < colorCount; i++)
    {
        Mainapp* pApp = Mainapp::getInstance();
        QString function = "getDefaultColor";
        QJSValueList args;
        args << i;
        ret = pApp->getInterpreter()->doFunction("PLAYER", function, args);
        playerColors.append(QColor(ret.toString()));
    }

    // add player selection information
    for (qint32 i = 0; i < m_pCurrentMap->getPlayerCount(); i++)
    {


        bool up = false;
        if ((m_pCurrentMap->getPlayerCount() - i <= 5) &&
            (i > 5))
        {
            up = true;
        }

        itemIndex = 2;
        spDropDownmenuColor playerColor = new DropDownmenuColor(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, playerColors, up);
        playerColor->setPosition(xPositions[itemIndex], y);
        playerColor->setCurrentItem(m_pCurrentMap->getPlayer(i)->getColor());
        m_pPlayerSelection->addItem(playerColor);
        connect(playerColor.get(), &DropDownmenuColor::sigItemChanged, this, [=](QColor value)
        {
            playerColorChanged(value, i);
        }, Qt::QueuedConnection);

        itemIndex = 4;
        spSpinBox playerStartFondsSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 100000);
        playerStartFondsSpinBox->setPosition(xPositions[itemIndex], y);
        playerStartFondsSpinBox->setCurrentValue(m_pCurrentMap->getPlayer(i)->getFonds());
        m_pPlayerSelection->addItem(playerStartFondsSpinBox);
        m_playerStartFonds.append(playerStartFondsSpinBox);
        connect(playerStartFondsSpinBox.get(), &SpinBox::sigValueChanged, this, [=](float value)
        {
            playerStartFondsChanged(value, i);
        }, Qt::QueuedConnection);

        itemIndex = 5;
        spSpinBox playerIncomeSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 10, SpinBox::Mode::Float);
        playerIncomeSpinBox->setPosition(xPositions[itemIndex], y);
        playerIncomeSpinBox->setCurrentValue(m_pCurrentMap->getPlayer(i)->getFondsModifier());
        playerIncomeSpinBox->setSpinSpeed(0.1f);
        m_pPlayerSelection->addItem(playerIncomeSpinBox);
        m_playerIncomes.append(playerIncomeSpinBox);
        connect(playerIncomeSpinBox.get(), &SpinBox::sigValueChanged, this, [=](float value)
        {
            playerIncomeChanged(value, i);
        }, Qt::QueuedConnection);

        itemIndex = 6;

        spDropDownmenu playerTeam = new DropDownmenu(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, teamList, up);
        playerTeam->setPosition(xPositions[itemIndex], y);
        playerTeam->setCurrentItem(m_pCurrentMap->getPlayer(i)->getTeam());
        m_pPlayerSelection->addItem(playerTeam);
        connect(playerTeam.get(), &DropDownmenu::sigItemChanged, this, [=](qint32 value)
        {
            playerTeamChanged(value, i);
        }, Qt::QueuedConnection);

        y += 10 + playerIncomeSpinBox->getHeight();
    }
    m_pPlayerSelection->setContentHeigth(y);
}

void MapSelectionMapsMenue::allPlayerIncomeChanged(float value)
{
    for (qint32 i = 0; i < m_pCurrentMap->getPlayerCount(); i++)
    {
        m_pCurrentMap->getPlayer(i)->setFondsModifier(value);
        m_playerIncomes[i]->setCurrentValue(value);
    }
}
void MapSelectionMapsMenue::allPlayerStartFondsChanged(float value)
{
    for (qint32 i = 0; i < m_pCurrentMap->getPlayerCount(); i++)
    {
        m_pCurrentMap->getPlayer(i)->setFonds(static_cast<qint32>(value));
        m_playerStartFonds[i]->setCurrentValue(value);
    }
}
void MapSelectionMapsMenue::playerIncomeChanged(float value, qint32 playerIdx)
{
    m_pCurrentMap->getPlayer(playerIdx)->setFondsModifier(static_cast<qint32>(value));
}
void MapSelectionMapsMenue::playerStartFondsChanged(float value, qint32 playerIdx)
{
    m_pCurrentMap->getPlayer(playerIdx)->setFonds(static_cast<qint32>(value));
}
void MapSelectionMapsMenue::playerTeamChanged(qint32 value, qint32 playerIdx)
{
    m_pCurrentMap->getPlayer(playerIdx)->setTeam(value);
}
void MapSelectionMapsMenue::playerColorChanged(QColor value, qint32 playerIdx)
{
    m_pCurrentMap->getPlayer(playerIdx)->setColor(value);
}
