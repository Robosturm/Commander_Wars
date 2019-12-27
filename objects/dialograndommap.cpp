#include "dialograndommap.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

DialogRandomMap::DialogRandomMap()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

    spPanel pPanel = new Panel(true, QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110),
                                     QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110));
    pPanel->setPosition(30, 30);
    pSpriteBox->addChild(pPanel);

    float y = 30;
    qint32 width = 250;
    // Label
    oxygine::spTextField text = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont();
    text->setStyle(style);
    text->setHtmlText(tr("Map name:"));
    text->setPosition(30, y);
    pPanel->addItem(text);
    m_MapName = new Textbox(pApp->getSettings()->getWidth() - 150 - width);
    m_MapName->setTooltipText(tr("Selects the name of the new map."));
    m_MapName->setPosition(text->getX() + width, text->getY());
    m_MapName->setCurrentText("");
    pPanel->addItem(m_MapName);
    y += 40;

    // Label
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Map Author:"));
    text->setPosition(30, y);
    pPanel->addItem(text);
    m_MapAuthor = new Textbox(pApp->getSettings()->getWidth() - 150 - width);
    m_MapAuthor->setTooltipText(tr("Selects the author of the new map."));
    m_MapAuthor->setPosition(text->getX() + width, text->getY());
    m_MapAuthor->setCurrentText(Settings::getUsername());
    pPanel->addItem(m_MapAuthor);
    y += 40;

    // Label
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Map Description:"));
    text->setPosition(30, y);
    pPanel->addItem(text);
    m_MapDescription = new Textbox(pApp->getSettings()->getWidth() - 150 - width);
    m_MapDescription->setTooltipText(tr("Selects the description for the new map."));
    m_MapDescription->setPosition(text->getX() + width, text->getY());
    m_MapDescription->setCurrentText("");
    pPanel->addItem(m_MapDescription);
    y += 40;

    // Label
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Map width:"));
    text->setPosition(30, 5 + y );
    pPanel->addItem(text);
    m_MapWidth = new SpinBox(300, 1, 999, SpinBox::Mode::Int);
    m_MapWidth->setTooltipText(tr("Selects the width for the new map."));
    m_MapWidth->setPosition(text->getX() + width, text->getY());
    m_MapWidth->setCurrentValue(20);
    pPanel->addItem(m_MapWidth);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Map heigth:"));
    text->setPosition(30, 5 + y + text->getHeight());
    pPanel->addItem(text);
    m_MapHeigth = new SpinBox(300, 1, 999, SpinBox::Mode::Int);
    m_MapHeigth->setTooltipText(tr("Selects the heigth for the new map."));
    m_MapHeigth->setPosition(text->getX() + width, text->getY());
    m_MapHeigth->setCurrentValue(20);
    pPanel->addItem(m_MapHeigth);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Map Player:"));
    text->setPosition(30, 5 + y + text->getHeight());
    pPanel->addItem(text);
    m_MapPlayerCount = new SpinBox(300, 2, 40, SpinBox::Mode::Int);
    m_MapPlayerCount->setTooltipText(tr("Selects the amount of players for the new map."));
    m_MapPlayerCount->setPosition(text->getX() + width, text->getY());
    m_MapPlayerCount->setCurrentValue(4);
    pPanel->addItem(m_MapPlayerCount);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Map Seed:"));
    text->setPosition(30, 5 + y + text->getHeight());
    pPanel->addItem(text);
    m_Seed = new SpinBox(300, 0, std::numeric_limits<qint32>::max() - 1, SpinBox::Mode::Int);
    m_Seed->setTooltipText(tr("The seed to generate the new map. Same map settings with the same seed generate the same map."));
    m_Seed->setPosition(text->getX() + width, text->getY());
    m_Seed->setCurrentValue(Mainapp::randInt(0, std::numeric_limits<qint32>::max() - 1));
    pPanel->addItem(m_Seed);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Create Road:"));
    text->setPosition(30, 5 + y + text->getHeight());
    pPanel->addItem(text);
    m_CreateRoad = new Checkbox();
    m_CreateRoad->setTooltipText(tr("If selected roads are created between the HQ's of the players."));
    m_CreateRoad->setChecked(true);
    m_CreateRoad->setPosition(text->getX() + width, text->getY());
    pPanel->addItem(m_CreateRoad);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Terrain Distribution"));
    text->setPosition(30, 5 + y + text->getHeight());
    pPanel->addItem(text);

    y += 40;
    QVector<QString> terrainStrings = {tr("Plains"), tr("Forest"), tr("Mountain"), tr("Sea"), tr("Town")};
    QVector<qint32> terrainChances = {50, 17, 16, 0, 17};
    m_TerrainChances = new Multislider(terrainStrings, pApp->getSettings()->getWidth() - 150, terrainChances);
    m_TerrainChances->setTooltipText(tr("The percent distribution between the different terrains and buildings."));
    m_TerrainChances->setPosition(30, y);
    pPanel->addItem(m_TerrainChances);

    // Label
    y += 40 * terrainStrings.size();
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setHtmlText(tr("Building Distribution"));
    text->setPosition(30, 5 + y + text->getHeight());
    pPanel->addItem(text);

    y += 40;
    QVector<QString> buildingStrings = {tr("Factory"), tr("Airport"), tr("Harbour"), tr("Town")};
    QVector<qint32> buildingChances = {20, 10, 10, 60};
    m_BuildingChances = new Multislider(buildingStrings, pApp->getSettings()->getWidth() - 150, buildingChances);
    m_BuildingChances->setTooltipText(tr("The percent distribution between the different buildings when a building is placed."));
    m_BuildingChances->setPosition(30, y);
    pPanel->addItem(m_BuildingChances);

    y += 40;
    pPanel->setContentHeigth(y + 40 * (buildingChances.size() + 1));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(pApp->getSettings()->getWidth() - m_OkButton->getWidth() - 30, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigFinished(m_MapName->getCurrentText(), m_MapAuthor->getCurrentText(),
                         m_MapDescription->getCurrentText(),
                         static_cast<qint32>(m_MapWidth->getCurrentValue()), static_cast<qint32>(m_MapHeigth->getCurrentValue()),
                         static_cast<qint32>(m_MapPlayerCount->getCurrentValue()),
                         m_CreateRoad->getChecked(), static_cast<qint32>(m_Seed->getCurrentValue()),
                         m_TerrainChances->getSliderValue(1), m_TerrainChances->getSliderValue(2),
                         m_TerrainChances->getSliderValue(3), m_TerrainChances->getSliderValue(4),
                         m_BuildingChances->getSliderValue(0), m_BuildingChances->getSliderValue(1),
                         m_BuildingChances->getSliderValue(2)
                         );
        this->getParent()->removeChild(this);
    });

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        this->getParent()->removeChild(this);
        emit sigCancel();
    });
}
