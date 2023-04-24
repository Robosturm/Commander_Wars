#include "menue/shopmenu.h"
#include "menue/mainwindow.h"

#include "coreengine/gameconsole.h"
#include "coreengine/mainapp.h"
#include "coreengine/audiomanager.h"

#include "wiki/wikidatabase.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/shoploader.h"

#include "objects/base/dropdownmenu.h"
#include "objects/base/checkbox.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

Shopmenu::Shopmenu()
{
#ifdef GRAPHICSUPPORT
    setObjectName("Shopmenu");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    Interpreter::setCppOwnerShip(this);
    CONSOLE_PRINT("Entering Shop Menue", GameConsole::eDEBUG);

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = oxygine::spSprite::create();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("shopmenu");
    if (pBackground != nullptr)
    {
        sprite->setResAnim(pBackground);
        // background should be last to draw
        sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
        sprite->setScaleX(static_cast<float>(oxygine::Stage::getStage()->getWidth()) / static_cast<float>(pBackground->getWidth()));
        sprite->setScaleY(static_cast<float>(oxygine::Stage::getStage()->getHeight()) / static_cast<float>(pBackground->getHeight()));
    }

    pApp->getAudioManager()->clearPlayList();
    pApp->getAudioManager()->loadFolder("resources/music/shop");
    pApp->getAudioManager()->playRandom();

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pButtonBox = oxygine::spBox9Sprite::create();
    pButtonBox->setResAnim(pAnim);
    pButtonBox->setSize(oxygine::Stage::getStage()->getWidth(), 60);
    pButtonBox->setY(oxygine::Stage::getStage()->getHeight() - 60);
    addChild(pButtonBox);

    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    pButtonBox->addChild(pButtonExit);
    pButtonExit->setPosition(oxygine::Stage::getStage()->getWidth() - pButtonExit->getScaledWidth() - 10,
                             10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &Shopmenu::sigExitMenue, this, &Shopmenu::exitMenue, Qt::QueuedConnection);

    ShopLoader* pShopLoader = ShopLoader::getInstance();
    oxygine::ResAnim* pHachi = pShopLoader->getResAnim("hachi_shop");
    sprite = oxygine::spSprite::create();
    sprite->setResAnim(pHachi);
    sprite->setScaleX(static_cast<float>(oxygine::Stage::getStage()->getWidth() * 0.5f) / static_cast<float>(pHachi->getWidth()));
    sprite->setScaleY(static_cast<float>(oxygine::Stage::getStage()->getHeight() - 60) / static_cast<float>(pHachi->getWidth()));
    sprite->setX(oxygine::Stage::getStage()->getWidth() - sprite->getScaledWidth());
    addChild(sprite);

    QSize size = QSize(oxygine::Stage::getStage()->getWidth() /2, oxygine::Stage::getStage()->getHeight() - 110);
    m_pPanel = spPanel::create(true, size, size);
    m_pPanel->setY(50);
    addChild(m_pPanel);

    const qint32 width = 150;
    spLabel pLabel = spLabel::create(width, true);
    pLabel->setHtmlText(tr("Filter:"));
    pLabel->setPosition(10, 10);
    addChild(pLabel);
    QVector<QString> list = {tr("All"), tr("CO's"), tr("CO Skins"), tr("Maps"), tr("Units"), tr("Perks")};
    spDropDownmenu pDropDownmenu = spDropDownmenu::create(300, list);
    pDropDownmenu->setPosition(10 + width, + 10);
    addChild(pDropDownmenu);
    connect(pDropDownmenu.get(), &DropDownmenu::sigItemChanged, this, &Shopmenu::filterChanged, Qt::QueuedConnection);
    connect(this, &Shopmenu::sigUpdateItemCosts, this, &Shopmenu::updateItemCosts, Qt::QueuedConnection);

    m_buyButton = ObjectManager::createButton(tr("Buy"));
    pButtonBox->addChild(m_buyButton);
    m_buyButton->setPosition(oxygine::Stage::getStage()->getWidth() - m_buyButton->getScaledWidth() - pButtonExit->getScaledWidth() - 20,
                             10);
    m_buyButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigBuy();
    });
    connect(this, &Shopmenu::sigBuy, this, &Shopmenu::buy, Qt::QueuedConnection);
    connect(this, &Shopmenu::sigShowWikipage, this, &Shopmenu::showWikipage, Qt::QueuedConnection);

    pLabel = spLabel::create(width - 30);
    pLabel->setHtmlText(tr("Points:"));
    pLabel->setPosition(10, 10);
    pButtonBox->addChild(pLabel);

    qint32 pointsSize = oxygine::Stage::getStage()->getWidth() - 10 * 7 - m_buyButton->getScaledWidth() - pButtonExit->getScaledWidth() - width * 2;
    m_points = spLabel::create(pointsSize / 2);
    m_points->setHtmlText(QString::number(Userdata::getInstance()->getCredtis()));
    m_points->setPosition(pLabel->getX() + pLabel->getScaledWidth() + 10, 10);
    pButtonBox->addChild(m_points);

    pLabel = spLabel::create(width + 30);
    pLabel->setHtmlText(tr("Total Price:"));
    pLabel->setPosition(m_points->getX() + m_points->getScaledWidth() + 10, 10);
    pButtonBox->addChild(pLabel);
    m_costs = spLabel::create(pointsSize / 2);
    m_costs->setHtmlText(QString::number(0));
    m_costs->setPosition(pLabel->getX() + pLabel->getScaledWidth() + 10, 10);
    pButtonBox->addChild(m_costs);

    filterChanged(0);
    pApp->continueRendering();
}

