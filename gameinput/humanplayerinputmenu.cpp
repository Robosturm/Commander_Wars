#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "gameinput/humanplayerinputmenu.h"

#include "game/gamemap.h"
#include "game/unit.h"

#include "wiki/fieldinfo.h"
#include "wiki/wikidatabase.h"

#include "menue/gamemenue.h"

#include "coreengine/interpreter.h"
#include "coreengine/audiomanager.h"

#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/unitspritemanager.h"

HumanPlayerInputMenu::HumanPlayerInputMenu(GameMenue* pMenu, GameMap* pMap, const QStringList & texts, const QStringList &  actionIDs, const QVector<oxygine::spActor> & icons,
                                           const QVector<qint32> & costList, const QVector<bool> & enabledList)
    : m_ActionIDs(actionIDs),
      m_CostList(costList),
      m_EnabledList(enabledList),
      m_pMap(pMap),
      m_pMenu(pMenu)
{
#ifdef GRAPHICSUPPORT
    setObjectName("HumanPlayerInputMenu");
#endif
    Interpreter::setCppOwnerShip(this);
    setScale(Settings::getInstance()->getIngameMenuScaling());
    connect(Mainapp::getInstance(), &Mainapp::sigKeyDown, this, &HumanPlayerInputMenu::keyInput, Qt::QueuedConnection);
    qint32 width = 0;
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMenuFont32());
    style.hAlign = oxygine::TextStyle::HALIGN_DEFAULT;
    oxygine::spTextField testText = MemoryManagement::create<oxygine::TextField>();
    for (qint32 i = 0; i < texts.size(); i++)
    {
        testText->setStyle(style);
        testText->setHtmlText(texts[i]);
        qint32 newWidth = testText->getTextRect().width() * static_cast<float>(GameMap::getImageSize()) / static_cast<float>(GameMap::defaultImageSize);
        if (newWidth > width )
        {
            width = newWidth;
        }
    }
    width += GameMap::getImageSize() + GameMap::getImageSize() * 3 / 4 ;
    m_itemWidth = width;
    m_itemHeigth = GameMap::getImageSize();
    
    Player* pPlayer = m_pMap->getCurrentViewPlayer();

    GameManager* pGameManager = GameManager::getInstance();
    qint32 heigth = createTopSprite(0, width, pPlayer);
    qint32 y = heigth;
    m_startY = y;
    m_Cursor = MemoryManagement::create<oxygine::Sprite>();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("cursor+menu");
    if (pAnim != nullptr)
    {
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
        m_Cursor->setScale(static_cast<float>(GameMap::getImageSize()) / static_cast<float>(pAnim->getWidth()));
        qint32 x = 0;
        qint32 xCount = 0;
        m_maxXCount = Settings::getInstance()->getMenuItemRowCount();
        if ((oxygine::Stage::getStage()->getWidth() - 120) / width < m_maxXCount)
        {
            m_maxXCount = (oxygine::Stage::getStage()->getWidth() - 120) / width;
        }
        m_rowCount = Settings::getInstance()->getMenuItemCount();
        if ((oxygine::Stage::getStage()->getHeight() - 60) / m_itemHeigth < m_rowCount)
        {
            m_rowCount = (oxygine::Stage::getStage()->getHeight() - 60) / m_itemHeigth;
        }
        qint32 maxY = 0;
        for (qint32 i = 0; i < actionIDs.size(); i++)
        {
            if (i > 0 && i % m_rowCount == 0 && xCount < m_maxXCount)
            {
                xCount++;
                createBottomSprite(x, y, width, pPlayer);
                if (xCount < m_maxXCount)
                {
                    x += width;
                    createTopSprite(x, width, pPlayer);
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
            oxygine::spBox9Sprite pItemBox = createMenuItem(enabled, x, y, width, style, texts[i], action, costs, icons[i], i, pPlayer);
            if (xCount < m_maxXCount)
            {
                y += static_cast<qint32>(pItemBox->getScaledHeight());
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
                    oxygine::spBox9Sprite pItemBox = MemoryManagement::create<oxygine::Box9Sprite>();
                    pAnim = pGameManager->getResAnim("menu+middle+mask");
                    if (pAnim != nullptr)
                    {
                        pItemBox->setResAnim(pAnim);
                        pItemBox->setSize(pAnim->getSize());
                        pItemBox->setColorTable(pPlayer->getColorTableAnim(), true);
                        pItemBox->setHeight(GameMap::getImageSize());
                        pItemBox->setY(y);
                        pItemBox->setX(x);
                        pItemBox->setWidth(width);
                        pItemBox->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *pEvent)->void
                        {
                            oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
                            if (pTouchEvent != nullptr)
                            {
                                pTouchEvent->stopPropagation();
                                if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
                                {
                                    emit sigCanceled(0, 0);
                                }
                            }
                        });
                        addChild(pItemBox);
                        m_ItemActors.append(pItemBox);
                        y += static_cast<qint32>(pItemBox->getScaledHeight());
                        m_itemHeigth = static_cast<qint32>(pItemBox->getScaledHeight());
                    }
                }
            }
        }
        qint32 bottomHeigth = createBottomSprite(x, maxY, width, pPlayer);
        addChild(m_Cursor);
        setPriority(static_cast<qint32>(Mainapp::ZOrder::FocusedObjects));

        qint32 scrollbarWidth = 0;
        if (m_ActionIDs.size() > m_rowCount * m_maxXCount)
        {
            qint32 height = maxY + bottomHeigth;
            qint32 max = m_ItemActors.size() - m_rowCount * m_maxXCount;
            m_scrollbar = MemoryManagement::create<H_Scrollbar>(height, height + max * m_itemHeigth * 1.5f);
            connect(m_scrollbar.get(), &H_Scrollbar::sigScrollValueChanged, this, [this, max](float value)
            {
                m_startItem = max * value;
                updateItemPositionAndVisibility();
            });
            scrollbarWidth = m_scrollbar->getScaledWidth() + 25;
            m_scrollbar->setX(width * xCount + 25);
            addChild(m_scrollbar);
            auto* pScrollbar = m_scrollbar.get();
            addEventListener(oxygine::TouchEvent::WHEEL_DIR, [=](oxygine::Event* pEvent)
            {
                oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
                if (pTouchEvent != nullptr)
                {
                    emit pScrollbar->sigChangeScrollValue(static_cast<float>(-pTouchEvent->wheelDirection.y()) / static_cast<float>(pScrollbar->getContentHeigth()));
                }
            });
        }
        setHeight(maxY + bottomHeigth);
        setWidth(width * xCount + scrollbarWidth);
        m_columnCount = xCount;
        updateItemPositionAndVisibility();
        addTouchMoveEvents();
    }
}

