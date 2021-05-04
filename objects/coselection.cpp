#include "coselection.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/gamemanager.h"

COSelection::COSelection(QSize maxSize, QStringList coids)
    : QObject(),
      m_Coids(coids)
{
    setObjectName("COSelection");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();

    pCOSpriteManager->loadAll();
    Interpreter* pInterpreter = Interpreter::getInstance();
    // create a co banner for each co in a different army
    qint32 bannerX = 0;
    qint32 y = 0;
    oxygine::ResAnim* pAnim = nullptr;
    oxygine::spSprite pSprite;
    QString function1 = "getArmies";
    QJSValueList args1;
    QJSValue ret = pInterpreter->doFunction("PLAYER", function1, args1);
    m_Armies = ret.toVariant().toStringList();
    QStringList allowedArmies;
    // remove unused armies
    // first search avaible armies if the selection is set like that
    for (qint32 i = 0; i < m_Coids.size(); i++)
    {
        QString function1 = "getCOArmy";
        QJSValue ret = pInterpreter->doFunction(m_Coids[i], function1);
        if (ret.isString())
        {
            QString army = ret.toString();
            if (!allowedArmies.contains(army))
            {
                allowedArmies.append(army);
            }
        }
    }
    // we have allowed armies? Else allow everything
    if (allowedArmies.size() > 0)
    {
        // remove all other armies
        qint32 iter = 0;
        while (iter < m_Armies.size())
        {
            if (allowedArmies.contains(m_Armies[iter]))
            {
                iter++;
            }
            else
            {
                m_Armies.removeAt(iter);
            }
        }
    }

    m_ArmyBannerPanel = spPanel::create(true, QSize(maxSize.width(),  75 * scale + 22), QSize(m_Armies.size() * 20 * scale + 40, 53 * scale + 22));
    addChild(m_ArmyBannerPanel);
    for (qint32 i = 0; i < m_Armies.size(); i++)
    {
        loadArmy(m_Armies[i], bannerX, y, i);
    }
    m_CoFieldPanel = spPanel::create(true, QSize(51 * 3 * scale + 80, maxSize.height() - m_ArmyBannerPanel->getScaledHeight()), QSize(51 * 3 * scale + 80, maxSize.height() - m_ArmyBannerPanel->getScaledHeight()));
    m_CoFieldPanel->setY(m_ArmyBannerPanel->getScaledHeight());
    addChild(m_CoFieldPanel);

    qint32 width = (maxSize.width() - m_CoFieldPanel->getScaledWidth()) / 2;
    m_CoDescription = spPanel::create(true, QSize(width, maxSize.height() - m_ArmyBannerPanel->getScaledHeight()), QSize(width, maxSize.height() - m_ArmyBannerPanel->getScaledHeight()));
    m_CoDescription->setPosition(m_CoFieldPanel->getScaledWidth(), m_ArmyBannerPanel->getScaledHeight());
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
    m_Cursor = oxygine::spSprite::create();
    m_Cursor->setResAnim(pAnim);
    m_Cursor->setPosition(5, 7);
    m_Cursor->setPriority(1);
    m_Cursor->setScale(scale);
    m_CoFieldPanel->addItem(m_Cursor);

    oxygine::spBox9Sprite pPanelbox = oxygine::spBox9Sprite::create();
    pAnim = pObjectManager->getResAnim("panel");
    pPanelbox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pPanelbox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pPanelbox->setResAnim(pAnim);
    pPanelbox->setX(m_CoDescription->getX() + m_CoDescription->getScaledWidth());
    pPanelbox->setY(m_ArmyBannerPanel->getScaledHeight());
    pPanelbox->setSize(width, maxSize.height() - m_ArmyBannerPanel->getScaledHeight());

    m_pCurrentCO = oxygine::spSprite::create();
    m_pCurrentCO->setPosition(10, 10);
    float scale = (width - 20) / 208.0f;
    float scale2 = (maxSize.height() - m_ArmyBannerPanel->getScaledHeight() - 20) / 352.0f;
    if (scale2 < scale)
    {
        scale = scale2;
    }
    m_pCurrentCO->setScale(scale);
    pPanelbox->addChild(m_pCurrentCO);
    addChild(pPanelbox);

    connect(this, &COSelection::armySelectedChange, this, &COSelection::armyChanged, Qt::QueuedConnection);
    oxygine::TextStyle headerStyle = FontManager::getMainFont48();
    headerStyle.color = FontManager::getFontColor();
    headerStyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;

    m_COName = spLabel::create(m_CoDescription->getWidth());
    m_COName->setStyle(headerStyle);
    m_COName->setSize(width, 55);
    m_COName->setPosition(0, 10);
    m_CoDescription->addItem(m_COName);

    width -= 70;
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_COPower = spLabel::create(m_CoDescription->getWidth());
    m_COPower->setStyle(style);
    m_COPower->setPosition(0, 70);
    m_CoDescription->addItem(m_COPower);
    m_COSuperpower = spLabel::create(m_CoDescription->getWidth());
    m_COSuperpower->setStyle(style);
    m_COSuperpower->setPosition(0, 100);
    m_CoDescription->addItem(m_COSuperpower);

    style.multiline = true;

    m_COBio = oxygine::spTextField::create();
    m_COBio->setStyle(style);
    m_COBio->setSize(width, 20);
    m_COBio->setY(150);
    m_CoDescription->addItem(m_COBio);

    m_CODesc = oxygine::spTextField::create();
    m_CODesc->setStyle(style);
    m_CODesc->setSize(width, 48);
    m_CoDescription->addItem(m_CODesc);
    armyBannerClicked(m_Armies[0], 0);
}

