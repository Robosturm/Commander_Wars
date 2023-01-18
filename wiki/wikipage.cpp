#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "wiki/wikipage.h"
#include "wiki/wikidatabase.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/gamemanager.h"

#include "game/player.h"

QStringList Wikipage::m_pageStack;

Wikipage::Wikipage(QString pageID)
    : m_pageID(pageID)
{
#ifdef GRAPHICSUPPORT
    setObjectName("Wikipage");
#endif
    Interpreter::setCppOwnerShip(this);

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() / 2 - m_OkButton->getScaledWidth() / 2, Settings::getHeight() - 30 - m_OkButton->getScaledHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigFinished();
    });

    // no the fun begins create checkboxes and stuff and a panel down here
    m_pPanel = spPanel::create(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 110),
                               QSize(Settings::getWidth() - 60, Settings::getHeight() - 110));
    m_pPanel->setPosition(30, 30);
    pSpriteBox->addChild(m_pPanel);

    Mainapp* pApp = Mainapp::getInstance();
    connect(pApp, &Mainapp::sigKeyDown, this, &Wikipage::keyInput, Qt::QueuedConnection);
    connect(this, &Wikipage::sigShowLink, this, &Wikipage::showLink, Qt::QueuedConnection);
    connect(this, &Wikipage::sigFinished, this, &Wikipage::remove, Qt::QueuedConnection);

    m_pageStack.append(pageID);
}

Wikipage::~Wikipage()
{
    m_pageStack.removeAll(m_pageID);
}

void Wikipage::keyInput(oxygine::KeyEvent event)
{
    if (!event.getContinousPress())
    {
        // for debugging
        Qt::Key cur = event.getKey();
        if (cur == Settings::getKey_information() ||
            cur == Settings::getKey_information2() ||
            cur == Settings::getKey_cancel() ||
            cur == Settings::getKey_cancel2())
        {
            emit sigFinished();
        }
    }
}

void Wikipage::remove()
{
    detach();
}

void Wikipage::loadText(QString text)
{
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    oxygine::spTextField pLabel = oxygine::spTextField::create();
    pLabel->setStyle(style);
    pLabel->setHtmlText(text);
    pLabel->setWidth(m_pPanel->getContentWidth() - 80);
    pLabel->setPosition(10, m_y);
    m_pPanel->addItem(pLabel);
    m_y += pLabel->getTextRect().getHeight() + 10;
}

void Wikipage::loadHeadline(QString text)
{
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont48());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    oxygine::spTextField pLabel = oxygine::spTextField::create();
    pLabel->setStyle(style);
    pLabel->setHtmlText(text);
    pLabel->setPosition(m_pPanel->getContentWidth() / 2 - pLabel->getTextRect().getWidth() / 2, m_y);
    m_pPanel->addItem(pLabel);
    m_y += pLabel->getTextRect().getHeight() + 10;
}

QStringList & Wikipage::getPageStack()
{
    return m_pageStack;
}

void Wikipage::showLink(QString pageID)
{
    if (!m_pageStack.contains(pageID))
    {
        WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
        auto entry = pWikiDatabase->getEntry(pageID);
        if (!entry.m_name.isEmpty() &&
            !entry.m_id.isEmpty())
        {
            oxygine::Stage::getStage()->addChild(pWikiDatabase->getPage(entry));
        }
    }
}

void Wikipage::loadImage(QString file, float scale, QString pageID)
{
    WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
    oxygine::spSprite pSprite = pWikiDatabase->getIcon(nullptr, file, 24);
    pSprite->setScale(scale * pSprite->getScaleX());
    pSprite->setPosition(m_pPanel->getContentWidth() / 2 - pSprite->getScaledWidth() / 2.0f, m_y);
    pSprite->addClickListener([this, pageID](oxygine::Event*)
    {
        emit sigShowLink(pageID);
    });
    m_pPanel->addItem(pSprite);
    m_y += pSprite->getScaledHeight() + 10;
}
