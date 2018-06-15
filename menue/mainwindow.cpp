#include "mainwindow.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "menue/ingamemenue.h"

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
    sprite->setPriority(-32000);
    sprite->setScaleX(pApp->getSettings()->getWidth() / pBackground->getWidth());
    sprite->setScaleY(pApp->getSettings()->getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->loadFolder("resources/music/hauptmenue");
    pApp->getAudioThread()->playRandom();

    // create the ui for the main menue here :)
    oxygine::spButton pButton = ObjectManager::createButton(tr("Singleplayer"));
    pButton->attachTo(this);
    pButton->setPosition(pApp->getSettings()->getWidth() / 2.0f - pButton->getWidth() / 2.0f, pBackground->getHeight() / 2.0f);
    pButton->addEventListener(oxygine::TouchEvent::CLICK, CLOSURE(this, &Mainwindow::enterSingleplayer));
}

void Mainwindow::enterSingleplayer(oxygine::Event *)
{

    leaveMenue();
    oxygine::getStage()->addChild(new InGameMenue());
}

void Mainwindow::leaveMenue()
{
    Console::print("Leaving Main Menue", Console::eDEBUG);
    oxygine::getStage()->removeChild(this);
}
