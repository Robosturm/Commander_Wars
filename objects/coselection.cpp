#include "objects/coselection.h"

#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/gamemanager.h"

#include "objects/base/moveinbutton.h"

COSelection::COSelection(QPoint position, QSize maxSize, QStringList coids)
    : m_Coids(coids)
{
#ifdef GRAPHICSUPPORT
    setObjectName("COSelection");
#endif
    Interpreter::setCppOwnerShip(this);
    setPosition(position.x(), position.y());

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    Interpreter* pInterpreter = Interpreter::getInstance();
    // create a co banner for each co in a different army
    qint32 bannerX = 0;
    qint32 y = 0;
    oxygine::ResAnim* pAnim = nullptr;
    oxygine::spSprite pSprite;
    m_Armies = pCOSpriteManager->getArmyList(m_Coids);

    m_ArmyBannerPanel = MemoryManagement::create<Panel>(true, QSize(maxSize.width(),  75 * scale + 22), QSize(m_Armies.size() * 20 * scale + 40, 53 * scale + 22));
    m_ArmyBannerPanel->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects) + 1);
    addChild(m_ArmyBannerPanel);
    for (qint32 i = 0; i < m_Armies.size(); i++)
    {
        loadArmy(m_Armies[i], bannerX, y, i);
    }
    qint32 baseYSize = maxSize.height();
    qint32 startY = m_ArmyBannerPanel->getScaledHeight();
    if (Settings::getInstance()->getSmallScreenDevice())
    {
        auto moveInButton = MemoryManagement::create<MoveInButton>(m_ArmyBannerPanel.get(), m_ArmyBannerPanel->getScaledHeight(), 1, -1, 2.0f, true);
        m_ArmyBannerPanel->setY(-m_ArmyBannerPanel->getScaledHeight() - getY() + 1);
        m_ArmyBannerPanel->addChild(moveInButton);
        startY = 0;
    }
    else
    {
        baseYSize -= m_ArmyBannerPanel->getScaledHeight();
    }
    m_CoFieldPanel = MemoryManagement::create<Panel>(true, QSize(51 * 3 * scale + 80, baseYSize), QSize(51 * 3 * scale + 80, baseYSize));
    m_CoFieldPanel->setY(startY);

    addChild(m_CoFieldPanel);

    qint32 width = (maxSize.width() - m_CoFieldPanel->getScaledWidth());
    if (!Settings::getInstance()->getSmallScreenDevice())
    {
        width /= 2;
    }
    m_CoDescription = MemoryManagement::create<Panel>(true, QSize(width, baseYSize), QSize(width, baseYSize));
    m_CoDescription->setPosition(m_CoFieldPanel->getScaledWidth(), startY);
    addChild(m_CoDescription);

    for (qint32 i = 0; i < pCOSpriteManager->getCount(); i++)
    {
        QString coid = pCOSpriteManager->getID(i);
        if (m_Coids.isEmpty() || m_Coids.contains(coid))
        {
            QString function1 = "getCOArmy";
            QJSValue ret = pInterpreter->doFunction(coid, function1);
            if (ret.isString())
            {
                QString army = ret.toString();
                if (!m_Armies.contains(army))
                {
                    m_Armies.append(army);
                    loadArmy(army, bannerX, y, i);
                }
            }
        }
    }

    pAnim = pObjectManager->getResAnim("co_cursor");
    m_Cursor = MemoryManagement::create<oxygine::Sprite>();
    m_Cursor->setResAnim(pAnim);
    m_Cursor->setPosition(5, 7);
    m_Cursor->setPriority(1);
    m_Cursor->setScale(scale);
    m_CoFieldPanel->addItem(m_Cursor);

    oxygine::spBox9Sprite pPanelbox = MemoryManagement::create<oxygine::Box9Sprite>();
    pAnim = pObjectManager->getResAnim("panel");
    pPanelbox->setResAnim(pAnim);
    pPanelbox->setX(m_CoDescription->getX() + m_CoDescription->getScaledWidth());
    pPanelbox->setY(startY);
    pPanelbox->setSize(width, baseYSize);

    m_pCurrentCO = MemoryManagement::create<oxygine::Sprite>();
    m_pCurrentCO->setPosition(10, 10);
    float scale = (width - 20) / 208.0f;
    float scale2 = (maxSize.height() - m_ArmyBannerPanel->getScaledHeight() - 20) / 352.0f;
    if (scale2 < scale)
    {
        scale = scale2;
    }
    m_pCurrentCO->setScale(scale);
    pPanelbox->addChild(m_pCurrentCO);
    if (Settings::getInstance()->getSmallScreenDevice())
    {
        pPanelbox->setVisible(false);
    }
    addChild(pPanelbox);

    connect(this, &COSelection::armySelectedChange, this, &COSelection::armyChanged, Qt::QueuedConnection);
    oxygine::TextStyle headerStyle = oxygine::TextStyle(FontManager::getMainFont48());
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;

    m_COName = MemoryManagement::create<Label>(m_CoDescription->getScaledWidth());
    m_COName->setStyle(headerStyle);
    m_COName->setSize(width, 55);
    m_COName->setPosition(10, 10);
    m_CoDescription->addItem(m_COName);

    width -= 70;
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_COPower = MemoryManagement::create<Label>(m_CoDescription->getScaledWidth());
    m_COPower->setStyle(style);
    m_COPower->setPosition(10, 70);
    m_CoDescription->addItem(m_COPower);
    m_COSuperpower = MemoryManagement::create<Label>(m_CoDescription->getScaledWidth());
    m_COSuperpower->setStyle(style);
    m_COSuperpower->setPosition(10, 100);
    m_CoDescription->addItem(m_COSuperpower);

    style.multiline = true;

    m_COBio = MemoryManagement::create<oxygine::TextField>();
    m_COBio->setStyle(style);
    m_COBio->setSize(width, 20);
    m_COBio->setPosition(10, 150);
    m_CoDescription->addItem(m_COBio);

    m_CODesc = MemoryManagement::create<oxygine::TextField>();
    m_CODesc->setStyle(style);
    m_CODesc->setSize(width, 48);
    m_CODesc->setX(10);
    m_CoDescription->addItem(m_CODesc);
    armyBannerClicked(m_Armies[0], 0);
}

