#include <QMutexLocker>

#include "menue/replaymenu.h"
#include "menue/victorymenue.h"

#include "game/gameanimation/gameanimationfactory.h"

#include "objects/dialogs/dialogmessagebox.h"
#include "objects/base/dropdownmenu.h"
#include "objects/base/slider.h"
#include "objects/base/checkbox.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "ingamescriptsupport/genericbox.h"

ReplayMenu::ReplayMenu(QString filename)
    : GameMenue(spGameMap::create(1, 1, 2))
{
    Interpreter::setCppOwnerShip(this);
    setObjectName("ReplayMenu");
    setIsReplay(true);
    connect(this, &ReplayMenu::sigExitReplay, this, &ReplayMenu::exitReplay, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigShowRecordInvalid, this, &ReplayMenu::showRecordInvalid, Qt::QueuedConnection);
    connect(this, &GameMenue::sigActionPerformed, this, &ReplayMenu::nextReplayAction, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigSwapPlay, this, &ReplayMenu::togglePlayUi, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigStartFastForward, this, &ReplayMenu::startFastForward, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigStopFastForward, this, &ReplayMenu::stopFastForward, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigShowConfig, this, &ReplayMenu::showConfig, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigOneStep, this, &ReplayMenu::oneStep, Qt::QueuedConnection);
    changeBackground("replaymenu");
    m_valid = m_ReplayRecorder.loadRecord(filename);
    if (m_valid)
    {
        m_Viewplayer = spViewplayer::create(m_pMap.get());
        // store animation modes
        m_storedOverworldAnimations = Settings::getOverworldAnimations();
        m_storedBattleAnimMode = Settings::getBattleAnimationMode();
        m_storedBatteAnimType = Settings::getBattleAnimationType();
        m_storedDialog = Settings::getDialogAnimation();

        m_storedAnimationSpeed = Settings::getAnimationSpeedValue();
        m_storedBattleAnimationSpeed = Settings::getBattleAnimationSpeedValue();
        m_storedDialogAnimationSpeed = Settings::getDialogAnimationSpeed();
        m_storedCaptureAnimationSpeed = Settings::getCaptureAnimationSpeed();
        
        loadHandling();
        loadGameMenue();
        m_pMap->registerMapAtInterpreter();
        m_pMap->updateSprites();
        loadUIButtons();
        m_HumanInput = spHumanPlayerInput::create(m_pMap.get());
        m_HumanInput->init();
        m_gameStarted = true;
        CONSOLE_PRINT("emitting sigActionPerformed()", Console::eDEBUG);
    }
}

void ReplayMenu::onEnter()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString object = "Init";
    QString func = "replayMenu";
    if (pInterpreter->exists(object, func))
    {
        CONSOLE_PRINT("Executing:" + object + "." + func, Console::eDEBUG);
        QJSValueList args;
        QJSValue value = pInterpreter->newQObject(this);
        args << value;
        pInterpreter->doFunction(object, func, args);
    }
    
    if (m_pMap.get() != nullptr &&
        m_pMap->getGameScript() != nullptr)
    {
        m_pMap->getGameScript()->onGameLoaded(this);
    }
    if (m_valid)
    {
        emit sigActionPerformed();
    }
}

ReplayMenu::~ReplayMenu()
{
    Settings::setOverworldAnimations(m_storedOverworldAnimations);
    Settings::setBattleAnimationMode(m_storedBattleAnimMode);
    Settings::setBattleAnimationType(m_storedBatteAnimType);
    Settings::setDialogAnimation(m_storedDialog);

    Settings::setAnimationSpeed(m_storedAnimationSpeed);
    Settings::setBattleAnimationSpeed(m_storedBattleAnimationSpeed);
    Settings::setDialogAnimationSpeed(m_storedDialogAnimationSpeed);
    Settings::setCaptureAnimationSpeed(m_storedCaptureAnimationSpeed);
}

