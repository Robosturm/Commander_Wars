#include "gameinput/humanplayerinputmenu.h"

#include "game/gamemap.h"
#include "game/unit.h"
#include "game/cursor.h"

#include "wiki/fieldinfo.h"
#include "wiki/wikidatabase.h"

#include "menue/gamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"
#include "coreengine/globalutils.h"

#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/unitspritemanager.h"

HumanPlayerInputMenu::HumanPlayerInputMenu(QStringList texts, QStringList actionIDs, QVector<oxygine::spActor> icons,
                                           QVector<qint32> costList, QVector<bool> enabledList)
    : m_ActionIDs(actionIDs),
      m_CostList(costList),
      m_EnabledList(enabledList)
{
    setObjectName("HumanPlayerInputMenu");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    connect(pApp, &Mainapp::sigKeyDown, this, &HumanPlayerInputMenu::keyInput, Qt::QueuedConnection);
    qint32 width = 0;
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_DEFAULT;
    style.fontSize = 20;
    oxygine::spTextField testText = oxygine::spTextField::create();
    for (qint32 i = 0; i < texts.size(); i++)
    {
        testText->setStyle(style);
        testText->setHtmlText(texts[i]);
        qint32 newWidth = testText->getTextRect().getWidth() * static_cast<float>(GameMap::getImageSize()) / static_cast<float>(GameMap::defaultImageSize);
        if (newWidth > width )
        {
            width = newWidth;
        }
    }
    width += GameMap::getImageSize() + GameMap::getImageSize() * 3 / 4 ;
    m_itemWidth = width;
    m_itemHeigth = GameMap::getImageSize();

    GameManager* pGameManager = GameManager::getInstance();
    qint32 heigth = createTopSprite(0, width);
    qint32 y = heigth;
    m_startY = y;
    m_Cursor = oxygine::spSprite::create();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("cursor+menu");
    if (pAnim->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
        m_Cursor->addTween(tween);
    }
    else
    {
        m_Cursor->setResAnim(pAnim);
    }
    m_Cursor->setPosition(width - m_Cursor->getScaledWidth() / 2, y + GameMap::getImageSize() / 2 - m_Cursor->getScaledHeight() / 2);
    m_Cursor->setScale(GameMap::getImageSize() / pAnim->getWidth());
    qint32 x = 0;
    qint32 xCount = 0;
    m_maxXCount = Settings::getMenuItemRowCount();
    if ((Settings::getWidth() - 120) / width < m_maxXCount)
    {
        m_maxXCount = (Settings::getWidth() - 120) / width;
    }
    m_rowCount = Settings::getMenuItemCount();
    if ((Settings::getHeight() - 60) / m_itemHeigth < m_rowCount)
    {
        m_rowCount = (Settings::getHeight() - 60) / m_itemHeigth;
    }
    qint32 maxY = 0;
    for (qint32 i = 0; i < actionIDs.size(); i++)
    {
        if (i > 0 && i % m_rowCount == 0 && xCount < m_maxXCount)
        {
            xCount++;
            createBottomSprite(x, y, width);
            if (xCount < m_maxXCount)
            {
                x += width;
                createTopSprite(x, width);
            }
            maxY = y;
            y = m_startY;
        }
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
        QString action = actionIDs[i];
        oxygine::spBox9Sprite pItemBox = createMenuItem(enabled, x, y, width, style, texts[i], action, costs, icons[i], i);
        if (xCount < m_maxXCount)
        {
            y += static_cast<qint32>(pItemBox->getHeight());
        }
        if (y > maxY)
        {
            maxY = y;
        }
    }

    if (xCount > 0 && actionIDs.size() < m_rowCount * m_maxXCount)
    {
        qint32 emptyItems = ((xCount + 1) * m_rowCount) - xCount * m_rowCount - actionIDs.size() % m_rowCount;
        if (emptyItems < m_rowCount)
        {
            for (qint32 i = 0; i < emptyItems; i++)
            {
                oxygine::spBox9Sprite pItemBox = oxygine::spBox9Sprite::create();
                pAnim = pGameManager->getResAnim("menu+middle");
                pItemBox->setResAnim(pAnim);
                pItemBox->setSize(pAnim->getSize());
                pItemBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
                pItemBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
                pItemBox->setHeight(GameMap::getImageSize());
                pItemBox->setY(y);
                pItemBox->setX(x);
                pItemBox->setWidth(width);
                pItemBox->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent)->void
                {
                    oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
                    pTouchEvent->stopPropagation();
                    if (pTouchEvent != nullptr)
                    {
                        if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
                        {
                            emit sigCanceled(0, 0);
                        }
                    }
                });
                addChild(pItemBox);
                m_ItemActors.append(pItemBox);
                y += static_cast<qint32>(pItemBox->getHeight());
                m_itemHeigth = static_cast<qint32>(pItemBox->getHeight());
            }
        }
    }
    qint32 bottomHeigth = createBottomSprite(x, maxY, width);
    addChild(m_Cursor);
    setPriority(static_cast<qint32>(Mainapp::ZOrder::FocusedObjects));

    qint32 scrollbarWidth = 0;
    if (m_ActionIDs.size() > m_rowCount * m_maxXCount)
    {
        qint32 height = maxY + bottomHeigth;
        qint32 max = m_ItemActors.size() - m_rowCount * m_maxXCount;
        m_scrollbar = spH_Scrollbar::create(height, height + max * m_itemHeigth * 1.5f);
        connect(m_scrollbar.get(), &H_Scrollbar::sigScrollValueChanged, this, [=](float value)
        {
            m_startItem = max * value;
            updateItemPositionAndVisibility();
        });
        scrollbarWidth = m_scrollbar->getWidth() + 25;
        m_scrollbar->setX(width * xCount + 25);
        addChild(m_scrollbar);
        addEventListener(oxygine::TouchEvent::WHEEL_DIR, [ = ](oxygine::Event* pEvent)
        {
            oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
            if (pTouchEvent != nullptr)
            {
                emit m_scrollbar->sigChangeScrollValue(-pTouchEvent->wheelDirection.y / m_scrollbar->getContentHeigth());
                pTouchEvent->stopPropagation();
            }
        });
    }
    setHeight(maxY + bottomHeigth);
    setWidth(width * xCount + scrollbarWidth);
    m_columnCount = xCount;
    updateItemPositionAndVisibility();
}

