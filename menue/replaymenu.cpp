#include <QMutexLocker>

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "menue/replaymenu.h"
#include "menue/victorymenue.h"

#include "game/gameanimation/gameanimationfactory.h"

#include "objects/dialogs/dialogmessagebox.h"
#include "objects/base/dropdownmenu.h"
#include "objects/base/dropdownmenucolor.h"
#include "objects/base/dropdownmenusprite.h"
#include "objects/base/slider.h"
#include "objects/base/checkbox.h"
#include "objects/base/moveinbutton.h"

#include "wiki/fieldinfo.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/gamemanager.h"

#include "ingamescriptsupport/genericbox.h"

#include "awbwReplayReader/awbwreplayplayer.h"

ReplayMenu::ReplayMenu(QString filename)
    : GameMenue(MemoryManagement::create<GameMap>(1, 1, 2), true)
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("ReplayMenu");
#endif
    registerAtInterpreter();
    setIsReplay(true);
    connect(this, &ReplayMenu::sigExitReplay, this, &ReplayMenu::exitReplay, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigShowRecordInvalid, this, &ReplayMenu::showRecordInvalid, Qt::QueuedConnection);
    connect(getActionPerformer(), &ActionPerformer::sigActionPerformed, this, &ReplayMenu::nextReplayAction, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigSwapPlay, this, &ReplayMenu::togglePlayUi, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigStartFastForward, this, &ReplayMenu::startFastForward, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigStopFastForward, this, &ReplayMenu::stopFastForward, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigShowConfig, this, &ReplayMenu::showConfig, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigOneStep, this, &ReplayMenu::oneStep, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigRewindDay, this, &ReplayMenu::rewindDay, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigRewindOneStep, this, &ReplayMenu::rewindOneStep, Qt::QueuedConnection);
    changeBackground("replaymenu");
    m_storedAnimationSettings.storeAnimationSettings();
    if (filename.endsWith(".zip"))
    {
        m_replayReader = MemoryManagement::create<AwbwReplayPlayer>(this, m_pMap.get());
    }
    else
    {
        m_replayReader = MemoryManagement::create<ReplayRecorder>(m_pMap.get());
    }
    connect(m_replayReader.get(), &IReplayReader::startReplay, this, &ReplayMenu::startReplay, Qt::QueuedConnection);
    m_valid = m_replayReader->loadRecord(filename);
    if (m_valid)
    {
        m_replayReader->requestReplayStart();
    }
}

ReplayMenu::~ReplayMenu()
{
    m_storedAnimationSettings.restoreAnimationSettings();
}

void ReplayMenu::startReplay()
{
    m_Viewplayer = MemoryManagement::create<Viewplayer>(this, m_pMap.get());
    fetchPlayerUiData();
    // store animation modes
    m_storedAnimationSettings.storeAnimationSettings();

    loadHandling();
    loadGameMenue();
    m_pMap->registerMapAtInterpreter();
    m_pMap->updateSprites();
    loadUIButtons();
    m_HumanInput = MemoryManagement::create<HumanPlayerInput>(m_pMap.get());
    m_HumanInput->init(this);
    m_gameStarted = true;
    CONSOLE_PRINT("emitting sigActionPerformed()", GameConsole::eDEBUG);
    emit getActionPerformer()->sigActionPerformed();
}

void ReplayMenu::onEnter()
{
}

void ReplayMenu::showRecordInvalid()
{    
    m_Focused = false;
    QString modList;
    QStringList mods = m_replayReader->getMods();
    for (auto& mod : mods)
    {
        modList += mod + "\n";
    }
    spDialogMessageBox pExit = MemoryManagement::create<DialogMessageBox>(tr("The current active mods or the current record are invalid or damaged! Exiting the Replay now. Mods used in the Replay:") + "\n" +
                                                          modList, false);
    connect(pExit.get(), &DialogMessageBox::sigOk, this, &ReplayMenu::exitReplay, Qt::QueuedConnection);
    addChild(pExit);
}

void ReplayMenu::exitReplay()
{    
    m_gameStarted = false;
    while (GameAnimationFactory::getAnimationCount() > 0)
    {
        GameAnimationFactory::finishAllAnimations();
    }
    CONSOLE_PRINT("Restoring interpreter after record replay", GameConsole::eDEBUG);
    Interpreter::reloadInterpreter(Interpreter::getInstance()->getRuntimeData());
    CONSOLE_PRINT("Leaving Replay Menue", GameConsole::eDEBUG);
    m_onEnterTimer.stop();
    auto window = MemoryManagement::create<VictoryMenue>(m_pMap, m_pNetworkInterface, true);
    oxygine::Stage::getStage()->addChild(window);
    oxygine::Actor::detach();
}

