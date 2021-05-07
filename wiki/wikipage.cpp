#include "wiki/wikipage.h"
#include "wiki/wikidatabase.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/coperkmanager.h"

#include "game/player.h"

Wikipage::Wikipage()
    : QObject()
{
    setObjectName("Wikipage");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() / 2 - m_OkButton->getWidth() / 2, Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigFinished();
        detach();
    });

    // no the fun begins create checkboxes and stuff and a panel down here
    m_pPanel = spPanel::create(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 110),
                                     QSize(Settings::getWidth() - 60, Settings::getHeight() - 110));
    m_pPanel->setPosition(30, 30);
    pSpriteBox->addChild(m_pPanel);

    connect(pApp, &Mainapp::sigKeyDown, this, &Wikipage::keyInput, Qt::QueuedConnection);
    connect(this, &Wikipage::sigShowLink, this, &Wikipage::showLink, Qt::QueuedConnection);
}

void Wikipage::keyInput(oxygine::KeyEvent event)
{
    // for debugging
    Qt::Key cur = event.getKey();
    if (cur == Settings::getKey_information() ||
        cur == Settings::getKey_information2() ||
        cur == Settings::getKey_cancel() ||
        cur == Settings::getKey_cancel2())
    {
        emit sigFinished();
        detach();
    }
    
}

void Wikipage::loadText(QString text)
{
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
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
    oxygine::TextStyle style = FontManager::getMainFont48();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    oxygine::spTextField pLabel = oxygine::spTextField::create();
    pLabel->setStyle(style);
    pLabel->setHtmlText(text);
    pLabel->setPosition(m_pPanel->getContentWidth() / 2 - pLabel->getTextRect().getWidth() / 2, m_y);
    m_pPanel->addItem(pLabel);
    m_y += 80;
}

void Wikipage::showLink(QString pageID)
{
    WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
    auto entry = pWikiDatabase->getEntry(pageID);
    if (!std::get<0>(entry).isEmpty() &&
        !std::get<1>(entry).isEmpty())
    {
       oxygine::getStage()->addChild(pWikiDatabase->getPage(entry));
    }
    
}

void Wikipage::loadImage(QString file, float scale, QString pageID)
{
    WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
    oxygine::spSprite pSprite = pWikiDatabase->getIcon(file, 24);
    pSprite->setScale(scale);
    pSprite->setPosition(m_pPanel->getContentWidth() / 2 - pSprite->getScaledWidth() / 2.0f, m_y);
    pSprite->addClickListener([=](oxygine::Event*)
    {
        emit sigShowLink(pageID);
    });
    m_pPanel->addItem(pSprite);
    m_y += pSprite->getScaledHeight() + 10;
}
