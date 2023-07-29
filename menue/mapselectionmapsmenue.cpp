#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "menue/mapselectionmapsmenue.h"
#include "menue/mainwindow.h"
#include "menue/gamemenue.h"
#include "menue/campaignmenu.h"

#include "coreengine/mainapp.h"
#include "coreengine/gameconsole.h"
#include "coreengine/audiomanager.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"

#include "game/gamemap.h"
#include "game/campaign.h"
#include "game/player.h"

#include "mapsupport/randomMapGenerator.h"

#include "objects/dialogs/editor/dialograndommap.h"
#include "objects/dialogs/filedialog.h"
#include "objects/dialogs/dialogmessagebox.h"

#include "multiplayer/multiplayermenu.h"
#include "multiplayer/networkcommands.h"

#include "objects/dialogs/mapSelection/mapselectionfilterdialog.h"

#include "ui_reader/uifactory.h"

MapSelectionMapsMenue::MapSelectionMapsMenue(spMapSelectionView pMapSelectionView, qint32 heigth)
    : Basemenu(),
      m_pMapSelectionView(pMapSelectionView)
{
#ifdef GRAPHICSUPPORT
    setObjectName("MapSelectionMapsMenue");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    Interpreter::setCppOwnerShip(this);
    CONSOLE_PRINT("Entering Map Selection Menue", GameConsole::eDEBUG);
    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = oxygine::spSprite::create();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("mapselectionmenu");
    if (pBackground != nullptr)
    {
        sprite->setResAnim(pBackground);
        // background should be last to draw
        sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
        sprite->setScaleX(static_cast<float>(oxygine::Stage::getStage()->getWidth()) / static_cast<float>(pBackground->getWidth()));
        sprite->setScaleY(static_cast<float>(oxygine::Stage::getStage()->getHeight()) / static_cast<float>(pBackground->getHeight()));
    }

    pApp->getAudioManager()->clearPlayList();
    pApp->getAudioManager()->loadFolder("resources/music/mapselection");
    pApp->getAudioManager()->playRandom();
    addChild(m_pMapSelectionView);

    connect(m_pMapSelectionView->getMapSelection(), &MapSelection::itemChanged, this, &MapSelectionMapsMenue::mapSelectionItemChanged, Qt::QueuedConnection);
    connect(m_pMapSelectionView->getMapSelection(), &MapSelection::itemClicked, this, &MapSelectionMapsMenue::mapSelectionItemClicked, Qt::QueuedConnection);

    m_pButtonBack = ObjectManager::createButton(tr("Back"));
    m_pButtonBack->setPosition(10, oxygine::Stage::getStage()->getHeight() - 10 - m_pButtonBack->getScaledHeight());
    addChild(m_pButtonBack);
    m_pButtonBack->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigButtonBack();
    });
    connect(this, &MapSelectionMapsMenue::sigButtonBack, this, &MapSelectionMapsMenue::buttonBack, Qt::QueuedConnection);

    m_pButtonNext = ObjectManager::createButton(tr("Next"));
    m_pButtonNext->setPosition(oxygine::Stage::getStage()->getWidth() - 10 - m_pButtonNext->getScaledWidth(),
                               oxygine::Stage::getStage()->getHeight() - 10 - m_pButtonNext->getScaledHeight());
    addChild(m_pButtonNext);
    m_pButtonNext->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigButtonNext();
    });
    connect(this, &MapSelectionMapsMenue::sigButtonNext, this, &MapSelectionMapsMenue::buttonNext, Qt::QueuedConnection);

    m_pRandomMap = ObjectManager::createButton(tr("Random Map"));
    m_pRandomMap->setPosition(m_pButtonBack->getX() + m_pButtonBack->getScaledWidth() + 20,
                              oxygine::Stage::getStage()->getHeight() - 10 - m_pButtonNext->getScaledHeight());
    addChild(m_pRandomMap);
    m_pRandomMap->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigRandomMap();
    });
    connect(this, &MapSelectionMapsMenue::sigRandomMap, this, &MapSelectionMapsMenue::showRandomMap, Qt::QueuedConnection);

    m_pMapFilter = ObjectManager::createButton(tr("Map filter"));
    m_pMapFilter->setPosition(m_pRandomMap->getX() + m_pRandomMap->getScaledWidth() + 20,
                              oxygine::Stage::getStage()->getHeight() - 10 - m_pButtonNext->getScaledHeight());
    addChild(m_pMapFilter);
    m_pMapFilter->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigMapFilter();
    });
    connect(this, &MapSelectionMapsMenue::sigMapFilter, this, &MapSelectionMapsMenue::showMapFilter, Qt::QueuedConnection);

    m_pButtonLoadRules = ObjectManager::createButton(tr("Load"));
    m_pButtonLoadRules->setPosition(oxygine::Stage::getStage()->getWidth() / 2 + 10,
                                    oxygine::Stage::getStage()->getHeight() - 10 - m_pButtonNext->getScaledHeight());
    addChild(m_pButtonLoadRules);
    m_pButtonLoadRules->setVisible(false);
    m_pButtonLoadRules->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigShowLoadRules();
    });
    connect(this, &MapSelectionMapsMenue::sigShowLoadRules, this, &MapSelectionMapsMenue::showLoadRules, Qt::QueuedConnection);

    m_pButtonSaveRules = ObjectManager::createButton(tr("Save"));
    m_pButtonSaveRules->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_pButtonSaveRules->getScaledWidth() - 10,
                                    oxygine::Stage::getStage()->getHeight() - 10 - m_pButtonNext->getScaledHeight());
    addChild(m_pButtonSaveRules);
    m_pButtonSaveRules->setVisible(false);
    m_pButtonSaveRules->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigShowSaveRules();
    });
    connect(this, &MapSelectionMapsMenue::sigShowSaveRules, this, &MapSelectionMapsMenue::showSaveRules, Qt::QueuedConnection);

    m_pButtonSaveMap = ObjectManager::createButton(tr("Save Map"));
    m_pButtonSaveMap->setPosition(m_pButtonBack->getX() + m_pButtonBack->getScaledWidth() + 20,
                                  oxygine::Stage::getStage()->getHeight() - 10 - m_pButtonNext->getScaledHeight());
    addChild(m_pButtonSaveMap);
    m_pButtonSaveMap->setVisible(false);
    m_pButtonSaveMap->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigShowSaveMap();
    });
    connect(this, &MapSelectionMapsMenue::sigShowSaveMap, this, &MapSelectionMapsMenue::showSaveMap, Qt::QueuedConnection);


    m_pButtonStart = ObjectManager::createButton(tr("Start Game"), 150);
    m_pButtonStart->setPosition(oxygine::Stage::getStage()->getWidth() - 10 - m_pButtonStart->getScaledWidth(),
                                oxygine::Stage::getStage()->getHeight() - 10 - m_pButtonStart->getScaledHeight());
    addChild(m_pButtonStart);
    m_pButtonStart->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigButtonStartGame();
    });
    connect(this, &MapSelectionMapsMenue::sigButtonStartGame, this, &MapSelectionMapsMenue::startGame, Qt::QueuedConnection);

    qint32 yPos = 10;
    if (heigth <  0)
    {
        heigth = oxygine::Stage::getStage()->getHeight() - 40 * 2;
    }
    m_pPlayerSelection = spPlayerSelection::create(oxygine::Stage::getStage()->getWidth() - 20, heigth);
    m_pPlayerSelection->setPosition(10, yPos);
    addChild(m_pPlayerSelection);

    QSize size(oxygine::Stage::getStage()->getWidth() - 20, oxygine::Stage::getStage()->getHeight() - 40 * 2);
    m_pRuleSelection = spPanel::create(true,  size, size);
    m_pRuleSelection->setPosition(10, 20);
    addChild(m_pRuleSelection);
    if (m_pMapSelectionView->getCurrentCampaign().get() == nullptr)
    {
        hidePlayerSelection();
        hideRuleSelection();
    }
    else
    {
        MapSelectionMapsMenue::hideMapSelection();
        hideRuleSelection();
        m_pPlayerSelection->attachCampaign(m_pMapSelectionView->getCurrentCampaign());
        MapSelectionMapsMenue::showPlayerSelection();
        m_MapSelectionStep = MapSelectionStep::selectPlayer;
    }
    UiFactory::getInstance().createUi("ui/mapselectionmapsmenu.xml", this);
    pApp->continueRendering();
}