void ReplayMenu::nextReplayAction()
{
    QMutexLocker locker(&m_replayMutex);
    m_replayReader->onPostAction();
    if (m_pauseRequested && m_replayCounter == 0)
    {
        m_pauseRequested = false;
        m_paused = true;
    }
    if (!m_paused || m_replayCounter > 0)
    {
        spGameAction pAction = m_replayReader->nextAction();
        m_HumanInput->cleanUpInput();
        float progress = 0.0f;
        if (m_replayReader->getRecordSize() > 0)
        {
            progress = static_cast<float>(m_replayReader->getProgess()) / static_cast<float>(m_replayReader->getRecordSize());
        }
        m_progressBar->setScrollvalue(progress);
        if (pAction.get() != nullptr)
        {            
            --m_replayCounter;
            CONSOLE_PRINT("Performing next replay action", GameConsole::eDEBUG);
            pAction->setSyncCounter(getActionPerformer()->getSyncCounter() + 1);
            getActionPerformer()->setActionRunning(false);
            getActionPerformer()->performAction(pAction);
        }
        else
        {
            CONSOLE_PRINT("Pausing replay", GameConsole::eDEBUG);
            m_replayCounter = 0;
            swapPlay();
            togglePlayUi();
        }
        endOneStepRewind();
    }
}

void ReplayMenu::endOneStepRewind()
{
    if (m_rewindTarget >= 0 && m_replayReader->getProgess() >= m_rewindTarget)
    {
        m_storedSeekingAnimationSettings.restoreAnimationSettings();
        getActionPerformer()->skipAnimations(false);
        m_replayCounter = m_rewindReplayCounter;
        m_paused = m_rewindPause;
        m_rewindTarget = -1;
    }
}

void ReplayMenu::showExitGame()
{    
    m_Focused = false;
    spDialogMessageBox pExit = MemoryManagement::create<DialogMessageBox>(tr("Do you want to exit the current replay?"), true);
    connect(pExit.get(), &DialogMessageBox::sigOk, this, &ReplayMenu::exitReplay, Qt::QueuedConnection);
    connect(pExit.get(), &DialogMessageBox::sigCancel, this, [this]()
    {
        m_Focused = true;
    });
    addChild(pExit);
}

Player* ReplayMenu::getCurrentViewPlayer() const
{
    return m_Viewplayer.get();
}

