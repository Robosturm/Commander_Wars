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

#include "objects/filedialog.h"

#include <QFile>

#include "game/co.h"

#include "SDL.h"

Mainwindow::Mainwindow()
{
    Console::print("Entering Main Menue", Console::eDEBUG);
    Mainapp* pApp = Mainapp::getInstance();
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
    // create the ui for the main menue here :)
    oxygine::spButton pButtonSingleplayer = ObjectManager::createButton(tr("Singleplayer"));
    setButtonPosition(pButtonSingleplayer, btnI);
    pButtonSingleplayer->attachTo(this);
    pButtonSingleplayer->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit this->sigEnterSingleplayer();
    });
    connect(this, SIGNAL(sigEnterSingleplayer()), this, SLOT(enterSingleplayer()), Qt::QueuedConnection);
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
    connect(this, SIGNAL(sigEnterEditor()), this, SLOT(enterEditor()), Qt::QueuedConnection);
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

    // quit button
    oxygine::spButton pQuit = ObjectManager::createButton(tr("Quit"));
    pQuit->attachTo(this);
    setButtonPosition(pQuit, btnI);
    pQuit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit this->sigQuit();
    });
    connect(this, SIGNAL(sigQuit()), this, SLOT(quitGame()), Qt::QueuedConnection);
    btnI++;
}

void Mainwindow::setButtonPosition(oxygine::spButton pButton, qint32 btnI)
{
    static const qint32 buttonCount = 6;
    float buttonHeigth = pButton->getHeight() + 30;
    Mainapp* pApp = Mainapp::getInstance();
    pButton->setPosition(pApp->getSettings()->getWidth() / 2.0f - pButton->getWidth() / 2.0f, pApp->getSettings()->getHeight() / 2.0f - buttonCount  / 2.0f * buttonHeigth + buttonHeigth * btnI);
}

Mainwindow::~Mainwindow()
{

}

void Mainwindow::enterSingleplayer()
{
    oxygine::getStage()->addChild(new MapSelectionMapsMenue());
    leaveMenue();
}

void Mainwindow::enterEditor()
{
    oxygine::getStage()->addChild(new EditorMenue());
    leaveMenue();
}

void Mainwindow::enterOptionmenue()
{
    oxygine::getStage()->addChild(new OptionMenue());
    leaveMenue();
}

void Mainwindow::enterLoadGame()
{
    // dummy impl for loading
    QVector<QString> wildcards;
    wildcards.append("*.sav");
    QString path = QCoreApplication::applicationDirPath() + "/savegames";
    spFileDialog saveDialog = new FileDialog(path, wildcards);
    this->addChild(saveDialog);
    connect(saveDialog.get(), &FileDialog::sigFileSelected, this, &Mainwindow::loadGame, Qt::QueuedConnection);
}

void Mainwindow::loadGame(QString filename)
{
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

            leaveMenue();
        }
    }
}

void Mainwindow::leaveMenue()
{
    Console::print("Leaving Main Menue", Console::eDEBUG);
    oxygine::Actor::detach();
}

void Mainwindow::quitGame()
{
     Mainapp* pApp = Mainapp::getInstance();
     pApp->quitGame();
}
