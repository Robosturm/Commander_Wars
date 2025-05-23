#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/dialogs/editor/dialogmodifyunit.h"

#include "game/unit.h"
#include "game/player.h"
#include "game/co.h"
#include "game/gamemap.h"

#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/unitspritemanager.h"

#include "objects/base/slider.h"
#include "objects/base/dropdownmenu.h"
#include "objects/base/dropdownmenusprite.h"
#include "objects/base/label.h"
#include "objects/base/textbox.h"
#include "objects/base/spinbox.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

DialogModifyUnit::DialogModifyUnit(GameMap* pMap, Unit* pUnit)
    : m_pUnit(pUnit),
      m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DialogModifyUnit");
#endif
    Interpreter::setCppOwnerShip(this);
    m_dropDownPlayer = MemoryManagement::create<Player>(m_pMap);
    m_dropDownPlayer->init();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_OkButton->getScaledWidth() / 2,
                            oxygine::Stage::getStage()->getHeight() - 30 - m_OkButton->getScaledHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigFinished();
    });


    m_pPanel = MemoryManagement::create<Panel>(true, QSize(oxygine::Stage::getStage()->getWidth() - 60, oxygine::Stage::getStage()->getHeight() - 110),
                                     QSize(oxygine::Stage::getStage()->getWidth() - 60, oxygine::Stage::getStage()->getHeight() - 110));
    m_pPanel->setPosition(30, 30);
    pSpriteBox->addChild(m_pPanel);

    connect(this, &DialogModifyUnit::sigUpdateData, this, &DialogModifyUnit::updateData, Qt::QueuedConnection);
    connect(this, &DialogModifyUnit::sigLoadUnit, this, &DialogModifyUnit::loadUnit, Qt::QueuedConnection);
    updateData();
    connect(this, &DialogModifyUnit::sigFinished, this, &DialogModifyUnit::remove, Qt::QueuedConnection);
    connect(this, &DialogModifyUnit::sigShowEditLoadedUnit, this, &DialogModifyUnit::showEditLoadedUnit, Qt::QueuedConnection);
}

void DialogModifyUnit::remove()
{
    detach();
}

