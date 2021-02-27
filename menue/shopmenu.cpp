#include "menue/shopmenu.h"
#include "menue/mainwindow.h"

#include "coreengine/console.h"
#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"

#include "wiki/wikidatabase.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/shoploader.h"

#include "objects/base/dropdownmenu.h"
#include "objects/base/checkbox.h"

Shopmenu::Shopmenu()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Shop Menue", Console::eDEBUG);

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("shopmenu");
    sprite->setResAnim(pBackground);
    // background should be last to draw
    sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
    sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
    sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/shop");
    pApp->getAudioThread()->playRandom();

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pButtonBox = new oxygine::Box9Sprite();
    pButtonBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setResAnim(pAnim);
    pButtonBox->setSize(Settings::getWidth(), 60);
    pButtonBox->setY(Settings::getHeight() - 60);
    addChild(pButtonBox);

    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    pButtonExit->attachTo(pButtonBox);
    pButtonExit->setPosition(Settings::getWidth() - pButtonExit->getWidth() - 10,
                             10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &Shopmenu::sigExitMenue, this, &Shopmenu::exitMenue, Qt::QueuedConnection);

    ShopLoader* pShopLoader = ShopLoader::getInstance();
    oxygine::ResAnim* pHachi = pShopLoader->getResAnim("hachi_shop");
    sprite = new oxygine::Sprite();
    sprite->setResAnim(pHachi);
    sprite->setScaleX(Settings::getWidth() * (1.0f / 2.0f) / pHachi->getWidth());
    sprite->setScaleY((Settings::getHeight() - 60) / pHachi->getWidth());
    sprite->setX(Settings::getWidth() - sprite->getScaledWidth());
    addChild(sprite);

    QSize size = QSize(Settings::getWidth() /2, Settings::getHeight() - 110);
    m_pPanel = new Panel(true, size, size);
    m_pPanel->setY(50);
    addChild(m_pPanel);

    const qint32 width = 150;
    spLabel pLabel = new Label(width);
    pLabel->setText(tr("Filter:"));
    pLabel->setPosition(10, 10);
    addChild(pLabel);
    QVector<QString> list = {tr("All"), tr("CO's"), tr("CO Skins"), tr("Maps"), tr("Units"), tr("Perks")};
    spDropDownmenu pDropDownmenu = new DropDownmenu(300, list);
    pDropDownmenu->setPosition(10 + width, + 10);
    addChild(pDropDownmenu);
    connect(pDropDownmenu.get(), &DropDownmenu::sigItemChanged, this, &Shopmenu::filterChanged, Qt::QueuedConnection);
    connect(this, &Shopmenu::sigUpdateItemCosts, this, &Shopmenu::updateItemCosts, Qt::QueuedConnection);

    m_buyButton = ObjectManager::createButton(tr("Buy"));
    m_buyButton->attachTo(pButtonBox);
    m_buyButton->setPosition(Settings::getWidth() - m_buyButton->getWidth() - pButtonExit->getWidth() - 20,
                             10);
    m_buyButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigBuy();
    });
    connect(this, &Shopmenu::sigBuy, this, &Shopmenu::buy, Qt::QueuedConnection);
    connect(this, &Shopmenu::sigShowWikipage, this, &Shopmenu::showWikipage, Qt::QueuedConnection);

    pLabel = new Label(width - 30);
    pLabel->setText(tr("Points:"));
    pLabel->setPosition(10, 10);
    pButtonBox->addChild(pLabel);

    qint32 pointsSize = Settings::getWidth() - 10 * 7 - m_buyButton->getWidth() - pButtonExit->getWidth() - width * 2;
    m_points = new Label(pointsSize / 2);
    m_points->setText(QString::number(Userdata::getInstance()->getCredtis()));
    m_points->setPosition(pLabel->getX() + pLabel->getWidth() + 10, 10);
    pButtonBox->addChild(m_points);

    pLabel = new Label(width + 30);
    pLabel->setText(tr("Total Price:"));
    pLabel->setPosition(m_points->getX() + m_points->getWidth() + 10, 10);
    pButtonBox->addChild(pLabel);
    m_costs = new Label(pointsSize / 2);
    m_costs->setText(QString::number(0));
    m_costs->setPosition(pLabel->getX() + pLabel->getWidth() + 10, 10);
    pButtonBox->addChild(m_costs);

    filterChanged(0);
    pApp->continueRendering();
}

