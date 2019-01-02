#include "humanplayerinputmenu.h"

#include "game/gamemap.h"

#include "coreengine/mainapp.h"

#include "resource_management/gamemanager.h"

#include "resource_management/fontmanager.h"

HumanPlayerInputMenu::HumanPlayerInputMenu(QStringList texts, QStringList actionIDs, QVector<oxygine::spSprite> icons)
{
    qint32 width = 100;
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pTopBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("menu+top");
    pTopBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pTopBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pTopBox->setResAnim(pAnim);
    pTopBox->setSize(pAnim->getSize());
    pTopBox->setWidth(width);
    this->addChild(pTopBox);

    qint32 y = pAnim->getHeight();
    m_Cursor = new oxygine::Sprite();
    pAnim = pGameManager->getResAnim("cursor+menu");
    if (pAnim->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
        m_Cursor->addTween(tween);
    }
    else
    {
        m_Cursor->setResAnim(pAnim);
    }
    m_Cursor->setPosition(width - m_Cursor->getScaledWidth() / 2, y + GameMap::Imagesize / 2 - m_Cursor->getScaledHeight() / 2);
    m_Cursor->setScale(GameMap::Imagesize / pAnim->getWidth());
    for (qint32 i = 0; i < actionIDs.size(); i++)
    {
        oxygine::spBox9Sprite pItemBox = new oxygine::Box9Sprite();
        pAnim = pGameManager->getResAnim("menu+middle");
        pItemBox->setResAnim(pAnim);
        pItemBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
        pItemBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
        pItemBox->setSize(pAnim->getSize());
        pItemBox->addChild(icons[i]);
        icons[i]->setPosition(3, 0);
        pItemBox->setHeight(GameMap::Imagesize);
        pItemBox->setY(y);
        pItemBox->setWidth(width);

        // text for the item
        oxygine::spTextField textField = new oxygine::TextField();
        oxygine::TextStyle style = FontManager::getMainFont();
        style.color = oxygine::Color(255, 255, 255, 255);
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_DEFAULT;
        style.fontSize = 20;
        textField->setStyle(style);
        textField->setText(texts[i].toStdString().c_str());
        textField->attachTo(pItemBox);
        textField->setPosition(3 + icons[i]->getScaledWidth(), 0);
        textField->setSize(width - textField->getX(), GameMap::Imagesize - 4);
        this->addChild(pItemBox);
        pItemBox->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event *)->void
        {
            m_Cursor->setY(y + GameMap::Imagesize / 2 - m_Cursor->getScaledHeight() / 2);
        });
        QString action = actionIDs[i];
        pItemBox->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent)->void
        {
            pEvent->stopPropagation();
            emit sigItemSelected(action);
        });
        y += pItemBox->getHeight();
    }
    oxygine::spBox9Sprite pBottomBox = new oxygine::Box9Sprite();
    pAnim = pGameManager->getResAnim("menu+bottom");
    pBottomBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBottomBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBottomBox->setResAnim(pAnim);
    pBottomBox->setY(y);
    pBottomBox->setSize(pAnim->getSize());
    pBottomBox->setWidth(width);
    this->addChild(pBottomBox);
    this->addChild(m_Cursor);
    this->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));
    this->setHeight(y + pBottomBox->getHeight());
}