void ReplayMenu::showRecordInvalid()
{    
    m_Focused = false;
    QString modList;
    QStringList mods = m_ReplayRecorder.getMods();
    for (auto& mod : mods)
    {
        modList += mod + "\n";
    }
    spDialogMessageBox pExit = spDialogMessageBox::create(tr("The current active mods or the current record are invalid! Exiting the Replay now. Mods used in the Replay:") + "\n" +
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
    CONSOLE_PRINT("Restoring interpreter after record replay", Console::eDEBUG);
    Interpreter::reloadInterpreter(Interpreter::getRuntimeData());
    CONSOLE_PRINT("Leaving Replay Menue", Console::eDEBUG);
    auto window = spVictoryMenue::create(m_pMap, m_pNetworkInterface, true);
    oxygine::Stage::getStage()->addChild(window);
    deleteMenu();
}

void ReplayMenu::nextReplayAction()
{
    QMutexLocker locker(&m_replayMutex);
    if (m_pauseRequested && m_replayCounter == 0)
    {
        m_pauseRequested = false;
        m_paused = true;
    }
    if (!m_paused || m_replayCounter > 0)
    {
        spGameAction pAction = m_ReplayRecorder.nextAction();
        m_HumanInput->cleanUpInput();
        float progress = 0.0f;
        if (m_ReplayRecorder.getRecordSize() > 0)
        {
            progress = static_cast<float>(m_ReplayRecorder.getProgess()) / static_cast<float>(m_ReplayRecorder.getRecordSize());
        }
        m_progressBar->setScrollvalue(progress);
        if (pAction.get() != nullptr)
        {
            --m_replayCounter;
            CONSOLE_PRINT("Performing next replay action", Console::eDEBUG);
            performAction(pAction);
        }
        else
        {
            CONSOLE_PRINT("Pausing replay", Console::eDEBUG);
            m_replayCounter = 0;
            swapPlay();
            togglePlayUi();
        }
    }
}

void ReplayMenu::showExitGame()
{    
    m_Focused = false;
    spDialogMessageBox pExit = spDialogMessageBox::create(tr("Do you want to exit the current replay?"), true);
    connect(pExit.get(), &DialogMessageBox::sigOk, this, &ReplayMenu::exitReplay, Qt::QueuedConnection);
    connect(pExit.get(), &DialogMessageBox::sigCancel, this, [=]()
    {
        m_Focused = true;
    });
    addChild(pExit);    
}

Player* ReplayMenu::getCurrentViewPlayer()
{
    return m_Viewplayer.get();
}

void ReplayMenu::loadUIButtons()
{    
    loadSeekUi();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pButtonBox = oxygine::spBox9Sprite::create();
    pButtonBox->setResAnim(pAnim);
    qint32 width = Settings::getWidth();
    if (!Settings::getSmallScreenDevice())
    {
        width += -m_IngameInfoBar->getWidth() - m_IngameInfoBar->getDetailedViewBox()->getWidth();
    }
    pButtonBox->setSize(width, 50);
    pButtonBox->setPosition(0, Settings::getHeight() - pButtonBox->getHeight() + 6);
    pButtonBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    addChild(pButtonBox);

    oxygine::spButton exitGame = pObjectManager->createButton(tr("Exit"), 130);
    exitGame->setPosition(pButtonBox->getWidth() - 8 - exitGame->getWidth(), 4);
    exitGame->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowExitGame();
    });
    pButtonBox->addChild(exitGame);

    qint32 y = 9;


    m_playButton = ObjectManager::createIconButton("play", 36);
    m_playButton->setVisible(false);
    m_pauseButton = ObjectManager::createIconButton("pause", 36);
    m_playButton->setPosition(exitGame->getX() - 4 - m_playButton->getWidth(), y);
    m_pauseButton->setPosition(exitGame->getX() - 4 - m_pauseButton->getWidth(), y);
    m_playButton->addClickListener([=](oxygine::Event * )
    {
        emit sigSwapPlay();
    });
    m_pauseButton->addClickListener([=](oxygine::Event * )
    {
        emit sigSwapPlay();
    });
    m_oneStepButton = ObjectManager::createIconButton("one_step", 36);
    m_oneStepButton->setPosition(m_playButton->getX() - 4 - m_oneStepButton->getWidth(), y);
    pButtonBox->addChild(m_oneStepButton);
    m_oneStepButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
    {
        emit sigOneStep();
    });

    pButtonBox->addChild(m_playButton);
    pButtonBox->addChild(m_pauseButton);
    m_fastForwardButton = ObjectManager::createIconButton("fastforward", 36);
    m_fastForwardButton->setPosition(m_playButton->getX() - 4 - m_oneStepButton->getWidth() - m_fastForwardButton->getWidth(), y);
    pButtonBox->addChild(m_fastForwardButton);
    m_fastForwardButton->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [=](oxygine::Event*)
    {
        emit sigStartFastForward();
    });
    m_fastForwardButton->addEventListener(oxygine::TouchEvent::TOUCH_UP, [=](oxygine::Event*)
    {
        emit sigStopFastForward();
    });
    m_configButton = ObjectManager::createIconButton("settings", 36);
    m_configButton->setPosition(m_fastForwardButton->getX() - 4 - m_configButton->getWidth(), y);
    m_configButton->addClickListener([=](oxygine::Event*)
    {
        emit sigShowConfig();
    });
    pButtonBox->addChild(m_configButton);

    qint32 content = m_ReplayRecorder.getRecordSize() * actionPixelSize;
    if (content < exitGame->getX())
    {
        content = exitGame->getX() + 80;
    }
    m_progressBar = spV_Scrollbar::create(m_configButton->getX() - 10, content);
    connect(m_progressBar.get(), &V_Scrollbar::sigScrollValueChanged, this, &ReplayMenu::seekChanged, Qt::QueuedConnection);
    connect(m_progressBar.get(), &V_Scrollbar::sigStartEditValue, this, &ReplayMenu::startSeeking, Qt::QueuedConnection);
    connect(m_progressBar.get(), &V_Scrollbar::sigEndEditValue, this, &ReplayMenu::seekRecord, Qt::QueuedConnection);
    m_progressBar->setContentWidth(content);
    m_progressBar->setPosition(8, y);
    m_progressBar->setScrollspeed((m_configButton->getX() - 10) / m_ReplayRecorder.getDayFromPosition(m_ReplayRecorder.getRecordSize() - 1));

    pButtonBox->addChild(m_progressBar);
    pAnim = pObjectManager->getResAnim("panel");
    pButtonBox = oxygine::spBox9Sprite::create();
    pButtonBox->setResAnim(pAnim);
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_xyTextInfo = spLabel::create(180);
    m_xyTextInfo->setStyle(style);
    m_xyTextInfo->setHtmlText("X: 0 Y: 0");
    m_xyTextInfo->setPosition(8, 8);
    pButtonBox->addChild(m_xyTextInfo);
    pButtonBox->setSize(200, 50);
    pButtonBox->setPosition((Settings::getWidth() - m_IngameInfoBar->getScaledWidth())  - pButtonBox->getWidth(), 0);
    pButtonBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_XYButtonBox = pButtonBox;
    m_XYButtonBox->setVisible(Settings::getShowIngameCoordinates());
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
        emit sigActionPerformed();
    }
}