void HumanPlayerInputMenu::addTouchMoveEvents()
{
    if (m_scrollbar.get() != nullptr)
    {
        addTouchDownListener([this](oxygine::Event* event)
        {
            oxygine::TouchEvent* te = oxygine::safeCast<oxygine::TouchEvent*>(event);
            m_lastScrollPoint = te->localPosition;
            m_moveScrolling = true;
            event->stopPropagation();

        });
        addTouchUpListener([this](oxygine::Event* event)
        {
            if (m_moveScrolling)
            {
                m_moveScrolling = false;
            }
            event->stopPropagation();
        });
        addEventListener(oxygine::TouchEvent::OUTX, [this](oxygine::Event* event)
        {
            if (m_moveScrolling)
            {
                m_moveScrolling = false;
            }
        });
        auto * pScrollbar = m_scrollbar.get();
        addEventListener(oxygine::TouchEvent::MOVE, [this, pScrollbar](oxygine::Event* event)
        {
            if (m_moveScrolling)
            {
                oxygine::TouchEvent* te = oxygine::safeCast<oxygine::TouchEvent*>(event);
                QPoint newPos = te->localPosition;
                qint32 speed = -(newPos.y() - m_lastScrollPoint.y());
                if (speed != 0)
                {
                    emit m_scrollbar->sigChangeScrollValue(speed / static_cast<float>(pScrollbar->getContentHeigth()));
                    m_lastScrollPoint = newPos;
                }
            }
        });
    }
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
            y += static_cast<qint32>(m_ItemActors[i]->getScaledHeight());
        }
        else
        {
            m_ItemActors[i]->setVisible(false);
        }
    }
}

