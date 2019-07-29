#include "coselection.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/fontmanager.h"

COSelection::COSelection(QStringList coids)
    : QObject(),
      m_Coids(coids)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    pCOSpriteManager->loadAll();
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
    // create a co banner for each co in a different army
    qint32 bannerX = 0;
    qint32 y = 0;
    oxygine::ResAnim* pAnim = nullptr;
    oxygine::spSprite pSprite;
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
                    pAnim = pObjectManager->getResAnim(army.toStdString().c_str());
                    if (pAnim != nullptr)
                    {
                        m_Armies.append(army);
                        oxygine::spClipRectActor pRect = new oxygine::ClipRectActor();
                        pRect->setPosition(bannerX, 0);
                        pRect->setSize(pAnim->getWidth(), pAnim->getHeight());
                        pSprite = new oxygine::Sprite();
                        pSprite->setResAnim(pAnim);
                        if (i != 0)
                        {
                            pSprite->setPosition(0, -12);
                        }
                        pRect->addChild(pSprite);
                        m_ArmyBanners.append(pSprite);

                        qint32 index = m_ArmyBanners.size() - 1;
                        pSprite->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
                        {
                            armyBannerClicked(army, index);
                        });
                        addChild(pRect);
                        bannerX += pAnim->getWidth();
                        y = pAnim->getHeight() + 5;
                    }
                }
            }
        }
    }

    // add border box
    pAnim = pObjectManager->getResAnim("co_selection");
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnim);
    pSprite->setPosition(0, y);
    addChild(pSprite);
    this->setSize(pAnim->getWidth(), pAnim->getHeight() + y);

    pAnim = pObjectManager->getResAnim("co_selection+mask");
    m_BackgroundMask = new oxygine::Sprite();
    m_BackgroundMask->setResAnim(pAnim);
    m_BackgroundMask->setPosition(0, y);

    addChild(m_BackgroundMask);
    pAnim = pObjectManager->getResAnim("co_cursor");
    m_Cursor = new oxygine::Sprite();
    m_Cursor->setResAnim(pAnim);
    m_Cursor->setPosition(5, 7 + m_BackgroundMask->getY());
    m_Cursor->setPriority(1);

    addChild(m_Cursor);

    connect(this, &COSelection::armySelectedChange, this, &COSelection::armyChanged, Qt::QueuedConnection);
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    m_COName = new oxygine::TextField();
    m_COName->setStyle(style);
    m_COName->setSize(175 / 0.8f, 20);
    m_COName->setScale(0.8f);
    m_COName->setPosition(163, 15 + m_BackgroundMask->getY());
    addChild(m_COName);

    m_COPower = new oxygine::TextField();
    m_COPower->setStyle(style);
    m_COPower->setPosition(165, 154 + m_BackgroundMask->getY());
    m_COPower->setScale(0.6f);
    addChild(m_COPower);

    m_COSuperpower = new oxygine::TextField();
    m_COSuperpower->setStyle(style);
    m_COSuperpower->setPosition(165, 170 + m_BackgroundMask->getY());
    m_COSuperpower->setScale(0.6f);
    addChild(m_COSuperpower);

    style.multiline = true;

    m_COBio = new oxygine::TextField();
    m_COBio->setStyle(style);
    m_COBio->setScale(0.5f);
    m_COBio->setSize(175 / m_COBio->getScaleY(), 20);

    m_COBioRect = new oxygine::SlidingActor();
    m_COBioRect->setPosition(m_COName->getX() + 2, m_COName->getY() + m_COName->getHeight() + 5);
    m_COBioRect->setSize(179, 110 - m_COName->getHeight());
    m_COBioRect->setContent(m_COBio);
    addChild(m_COBioRect);

    m_CODesc = new oxygine::TextField();
    m_CODesc->setStyle(style);
    m_CODesc->setScale(0.5f);
    m_CODesc->setSize(175 / m_CODesc->getScaleY(), 48);

    m_CODescRect = new oxygine::SlidingActor();
    m_CODescRect->setPosition(m_COName->getX() + 2, 212 + m_BackgroundMask->getY());
    m_CODescRect->setSize(179, 48);
    m_CODescRect->setContent(m_COBio);
    addChild(m_CODescRect);

    armyBannerClicked(m_Armies[0], 0);
}

