#include "wiki/wikiview.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "objects/base/label.h"

WikiView::WikiView(qint32 viewWidth, qint32 viewHeigth)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 y = 10;
    qint32 width = 150;
    spLabel pTextfield = new Label(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Search: "));
    pTextfield->setPosition(10, y);
    addChild(pTextfield);
    m_SearchString = new Textbox(viewWidth - 380);
    m_SearchString->setTooltipText(tr("Text that will be searched for in the title of each wikipage."));
    m_SearchString->setPosition(150, y);
    connect(m_SearchString.get(), &Textbox::sigTextChanged, this, &WikiView::searchChanged, Qt::QueuedConnection);
    addChild(m_SearchString);
    oxygine::spButton pButton = ObjectManager::createButton(tr("Search"));
    addChild(pButton);
    pButton->setPosition(m_SearchString->getWidth() + m_SearchString->getX() + 10, y);
    pButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigSearch(false);
    });
    connect(this, &WikiView::sigSearch, this, &WikiView::search, Qt::QueuedConnection);
    y += 50;

    pTextfield = new Label(width - 10);
    pTextfield->setStyle(style);
    pTextfield->setText(tr("Tags: "));
    pTextfield->setPosition(10, y);
    addChild(pTextfield);
    m_Tags = new DropDownmenu(300, WikiDatabase::getInstance()->getTags());
    m_Tags->setTooltipText(tr("Shows all pages grouped under a given tag. A page can be grouped in several groups at once."));
    m_Tags->setPosition(150, y);
    connect(m_Tags.get(), &DropDownmenu::sigItemChanged, this, &WikiView::tagChanged, Qt::QueuedConnection);
    addChild(m_Tags);
    connect(this, &WikiView::sigSearch, this, &WikiView::search, Qt::QueuedConnection);
    y += 50;

    QSize size(viewWidth - 20, viewHeigth - y - 50);
    m_MainPanel = new Panel(true, size, size);
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

void WikiView::showWikipage(WikiDatabase::pageData page)
{
   oxygine::getStage()->addChild(WikiDatabase::getInstance()->getPage(page));
}

void WikiView::tagChanged(qint32)
{
    m_SearchString->setCurrentText(m_Tags->getCurrentItemText());
    emit sigSearch(true);
}