void ReplayMenu::loadSeekUi()
{    
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pDayBox = oxygine::spBox9Sprite::create();
    pDayBox->setResAnim(pAnim);
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_seekDayLabel = spLabel::create(140);
    m_seekDayLabel->setStyle(style);
    m_seekDayLabel->setHtmlText(tr("Day: "));
    m_seekDayLabel->setPosition(8, 8);
    pDayBox->addChild(m_seekDayLabel);
    pDayBox->setSize(160, 50);
    pDayBox->setPosition(0, Settings::getHeight() - pDayBox->getHeight() + 6 - pDayBox->getHeight());
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
    m_seekingOverworldAnimations = Settings::getOverworldAnimations();
    m_seekingDialog = Settings::getDialogAnimation();
    m_seekingBattleAnimations = Settings::getBattleAnimationMode();
    Settings::setOverworldAnimations(false);
    Settings::setDialogAnimation(false);
    Settings::setBattleAnimationMode(GameEnums::BattleAnimationMode::BattleAnimationMode_None);

    if (GameAnimationFactory::getAnimationCount() > 0)
    {
        GameAnimationFactory::finishAllAnimations();
    }
    Settings::setBattleAnimationMode(m_seekingBattleAnimations);
    Settings::setOverworldAnimations(m_seekingOverworldAnimations);
    Settings::setDialogAnimation(m_seekingDialog);

    m_seeking = true;    
}

void ReplayMenu::seekChanged(float value)
{    
    m_seekActor->setVisible(true);
    qint32 count = static_cast<qint32>(static_cast<float>(m_ReplayRecorder.getRecordSize()) * value);
    qint32 day = 0;
    if (m_ReplayRecorder.getRecordSize() > 0)
    {
        day = m_ReplayRecorder.getDayFromPosition(count);
    }
    m_seekDayLabel->setHtmlText(tr("Day: ") + QString::number(day));    
}

void ReplayMenu::seekRecord(float value)
{
    QMutexLocker locker(&m_replayMutex);    
    qint32 count = static_cast<qint32>(static_cast<float>(m_ReplayRecorder.getRecordSize()) * value);
    qint32 day = m_ReplayRecorder.getDayFromPosition(count);
    seekToDay(day);
    m_seekActor->setVisible(false);
    m_seeking = false;    
}

