#include "victorymenue.h"

#include "menue/mainwindow.h"

#include "game/gamemap.h"

#include "resource_management/gamemanager.h"
#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"


#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"

VictoryMenue::VictoryMenue()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Victory Menue", Console::eDEBUG);

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

    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pBoxSprite = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("graph");
    pBoxSprite->setResAnim(pAnim);
    qint32 widthCount = (pApp->getSettings()->getWidth() - 210) / static_cast<qint32>(pAnim->getWidth());
    qint32 heigthCount = (pApp->getSettings()->getHeight() - 60) / static_cast<qint32>(pAnim->getHeight());
    pBoxSprite->setSize(widthCount * pAnim->getWidth(), heigthCount * pAnim->getHeight());
    pBoxSprite->setVerticalMode(oxygine::Box9Sprite::TILING);
    pBoxSprite->setHorizontalMode(oxygine::Box9Sprite::TILING);
    pBoxSprite->setColor(255, 255, 255, 200);
    pBoxSprite->setGuides(0, pAnim->getWidth(), 0, pAnim->getHeight());
    pBoxSprite->setPosition(pApp->getSettings()->getWidth() - 10 - pBoxSprite->getWidth(), 20);
    addChild(pBoxSprite);


    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    pButtonExit->attachTo(this);
    pButtonExit->setPosition(pApp->getSettings()->getWidth() - pButtonExit->getWidth() - 10,
                             pApp->getSettings()->getHeight() - pButtonExit->getHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &VictoryMenue::sigExitMenue, this, &VictoryMenue::exitMenue, Qt::QueuedConnection);
}

void VictoryMenue::exitMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    Console::print("Leaving Victory Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new Mainwindow());
    GameMap::getInstance()->deleteMap();
    oxygine::Actor::detach();
    pApp->continueThread();
}
