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
#include "resource_management/gamemanager.h"

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
    CONSOLE_PRINT("Entering Campaign Menue", Console::eDEBUG);
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
        emit sigButtonNext();
    });
    connect(this, &CampaignMenu::sigButtonNext, this, &CampaignMenu::slotButtonNext, Qt::QueuedConnection);

    m_pButtonSave = ObjectManager::createButton(tr("Save"));
    m_pButtonSave->setPosition(Settings::getWidth() / 2 - m_pButtonSave->getWidth() / 2, Settings::getHeight() - 10 - m_pButtonSave->getHeight());
    addChild(m_pButtonSave);
    m_pButtonSave->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *)->void
    {
        emit sigShowSaveCampaign();
    });
    connect(this, &CampaignMenu::sigShowSaveCampaign, this, &CampaignMenu::showSaveCampaign, Qt::QueuedConnection);

    if (campaign->getUsesCampaignMap())
    {
        createCampaignMapSelection(campaign);
    }
    else
    {
        createMapSelection(campaign);
    }

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

void CampaignMenu::createCampaignMapSelection(spCampaign & campaign)
{
    m_pMapSelectionView = spMapSelectionView::create();
    m_pMapSelectionView->setCurrentSetCampaign(campaign);
    GameManager* pGameManager = GameManager::getInstance();
    Mainapp* pApp = Mainapp::getInstance();
    campaign->getCampaignMapData(m_campaignData);
    m_pSlidingActor = oxygine::spSlidingActor::create();
    m_pSlidingActor->setSize(Settings::getWidth(), Settings::getHeight());
    addChild(m_pSlidingActor);
    m_campaignBackground = oxygine::spSingleResAnim::create();
    QString path = Settings::getUserPath() + m_campaignData.getMapBackground();
    if (!QFile::exists(path))
    {
        path = oxygine::Resource::RCC_PREFIX_PATH + m_campaignData.getMapBackground();
    }
    QImage background(path);
    pApp->loadResAnim(m_campaignBackground, background, 1, 1, 1.0f, false);
    m_pMapBackground = oxygine::spSprite::create();
    m_pMapBackground->setResAnim(m_campaignBackground.get());
    m_pMapBackground->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event* event)
    {
        emit sigMapSelected(-1, -1, -1);
    });
    if (m_campaignData.getMapHeight() < Settings::getHeight())
    {
        m_pMapBackground->setHeight(Settings::getHeight());
    }
    else
    {
        m_pMapBackground->setHeight(m_campaignData.getMapHeight());
    }
    if (m_campaignData.getMapWidth() < Settings::getWidth())
    {
        m_pMapBackground->setWidth(Settings::getWidth());
    }
    else
    {
        m_pMapBackground->setWidth(m_campaignData.getMapWidth());
    }
    m_pSlidingActor->setContent(m_pMapBackground);
    // create won map data
    qint32 width = m_pMapBackground->getScaledWidth();
    qint32 height = m_pMapBackground->getScaledHeight();
    auto positions = m_campaignData.getWonMapPositions();
    qint32 newlyWonMap = m_campaignData.getNewlyWonMap();
    bool eventQueueEmpty = true;
    for (qint32 i = 0; i < positions.size(); ++i)
    {
        const auto & position = positions[i];
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        pSprite->setPosition(position.x() * width, position.y() * height);
        oxygine::ResAnim* pAnim = pGameManager->getResAnim("campaignFlagDone");
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
        m_pMapBackground->addChild(pSprite);
        if (i == newlyWonMap)
        {
            pSprite = oxygine::spSprite::create();
            qint32 flagHeight = pAnim->getHeight();
            oxygine::ResAnim* pAnim = pGameManager->getResAnim("campaignMapDone");
            pSprite->setPosition(position.x() * width - pAnim->getWidth() / 2, position.y() * height + flagHeight - pAnim->getHeight());
            focusOnPosition(pSprite->getPosition());
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime));
            tween->addDoneCallback([=](oxygine::Event* pEvent)
            {
                emit sigEventPlayed(0);
            });
            pSprite->addTween(tween);
            m_pMapBackground->addChild(pSprite);
            Mainapp::getInstance()->getAudioThread()->playSound("firework.wav");
            eventQueueEmpty = false;
        }
    }
    // create open map data
    positions = m_campaignData.getOpenMapPositions();
    auto newMapPosition = m_campaignData.getNewMapPosition();
    for (qint32 i = 0; i < positions.size(); ++i)
    {
        if (newMapPosition.contains(i))
        {
            if (eventQueueEmpty)
            {
                eventQueueEmpty = false;
                playNextEvent(0);
            }
        }
        else
        {
            oxygine::spSprite pSprite = oxygine::spSprite::create();
            const auto & position = positions[i];
            pSprite->setPosition(position.x() * width, position.y() * height);
            flagAppeared(pSprite.get(), i);
        }
    }
    connect(this, &CampaignMenu::sigMapSelected, this, &CampaignMenu::mapSelected, Qt::QueuedConnection);
    connect(this, &CampaignMenu::sigFlagAppeared, this, &CampaignMenu::flagAppeared, Qt::QueuedConnection);
    connect(this, &CampaignMenu::sigEventPlayed, this, &CampaignMenu::playNextEvent, Qt::QueuedConnection);
}