void COSelection::loadArmy(QString army, qint32& bannerX, qint32& y, qint32 i)
{
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim(army);
    if (pAnim != nullptr)
    {
        oxygine::spClipRectActor pRect = oxygine::spClipRectActor::create();
        pRect->setPosition(bannerX, 0);
        pRect->setSize(pAnim->getWidth() * scale, pAnim->getHeight() * scale);
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        pSprite->setResAnim(pAnim);
        pSprite->setScale(scale);
        if (i != 0)
        {
            pSprite->setPosition(0, -12 * scale);
        }
        pRect->addChild(pSprite);
        m_ArmyBanners.append(pSprite);

        qint32 index = m_ArmyBanners.size() - 1;
        pSprite->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
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

COSelection::~COSelection()
{

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
    oxygine::spActor actor = oxygine::spActor::create();
    if (COArmy == army)
    {
        QString resAnim = coid.toLower() + "+face";
        pAnim = pCOSpriteManager->getResAnim(resAnim);
        pSprite = oxygine::spSprite::create();
        pSprite->setResAnim(pAnim);
        actor->addChild(pSprite);
    }
    else
    {
        pAnim = pObjectManager->getResAnim("no_co");
        pSprite = oxygine::spSprite::create();
        pSprite->setResAnim(pAnim);
        actor->addChild(pSprite);
        coid = "";

    }
    pSprite->setScale(scale);
    pAnim = pObjectManager->getResAnim("co_background");
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setScale(scale);
    actor->addChild(pSprite);

    actor->setPosition(5 + x * 51 * scale, 7 + y * 51 * scale);
    actor->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
    {
        m_Cursor->setPosition(5 + x * 51 * scale, 7 + y * 51 * scale);
        m_CurrentCO = coid;
    });
    connect(this, &COSelection::sigHoveredCOChanged, this, &COSelection::hoveredCOChanged, Qt::QueuedConnection);
    actor->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigHoveredCOChanged(m_CurrentCO);
        emit coSelected(m_CurrentCO);
    });
    m_COFields.append(actor);
    m_CoFieldPanel->addItem(actor);
    m_CoIDs.append(coid);
}

void COSelection::colorChanged(QColor color)
{
    
    QColor colorAlpha(color);
    colorAlpha.setAlpha(120);
    for (qint32 i = 0; i < m_COFields.size(); i++)
    {
        oxygine::Sprite* pSprite = dynamic_cast<oxygine::Sprite*>(m_COFields[i]->getLastChild().get());
        pSprite->setColor(colorAlpha);
    }
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
        m_COName->setX(5);
//        m_COName->setX(m_CoDescription->getWidth() / 2 - m_COName->getTextRect().getWidth() / 2 - 35);
//        if (m_COName->getX() < 5)
//        {
//            m_COName->setX(5);
//        }

        m_COBio->setHtmlText(coBio);
        m_COBio->setHeight(m_COBio->getTextRect().getHeight() + 20);

        m_CODesc->setHtmlText(coDesc);
        m_CODesc->setHeight(m_CODesc->getTextRect().getHeight() + 20);
        m_CODesc->setY(m_COBio->getY() + m_COBio->getHeight() + 10);

        m_CoDescription->setContentHeigth(m_CODesc->getY() + m_CODesc->getHeight() + 20);

        m_COPower->setHtmlText(coPower);
        m_COSuperpower->setHtmlText(coSuperpower);

        COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
        oxygine::ResAnim* pAnim = nullptr;
        if (!coid.isEmpty())
        {
            pAnim = pCOSpriteManager->getResAnim((coid + "+nrm"));
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
