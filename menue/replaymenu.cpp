#include <QMutexLocker>

#include "menue/replaymenu.h"
#include "menue/mainwindow.h"

#include "game/gameanimationfactory.h"

#include "objects/dialogs/dialogmessagebox.h"
#include "objects/base/dropdownmenu.h"
#include "objects/base/slider.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "ingamescriptsupport/genericbox.h"

ReplayMenu::ReplayMenu(QString filename)
    : GameMenue()
{
    connect(this, &ReplayMenu::sigExitReplay, this, &ReplayMenu::exitReplay, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigShowRecordInvalid, this, &ReplayMenu::showRecordInvalid, Qt::QueuedConnection);
    connect(this, &GameMenue::sigActionPerformed, this, &ReplayMenu::nextReplayAction, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigSwapPlay, this, &ReplayMenu::togglePlayUi, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigStartFastForward, this, &ReplayMenu::startFastForward, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigStopFastForward, this, &ReplayMenu::stopFastForward, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigShowConfig, this, &ReplayMenu::showConfig, Qt::QueuedConnection);
    changeBackground("replaymenu");
    bool valid = m_ReplayRecorder.loadRecord(filename);
    if (valid)
    {
        m_Viewplayer = new Viewplayer();
        // store animation modes
        m_storedAnimMode = Settings::getShowAnimations();
        _storedBatteAnimMode = Settings::getBattleAnimations();
        m_storedAnimationSpeed = Settings::getAnimationSpeedValue();
        m_storedBattleAnimationSpeed = Settings::getBattleAnimationSpeedValue();
        spGameMap pMap = GameMap::getInstance();
        pMap->registerMapAtInterpreter();
        oxygine::Actor::addChild(pMap);
        pMap->updateSprites();
        loadHandling();
        loadGameMenue();
        loadUIButtons();
        _HumanInput = new HumanPlayerInput();
        _HumanInput->init();
        gameStarted = true;
        Console::print("emitting sigActionPerformed()", Console::eDEBUG);
        emit sigActionPerformed();
    }
    else
    {
        emit sigShowRecordInvalid();
    }
}

ReplayMenu::~ReplayMenu()
{
    Settings::setShowAnimations(m_storedAnimMode);
    Settings::setBattleAnimations(_storedBatteAnimMode);
    Settings::setAnimationSpeed(m_storedAnimationSpeed);
    Settings::setBattleAnimationSpeed(m_storedBattleAnimationSpeed);
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
    spDialogMessageBox pExit = new DialogMessageBox(tr("The current active mods or the current record are invalid! Exiting the Replay now. Mods used in the Replay:") + "\n" +
                                                    modList, false);
    connect(pExit.get(), &DialogMessageBox::sigOk, this, &ReplayMenu::exitReplay, Qt::QueuedConnection);    
    addChild(pExit);
    
}

