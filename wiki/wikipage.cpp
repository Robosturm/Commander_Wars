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
    pSpriteBox->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(pApp->getSettings()->getWidth() / 2 - m_OkButton->getWidth() / 2, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigFinished();
        detach();
    });

    // no the fun begins create checkboxes and stuff and a panel down here
    m_pPanel = new Panel(true, QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110),
                                     QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110));
    m_pPanel->setPosition(30, 30);
    pSpriteBox->addChild(m_pPanel);

    connect(pApp, &Mainapp::sigKeyDown, this, &Wikipage::keyInput, Qt::QueuedConnection);
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
    style.color = FontManager::defaultColor;
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    oxygine::spTextField pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setHtmlText(text);
    pLabel->setScale(1.0f);
    pLabel->setWidth(m_pPanel->getContentWidth() - 80);
    pLabel->setPosition(10, y);
    m_pPanel->addItem(pLabel);
    y += pLabel->getTextRect().getHeight() + 10;
}

void Wikipage::loadImage(QString file, float scale)
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
            m_pPanel->addItem(pSprite);
            y += pSprite->getScaledHeight() + 10;
        }
    }
}

void Wikipage::loadHeadline(QString text)
{
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::defaultColor;
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    oxygine::spTextField pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setHtmlText(text);
    pLabel->setScale(2.0f);
    pLabel->setPosition(m_pPanel->getContentWidth() / 2 - pLabel->getTextRect().getWidth(), y);
    m_pPanel->addItem(pLabel);
    y += 80;
}