void DialogModifyUnit::updateData()
{
    
    m_pPanel->clearContent();

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::TextStyle headerStyle = oxygine::TextStyle(FontManager::getMainFont48());
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;

    qint32 sliderOffset = 400;
    spLabel pLabel = MemoryManagement::create<Label>(m_pPanel->getScaledWidth() - 80);
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText(tr("Edit unit"));
    pLabel->setPosition(m_pPanel->getScaledWidth() / 2 - pLabel->getTextRect().width() / 2, 10);
    m_pPanel->addItem(pLabel);

    qint32 y = 30 + pLabel->getTextRect().height();
    pLabel = MemoryManagement::create<Label>(sliderOffset - 140);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Custom name: "));
    pLabel->setPosition(10, y);
    m_pPanel->addItem(pLabel);
    spTextbox pTexbox = MemoryManagement::create<Textbox>(oxygine::Stage::getStage()->getWidth() - 40 - sliderOffset);
    pTexbox->setPosition(sliderOffset - 160, y);
    pTexbox->setCurrentText(m_pUnit->getName());
    pTexbox->setTooltipText(tr("Selects the custom name of the unit shown instead of the actual unit name. An empty name will display the default name for that unit."));
    connect(pTexbox.get(), &Textbox::sigTextChanged, this, [this](QString value)
    {
        m_pUnit->setCustomName(value);
    });
    m_pPanel->addItem(pTexbox);
    y += pLabel->getHeight() + 10;

    pLabel = MemoryManagement::create<Label>(sliderOffset - 140);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("HP: "));
    pLabel->setPosition(10, y);
    m_pPanel->addItem(pLabel);
    spSlider pSlider = MemoryManagement::create<Slider>(oxygine::Stage::getStage()->getWidth() - 40 - sliderOffset, 1, 10, tr("HP"));
    pSlider->setTooltipText(tr("Selects the HP of the current unit. This is immediately applied."));
    pSlider->setPosition(sliderOffset - 160, y);
    pSlider->setCurrentValue(m_pUnit->getHpRounded());
    connect(pSlider.get(), &Slider::sliderValueChanged, this, [this](qint32 value)
    {
        m_pUnit->setHp(value);
    });
    m_pPanel->addItem(pSlider);
    y += pLabel->getHeight() + 10;
    if (m_pUnit->getMaxFuel() > 0)
    {
        pLabel = MemoryManagement::create<Label>(sliderOffset - 140);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Fuel: "));
        pLabel->setPosition(10, y);
        m_pPanel->addItem(pLabel);
        spSlider pSlider = MemoryManagement::create<Slider>(oxygine::Stage::getStage()->getWidth() - 40 - sliderOffset, 0, m_pUnit->getMaxFuel(), tr("Fuel"));
        pSlider->setTooltipText(tr("Selects the Fuel of the current unit. This is immediately applied."));
        pSlider->setPosition(sliderOffset - 160, y);
        pSlider->setCurrentValue(m_pUnit->getFuel());
        connect(pSlider.get(), &Slider::sliderValueChanged, this, [this](qint32 value)
        {
            m_pUnit->setFuel(value);
            if (value == m_pUnit->getMaxFuel())
            {
                m_pUnit->setModdingFlags(static_cast<Unit::ModdingFlags>(m_pUnit->getModdingFlags() & ~Unit::ModdingFlags::FlagFuel));
            }
            else
            {
                m_pUnit->setModdingFlags(static_cast<Unit::ModdingFlags>(m_pUnit->getModdingFlags() | Unit::ModdingFlags::FlagFuel));
            }
        });
        m_pPanel->addItem(pSlider);
        y += pLabel->getHeight() + 10;
    }
    if (m_pUnit->getMaxAmmo1() > 0)
    {
        pLabel = MemoryManagement::create<Label>(sliderOffset - 140);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Ammo 1: "));
        pLabel->setPosition(10, y);
        m_pPanel->addItem(pLabel);
        spSlider pSlider = MemoryManagement::create<Slider>(oxygine::Stage::getStage()->getWidth() - 40 - sliderOffset, 0, m_pUnit->getMaxAmmo1(), tr("Ammo"));
        pSlider->setTooltipText(tr("Selects the Ammo 1 of the current unit. This is immediately applied."));
        pSlider->setPosition(sliderOffset - 160, y);
        pSlider->setCurrentValue(m_pUnit->getAmmo1());
        connect(pSlider.get(), &Slider::sliderValueChanged, this, [this](qint32 value)
        {
            m_pUnit->setAmmo1(value);
            if (value == m_pUnit->getMaxAmmo1())
            {
                m_pUnit->setModdingFlags(static_cast<Unit::ModdingFlags>(m_pUnit->getModdingFlags() & ~Unit::ModdingFlags::FlagAmmo1));
            }
            else
            {
                m_pUnit->setModdingFlags(static_cast<Unit::ModdingFlags>(m_pUnit->getModdingFlags() | Unit::ModdingFlags::FlagAmmo1));
            }
        });
        m_pPanel->addItem(pSlider);
        y += pLabel->getHeight() + 10;
    }
    if (m_pUnit->getMaxAmmo2() > 0)
    {
        pLabel = MemoryManagement::create<Label>(sliderOffset - 140);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Ammo 2: "));
        pLabel->setPosition(10, y);
        m_pPanel->addItem(pLabel);
        spSlider pSlider = MemoryManagement::create<Slider>(oxygine::Stage::getStage()->getWidth() - 40 - sliderOffset, 0, m_pUnit->getMaxAmmo2(), tr("Ammo"));
        pSlider->setTooltipText(tr("Selects the Ammo 2 of the current unit. This is immediately applied."));
        pSlider->setPosition(sliderOffset - 160, y);
        pSlider->setCurrentValue(m_pUnit->getAmmo2());
        connect(pSlider.get(), &Slider::sliderValueChanged, this, [this](qint32 value)
        {
            m_pUnit->setAmmo2(value);
            if (value == m_pUnit->getMaxAmmo2())
            {
                m_pUnit->setModdingFlags(static_cast<Unit::ModdingFlags>(m_pUnit->getModdingFlags() & ~Unit::ModdingFlags::FlagAmmo2));
            }
            else
            {
                m_pUnit->setModdingFlags(static_cast<Unit::ModdingFlags>(m_pUnit->getModdingFlags() | Unit::ModdingFlags::FlagAmmo2));
            }
        });
        m_pPanel->addItem(pSlider);
        y += pLabel->getHeight() + 10;
    }

    pLabel = MemoryManagement::create<Label>(sliderOffset - 140);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Player: "));
    pLabel->setPosition(10, y);
    m_pPanel->addItem(pLabel);
    QStringList items;
    
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        items.append(tr("Player ") + QString::number(i + 1));
    }
    spDropDownmenu pDropdownmenu = MemoryManagement::create<DropDownmenu>(300, items);
    pDropdownmenu->setTooltipText(tr("Selects the Owner of the current unit. This is immediately applied."));
    pDropdownmenu->setPosition(sliderOffset - 160, y);
    pDropdownmenu->setCurrentItem(m_pUnit->getOwner()->getPlayerID());
    connect(pDropdownmenu.get(), &DropDownmenu::sigItemChanged, this, [this](qint32 value)
    {
        m_pUnit->setOwner(m_pMap->getPlayer(value));

    });
    m_pPanel->addItem(pDropdownmenu);
    y += pLabel->getHeight() + 10;

    pLabel = MemoryManagement::create<Label>(sliderOffset - 140);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("AI-Mode: "));
    pLabel->setPosition(10, y);
    m_pPanel->addItem(pLabel);
    items = {tr("Normal"),
             tr("Offensive"),
             tr("Defensive"),
             tr("Hold"),
             tr("Patrol"),
             tr("Patrol Loop"),
             tr("Target Enemy HQ"),
             tr("Scripted")};
    pDropdownmenu = MemoryManagement::create<DropDownmenu>(300, items);
    pDropdownmenu->setTooltipText(tr("Selects how the AI uses this unit\n"
                                     "Normal AI uses the unit like always.\n"
                                     "Offensive AI the ai moves the unit to the closest enemy and attacks\n"
                                     "Defensive AI the ai moves the unit only if it can attack.\n"
                                     "Hold AI the ai only attacks but never moves with this unit.\n"
                                     "Patrol the unit will move to each position in the given order\n"
                                     "Patrol Loop the unit will move to each position in the given order and restart at the first\n"
                                     "Target Enemy HQ moves to the nearest enemy HQ\n"
                                     "Scripted you need to provide a game script containing the code for what this unit does\n"
                                     "This is immediately applied."));
    pDropdownmenu->setPosition(sliderOffset - 160, y);
    pDropdownmenu->setCurrentItem(static_cast<qint32>(m_pUnit->getAiMode()));
    connect(pDropdownmenu.get(), &DropDownmenu::sigItemChanged, this, [this](qint32 value)
    {
        m_pUnit->setAiMode(static_cast<GameEnums::GameAi>(value));
        emit sigUpdateData();
    });
    m_pPanel->addItem(pDropdownmenu);
    y += pLabel->getHeight() + 10;
    addLoadLoopPoints(y, sliderOffset);

    pLabel = MemoryManagement::create<Label>(sliderOffset - 140);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Unit Rank: "));
    pLabel->setPosition(10, y);
    m_pPanel->addItem(pLabel);
    QStringList rankItems;
    qint32 maxRang = m_pUnit->getMaxUnitRang();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getIcon";
    for (qint32 i = GameEnums::UnitRank_CO1; i <= maxRang; i++)
    {
        QJSValueList args({i,
                           GameMap::getMapJsThis(m_pMap)});
        QJSValue ret = pInterpreter->doFunction("UNITRANKINGSYSTEM", function1, args);
        rankItems.append(ret.toString());
    }
    auto unitRankCreator = [=](QString id)
    {
        oxygine::ResAnim* pAnim = nullptr;
        if (id.isEmpty())
        {
            pAnim = UnitSpriteManager::getInstance()->getResAnim("no_rank");
        }
        else
        {
            pAnim = UnitSpriteManager::getInstance()->getResAnim(id);
        }
        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
        if (pAnim != nullptr)
        {
            pSprite->setResAnim(pAnim);
            pSprite->setScale(static_cast<float>(pAnim->getWidth()) / 30.0f);
            pSprite->setSize(pAnim->getSize());
        }
        return pSprite;
    };
    spDropDownmenuSprite pDropdownmenuSprite = MemoryManagement::create<DropDownmenuSprite>(105, rankItems, unitRankCreator, 30);
    pDropdownmenuSprite->setTooltipText(tr("Selects the Rank of this Unit. CO Ranks may be replaced with highest rank. This is immediately applied."));
    pDropdownmenuSprite->setPosition(sliderOffset - 160, y);
    pDropdownmenuSprite->setCurrentItem(static_cast<qint32>(m_pUnit->getUnitRank() - GameEnums::UnitRank_CO1));
    qint32 size = rankItems.size();
    connect(pDropdownmenuSprite.get(), &DropDownmenuSprite::sigItemChanged, this, [this, size](qint32 value)
    {
        m_pUnit->setUnitRank(value + GameEnums::UnitRank_CO1, true);
    });
    m_pPanel->addItem(pDropdownmenuSprite);
    y += pLabel->getHeight() + 10;

    for (qint32 i = 0; i < m_pUnit->getLoadedUnitCount(); i++)
    {
        addLoadUnit(i, sliderOffset, y);
    }
    if (m_pUnit->getLoadedUnitCount() < m_pUnit->getLoadingPlace())
    {
        addLoadUnit(m_pUnit->getLoadedUnitCount(), sliderOffset, y);
    }
    m_pPanel->setContentHeigth(y);
    
}

