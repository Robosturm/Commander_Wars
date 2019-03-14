#include "coselection.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"
#include "resource_management/cospritemanager.h"

COSelection::COSelection()
    : QObject()
{
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

    connect(this, SIGNAL(armySelectedChange(QString)), this, SLOT(armyChanged(QString)));
    armyChanged(m_Armies[0]);


}

void COSelection::armyBannerClicked(QString army, qint32 index)
{
    for (qint32 i2 = 0; i2 < m_ArmyBanners.size(); i2++)
    {
        m_ArmyBanners[i2]->setY(-12);
    }
    m_ArmyBanners[index]->setY(0);
    emit armySelectedChange(army);
}

COSelection::~COSelection()
{

}

void COSelection::armyChanged(QString army)
{
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
            });
            actor->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
            {
                emit coSelected(m_CurrentCO);
            });
            m_COFields.append(actor);
            addChild(actor);
        }
    }
    colorChanged(m_CurrentColor);

}

void COSelection::colorChanged(QColor color)
{
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
}
