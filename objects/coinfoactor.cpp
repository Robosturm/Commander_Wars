#include "coinfoactor.h"

#include "resource_management/cospritemanager.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "resource_management/cospritemanager.h"

#include "resource_management/unitspritemanager.h"

#include "resource_management/gamemanager.h"

#include "game/gamemap.h"

COInfoActor::COInfoActor(qint32 width)
{
    Mainapp* pApp = Mainapp::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::spTextField pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("CO Information"));
    pLabel->setScale(2.0f);
    pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth(), 10);
    addChild(pLabel);
    m_pCurrentCO = new oxygine::Sprite();
    m_pCurrentCO->setScale((pApp->getSettings()->getHeight() - 200) / 352.0f);
    m_pCurrentCO->setSize(208, 352);
    m_pCurrentCO->setPosition(pApp->getSettings()->getWidth() - 120 - m_pCurrentCO->getScaledWidth(), 90);
    addChild(m_pCurrentCO);

    m_COName = new oxygine::TextField();
    m_COName->setStyle(style);
    m_COName->setScale(2.0f);
    addChild(m_COName);

    style.multiline = true;
    m_COBio = new oxygine::TextField();
    m_COBio->setStyle(style);
    m_COBio->setWidth(m_pCurrentCO->getX() - 50);
    m_COBio->setPosition(10, m_COBio->getY() + 120);
    addChild(m_COBio);

    m_HitSprite = new oxygine::Box9Sprite();
    m_HitSprite->setResAnim(pCOSpriteManager->getResAnim("hit"));
    m_HitSprite->setSize(100, 16);
    m_HitSprite->setScale(2.5f);
    m_HitSprite->setPosition(10, 200);
    oxygine::spTextField pTextField = new oxygine::TextField();
    pTextField->setPosition(14, 1);
    pTextField->setStyle(style);
    pTextField->setScale(1 / m_HitSprite->getScaleX());
    pTextField->setHtmlText(tr("Hit"));
    m_HitSprite->setWidth(pTextField->getTextRect().getWidth() / m_HitSprite->getScaleX() + 21);
    m_HitSprite->addChild(pTextField);
    addChild(m_HitSprite);
    m_HitText = new oxygine::TextField();
    m_HitText->setStyle(style);
    m_HitText->setX(m_HitSprite->getX() + m_HitSprite->getScaledWidth() +10);
    addChild(m_HitText);

    m_MissSprite = new oxygine::Box9Sprite();
    m_MissSprite->setResAnim(pCOSpriteManager->getResAnim("miss"));
    m_MissSprite->setSize(100, 16);
    m_MissSprite->setScale(2.5f);
    m_MissSprite->setPosition(10, 300);
    pTextField = new oxygine::TextField();
    pTextField->setPosition(14, 1);
    pTextField->setStyle(style);
    pTextField->setScale(1 / m_MissSprite->getScaleX());
    pTextField->setHtmlText(tr("Miss"));
    m_MissSprite->setWidth(pTextField->getTextRect().getWidth() / m_MissSprite->getScaleX()  + 21);
    m_MissSprite->addChild(pTextField);
    addChild(m_MissSprite);
    m_MissText = new oxygine::TextField();
    m_MissText->setStyle(style);
    m_MissText->setX(m_MissSprite->getX() + m_MissSprite->getScaledWidth() + 10);
    addChild(m_MissText);

    m_InfoSprite = new oxygine::Box9Sprite();
    m_InfoSprite->setResAnim(pCOSpriteManager->getResAnim("skill"));
    m_InfoSprite->setSize(100, 16);
    m_InfoSprite->setScale(2.5f);
    m_InfoSprite->setPosition(10, 400);
    pTextField = new oxygine::TextField();
    pTextField->setPosition(3, 2);
    pTextField->setStyle(style);
    pTextField->setScale(1 / m_InfoSprite->getScaleX());
    pTextField->setHtmlText(tr("Info"));
    m_InfoSprite->setWidth(pTextField->getTextRect().getWidth() / m_InfoSprite->getScaleX() + 12);
    m_InfoSprite->addChild(pTextField);
    m_InfoSprite->setX((pApp->getSettings()->getWidth() - m_pCurrentCO->getScaledWidth()) / 2 - m_InfoSprite->getScaledWidth() / 2);
    addChild(m_InfoSprite);
    m_InfoText = new oxygine::TextField();
    m_InfoText->setStyle(style);
    m_InfoText->setWidth(m_pCurrentCO->getX() - 50);
    addChild(m_InfoText);

    m_PowerSprite = new oxygine::Sprite();
    m_PowerSprite->setResAnim(pCOSpriteManager->getResAnim("power"));
    m_PowerSprite->setScale(2.5f);
    m_PowerSprite->setPosition(10, 400);
    addChild(m_PowerSprite);
    m_Powername = new oxygine::TextField();
    m_Powername->setStyle(style);
    m_Powername->setX(m_PowerSprite->getX() + m_PowerSprite->getScaledWidth() + 10);
    addChild(m_Powername);
    m_PowerDesc = new oxygine::TextField();
    m_PowerDesc->setStyle(style);
    m_PowerDesc->setWidth(m_pCurrentCO->getX() - 50);
    m_PowerDesc->setX(10);
    addChild(m_PowerDesc);

    m_SuperPowerSprite = new oxygine::Sprite();
    m_SuperPowerSprite->setResAnim(pCOSpriteManager->getResAnim("superpower"));
    m_SuperPowerSprite->setScale(2.5f);
    m_SuperPowerSprite->setPosition(10, 400);
    addChild(m_SuperPowerSprite);
    m_SuperPowername = new oxygine::TextField();
    m_SuperPowername->setStyle(style);
    m_SuperPowername->setX(m_SuperPowerSprite->getX() + m_SuperPowerSprite->getScaledWidth() + 10);
    addChild(m_SuperPowername);
    m_SuperPowerDesc = new oxygine::TextField();
    m_SuperPowerDesc->setStyle(style);
    m_SuperPowerDesc->setWidth(m_pCurrentCO->getX() - 50);
    m_SuperPowerDesc->setX(10);
    addChild(m_SuperPowerDesc);
    m_SynergyText = new oxygine::TextField();
    m_SynergyText->setStyle(style);
    m_SynergyText->setHtmlText(tr("Synergy"));
    m_SynergyText->setX(10);
    addChild(m_SynergyText);

}

