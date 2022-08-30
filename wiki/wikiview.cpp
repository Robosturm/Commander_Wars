#include "wiki/wikiview.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "objects/base/label.h"

WikiView::WikiView(qint32 viewWidth, qint32 viewHeigth)
{
#ifdef GRAPHICSUPPORT
    setObjectName("WikiView");
#endif
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 y = 10;
    qint32 width = 150;
    spLabel pTextfield = spLabel::create(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Search: "));
    pTextfield->setPosition(10, y);
    addChild(pTextfield);
    m_SearchString = spTextbox::create(viewWidth - 380);
    m_SearchString->setTooltipText(tr("Text that will be searched for in the title of each wikipage."));
    m_SearchString->setPosition(150, y);
    connect(m_SearchString.get(), &Textbox::sigTextChanged, this, &WikiView::searchChanged, Qt::QueuedConnection);
    addChild(m_SearchString);
    oxygine::spButton pButton = ObjectManager::createButton(tr("Search"));
    addChild(pButton);
    pButton->setPosition(m_SearchString->getWidth() + m_SearchString->getX() + 10, y);
    pButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
    {
        emit sigSearch(false);
    });
    connect(this, &WikiView::sigSearch, this, &WikiView::search, Qt::QueuedConnection);
    y += 50;

    pTextfield = spLabel::create(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setHtmlText(tr("Tags: "));
    pTextfield->setPosition(10, y);
    addChild(pTextfield);
    m_Tags = spDropDownmenu::create(300, WikiDatabase::getInstance()->getTags());
    m_Tags->setTooltipText(tr("Shows all pages grouped under a given tag. A page can be grouped in several groups at once."));
    m_Tags->setPosition(150, y);
    connect(m_Tags.get(), &DropDownmenu::sigItemChanged, this, &WikiView::tagChanged, Qt::QueuedConnection);
    addChild(m_Tags);
    connect(this, &WikiView::sigSearch, this, &WikiView::search, Qt::QueuedConnection);
    y += 50;

    QSize size(viewWidth - 20, viewHeigth - y - 50);
    m_MainPanel = spPanel::create(true, size, size);
    m_MainPanel->setPosition(10, y);
    addChild(m_MainPanel);
    y += 50;

    connect(this, &WikiView::sigShowWikipage, this, &WikiView::showWikipage, Qt::QueuedConnection);
    pApp->continueRendering();
}

void WikiView::searchChanged(QString)
{
    search(false);
}

void WikiView::search(bool onlyTag)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    CONSOLE_PRINT("WikiView::searchChanged ", Console::eDEBUG);
    m_MainPanel->clearContent();
    QVector<WikiDatabase::PageData> items = WikiDatabase::getInstance()->getEntries(m_SearchString->getCurrentText(), onlyTag);
    qint32 itemCount = 0;
    for (auto & wikiItem : items)
    {
        ObjectManager* pObjectManager = ObjectManager::getInstance();
        oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialogitems");
        oxygine::spBox9Sprite pBox = oxygine::spBox9Sprite::create();
        pBox->setResAnim(pAnim);
        pBox->setSize(m_MainPanel->getWidth() - 70, 40);
        oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
        style.color = FontManager::getFontColor();
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = false;
        spLabel textField = spLabel::create(pBox->getWidth() - 18);
        textField->setStyle(style);
        textField->setHeight(40);
        textField->setX(13);
        textField->setY(5);
        // loop through all entries :)
        QString item = wikiItem.m_name;
        textField->setHtmlText(item);
        pBox->addChild(textField);
        pBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
        auto* pPtrBox = pBox.get();
        // add some event handling :)
        pBox->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event*)
        {
            pPtrBox->addTween(oxygine::Sprite::TweenAddColor(QColor(32, 200, 32, 0)), oxygine::timeMS(300));
        });
        pBox->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event*)
        {
            pPtrBox->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
        });
        pBox->setPosition(0, itemCount * 40);
        pBox->addEventListener(oxygine::TouchEvent::CLICK, [this, wikiItem](oxygine::Event*)
        {
            emit sigShowWikipage(wikiItem);
        });
        m_MainPanel->addItem(pBox);
        itemCount++;
    }
    m_MainPanel->setContentHeigth(itemCount * 40 + 50);
    pApp->continueRendering();
}

void WikiView::showWikipage(WikiDatabase::PageData page)
{
   addChild(WikiDatabase::getInstance()->getPage(page));
}

void WikiView::showPage(QString id)
{
    WikiDatabase::PageData page;
    page.m_id = id;
    addChild(WikiDatabase::getInstance()->getPage(page));
}

void WikiView::tagChanged(qint32)
{
    m_SearchString->setCurrentText(m_Tags->getCurrentItemText());
    emit sigSearch(true);
}