void DialogModifyUnit::addLoadUnit(qint32 index, qint32 sliderOffset, qint32& y)
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    spLabel  pLabel = MemoryManagement::create<Label>(sliderOffset - 140);
    pLabel->setStyle(style);
    pLabel->setHtmlText((tr("Loaded Unit ") + QString::number(index + 1) + ": "));
    pLabel->setPosition(10, y);
    m_pPanel->addItem(pLabel);
    QStringList items = {"-"};
    QStringList units = m_pUnit->getTransportUnits();
    for (qint32 i = 0; i < units.size(); i++)
    {
        if (pUnitSpriteManager->exists(units[i]))
        {
            items.append(units[i]);
        }
    }
    Player* pPlayer = m_dropDownPlayer.get();
    auto unitCreator = [this, pPlayer](QString id)
    {
        oxygine::spActor ret;
        if (id != "-")
        {
            spUnit pSprite = MemoryManagement::create<Unit>(id, pPlayer, false, m_pMap);
            pSprite->setOwner(nullptr);
            ret = pSprite;
        }
        else
        {
            spLabel pText = MemoryManagement::create<Label>(30);
            pText->setHtmlText("-");
            ret = pText;
        }
        return ret;
    };
    spDropDownmenuSprite pDropdownmenu = MemoryManagement::create<DropDownmenuSprite>(105, items, unitCreator, 30);
    pDropdownmenu->setPosition(sliderOffset - 160, y);
    pDropdownmenu->setTooltipText(tr("Selects the unit loaded by the transporter. Enter '-' for no unit. This is immediately applied."));
    Unit* pLoadedUnit = m_pUnit->getLoadedUnit(index);
    if (pLoadedUnit != nullptr)
    {
        pDropdownmenu->setCurrentItem(pLoadedUnit->getUnitID());
        oxygine::spButton pButton = ObjectManager::createButton(tr("Edit unit"), 200, "Edit this loaded unit", "button");
        pButton->addClickListener([this, pLoadedUnit](oxygine::Event*)
        {
            emit sigShowEditLoadedUnit(m_pMap, pLoadedUnit);
        });
        pButton->setY(pDropdownmenu->getY());
        pButton->setX(pDropdownmenu->getX() + pDropdownmenu->getWidth() + 10);
        m_pPanel->addItem(pButton);
    }
    else
    {
        pDropdownmenu->setCurrentItem("-");
    }
    auto* pPtrDropdownmenu = pDropdownmenu.get();
    connect(pDropdownmenu.get(), &DropDownmenuSprite::sigItemChanged, this, [this, pPtrDropdownmenu, index](qint32)
    {
        emit sigLoadUnit(pPtrDropdownmenu->getCurrentItemText(), index);
    });
    m_pPanel->addItem(pDropdownmenu);
    y += pLabel->getHeight() + 10;
}