void COInfoActor::showCO(spCO pCO, spPlayer pPlayer)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = nullptr;
    QString coid = "";
    if (pCO.get() != nullptr)
    {
        coid = pCO->getCoID();
    }
    if (!coid.isEmpty())
    {
        pAnim = pCOSpriteManager->getResAnim((coid + "+nrm"));
    }
    m_pCurrentCO->setResAnim(pAnim);

    QString coName = "";
    QString coBio = "";
    QString coHits = "";
    QString coMiss = "";
    QString coDesc = "";
    QString coPower = "";
    QString coPowerDesc = "";
    QString coSuperpower = "";
    QString coSuperpowerDesc = "";
    qint32 corange = 0;
    if (pCO.get() != nullptr)
    {
        corange = pCO->getCORange();
    }

    Interpreter* pInterpreter = pApp->getInterpreter();
    QJSValue value;
    if (pCO.get() != nullptr)
    {
        QJSValue value = pInterpreter->doFunction(coid, "getName");
        if (value.isString())
        {
            coName = value.toString();
        }
    }
    m_COName->setHtmlText(coName);
    m_COName->setPosition((pApp->getSettings()->getWidth() - m_pCurrentCO->getScaledWidth()) / 2 - m_COName->getTextRect().getWidth(), 60);
    if (pCO.get() != nullptr)
    {
        value = pInterpreter->doFunction(coid, "getBio");
        if (value.isString())
        {
            coBio = value.toString();
        }
        value = pInterpreter->doFunction(coid, "getLongBio");
        if (value.isString())
        {
            coBio += " " + value.toString();
        }
    }
    m_COBio->setHtmlText(coBio);

    if (pCO.get() != nullptr)
    {
        value = pInterpreter->doFunction(coid, "getHits");
        if (value.isString())
        {
            coHits = value.toString();
        }
    }
    m_HitSprite->setY(m_COBio->getY() + m_COBio->getTextRect().getHeight() + 10);
    m_HitText->setHtmlText(coHits);
    m_HitText->setY(m_HitSprite->getY());

    if (pCO.get() != nullptr)
    {
        value = pInterpreter->doFunction(coid, "getMiss");
        if (value.isString())
        {
            coMiss = value.toString();
        }
    }
    m_MissSprite->setY(m_HitSprite->getY() + 50);
    m_MissText->setHtmlText(coMiss);
    m_MissText->setY(m_MissSprite->getY());

    if (pCO.get() != nullptr)
    {
        value = pInterpreter->doFunction(coid, "getCODescription");
        if (value.isString())
        {
            coDesc = value.toString();
        }
        value = pInterpreter->doFunction(coid, "getLongCODescription");
        if (value.isString())
        {
            coDesc += " " + value.toString();
        }
    }
    m_InfoSprite->setY(m_MissSprite->getY() + 50);
    m_InfoText->setHtmlText((coDesc + "\n\n" + tr("CO-Zone-Range: ") + QString::number(corange)));
    m_InfoText->setY(m_InfoSprite->getY() + 50);

    if (pCO.get() != nullptr)
    {
        value = pInterpreter->doFunction(coid, "getPowerDescription");
        if (value.isString())
        {
            coPowerDesc = value.toString();
        }
    }
    if (pCO.get() != nullptr)
    {
        value = pInterpreter->doFunction(coid, "getPowerName");
        if (value.isString())
        {
            coPower = value.toString();
        }
    }
    m_PowerSprite->setY(m_InfoText->getY() + m_InfoText->getTextRect().getHeight() + 20);
    m_Powername->setY(m_PowerSprite->getY());
    m_PowerDesc->setY(m_Powername->getY() + 40);
    m_Powername->setHtmlText(coPower);
    m_PowerDesc->setHtmlText(coPowerDesc);

    if (pCO.get() != nullptr)
    {
        value = pInterpreter->doFunction(coid, "getSuperPowerDescription");
        if (value.isString())
        {
            coSuperpowerDesc = value.toString();
        }
    }
    if (pCO.get() != nullptr)
    {
        value = pInterpreter->doFunction(coid, "getSuperPowerName");
        if (value.isString())
        {
            coSuperpower = value.toString();
        }
    }
    m_SuperPowerSprite->setY(m_PowerDesc->getY() + m_PowerDesc->getTextRect().getHeight() + 20);
    m_SuperPowername->setY(m_SuperPowerSprite->getY());
    m_SuperPowerDesc->setY(m_SuperPowername->getY() + 40);
    m_SuperPowername->setHtmlText(coSuperpower);
    m_SuperPowerDesc->setHtmlText(coSuperpowerDesc);

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 y = m_SuperPowerDesc->getY() + m_SuperPowerDesc->getTextRect().getHeight() + 20;
    if (pCO.get() != nullptr)
    {
        m_SynergyText->setY(y);
        y += 40;

        // show co synergys
        for (qint32 i = 0; i < m_SynergyCONames.size(); i++)
        {
            m_SynergyCONames[i]->detach();
            m_SynergyStarActors[i]->detach();
        }
        m_SynergyCONames.clear();
        m_SynergyStarActors.clear();
        GameManager* pGameManager = GameManager::getInstance();
        pAnim = pGameManager->getResAnim("tagaffinitystar");
        for (qint32 i = 0; i < pCOSpriteManager->getCOCount(); i++)
        {
            QString coid = pCOSpriteManager->getCOID(i);
            spCO testCO = new CO(coid, nullptr);

            QJSValueList args;
            QJSValue obj1 = pApp->getInterpreter()->newQObject(pCO.get());
            QJSValue obj2 = pApp->getInterpreter()->newQObject(testCO.get());
            args << obj1;
            args << obj2;
            value = pInterpreter->doFunction("TAGPOWER", "getTagstars", args);
            if (value.isNumber())
            {
                qint32 synergy = value.toInt();
                if (synergy > 0)
                {
                    QString coSynergyName = "";
                    value = pInterpreter->doFunction(coid, "getName");
                    if (value.isString())
                    {
                        coSynergyName = value.toString();
                    }
                    oxygine::spTextField pText = new oxygine::TextField();
                    pText->setStyle(style);
                    pText->setHtmlText(coSynergyName);
                    pText->setX(10);
                    pText->setY(y);
                    addChild(pText);
                    m_SynergyCONames.append(pText);
                    oxygine::spActor pActor = new oxygine::Actor();
                    pActor->setX(120);
                    pActor->setY(y);
                    for (qint32 i2 = 0; i2 < synergy; i2++)
                    {
                        oxygine::spSprite pSprite = new oxygine::Sprite();
                        pSprite->setResAnim(pAnim);
                        pSprite->setScale(1.5f);
                        pSprite->setX(i2 * pSprite->getScaledWidth() + 3);
                        pActor->addChild(pSprite);
                    }
                    addChild(pActor);
                    m_SynergyStarActors.append(pActor);
                    y += 40;
                }
            }
        }
    }


    for (qint32 i = 0; i < m_UnitDataActors.size(); i++)
    {
        m_UnitDataActors[i]->detach();
    }
    m_UnitDataActors.clear();


    qint32 x = 10;
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    for (qint32 i = 0; i < pUnitSpriteManager->getUnitCount(); i++)
    {
        QString unitID = pUnitSpriteManager->getUnitID(i);
        m_UnitDataActors.append(new oxygine::Actor());
        m_UnitDataActors[i]->setPosition(x, y);
        addChild(m_UnitDataActors[i]);
        spUnit pUnit = new Unit(unitID, pPlayer.get(), false);
        // gather basic co information
        qint32 offBonus = 0;
        qint32 defBonus = 0;
        qint32 firerangeBonus = 0;
        qint32 movementBonus = 0;
        // not sure which one is better for ingame data
        //        if (m_Ingame)
        //        {
        //            offBonus = pUnit->getBonusOffensive(QPoint(-1, -1), nullptr, QPoint(-1, -1), false);
        //            defBonus = pUnit->getBonusDefensive(QPoint(-1, -1), nullptr, QPoint(-1, -1), false);
        //            firerangeBonus = pUnit->getBonusMaxRange(QPoint(-1, -1));
        //            movementBonus = pUnit->getBonusMovementpoints(QPoint(-1, -1));
        //        }
        //        else
        if (pCO.get() != nullptr)
        {
            offBonus = pCO->getOffensiveBonus(pUnit.get(), QPoint(-1, -1), nullptr, QPoint(-1, -1), false);
            defBonus = pCO->getDeffensiveBonus(nullptr, QPoint(-1, -1), pUnit.get(), QPoint(-1, -1), false);
            firerangeBonus = pCO->getFirerangeModifier(pUnit.get(), QPoint(-1, -1));
            movementBonus = pCO->getMovementpointModifier(pUnit.get(), QPoint(-1, -1));
        }

        createStrengthBar(m_UnitDataActors[i], offBonus, 0);
        createStrengthBar(m_UnitDataActors[i], defBonus, GameMap::Imagesize / 2);

        if (firerangeBonus != 0)
        {
            oxygine::spSprite pSprite = new oxygine::Sprite();
            pSprite->setResAnim(pCOSpriteManager->getResAnim("atkRange"));
            pSprite->setY(5 +  GameMap::Imagesize);
            pSprite->setScale(2.0f);
            m_UnitDataActors[i]->addChild(pSprite);
            oxygine::spTextField pText = new oxygine::TextField();
            pText->setStyle(style);
            if (firerangeBonus > 0)
            {
                pText->setHtmlText(("+" + QString::number(firerangeBonus)));
            }
            else
            {
                pText->setHtmlText((QString::number(firerangeBonus)));
            }
            pText->setPosition(pSprite->getX() + pSprite->getScaledWidth() + 2, pSprite->getY() - 2);
            pText->setScale(0.75f);
            m_UnitDataActors[i]->addChild(pText);
        }
        if (movementBonus != 0)
        {
            oxygine::spSprite pSprite = new oxygine::Sprite();
            pSprite->setResAnim(pCOSpriteManager->getResAnim("moveRange"));
            pSprite->setPosition(25 +  GameMap::Imagesize, 5 +  GameMap::Imagesize);
            pSprite->setScale(2.0f);
            m_UnitDataActors[i]->addChild(pSprite);
            oxygine::spTextField pText = new oxygine::TextField();
            pText->setStyle(style);
            if (movementBonus > 0)
            {
                pText->setHtmlText(("+" + QString::number(movementBonus)));
            }
            else
            {
                pText->setHtmlText((QString::number(movementBonus)));
            }

            pText->setPosition(pSprite->getX() + pSprite->getScaledWidth() + 2, pSprite->getY() - 2);
            pText->setScale(0.75f);
            m_UnitDataActors[i]->addChild(pText);
        }

        m_UnitDataActors[i]->addChild(pUnit);
        if (x + 120 > m_pCurrentCO->getX() - 50)
        {
            x = 10;
            y += 60;
        }
        else
        {
            x += 100;
        }
    }
    setHeight(y + 100);
    pApp->continueThread();
}