void Shopmenu::exitMenue()
{
    Console::print("Leaving Shop Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new Mainwindow());
    oxygine::Actor::detach();
}

void Shopmenu::filterChanged(qint32 item)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    m_currentFilter = item;
    qint32 labelX = 95;
    qint32 width = 150;
    if (Settings::getWidth() / 2 - 50 - labelX > width)
    {
        width = Settings::getWidth() / 2 - 50 - labelX;
    }
    auto items = getItems(item);
    qint32 y = 10;
    m_pPanel->clearContent();
    m_shoppingList.clear();
    for (const auto & item : items)
    {
        m_shoppingList.append(false);
        spCheckbox pCheckbox = new Checkbox();
        pCheckbox->setPosition(10, y);
        pCheckbox->setTooltipText(tr("Check to but the item on the buy list. Afterwards click buy to confirm your shopping."));
        qint32 costs = item.price;
        qint32 itemPos = m_shoppingList.size() - 1;
        connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool checked)
        {
            m_shoppingList[itemPos] = checked;
            if (checked)
            {
                emit sigUpdateItemCosts(costs);
            }
            else
            {
                emit sigUpdateItemCosts(-costs);
            }
        });
        m_pPanel->addItem(pCheckbox);
        oxygine::spSprite icon = getIcon(item.itemType);
        oxygine::spButton pButton = ObjectManager::createIconButton(icon);
        icon->setPosition(2, 2);
        loadWikiInfo(pButton, item.itemType, item.key);
        pButton->setPosition(55, y);
        m_pPanel->addItem(pButton);
        spLabel pLabel = new Label(width);
        pLabel->setPosition(95, y);
        pLabel->setText(QString::number(costs) + " " + item.name);
        m_pPanel->addItem(pLabel);
        y += 40;
    }
    m_pPanel->setContentHeigth(y + 40);
    m_pPanel->setContentWidth(width + labelX + 50);
    updateItemCosts(-m_pointCosts);
    pApp->continueRendering();
}

QVector<Userdata::ShopItem> Shopmenu::getItems(qint32 itemType)
{
    return Userdata::getInstance()->getItems(static_cast<GameEnums::ShopItemType>(itemType - 1), true, false);
}

void Shopmenu::updateItemCosts(qint32 costChange)
{
    m_pointCosts += costChange;
    m_costs->setText(QString::number(m_pointCosts));
    if (m_pointCosts > Userdata::getInstance()->getCredtis() || m_pointCosts <= 0)
    {
        m_buyButton->setEnabled(false);
    }
    else
    {
        m_buyButton->setEnabled(true);
    }
}

void Shopmenu::buy()
{
    auto items = getItems(m_currentFilter);
    Userdata* pUserdata = Userdata::getInstance();
    ShopLoader* pShopLoader = ShopLoader::getInstance();
    for (qint32 i = 0; i < m_shoppingList.size(); ++i)
    {
        if (m_shoppingList[i])
        {
            GameEnums::ShopItemType item = items[i].itemType;
            QString key = items[i].key;
            pUserdata->setShopItemBought(item, key, true);
            pShopLoader->onItemBought(item, key);
        }
    }
    pUserdata->addCredtis(-m_pointCosts);
    m_points->setText(QString::number(Userdata::getInstance()->getCredtis()));
    pUserdata->storeUser();
    filterChanged(0);
    updateItemCosts(0);
}

oxygine::spSprite Shopmenu::getIcon(GameEnums::ShopItemType itemType)
{
    oxygine::spSprite pRet = nullptr;
    switch (itemType)
    {
        case GameEnums::ShopItemType_CO:
        {
            pRet = WikiDatabase::getInstance()->getIcon("shop_co", 32);
            break;
        }
        case GameEnums::ShopItemType_CO_Skin:
        {
            pRet = WikiDatabase::getInstance()->getIcon("shop_co_color", 32);
            break;
        }
        case GameEnums::ShopItemType_Map:
        {
            pRet = WikiDatabase::getInstance()->getIcon("shop_map", 32);
            break;
        }
        case GameEnums::ShopItemType_Perk:
        {
            pRet = WikiDatabase::getInstance()->getIcon("shop_perk", 32);
            break;
        }
        case GameEnums::ShopItemType_Unit:
        {
            pRet = WikiDatabase::getInstance()->getIcon("INFANTRY", 32);
            break;
        }
        case GameEnums::ShopItemType_All:
        case GameEnums::ShopItemType_Max:
        {
            // do nothing
        }
    }
    static_assert(GameEnums::ShopItemType_Max == 5, "Check switch case");
    return pRet;
}

void Shopmenu::loadWikiInfo(oxygine::spButton pIcon, GameEnums::ShopItemType itemType, QString key)
{
    switch (itemType)
    {
        case GameEnums::ShopItemType_CO:
        case GameEnums::ShopItemType_CO_Skin:
        case GameEnums::ShopItemType_Perk:
        case GameEnums::ShopItemType_Unit:
        {
            pIcon->addClickListener([=](oxygine::Event*)
            {
                emit sigShowWikipage(key);
            });
            break;
        }
        case GameEnums::ShopItemType_All:
        case GameEnums::ShopItemType_Max:
        case GameEnums::ShopItemType_Map:
        {
            pIcon->setEnabled(false);
        }
    }
    static_assert(GameEnums::ShopItemType_Max == 5, "Check switch case");
}

void Shopmenu::showWikipage(QString key)
{
    WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
    oxygine::getStage()->addChild(pWikiDatabase->getPage(pWikiDatabase->getEntry(key)));
}