void DialogModifyUnit::loadUnit(QString unitID, qint32 index)
{
    
    if (unitID == "-")
    {
        m_pUnit->unloadUnitAtIndex(index, QPoint(-1, -1));
    }
    else
    {
        spUnit pUnit = MemoryManagement::create<Unit>(unitID, m_pUnit->getOwner(), false, m_pMap);
        m_pUnit->loadUnit(pUnit.get(), index);
    }
    emit sigUpdateData();
    
}

void DialogModifyUnit::addLoadLoopPoints(qint32& y, qint32 sliderOffset)
{
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    if (m_pUnit->getAiMode() == GameEnums::GameAi_Patrol ||
        m_pUnit->getAiMode() == GameEnums::GameAi_PatrolLoop)
    {
        spLabel pLabel = MemoryManagement::create<Label>(sliderOffset - 140);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Move Point: "));
        pLabel->setPosition(10, y);
        m_pPanel->addItem(pLabel);
        auto points = m_pUnit->getAiMovePath();
        
        for (qint32 i = 0; i < points.size(); i++)
        {
            spSpinBox pSpinbox = MemoryManagement::create<SpinBox>(200, 0, m_pMap->getMapWidth() - 1);
            pSpinbox->setCurrentValue(points[i].x());
            pSpinbox->setTooltipText("X-Coordinate for the move path.");
            pSpinbox->setPosition(sliderOffset - 160, y);
            m_pPanel->addItem(pSpinbox);
            connect(pSpinbox.get(), &SpinBox::sigValueChanged, this, [this, i](qreal value)
            {
                auto points = m_pUnit->getAiMovePath();
                points[i].setX(value);
                m_pUnit->setAiMovePathPoint(i, points[i]);
            });
            pSpinbox = MemoryManagement::create<SpinBox>(200, 0, m_pMap->getMapHeight() - 1);
            pSpinbox->setCurrentValue(points[i].y());
            pSpinbox->setTooltipText("Y-Coordinate for the move path.");
            pSpinbox->setPosition(sliderOffset + 50, y);
            connect(pSpinbox.get(), &SpinBox::sigValueChanged, this, [this, i](qreal value)
            {
                auto points = m_pUnit->getAiMovePath();
                points[i].setY(value);
                m_pUnit->setAiMovePathPoint(i, points[i]);
            });
            m_pPanel->addItem(pSpinbox);
            y += pLabel->getHeight() + 10;
        }
        ObjectManager* pObjectManager = ObjectManager::getInstance();
        oxygine::spButton pButton = pObjectManager->createButton(tr("Add Point"), 150);
        pButton->setPosition(sliderOffset - 160, y + 10);
        m_pPanel->addItem(pButton);
        pButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
        {
            m_pUnit->addAiMovePathPoint(QPoint(0, 0));
            emit sigUpdateData();
        });
        pButton = pObjectManager->createButton(tr("Remove last Point"), 150);
        pButton->setPosition(sliderOffset, y + 10);
        m_pPanel->addItem(pButton);
        pButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
        {
            m_pUnit->removeLastAiMovePathPoint();
            emit sigUpdateData();
        });
        y += pLabel->getHeight() + 20;
    }
    else
    {
        m_pUnit->setAiMovePath(QVector<QPoint>());
    }
}

void DialogModifyUnit::showEditLoadedUnit(GameMap* pMap, Unit* pUnit)
{
    spDialogModifyUnit pDialog = MemoryManagement::create<DialogModifyUnit>(pMap, pUnit);
    addChild(pDialog);
}