void CampaignMenu::playNextEvent(qint32 event)
{
    auto positions = m_campaignData.getOpenMapPositions();
    auto newMapPosition = m_campaignData.getNewMapPosition();
    if (event < newMapPosition.size())
    {
        GameManager* pGameManager = GameManager::getInstance();
        oxygine::ResAnim* pAnim = pGameManager->getResAnim("campaignFlagAppear");
        Mainapp::getInstance()->getAudioThread()->playSound("flagAppear.wav");
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        qint32 width = m_pMapBackground->getScaledWidth();
        qint32 height = m_pMapBackground->getScaledHeight();
        qint32 index = newMapPosition[event];
        const auto & position = positions[index];
        pSprite->setPosition(position.x() * width, position.y() * height);
        focusOnPosition(pSprite->getPosition());
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::Sprite* pPtrSprite = pSprite.get();
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), 1);
            tween->addDoneCallback([=](oxygine::Event* pEvent)
            {
                emit sigFlagAppeared(pPtrSprite, index);
                emit sigEventPlayed(event + 1);
            });
            pSprite->addTween(tween);
            m_pMapBackground->addChild(pSprite);
        }
    }
}

void CampaignMenu::focusOnPosition(oxygine::Vector2 position)
{
    m_pMapBackground->setPosition(Settings::getWidth() / 2 - position.x, Settings::getHeight() / 2 - position.y);
    m_pSlidingActor->snap();
}

void CampaignMenu::flagAppeared(oxygine::Sprite* pPtrSprite, qint32 map)
{
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("campaignFlag");
    if (pAnim->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
        pPtrSprite->addTween(tween);
    }
    else
    {
        pPtrSprite->setResAnim(pAnim);
    }
    pPtrSprite->addClickListener([=](oxygine::Event* event)
    {
        event->stopImmediatePropagation();
        event->stopPropagation();
        emit sigMapSelected(map, pPtrSprite->getX(), pPtrSprite->getY());
    });
    if (pPtrSprite->getParent() == nullptr)
    {
        m_pMapBackground->addChild(oxygine::spSprite(pPtrSprite));
    }
}