void COSelection::loadArmy(QString army, qint32& bannerX, qint32& y, qint32 i)
{
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim(army);
    if (pAnim != nullptr)
    {
        oxygine::spClipRectActor pRect = MemoryManagement::create<oxygine::ClipRectActor>();
        pRect->setPosition(bannerX, 0);
        pRect->setSize(pAnim->getWidth() * scale, pAnim->getHeight() * scale);
        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
        pSprite->setResAnim(pAnim);
        pSprite->setScale(scale);
        if (i != 0)
        {
            pSprite->setPosition(0, -12 * scale);
        }
        pRect->addChild(pSprite);
        m_ArmyBanners.append(pSprite);

        qint32 index = m_ArmyBanners.size() - 1;
        pSprite->addEventListener(oxygine::TouchEvent::CLICK, [this, army, index](oxygine::Event*)
        {
            armyBannerClicked(army, index);
        });
        m_ArmyBannerPanel->addItem(pRect);
        bannerX += static_cast<qint32>(pAnim->getWidth() * scale);
        y = static_cast<qint32>(pAnim->getHeight() * scale) + 5;
    }
}

void COSelection::armyBannerClicked(QString army, qint32 index)
{
    if (index >= 0 && index < m_ArmyBanners.size())
    {
        for (qint32 i2 = 0; i2 < m_ArmyBanners.size(); i2++)
        {
            m_ArmyBanners[i2]->setY(-12 * scale);
        }
        m_ArmyBanners[index]->setY(0);
        emit armySelectedChange(army);
    }
}

void COSelection::armyChanged(QString army)
{
    
    for (qint32 i = 0; i < m_COFields.size(); i++)
    {
        m_COFields[i]->detach();
    }
    m_COFields.clear();
    m_CoIDs.clear();    

    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue ret = pInterpreter->doFunction("PLAYER", "getArmyCOs" + army);
    QStringList preSetCOOrder = ret.toVariant().toStringList();
    qint32 index = 0;
    if (m_Coids.size() > 0)
    {
        // remove all unallowed co's
        while (index < preSetCOOrder.size())
        {
            if (m_Coids.contains(preSetCOOrder[index]))
            {
                index++;
            }
            else
            {
                preSetCOOrder.removeAt(index);
            }
        }
    }
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    qint32 startX = 0;
    qint32 startY = 0;
    for (qint32 i = 0; i < preSetCOOrder.size(); i++)
    {
        addCO(preSetCOOrder[i], "dummy", startX, startY, "dummy");
        startX++;
        if (startX >= 3)
        {
            startY++;
            startX = 0;
        }
    }
    qint32 y = startY;
    bool coFound = true;
    bool emptyCOsAdded = false;
    while (coFound && !emptyCOsAdded)
    {
        qint32 x = startX;
        if (y != startY)
        {
            x = 0;
        }
        while (x < 3)
        {
            coFound = false;
            for (qint32 i = 0; i < pCOSpriteManager->getCount(); i++)
            {
                QString coid = pCOSpriteManager->getID(i);
                if ((m_Coids.isEmpty() || m_Coids.contains(coid)) &&
                    !m_CoIDs.contains(coid))
                {
                    QString function1 = "getCOArmy";
                    QJSValue ret = pInterpreter->doFunction(coid, function1);
                    if (ret.isString())
                    {
                        QString COArmy = ret.toString();
                        if (COArmy == army && !m_CoIDs.contains(coid))
                        {
                            coFound = true;
                            addCO(coid, COArmy, x, y, army);
                            break;
                        }
                    }
                }
            }
            if (!coFound && (!emptyCOsAdded || x < 3))
            {
                coFound = true;
                emptyCOsAdded = true;
                addCO("", "", x, y, army);
            }
            x++;
        }
        y++;
    }
    colorChanged(m_CurrentColor);
    m_CoFieldPanel->setContentHeigth(y * 51 * scale + 30);
    
}