void ReplayMenu::loadUIButtons()
{    
    loadSeekUi();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    m_taskBar = MemoryManagement::create<oxygine::Box9Sprite>();
    m_taskBar->setResAnim(pAnim);
    qint32 width = oxygine::Stage::getStage()->getWidth();
    if (!Settings::getInstance()->getSmallScreenDevice())
    {
        width += -m_IngameInfoBar->getWidth() - m_IngameInfoBar->getDetailedViewBox()->getScaledWidth();
    }
    m_taskBar->setSize(width, 50);
    m_taskBar->setPosition(0, oxygine::Stage::getStage()->getHeight() + 6);
    m_taskBar->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    addChild(m_taskBar);

    oxygine::spButton exitGame = pObjectManager->createButton(tr("Exit"), 130);
    exitGame->setPosition(m_taskBar->getScaledWidth() - 8 - exitGame->getScaledWidth(), 4);
    exitGame->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigShowExitGame();
    });
    m_taskBar->addChild(exitGame);

    qint32 y = 9;


    m_playButton = ObjectManager::createIconButton("play", 36);
    m_playButton->setVisible(false);
    m_pauseButton = ObjectManager::createIconButton("pause", 36);
    m_playButton->setPosition(exitGame->getX() - 4 - m_playButton->getScaledWidth(), y);
    m_pauseButton->setPosition(exitGame->getX() - 4 - m_pauseButton->getScaledWidth(), y);
    m_playButton->addClickListener([this](oxygine::Event * )
    {
        emit sigSwapPlay();
    });
    m_pauseButton->addClickListener([this](oxygine::Event * )
    {
        emit sigSwapPlay();
    });
    m_oneStepButton = ObjectManager::createIconButton("one_step", 36);
    m_oneStepButton->setPosition(m_playButton->getX() - 4 - m_oneStepButton->getScaledWidth(), y);
    m_taskBar->addChild(m_oneStepButton);
    m_oneStepButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigOneStep();
    });

    m_taskBar->addChild(m_playButton);
    m_taskBar->addChild(m_pauseButton);
    m_fastForwardButton = ObjectManager::createIconButton("fastforward", 36);
    m_fastForwardButton->setPosition(m_playButton->getX() - 4 - m_oneStepButton->getScaledWidth() - m_fastForwardButton->getScaledWidth(), y);
    m_taskBar->addChild(m_fastForwardButton);
    m_fastForwardButton->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [this](oxygine::Event*)
    {
        emit sigStartFastForward();
    });
    m_fastForwardButton->addEventListener(oxygine::TouchEvent::TOUCH_UP, [this](oxygine::Event*)
    {
        emit sigStopFastForward();
    });
    m_rewindDayButton = ObjectManager::createIconButton("rewindTurn", 36);
    m_rewindDayButton->setPosition(m_fastForwardButton->getX() - 4 - m_rewindDayButton->getScaledWidth(), y);
    m_rewindDayButton->addClickListener([this](oxygine::Event*)
    {
        emit sigRewindDay();
    });
    m_taskBar->addChild(m_rewindDayButton);

    m_rewindOneStepButton = ObjectManager::createIconButton("rewind", 36);
    m_rewindOneStepButton->setPosition(m_rewindDayButton->getX() - 4 - m_rewindOneStepButton->getScaledWidth(), y);
    m_rewindOneStepButton->addClickListener([this](oxygine::Event*)
    {
        emit sigRewindOneStep();
    });
    m_taskBar->addChild(m_rewindOneStepButton);

    m_configButton = ObjectManager::createIconButton("settings", 36);
    m_configButton->setPosition(m_rewindOneStepButton->getX() - 4 - m_configButton->getScaledWidth(), y);
    m_configButton->addClickListener([this](oxygine::Event*)
    {
        emit sigShowConfig();
    });
    m_taskBar->addChild(m_configButton);

    qint32 content = m_replayReader->getRecordSize() * actionPixelSize;
    if (content < exitGame->getX())
    {
        content = exitGame->getX() + 80;
    }
    m_progressBar = MemoryManagement::create<V_Scrollbar>(m_configButton->getX() - 10, content);
    connect(m_progressBar.get(), &V_Scrollbar::sigScrollValueChanged, this, &ReplayMenu::seekChanged, Qt::QueuedConnection);
    connect(m_progressBar.get(), &V_Scrollbar::sigStartEditValue, this, &ReplayMenu::startSeeking, Qt::QueuedConnection);
    connect(m_progressBar.get(), &V_Scrollbar::sigEndEditValue, this, &ReplayMenu::seekRecord, Qt::QueuedConnection);
    m_progressBar->setContentWidth(content);
    m_progressBar->setPosition(8, y);
    m_progressBar->setScrollspeed((m_configButton->getX() - 10) / m_replayReader->getDayFromPosition(m_replayReader->getRecordSize() - 1).day);
    m_taskBar->addChild(m_progressBar);
    auto moveButton = MemoryManagement::create<MoveInButton>(m_taskBar.get(), m_taskBar->getScaledHeight(),
                           -1, -1, 2.0f, true);
    m_taskBar->addChild(moveButton);

    pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pButtonBox = MemoryManagement::create<oxygine::Box9Sprite>();
    pButtonBox->setResAnim(pAnim);
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_xyTextInfo = MemoryManagement::create<Label>(180);
    m_xyTextInfo->setStyle(style);
    m_xyTextInfo->setHtmlText("X: 0 Y: 0");
    m_xyTextInfo->setPosition(8, 8);
    pButtonBox->addChild(m_xyTextInfo);
    pButtonBox->setSize(200, 50);
    pButtonBox->setPosition((oxygine::Stage::getStage()->getWidth() - m_IngameInfoBar->getScaledWidth()) - pButtonBox->getScaledWidth(), 0);
    pButtonBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_XYButtonBox = pButtonBox;
    m_XYButtonBox->setVisible(Settings::getInstance()->getShowIngameCoordinates());
    addChild(pButtonBox);
}

void ReplayMenu::oneStep()
{
    QMutexLocker locker(&m_replayMutex);
    ++m_replayCounter;
    if (m_paused)
    {
        m_paused = false;
        m_pauseRequested = true;
        emit getActionPerformer()->sigActionPerformed();
    }
}

void ReplayMenu::loadSeekUi()
{    
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pDayBox = MemoryManagement::create<oxygine::Box9Sprite>();
    pDayBox->setResAnim(pAnim);
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_seekDayLabel = MemoryManagement::create<Label>(200);
    m_seekDayLabel->setStyle(style);
    m_seekDayLabel->setHtmlText(tr("Day: "));
    m_seekDayLabel->setPosition(8, 8);
    pDayBox->addChild(m_seekDayLabel);
    pDayBox->setSize(220, 50);
    pDayBox->setPosition(0, oxygine::Stage::getStage()->getHeight() - pDayBox->getScaledHeight() + 6 - pDayBox->getScaledHeight());
    pDayBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    addChild(pDayBox);
    m_seekActor = pDayBox;
    m_seekActor->setVisible(false);
    
}