void ReplayMenu::exitReplay()
{
    
    gameStarted = false;
    while (GameAnimationFactory::getAnimationCount() > 0)
    {
        GameAnimationFactory::finishAllAnimations();
    }
    Console::print("Leaving Replay Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new Mainwindow());
    oxygine::Actor::detach();
    
}

void ReplayMenu::nextReplayAction()
{
    QMutexLocker locker(&m_replayMutex);
    if (m_pauseRequested)
    {
        m_pauseRequested = false;
        m_paused = true;
    }
    if (!m_paused)
    {
        spGameAction pAction = m_ReplayRecorder.nextAction();
        _HumanInput->cleanUpInput();
        float progress = 0.0f;
        if (m_ReplayRecorder.getRecordSize() > 0)
        {
            progress = static_cast<float>(m_ReplayRecorder.getProgess()) / static_cast<float>(m_ReplayRecorder.getRecordSize());
        }
        m_progressBar->setScrollvalue(progress);
        if (pAction.get() != nullptr)
        {
            Console::print("Performing next replay action", Console::eDEBUG);
            performAction(pAction);
        }
        else
        {
            Console::print("Pausing replay", Console::eDEBUG);
            swapPlay();
            togglePlayUi();
        }
    }
}

void ReplayMenu::showExitGame()
{
    
    m_Focused = false;
    spDialogMessageBox pExit = new DialogMessageBox(tr("Do you want to exit the current replay?"), true);
    connect(pExit.get(), &DialogMessageBox::sigOk, this, &ReplayMenu::exitReplay, Qt::QueuedConnection);
    connect(pExit.get(), &DialogMessageBox::sigCancel, [=]()
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
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pButtonBox = new oxygine::Box9Sprite();
    pButtonBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setResAnim(pAnim);
    pButtonBox->setSize(Settings::getWidth() - m_IngameInfoBar->getWidth(), 50);
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
    m_playButton = ObjectManager::createIconButton("play");
    m_playButton->setVisible(false);
    m_pauseButton = ObjectManager::createIconButton("pause");
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
    pButtonBox->addChild(m_playButton);
    pButtonBox->addChild(m_pauseButton);
    oxygine::spButton _fastForwardButton = ObjectManager::createIconButton("fastforward");
    _fastForwardButton->setPosition(m_playButton->getX() - 4 - _fastForwardButton->getWidth(), y);
    pButtonBox->addChild(_fastForwardButton);
    _fastForwardButton->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [=](oxygine::Event*)
    {
        emit sigStartFastForward();
    });
    _fastForwardButton->addEventListener(oxygine::TouchEvent::TOUCH_UP, [=](oxygine::Event*)
    {
        emit sigStopFastForward();
    });
    m_configButton = ObjectManager::createIconButton("settings");
    m_configButton->setPosition(_fastForwardButton->getX() - 4 - m_configButton->getWidth(), y);
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
    m_progressBar = new V_Scrollbar(m_configButton->getX() - 10, content);
    connect(m_progressBar.get(), &V_Scrollbar::sigScrollValueChanged, this, &ReplayMenu::seekChanged, Qt::QueuedConnection);
    connect(m_progressBar.get(), &V_Scrollbar::sigStartEditValue, this, &ReplayMenu::startSeeking, Qt::QueuedConnection);
    connect(m_progressBar.get(), &V_Scrollbar::sigEndEditValue, this, &ReplayMenu::seekRecord, Qt::QueuedConnection);
    m_progressBar->setContentWidth(content);
    m_progressBar->setPosition(8, y);
    m_progressBar->setScrollspeed((m_configButton->getX() - 10) / m_ReplayRecorder.getDayFromPosition(m_ReplayRecorder.getRecordSize() - 1));

    pButtonBox->addChild(m_progressBar);
    pAnim = pObjectManager->getResAnim("panel");
    pButtonBox = new oxygine::Box9Sprite();
    pButtonBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setResAnim(pAnim);
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    xyTextInfo = new Label(180);
    xyTextInfo->setStyle(style);
    xyTextInfo->setHtmlText("X: 0 Y: 0");
    xyTextInfo->setPosition(8, 8);
    pButtonBox->addChild(xyTextInfo);
    pButtonBox->setSize(200, 50);
    pButtonBox->setPosition((Settings::getWidth() - m_IngameInfoBar->getScaledWidth())  - pButtonBox->getWidth(), 0);
    pButtonBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    addChild(pButtonBox);
    
}

void ReplayMenu::loadSeekUi()
{
    
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pDayBox = new oxygine::Box9Sprite();
    pDayBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pDayBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pDayBox->setResAnim(pAnim);
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    _seekDayLabel = new Label(140);
    _seekDayLabel->setStyle(style);
    _seekDayLabel->setHtmlText(tr("Day: "));
    _seekDayLabel->setPosition(8, 8);
    pDayBox->addChild(_seekDayLabel);
    pDayBox->setSize(160, 50);
    pDayBox->setPosition(0, Settings::getHeight() - pDayBox->getHeight() + 6 - pDayBox->getHeight());
    pDayBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    addChild(pDayBox);
    _seekActor = pDayBox;
    _seekActor->setVisible(false);
    
}

void ReplayMenu::startSeeking()
{
    QMutexLocker locker(&m_replayMutex);
    if (!m_paused)
    {
        swapPlay();
    }
    
    _StoredShowAnimations = Settings::getShowAnimations();
    Settings::setShowAnimations(GameEnums::AnimationMode::AnimationMode_None);
    if (GameAnimationFactory::getAnimationCount() > 0)
    {
        GameAnimationFactory::finishAllAnimations();
    }
    Settings::setShowAnimations(_StoredShowAnimations);
    m_seeking = true;
    
}

void ReplayMenu::seekChanged(float value)
{
    
    _seekActor->setVisible(true);
    qint32 count = static_cast<qint32>(static_cast<float>(m_ReplayRecorder.getRecordSize()) * value);
    qint32 day = 0;
    if (m_ReplayRecorder.getRecordSize() > 0)
    {
        day = m_ReplayRecorder.getDayFromPosition(count);
    }
    _seekDayLabel->setHtmlText(tr("Day: ") + QString::number(day));
    
}

void ReplayMenu::seekRecord(float value)
{
    QMutexLocker locker(&m_replayMutex);
    
    qint32 count = static_cast<qint32>(static_cast<float>(m_ReplayRecorder.getRecordSize()) * value);
    qint32 day = m_ReplayRecorder.getDayFromPosition(count);
    seekToDay(day);
    _seekActor->setVisible(false);
    m_seeking = false;
    
}

void ReplayMenu::seekToDay(qint32 day)
{
    QMutexLocker locker(&m_replayMutex);
    if (m_ReplayRecorder.getRecordSize() > 0)
    {
        Console::print("Seeking to day " + QString::number(day), Console::eDEBUG);
        Mainapp::getInstance()->pauseRendering();
        spGameMap pMap = GameMap::getInstance();
        auto pos = pMap->getPosition();
        m_ReplayRecorder.seekToDay(day);
        pMap = GameMap::getInstance();
        pMap->registerMapAtInterpreter();
        addChild(pMap);
        pMap->setPosition(pos);
        pMap->updateSprites();
        pMap->getGameRules()->createFogVision();
        Mainapp::getInstance()->continueRendering();
        connectMap();
        connectMapCursor();
        swapPlay();
    }
}

void ReplayMenu::swapPlay()
{
    QMutexLocker locker(&m_replayMutex);
    if (m_paused)
    {
        Console::print("emitting sigActionPerformed()", Console::eDEBUG);
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
    Console::print("ReplayMenu::swapPlay()", Console::eDEBUG);
    if (m_playButton->getVisible())
    {
        m_playButton->setVisible(false);
        m_pauseButton->setVisible(true);
        m_pauseRequested = false;
        if (m_paused)
        {
            swapPlay();
        }
    }
    else
    {
        Console::print("requesting pause", Console::eDEBUG);
        m_playButton->setVisible(true);
        m_pauseButton->setVisible(false);
        m_pauseRequested = true;
    }
}

void ReplayMenu::startFastForward()
{
    QMutexLocker locker(&m_replayMutex);
    
    _StoredShowAnimations = Settings::getShowAnimations();
    Settings::setShowAnimations(GameEnums::AnimationMode::AnimationMode_None);
    if (GameAnimationFactory::getAnimationCount() > 0)
    {
        GameAnimationFactory::finishAllAnimations();
    }
    
}

void ReplayMenu::stopFastForward()
{
    QMutexLocker locker(&m_replayMutex);
    Settings::setShowAnimations(_StoredShowAnimations);
}

void ReplayMenu::showConfig()
{
    
    if (m_pauseButton->getVisible())
    {
        swapPlay();
    }

    spGenericBox pBox = new GenericBox();

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    spPanel pPanel = new Panel(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 110),
                         QSize(Settings::getWidth() - 60, Settings::getHeight() - 110));
    pPanel->setPosition(30, 30);
    pBox->addChild(pPanel);
    qint32 width = 450;
    qint32 y = 10;
    QVector<qint32> teams;
    QVector<QString> teamNames;
    spGameMap pMap = GameMap::getInstance();
    teamNames.append(tr("Current Team"));
    teamNames.append(tr("All Teams"));
    teamNames.append(tr("Map"));
    qint32 viewType = m_Viewplayer->getViewType();
    bool found = false;
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        qint32 team = pMap->getPlayer(i)->getTeam();
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
    spLabel pText = new Label(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Teamview:"));
    pText->setPosition(10, y);
    pPanel->addItem(pText);
    spDropDownmenu dropDown = new DropDownmenu(300, teamNames);
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

    spLabel pTextfield = new Label(800);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Gameplay Settings"));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    y += 40;

    pTextfield = new Label(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Ingame Animations: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    QVector<QString> items = {tr("None"), tr("All"), tr("Own"), tr("Ally"), tr("Enemy"),
                             tr("Only Detailed Battle All"), tr("Only Detailed Battle Own"),
                             tr("Only Detailed Battle Ally"), tr("Only Detailed Battle Enemy")};
    spDropDownmenu pAnimationMode = new DropDownmenu(450, items);
    pAnimationMode->setCurrentItem(static_cast<qint32>(Settings::getShowAnimations()));
    pAnimationMode->setPosition(width - 130, y);
    pAnimationMode->setTooltipText(tr("Select which ingame animations are played."));
    pPanel->addItem(pAnimationMode);
    connect(pAnimationMode.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        Settings::setShowAnimations(static_cast<GameEnums::AnimationMode>(value));
    });
    y += 40;

    pTextfield = new Label(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Battle Animations: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    items = {tr("Detailed"), tr("Overworld")};
    spDropDownmenu pBattleAnimationMode = new DropDownmenu(450, items);
    pBattleAnimationMode->setTooltipText(tr("Selects which battle animations are played when fighting an enemy."));
    pBattleAnimationMode->setCurrentItem(static_cast<qint32>(Settings::getBattleAnimations()));
    pBattleAnimationMode->setPosition(width - 130, y);
    pPanel->addItem(pBattleAnimationMode);
    connect(pBattleAnimationMode.get(), &DropDownmenu::sigItemChanged, [=](qint32 value)
    {
        Settings::setBattleAnimations(static_cast<GameEnums::BattleAnimationMode>(value));
    });
    y += 40;

    pTextfield = new Label(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Animation Speed: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spSlider pAnimationSpeed = new Slider(Settings::getWidth() - 20 - width, 1, 100, "");
    pAnimationSpeed->setTooltipText(tr("Selects the speed at which animations are played. Except battle animations."));
    pAnimationSpeed->setPosition(width - 130, y);
    pAnimationSpeed->setCurrentValue(static_cast<qint32>(Settings::getAnimationSpeedValue()));
    pPanel->addItem(pAnimationSpeed);
    connect(pAnimationSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::setAnimationSpeed(static_cast<quint32>(value));
    });
    y += 40;

    pTextfield = new Label(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Walk Speed: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spSlider pWalkSpeed = new Slider(Settings::getWidth() - 20 - width, 1, 100, "");
    pWalkSpeed->setTooltipText(tr("Selects the speed at which units walk across the map."));
    pWalkSpeed->setPosition(width - 130, y);
    pWalkSpeed->setCurrentValue(static_cast<qint32>(Settings::getWalkAnimationSpeedValue()));
    pPanel->addItem(pWalkSpeed);
    connect(pWalkSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::setWalkAnimationSpeed(static_cast<quint32>(value));
    });
    y += 40;

    pTextfield = new Label(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Battle Anim. Speed: "));
    pTextfield->setPosition(10, y);
    pPanel->addItem(pTextfield);
    spSlider pBattleAnimationSpeed = new Slider(Settings::getWidth() - 20 - width, 1, 100, "");
    pBattleAnimationSpeed->setTooltipText(tr("Selects the speed at which battle animations are played."));
    pBattleAnimationSpeed->setPosition(width - 130, y);
    pBattleAnimationSpeed->setCurrentValue(static_cast<qint32>(Settings::getBattleAnimationSpeedValue()));
    pPanel->addItem(pBattleAnimationSpeed);
    connect(pBattleAnimationSpeed.get(), &Slider::sliderValueChanged, [=](qint32 value)
    {
        Settings::setBattleAnimationSpeed(static_cast<quint32>(value));
    });
    y += 40;

    addChild(pBox);
    
}

void ReplayMenu::setViewTeam(qint32 item)
{
    
    spGameMap pMap = GameMap::getInstance();
    if (item <= -Viewplayer::ViewType::CurrentTeam)
    {
        m_Viewplayer->setViewType(item + Viewplayer::ViewType::CurrentTeam);
    }
    else
    {
        QVector<qint32> teams;
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            qint32 team = pMap->getPlayer(i)->getTeam();
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
    pMap->getGameRules()->createFogVision();
    
}