void HumanPlayerInputMenu::scroll(qint32 count)
{
    m_startItem += count;
    if (m_startItem + m_rowCount * m_maxXCount >= m_ItemActors.size())
    {
        m_startItem = m_ItemActors.size() - m_rowCount * m_maxXCount - 1;
    }
    if (m_startItem < 0)
    {
        m_startItem = 0;
    }
    updateItemPositionAndVisibility();
}

void HumanPlayerInputMenu::updateItemPositionAndVisibility()
{
    qint32 x = 0;
    qint32 y = m_startY;
    qint32 xCount = 0;
    for (qint32 i = 0; i < m_startItem; ++i)
    {
        m_ItemActors[i]->setVisible(false);
    }
    for (qint32 i = m_startItem; i < m_ItemActors.size(); ++i)
    {
        if ((i - m_startItem) > 0 && (i - m_startItem) % m_rowCount == 0 && xCount < m_maxXCount)
        {
            ++xCount;
            x += m_itemWidth;
            y = m_startY;
        }
        if (xCount < m_maxXCount)
        {
            m_ItemActors[i]->setPosition(x, y);
            m_ItemActors[i]->setVisible(true);
            y += static_cast<qint32>(m_ItemActors[i]->getHeight());
        }
        else
        {
            m_ItemActors[i]->setVisible(false);
        }
    }
}

