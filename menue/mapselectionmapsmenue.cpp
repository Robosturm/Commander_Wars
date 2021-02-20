#include "menue/mapselectionmapsmenue.h"
#include "menue/mainwindow.h"
#include "menue/gamemenue.h"
#include "menue/campaignmenu.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/audiothread.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/cospritemanager.h"

#include "game/gamemap.h"
#include "game/campaign.h"
#include "game/player.h"
#include "game/co.h"

#include "objects/dialogs/editor/dialograndommap.h"
#include "objects/dialogs/filedialog.h"

#include "multiplayer/multiplayermenu.h"
#include "multiplayer/networkcommands.h"

MapSelectionMapsMenue::MapSelectionMapsMenue(qint32 heigth, spMapSelectionView pMapSelectionView)
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    this->moveToThread(pApp->getWorkerthread());

    Console::print("Entering Map Selection Menue", Console::eDEBUG);
    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    pBuildingSpriteManager->loadAll();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("mapselectionmenu");
    sprite->setResAnim(pBackground);
    // background should be last to draw
    sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
    sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
    sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());

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
    m_pButtonBack->setPosition(10, Settings::getHeight() - 10 - m_pButtonBack->getHeight());
    m_pButtonBack->attachTo(this);
    m_pButtonBack->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonBack();
    });
    connect(this, &MapSelectionMapsMenue::buttonBack, this, &MapSelectionMapsMenue::slotButtonBack, Qt::QueuedConnection);

    m_pButtonNext = ObjectManager::createButton(tr("Next"));
    m_pButtonNext->setPosition(Settings::getWidth() - 10 - m_pButtonNext->getWidth(), Settings::getHeight() - 10 - m_pButtonNext->getHeight());
    m_pButtonNext->attachTo(this);
    m_pButtonNext->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonNext();
    });
    connect(this, &MapSelectionMapsMenue::buttonNext, this, &MapSelectionMapsMenue::slotButtonNext, Qt::QueuedConnection);

    m_pRandomMap = ObjectManager::createButton(tr("Random Map"));
    m_pRandomMap->setPosition(m_pButtonBack->getX() + m_pButtonBack->getWidth() + 20, Settings::getHeight() - 10 - m_pButtonNext->getHeight());
    m_pRandomMap->attachTo(this);
    m_pRandomMap->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit randomMap();
    });
    connect(this, &MapSelectionMapsMenue::randomMap, this, &MapSelectionMapsMenue::showRandomMap, Qt::QueuedConnection);

    m_pButtonLoadRules = ObjectManager::createButton(tr("Load"));
    m_pButtonLoadRules->setPosition(Settings::getWidth() / 2 + 10, Settings::getHeight() - 10 - m_pButtonNext->getHeight());
    m_pButtonLoadRules->attachTo(this);
    m_pButtonLoadRules->setVisible(false);
    m_pButtonLoadRules->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowLoadRules();
    });
    connect(this, &MapSelectionMapsMenue::sigShowLoadRules, this, &MapSelectionMapsMenue::showLoadRules, Qt::QueuedConnection);

    m_pButtonSaveRules = ObjectManager::createButton(tr("Save"));
    m_pButtonSaveRules->setPosition(Settings::getWidth() / 2 - m_pButtonSaveRules->getWidth() - 10, Settings::getHeight() - 10 - m_pButtonNext->getHeight());
    m_pButtonSaveRules->attachTo(this);
    m_pButtonSaveRules->setVisible(false);
    m_pButtonSaveRules->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowSaveRules();
    });
    connect(this, &MapSelectionMapsMenue::sigShowSaveRules, this, &MapSelectionMapsMenue::showSaveRules, Qt::QueuedConnection);

    m_pButtonSaveMap = ObjectManager::createButton(tr("Save Map"));
    m_pButtonSaveMap->setPosition(m_pButtonBack->getX() + m_pButtonBack->getWidth() + 20, Settings::getHeight() - 10 - m_pButtonNext->getHeight());
    m_pButtonSaveMap->attachTo(this);
    m_pButtonSaveMap->setVisible(false);
    m_pButtonSaveMap->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowSaveMap();
    });
    connect(this, &MapSelectionMapsMenue::sigShowSaveMap, this, &MapSelectionMapsMenue::showSaveMap, Qt::QueuedConnection);


    m_pButtonStart = ObjectManager::createButton(tr("Start Game"));
    m_pButtonStart->setPosition(Settings::getWidth() - 10 - m_pButtonStart->getWidth(), Settings::getHeight() - 10 - m_pButtonStart->getHeight());
    m_pButtonStart->attachTo(this);
    m_pButtonStart->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonStartGame();
    });
    connect(this, &MapSelectionMapsMenue::buttonStartGame, this, &MapSelectionMapsMenue::startGame, Qt::QueuedConnection);

    qint32 yPos = 10;
    if (heigth <  0)
    {
        heigth = Settings::getHeight() - 40 * 2;
    }
    m_pPlayerSelection = new PlayerSelection(Settings::getWidth() - 20,
                                             heigth);
    m_pPlayerSelection->setPosition(10, yPos);
    addChild(m_pPlayerSelection);

    QSize size(Settings::getWidth() - 20, Settings::getHeight() - 40 * 2);
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
    connect(this, &MapSelectionMapsMenue::sigOnEnter, this, &MapSelectionMapsMenue::onEnter, Qt::QueuedConnection);
    emit sigOnEnter();
    pApp->continueRendering();
}