void ReplayMenu::startSeeking()
{
    QMutexLocker locker(&m_replayMutex);
    if (!m_paused)
    {
        swapPlay();
    }
    m_replayCounter = 0;

    m_storedSeekingAnimationSettings.startSeeking();
    GameAnimationFactory::clearAllAnimations();
    m_storedSeekingAnimationSettings.restoreAnimationSettings();
    m_seeking = true;
}

void ReplayMenu::seekChanged(float value)
{    
    m_seekActor->setVisible(true);
    qint32 count = static_cast<qint32>(static_cast<float>(m_replayReader->getRecordSize()) * value);
    IReplayReader::DayInfo dayInfo;
    if (m_replayReader->getRecordSize() > 0)
    {
        dayInfo = m_replayReader->getDayFromPosition(count);
    }
    m_seekDayLabel->setHtmlText(tr("Day: ") + QString::number(dayInfo.day) + tr(" Player: ") + QString::number(dayInfo.player + 1));
}

void ReplayMenu::seekRecord(float value)
{
    QMutexLocker locker(&m_replayMutex);
    qint32 count = static_cast<qint32>(static_cast<float>(m_replayReader->getRecordSize()) * value);
    IReplayReader::DayInfo dayInfo = m_replayReader->getDayFromPosition(count);
    seekToDay(dayInfo);
    m_seekActor->setVisible(false);
    m_seeking = false;
}

void ReplayMenu::rewindDay()
{
    startSeeking();
    QMutexLocker locker(&m_replayMutex);
    IReplayReader::DayInfo dayInfo;
    dayInfo.day = m_pMap->getCurrentDay();
    dayInfo.player = 0;
    auto currentTimestamp = QDateTime::currentMSecsSinceEpoch();
    constexpr auto msPerSec = 1000;
    if (currentTimestamp - m_lastRewind <= msPerSec * 10)
    {
        dayInfo.day -= 1;
    }
    m_lastRewind = currentTimestamp;
    if (dayInfo.day < 0)
    {
        dayInfo.day = 0;
    }
    seekToDay(dayInfo);
    m_seeking = false;    
}

void ReplayMenu::rewindOneStep()
{
    startSeeking();
    QMutexLocker locker(&m_replayMutex);
    m_rewindTarget = m_replayReader->getProgess() - 1;
    m_seeking = false;
    m_rewindReplayCounter = m_replayCounter;
    m_rewindPause = m_paused;
    m_paused = false;
    if (m_rewindTarget < 0)
    {
        m_rewindTarget = 0;
    }
    m_storedSeekingAnimationSettings.startSeeking();
    auto dayInfo = m_replayReader->getDayFromPosition(m_rewindTarget);
    CONSOLE_PRINT("Seeking to day " + QString::number(dayInfo.day) + " and player " + QString::number(dayInfo.player), GameConsole::eDEBUG);
    Mainapp::getInstance()->pauseRendering();

    // save map position and scale
    auto scale = m_pMap->getScale();
    auto slidingPos = m_mapSliding->getPosition();
    auto actorPos = m_mapSlidingActor->getPosition();
    // load map state during that day
    m_replayReader->seekToDay(dayInfo);
    m_mapSlidingActor->addChild(m_pMap);

    // restore map position and scale
    m_pMap->setScale(scale);
    m_mapSliding->setPosition(slidingPos);
    m_mapSlidingActor->setPosition(actorPos);
    updatePlayerUiData();
    m_pMap->updateSprites();
    m_pMap->getGameRules()->createFogVision();
    Mainapp::getInstance()->continueRendering();
    emit getActionPerformer()->sigActionPerformed();
}

void ReplayMenu::seekToDay(IReplayReader::DayInfo dayInfo)
{
    QMutexLocker locker(&m_replayMutex);
    if (m_replayReader->getRecordSize() > 0)
    {
        CONSOLE_PRINT("Seeking to day " + QString::number(dayInfo.day) + " and player " + QString::number(dayInfo.player), GameConsole::eDEBUG);
        Mainapp::getInstance()->pauseRendering();

        // save map position and scale
        auto scale = m_pMap->getScale();
        auto slidingPos = m_mapSliding->getPosition();
        auto actorPos = m_mapSlidingActor->getPosition();
        // load map state during that day
        m_replayReader->seekToDay(dayInfo);
        m_mapSlidingActor->addChild(m_pMap);
        // restore map position and scale
        m_pMap->setScale(scale);
        m_mapSliding->setPosition(slidingPos);
        m_mapSlidingActor->setPosition(actorPos);
        updatePlayerUiData();
        m_pMap->updateSprites();
        m_pMap->getGameRules()->createFogVision();
        Mainapp::getInstance()->continueRendering();
        if (!m_uiPause)
        {
            swapPlay();
        }
    }
}