void Shopmenu::onEnter()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString object = "Init";
    QString func = "shopMenu";
    if (pInterpreter->exists(object, func))
    {
        CONSOLE_PRINT("Executing:" + object + "." + func, GameConsole::eDEBUG);
        QJSValueList args({pInterpreter->newQObject(this)});
        pInterpreter->doFunction(object, func, args);
    }
}

void Shopmenu::exitMenue()
{
    CONSOLE_PRINT("Leaving Shop Menue", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    auto window = spMainwindow::create("ui/menu/playermenu.xml");
    oxygine::Stage::getStage()->addChild(window);
    oxygine::Actor::detach();
}

void Shopmenu::filterChanged(qint32 item)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    m_currentFilter = item;
    qint32 labelX = 95;
    qint32 width = 150;
    if (oxygine::Stage::getStage()->getWidth() / 2 - 50 - labelX > width)
    {
        width = oxygine::Stage::getStage()->getWidth() / 2 - 50 - labelX;
    }
    auto items = getItems(item);
    qint32 y = 10;
    m_pPanel->clearContent();
    m_shoppingList.clear();
    for (const auto & item : items)
    {
        m_shoppingList.append(false);
        spCheckbox pCheckbox = spCheckbox::create();
        pCheckbox->setPosition(10, y);
        pCheckbox->setTooltipText(tr("Check any items you'd like to buy from the shop, then click 'Buy' to confirm your purchase."));
        qint32 costs = item.price;
        qint32 itemPos = m_shoppingList.size() - 1;
        connect(pCheckbox.get(), &Checkbox::checkChanged, this, [this, itemPos, costs](bool checked)
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
        oxygine::spSprite icon = getIcon(item.itemType, item.key);
        oxygine::spButton pButton = ObjectManager::createIconButton(icon, 36);
        icon->setPosition(2, 2);
        loadWikiInfo(pButton, item.itemType, item.key);
        pButton->setPosition(55, y);
        m_pPanel->addItem(pButton);
        spLabel pLabel = spLabel::create(width);
        pLabel->setPosition(95, y);
        pLabel->setHtmlText(QString::number(costs) + " " + item.name);
        m_pPanel->addItem(pLabel);
        y += pLabel->getHeight() + 10;
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
    m_costs->setHtmlText(QString::number(m_pointCosts));
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
    m_points->setHtmlText(QString::number(Userdata::getInstance()->getCredtis()));
    pUserdata->storeUser();
    filterChanged(0);
    updateItemCosts(0);
}

oxygine::spSprite Shopmenu::getIcon(GameEnums::ShopItemType itemType, QString key)
{
    oxygine::spSprite pRet;
    switch (itemType)
    {
        case GameEnums::ShopItemType_CO:
        {
            pRet = WikiDatabase::getInstance()->getIcon(nullptr, "shop_co", 32);
            break;
        }
        case GameEnums::ShopItemType_CO_Skin:
        {
            pRet = WikiDatabase::getInstance()->getIcon(nullptr, "shop_co_color", 32);
            break;
        }
        case GameEnums::ShopItemType_Map:
        {
            pRet = WikiDatabase::getInstance()->getIcon(nullptr, "shop_map", 32);
            break;
        }
        case GameEnums::ShopItemType_Perk:
        {
            pRet = WikiDatabase::getInstance()->getIcon(nullptr, "shop_perk", 32);
            break;
        }
        case GameEnums::ShopItemType_Unit:
        {
            pRet = WikiDatabase::getInstance()->getIcon(nullptr, key, 32);
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
            pIcon->addClickListener([this, key](oxygine::Event*)
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
    oxygine::Stage::getStage()->addChild(pWikiDatabase->getPage(pWikiDatabase->getEntry(key)));
}