void MapSelectionMapsMenue::buttonBack()
{    
    CONSOLE_PRINT("slotButtonBack()", GameConsole::eDEBUG);
    Mainapp::getInstance()->pauseRendering();
    switch (m_MapSelectionStep)
    {
        case MapSelectionStep::selectMap:
        {
            exitMenu();
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
                CONSOLE_PRINT("Leaving Map Selection Menue", GameConsole::eDEBUG);
                m_onEnterTimer.stop();
                if (dynamic_cast<Multiplayermenu*>(this) != nullptr)
                {
                    oxygine::Stage::getStage()->addChild(spCampaignMenu::create(m_pMapSelectionView->getCurrentCampaign(), true));
                }
                else
                {
                    oxygine::Stage::getStage()->addChild(spCampaignMenu::create(m_pMapSelectionView->getCurrentCampaign(), false));
                }
                oxygine::Actor::detach();
            }
            break;
        }
    }
    Mainapp::getInstance()->continueRendering();
}

void MapSelectionMapsMenue::buttonNext()
{    
    CONSOLE_PRINT("slotButtonNext()", GameConsole::eDEBUG);
    Mainapp::getInstance()->pauseRendering();
    switch (m_MapSelectionStep)
    {
        case MapSelectionStep::selectMap:
        {
            QString mapFile = m_pMapSelectionView->getCurrentFile().filePath();
            bool isExternal = (mapFile == NetworkCommands::RANDOMMAPIDENTIFIER ||
                               mapFile == NetworkCommands::SERVERMAPIDENTIFIER);
            if (QFile::exists(mapFile) || isExternal)
            {
                if (!isExternal)
                {
                    m_pMapSelectionView->loadCurrentMap();
                }
                QString file = m_pMapSelectionView->getMapSelection()->getCurrentFile();

                if ((m_pMapSelectionView->getCurrentMap().get() != nullptr && file.endsWith(".map")) ||
                    isExternal)
                {
                    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
                    pMap->setCampaign(m_pMapSelectionView->getCurrentCampaign());
                    if (pMap->getGameScript()->immediateStart())
                    {
                        startGame();
                    }
                    else
                    {
                        hideMapSelection();
                        loadRules(Settings::getInstance()->getDefaultRuleset());
                        showRuleSelection();
                        m_MapSelectionStep = MapSelectionStep::selectRules;
                    }
                }
                else
                {
                    if (file.endsWith(".jsm") &&
                        m_pMapSelectionView->getCurrentLoadedCampaign().get() != nullptr)
                    {
                        CONSOLE_PRINT("Leaving Map Selection Menue", GameConsole::eDEBUG);
                        m_onEnterTimer.stop();
                        if (dynamic_cast<Multiplayermenu*>(this) != nullptr)
                        {
                            oxygine::Stage::getStage()->addChild(spCampaignMenu::create(m_pMapSelectionView->getCurrentLoadedCampaign(), true));
                        }
                        else
                        {
                            oxygine::Stage::getStage()->addChild(spCampaignMenu::create(m_pMapSelectionView->getCurrentLoadedCampaign(), false));
                        }
                        oxygine::Actor::detach();
                    }
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
    Mainapp::getInstance()->continueRendering();
}

void MapSelectionMapsMenue::exitMenu()
{
    CONSOLE_PRINT("Leaving Map Selection Menue", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    spMainwindow window = spMainwindow::create("ui/menu/mainsinglemenu.xml");
    oxygine::Stage::getStage()->addChild(window);
    oxygine::Actor::detach();
}

void MapSelectionMapsMenue::mapSelectionItemClicked(QString item)
{    
    QFileInfo info = QFileInfo(item);
    if (info.isFile())
    {
        m_pMapSelectionView->setCurrentFile(info.filePath());
        emit sigButtonNext();
    }    
}

void MapSelectionMapsMenue::mapSelectionItemChanged(QString item)
{    
    if (m_pMapSelectionView->getVisible())
    {
        QFileInfo info = QFileInfo(item);
        m_pMapSelectionView->loadMap(info);
    }
    else
    {
        CONSOLE_PRINT("Ignoring map selection change", GameConsole::eDEBUG);
    }
}

void MapSelectionMapsMenue::hideMapSelection()
{    
    m_pRandomMap->setVisible(false);
    m_pMapFilter->setVisible(false);
    m_pMapSelectionView->setVisible(false);    
}

void MapSelectionMapsMenue::showMapSelection()
{    
    m_pRandomMap->setVisible(true);
    m_pMapFilter->setVisible(true);
    m_pMapSelectionView->setVisible(true);    
    m_pMapSelectionView->getMapSelection()->refresh();
}

void MapSelectionMapsMenue::hideRuleSelection()
{    
    m_pRuleSelection->setVisible(false);
    m_pButtonSaveRules->setVisible(false);
    m_pButtonLoadRules->setVisible(false);
    m_pRuleSelectionView.free();
    m_pRuleSelection->clearContent();    
}

void MapSelectionMapsMenue::showRuleSelection()
{    
    m_pRuleSelection->setVisible(true);
    m_pButtonSaveRules->setVisible(true);
    m_pButtonLoadRules->setVisible(true);
    m_pRuleSelection->clearContent();
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    m_pRuleSelectionView = spRuleSelection::create(pMap.get(), oxygine::Stage::getStage()->getWidth() - 80, RuleSelection::Mode::Singleplayer);
    connect(m_pRuleSelectionView.get(), &RuleSelection::sigSizeChanged, this, &MapSelectionMapsMenue::ruleSelectionSizeChanged, Qt::QueuedConnection);
    m_pRuleSelection->addItem(m_pRuleSelectionView);
    m_pRuleSelection->setContentHeigth(m_pRuleSelectionView->getScaledHeight() + 60);
    m_pRuleSelection->setContentWidth(m_pRuleSelectionView->getScaledWidth() + 60);
}

void MapSelectionMapsMenue::ruleSelectionSizeChanged()
{
    m_pRuleSelection->setContentHeigth(m_pRuleSelectionView->getScaledHeight() + 60);
    m_pRuleSelection->setContentWidth(m_pRuleSelectionView->getScaledWidth() + 60);
}

void MapSelectionMapsMenue::showPlayerSelection(bool relaunchedLobby)
{
    m_pButtonStart->setVisible(true);
    m_pButtonNext->setVisible(false);
    m_pButtonSaveMap->setVisible(true);
    m_pPlayerSelection->setVisible(true);
    m_pPlayerSelection->setMap(m_pMapSelectionView->getCurrentMap().get());
    m_pPlayerSelection->showPlayerSelection(relaunchedLobby);
}

void MapSelectionMapsMenue::hidePlayerSelection()
{
    m_pButtonStart->setVisible(false);
    m_pButtonNext->setVisible(true);
    m_pButtonSaveMap->setVisible(false);
    m_pPlayerSelection->setVisible(false);
}

void MapSelectionMapsMenue::startGame()
{    
    CONSOLE_PRINT("Start game", GameConsole::eDEBUG);
    defeatClosedPlayers();

    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    pMap->setVisible(false);
    pMap->initPlayersAndSelectCOs();
    pMap->setCampaign(m_pMapSelectionView->getCurrentCampaign());
    pMap->getGameScript()->gameStart();
    bool applyRulesPalette = pMap->getGameRules()->getMapPalette() > 0;
    pMap->updateSprites(-1, -1, false, false, applyRulesPalette);
    // start game
    CONSOLE_PRINT("Leaving Map Selection Menue", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    spGameMenue window = spGameMenue::create(pMap, false, spNetworkInterface(), false);
    oxygine::Stage::getStage()->addChild(window);
    oxygine::Actor::detach();
}

void MapSelectionMapsMenue::defeatClosedPlayers()
{    
    spGameMap pMap = m_pMapSelectionView->getCurrentMap();
    if(!pMap->getGameScript()->immediateStart())
    {
        for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getPlayerCount(); i++)
        {
            GameEnums::AiTypes aiType =  m_pPlayerSelection->getPlayerAiType(i);
            if (aiType == GameEnums::AiTypes::AiTypes_Closed)
            {
                CONSOLE_PRINT("Defeating player " + QString::number(i) + " cause he's selected as closed player.", GameConsole::eDEBUG);
                pMap->getPlayer(i)->setIsDefeated(true);
            }
        }
    }
}

void MapSelectionMapsMenue::showRandomMap()
{
    spDialogRandomMap pDialogRandomMap = spDialogRandomMap::create("");
    addChild(pDialogRandomMap);
    connect(pDialogRandomMap.get(), &DialogRandomMap::sigFinished, this, &MapSelectionMapsMenue::selectRandomMap, Qt::QueuedConnection);
}

void MapSelectionMapsMenue::selectRandomMap(QString mapName, QString author, QString description,
                                            qint32 width, qint32 heigth, qint32 playerCount,
                                            bool roadSupport, qint32 seed,
                                            QVector<std::tuple<QString, float>> terrains,
                                            QVector<std::tuple<QString, float>> buildings,
                                            QVector<float> ownedBaseSize,
                                            float startBaseSize,
                                            QVector<std::tuple<QString, float>> units,
                                            qint32 unitCount,
                                            float startBaseUnitSize,
                                            QVector<float> unitDistribution,
                                            bool unitsDistributed,
                                            bool mirrored)
{
    
    spGameMap pMap = spGameMap::create(width, heigth, playerCount);
    RandomMapGenerator::randomMap(pMap.get(), width, heigth, playerCount, roadSupport, seed,
                        terrains, buildings, ownedBaseSize,
                        startBaseSize / 100.0f,
                        units, unitCount, startBaseUnitSize / 100.0f, unitDistribution, unitsDistributed, mirrored);
    pMap->setMapName(mapName);
    if (mapName.isEmpty())
    {
        pMap->setMapName("Random Map");
    }
    pMap->setMapAuthor(author);
    pMap->setMapDescription(description);
    m_pMapSelectionView->setCurrentMap(pMap);
    m_pMapSelectionView->setCurrentFile(NetworkCommands::RANDOMMAPIDENTIFIER);
    emit sigButtonNext();
    
}

void MapSelectionMapsMenue::showLoadRules()
{
    
    QStringList wildcards;
    wildcards.append("*.grl");
    QString path = Settings::getInstance()->getUserPath() + "data/gamerules";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards, false, "", false, tr("Load"));
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &MapSelectionMapsMenue::loadRules, Qt::QueuedConnection);
    
}

void MapSelectionMapsMenue::showSaveRules()
{    
    QStringList wildcards;
    wildcards.append("*.grl");
    QString path = Settings::getInstance()->getUserPath() + "data/gamerules";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards, true, "", false, tr("Save"));
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &MapSelectionMapsMenue::saveRules, Qt::QueuedConnection);
}

void MapSelectionMapsMenue::loadRules(QString filename)
{
    if (filename.endsWith(".grl"))
    {
        QFile file(filename);
        if (file.exists())
        {
            spGameMap pMap = m_pMapSelectionView->getCurrentMap();
            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            QDataStream stream(&file);
            stream.setVersion(QDataStream::Version::Qt_6_5);
            pMap->getGameRules()->deserializeObject(stream);
            file.close();
            hideRuleSelection();
            showRuleSelection();
        }
    }
}

void MapSelectionMapsMenue::saveRules(QString filename)
{
    if (filename.endsWith(".grl"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        spGameMap pMap = m_pMapSelectionView->getCurrentMap();
        pMap->getGameRules()->serializeObject(stream);
        file.close();
        spDialogMessageBox pMessageBox = spDialogMessageBox::create(tr("Do you want to make the saved ruleset the default ruleset?"), true, tr("Yes"), tr("No"));
        addChild(pMessageBox);
        connect(pMessageBox.get(),  &DialogMessageBox::sigOk, this, [=]()
        {
            Settings::getInstance()->setDefaultRuleset(filename);
        }, Qt::QueuedConnection);
    }
}

void MapSelectionMapsMenue::showSaveMap()
{    
    QStringList wildcards;
    wildcards.append("*.map");
    QString path = Settings::getInstance()->getUserPath() + "maps/";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards, true, "", false, tr("Save"));
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &MapSelectionMapsMenue::saveMap, Qt::QueuedConnection);
}

