#include <QMutexLocker>

#include "replaymenu.h"

#include "game/gameanimationfactory.h"

#include "menue/mainwindow.h"

#include "objects/dialogmessagebox.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

ReplayMenu::ReplayMenu(QString filename)
    : GameMenue()
{
    connect(this, &ReplayMenu::sigExitReplay, this, &ReplayMenu::exitReplay, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigShowRecordInvalid, this, &ReplayMenu::showRecordInvalid, Qt::QueuedConnection);
    connect(this, &GameMenue::sigActionPerformed, this, &ReplayMenu::nextReplayAction, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigSwapPlay, this, &ReplayMenu::swapPlay, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigStartFastForward, this, &ReplayMenu::startFastForward, Qt::QueuedConnection);
    connect(this, &ReplayMenu::sigStopFastForward, this, &ReplayMenu::stopFastForward, Qt::QueuedConnection);

    bool valid = m_ReplayRecorder.loadRecord(filename);
    if (valid)
    {
        GameMap* pMap = GameMap::getInstance();
        oxygine::Actor::addChild(pMap);
        pMap->updateSprites();
        loadHandling();
        loadGameMenue();
        loadUIButtons();
        emit sigActionPerformed();
    }
    else
    {
        emit sigShowRecordInvalid();
    }
}

ReplayMenu::~ReplayMenu()
{
}

void ReplayMenu::showRecordInvalid()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Focused = false;
    QString modList;
    QStringList mods = m_ReplayRecorder.getMods();
    for (auto& mod : mods)
    {
        modList += mod + "\n";
    }
    spDialogMessageBox pExit = new DialogMessageBox(tr("The current active mods or the current record are invalid! Exiting the Replay now. Mods in Replay:") + "\n" +
                                                    modList, true);
    connect(pExit.get(), &DialogMessageBox::sigOk, this, &ReplayMenu::exitReplay, Qt::QueuedConnection);
    connect(pExit.get(), &DialogMessageBox::sigCancel, [=]()
    {
        m_Focused = true;
    });
    addChild(pExit);
    pApp->continueThread();
}

void ReplayMenu::exitReplay()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    gameStarted = false;
    while (GameAnimationFactory::getAnimationCount() > 0)
    {
        GameAnimationFactory::finishAllAnimations();
    }
    Console::print("Leaving Replay Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new Mainwindow());
    addRef();
    oxygine::Actor::detach();
    deleteLater();
    pApp->continueThread();
}

void ReplayMenu::nextReplayAction()
{
    QMutexLocker locker(&_replayMutex);
    if (requestPause)
    {
        _paused = true;
        requestPause = false;
    }
    if (!_paused)
    {
        GameAction* pAction = m_ReplayRecorder.nextAction();

        float progress = 0.0f;
        if (m_ReplayRecorder.getRecordSize() > 0)
        {
            progress = static_cast<float>(m_ReplayRecorder.getProgess()) / static_cast<float>(m_ReplayRecorder.getRecordSize());
        }
        _progressBar->setScrollvalue(progress);
        if (pAction != nullptr)
        {
            performAction(pAction);
        }
        else
        {
            // todo replay ended
        }
    }
}

void ReplayMenu::showExitGame()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Focused = false;
    spDialogMessageBox pExit = new DialogMessageBox(tr("Do you want to exit the current replay?"), true);
    connect(pExit.get(), &DialogMessageBox::sigOk, this, &ReplayMenu::exitReplay, Qt::QueuedConnection);
    connect(pExit.get(), &DialogMessageBox::sigCancel, [=]()
    {
        m_Focused = true;
    });
    addChild(pExit);
    pApp->continueThread();
}

void ReplayMenu::loadUIButtons()
{
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
    pButtonBox->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));
    addChild(pButtonBox);

    oxygine::spButton exitGame = pObjectManager->createButton(tr("Exit"), 130);
    exitGame->setPosition(pButtonBox->getWidth() - 8 - exitGame->getWidth(), 4);
    exitGame->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowExitGame();
    });
    pButtonBox->addChild(exitGame);

    qint32 content = m_ReplayRecorder.getRecordSize() * actionPixelSize;
    if (content < exitGame->getX())
    {
        content = exitGame->getX() + 80;
    }

    qint32 y = 9;
    _playButton = ObjectManager::createIconButton("play");
    _playButton->setVisible(false);
    _pauseButton = ObjectManager::createIconButton("pause");
    _playButton->setPosition(exitGame->getX() - 4 - _playButton->getWidth(), y);
    _pauseButton->setPosition(exitGame->getX() - 4 - _pauseButton->getWidth(), y);
    _playButton->addClickListener([=](oxygine::Event * )
    {
        emit sigSwapPlay();
    });
    _pauseButton->addClickListener([=](oxygine::Event * )
    {
        emit sigSwapPlay();
    });
    pButtonBox->addChild(_playButton);
    pButtonBox->addChild(_pauseButton);
    oxygine::spButton _fastForwardButton = ObjectManager::createIconButton("fastforward");
    _fastForwardButton->setPosition(_playButton->getX() - 4 - _fastForwardButton->getWidth(), y);
    pButtonBox->addChild(_fastForwardButton);
    _fastForwardButton->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [=](oxygine::Event*)
    {
        emit sigStartFastForward();
    });
    _fastForwardButton->addEventListener(oxygine::TouchEvent::TOUCH_UP, [=](oxygine::Event*)
    {
        emit sigStopFastForward();
    });

    _progressBar = new V_Scrollbar(exitGame->getX() - 80, content);
    _progressBar->setPosition(8, y);
    _progressBar->setEnabled(false);
    pButtonBox->addChild(_progressBar);
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
    pButtonBox->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));
    addChild(pButtonBox);
}

void ReplayMenu::swapPlay()
{
    QMutexLocker locker(&_replayMutex);
    if (_playButton->getVisible())
    {
        _playButton->setVisible(false);
        _pauseButton->setVisible(true);
        requestPause = false;
        if (_paused)
        {
            _paused = false;
            emit sigActionPerformed();
        }
    }
    else
    {
        _playButton->setVisible(true);
        _pauseButton->setVisible(false);
        requestPause = true;
    }
}

void ReplayMenu::startFastForward()
{
    QMutexLocker locker(&_replayMutex);
    _StoredShowAnimations = Settings::getShowAnimations();
    Settings::setShowAnimations(GameEnums::AnimationMode::AnimationMode_None);
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (GameAnimationFactory::getAnimationCount() > 0)
    {
        GameAnimationFactory::finishAllAnimations();
    }
    pApp->continueThread();
}

void ReplayMenu::stopFastForward()
{
    QMutexLocker locker(&_replayMutex);
    Settings::setShowAnimations(_StoredShowAnimations);
}