oxygine::spBox9Sprite HumanPlayerInputMenu::createMenuItem(bool enabled, qint32& x, qint32& y, qint32 width, oxygine::TextStyle style,
                                                           QString text, QString action, qint32 costs, oxygine::spActor icon, qint32 item)
{
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pItemBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("menu+middle");
    pItemBox->setResAnim(pAnim);
    pItemBox->setSize(pAnim->getSize());
    pItemBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pItemBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pItemBox->addChild(icon);
    icon->setPosition(3, 0);
    pItemBox->setHeight(GameMap::getImageSize());
    pItemBox->setY(y);
    pItemBox->setX(x);
    pItemBox->setWidth(width);
    // text for the item
    oxygine::spTextField textField = oxygine::spTextField::create();
    // set color font based
    if (!enabled)
    {
        style.color = QColor(255, 0, 0, 255);
    }
    else
    {
        style.color = FontManager::getFontColor();
    }
    textField->setStyle(style);
    textField->setScale(static_cast<float>(GameMap::getImageSize()) / static_cast<float>(GameMap::defaultImageSize));
    textField->setHtmlText(text);
    textField->attachTo(pItemBox);
    textField->setPosition(3 + GameMap::getImageSize(), 0);
    textField->setSize(width - textField->getX(), GameMap::getImageSize() - 4);
    addChild(pItemBox);
    oxygine::Actor* pBox = pItemBox.get();
    pItemBox->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event *pEvent)->void
    {
        Mainapp::getInstance()->getAudioThread()->playSound("switchmenu.wav");
        pEvent->stopPropagation();
        m_Cursor->setY(pBox->getY() + GameMap::getImageSize() / 2 - m_Cursor->getScaledHeight() / 2);
        m_Cursor->setX(pBox->getX() + width);
        m_currentAction = item;
    });
    if (enabled)
    {
        pItemBox->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent)->void
        {
            oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
            pTouchEvent->stopPropagation();
            if (pTouchEvent != nullptr)
            {
                if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
                {
                    Mainapp::getInstance()->getAudioThread()->playSound("okay.wav");
                    emit sigItemSelected(action, costs);
                }
                else if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
                {                        emit sigCanceled(0, 0);
                }
            }

        });
    }
    else
    {
        pItemBox->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent)->void
        {
            oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
            pTouchEvent->stopPropagation();
            if (pTouchEvent != nullptr)
            {
                if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
                {
                    emit sigCanceled(0, 0);
                }
            }
        });
    }
    m_ItemActors.append(pItemBox);
    return pItemBox;
}

qint32 HumanPlayerInputMenu::createBottomSprite(qint32 x, qint32 y, qint32 width)
{
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pBottomBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("menu+bottom");
    pBottomBox->setResAnim(pAnim);
    pBottomBox->setSize(pAnim->getSize());
    pBottomBox->setWidth(width);
    pBottomBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBottomBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBottomBox->setY(y);
    pBottomBox->setX(x);
    addChild(pBottomBox);
    return static_cast<qint32>(pBottomBox->getHeight());
}

qint32 HumanPlayerInputMenu::createTopSprite(qint32 x, qint32 width)
{
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pTopBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("menu+top");
    pTopBox->setResAnim(pAnim);
    pTopBox->setSize(pAnim->getSize());
    pTopBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pTopBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pTopBox->setWidth(width);
    pTopBox->setX(x);
    addChild(pTopBox);
    return static_cast<qint32>(pTopBox->getHeight());
}

void HumanPlayerInputMenu::setMenuPosition(qint32 x, qint32 y)
{
    spGameMap pMap = GameMap::getInstance();

    if (x + getWidth() + GameMap::getImageSize() / 2 > pMap->getMapWidth() * GameMap::getImageSize())
    {
        x = pMap->getMapWidth() * GameMap::getImageSize() - getWidth() - GameMap::getImageSize() / 2;
        if (x < 0)
        {
            x = 0;
        }
    }
    if (y + getHeight() + GameMap::getImageSize() / 2 > pMap->getMapHeight() * GameMap::getImageSize())
    {
        y = pMap->getMapHeight() * GameMap::getImageSize() - getHeight() - GameMap::getImageSize() / 2;
        if (y < 0)
        {
            y = 0;
        }
    }
    setPosition(x, y);
    moveMouseToItem(0, 0);
}