Viewplayer* ReplayMenu::getViewplayer()
{
    return m_Viewplayer.get();
}

bool ReplayMenu::getValid() const
{
    return m_valid;
}

void ReplayMenu::fetchPlayerUiData()
{
    m_playerUiInfo.clear();
    for (qint32 i = 0; i <  m_pMap->getPlayerCount(); ++i)
    {
        Player* pPlayer = m_pMap->getPlayer(i);
        m_playerUiInfo.append(PlayerUiInfo());
        m_playerUiInfo[i].color = pPlayer->getColor();
        m_playerUiInfo[i].playerArmy = pPlayer->getPlayerArmy();
    }
}

void ReplayMenu::updatePlayerUiData()
{
    for (qint32 i = 0; i < m_playerUiInfo.size(); ++i)
    {
        Player* pPlayer = m_pMap->getPlayer(i);
        pPlayer->setColor(m_playerUiInfo[i].color);
        pPlayer->setPlayerArmy(m_playerUiInfo[i].playerArmy);
    }
}

void ReplayMenu::swapPlay()
{
    QMutexLocker locker(&m_replayMutex);
    if (m_paused)
    {
        CONSOLE_PRINT("emitting sigActionPerformed()", GameConsole::eDEBUG);
        m_paused = false;
        emit getActionPerformer()->sigActionPerformed();
    }
    else
    {
        m_paused = true;
    }
}

void ReplayMenu::togglePlayUi()
{
    QMutexLocker locker(&m_replayMutex);
    CONSOLE_PRINT("ReplayMenu::swapPlay()", GameConsole::eDEBUG);
    if (m_playButton->getVisible())
    {
        m_playButton->setVisible(false);
        m_pauseButton->setVisible(true);
        m_pauseRequested = false;
        m_uiPause = false;
        if (m_paused)
        {
            swapPlay();
        }
    }
    else
    {
        CONSOLE_PRINT("requesting pause", GameConsole::eDEBUG);
        m_playButton->setVisible(true);
        m_pauseButton->setVisible(false);
        m_pauseRequested = true;
        m_uiPause = true;
        m_replayCounter = 0;
    }
}

void ReplayMenu::startFastForward()
{
    QMutexLocker locker(&m_replayMutex);
    m_storedSeekingAnimationSettings.startSeeking();
    getActionPerformer()->skipAnimations(false);
}

void ReplayMenu::stopFastForward()
{
    QMutexLocker locker(&m_replayMutex);
    m_storedSeekingAnimationSettings.restoreAnimationSettings();
}