oxygine::spBox9Sprite HumanPlayerInputMenu::createMenuItem(bool enabled, qint32& x, qint32& y, qint32 width, oxygine::TextStyle style,
                                                           const QString & text, const QString & action, qint32 costs, oxygine::spActor icon, qint32 item,
                                                           Player* pPlayer)
{
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pItemBox = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("menu+middle+mask");
    if (pAnim != nullptr)
    {
        pItemBox->setResAnim(pAnim);
        pItemBox->setSize(pAnim->getSize());
        pItemBox->setColorTable(pPlayer->getColorTableAnim(), true);
        if (icon.get() != nullptr)
        {
            pItemBox->addChild(icon);
            icon->setPosition(3, 0);
        }
        pItemBox->setHeight(GameMap::getImageSize());
        pItemBox->setY(y);
        pItemBox->setX(x);
        pItemBox->setWidth(width);
        // text for the item
        oxygine::spTextField textField = MemoryManagement::create<oxygine::TextField>();
        // set color font based
        if (!enabled)
        {
            style.color = QColor(255, 0, 0, 255);
        }
        textField->setStyle(style);
        textField->setScale(static_cast<float>(GameMap::getImageSize()) / static_cast<float>(GameMap::defaultImageSize));
        textField->setHtmlText(text);
        textField->setPosition(3 + GameMap::getImageSize(), 0);
        textField->setSize(width - textField->getX(), GameMap::getImageSize());
        pItemBox->addChild(textField);
        addChild(pItemBox);
        oxygine::Actor* pBox = pItemBox.get();
        oxygine::Sprite* pCursor = m_Cursor.get();
        pItemBox->addEventListener(oxygine::TouchEvent::OVER, [this, pCursor, pBox, width, item](oxygine::Event *pEvent)->void
        {
            Mainapp::getInstance()->getAudioManager()->playSound("switchmenu.wav");
            pEvent->stopPropagation();
            pCursor->setY(pBox->getY() + GameMap::getImageSize() / 2 - pCursor->getScaledHeight() / 2);
            pCursor->setX(pBox->getX() + width);
            m_currentAction = item;
        });
        if (enabled)
        {
            pItemBox->addEventListener(oxygine::TouchEvent::CLICK, [this, action, costs](oxygine::Event *pEvent)->void
            {
                oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
                if (pTouchEvent != nullptr)
                {
                    pEvent->stopPropagation();
                    if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
                    {
                        Mainapp::getInstance()->getAudioManager()->playSound("okay.wav");
                        emit sigItemSelected(action, costs);
                    }
                    else if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
                    {
                        emit sigCanceled(0, 0);
                    }
                }
            });
        }
        else
        {
            pItemBox->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event *pEvent)->void
            {
                oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
                if (pTouchEvent != nullptr)
                {
                    pEvent->stopPropagation();
                    if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
                    {
                        emit sigCanceled(0, 0);
                    }
                }
            });
        }
    }
    m_ItemActors.append(pItemBox);
    return pItemBox;
}

qint32 HumanPlayerInputMenu::createBottomSprite(qint32 x, qint32 y, qint32 width, Player* pPlayer)
{
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pBottomBox = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("menu+bottom+mask");
    if (pAnim != nullptr)
    {
        pBottomBox->setResAnim(pAnim);
        pBottomBox->setSize(pAnim->getSize());
        pBottomBox->setColorTable(pPlayer->getColorTableAnim(), true);
        pBottomBox->setWidth(width);
        pBottomBox->setY(y);
        pBottomBox->setX(x);
    }
    addChild(pBottomBox);
    return static_cast<qint32>(pBottomBox->getScaledHeight());
}

qint32 HumanPlayerInputMenu::createTopSprite(qint32 x, qint32 width, Player* pPlayer)
{
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pTopBox = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("menu+top+mask");
    if (pAnim != nullptr)
    {
        pTopBox->setResAnim(pAnim);
        pTopBox->setSize(pAnim->getSize());
        pTopBox->setColorTable(pPlayer->getColorTableAnim(), true);
        pTopBox->setWidth(width);
        pTopBox->setX(x);
    }
    addChild(pTopBox);
    return static_cast<qint32>(pTopBox->getScaledHeight());
}

