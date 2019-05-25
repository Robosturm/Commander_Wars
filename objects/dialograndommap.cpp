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

    float y = 30;
    qint32 width = 250;
    // Label
    oxygine::spTextField text = new oxygine::TextField();
    oxygine::TextStyle style = FontManager::getMainFont();
    text->setStyle(style);
    text->setText(tr("Map name:").toStdString().c_str());
    text->setPosition(30, y);
    pSpriteBox->addChild(text);
    m_MapName = new Textbox(pApp->getSettings()->getWidth() - 60 - width);
    m_MapName->setPosition(text->getX() + width, text->getY());
    m_MapName->setCurrentText("");
    pSpriteBox->addChild(m_MapName);
    y += 40;

    // Label
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Map Author:").toStdString().c_str());
    text->setPosition(30, y);
    pSpriteBox->addChild(text);
    m_MapAuthor = new Textbox(pApp->getSettings()->getWidth() - 60 - width);
    m_MapAuthor->setPosition(text->getX() + width, text->getY());
    m_MapAuthor->setCurrentText(Settings::getUsername());
    pSpriteBox->addChild(m_MapAuthor);
    y += 40;

    // Label
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Map Description:").toStdString().c_str());
    text->setPosition(30, y);
    pSpriteBox->addChild(text);
    m_MapDescription = new Textbox(pApp->getSettings()->getWidth() - 60 - width);
    m_MapDescription->setPosition(text->getX() + width, text->getY());
    m_MapDescription->setCurrentText("");
    pSpriteBox->addChild(m_MapDescription);
    y += 40;

    // Label
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Map width:").toStdString().c_str());
    text->setPosition(30, 5 + y );
    pSpriteBox->addChild(text);
    m_MapWidth = new SpinBox(300, 1, 999, SpinBox::Mode::Int);
    m_MapWidth->setPosition(text->getX() + width, text->getY());
    m_MapWidth->setCurrentValue(20);
    pSpriteBox->addChild(m_MapWidth);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Map heigth:").toStdString().c_str());
    text->setPosition(30, 5 + y + text->getHeight());
    pSpriteBox->addChild(text);
    m_MapHeigth = new SpinBox(300, 1, 999, SpinBox::Mode::Int);
    m_MapHeigth->setPosition(text->getX() + width, text->getY());
    m_MapHeigth->setCurrentValue(20);
    pSpriteBox->addChild(m_MapHeigth);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Map Player:").toStdString().c_str());
    text->setPosition(30, 5 + y + text->getHeight());
    pSpriteBox->addChild(text);
    m_MapPlayerCount = new SpinBox(300, 2, 40, SpinBox::Mode::Int);
    m_MapPlayerCount->setPosition(text->getX() + width, text->getY());
    m_MapPlayerCount->setCurrentValue(4);
    pSpriteBox->addChild(m_MapPlayerCount);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Map Seed:").toStdString().c_str());
    text->setPosition(30, 5 + y + text->getHeight());
    pSpriteBox->addChild(text);
    m_Seed = new SpinBox(300, 0, std::numeric_limits<qint32>::max() - 1, SpinBox::Mode::Int);
    m_Seed->setPosition(text->getX() + width, text->getY());
    m_Seed->setCurrentValue(Mainapp::randInt(0, std::numeric_limits<qint32>::max() - 1));
    pSpriteBox->addChild(m_Seed);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Create Road:").toStdString().c_str());
    text->setPosition(30, 5 + y + text->getHeight());
    pSpriteBox->addChild(text);
    m_CreateRoad = new Checkbox();
    m_CreateRoad->setChecked(true);
    m_CreateRoad->setPosition(text->getX() + width, text->getY());
    pSpriteBox->addChild(m_CreateRoad);

    // Label
    y += 40;
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Terrain Distribution").toStdString().c_str());
    text->setPosition(30, 5 + y + text->getHeight());
    pSpriteBox->addChild(text);

    y += 40;
    QVector<QString> terrainStrings = {tr("Plains"), tr("Forest"), tr("Mountain"), tr("Sea"), tr("Town")};
    QVector<qint32> terrainChances = {50, 17, 16, 0, 17};
    m_TerrainChances = new Multislider(terrainStrings, pApp->getSettings()->getWidth() - 80, terrainChances);
    m_TerrainChances->setPosition(30, y);
    pSpriteBox->addChild(m_TerrainChances);

    // Label
    y += 40 * terrainStrings.size();
    text = new oxygine::TextField();
    text->setStyle(style);
    text->setText(tr("Building Distribution").toStdString().c_str());
    text->setPosition(30, 5 + y + text->getHeight());
    pSpriteBox->addChild(text);

    y += 40;
    QVector<QString> buildingStrings = {tr("Factory"), tr("Airport"), tr("Harbour"), tr("Town")};
    QVector<qint32> buildingChances = {20, 10, 10, 60};
    m_BuildingChances = new Multislider(buildingStrings, pApp->getSettings()->getWidth() - 80, buildingChances);
    m_BuildingChances->setPosition(30, y);
    pSpriteBox->addChild(m_BuildingChances);



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
