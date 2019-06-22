#include "mapselectionmapsmenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/cospritemanager.h"


#include "game/gamemap.h"

#include "game/campaign.h"

#include "game/player.h"

#include "game/co.h"

#include "menue/mainwindow.h"
#include "menue/gamemenue.h"
#include "menue/campaignmenu.h"
#include "multiplayer/multiplayermenu.h"

#include "objects/ruleselection.h"

MapSelectionMapsMenue::MapSelectionMapsMenue(qint32 heigth, spMapSelectionView pMapSelectionView)
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    Console::print("Entering Map Selection Menue", Console::eDEBUG);
    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
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

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/mapselection");
    pApp->getAudioThread()->playRandom();

    if (pMapSelectionView.get() == nullptr)
    {
        m_pMapSelectionView = new MapSelectionView();
    }
    else
    {
        m_pMapSelectionView = pMapSelectionView;
    }
    addChild(m_pMapSelectionView);

    connect(m_pMapSelectionView->getMapSelection(), &MapSelection::itemChanged, this, &MapSelectionMapsMenue::mapSelectionItemChanged, Qt::QueuedConnection);
    connect(m_pMapSelectionView->getMapSelection(), &MapSelection::itemClicked, this, &MapSelectionMapsMenue::mapSelectionItemClicked, Qt::QueuedConnection);

    m_pButtonBack = ObjectManager::createButton(tr("Back"));
    m_pButtonBack->setPosition(10, pApp->getSettings()->getHeight() - 10 - m_pButtonBack->getHeight());
    m_pButtonBack->attachTo(this);
    m_pButtonBack->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonBack();
    });
    connect(this, &MapSelectionMapsMenue::buttonBack, this, &MapSelectionMapsMenue::slotButtonBack, Qt::QueuedConnection);

    m_pButtonNext = ObjectManager::createButton(tr("Next"));
    m_pButtonNext->setPosition(pApp->getSettings()->getWidth() - 10 - m_pButtonNext->getWidth(), pApp->getSettings()->getHeight() - 10 - m_pButtonNext->getHeight());
    m_pButtonNext->attachTo(this);
    m_pButtonNext->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonNext();
    });
    connect(this, &MapSelectionMapsMenue::buttonNext, this, &MapSelectionMapsMenue::slotButtonNext, Qt::QueuedConnection);

    m_pButtonStart = ObjectManager::createButton(tr("Start Game"));
    m_pButtonStart->setPosition(pApp->getSettings()->getWidth() - 10 - m_pButtonStart->getWidth(), pApp->getSettings()->getHeight() - 10 - m_pButtonStart->getHeight());
    m_pButtonStart->attachTo(this);
    m_pButtonStart->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonStartGame();
    });
    connect(this, &MapSelectionMapsMenue::buttonStartGame, this, &MapSelectionMapsMenue::startGame, Qt::QueuedConnection);

    qint32 yPos = 10;
    if (heigth <  0)
    {
        heigth = pApp->getSettings()->getHeight() - 40 * 2;
    }
    m_pPlayerSelection = new PlayerSelection(pApp->getSettings()->getWidth() - 20,
                                             heigth);
    m_pPlayerSelection->setPosition(10, yPos);
    addChild(m_pPlayerSelection);

    QSize size(pApp->getSettings()->getWidth() - 20, pApp->getSettings()->getHeight() - 40 * 2);
    m_pRuleSelection = new  Panel(true,  size, size);
    m_pRuleSelection->setPosition(10, 20);
    addChild(m_pRuleSelection);
    if (m_pMapSelectionView->getCurrentCampaign().get() == nullptr)
    {
        hidePlayerSelection();
        hideRuleSelection();
    }
    else
    {
        hideMapSelection();
        hideRuleSelection();
        m_pPlayerSelection->attachCampaign(m_pMapSelectionView->getCurrentCampaign());
        showPlayerSelection();
        m_MapSelectionStep = MapSelectionStep::selectPlayer;
    }
}

MapSelectionMapsMenue::~MapSelectionMapsMenue()
{

}