void ReplayMenu::seekToDay(qint32 day)
{
    QMutexLocker locker(&m_replayMutex);
    if (m_ReplayRecorder.getRecordSize() > 0)
    {
        CONSOLE_PRINT("Seeking to day " + QString::number(day), Console::eDEBUG);
        Mainapp::getInstance()->pauseRendering();
        
        // save map position and scale
        auto scale = m_pMap->getScale();
        auto slidingPos = m_mapSliding->getPosition();
        auto actorPos = m_mapSlidingActor->getPosition();
        // load map state during that day
        m_ReplayRecorder.seekToDay(day);
        m_pMap->registerMapAtInterpreter();
        m_mapSlidingActor->addChild(m_pMap);
        // restore map position and scale
        m_pMap->setScale(scale);
        m_mapSliding->setPosition(slidingPos);
        m_mapSlidingActor->setPosition(actorPos);
        m_pMap->updateSprites();
        m_pMap->getGameRules()->createFogVision();
        updatePlayerinfo();
        Mainapp::getInstance()->continueRendering();
        connectMap();
        connectMapCursor();
        if (!m_uiPause)
        {
            swapPlay();
        }
    }
}

bool ReplayMenu::getValid() const
{
    return m_valid;
}

void ReplayMenu::swapPlay()
{
    QMutexLocker locker(&m_replayMutex);
    if (m_paused)
    {
        CONSOLE_PRINT("emitting sigActionPerformed()", Console::eDEBUG);
        m_paused = false;
        emit sigActionPerformed();
    }
    else
    {
        m_paused = true;
    }
}

void ReplayMenu::togglePlayUi()
{
    QMutexLocker locker(&m_replayMutex);
    CONSOLE_PRINT("ReplayMenu::swapPlay()", Console::eDEBUG);
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
        CONSOLE_PRINT("requesting pause", Console::eDEBUG);
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
    m_seekingOverworldAnimations = Settings::getOverworldAnimations();
    m_seekingDialog = Settings::getDialogAnimation();
    m_seekingBattleAnimations = Settings::getBattleAnimationMode();
    Settings::setOverworldAnimations(false);
    Settings::setDialogAnimation(false);
    Settings::setBattleAnimationMode(GameEnums::BattleAnimationMode::BattleAnimationMode_None);

    skipAnimations(false);
    
}

void ReplayMenu::stopFastForward()
{
    QMutexLocker locker(&m_replayMutex);
    Settings::setBattleAnimationMode(m_seekingBattleAnimations);
    Settings::setOverworldAnimations(m_seekingOverworldAnimations);
    Settings::setDialogAnimation(m_seekingDialog);
}

