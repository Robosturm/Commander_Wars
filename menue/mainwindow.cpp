#include "mainwindow.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "menue/gamemenue.h"
#include "menue/editormenue.h"
#include "menue/optionmenue.h"
#include "menue/mapselectionmapsmenue.h"
#include "menue/creditsmenue.h"
#include "multiplayer/lobbymenu.h"

#include "objects/filedialog.h"
#include "objects/dialogusername.h"

#include <QFile>

#include "game/co.h"

#include "SDL.h"

Mainwindow::Mainwindow()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    Console::print("Entering Main Menue", Console::eDEBUG);
    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("Background+1");
    sprite->setResAnim(pBackground);
    sprite->setPosition(0, 0);
    // background should be last to draw
    sprite->setPriority(static_cast<short>(Mainapp::ZOrder::Background));
    sprite->setScaleX(pApp->getSettings()->getWidth() / pBackground->getWidth());
    sprite->setScaleY(pApp->getSettings()->getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/hauptmenue");
    pApp->getAudioThread()->playRandom();

    qint32 btnI = 0;
    QString lastSaveGame = Settings::getLastSaveGame();
    if (QFile::exists(lastSaveGame) && lastSaveGame.endsWith(".sav"))
    {
        // create the ui for the main menue here :)
        oxygine::spButton pButtonLastSaveGame = ObjectManager::createButton(tr("Continue"));
        setButtonPosition(pButtonLastSaveGame, btnI);
        pButtonLastSaveGame->attachTo(this);
        pButtonLastSaveGame->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
        {
            emit this->sigLastSaveGame();
        });
        connect(this, &Mainwindow::sigLastSaveGame, this, &Mainwindow::lastSaveGame, Qt::QueuedConnection);
        btnI++;
    }

    // create the ui for the main menue here :)
    oxygine::spButton pButtonSingleplayer = ObjectManager::createButton(tr("Singleplayer"));
    setButtonPosition(pButtonSingleplayer, btnI);
    pButtonSingleplayer->attachTo(this);
    pButtonSingleplayer->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit this->sigEnterSingleplayer();
    });
    connect(this, &Mainwindow::sigEnterSingleplayer, this, &Mainwindow::enterSingleplayer, Qt::QueuedConnection);
    btnI++;

    // create the ui for the main menue here :)
    oxygine::spButton pButtonMultiplayer = ObjectManager::createButton(tr("Multiplayer"));
    setButtonPosition(pButtonMultiplayer, btnI);
    pButtonMultiplayer->attachTo(this);
    pButtonMultiplayer->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit this->sigEnterMultiplayer();
    });
    connect(this, &Mainwindow::sigEnterMultiplayer, this, &Mainwindow::enterMultiplayer, Qt::QueuedConnection);
    btnI++;

    // load button
    oxygine::spButton pButtonLoadGame = ObjectManager::createButton(tr("Load Game"));
    pButtonLoadGame->attachTo(this);
    setButtonPosition(pButtonLoadGame, btnI);
    pButtonLoadGame->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterLoadGame();
    });
    connect(this, &Mainwindow::sigEnterLoadGame, this, &Mainwindow::enterLoadGame, Qt::QueuedConnection);
    btnI++;

    // editor button
    oxygine::spButton pButtonEditor = ObjectManager::createButton(tr("Map Editor"));
    pButtonEditor->attachTo(this);
    setButtonPosition(pButtonEditor, btnI);
    pButtonEditor->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit this->sigEnterEditor();
    });
    connect(this, &Mainwindow::sigEnterEditor, this, &Mainwindow::enterEditor, Qt::QueuedConnection);
    btnI++;

    // option button
    oxygine::spButton pButtonOptions = ObjectManager::createButton(tr("Options"));
    pButtonOptions->attachTo(this);
    setButtonPosition(pButtonOptions, btnI);
    pButtonOptions->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterOptionmenue();
    });
    connect(this, &Mainwindow::sigEnterOptionmenue, this, &Mainwindow::enterOptionmenue, Qt::QueuedConnection);
    btnI++;

    // credits button
    oxygine::spButton pButtonCredtis = ObjectManager::createButton(tr("Credits"));
    pButtonCredtis->attachTo(this);
    setButtonPosition(pButtonCredtis, btnI);
    pButtonCredtis->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEnterCreditsmenue();
    });
    connect(this, &Mainwindow::sigEnterCreditsmenue, this, &Mainwindow::enterCreditsmenue, Qt::QueuedConnection);
    btnI++;

    // quit button
    oxygine::spButton pQuit = ObjectManager::createButton(tr("Quit"));
    pQuit->attachTo(this);
    setButtonPosition(pQuit, btnI);
    pQuit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit this->sigQuit();
    });
    connect(this, &Mainwindow::sigQuit, this, &Mainwindow::quitGame, Qt::QueuedConnection);
    btnI++;

    if (Settings::getUsername().isEmpty())
    {
        addChild(new DialogUsername());
    }
}

