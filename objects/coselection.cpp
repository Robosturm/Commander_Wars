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
    // delete unused armies
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
        // delete all other armies
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

    m_ArmyBannerPanel = new Panel(true, QSize(maxSize.width(),  53 * scale + 55), QSize(m_Armies.size() * 25 * scale+ 20, 53 * scale + 55));
    addChild(m_ArmyBannerPanel);
    for (qint32 i = 0; i < m_Armies.size(); i++)
    {
        loadArmy(m_Armies[i], bannerX, y, i);
    }
    m_CoFieldPanel = new Panel(true, QSize(51 * 3 * scale + 80, maxSize.height() - m_ArmyBannerPanel->getScaledHeight()), QSize(51 * 3 * scale + 80, maxSize.height() - m_ArmyBannerPanel->getScaledHeight()));
    m_CoFieldPanel->setY(m_ArmyBannerPanel->getScaledHeight());
    addChild(m_CoFieldPanel);

    qint32 width = (maxSize.width() - m_CoFieldPanel->getScaledWidth()) / 2;
    m_CoDescription = new Panel(true, QSize(width, maxSize.height() - m_ArmyBannerPanel->getScaledHeight()), QSize(width, maxSize.height() - m_ArmyBannerPanel->getScaledHeight()));
    m_CoDescription->setPosition(m_CoFieldPanel->getScaledWidth(), m_ArmyBannerPanel->getScaledHeight());
    addChild(m_CoDescription);

    for (qint32 i = 0; i < pCOSpriteManager->getCOCount(); i++)
    {
        QString coid = pCOSpriteManager->getCOID(i);
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
    m_Cursor = new oxygine::Sprite();
    m_Cursor->setResAnim(pAnim);
    m_Cursor->setPosition(5, 7 + y);
    m_Cursor->setPriority(1);
    m_Cursor->setScale(scale);
    m_CoFieldPanel->addItem(m_Cursor);

    m_pCurrentCO = new oxygine::Sprite();
    m_pCurrentCO->setPosition(m_CoDescription->getX() + m_CoDescription->getScaledWidth(), m_ArmyBannerPanel->getScaledHeight());
    float scale = width / 208.0f;
    float scale2 = (maxSize.height() - m_ArmyBannerPanel->getScaledHeight()) / 352.0f;
    if (scale2 < scale)
    {
        scale = scale2;
    }
    m_pCurrentCO->setScale(scale);
    addChild(m_pCurrentCO);

    connect(this, &COSelection::armySelectedChange, this, &COSelection::armyChanged, Qt::QueuedConnection);
    oxygine::TextStyle headerStyle = FontManager::getMainFont48();
    headerStyle.color = FontManager::getFontColor();
    headerStyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;

    m_COName = new oxygine::TextField();
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
    m_COPower = new oxygine::TextField();
    m_COPower->setStyle(style);
    m_COPower->setPosition(0, 70);
    m_CoDescription->addItem(m_COPower);
    m_COSuperpower = new oxygine::TextField();
    m_COSuperpower->setStyle(style);
    m_COSuperpower->setPosition(0, 100);
    m_CoDescription->addItem(m_COSuperpower);

    style.multiline = true;

    m_COBio = new oxygine::TextField();
    m_COBio->setStyle(style);
    m_COBio->setSize(width, 20);
    m_COBio->setY(150);
    m_CoDescription->addItem(m_COBio);

    m_CODesc = new oxygine::TextField();
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
        oxygine::spClipRectActor pRect = new oxygine::ClipRectActor();
        pRect->setPosition(bannerX, 0);
        pRect->setSize(pAnim->getWidth() * scale, pAnim->getHeight() * scale);
        oxygine::spSprite pSprite = new oxygine::Sprite();
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
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
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
        // delete all unallowed co's
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
    while (y < 5)
    {
        qint32 x = startX;
        if (y != startY)
        {
            x = 0;
        }
        while (x < 3)
        {
            bool coFound = false;
            for (qint32 i = 0; i < pCOSpriteManager->getCOCount(); i++)
            {
                QString coid = pCOSpriteManager->getCOID(i);
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
            if (!coFound)
            {
                coFound = true;
                addCO("", "", x, y, army);
            }
            x++;
        }
        y++;
    }
    colorChanged(m_CurrentColor);
    m_CoFieldPanel->setContentHeigth(y * 51 + 30);
    pApp->continueThread();
}

void COSelection::addCO(QString coid, QString COArmy, qint32 x, qint32 y, QString army)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = nullptr;
    oxygine::spSprite pSprite;
    oxygine::spActor actor = new oxygine::Actor();
    if (COArmy == army)
    {
        QString resAnim = coid.toLower() + "+face";
        pAnim = pCOSpriteManager->getResAnim(resAnim);
        pSprite = new oxygine::Sprite();
        pSprite->setResAnim(pAnim);
        actor->addChild(pSprite);
    }
    else
    {
        pAnim = pObjectManager->getResAnim("no_co");
        pSprite = new oxygine::Sprite();
        pSprite->setResAnim(pAnim);
        actor->addChild(pSprite);
        coid = "";

    }
    pSprite->setScale(scale);
    pAnim = pObjectManager->getResAnim("co_background");
    pSprite = new oxygine::Sprite();
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
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    QColor colorAlpha(color);
    colorAlpha.setAlpha(120);
    for (qint32 i = 0; i < m_COFields.size(); i++)
    {
        oxygine::Sprite* pSprite = dynamic_cast<oxygine::Sprite*>(m_COFields[i]->getLastChild().get());
        pSprite->setColor(colorAlpha);
    }
    m_Cursor->setColor(color);
    m_CurrentColor = color;
    pApp->continueThread();
}

void COSelection::hoveredCOChanged(QString coid)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
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
        m_COName->setX(m_CoDescription->getWidth() / 2 - m_COName->getTextRect().getWidth() / 2 - 35);

        m_COBio->setHtmlText(coBio);
        m_COBio->setHeight(m_COBio->getTextRect().getHeight() + 20);

        m_CODesc->setHtmlText(coDesc);
        m_CODesc->setHeight(m_CODesc->getTextRect().getHeight() + 20);
        m_CODesc->setY(m_COBio->getY() + m_COBio->getHeight() + 10);

        m_COPower->setHtmlText(coPower);
        m_COSuperpower->setHtmlText(coSuperpower);

        COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
        oxygine::ResAnim* pAnim = nullptr;
        if (!coid.isEmpty())
        {
            pAnim = pCOSpriteManager->getResAnim((coid + "+nrm"));
        }
        m_pCurrentCO->setResAnim(pAnim);

        addChild(m_pCurrentCO);
    }
    pApp->continueThread();
}