void ReplayMenu::showConfig()
{    
    if (m_pauseButton->getVisible())
    {
        swapPlay();
    }

    spGenericBox pBox = spGenericBox::create();

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    spPanel pPanel = spPanel::create(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 110),
                         QSize(Settings::getWidth() - 60, Settings::getHeight() - 110));
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
    spLabel pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Teamview:"));
    pText->setPosition(10, y);
    pPanel->addItem(pText);
    spDropDownmenu dropDown = spDropDownmenu::create(300, teamNames);
    dropDown->setPosition(width, y);
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
    y += 40;

    spLabel pTextfield = spLabel::create(800);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Gameplay Settings"));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    y += 40;

    pTextfield = spLabel::create(width - 140);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Overworld Animations: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spCheckbox pCheckbox = spCheckbox::create();
    pCheckbox->setTooltipText(tr("If active walk, capture power animations and so on will be shown"));
    pCheckbox->setChecked(Settings::getOverworldAnimations());
    connect(pCheckbox.get(), &Checkbox::checkChanged, Settings::getInstance(), &Settings::setOverworldAnimations, Qt::QueuedConnection);
    pCheckbox->setPosition(width - 130, y);
    pPanel->addItem(pCheckbox);
    y += 40;

    pTextfield = spLabel::create(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Battle Animations: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    QStringList items = {tr("None"), tr("All"), tr("Own"), tr("Ally"), tr("Enemy")};
    spDropDownmenu pAnimationMode = spDropDownmenu::create(450, items);
    pAnimationMode->setCurrentItem(static_cast<qint32>(Settings::getBattleAnimationMode()));
    pAnimationMode->setPosition(width - 130, y);
    pAnimationMode->setTooltipText(tr("Select which ingame animations are played."));
    pPanel->addItem(pAnimationMode);
    connect(pAnimationMode.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        Settings::setBattleAnimationMode(static_cast<GameEnums::BattleAnimationMode>(value));
    });
    y += 40;

    pTextfield = spLabel::create(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Battle Animations: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    items = {tr("Detailed"), tr("Overworld")};
    spDropDownmenu pBattleAnimationMode = spDropDownmenu::create(450, items);
    pBattleAnimationMode->setTooltipText(tr("Selects which battle animations are played when fighting an enemy."));
    pBattleAnimationMode->setCurrentItem(static_cast<qint32>(Settings::getBattleAnimationType()));
    pBattleAnimationMode->setPosition(width - 130, y);
    pPanel->addItem(pBattleAnimationMode);
    connect(pBattleAnimationMode.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        Settings::setBattleAnimationType(static_cast<GameEnums::BattleAnimationType>(value));
    });
    y += 40;

    pTextfield = spLabel::create(width - 140);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Dialogs: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    items = {tr("off"), tr("on")};
    spDropDownmenu pDialogAnimationMode = spDropDownmenu::create(450, items);
    pDialogAnimationMode->setTooltipText(tr("Selects if the dialogs are shown or not."));
    pDialogAnimationMode->setCurrentItem(static_cast<qint32>(Settings::getDialogAnimation()));
    pDialogAnimationMode->setPosition(width - 130, y);
    pPanel->addItem(pDialogAnimationMode);
    connect(pDialogAnimationMode.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        Settings::setDialogAnimation(value);
    });
    y += 40;

    pTextfield = spLabel::create(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Animation Speed: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spSlider pAnimationSpeed = spSlider::create(Settings::getWidth() - 20 - width, 1, 100, "");
    pAnimationSpeed->setTooltipText(tr("Selects the speed at which animations are played. Except battle animations."));
    pAnimationSpeed->setPosition(width - 150, y);
    pAnimationSpeed->setCurrentValue(static_cast<qint32>(Settings::getAnimationSpeedValue()));
    pPanel->addItem(pAnimationSpeed);
    connect(pAnimationSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::setAnimationSpeed(static_cast<quint32>(value));
    });
    y += 40;

    pTextfield = spLabel::create(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Walk Speed: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spSlider pWalkSpeed = spSlider::create(Settings::getWidth() - 20 - width, 1, 100, "");
    pWalkSpeed->setTooltipText(tr("Selects the speed at which units walk across the map."));
    pWalkSpeed->setPosition(width - 150, y);
    pWalkSpeed->setCurrentValue(static_cast<qint32>(Settings::getWalkAnimationSpeedValue()));
    pPanel->addItem(pWalkSpeed);
    connect(pWalkSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::setWalkAnimationSpeed(static_cast<quint32>(value));
    });
    y += 40;

    pTextfield = spLabel::create(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Battle Anim. Speed: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spSlider pBattleAnimationSpeed = spSlider::create(Settings::getWidth() - 20 - width, 1, 100, "");
    pBattleAnimationSpeed->setTooltipText(tr("Selects the speed at which battle animations are played."));
    pBattleAnimationSpeed->setPosition(width - 150, y);
    pBattleAnimationSpeed->setCurrentValue(static_cast<qint32>(Settings::getBattleAnimationSpeedValue()));
    pPanel->addItem(pBattleAnimationSpeed);
    connect(pBattleAnimationSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::setBattleAnimationSpeed(static_cast<quint32>(value));
    });
    y += 40;

    pTextfield = spLabel::create(width - 140);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Capture Anim. Speed: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spSlider pCaptureAnimationSpeed = spSlider::create(Settings::getWidth() - 20 - width, 1, 100, "");
    pCaptureAnimationSpeed->setTooltipText(tr("Selects the speed at which capture animations are played."));
    pCaptureAnimationSpeed->setPosition(width - 130, y);
    pCaptureAnimationSpeed->setCurrentValue(static_cast<qint32>(Settings::getCaptureAnimationSpeedValue()));
    pPanel->addItem(pCaptureAnimationSpeed);
    connect(pCaptureAnimationSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::setCaptureAnimationSpeed(static_cast<quint32>(value));
    });
    y += 40;

    pTextfield = spLabel::create(width - 140);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Dialog Speed: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spSlider pDialogAnimationSpeed = spSlider::create(Settings::getWidth() - 20 - width, 1, 100, "");
    pDialogAnimationSpeed->setTooltipText(tr("Selects the speed at which dialog animations are played."));
    pDialogAnimationSpeed->setPosition(width - 130, y);
    pDialogAnimationSpeed->setCurrentValue(static_cast<qint32>(Settings::getDialogAnimationSpeedValue()));
    pPanel->addItem(pDialogAnimationSpeed);
    connect(pDialogAnimationSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::setDialogAnimationSpeed(static_cast<quint32>(value));
    });
    y += 40;

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
