#include <QFile>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "coreengine/mainapp.h"
#include "coreengine/gameconsole.h"
#include "coreengine/audiomanager.h"
#include "coreengine/userdata.h"
#include "coreengine/userdata.h"

#include "game/co.h"

#include "menue/mainwindow.h"
#include "menue/costylemenu.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/cospritemanager.h"

#include "objects/dialogs/dialogcostyle.h"
#include "objects/dialogs/ingame/coinfodialog.h"

COStyleMenu::COStyleMenu()
    : Basemenu()
{
#ifdef GRAPHICSUPPORT
    setObjectName("COStyleMenu");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    Interpreter::setCppOwnerShip(this);

    m_pPlayer = spPlayer::create(nullptr);
    m_pPlayer->init();

    CONSOLE_PRINT("Entering Co Style Menue", GameConsole::eDEBUG);
    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = oxygine::spSprite::create();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("costylemenu");
    if (pBackground != nullptr)
    {
        sprite->setResAnim(pBackground);
        // background should be last to draw
        sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
        sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
        sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());
    }

    pApp->getAudioManager()->clearPlayList();
    pApp->getAudioManager()->loadFolder("resources/music/hauptmenue");
    pApp->getAudioManager()->playRandom();

    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    addChild(pButtonExit);
    pButtonExit->setPosition(20, Settings::getHeight() - pButtonExit->getScaledHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &COStyleMenu::sigExitMenue, this, &COStyleMenu::exitMenue, Qt::QueuedConnection);

    // show co info button
    oxygine::spButton pShowCOInfoButton = ObjectManager::createButton(tr("Show CO Info"), 250);
    pShowCOInfoButton->setPosition(Settings::getWidth() / 2 - pShowCOInfoButton->getScaledWidth() / 2,
                                   Settings::getHeight() - pButtonExit->getScaledHeight() - 10);
    pShowCOInfoButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigShowCOInfo();
    });
    connect(this, &COStyleMenu::sigShowCOInfo, this, &COStyleMenu::showCOInfo, Qt::QueuedConnection);
    addChild(pShowCOInfoButton);

    oxygine::spButton pButtonEdit = ObjectManager::createButton(tr("Edit CO"));
    addChild(pButtonEdit);
    pButtonEdit->setPosition(Settings::getWidth() - pButtonEdit->getScaledWidth() - 20,
                             Settings::getHeight() - pButtonExit->getScaledHeight() - 10);
    pButtonEdit->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigEditCOStyle();
    });
    connect(this, &COStyleMenu::sigEditCOStyle, this, &COStyleMenu::editCOStyle, Qt::QueuedConnection);

    Userdata* pUserdata = Userdata::getInstance();
    auto items = pUserdata->getShopItemsList(GameEnums::ShopItemType_CO_Skin, false);
    QStringList bannList = COSpriteManager::getInstance()->getLoadedRessources();
    for (const auto & item : items)
    {
        bannList.removeAll(item);
    }
    m_COSelection = spCOSelection::create(QPoint(0, 0), QSize(Settings::getWidth(), Settings::getHeight() - 50), bannList);
    m_COSelection->colorChanged(QColor(248, 88, 0));
    addChild(m_COSelection);
    connect(m_COSelection.get(), &COSelection::coSelected, this, &COStyleMenu::selectedCOIDChanged, Qt::QueuedConnection);
    pApp->continueRendering();
}

void COStyleMenu::onEnter()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString object = "Init";
    QString func = "coStyleMenu";
    if (pInterpreter->exists(object, func))
    {
        CONSOLE_PRINT("Executing:" + object + "." + func, GameConsole::eDEBUG);
        QJSValueList args({pInterpreter->newQObject(this)});
        pInterpreter->doFunction(object, func, args);
    }
}

void COStyleMenu::exitMenue()
{    
    CONSOLE_PRINT("Leaving CO Style Menue", GameConsole::eDEBUG);
    auto window = spMainwindow::create("ui/menu/playermenu.xml");
    oxygine::Stage::getStage()->addChild(window);
    oxygine::Actor::detach();
}

void COStyleMenu::reloadMenue()
{
    CONSOLE_PRINT("Leaving CO Style Menue", GameConsole::eDEBUG);
    auto window = spCOStyleMenu::create();
    oxygine::Stage::getStage()->addChild(window);
    oxygine::Actor::detach();
}

void COStyleMenu::selectedCOIDChanged(QString coid)
{    
    m_currentCOID = coid;
}

void COStyleMenu::editCOStyle()
{    
    if (!m_currentCOID.isEmpty())
    {
        spDialogCOStyle pDialogCOStyle = spDialogCOStyle::create(m_currentCOID);
        addChild(pDialogCOStyle);
        connect(pDialogCOStyle.get(), &DialogCOStyle::sigFinished, this, &COStyleMenu::reloadMenue, Qt::QueuedConnection);
    }
}

void COStyleMenu::showCOInfo()
{
    QString coid = m_currentCOID;
    spCO co = spCO::create(coid, m_pPlayer.get(), nullptr);

    auto* pPtrPlayer = m_pPlayer.get();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    QStringList inCoids = pCOSpriteManager->getLoadedRessources();
    Userdata* pUserdata = Userdata::getInstance();
    auto items = pUserdata->getShopItemsList(GameEnums::ShopItemType_CO_Skin, false);
    for (const auto & item : items)
    {
        inCoids.removeAll(item);
    }
    QStringList armies = pCOSpriteManager->getArmyList(inCoids);
    QStringList outCoids;
    for (const auto & army : armies)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue ret = pInterpreter->doFunction("PLAYER", "getArmyCOs" + army);
        QStringList preSetCOOrder = ret.toVariant().toStringList();
        for (qint32 i = 0; i < pCOSpriteManager->getCount(); ++i)
        {
            QString coid = pCOSpriteManager->getID(i);
            if ((inCoids.isEmpty() || inCoids.contains(coid)) &&
                !outCoids.contains(coid) && !preSetCOOrder.contains(coid))
            {
                QString function1 = "getCOArmy";
                QJSValue ret = pInterpreter->doFunction(coid, function1);
                if (ret.isString())
                {
                    QString COArmy = ret.toString();
                    if (COArmy == army && !preSetCOOrder.contains(coid))
                    {
                        preSetCOOrder.append(coid);
                        break;
                    }
                }
            }
        }
        outCoids.append(preSetCOOrder);
    }
    outCoids.removeAll(CO::CO_RANDOM);
    outCoids.removeDuplicates();

    addChild(spCOInfoDialog::create(co, m_pPlayer, [pPtrPlayer, outCoids](spCO& pCurrentCO, spPlayer&, qint32 direction)
    {
        qint32 index = outCoids.indexOf(pCurrentCO->getCoID());
        index += direction;
        QString coid;
        if (index < 0)
        {
            coid = outCoids[outCoids.size() - 1];
        }
        else if (index >= outCoids.size())
        {
            coid = outCoids[0];
        }
        else
        {
            coid = outCoids[index];
        }
        pCurrentCO = spCO::create(coid, pPtrPlayer, nullptr);
    }, false));
}