void HumanPlayerInputMenu::keyInput(oxygine::KeyEvent event)
{
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr)
    {
        Chat* pChat = pMenu->getChat();
        if (m_Focused && (pChat == nullptr || pChat->getVisible() == false))
        {
            // for debugging
            Qt::Key cur = event.getKey();
            float max = m_ItemActors.size() - m_rowCount * m_maxXCount;
            float value = 1.0f / max;
            qint32 endItemCount = m_startItem + m_rowCount * m_columnCount;
            if (cur == Settings::getKey_up() ||
                cur == Settings::getKey_up2())
            {
                Mainapp::getInstance()->getAudioThread()->playSound("switchmenu.wav");
                if (m_currentAction > m_startItem)
                {
                    m_currentAction--;
                }
                else if (m_startItem > 0)
                {
                    if (m_scrollbar.get() != nullptr)
                    {
                        m_scrollbar->changeScrollValue(-value);
                    }
                    m_currentAction--;
                }
                else
                {
                    if (m_scrollbar.get() != nullptr)
                    {
                        m_scrollbar->changeScrollValue(1.0f);
                    }
                    m_currentAction = m_ActionIDs.size() - 1;
                }
            }
            else if (cur == Settings::getKey_down() ||
                     cur == Settings::getKey_down2())
            {
                Mainapp::getInstance()->getAudioThread()->playSound("switchmenu.wav");
                if (m_currentAction < endItemCount - 1)
                {
                    m_currentAction++;
                }
                else if (m_currentAction < m_ActionIDs.size() - 1)
                {
                    if (m_scrollbar.get() != nullptr)
                    {
                        m_scrollbar->changeScrollValue(value);
                    }
                    m_currentAction++;
                }
                else
                {
                    if (m_scrollbar.get() != nullptr)
                    {
                        m_scrollbar->changeScrollValue(-1.0f);
                    }
                    m_currentAction = 0;
                }
            }
            if (cur == Settings::getKey_left() ||
                cur == Settings::getKey_left2())
            {
                Mainapp::getInstance()->getAudioThread()->playSound("switchmenu.wav");
                if (m_currentAction - m_rowCount >= m_startItem)
                {
                    m_currentAction -= m_rowCount;
                }
                else
                {
                    m_currentAction = endItemCount - 1;
                }
            }
            else if (cur == Settings::getKey_right() ||
                     cur == Settings::getKey_right2())
            {
                Mainapp::getInstance()->getAudioThread()->playSound("switchmenu.wav");
                if (m_currentAction + m_rowCount < endItemCount)
                {
                    m_currentAction += m_rowCount;
                }
                else
                {
                    m_currentAction = m_startItem;
                }
            }
            else if (cur == Settings::getKey_confirm() ||
                     cur == Settings::getKey_confirm2())
            {
                if (m_ActionIDs.size() > 0)
                {
                    if ((m_EnabledList.size() > 0 && m_EnabledList[m_currentAction]) ||
                        (m_EnabledList.size() == 0))
                    {
                        Mainapp::getInstance()->getAudioThread()->playSound("okay.wav");
                        if (m_CostList.size() == m_ActionIDs.size())
                        {
                            emit sigItemSelected(m_ActionIDs[m_currentAction], m_CostList[m_currentAction]);
                        }
                        else
                        {
                            emit sigItemSelected(m_ActionIDs[m_currentAction], 0);
                        }
                    }
                }
            }
            else if (cur == Settings::getKey_cancel() ||
                     cur == Settings::getKey_cancel2())
            {
                emit sigCanceled(0, 0);
            }
            else if (cur == Settings::getKey_information() ||
                     cur == Settings::getKey_information2())
            {
                QString id = m_ActionIDs[m_currentAction];
                UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
                if (pUnitSpriteManager->exists(id))
                {
                    spUnit pDummy = spUnit::create(id, GameMap::getInstance()->getCurrentPlayer(), false);
                    spFieldInfo fieldinfo = spFieldInfo::create(nullptr, pDummy.get());
                    pMenu->addChild(fieldinfo);
                    connect(fieldinfo.get(), &FieldInfo::sigFinished, [=]
                    {
                        m_Focused = true;
                    });
                    m_Focused = false;
                }
                else
                {
                    WikiDatabase* pDatabase = WikiDatabase::getInstance();
                    WikiDatabase::pageData data = pDatabase->getEntry(id);
                    if (std::get<1>(data) != "")
                    {
                        spWikipage page = pDatabase->getPage(data);
                        pMenu->addChild(page);
                        connect(page.get(), &FieldInfo::sigFinished, [=]
                        {
                            m_Focused = true;
                        });
                        m_Focused = false;
                    }
                }
            }
            qint32 x = m_ItemActors[m_currentAction]->getX();
            qint32 y = m_ItemActors[m_currentAction]->getY();
            m_Cursor->setY(m_startY + y + GameMap::getImageSize() / 2 - m_Cursor->getScaledHeight() / 2);
            m_Cursor->setX(x + m_itemWidth);
        }
    }
}

void HumanPlayerInputMenu::moveMouseToItem(qint32 x, qint32 y)
{
    oxygine::Vector2 pos = local2stage();
    Mainapp* pApp = Mainapp::getInstance();
    QPoint curPos = pApp->mapToGlobal(QPoint(pos.x + m_itemWidth / 2 + m_itemWidth * x, pos.y + m_startY + m_itemHeigth / 2 + m_itemHeigth * y));
    pApp->cursor().setPos(curPos);
}
