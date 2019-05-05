#include "dialogmodifyunit.h"

#include "game/unit.h"

#include "game/player.h"

#include "game/co.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "game/gamemap.h"

#include "objects/panel.h"
#include "objects/slider.h"
#include "objects/dropdownmenu.h"

DialogModifyUnit::DialogModifyUnit(Unit* pUnit)
    : QObject(),
      m_pUnit(pUnit)
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

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(pApp->getSettings()->getWidth() / 2 - m_OkButton->getWidth() / 2, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        this->getParent()->removeChild(this);
    });

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    spPanel pPanel = new Panel(true, QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110),
                                     QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110));
    pPanel->setPosition(30, 30);
    pSpriteBox->addChild(pPanel);

    oxygine::spTextField pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setText((tr("Unit: ") + m_pUnit->getName()).toStdString().c_str());
    pLabel->setScale(2.0f);
    pLabel->setPosition(pPanel->getWidth() / 2 - pLabel->getTextRect().getWidth(), 10);
    pPanel->addItem(pLabel);

    qint32 sliderOffset = 400;
    qint32 y = 30 + pLabel->getTextRect().getHeight() * 2;
    pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setText(tr("HP: ").toStdString().c_str());
    pLabel->setPosition(10, y);
    pPanel->addItem(pLabel);
    spSlider pSlider = new Slider(pApp->getSettings()->getWidth() - 40 - sliderOffset, 1, 10, tr("HP"));
    pSlider->setPosition(sliderOffset - 160, y);
    pSlider->setCurrentValue(m_pUnit->getHpRounded());
    connect(pSlider.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        m_pUnit->setHp(value);
    });
    pPanel->addItem(pSlider);
    y += 40;
    if (m_pUnit->getMaxFuel() > 0)
    {
        pLabel = new oxygine::TextField();
        pLabel->setStyle(style);
        pLabel->setText(tr("Fuel: ").toStdString().c_str());
        pLabel->setPosition(10, y);
        pPanel->addItem(pLabel);
        spSlider pSlider = new Slider(pApp->getSettings()->getWidth() - 40 - sliderOffset, 0, m_pUnit->getMaxFuel(), tr("Fuel"));
        pSlider->setPosition(sliderOffset - 160, y);
        pSlider->setCurrentValue(m_pUnit->getFuel());
        connect(pSlider.get(), &Slider::sliderValueChanged, [=](qint32 value)
        {
            m_pUnit->setFuel(value);
        });
        pPanel->addItem(pSlider);
        y += 40;
    }
    if (m_pUnit->getMaxAmmo1() > 0)
    {
        pLabel = new oxygine::TextField();
        pLabel->setStyle(style);
        pLabel->setText(tr("Ammo 1: ").toStdString().c_str());
        pLabel->setPosition(10, y);
        pPanel->addItem(pLabel);
        spSlider pSlider = new Slider(pApp->getSettings()->getWidth() - 40 - sliderOffset, 0, m_pUnit->getMaxAmmo1(), tr("Ammo"));
        pSlider->setPosition(sliderOffset - 160, y);
        pSlider->setCurrentValue(m_pUnit->getAmmo1());
        connect(pSlider.get(), &Slider::sliderValueChanged, [=](qint32 value)
        {
            m_pUnit->setAmmo1(value);
        });
        pPanel->addItem(pSlider);
        y += 40;
    }
    if (m_pUnit->getMaxAmmo2() > 0)
    {
        pLabel = new oxygine::TextField();
        pLabel->setStyle(style);
        pLabel->setText(tr("Ammo 2: ").toStdString().c_str());
        pLabel->setPosition(10, y);
        pPanel->addItem(pLabel);
        spSlider pSlider = new Slider(pApp->getSettings()->getWidth() - 40 - sliderOffset, 0, m_pUnit->getMaxAmmo2(), tr("Ammo"));
        pSlider->setPosition(sliderOffset - 160, y);
        pSlider->setCurrentValue(m_pUnit->getAmmo2());
        connect(pSlider.get(), &Slider::sliderValueChanged, [=](qint32 value)
        {
            m_pUnit->setAmmo2(value);
        });
        pPanel->addItem(pSlider);
        y += 40;
    }

    pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setText(tr("Player: ").toStdString().c_str());
    pLabel->setPosition(10, y);
    pPanel->addItem(pLabel);
    QVector<QString> items;
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        items.append(tr("Player ") + QString::number(i + 1));
    }
    spDropDownmenu pDropdownmenu = new DropDownmenu(200, items);
    pDropdownmenu->setPosition(sliderOffset - 160, y);
    pDropdownmenu->setCurrentItem(m_pUnit->getOwner()->getPlayerID());
    connect(pDropdownmenu.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        m_pUnit->setOwner(pMap->getPlayer(value));

    });
    pPanel->addItem(pDropdownmenu);
    y += 40;

    pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setText(tr("AI-Mode: ").toStdString().c_str());
    pLabel->setPosition(10, y);
    pPanel->addItem(pLabel);
    items = {tr("Normal"), tr("Offensive"), tr("Defensive"), tr("Hold")};
    pDropdownmenu = new DropDownmenu(200, items);
    pDropdownmenu->setPosition(sliderOffset - 160, y);
    pDropdownmenu->setCurrentItem(static_cast<qint32>(m_pUnit->getAiMode()));
    connect(pDropdownmenu.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        m_pUnit->setAiMode(static_cast<GameEnums::GameAi>(value));
    });
    pPanel->addItem(pDropdownmenu);
    y += 40;

    pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setText(tr("Unit Rank: ").toStdString().c_str());
    pLabel->setPosition(10, y);
    pPanel->addItem(pLabel);
    items = {tr("Soldier"), tr("Lieutenant"), tr("General"), tr("Veteran")};
    pDropdownmenu = new DropDownmenu(200, items);
    pDropdownmenu->setPosition(sliderOffset - 160, y);
    pDropdownmenu->setCurrentItem(static_cast<qint32>(m_pUnit->getUnitRank()));
    connect(pDropdownmenu.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        m_pUnit->setUnitRank(static_cast<GameEnums::UnitRanks>(value));
    });
    pPanel->addItem(pDropdownmenu);
    y += 40;

    pPanel->setContentHeigth(y);
}