MapSelectionMapsMenue::~MapSelectionMapsMenue()
{

}

void MapSelectionMapsMenue::slotButtonBack()
{
    
    Console::print("slotButtonBack()", Console::eDEBUG);
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
    
}

void MapSelectionMapsMenue::slotButtonNext()
{    
    Console::print("slotButtonNext()", Console::eDEBUG);
    switch (m_MapSelectionStep)
    {
        case MapSelectionStep::selectMap:
        {
            m_pMapSelectionView->loadCurrentMap();
            QString file = m_pMapSelectionView->getMapSelection()->getCurrentFile();
            QString mapFile = m_pMapSelectionView->getCurrentFile().filePath();
            if ((m_pMapSelectionView->getCurrentMap() != nullptr && file.endsWith(".map")) ||
                (mapFile == NetworkCommands::RANDOMMAPIDENTIFIER) ||
                (mapFile == NetworkCommands::SERVERMAPIDENTIFIER))
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
}

void MapSelectionMapsMenue::mapSelectionItemClicked(QString item)
{
    
    QFileInfo info = m_pMapSelectionView->getMapSelection()->getCurrentFolder() + item;
    if (info.isFile())
    {
        m_pMapSelectionView->setCurrentFile(info.filePath());
        emit buttonNext();
    }
    
}

void MapSelectionMapsMenue::mapSelectionItemChanged(QString item)
{
    
    QFileInfo info = m_pMapSelectionView->getMapSelection()->getCurrentFolder() + item;
    m_pMapSelectionView->loadMap(info);
    
}

void MapSelectionMapsMenue::hideMapSelection()
{
    
    m_pRandomMap->setVisible(false);
    m_pMapSelectionView->setVisible(false);
    
}

void MapSelectionMapsMenue::showMapSelection()
{
    
    m_pRandomMap->setVisible(true);
    m_pMapSelectionView->setVisible(true);
    
}

void MapSelectionMapsMenue::hideRuleSelection()
{
    
    m_pRuleSelection->setVisible(false);
    m_pButtonSaveRules->setVisible(false);
    m_pButtonLoadRules->setVisible(false);
    if (m_pRuleSelectionView.get() != nullptr)
    {
        m_pRuleSelectionView->confirmRuleSelectionSetup();
        m_pRuleSelectionView = nullptr;
    }
    m_pRuleSelection->clearContent();
    
}

void MapSelectionMapsMenue::showRuleSelection()
{
    
    m_pRuleSelection->setVisible(true);
    m_pButtonSaveRules->setVisible(true);
    m_pButtonLoadRules->setVisible(true);
    m_pRuleSelection->clearContent();
    m_pRuleSelectionView = new RuleSelection(Settings::getWidth() - 80, RuleSelection::Mode::Singleplayer);
    m_pRuleSelection->addItem(m_pRuleSelectionView);
    m_pRuleSelection->setContentHeigth(m_pRuleSelectionView->getHeight() + 40);
    m_pRuleSelection->setContentWidth(m_pRuleSelectionView->getWidth());
    
}

void MapSelectionMapsMenue::showPlayerSelection()
{
    m_pButtonStart->setVisible(true);
    m_pButtonNext->setVisible(false);
    m_pButtonSaveMap->setVisible(true);
    m_pPlayerSelection->setVisible(true);
    m_pPlayerSelection->showPlayerSelection();
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
    Console::print("Start game", Console::eDEBUG);
    defeatClosedPlayers();
    spGameMap pMap = GameMap::getInstance();
    pMap->setVisible(false);
    pMap->initPlayersAndSelectCOs();
    pMap->setCampaign(m_pMapSelectionView->getCurrentCampaign());
    pMap->getGameScript()->gameStart();
    pMap->updateSprites(-1, -1, false, true);
    // start game
    Console::print("Leaving Map Selection Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new GameMenue(false, nullptr));
    oxygine::Actor::detach();    
}

void MapSelectionMapsMenue::defeatClosedPlayers()
{
    spGameMap pMap = GameMap::getInstance();
    if(!pMap->getGameScript()->immediateStart())
    {
        for (qint32 i = 0; i < m_pMapSelectionView->getCurrentMap()->getPlayerCount(); i++)
        {
            GameEnums::AiTypes aiType =  m_pPlayerSelection->getPlayerAiType(i);
            if (aiType == GameEnums::AiTypes::AiTypes_Closed)
            {
                Console::print("Defeating player " + QString::number(i) + " cause he's selected as closed player.", Console::eDEBUG);
                pMap->getPlayer(i)->setIsDefeated(true);
            }
        }
    }
}

void MapSelectionMapsMenue::showRandomMap()
{
    spDialogRandomMap pDialogRandomMap = new DialogRandomMap();
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
                                            bool unitsDistributed)
{
    
    spGameMap pGameMap = new GameMap(width, heigth, playerCount);
    pGameMap->randomMap(width, heigth, playerCount, roadSupport, seed,
                        terrains, buildings, ownedBaseSize,
                        startBaseSize / 100.0f,
                        units, unitCount, startBaseUnitSize / 100.0f, unitDistribution, unitsDistributed);
    pGameMap->setMapName(mapName);
    if (mapName.isEmpty())
    {
        pGameMap->setMapName("Random Map");
    }
    pGameMap->setMapAuthor(author);
    pGameMap->setMapDescription(description);
    m_pMapSelectionView->setCurrentFile(NetworkCommands::RANDOMMAPIDENTIFIER);
    m_pMapSelectionView->setCurrentMap(pGameMap.get());
    emit buttonNext();
    
}

void MapSelectionMapsMenue::showLoadRules()
{
    
    QVector<QString> wildcards;
    wildcards.append("*.grl");
    QString path = QCoreApplication::applicationDirPath() + "/data/gamerules";
    spFileDialog fileDialog = new FileDialog(path, wildcards);
    this->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &MapSelectionMapsMenue::loadRules, Qt::QueuedConnection);
    
}

void MapSelectionMapsMenue::showSaveRules()
{
    
    QVector<QString> wildcards;
    wildcards.append("*.grl");
    QString path = QCoreApplication::applicationDirPath() + "/data/gamerules";
    spFileDialog fileDialog = new FileDialog(path, wildcards);
    this->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &MapSelectionMapsMenue::saveRules, Qt::QueuedConnection);
    
}

void MapSelectionMapsMenue::loadRules(QString filename)
{
    

    if (filename.endsWith(".grl"))
    {
        QFile file(filename);
        if (file.exists())
        {
            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            QDataStream stream(&file);
            GameMap::getInstance()->getGameRules()->deserializeObject(stream);
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
        spGameMap pMap = GameMap::getInstance();
        pMap->getGameRules()->serializeObject(stream);
        file.close();
    }
    
}

void MapSelectionMapsMenue::showSaveMap()
{
    
    QVector<QString> wildcards;
    wildcards.append("*.map");
    QString path = QCoreApplication::applicationDirPath() + "/maps/";
    spFileDialog fileDialog = new FileDialog(path, wildcards);
    this->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &MapSelectionMapsMenue::saveMap, Qt::QueuedConnection);
    
}

void MapSelectionMapsMenue::saveMap(QString filename)
{
    
    if (filename.endsWith(".map"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        spGameMap pMap = GameMap::getInstance();
        pMap->serializeObject(stream);
        file.close();
    }
    
}

void MapSelectionMapsMenue::selectMap(QString folder, QString filename)
{
    QFileInfo info(folder + filename);
    m_pMapSelectionView->getMapSelection()->changeFolder(folder);
    m_pMapSelectionView->getMapSelection()->setCurrentItem(filename);
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
        QJSValueList args;
        QJSValue value = pInterpreter->newQObject(this);
        args << value;
        pInterpreter->doFunction(object, func, args);
    }
}