void COInfoActor::createStrengthBar(oxygine::spActor pActor, qint32 bonus, qint32 y)
{
    oxygine::ResAnim* pStartAnim;
    oxygine::ResAnim* pEndAnim;
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    qint32 width = 50;
    float divider = 0;
    if (bonus == 0)
    {
        pStartAnim = pCOSpriteManager->getResAnim("normalPowerStart");
        pEndAnim = pCOSpriteManager->getResAnim("normalPowerEnd");
        divider = 1;
    }
    else if (bonus < 0)
    {
        pStartAnim = pCOSpriteManager->getResAnim("lessPowerStart");
        pEndAnim = pCOSpriteManager->getResAnim("lessPowerEnd");
        divider = (100.0f + bonus) / 100.0f;
    }
    else if (bonus > 100)
    {
        while (bonus > 100)
        {
            bonus -= 100;
        }
        pStartAnim = pCOSpriteManager->getResAnim("superStrongPowerStart");
        pEndAnim = pCOSpriteManager->getResAnim("strongPowerEnd");
        divider = bonus / 100.0f;
    }
    else
    {
        pStartAnim = pCOSpriteManager->getResAnim("strongPowerStart");
        pEndAnim = pCOSpriteManager->getResAnim("normalPowerEnd");
        divider = bonus / 100.0f;
    }
    oxygine::spBox9Sprite pStartBox = new oxygine::Box9Sprite();
    pStartBox->setResAnim(pStartAnim);
    pStartBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pStartBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    pStartBox->setSize(width * divider + 1, 8);
    pStartBox->setPosition(5 +  GameMap::Imagesize, y);
    pActor->addChild(pStartBox);

    oxygine::spBox9Sprite pEndBox = new oxygine::Box9Sprite();
    pEndBox->setResAnim(pEndAnim);
    pEndBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pEndBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    pEndBox->setSize(width * (1 - divider) + 2, 8);
    pEndBox->setPosition(5 +  GameMap::Imagesize + pStartBox->getWidth(), y);
    pActor->addChild(pEndBox);
}