void ReplayMenu::showConfig()
{    
    if (m_pauseButton->getVisible())
    {
        swapPlay();
    }

    spGenericBox pBox = MemoryManagement::create<GenericBox>();

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    spPanel pPanel = MemoryManagement::create<Panel>(true, QSize(oxygine::Stage::getStage()->getWidth() - 60, oxygine::Stage::getStage()->getHeight() - 110),
                                     QSize(oxygine::Stage::getStage()->getWidth() - 60, oxygine::Stage::getStage()->getHeight() - 110));
    pPanel->setPosition(30, 30);
    pBox->addChild(pPanel);
    qint32 width = 450;
    qint32 y = 10;
    QVector<qint32> teams;
    QStringList teamNames;
    
    teamNames.append(tr("Current Team"));
    teamNames.append(tr("All Teams"));
    teamNames.append(tr("Map"));
    qint32 viewType = m_Viewplayer->getViewType();
    bool found = false;
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        qint32 team = m_pMap->getPlayer(i)->getTeam();
        if (!teams.contains(team))
        {
            if (team == viewType && !found)
            {
                viewType = teamNames.size();
                found = true;
            }
            teams.append(team);
            teamNames.append(tr("Team ") + QString::number(team + 1));
        }
    }
    spLabel pText = MemoryManagement::create<Label>(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Teamview:"));
    pText->setPosition(10, y);
    pPanel->addItem(pText);
    spDropDownmenu dropDown = MemoryManagement::create<DropDownmenu>(300, teamNames);
    dropDown->setPosition(width - 130, y);
    pPanel->addItem(dropDown);

    if (viewType < 0)
    {
        dropDown->setCurrentItem(viewType - Viewplayer::ViewType::CurrentTeam);
    }
    else
    {
        dropDown->setCurrentItem(viewType);
    }

    connect(dropDown.get(), &DropDownmenu::sigItemChanged, this, &ReplayMenu::setViewTeam, Qt::QueuedConnection);
    y += pText->getHeight() + 10;

    spLabel pTextfield = MemoryManagement::create<Label>(800);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Gameplay Settings"));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    y += pTextfield->getHeight() + 10;

    pTextfield = MemoryManagement::create<Label>(width - 140);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Overworld Animations: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spCheckbox pCheckbox = MemoryManagement::create<Checkbox>();
    pCheckbox->setTooltipText(tr("If active: walk, capture power animations, and so on will be shown."));
    pCheckbox->setChecked(Settings::getInstance()->getOverworldAnimations());
    connect(pCheckbox.get(), &Checkbox::checkChanged, Settings::getInstance(), &Settings::setOverworldAnimations, Qt::QueuedConnection);
    pCheckbox->setPosition(width - 130, y);
    pPanel->addItem(pCheckbox);
    y += pTextfield->getHeight() + 10;

    pTextfield = MemoryManagement::create<Label>(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Battle Animations: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    QStringList items = {tr("None"), tr("All"), tr("Own"), tr("Ally"), tr("Enemy")};
    spDropDownmenu pAnimationMode = MemoryManagement::create<DropDownmenu>(450, items);
    pAnimationMode->setCurrentItem(static_cast<qint32>(Settings::getInstance()->getBattleAnimationMode()));
    pAnimationMode->setPosition(width - 130, y);
    pAnimationMode->setTooltipText(tr("Select which in-game animations are played."));
    pPanel->addItem(pAnimationMode);
    connect(pAnimationMode.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        Settings::getInstance()->setBattleAnimationMode(static_cast<GameEnums::BattleAnimationMode>(value));
    });
    y += pTextfield->getHeight() + 10;

    pTextfield = MemoryManagement::create<Label>(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Battle Animations: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    items = {tr("Detailed"), tr("Overworld")};
    spDropDownmenu pBattleAnimationMode = MemoryManagement::create<DropDownmenu>(450, items);
    pBattleAnimationMode->setTooltipText(tr("Selects which battle animations are played during combat."));
    pBattleAnimationMode->setCurrentItem(static_cast<qint32>(Settings::getInstance()->getBattleAnimationType()));
    pBattleAnimationMode->setPosition(width - 130, y);
    pPanel->addItem(pBattleAnimationMode);
    connect(pBattleAnimationMode.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        Settings::getInstance()->setBattleAnimationType(static_cast<GameEnums::BattleAnimationType>(value));
    });
    y += pTextfield->getHeight() + 10;

    pTextfield = MemoryManagement::create<Label>(width - 140);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Dialogs: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    items = {tr("off"), tr("on")};
    spDropDownmenu pDialogAnimationMode = MemoryManagement::create<DropDownmenu>(450, items);
    pDialogAnimationMode->setTooltipText(tr("Selects if capture animations are shown or not."));
    pDialogAnimationMode->setCurrentItem(static_cast<qint32>(Settings::getInstance()->getDialogAnimation()));
    pDialogAnimationMode->setPosition(width - 130, y);
    pPanel->addItem(pDialogAnimationMode);
    connect(pDialogAnimationMode.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        Settings::getInstance()->setDialogAnimation(value);
    });
    y += pTextfield->getHeight() + 10;

    pTextfield = MemoryManagement::create<Label>(width - 140);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Capture: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    items = {tr("off"), tr("on")};
    spDropDownmenu pCaptureAnimationMode = MemoryManagement::create<DropDownmenu>(450, items);
    pCaptureAnimationMode->setTooltipText(tr("Selects if the dialogs are shown or not."));
    pCaptureAnimationMode->setCurrentItem(static_cast<qint32>(Settings::getInstance()->getCaptureAnimation()));
    pCaptureAnimationMode->setPosition(width - 130, y);
    pPanel->addItem(pCaptureAnimationMode);
    connect(pCaptureAnimationMode.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        Settings::getInstance()->setCaptureAnimation(value);
    });
    y += pTextfield->getHeight() + 10;

    pTextfield = MemoryManagement::create<Label>(width - 140);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Day 2 Day: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    items = {tr("off"), tr("on")};
    spDropDownmenu pDay2DayMode = MemoryManagement::create<DropDownmenu>(450, items);
    pDay2DayMode->setTooltipText(tr("Selects if the day to day screen gets skipped or not. Note on fog of war maps the screen is still shown."));
    pDay2DayMode->setCurrentItem(static_cast<qint32>(Settings::getInstance()->getDay2dayScreen()));
    pDay2DayMode->setPosition(width - 130, y);
    pPanel->addItem(pDay2DayMode);
    connect(pDay2DayMode.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        Settings::getInstance()->setDay2dayScreen(value);
    });
    y += pTextfield->getHeight() + 10;

    pTextfield = MemoryManagement::create<Label>(width - 140);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Movement: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    items = {tr("off"), tr("on")};
    spDropDownmenu pMovementAnimationMode = MemoryManagement::create<DropDownmenu>(450, items);
    pMovementAnimationMode->setTooltipText(tr("Selects if movement animations get shown or not."));
    pMovementAnimationMode->setCurrentItem(static_cast<qint32>(Settings::getInstance()->getMovementAnimations()));
    pMovementAnimationMode->setPosition(width - 130, y);
    pPanel->addItem(pMovementAnimationMode);
    connect(pMovementAnimationMode.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        Settings::getInstance()->setMovementAnimations(value);
    });
    y += pTextfield->getHeight() + 10;

    pTextfield = MemoryManagement::create<Label>(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Animation Speed: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spSlider pAnimationSpeed = MemoryManagement::create<Slider>(oxygine::Stage::getStage()->getWidth() - 60 - width, 1, 100, "");
    pAnimationSpeed->setTooltipText(tr("Selects the speed at which animations are played. Note: This does not include capture or battle animations."));
    pAnimationSpeed->setPosition(width - 130, y);
    pAnimationSpeed->setCurrentValue(static_cast<qint32>(Settings::getInstance()->getAnimationSpeedValue()));
    pPanel->addItem(pAnimationSpeed);
    connect(pAnimationSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::getInstance()->setAnimationSpeed(static_cast<quint32>(value));
    });
    y += pTextfield->getHeight() + 10;

    pTextfield = MemoryManagement::create<Label>(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Walk Speed: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spSlider pWalkSpeed = MemoryManagement::create<Slider>(oxygine::Stage::getStage()->getWidth() - 60 - width, 1, 100, "");
    pWalkSpeed->setTooltipText(tr("Selects the speed at which units walk across the map."));
    pWalkSpeed->setPosition(width - 130, y);
    pWalkSpeed->setCurrentValue(static_cast<qint32>(Settings::getInstance()->getWalkAnimationSpeedValue()));
    pPanel->addItem(pWalkSpeed);
    connect(pWalkSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::getInstance()->setWalkAnimationSpeed(static_cast<quint32>(value));
    });
    y += pTextfield->getHeight() + 10;

    pTextfield = MemoryManagement::create<Label>(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Battle Anim. Speed: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spSlider pBattleAnimationSpeed = MemoryManagement::create<Slider>(oxygine::Stage::getStage()->getWidth() - 60 - width, 1, 100, "");
    pBattleAnimationSpeed->setTooltipText(tr("Selects the speed at which battle animations are played."));
    pBattleAnimationSpeed->setPosition(width - 130, y);
    pBattleAnimationSpeed->setCurrentValue(static_cast<qint32>(Settings::getInstance()->getBattleAnimationSpeedValue()));
    pPanel->addItem(pBattleAnimationSpeed);
    connect(pBattleAnimationSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::getInstance()->setBattleAnimationSpeed(static_cast<quint32>(value));
    });
    y += pTextfield->getHeight() + 10;

    pTextfield = MemoryManagement::create<Label>(width - 140);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Capture Anim. Speed: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spSlider pCaptureAnimationSpeed = MemoryManagement::create<Slider>(oxygine::Stage::getStage()->getWidth() - 60 - width, 1, 100, "");
    pCaptureAnimationSpeed->setTooltipText(tr("Selects the speed at which capture animations are played."));
    pCaptureAnimationSpeed->setPosition(width - 130, y);
    pCaptureAnimationSpeed->setCurrentValue(static_cast<qint32>(Settings::getInstance()->getCaptureAnimationSpeedValue()));
    pPanel->addItem(pCaptureAnimationSpeed);
    connect(pCaptureAnimationSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::getInstance()->setCaptureAnimationSpeed(static_cast<quint32>(value));
    });
    y += pTextfield->getHeight() + 10;

    pTextfield = MemoryManagement::create<Label>(width - 140);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Dialog Speed: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spSlider pDialogAnimationSpeed = MemoryManagement::create<Slider>(oxygine::Stage::getStage()->getWidth() - 60 - width, 1, 100, "");
    pDialogAnimationSpeed->setTooltipText(tr("Selects the speed at which dialog animations are played."));
    pDialogAnimationSpeed->setPosition(width - 130, y);
    pDialogAnimationSpeed->setCurrentValue(static_cast<qint32>(Settings::getInstance()->getDialogAnimationSpeedValue()));
    pPanel->addItem(pDialogAnimationSpeed);
    connect(pDialogAnimationSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::getInstance()->setDialogAnimationSpeed(static_cast<quint32>(value));
    });
    y += pTextfield->getHeight() + 10;

    pTextfield = MemoryManagement::create<Label>(800);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Player settings"));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    y += pTextfield->getHeight() + 10;

    for (qint32 i = 0; i < m_pMap->getPlayerCount(); ++i)
    {
        Player* pPlayer = m_pMap->getPlayer(i);
        pTextfield = MemoryManagement::create<Label>(width - 140);
        pTextfield->setStyle(style);
        pTextfield->setHtmlText(pPlayer->getPlayerNameId());
        pTextfield->setPosition(10, y);
        pPanel->addItem(pTextfield);

        auto playerColorNames = Player::getDropDownColorNames();
        QVector<QColor> playerColors;
        for (const auto & name : playerColorNames)
        {
            playerColors.append(name);
        }
        spDropDownmenuColor pPlayerColor = MemoryManagement::create<DropDownmenuColor>(110, playerColors);
        pPlayerColor->setCurrentItem(pPlayer->getColor());
        connect(pPlayerColor.get(),  &DropDownmenuColor::sigItemChanged, this, [this, i](QColor displayColor)
        {
            QColor tableColor = Player::displayColorToTableColor(displayColor);
            Player* pPlayer = m_pMap->getPlayer(i);
            pPlayer->setColor(tableColor);
            m_playerUiInfo[i].color = pPlayer->getColor();
            m_pMap->updateSprites();
        });
        pPlayerColor->setPosition(width - 130, y);
        pPanel->addItem(pPlayerColor);

        GameManager* pGameManager = GameManager::getInstance();
        std::function<oxygine::spActor(QString item)> creator = [pGameManager](QString army)
        {
            oxygine::ResAnim* pAnim = pGameManager->getResAnim("icon_" + army.toLower());
            oxygine::spSprite ret = MemoryManagement::create<oxygine::Sprite>();
            ret->setResAnim(pAnim);
            return ret;
        };
        spDropDownmenuSprite pPlayerArmy = MemoryManagement::create<DropDownmenuSprite>(110, Player::getSelectableArmies(), creator, 30);
        pPlayerArmy->setCurrentItem(pPlayer->getPlayerArmy());
        connect(pPlayerArmy.get(),  &DropDownmenuSprite::sigItemString, this, [this, i](QString army)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            pPlayer->setPlayerArmy(army);
            m_playerUiInfo[i].playerArmy = pPlayer->getPlayerArmy();
            m_pMap->updateSprites();
        });
        pPlayerArmy->setPosition(pPlayerColor->getX() + pPlayerColor->getWidth() + 10, y);
        pPanel->addItem(pPlayerArmy);
        y += pTextfield->getHeight() + 10;
    }


    pPanel->setContentHeigth(y + 20);
    pPanel->setContentWidth(width + 10);
    addChild(pBox);
}

