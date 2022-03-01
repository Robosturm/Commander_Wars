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
    if (pBackground != nullptr)
    {
        sprite->setResAnim(pBackground);
        // background should be last to draw
        sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
        sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
        sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());
    }
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
    m_pMapSelectionView = spMapSelectionView::create(Settings::getHeight() / 3 - 30);
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
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(event);
        if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
        {
            emit sigMapSelected(-1, -1, -1);
        }
        else if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
        {
            emit sigHideMinimap();
        }
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
    connect(this, &CampaignMenu::sigHideMinimap, this, &CampaignMenu::hideMinimap, Qt::QueuedConnection);
    connect(this, &CampaignMenu::sigShowMinimap, this, &CampaignMenu::showMinimap, Qt::QueuedConnection);
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
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(event);
        if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
        {
            pTouchEvent->stopImmediatePropagation();
            pTouchEvent->stopPropagation();
            emit sigMapSelected(map, pPtrSprite->getX(), pPtrSprite->getY());
        }
        else if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
        {
            pTouchEvent->stopImmediatePropagation();
            pTouchEvent->stopPropagation();
            emit sigShowMinimap();
        }
    });
    if (pPtrSprite->getParent() == nullptr)
    {
        m_pMapBackground->addChild(oxygine::spSprite(pPtrSprite));
    }
}

void CampaignMenu::showMinimap()
{
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    if (pMap.get() != nullptr)
    {
        Mainapp::getInstance()->getAudioThread()->playSound("minimapOpen.wav");
        qint32 x = m_currentMapFlagPosition.x();
        qint32 y = m_currentMapFlagPosition.y();
        GameManager* pGameManager = GameManager::getInstance();
        auto pMiniMapPanel = m_pMapSelectionView->getMinimapPanel();
        if (y - pMiniMapPanel->getHeight() / 2 + m_pMapBackground->getY() > 0)
        {
            pMiniMapPanel->setY(y - pMiniMapPanel->getHeight() / 2);
        }
        else
        {
            pMiniMapPanel->setY(y + pGameManager->getResAnim("campaignFlag")->getHeight() + pMiniMapPanel->getHeight() / 2);
        }
        qint32 xPos = x - pMiniMapPanel->getScaledWidth() * 0.5f;
        if (xPos + m_pMapBackground->getX() < 0)
        {
            xPos = -m_pMapBackground->getX();
        }
        else if (xPos + pMiniMapPanel->getScaledWidth() + m_pMapBackground->getX() > Settings::getWidth())
        {
            xPos = Settings::getWidth() - pMiniMapPanel->getScaledWidth() - m_pMapBackground->getX();
        }
        pMiniMapPanel->setX(xPos);
        m_pMapBackground->addChild(pMiniMapPanel);
    }
}

void CampaignMenu::hideMinimap()
{
    if (m_pMapSelectionView->getMinimapPanel()->getParent() == m_pMapBackground.get())
    {
        Mainapp::getInstance()->getAudioThread()->playSound("minimapOpen.wav");
        m_pMapSelectionView->getMinimapPanel()->detach();
    }
}

void CampaignMenu::mapSelected(qint32 index, qint32 x, qint32 y)
{
    m_currentMapFlagPosition = QPoint(x, y);
    
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
        QFileInfo info(dir, file);
        if (info != m_pMapSelectionView->getCurrentMapFile())
        {
            GameManager* pGameManager = GameManager::getInstance();
            m_pMapSelectionView->loadMap(info);
            // todo show map info
            auto pBuildingBackground = m_pMapSelectionView->getBuildingBackground();
            qint32 contentHeight = pBuildingBackground->getScaledHeight();
            if (y - contentHeight + m_pMapBackground->getY() > 0)
            {
                pBuildingBackground->setY(y - contentHeight);
            }
            else
            {
                pBuildingBackground->setY(y + pGameManager->getResAnim("campaignFlag")->getHeight());
            }
            qint32 xPos = x - pBuildingBackground->getScaledWidth() * 0.5f;
            if (xPos + m_pMapBackground->getX() < 0)
            {
                xPos = -m_pMapBackground->getX();
            }
            else if (xPos + pBuildingBackground->getScaledWidth() + m_pMapBackground->getX() > Settings::getWidth())
            {
                xPos = Settings::getWidth() - pBuildingBackground->getScaledWidth() - m_pMapBackground->getX();
            }
            pBuildingBackground->setX(xPos);
            m_pMapBackground->addChild(pBuildingBackground);
            auto pMapInfo = m_pMapSelectionView->getMapInfo();
            if (y - contentHeight - pMapInfo->getHeight() + m_pMapBackground->getY() > 0)
            {
                pMapInfo->setY(y - contentHeight - pMapInfo->getHeight());
            }
            else
            {
                pMapInfo->setY(y + pGameManager->getResAnim("campaignFlag")->getHeight() + contentHeight);
            }
            xPos = x - pMapInfo->getScaledWidth() * 0.5f;
            if (xPos + m_pMapBackground->getX() < 0)
            {
                xPos = -m_pMapBackground->getX();
            }
            else if (xPos + pMapInfo->getScaledWidth() + m_pMapBackground->getX() > Settings::getWidth())
            {
                xPos = Settings::getWidth() - pMapInfo->getScaledWidth() - m_pMapBackground->getX();
            }
            pMapInfo->setX(xPos);
            m_pMapBackground->addChild(pMapInfo);
        }
        else
        {
            slotButtonNext();
        }
    }
    else
    {
        QFileInfo info;
        m_pMapSelectionView->loadMap(info);
        m_pMapSelectionView->getBuildingBackground()->detach();
        m_pMapSelectionView->getMinimapPanel()->detach();
        m_pMapSelectionView->getMapInfo()->detach();
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
        CONSOLE_PRINT("Executing:" + object + "." + func, Console::eDEBUG);
        QJSValueList args({pInterpreter->newQObject(this)});
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
    Mainapp::getInstance()->getAudioThread()->playSound("moveOut.wav");
    m_pMapSelectionView->loadCurrentMap();
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    if (pMap.get() != nullptr)
    {
        if (pMap->getGameScript()->immediateStart())
        {
            pMap->initPlayersAndSelectCOs();
            pMap->setCampaign(m_pMapSelectionView->getCurrentCampaign());
            pMap->getGameScript()->gameStart();
            pMap->updateSprites();
            // start game
            CONSOLE_PRINT("Leaving Campaign Menue", Console::eDEBUG);
            auto window = spGameMenue::create(pMap, false, spNetworkInterface());
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
        m_pMapSelectionView->getCurrentCampaign()->serializeObject(stream);
        file.close();
    }   
}

void CampaignMenu::autosave()
{
    if (Settings::getAutoSavingCycle() > 0)
    {
        CONSOLE_PRINT("CampaignMenu::autosave()", Console::eDEBUG);
        QString path = GlobalUtils::getNextAutosavePath(Settings::getUserPath() + "savegames/" + m_pMapSelectionView->getCurrentCampaign()->getName() + "_autosave_", ".camp", Settings::getAutoSavingCycle());
        saveCampaign(path);
    }
}