void COSelection::addCO(QString coid, QString COArmy, qint32 x, qint32 y, QString army)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = nullptr;
    oxygine::spSprite pSprite;
    oxygine::spActor actor = MemoryManagement::create<oxygine::Actor>();
    if (COArmy == army && !coid.isEmpty())
    {
        QString resAnim = coid.toLower() + "+face";
        pAnim = pCOSpriteManager->getResAnim(resAnim);        
        pSprite = MemoryManagement::create<oxygine::Sprite>();
        pSprite->setResAnim(pAnim);
        actor->addChild(pSprite);
    }
    else
    {
        pAnim = pCOSpriteManager->getResAnim("no_co+face");
        pSprite = MemoryManagement::create<oxygine::Sprite>();
        pSprite->setResAnim(pAnim);
        actor->addChild(pSprite);
        coid = "";
    }
    pSprite->setScale(scale);
    pAnim = pObjectManager->getResAnim("co_background");
    pSprite = MemoryManagement::create<oxygine::Sprite>();
    pSprite->setResAnim(pAnim);
    pSprite->setScale(scale);
    actor->addChild(pSprite);

    actor->setPosition(5 + x * 51 * scale, 7 + y * 51 * scale);
    auto* pCursor = m_Cursor.get();
    actor->addEventListener(oxygine::TouchEvent::OVER, [this, pCursor, x, y, coid](oxygine::Event*)
    {
        pCursor->setPosition(5 + x * 51 * scale, 7 + y * 51 * scale);
        m_CurrentCO = coid;
    });
    connect(this, &COSelection::sigHoveredCOChanged, this, &COSelection::hoveredCOChanged, Qt::QueuedConnection);
    actor->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigHoveredCOChanged(m_CurrentCO);
        emit coSelected(m_CurrentCO);
    });
    m_COFields.append(actor);
    m_CoFieldPanel->addItem(actor);
    m_CoIDs.append(coid);
}

const QStringList &COSelection::getCoids() const
{
    return m_Coids;
}

void COSelection::colorChanged(QColor color)
{    
    QColor colorAlpha(color);
    colorAlpha.setAlpha(120);

    Mainapp::getInstance()->pauseRendering();
    for (qint32 i = 0; i < m_COFields.size(); i++)
    {
        oxygine::spSprite pSprite = oxygine::safeSpCast<oxygine::Sprite>(m_COFields[i]->getLastChild());
        pSprite->setColor(colorAlpha);
    }
    Mainapp::getInstance()->continueRendering();

    m_Cursor->setColor(color);
    m_CurrentColor = color;    
}

void COSelection::hoveredCOChanged(QString coid)
{
    
    if (coid != "")
    {
        QString coName = "";
        QString coBio = "";
        QString coDesc = "";
        QString coPower = "";
        QString coSuperpower = "";

        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue value = pInterpreter->doFunction(coid, "getName");
        if (value.isString())
        {
            coName = value.toString();
        }
        value = pInterpreter->doFunction(coid, "getBio");
        if (value.isString())
        {
            coBio = value.toString();
        }
        value = pInterpreter->doFunction(coid, "getCODescription");
        if (value.isString())
        {
            coDesc = value.toString();
        }
        value = pInterpreter->doFunction(coid, "getPowerName");
        if (value.isString())
        {
            coPower = value.toString();
        }
        value = pInterpreter->doFunction(coid, "getSuperPowerName");
        if (value.isString())
        {
            coSuperpower = value.toString();
        }
        m_COName->setHtmlText(coName);
        m_COName->setX(10);

        m_COBio->setHtmlText(coBio);
        m_COBio->setHeight(m_COBio->getTextRect().height() + 20);

        m_CODesc->setHtmlText(coDesc);
        m_CODesc->setHeight(m_CODesc->getTextRect().height() + 20);
        m_CODesc->setY(m_COBio->getY() + m_COBio->getScaledHeight() + 10);

        m_CoDescription->setContentHeigth(m_CODesc->getY() + m_CODesc->getScaledHeight() + 20);

        m_COPower->setHtmlText(coPower);
        m_COSuperpower->setHtmlText(coSuperpower);

        COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
        oxygine::ResAnim* pAnim = nullptr;
        if (!coid.isEmpty())
        {
            pAnim = pCOSpriteManager->getResAnim(coid + "+nrm", oxygine::ep_ignore_error);
        }
        m_pCurrentCO->setResAnim(pAnim);
    }
    else
    {
        m_COName->setHtmlText("");
        m_COBio->setHtmlText("");
        m_CODesc->setHtmlText("");
        m_CoDescription->setContentHeigth(0);
        m_COPower->setHtmlText("");
        m_COSuperpower->setHtmlText("");
        m_pCurrentCO->setResAnim(nullptr);
    }    
}