void ReplayMenu::setViewTeam(qint32 item)
{
    if (item <= -Viewplayer::ViewType::CurrentTeam)
    {
        m_Viewplayer->setViewType(item + Viewplayer::ViewType::CurrentTeam);
    }
    else
    {
        QVector<qint32> teams;
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            qint32 team = m_pMap->getPlayer(i)->getTeam();
            if (!teams.contains(team))
            {
                if (teams.size() == item + Viewplayer::ViewType::CurrentTeam)
                {
                    m_Viewplayer->setViewType(team);
                    break;
                }
                teams.append(team);
            }
        }
    }
    m_pMap->getGameRules()->createFogVision();
}

void ReplayMenu::keyInput(oxygine::KeyEvent event)
{
    if (!event.getContinousPress())
    {
        // for debugging
        Qt::Key cur = event.getKey();
        if (m_Focused)
        {
            if (cur == Settings::getInstance()->getKey_information() ||
                     cur == Settings::getInstance()->getKey_information2())
            {

                Player* pPlayer = m_pMap->getCurrentViewPlayer();
                GameEnums::VisionType visionType = pPlayer->getFieldVisibleType(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
                if (m_pMap->onMap(m_Cursor->getMapPointX(), m_Cursor->getMapPointY()) &&
                    visionType != GameEnums::VisionType_Shrouded)
                {
                    spTerrain pTerrain = m_pMap->getSpTerrain(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
                    spUnit pUnit = pTerrain->getSpUnit();
                    if (pUnit != nullptr && pUnit->isStealthed(pPlayer))
                    {
                        pUnit = nullptr;
                    }
                    spFieldInfo fieldinfo = MemoryManagement::create<FieldInfo>(pTerrain, pUnit);
                    addChild(fieldinfo);
                    connect(fieldinfo.get(), &FieldInfo::sigFinished, this, [this]
                    {
                        setFocused(true);
                    });
                    setFocused(false);
                }

            }
        }
    }
    BaseGamemenu::keyInput(event);
}
