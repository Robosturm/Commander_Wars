#include "humanplayerinputmenu.h"

#include "game/gamemap.h"

#include "coreengine/mainapp.h"

#include "resource_management/gamemanager.h"

#include "resource_management/fontmanager.h"

HumanPlayerInputMenu::HumanPlayerInputMenu(QStringList texts, QStringList actionIDs, QVector<oxygine::spActor> icons,
                                           QVector<qint32> costList, QVector<bool> enabledList)
{
    qint32 width = 0;
    oxygine::TextStyle style = FontManager::getMainFont();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_DEFAULT;
    style.fontSize = 20;
    oxygine::spTextField testText = new oxygine::TextField();
    for (qint32 i = 0; i < texts.size(); i++)
    {

        testText->setStyle(style);
        testText->setText(texts[i].toStdString().c_str());
        qint32 newWidth = testText->getTextRect().getWidth();
        if (newWidth > width )
        {
            width = newWidth;
        }
    }
    width += GameMap::Imagesize + GameMap::Imagesize * 3 / 4 ;

    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pTopBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("menu+top");
    pTopBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pTopBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pTopBox->setResAnim(pAnim);
    pTopBox->setSize(pAnim->getSize());
    pTopBox->setWidth(width);
    this->addChild(pTopBox);

    qint32 y = static_cast<qint32>(pAnim->getHeight());
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
        bool enabled = true;
        if (enabledList.size() > 0)
        {
            enabled = enabledList[i];
        }
        qint32 costs = 0;
        if (costList.size() > 0)
        {
            costs = costList[i];
        }

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

        // set color font based
        if (!enabled)
        {
            style.color = oxygine::Color(255, 0, 0, 255);
        }
        else
        {
            style.color = oxygine::Color(255, 255, 255, 255);
        }
        textField->setStyle(style);
        textField->setText(texts[i].toStdString().c_str());
        textField->attachTo(pItemBox);
        textField->setPosition(3 + GameMap::Imagesize, 0);
        textField->setSize(width - textField->getX(), GameMap::Imagesize - 4);

        this->addChild(pItemBox);
        pItemBox->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event *)->void
        {
            m_Cursor->setY(y + GameMap::Imagesize / 2 - m_Cursor->getScaledHeight() / 2);
        });
        QString action = actionIDs[i];
        if (enabled)
        {
            pItemBox->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent)->void
            {
                oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
                if (pTouchEvent != nullptr)
                {
                    if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
                    {
                        pEvent->stopPropagation();
                        emit sigItemSelected(action, costs);
                    }
                }
            });
        }
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
    this->setWidth(width);
}

void HumanPlayerInputMenu::setMenuPosition(qint32 x, qint32 y)
{
    GameMap* pMap = GameMap::getInstance();

    if (x + getWidth() + GameMap::Imagesize / 2 > pMap->getMapWidth() * GameMap::Imagesize)
    {
        x = pMap->getMapWidth() * GameMap::Imagesize - getWidth() - GameMap::Imagesize / 2;
    }
    if (y + getHeight() + GameMap::Imagesize / 2 > pMap->getMapHeight() * GameMap::Imagesize)
    {
        y = pMap->getMapHeight() * GameMap::Imagesize - getHeight() - GameMap::Imagesize / 2;
    }
    this->setPosition(x, y);
}
