#include "campaignmenu.h"
#include "menue/mainwindow.h"

#include "menue/mapselectionmapsmenue.h"
#include "menue/gamemenue.h"
#include "multiplayer/multiplayermenu.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"
#include "coreengine/audiothread.h"

#include "game/gamemap.h"
#include "game/gamescript.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"

CampaignMenu::CampaignMenu(spCampaign campaign, bool multiplayer)
    : QObject(),
      m_Multiplayer(multiplayer)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Campaign Menue", Console::eDEBUG);

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("Background+1");
    sprite->setResAnim(pBackground);
    sprite->setPosition(0, 0);
    // background should be last to draw
    sprite->setPriority(static_cast<short>(Mainapp::ZOrder::Background));
    sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
    sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/mapselection");
    pApp->getAudioThread()->playRandom();

    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    pButtonExit->attachTo(this);
    pButtonExit->setPosition(10,
                             Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &CampaignMenu::sigExitMenue, this, &CampaignMenu::exitMenue, Qt::QueuedConnection);

    m_pButtonNext = ObjectManager::createButton(tr("Next"));
    m_pButtonNext->setPosition(Settings::getWidth() - 10 - m_pButtonNext->getWidth(), Settings::getHeight() - 10 - m_pButtonNext->getHeight());
    m_pButtonNext->attachTo(this);
    m_pButtonNext->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonNext();
    });
    connect(this, &CampaignMenu::buttonNext, this, &CampaignMenu::slotButtonNext, Qt::QueuedConnection);


    m_pMapSelectionView = new MapSelectionView();
    m_pMapSelectionView->setCurrentCampaign(campaign);
    addChild(m_pMapSelectionView);
    connect(m_pMapSelectionView->getMapSelection(), &MapSelection::itemChanged, this, &CampaignMenu::mapSelectionItemChanged, Qt::QueuedConnection);
    connect(m_pMapSelectionView->getMapSelection(), &MapSelection::itemClicked, this, &CampaignMenu::mapSelectionItemClicked, Qt::QueuedConnection);

    std::tuple<QString, QStringList> data = campaign->getCampaignMaps();
    m_pMapSelectionView->getMapSelection()->setSelection(std::get<0>(data), std::get<1>(data));
}

void CampaignMenu::exitMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    Console::print("Leaving Option Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new MapSelectionMapsMenue());
    addRef();
    oxygine::Actor::detach();
    deleteLater();
    pApp->continueThread();
}


void CampaignMenu::mapSelectionItemClicked(QString item)
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

void CampaignMenu::mapSelectionItemChanged(QString item)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    QFileInfo info = m_pMapSelectionView->getMapSelection()->getCurrentFolder() + item;
    m_pMapSelectionView->loadMap(info);
    pApp->continueThread();
}

void CampaignMenu::slotButtonNext()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (m_pMapSelectionView->getCurrentMap()->getGameScript()->immediateStart())
    {
        GameMap* pMap = GameMap::getInstance();
        pMap->initPlayers();
        pMap->setCampaign(m_pMapSelectionView->getCurrentCampaign());
        pMap->getGameScript()->gameStart();
        pMap->updateSprites();
        // start game
        Console::print("Leaving Campaign Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new GameMenue(false, nullptr));
        addRef();
        oxygine::Actor::detach();
        deleteLater();
    }
    else if (m_Multiplayer)
    {
        // todo
    }
    else
    {
        Console::print("Leaving Campaign Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new MapSelectionMapsMenue(-1, m_pMapSelectionView));
        addRef();
        oxygine::Actor::detach();
        deleteLater();
    }
    pApp->continueThread();
}