void Mainwindow::setButtonPosition(oxygine::spButton pButton, qint32 btnI)
{
    static const qint32 buttonCount = 8;
    float buttonHeigth = pButton->getHeight() + 30;
    Mainapp* pApp = Mainapp::getInstance();
    pButton->setPosition(pApp->getSettings()->getWidth() / 2.0f - pButton->getWidth() / 2.0f, pApp->getSettings()->getHeight() / 2.0f - buttonCount  / 2.0f * buttonHeigth + buttonHeigth * btnI);
}

Mainwindow::~Mainwindow()
{

}

void Mainwindow::enterSingleplayer()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    oxygine::getStage()->addChild(new MapSelectionMapsMenue());
    leaveMenue();
    pApp->continueThread();
}

void Mainwindow::enterMultiplayer()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    oxygine::getStage()->addChild(new LobbyMenu());
    leaveMenue();
    pApp->continueThread();
}

void Mainwindow::enterEditor()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    oxygine::getStage()->addChild(new EditorMenue());
    leaveMenue();
    pApp->continueThread();
}

void Mainwindow::enterOptionmenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    oxygine::getStage()->addChild(new OptionMenue());
    leaveMenue();
    pApp->continueThread();
}

void Mainwindow::enterCreditsmenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    oxygine::getStage()->addChild(new CreditsMenue());
    leaveMenue();
    pApp->continueThread();
}

void Mainwindow::enterLoadGame()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    // dummy impl for loading
    QVector<QString> wildcards;
    wildcards.append("*.sav");
    QString path = QCoreApplication::applicationDirPath() + "/savegames";
    spFileDialog saveDialog = new FileDialog(path, wildcards);
    this->addChild(saveDialog);
    connect(saveDialog.get(), &FileDialog::sigFileSelected, this, &Mainwindow::loadGame, Qt::QueuedConnection);
    pApp->continueThread();
}

void Mainwindow::lastSaveGame()
{
    loadGame(Settings::getLastSaveGame());
}

void Mainwindow::loadGame(QString filename)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (filename.endsWith(".sav"))
    {
        QFile file(filename);
        if (file.exists())
        {
            oxygine::getStage()->addChild(new GameMenue(filename));

            Mainapp* pApp = Mainapp::getInstance();
            pApp->getAudioThread()->clearPlayList();
            GameMap* pMap = GameMap::getInstance();
            pMap->getCurrentPlayer()->loadCOMusic();
            pMap->updateUnitIcons();
            pMap->getGameRules()->createFogVision();
            pApp->getAudioThread()->playRandom();
            GameMenue::getInstance()->updatePlayerinfo();
            leaveMenue();
        }
    }
    pApp->continueThread();
}

void Mainwindow::leaveMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    Console::print("Leaving Main Menue", Console::eDEBUG);
    oxygine::Actor::detach();
    pApp->continueThread();
}

void Mainwindow::quitGame()
{
     Mainapp* pApp = Mainapp::getInstance();
     pApp->quitGame();
}