void COSelection::armyBannerClicked(QString army, qint32 index)
{
    if (index >= 0 && index < m_ArmyBanners.size())
    {
        for (qint32 i2 = 0; i2 < m_ArmyBanners.size(); i2++)
        {
            m_ArmyBanners[i2]->setY(-12);
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
        removeChild(m_COFields[i]);
    }
    m_COFields.clear();
    m_CoIDs.clear();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
    qint32 coStartIndex = 0;
    oxygine::ResAnim* pAnim = nullptr;
    oxygine::spSprite pSprite;
    for (qint32 y = 0; y < 5; y++)
    {
        for (qint32 x = 0; x < 3; x++)
        {
            oxygine::spActor actor = new oxygine::Actor();
            bool coFound = false;
            QString coid = "";
            for (qint32 i = coStartIndex; i < pCOSpriteManager->getCOCount(); i++)
            {
                coid = pCOSpriteManager->getCOID(i);
                if (m_Coids.isEmpty() || m_Coids.contains(coid))
                {
                    QString function1 = "getCOArmy";
                    QJSValue ret = pInterpreter->doFunction(coid, function1);
                    if (ret.isString())
                    {
                        QString COArmy = ret.toString();
                        if (COArmy == army)
                        {
                            coStartIndex = i + 1;
                            coFound = true;
                            QString resAnim = coid.toLower() + "+face";
                            pAnim = pCOSpriteManager->getResAnim(resAnim.toStdString().c_str());
                            pSprite = new oxygine::Sprite();
                            pSprite->setResAnim(pAnim);
                            actor->addChild(pSprite);
                            break;
                        }
                    }
                }
            }
            if (!coFound)
            {
                pAnim = pObjectManager->getResAnim("no_co");
                pSprite = new oxygine::Sprite();
                pSprite->setResAnim(pAnim);
                actor->addChild(pSprite);
                coid = "";

            }
            m_CoIDs.append(coid);
            pAnim = pObjectManager->getResAnim("co_background");
            pSprite = new oxygine::Sprite();
            pSprite->setResAnim(pAnim);
            actor->addChild(pSprite);

            actor->setPosition(5 + x * 51, 7 + y * 51 + m_BackgroundMask->getY());
            actor->addEventListener(oxygine::TouchEvent::OVER, [ = ](oxygine::Event*)
            {
                m_Cursor->setPosition(5 + x * 51, 7 + y * 51 + m_BackgroundMask->getY());
                m_CurrentCO = coid;
                emit sigHoveredCOChanged(m_CurrentCO);
            });
            connect(this, &COSelection::sigHoveredCOChanged, this, &COSelection::hoveredCOChanged, Qt::QueuedConnection);
            actor->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
            {
                emit coSelected(m_CurrentCO);
            });
            m_COFields.append(actor);
            addChild(actor);
        }
    }
    colorChanged(m_CurrentColor);
    pApp->continueThread();
}

void COSelection::colorChanged(QColor color)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    oxygine::Color oxColor(color.red(), color.green(), color.blue(), 255);
    oxygine::Color oxColorAlpha(color.red(), color.green(), color.blue(), 120);
    m_BackgroundMask->setColor(oxColor);
    for (qint32 i = 0; i < m_COFields.size(); i++)
    {
        oxygine::Sprite* pSprite = dynamic_cast<oxygine::Sprite*>(m_COFields[i]->getLastChild().get());
        pSprite->setColor(oxColorAlpha);
    }
    m_Cursor->setColor(oxColor);
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

        Mainapp* pApp = Mainapp::getInstance();
        Interpreter* pInterpreter = pApp->getInterpreter();
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
        m_COName->setHtmlText(coName.toStdString().c_str());
        m_COName->setX(165 + m_COName->getScaledWidth() / 2 - m_COName->getTextRect().getWidth() / 2);

        m_COBio->setHtmlText(coBio.toStdString().c_str());
        m_COBio->setHeight(m_COBio->getTextRect().getHeight() + 20);
        m_COBioRect->setContent(m_COBio);
        m_COBioRect->snap();

        m_CODesc->setHtmlText(coDesc.toStdString().c_str());
        m_CODesc->setHeight(m_CODesc->getTextRect().getHeight() + 20);
        m_CODescRect->setContent(m_CODesc);
        m_CODescRect->snap();

        m_COPower->setHtmlText(coPower.toStdString().c_str());
        m_COSuperpower->setHtmlText(coSuperpower.toStdString().c_str());
    }
    pApp->continueThread();
}