void MapSelectionMapsMenue::slotButtonBack()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
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
            hideRuleSelection();
            showMapSelection();
            m_MapSelectionStep = MapSelectionStep::selectMap;
            break;
        }
        case MapSelectionStep::selectPlayer:
        {
            if (m_pMapSelectionView->getCurrentCampaign().get() == nullptr)
            {
                showRuleSelection();
                hidePlayerSelection();
                m_MapSelectionStep = MapSelectionStep::selectRules;
            }
            else
            {
                Console::print("Leaving Map Selection Menue", Console::eDEBUG);
                if (dynamic_cast<Multiplayermenu*>(this) != nullptr)
                {
                    oxygine::getStage()->addChild(new CampaignMenu(m_pMapSelectionView->getCurrentCampaign(), true));
                }
                else
                {
                    oxygine::getStage()->addChild(new CampaignMenu(m_pMapSelectionView->getCurrentCampaign(), false));
                }
                oxygine::Actor::detach();
            }
            break;
        }
    }
    pApp->continueThread();
}

void MapSelectionMapsMenue::slotButtonNext()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    switch (m_MapSelectionStep)
    {
        case MapSelectionStep::selectMap:
        {
            QString file = m_pMapSelectionView->getMapSelection()->getCurrentFile();
            if (m_pMapSelectionView->getCurrentMap() != nullptr && file.endsWith(".map"))
            {
                m_pMapSelectionView->setCurrentCampaign(nullptr);
                if (m_pMapSelectionView->getCurrentMap()->getGameScript()->immediateStart())
                {
                    startGame();
                }
                else
                {
                    hideMapSelection();
                    showRuleSelection();
                    m_MapSelectionStep = MapSelectionStep::selectRules;
                }
            }
            else
            {
                if (file.endsWith(".jsm"))
                {
                    Console::print("Leaving Map Selection Menue", Console::eDEBUG);
                    if (dynamic_cast<Multiplayermenu*>(this) != nullptr)
                    {
                        oxygine::getStage()->addChild(new CampaignMenu(m_pMapSelectionView->getCurrentCampaign(), true));
                    }
                    else
                    {
                        oxygine::getStage()->addChild(new CampaignMenu(m_pMapSelectionView->getCurrentCampaign(), false));
                    }
                    oxygine::Actor::detach();
                }
            }
            break;
        }
        case MapSelectionStep::selectRules:
        {
            hideRuleSelection();
            showPlayerSelection();
            m_MapSelectionStep = MapSelectionStep::selectPlayer;
            break;
        }
        case MapSelectionStep::selectPlayer:
        {
            break;
        }
    }
    pApp->continueThread();
}

void MapSelectionMapsMenue::mapSelectionItemClicked(QString item)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    QFileInfo info = m_pMapSelectionView->getMapSelection()->getCurrentFolder() + item;
    if (info.isFile())
    {
        emit buttonNext();
    }
    pApp->continueThread();
}

void MapSelectionMapsMenue::mapSelectionItemChanged(QString item)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    QFileInfo info = m_pMapSelectionView->getMapSelection()->getCurrentFolder() + item;
    m_pMapSelectionView->loadMap(info);
    pApp->continueThread();
}

void MapSelectionMapsMenue::hideMapSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pMapSelectionView->setVisible(false);
    pApp->continueThread();
}

void MapSelectionMapsMenue::showMapSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pMapSelectionView->setVisible(true);
    pApp->continueThread();
}

void MapSelectionMapsMenue::hideRuleSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pRuleSelection->setVisible(false);
    pApp->continueThread();
}

void MapSelectionMapsMenue::showRuleSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pRuleSelection->setVisible(true);
    m_pRuleSelection->clearContent();
    spRuleSelection pRuleSelection = new RuleSelection(pApp->getSettings()->getWidth() - 80);
    m_pRuleSelection->addItem(pRuleSelection);
    m_pRuleSelection->setContentHeigth(pRuleSelection->getHeight() + 40);
    pApp->continueThread();
}

void MapSelectionMapsMenue::showPlayerSelection()
{
    m_pButtonStart->setVisible(true);
    m_pButtonNext->setVisible(false);
    m_pPlayerSelection->setVisible(true);
    m_pPlayerSelection->showPlayerSelection();
}

void MapSelectionMapsMenue::hidePlayerSelection()
{
    m_pButtonStart->setVisible(false);
    m_pButtonNext->setVisible(true);
    m_pPlayerSelection->setVisible(false);
}

void MapSelectionMapsMenue::startGame()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    pMap->initPlayers();
    pMap->setCampaign(m_pMapSelectionView->getCurrentCampaign());
    pMap->getGameScript()->gameStart();
    pMap->updateSprites();
    // start game
    Console::print("Leaving Map Selection Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new GameMenue());
    oxygine::Actor::detach();
    pApp->continueThread();
}