void MapSelectionMapsMenue::showMapFilter()
{
    spMapSelectionFilterDialog mapSelectionFilterDialog = spMapSelectionFilterDialog::create(m_pMapSelectionView->getMapSelection()->getMapFilter());
    addChild(mapSelectionFilterDialog);
    connect(mapSelectionFilterDialog.get(), &MapSelectionFilterDialog::sigFinished, m_pMapSelectionView->getMapSelection(), &MapSelection::filterChanged, Qt::QueuedConnection);
}

void MapSelectionMapsMenue::saveMap(QString filename)
{    
    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        spGameMap pMap = m_pMapSelectionView->getCurrentMap();
        pMap->serializeObject(stream);
        file.close();
    }    
}

void MapSelectionMapsMenue::selectMap(QString folder, QString filename)
{
    QFileInfo info(folder + filename);
    m_pMapSelectionView->getMapSelection()->changeFolder(folder);
    m_pMapSelectionView->getMapSelection()->setCurrentItem(filename);
    if (!QFile::exists(folder + filename))
    {
        info = QFileInfo(oxygine::Resource::RCC_PREFIX_PATH + folder + filename);
    }
    m_pMapSelectionView->loadMap(info, true);
}

PlayerSelection* MapSelectionMapsMenue::getPlayerSelection() const
{
    return m_pPlayerSelection.get();
}

void MapSelectionMapsMenue::onEnter()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString object = "Init";
    QString func = "mapsSelection";
    if (pInterpreter->exists(object, func))
    {
        CONSOLE_PRINT("Executing:" + object + "." + func, GameConsole::eDEBUG);
        QJSValueList args({pInterpreter->newQObject(this)});
        pInterpreter->doFunction(object, func, args);
    }
}
