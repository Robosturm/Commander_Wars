#include "qfile.h"

#include "costylemenu.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/audiothread.h"

#include "menue/mainwindow.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/cospritemanager.h"

#include "objects/coselection.h"

#include "objects/dialogcostyle.h"

COStyleMenu::COStyleMenu()
    : QObject()
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
    sprite->setPosition(-1, -1);
    // background should be last to draw
    sprite->setPriority(static_cast<short>(Mainapp::ZOrder::Background));
    sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
    sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/hauptmenue");
    pApp->getAudioThread()->playRandom();

    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    pButtonExit->attachTo(this);
    pButtonExit->setPosition(20, Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &COStyleMenu::sigExitMenue, this, &COStyleMenu::exitMenue, Qt::QueuedConnection);

    oxygine::spButton pButtonEdit = ObjectManager::createButton(tr("Edit CO"));
    pButtonEdit->attachTo(this);
    pButtonEdit->setPosition(Settings::getWidth() - pButtonEdit->getWidth() - 20, Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonEdit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigEditCOStyle();
    });
    connect(this, &COStyleMenu::sigEditCOStyle, this, &COStyleMenu::editCOStyle, Qt::QueuedConnection);

    spCOSelection pCOSelection = new COSelection(QSize(Settings::getWidth(), Settings::getHeight() - 100));
    pCOSelection->colorChanged(QColor(248, 88, 0));
    addChild(pCOSelection);
    connect(pCOSelection.get(), &COSelection::coSelected, this, &COStyleMenu::selectedCOIDChanged, Qt::QueuedConnection);
}

void COStyleMenu::exitMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    Console::print("Leaving CO Style Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new Mainwindow());
    addRef();
    oxygine::Actor::detach();
    deleteLater();
    pApp->continueThread();
}

void COStyleMenu::reloadMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    Console::print("Leaving CO Style Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new COStyleMenu());
    addRef();
    oxygine::Actor::detach();
    deleteLater();
    pApp->continueThread();
}

void COStyleMenu::selectedCOIDChanged(QString coid)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_currentCOID = coid;
    pApp->continueThread();
}

void COStyleMenu::editCOStyle()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (!m_currentCOID.isEmpty())
    {
        spDialogCOStyle pDialogCOStyle = new DialogCOStyle(m_currentCOID);
        addChild(pDialogCOStyle);
        connect(pDialogCOStyle.get(), &DialogCOStyle::sigFinished, this, &COStyleMenu::reloadMenue, Qt::QueuedConnection);
    }
    pApp->continueThread();
}
