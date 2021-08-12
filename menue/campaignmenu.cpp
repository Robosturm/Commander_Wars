#include "menue/campaignmenu.h"
#include "menue/mainwindow.h"
#include "menue/mapselectionmapsmenue.h"
#include "menue/gamemenue.h"

#include "multiplayer/multiplayermenu.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"
#include "coreengine/audiothread.h"
#include "coreengine/globalutils.h"

#include "game/gamemap.h"
#include "game/gamescript.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"

#include "objects/dialogs/filedialog.h"

#include "ui_reader/uifactory.h"

CampaignMenu::CampaignMenu(spCampaign campaign, bool multiplayer, bool autosaveCampaign)
    : Basemenu(),
      m_Multiplayer(multiplayer)
{
    setObjectName("CampaignMenu");
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    moveToThread(pApp->getWorkerthread());
    Console::print("Entering Campaign Menue", Console::eDEBUG);
    Interpreter::setCppOwnerShip(this);

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = oxygine::spSprite::create();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("campaignmenu");
    sprite->setResAnim(pBackground);
    // background should be last to draw
    sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
    sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
    sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/mapselection");
    pApp->getAudioThread()->playRandom();

    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    addChild(pButtonExit);
    pButtonExit->setPosition(10,
                             Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &CampaignMenu::sigExitMenue, this, &CampaignMenu::exitMenue, Qt::QueuedConnection);

    m_pButtonNext = ObjectManager::createButton(tr("Next"));
    m_pButtonNext->setPosition(Settings::getWidth() - 10 - m_pButtonNext->getWidth(), Settings::getHeight() - 10 - m_pButtonNext->getHeight());
    addChild(m_pButtonNext);
    m_pButtonNext->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonNext();
    });
    connect(this, &CampaignMenu::buttonNext, this, &CampaignMenu::slotButtonNext, Qt::QueuedConnection);

    m_pButtonSave = ObjectManager::createButton(tr("Save"));
    m_pButtonSave->setPosition(Settings::getWidth() / 2 - m_pButtonSave->getWidth() / 2, Settings::getHeight() - 10 - m_pButtonSave->getHeight());
    addChild(m_pButtonSave);
    m_pButtonSave->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowSaveCampaign();
    });
    connect(this, &CampaignMenu::sigShowSaveCampaign, this, &CampaignMenu::showSaveCampaign, Qt::QueuedConnection);


    m_pMapSelectionView = spMapSelectionView::create();
    m_pMapSelectionView->setCurrentCampaign(campaign);
    addChild(m_pMapSelectionView);
    connect(m_pMapSelectionView->getMapSelection(), &MapSelection::itemChanged, this, &CampaignMenu::mapSelectionItemChanged, Qt::QueuedConnection);
    connect(m_pMapSelectionView->getMapSelection(), &MapSelection::itemClicked, this, &CampaignMenu::mapSelectionItemClicked, Qt::QueuedConnection);

    Campaign::CampaignMapInfo data = campaign->getCampaignMaps();
    m_pMapSelectionView->getMapSelection()->setSelection(data.m_folder, data.m_mapFilenames);

    if (autosaveCampaign)
    {
        autosave();
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue obj = pInterpreter->newQObject(this);
    pInterpreter->setGlobal("currentMenu", obj);
    UiFactory::getInstance().createUi("ui/campaignmenu.xml", this);
    pApp->continueRendering();
}

void CampaignMenu::exitMenue()
{    
    Console::print("Leaving Option Menue", Console::eDEBUG);
    auto window = spMapSelectionMapsMenue::create();
    oxygine::getStage()->addChild(window);
    emit window->sigOnEnter();
    oxygine::Actor::detach();    
}

void CampaignMenu::mapSelectionItemClicked(QString item)
{    
    QFileInfo info = QFileInfo(item);
    if (!info.exists())
    {
        info = QFileInfo(Settings::getUserPath() + item);
        if (!info.exists())
        {
            info = QFileInfo(oxygine::Resource::RCC_PREFIX_PATH + item);
        }
    }
    if (info.isFile())
    {
        emit buttonNext();
    }    
}

void CampaignMenu::mapSelectionItemChanged(QString item)
{    
    QFileInfo info = QFileInfo(item);
    if (!info.exists())
    {
        info = QFileInfo(Settings::getUserPath() + item);
        if (!info.exists())
        {
            info = QFileInfo(oxygine::Resource::RCC_PREFIX_PATH + item);
        }
    }
    m_pMapSelectionView->loadMap(info);    
}

void CampaignMenu::slotButtonNext()
{    
    m_pMapSelectionView->loadCurrentMap();
    if (m_pMapSelectionView->getCurrentMap()->getGameScript()->immediateStart())
    {
        spGameMap pMap = GameMap::getInstance();
        pMap->initPlayersAndSelectCOs();
        pMap->setCampaign(m_pMapSelectionView->getCurrentCampaign());
        pMap->getGameScript()->gameStart();
        pMap->updateSprites();
        // start game
        Console::print("Leaving Campaign Menue", Console::eDEBUG);
        auto window = spGameMenue::create(false, spNetworkInterface());
        oxygine::getStage()->addChild(window);
        emit window->sigOnEnter();
        oxygine::Actor::detach();
    }
    else if (m_Multiplayer)
    {
        // todo
    }
    else
    {
        Console::print("Leaving Campaign Menue", Console::eDEBUG);
        auto window = spMapSelectionMapsMenue::create(-1, m_pMapSelectionView);
        oxygine::getStage()->addChild(window);
        emit window->sigOnEnter();
        oxygine::Actor::detach();
    }    
}

void CampaignMenu::showSaveCampaign()
{    
    QVector<QString> wildcards;
    wildcards.append("*.camp");
    QString path = Settings::getUserPath() + "savegames";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards);
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &CampaignMenu::saveCampaign, Qt::QueuedConnection);    
}

void CampaignMenu::saveCampaign(QString filename)
{    
    if (filename.endsWith(".camp"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        m_pMapSelectionView->getCurrentCampaign()->serializeObject(stream);
        file.close();
    }   
}

void CampaignMenu::autosave()
{
    if (Settings::getAutoSavingCycle() > 0)
    {
        Console::print("CampaignMenu::autosave()", Console::eDEBUG);
        QString path = GlobalUtils::getNextAutosavePath(Settings::getUserPath() + "savegames/" + m_pMapSelectionView->getCurrentCampaign()->getName() + "_autosave_", ".camp", Settings::getAutoSavingCycle());
        saveCampaign(path);
    }
}