void CampaignMenu::mapSelected(qint32 index, qint32 x, qint32 y)
{
    spGameMap pMap = GameMap::getInstance();
    QString folder = m_campaignData.getFolder();
    auto files = m_campaignData.getMapFilenames();
    QString file;
    QString dir;
    if (index >= 0 && index < files.size())
    {
        dir = Settings::getUserPath() + folder;
        file = files[index];
        if (!QFile::exists(dir + "/" + file))
        {
            dir = oxygine::Resource::RCC_PREFIX_PATH + folder;
        }
    }
    QFileInfo info(dir, file);
    if (info != m_pMapSelectionView->getCurrentMapFile())
    {
        GameManager* pGameManager = GameManager::getInstance();
        m_pMapSelectionView->loadMap(info);
        // todo show map info
        auto content = m_pMapSelectionView->getContentSlider();
        qint32 contentHeight = content->getScaledHeight();
        if (y - contentHeight > 0)
        {
            content->setY(y - contentHeight);
        }
        else
        {
            content->setY(y + pGameManager->getResAnim("campaignFlag")->getHeight());
        }
        addChild(content);
    }
    else
    {
        m_pMapSelectionView->getContentSlider()->detach();
        slotButtonNext();
    }
}

void CampaignMenu::createMapSelection(spCampaign & campaign)
{
    m_pMapSelectionView = spMapSelectionView::create();
    m_pMapSelectionView->setCurrentSetCampaign(campaign);
    addChild(m_pMapSelectionView);
    connect(m_pMapSelectionView->getMapSelection(), &MapSelection::itemChanged, this, &CampaignMenu::mapSelectionItemChanged, Qt::QueuedConnection);
    connect(m_pMapSelectionView->getMapSelection(), &MapSelection::itemClicked, this, &CampaignMenu::mapSelectionItemClicked, Qt::QueuedConnection);
    Campaign::CampaignMapInfo data = campaign->getCampaignMaps();
    m_pMapSelectionView->getMapSelection()->setSelection(data.m_folder, data.m_mapFilenames);
}

void CampaignMenu::exitMenue()
{    
    CONSOLE_PRINT("Leaving Option Menue", Console::eDEBUG);
    auto window = spMapSelectionMapsMenue::create();
    oxygine::Stage::getStage()->addChild(window);
    oxygine::Actor::detach();    
}

void CampaignMenu::onEnter()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString object = "Init";
    QString func = "campaignMenu";
    if (pInterpreter->exists(object, func))
    {
        QJSValueList args;
        QJSValue value = pInterpreter->newQObject(this);
        args << value;
        pInterpreter->doFunction(object, func, args);
    }
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
        emit sigButtonNext();
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
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr &&
        pMap->getGameScript()->immediateStart())
    {
        pMap->initPlayersAndSelectCOs();
        pMap->setCampaign(m_pMapSelectionView->getCurrentSetCampaign());
        pMap->getGameScript()->gameStart();
        pMap->updateSprites();
        // start game
        CONSOLE_PRINT("Leaving Campaign Menue", Console::eDEBUG);
        auto window = spGameMenue::create(false, spNetworkInterface());
        oxygine::Stage::getStage()->addChild(window);
        oxygine::Actor::detach();
    }
    else if (m_Multiplayer)
    {
        // todo
    }
    else
    {
        auto window = spMapSelectionMapsMenue::create(-1, m_pMapSelectionView);
        oxygine::Stage::getStage()->addChild(window);
        oxygine::Actor::detach();
    }    
}

void CampaignMenu::showSaveCampaign()
{    
    QStringList wildcards;
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
        m_pMapSelectionView->getCurrentSetCampaign()->serializeObject(stream);
        file.close();
    }   
}

void CampaignMenu::autosave()
{
    if (Settings::getAutoSavingCycle() > 0)
    {
        CONSOLE_PRINT("CampaignMenu::autosave()", Console::eDEBUG);
        QString path = GlobalUtils::getNextAutosavePath(Settings::getUserPath() + "savegames/" + m_pMapSelectionView->getCurrentSetCampaign()->getName() + "_autosave_", ".camp", Settings::getAutoSavingCycle());
        saveCampaign(path);
    }
}