void HumanPlayerInputMenu::setMenuPosition(qint32 x, qint32 y)
{
    if (x + getWidth() + GameMap::getImageSize() / 2 > m_pMap->getMapWidth() * GameMap::getImageSize())
    {
        x = m_pMap->getMapWidth() * GameMap::getImageSize() - getWidth() - GameMap::getImageSize() / 2;
        if (x < 0)
        {
            x = 0;
        }
    }
    if (y + getHeight() + GameMap::getImageSize() / 2 > m_pMap->getMapHeight() * GameMap::getImageSize())
    {
        y = m_pMap->getMapHeight() * GameMap::getImageSize() - getHeight() - GameMap::getImageSize() / 2;
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
    if (m_pMenu != nullptr)
    {
        Chat* pChat = m_pMenu->getChat();
        if (m_Focused && (pChat == nullptr || pChat->getVisible() == false))
        {
            // for debugging
            Qt::Key cur = event.getKey();
            float max = m_ItemActors.size() - m_rowCount * m_maxXCount;
            float value = 1.0f / max;
            qint32 endItemCount = m_startItem + m_rowCount * m_columnCount;
            if (cur == Settings::getInstance()->getKey_up() ||
                cur == Settings::getInstance()->getKey_up2())
            {
                Mainapp::getInstance()->getAudioManager()->playSound("switchmenu.wav");
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
            else if (cur == Settings::getInstance()->getKey_down() ||
                     cur == Settings::getInstance()->getKey_down2())
            {
                Mainapp::getInstance()->getAudioManager()->playSound("switchmenu.wav");
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
            if (cur == Settings::getInstance()->getKey_left() ||
                cur == Settings::getInstance()->getKey_left2())
            {
                Mainapp::getInstance()->getAudioManager()->playSound("switchmenu.wav");
                if (m_currentAction - m_rowCount >= m_startItem)
                {
                    m_currentAction -= m_rowCount;
                }
                else if (endItemCount > 0)
                {
                    m_currentAction = endItemCount - 1;
                }
                else
                {
                    m_currentAction = m_ActionIDs.size() - 1;
                }
            }
            else if (cur == Settings::getInstance()->getKey_right() ||
                     cur == Settings::getInstance()->getKey_right2())
            {
                Mainapp::getInstance()->getAudioManager()->playSound("switchmenu.wav");
                if (m_currentAction + m_rowCount < endItemCount)
                {
                    m_currentAction += m_rowCount;
                }
                else
                {
                    m_currentAction = m_startItem;
                }
            }
            else if (cur == Settings::getInstance()->getKey_confirm() ||
                     cur == Settings::getInstance()->getKey_confirm2())
            {
                if (!event.getContinousPress())
                {
                    if (m_ActionIDs.size() > 0)
                    {
                        if ((m_EnabledList.size() > 0 && m_EnabledList[m_currentAction]) ||
                            (m_EnabledList.size() == 0))
                        {
                            Mainapp::getInstance()->getAudioManager()->playSound("okay.wav");
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
            }
            else if (cur == Settings::getInstance()->getKey_cancel() ||
                     cur == Settings::getInstance()->getKey_cancel2())
            {
                if (!event.getContinousPress())
                {
                    emit sigCanceled(0, 0);
                }
            }
            else if (cur == Settings::getInstance()->getKey_information() ||
                     cur == Settings::getInstance()->getKey_information2())
            {
                if (!event.getContinousPress())
                {
                    QString id = m_ActionIDs[m_currentAction];
                    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
                    if (pUnitSpriteManager->exists(id))
                    {
                        spUnit pDummy = MemoryManagement::create<Unit>(id, m_pMap->getCurrentPlayer(), false, m_pMap);
                        spFieldInfo fieldinfo = MemoryManagement::create<FieldInfo>(nullptr, pDummy);
                        m_pMenu->addChild(fieldinfo);
                        connect(fieldinfo.get(), &FieldInfo::sigFinished, this, [this]
                        {
                            m_Focused = true;
                        });
                        m_Focused = false;
                    }
                    else
                    {
                        WikiDatabase* pDatabase = WikiDatabase::getInstance();
                        const PageData * data = pDatabase->getEntry(id);
                        if (data->m_id != "")
                        {
                            spWikipage page = pDatabase->getPage(data);
                            m_pMenu->addChild(page);
                            connect(page.get(), &FieldInfo::sigFinished, this, [this]
                            {
                                m_Focused = true;
                            });
                            m_Focused = false;
                        }
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
    Mainapp* pApp = Mainapp::getInstance();
    if (pApp->hasCursor() && Settings::getInstance()->getAutoMoveCursor())
    {
#ifdef GRAPHICSUPPORT
        QPoint pos = local2stage();
        QPoint curPos = pApp->mapPosToGlobal(QPoint(pos.x() + m_itemWidth / 2 + m_itemWidth * x, pos.y() + m_startY + m_itemHeigth / 2 + m_itemHeigth * y));
        pApp->cursor().setPos(curPos);
#endif
    }
}
