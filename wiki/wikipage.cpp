#include "wikipage.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "game/player.h"

#include "resource_management/gamemanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "wiki/wikidatabase.h"

Wikipage::Wikipage()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

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
    m_pPanel = new Panel(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 110),
                                     QSize(Settings::getWidth() - 60, Settings::getHeight() - 110));
    m_pPanel->setPosition(30, 30);
    pSpriteBox->addChild(m_pPanel);

    connect(pApp, &Mainapp::sigKeyDown, this, &Wikipage::keyInput, Qt::QueuedConnection);
    connect(this, &Wikipage::sigShowLink, this, &Wikipage::showLink, Qt::QueuedConnection);
}

void Wikipage::keyInput(oxygine::KeyEvent event)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    // for debugging
    Qt::Key cur = event.getKey();
    if ((cur == Settings::getKey_information()) ||
        cur == Settings::getKey_cancel())
    {
        emit sigFinished();
        detach();
    }
    pApp->continueThread();
}

void Wikipage::loadText(QString text)
{
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    oxygine::spTextField pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setHtmlText(text);
    pLabel->setWidth(m_pPanel->getContentWidth() - 80);
    pLabel->setPosition(10, y);
    m_pPanel->addItem(pLabel);
    y += pLabel->getTextRect().getHeight() + 10;
}

void Wikipage::loadImage(QString file, float scale, QString pageID)
{
    oxygine::ResAnim* pAnim = WikiDatabase::getInstance()->getResAnim(file, oxygine::error_policy::ep_ignore_error);
    if (pAnim == nullptr)
    {
        pAnim = COSpriteManager::getInstance()->getResAnim(file, oxygine::error_policy::ep_ignore_error);
    }
    if (pAnim == nullptr)
    {
        pAnim = GameManager::getInstance()->getResAnim(file, oxygine::error_policy::ep_ignore_error);
    }
    if (pAnim != nullptr)
    {
         oxygine::spSprite pSprite = new oxygine::Sprite();
        pSprite->setResAnim(pAnim);
        pSprite->setScale(scale);
        pSprite->setPosition(m_pPanel->getContentWidth() / 2 - pSprite->getScaledWidth() / 2.0f, y);
        pSprite->addClickListener([=](oxygine::Event*)
        {
           emit sigShowLink(pageID);
        });
        m_pPanel->addItem(pSprite);
        y += pSprite->getScaledHeight() + 10;
    }
    else
    {
        UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
        if (pUnitSpriteManager->existsUnit(file))
        {
            spPlayer pPlayer = new Player();
            pPlayer->init();
            spUnit pSprite = new Unit(file, pPlayer.get(), false);
            pSprite->setOwner(nullptr);
            pSprite->setScale(scale);
            pSprite->setPosition(m_pPanel->getContentWidth() / 2 - pSprite->getScaledWidth() / 2.0f, y);
            pSprite->addClickListener([=](oxygine::Event*)
            {
               emit sigShowLink(pageID);
            });
            m_pPanel->addItem(pSprite);
            y += pSprite->getScaledHeight() + 10;
        }
    }
}

void Wikipage::loadHeadline(QString text)
{
    oxygine::TextStyle style = FontManager::getMainFont48();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    oxygine::spTextField pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setHtmlText(text);
    pLabel->setPosition(m_pPanel->getContentWidth() / 2 - pLabel->getTextRect().getWidth() / 2, y);
    m_pPanel->addItem(pLabel);
    y += 80;
}

void Wikipage::showLink(QString pageID)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
    auto entry = pWikiDatabase->getEntry(pageID);
    if (!std::get<0>(entry).isEmpty() &&
        !std::get<1>(entry).isEmpty())
    {
       oxygine::getStage()->addChild(pWikiDatabase->getPage(entry));
    }
    pApp->continueThread();
}
