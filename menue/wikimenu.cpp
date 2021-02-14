#include "menue/wikimenu.h"
#include "menue/mainwindow.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"
#include "coreengine/audiothread.h"

#include "objects/base/label.h"

Wikimenu::Wikimenu()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Wiki Menue", Console::eDEBUG);
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("wikimenu");
    sprite->setResAnim(pBackground);
    // background should be last to draw
    sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
    sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
    sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());

    pApp->getAudioThread()->clearPlayList();
    pApp->getAudioThread()->loadFolder("resources/music/hauptmenue");
    pApp->getAudioThread()->playRandom();

    oxygine::spButton pButtonExit = ObjectManager::createButton(tr("Exit"));
    pButtonExit->attachTo(this);
    pButtonExit->setPosition(Settings::getWidth()  / 2.0f - pButtonExit->getWidth() / 2.0f,
                             Settings::getHeight() - pButtonExit->getHeight() - 10);
    pButtonExit->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigExitMenue();
    });
    connect(this, &Wikimenu::sigExitMenue, this, &Wikimenu::exitMenue, Qt::QueuedConnection);

    qint32 y = 10;
    qint32 width = 150;
    spLabel pTextfield = new Label(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Search: "));
    pTextfield->setPosition(10, y);
    addChild(pTextfield);
    m_SearchString = new Textbox(Settings::getWidth() - 380);
    m_SearchString->setTooltipText(tr("Text that will be searched for in the title of each wikipage."));
    m_SearchString->setPosition(150, y);
    connect(m_SearchString.get(), &Textbox::sigTextChanged, this, &Wikimenu::searchChanged, Qt::QueuedConnection);
    addChild(m_SearchString);
    oxygine::spButton pButton = ObjectManager::createButton(tr("Search"));
    addChild(pButton);
    pButton->setPosition(m_SearchString->getWidth() + m_SearchString->getX() + 10, y);
    pButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigSearch(false);
    });
    connect(this, &Wikimenu::sigSearch, this, &Wikimenu::search, Qt::QueuedConnection);
    y += 50;

    pTextfield = new Label(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Tags: "));
    pTextfield->setPosition(10, y);
    addChild(pTextfield);
    m_Tags = new DropDownmenu(300, WikiDatabase::getInstance()->getTags());
    m_Tags->setTooltipText(tr("Shows all pages grouped under a given tag. A page can be grouped in several groups at once."));
    m_Tags->setPosition(150, y);
    connect(m_Tags.get(), &DropDownmenu::sigItemChanged, this, &Wikimenu::tagChanged, Qt::QueuedConnection);
    addChild(m_Tags);
    connect(this, &Wikimenu::sigSearch, this, &Wikimenu::search, Qt::QueuedConnection);
    y += 50;

    QSize size(Settings::getWidth() - 20, Settings::getHeight() - y - 50);
    m_MainPanel = new Panel(true, size, size);
    m_MainPanel->setPosition(10, y);
    addChild(m_MainPanel);
    y += 50;

    connect(this, &Wikimenu::sigShowWikipage, this, &Wikimenu::showWikipage, Qt::QueuedConnection);
    pApp->continueRendering();
}


void Wikimenu::exitMenue()
{
    
    // save changed settings :)
    Console::print("Leaving Wiki Menue", Console::eDEBUG);
    oxygine::getStage()->addChild(new Mainwindow());
    oxygine::Actor::detach();
    
}

void Wikimenu::searchChanged(QString)
{
    search(false);
}

void Wikimenu::search(bool onlyTag)
{
    
    m_MainPanel->clearContent();
    QVector<WikiDatabase::pageData> items = WikiDatabase::getInstance()->getEntries(m_SearchString->getCurrentText(), onlyTag);
    qint32 itemCount = 0;
    for (qint32 i = 0; i < items.size(); i++)
    {
        ObjectManager* pObjectManager = ObjectManager::getInstance();
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialogitems");
        oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
        pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
        pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
        pBox->setResAnim(pAnim);
        oxygine::spTextField textField = new oxygine::TextField();
        oxygine::TextStyle style = FontManager::getMainFont24();
        style.color = FontManager::getFontColor();
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = false;
        textField->setStyle(style);

        textField->attachTo(pBox);
        pBox->setSize(m_MainPanel->getWidth() - 70, 40);
        textField->setHeight(40);
        textField->setWidth(pBox->getWidth() - 18);
        textField->setX(13);
        textField->setY(5);
        pBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
        this->m_MainPanel->addItem(pBox);
        // add some event handling :)
        pBox->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
        {
            pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(32, 200, 32, 0)), oxygine::timeMS(300));
        });
        pBox->addEventListener(oxygine::TouchEvent::OUTX, [ = ](oxygine::Event*)
        {
            pBox->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
        });
        pBox->setPosition(0, itemCount * 40);

        // loop through all entries :)
        QString item = std::get<0>(items[i]);
        textField->setHtmlText(item);
        pBox->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigShowWikipage(items[i]);
        });
        itemCount++;
    }
    m_MainPanel->setContentHeigth(itemCount * 40 + 50);
    
}

void Wikimenu::showWikipage(WikiDatabase::pageData page)
{
    addChild(WikiDatabase::getInstance()->getPage(page));
}

void Wikimenu::tagChanged(qint32)
{
    m_SearchString->setCurrentText(m_Tags->getCurrentItemText());
    emit sigSearch(true);
}
