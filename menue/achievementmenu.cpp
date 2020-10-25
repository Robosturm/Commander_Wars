#include "menue/achievementmenu.h"
#include "menue/mainwindow.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/audiothread.h"
#include "coreengine/userdata.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "game/gamemap.h"
#include "wiki/wikidatabase.h"

#include "objects/label.h"
#include "objects/progressinfobar.h"

Achievementmenu::Achievementmenu()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Achievement Menue", Console::eDEBUG);

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
    pApp->getAudioThread()->loadFolder("resources/music/credits_options");
    pApp->getAudioThread()->playRandom();


    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    pButtonExit->attachTo(this);
    pButtonExit->setPosition(Settings::getWidth()  / 2.0f - pButtonExit->getWidth() / 2.0f,
                             Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &Achievementmenu::sigExitMenue, this, &Achievementmenu::exitMenue, Qt::QueuedConnection);

    QSize size(Settings::getWidth() - 20, Settings::getHeight() - 110);
    m_MainPanel = new Panel(true, size, size);
    m_MainPanel->setPosition(10, 50);
    addChild(m_MainPanel);

    oxygine::TextStyle styleLarge = FontManager::getMainFont48();
    styleLarge.color = FontManager::getFontColor();
    styleLarge.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    styleLarge.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    styleLarge.multiline = false;

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    Userdata* pUserdata = Userdata::getInstance();
    auto achievements = pUserdata->getAchievements();
    qint32 x = 10;
    qint32 y = 10;
    qint32 singleWidth = Settings::getWidth() - 80;
    qint32 achieveCount = 0;
    for (auto achievement : *achievements)
    {

        if (achievement.progress >= achievement.targetValue)
        {
            WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
            oxygine::spSprite pIcon = pWikiDatabase->getIcon(achievement.icon, GameMap::defaultImageSize * 2);
            pIcon->setPosition(x, y + 16);
            m_MainPanel->addItem(pIcon);
            achieveCount += 1;
        }
        else
        {
//            WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
//            oxygine::spSprite pIcon = pWikiDatabase->getIcon(achievement.icon, GameMap::defaultImageSize * 2);
//            pIcon->setPosition(x, y + 16);
//            m_MainPanel->addItem(pIcon);
            spLabel pTextfield = new Label(50);
            pTextfield->setStyle(styleLarge);
            pTextfield->setText("?");
            pTextfield->setPosition(x, y + 8);
            m_MainPanel->addItem(pTextfield);
        }

        spLabel pTextfield = new Label(singleWidth - 60);
        pTextfield->setStyle(style);
        if (achievement.hide)
        {
            pTextfield->setText("?");
        }
        else
        {
            pTextfield->setText(achievement.name);
        }
        pTextfield->setPosition(x + 60, y);
        m_MainPanel->addItem(pTextfield);

        pTextfield = new Label(singleWidth - 60);
        pTextfield->setStyle(style);
        if (achievement.hide)
        {
            pTextfield->setText("?");
        }
        else
        {
            pTextfield->setText(achievement.description);
        }
        pTextfield->setPosition(x + 60, y + 40);
        m_MainPanel->addItem(pTextfield);

        QString info = QString::number(achievement.progress) + " / " + QString::number(achievement.targetValue);
        spProgressInfoBar pProgressInfoBar = new ProgressInfoBar(singleWidth, 32, info, static_cast<float>(achievement.progress) / static_cast<float>(achievement.targetValue));
        pProgressInfoBar->setPosition(x, y + 80);
        m_MainPanel->addItem(pProgressInfoBar);
        y += 120;
    }
    m_MainPanel->setContentHeigth(y + 50);

    spLabel pTextfield = new Label(singleWidth);
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Achievement Progress: ") + QString::number(achieveCount) + " / " + QString::number(achievements->length()));
    pTextfield->setPosition(10, 10);
    addChild(pTextfield);
}

void Achievementmenu::exitMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    Console::print("Leaving Achievement Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new Mainwindow());
    oxygine::Actor::detach();
    pApp->continueThread();
}

