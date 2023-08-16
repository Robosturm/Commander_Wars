#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/coinfoactor.h"

#include "resource_management/cospritemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/coperkmanager.h"

#include "game/gamemap.h"

#include "objects/base/label.h"

#include "wiki/wikidatabase.h"

COInfoActor::COInfoActor(GameMap* pMap, qint32 width)
    : m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("COInfoActor");
#endif
    Interpreter::setCppOwnerShip(this);
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::TextStyle headerStyle = oxygine::TextStyle(FontManager::getMainFont48());
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;

    oxygine::spTextField pLabel = MemoryManagement::create<oxygine::TextField>();
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText(tr("CO Information"));
    pLabel->setPosition(width / 2 - pLabel->getTextRect().width() / 2, 10);
    addChild(pLabel);

    m_pCurrentCoFaction = MemoryManagement::create<oxygine::Sprite>();
    m_pCurrentCoFaction->setPosition(width * 0.5f - 36 * 0.5f, pLabel->getY() + pLabel->getTextRect().height() + 5);
    m_pCurrentCoFaction->setScale(2.0f);
    addChild(m_pCurrentCoFaction);

    m_COName = MemoryManagement::create<oxygine::TextField>();
    m_COName->setWidth(width);
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    m_COName->setStyle(headerStyle);
    m_COName->setY(m_pCurrentCoFaction->getY() + 40);
    m_COName->setText(tr("Unknown"));
    addChild(m_COName);

    m_pCurrentCO = MemoryManagement::create<oxygine::Sprite>();
    m_pCurrentCO->setScale(static_cast<float>(oxygine::Stage::getStage()->getHeight() - 200) / 352.0f);
    m_pCurrentCO->setSize(208, 352);
    m_pCurrentCO->setPosition(oxygine::Stage::getStage()->getWidth() - 120 - m_pCurrentCO->getScaledWidth(), m_COName->getY() + m_COName->getTextRect().height() + 10);
    addChild(m_pCurrentCO);


    style.multiline = true;
    m_COBio = MemoryManagement::create<oxygine::TextField>();
    m_COBio->setStyle(style);
    m_COBio->setWidth(m_pCurrentCO->getX() - 50);
    m_COBio->setPosition(10, m_COName->getY() + m_COName->getTextRect().height() + 10);
    addChild(m_COBio);

    m_HitSprite = MemoryManagement::create<oxygine::Box9Sprite>();
    m_HitSprite->setResAnim(pCOSpriteManager->getResAnim("hit"));
    m_HitSprite->setSize(100, 16);
    m_HitSprite->setScale(2.5f);
    m_HitSprite->setPosition(10, 200);
    auto blackStyle = style;
    blackStyle.color = Qt::black;
    oxygine::spTextField pTextField = MemoryManagement::create<oxygine::TextField>();
    pTextField->setPosition(14, 1);
    pTextField->setStyle(blackStyle);
    pTextField->setHtmlText(tr("Hit"));
    if (m_HitSprite->getScaleX() > 0)
    {
        pTextField->setScale(1.0f / m_HitSprite->getScaleX());
        m_HitSprite->setWidth(pTextField->getTextRect().width() / m_HitSprite->getScaleX() + 21);
    }
    m_HitSprite->addChild(pTextField);
    addChild(m_HitSprite);
    m_HitText = MemoryManagement::create<oxygine::TextField>();
    m_HitText->setStyle(style);
    m_HitText->setX(m_HitSprite->getX() + m_HitSprite->getScaledWidth() + 10);
    addChild(m_HitText);

    m_MissSprite = MemoryManagement::create<oxygine::Box9Sprite>();
    m_MissSprite->setResAnim(pCOSpriteManager->getResAnim("miss"));
    m_MissSprite->setSize(100, 16);
    m_MissSprite->setScale(2.5f);
    m_MissSprite->setPosition(10, 300);
    pTextField = MemoryManagement::create<oxygine::TextField>();
    pTextField->setPosition(14, 1);
    pTextField->setStyle(blackStyle);
    pTextField->setHtmlText(tr("Miss"));
    if (m_MissSprite->getScaleX() > 0)
    {
        pTextField->setScale(1.0f / m_MissSprite->getScaleX());
        m_MissSprite->setWidth(pTextField->getTextRect().width() / m_MissSprite->getScaleX()  + 21);
    }
    m_MissSprite->addChild(pTextField);
    addChild(m_MissSprite);
    m_MissText = MemoryManagement::create<oxygine::TextField>();
    m_MissText->setStyle(style);
    m_MissText->setX(m_MissSprite->getX() + m_MissSprite->getScaledWidth() + 10);
    addChild(m_MissText);

    m_InfoSprite = MemoryManagement::create<oxygine::Sprite>();
    m_InfoSprite->setResAnim(pCOSpriteManager->getResAnim("skill"));
    m_InfoSprite->setScale(2.5f);
    m_InfoSprite->setPosition(14, 400);
    addChild(m_InfoSprite);
    m_InfoText = MemoryManagement::create<oxygine::TextField>();
    m_InfoText->setStyle(style);
    m_InfoText->setWidth(m_pCurrentCO->getX() - 50);
    m_InfoText->setX(10);
    addChild(m_InfoText);

    m_pCoPowermeter = MemoryManagement::create<CoPowermeter>(m_pMap, nullptr);
    addChild(m_pCoPowermeter);

    m_PowerSprite = MemoryManagement::create<oxygine::Sprite>();
    m_PowerSprite->setResAnim(pCOSpriteManager->getResAnim("power"));
    m_PowerSprite->setScale(2.5f);
    m_PowerSprite->setPosition(10, 400);
    addChild(m_PowerSprite);
    m_Powername = MemoryManagement::create<oxygine::TextField>();
    m_Powername->setStyle(style);
    m_Powername->setX(m_PowerSprite->getX() + m_PowerSprite->getScaledWidth() + 10);
    addChild(m_Powername);
    m_PowerDesc = MemoryManagement::create<oxygine::TextField>();
    m_PowerDesc->setStyle(style);
    m_PowerDesc->setWidth(m_pCurrentCO->getX() - 50);
    m_PowerDesc->setX(10);
    addChild(m_PowerDesc);

    m_SuperPowerSprite = MemoryManagement::create<oxygine::Sprite>();
    m_SuperPowerSprite->setResAnim(pCOSpriteManager->getResAnim("superpower"));
    m_SuperPowerSprite->setScale(2.5f);
    m_SuperPowerSprite->setPosition(10, 400);
    addChild(m_SuperPowerSprite);
    m_SuperPowername = MemoryManagement::create<oxygine::TextField>();
    m_SuperPowername->setStyle(style);
    m_SuperPowername->setX(m_SuperPowerSprite->getX() + m_SuperPowerSprite->getScaledWidth() + 10);
    addChild(m_SuperPowername);
    m_SuperPowerDesc = MemoryManagement::create<oxygine::TextField>();
    m_SuperPowerDesc->setStyle(style);
    m_SuperPowerDesc->setWidth(m_pCurrentCO->getX() - 50);
    m_SuperPowerDesc->setX(10);
    addChild(m_SuperPowerDesc);

    m_synergySprite = MemoryManagement::create<oxygine::Sprite>();
    m_synergySprite->setResAnim(pCOSpriteManager->getResAnim("tagCos"));
    m_synergySprite->setScale(2.0f);
    m_synergySprite->setPosition(10, 400);
    addChild(m_synergySprite);
    m_SynergyText = MemoryManagement::create<oxygine::TextField>();
    m_SynergyText->setStyle(style);
    m_SynergyText->setX(m_synergySprite->getX() + m_synergySprite->getScaledWidth() + 10);
    m_SynergyText->setHtmlText(tr("Synergy"));
    addChild(m_SynergyText);

    m_PerkText = MemoryManagement::create<oxygine::TextField>();
    m_PerkText->setStyle(style);
    m_PerkText->setHtmlText(tr("Active Perks"));
    m_PerkText->setX(10);
    addChild(m_PerkText);

    m_powerProgress = MemoryManagement::create<oxygine::TextField>();
    m_powerProgress->setStyle(style);
    m_powerProgress->setX(10);
    addChild(m_powerProgress);

    m_GlobalBoosts = MemoryManagement::create<oxygine::TextField>();
    m_GlobalBoosts->setStyle(style);
    m_GlobalBoosts->setHtmlText(tr("Global Boosts"));
    m_GlobalBoosts->setX(10);
    addChild(m_GlobalBoosts);

    m_CoBoost = MemoryManagement::create<oxygine::TextField>();
    m_CoBoost->setStyle(style);
    m_CoBoost->setHtmlText(tr("CO Zone Boosts"));
    m_CoBoost->setX(10);
    addChild(m_CoBoost);
}

void COInfoActor::showCO(spCO pCO, spPlayer pPlayer)
{
    GameManager* pGameManager = GameManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = nullptr;
    QString coid = "";
    m_pMap = nullptr;
    if (pCO.get() != nullptr)
    {
        m_pMap = pCO->getMap();
        coid = pCO->getCoID();
        pAnim = pCO->getResAnim((coid + "+nrm"));
    }
    else if (!coid.isEmpty())
    {
        pAnim = pCOSpriteManager->getResAnim((coid + "+nrm"));        
    }
    m_pCoPowermeter->setMap(m_pMap);
    m_pCurrentCO->setResAnim(pAnim);

    QString coName = tr("Unknown");
    QString coBio = tr("Unknown");
    QString coHits = tr("Unknown");
    QString coMiss = tr("Unknown");
    QString coDesc = tr("Unknown");
    QString coPower = tr("Unknown");
    QString coPowerDesc = tr("Unknown");
    QString coSuperpower = tr("Unknown");
    QString coSuperpowerDesc = tr("Unknown");
    QString powerProgress = "-/-";
    qint32 corange = 0;
    if (pCO.get() != nullptr)
    {
        corange = pCO->getCORange();
    }

    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue value;
    if (pCO.get() != nullptr)
    {
        QJSValue value = pInterpreter->doFunction(coid, "getName");
        if (value.isString())
        {
            coName = value.toString();
        }
        auto* pResAnim = pGameManager->getResAnim("icon_" + pCO->getCOArmy().toLower(), oxygine::ep_ignore_error);
        m_pCurrentCoFaction->setResAnim(pResAnim);
        if (pResAnim != nullptr && pResAnim->getWidth() > 0)
        {
            m_pCurrentCoFaction->setScale(36.0f / static_cast<float>(pResAnim->getWidth()));
        }
    }
    m_COName->setHtmlText(coName);
    if (pCO.get() != nullptr)
    {
        coBio = pCO->getBio();
        coBio += pCO->getLongBio();
        coHits = pCO->getHits();
        coMiss = pCO->getMiss();
        coDesc = pCO->getCODescription();
        coDesc += "\n" + pCO->getLongCODescription();
        coPowerDesc = pCO->getPowerDescription();
        coPower = pCO->getPowerName();
        coSuperpowerDesc = pCO->getSuperPowerDescription();
        coSuperpower = pCO->getSuperPowerName();
    }
    m_COBio->setHtmlText(coBio);
    m_COBio->setHeight(m_COBio->getTextRect().height());
    m_HitSprite->setY(m_COBio->getY() + m_COBio->getTextRect().height() + 10);
    m_HitText->setHtmlText(coHits);
    m_HitText->setY(m_HitSprite->getY() + 5);
    m_MissSprite->setY(m_HitSprite->getY() + 50);
    m_MissText->setHtmlText(coMiss);
    m_MissText->setY(m_MissSprite->getY() + 5);
    m_InfoSprite->setY(m_MissSprite->getY() + 50);
    m_InfoText->setHtmlText((coDesc + "\n\n" + tr("CO-Zone-Range: ") + QString::number(corange)));
    m_InfoText->setHeight(m_InfoText->getTextRect().height());
    m_InfoText->setY(m_InfoSprite->getY() + 50);

    m_pCoPowermeter->setCO(pCO.get());
    m_pCoPowermeter->drawPowerMeter();
    m_pCoPowermeter->setY(m_InfoText->getY() + m_InfoText->getTextRect().height() + 20);
    m_pCoPowermeter->setX(20);

    if (pCO.get() != nullptr)
    {
        float starCost = pCO->getStarCost();
        float powerStars = pCO->getPowerFilled();
        powerProgress = QString::number(static_cast<qint32>(powerStars * starCost));
        powerProgress += " / ";
        qint32 powerCost = pCO->getPowerStars();
        if (powerStars > powerCost)
        {
            powerProgress += QString::number(static_cast<qint32>((powerCost + pCO->getSuperpowerStars()) * starCost));
        }
        else
        {
            powerProgress += QString::number(static_cast<qint32>(powerCost * starCost));
        }
    }
    m_powerProgress->setText(tr("Progress: ") + powerProgress);
    m_powerProgress->setY(m_pCoPowermeter->getY() + 40);

    m_PowerSprite->setY(m_powerProgress->getY() + 40);
    m_Powername->setY(m_PowerSprite->getY());
    m_PowerDesc->setY(m_Powername->getY() + 40);
    m_Powername->setHtmlText(coPower);
    m_PowerDesc->setHtmlText(coPowerDesc);
    m_PowerDesc->setHeight(m_PowerDesc->getTextRect().height());

    m_SuperPowerSprite->setY(m_PowerDesc->getY() + m_PowerDesc->getTextRect().height() + 20);
    m_SuperPowername->setY(m_SuperPowerSprite->getY());
    m_SuperPowerDesc->setY(m_SuperPowername->getY() + 40);
    m_SuperPowername->setHtmlText(coSuperpower);
    m_SuperPowerDesc->setHtmlText(coSuperpowerDesc);
    m_SuperPowerDesc->setHeight(m_SuperPowerDesc->getTextRect().height());

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 y = m_SuperPowerDesc->getY() + m_SuperPowerDesc->getTextRect().height() + 20;
    if (pCO.get() != nullptr)
    {
        m_SynergyText->setY(y);
        m_synergySprite->setY(y);
        y += m_SynergyText->getTextRect().height() + 10;

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
        for (qint32 i = 0; i < pCOSpriteManager->getCount(); i++)
        {
            QString coid = pCOSpriteManager->getID(i);
            spCO testCO = MemoryManagement::create<CO>(coid, pPlayer.get(), pPlayer->getMap());

            QJSValueList args({pInterpreter->newQObject(pCO.get()),
                               pInterpreter->newQObject(testCO.get())});
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
                    oxygine::spTextField pText = MemoryManagement::create<oxygine::TextField>();
                    pText->setStyle(style);
                    pText->setHtmlText(coSynergyName);
                    pText->setX(10);
                    pText->setY(y);
                    addChild(pText);
                    m_SynergyCONames.append(pText);
                    oxygine::spActor pActor = MemoryManagement::create<oxygine::Actor>();
                    pActor->setX(220);
                    pActor->setY(y);
                    for (qint32 i2 = 0; i2 < synergy; i2++)
                    {
                        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
                        pSprite->setResAnim(pAnim);
                        pSprite->setScale(1.5f);
                        pSprite->setX(i2 * pSprite->getScaledWidth() + 3);
                        pActor->addChild(pSprite);
                    }
                    addChild(pActor);
                    m_SynergyStarActors.append(pActor);
                    y += pText->getTextRect().height() + 10;
                }
            }
        }
    }
    else
    {
        m_synergySprite->setY(y);
        m_SynergyText->setY(y);
        y += m_SynergyText->getTextRect().height() + 10;
    }

    m_PerkText->setPosition(10, y);
    y += m_PerkText->getTextRect().height() + 10;
    showPerks(pCO, y);
    y += 50;

    for (qint32 i = 0; i < m_UnitDataActors.size(); i++)
    {
        m_UnitDataActors[i]->detach();
    }
    m_UnitDataActors.clear();

    if (y < m_pCurrentCO->getY() + m_pCurrentCO->getHeight() + 10)
    {
        y = m_pCurrentCO->getY() + m_pCurrentCO->getHeight() + 10;
    }

    qint32 x = 10;
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    QStringList sortedUnits = pUnitSpriteManager->getUnitsSorted();
    m_GlobalBoosts->setPosition(10, y);
    constexpr qint32 textAdvance = 25;
    y += GameMap::getImageSize() * 2 + textAdvance;
    if (pCO.get() != nullptr)
    {
        qint32 customCount = pCO->getCustomUnitGlobalBoostCount();
        if (customCount > 0)
        {
            for (qint32 i = 0; i < customCount; i++)
            {
                showCustomCOBoost(pCO, x, y, i, true);
            }
            x = 10;
            y += GameMap::getImageSize() * 2 + textAdvance;
        }
        if (pCO->showDefaultUnitGlobalBoost())
        {
            for (qint32 i = 0; i < sortedUnits.size(); i++)
            {
                QString unitID = sortedUnits[i];
                spUnit pUnit = MemoryManagement::create<Unit>(unitID, pPlayer.get(), false, pPlayer->getMap());
                pUnit->setVirtuellX(-2);
                pUnit->setVirtuellY(-2);
                showCOBoost(pUnit, pCO, x, y);
            }
            x = 10;
            y += GameMap::getImageSize() * 2 + textAdvance;
        }
        m_CoBoost->setPosition(10, y);
        y += GameMap::getImageSize() * 2 + textAdvance;
        customCount = pCO->getCustomUnitZoneBoostCount();
        if (customCount > 0)
        {
            for (qint32 i = 0; i < customCount; i++)
            {
                showCustomCOBoost(pCO, x, y, i, false);
            }
            x = 10;
            y += GameMap::getImageSize() * 2 + textAdvance;
        }
        if (pCO->showDefaultUnitZoneBoost())
        {
            for (qint32 i = 0; i < sortedUnits.size(); i++)
            {
                QString unitID = sortedUnits[i];
                spUnit pUnit = MemoryManagement::create<Unit>(unitID, pPlayer.get(), false, pPlayer->getMap());
                showCOBoost(pUnit, pCO, x, y);
            }
        }
    }
    else
    {
        m_CoBoost->setPosition(10, y);
        y += GameMap::getImageSize() * 2 + textAdvance;
    }
    setHeight(y + 100);
    connect(this, &COInfoActor::sigShowLink, this, &COInfoActor::showLink, Qt::QueuedConnection);    
}

void COInfoActor::showCOBoost(spUnit pUnit, spCO pCO, qint32 & x, qint32 & y)
{
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    m_UnitDataActors.append(MemoryManagement::create<oxygine::Actor>());
    qint32 i = m_UnitDataActors.size() - 1;
    m_UnitDataActors[i]->setPosition(x, y);
    QString unitId = pUnit->getUnitID();
    m_UnitDataActors[i]->addClickListener([this, unitId](oxygine::Event*)
    {
        emit sigShowLink(unitId);
    });
    addChild(m_UnitDataActors[i]);

    // gather basic co information
    qint32 offBonus = 0;
    qint32 defBonus = 0;
    qint32 firerangeBonus = 0;
    qint32 movementBonus = 0;

    if (pCO.get() != nullptr)
    {
        offBonus = pCO->getOffensiveBonus(nullptr, pUnit.get(), pUnit->getPosition(), nullptr, pUnit->getPosition(), false, GameEnums::LuckDamageMode_Off);
        defBonus = pCO->getDeffensiveBonus(nullptr, nullptr, pUnit->getPosition(), pUnit.get(), pUnit->getPosition(), true, GameEnums::LuckDamageMode_Off);
        firerangeBonus = pCO->getFirerangeModifier(pUnit.get(), pUnit->getPosition());
        movementBonus = pCO->getMovementpointModifier(pUnit.get(), pUnit->getPosition());
    }

    createStrengthBar(m_UnitDataActors[i], offBonus, 0);
    createStrengthBar(m_UnitDataActors[i], defBonus, GameMap::getImageSize() / 2);

    if (firerangeBonus != 0)
    {
        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
        pSprite->setResAnim(pCOSpriteManager->getResAnim("atkRange"));
        pSprite->setY(5 +  GameMap::getImageSize());
        pSprite->setScale(2.0f);
        m_UnitDataActors[i]->addChild(pSprite);
        oxygine::spTextField pText = MemoryManagement::create<oxygine::TextField>();
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
        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
        pSprite->setResAnim(pCOSpriteManager->getResAnim("moveRange"));
        pSprite->setPosition(20 +  GameMap::getImageSize(), 5 + GameMap::getImageSize());
        pSprite->setScale(2.0f);
        m_UnitDataActors[i]->addChild(pSprite);
        oxygine::spTextField pText = MemoryManagement::create<oxygine::TextField>();
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
        y += GameMap::getImageSize() * 2 + 25;
    }
    else
    {
        x += 100;
    }
}

void COInfoActor::showCustomCOBoost(spCO pCO, qint32 & x, qint32 & y, qint32 index, bool global)
{
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    m_UnitDataActors.append(MemoryManagement::create<oxygine::Actor>());
    qint32 i = m_UnitDataActors.size() - 1;
    m_UnitDataActors[i]->setPosition(x, y);

    CustomCoBoostInfo info;
    if (global)
    {
        pCO->getCustomUnitGlobalBoost(index, info);
    }
    else
    {
        pCO->getCustomUnitZoneBoost(index, info);
    }
    WikiDatabase* pDatabase = WikiDatabase::getInstance();
    m_UnitDataActors[i]->addChild(pDatabase->getIcon(pCO->getMap(), info.getIconId(), GameMap::defaultImageSize));
    QString wikiLink = info.getLink();
    m_UnitDataActors[i]->addClickListener([this, wikiLink](oxygine::Event*)
    {
        emit sigShowLink(wikiLink);
    });
    addChild(m_UnitDataActors[i]);
    // gather basic co information
    qint32 offBonus = 0;
    qint32 defBonus = 0;
    createStrengthBar(m_UnitDataActors[i], info.getOffensiveBoost(), 0);
    createStrengthBar(m_UnitDataActors[i], info.getDefensiveBoost(), GameMap::getImageSize() / 2);

    auto icons = info.getBonusIcons();
    auto bonus = info.getIconBonus();
    for (qint32 i2 = 0; i2 < icons.size(); ++i2)
    {
        if (i2 < 2)
        {
            oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
            pSprite->setResAnim(pCOSpriteManager->getResAnim(icons[i2]));
            pSprite->setY(5 +  GameMap::getImageSize());
            if (i2 > 0)
            {
                pSprite->setX(20 +  GameMap::getImageSize());
            }
            pSprite->setScale(2.0f);
            m_UnitDataActors[i]->addChild(pSprite);
            oxygine::spTextField pText = MemoryManagement::create<oxygine::TextField>();
            pText->setStyle(style);
            pText->setHtmlText(bonus[i2]);
            pText->setPosition(pSprite->getX() + pSprite->getScaledWidth() + 2, pSprite->getY() - 2);
            pText->setScale(0.75f);
            m_UnitDataActors[i]->addChild(pText);
        }
        else
        {
            oxygine::handleErrorPolicy(oxygine::error_policy::ep_show_error, "Unable to show bonus icon " + icons[i] + " because it exceeds the maximum of 2");
        }
    }
    if (x + 120 > m_pCurrentCO->getX() - 50)
    {
        x = 10;
        y += GameMap::getImageSize() * 2 + 25;
    }
    else
    {
        x += 100;
    }
}

void COInfoActor::showPerks(spCO pCO, qint32 & y)
{
    for (qint32 i = 0; i < m_PerkActors.size(); i++)
    {
        m_PerkActors[i]->detach();
    }
    m_PerkActors.clear();
    if (pCO.get() != nullptr)
    {
        oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;

        COPerkManager* pCOPerkManager = COPerkManager::getInstance();
        QStringList perks = pCO->getPerkList();
        qint32 x = 10;
        const qint32 width = 300;
        for (const auto & perk : perks)
        {
            oxygine::spActor perkActor = MemoryManagement::create<oxygine::Actor>();
            qint32 i = pCOPerkManager->getIndex(perk);
            QString name = pCOPerkManager->getName(i);
            QString icon = pCOPerkManager->getIcon(i);
            oxygine::ResAnim* pAnim = pCOPerkManager->getResAnim(icon, oxygine::error_policy::ep_ignore_error);
            oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
            pSprite->setResAnim(pAnim);
            if (pAnim != nullptr && pAnim->getWidth() > 0)
            {
                pSprite->setScale(static_cast<float>(GameMap::getImageSize() * 2) / static_cast<float>(pAnim->getWidth()));
            }
            pSprite->setPosition(0, 0);
            perkActor->addChild(pSprite);
            spLabel pLabel = MemoryManagement::create<Label>(250);
            pLabel->setStyle(style);
            pLabel->setHtmlText(name);
            pLabel->setPosition(GameMap::getImageSize() * 2 + 10, 10);
            perkActor->addChild(pLabel);
            perkActor->setPosition(x, y);
            addChild(perkActor);
            perkActor->addClickListener([this, perk](oxygine::Event*)
            {
                emit sigShowLink(perk);
            });
            m_PerkActors.append(perkActor);
            x += width;
            if (x + width > m_pCurrentCO->getX() - 50)
            {
                x = 10;
                y += GameMap::getImageSize() * 2 + 10;
            }
        }
    }
}

void COInfoActor::createStrengthBar(oxygine::spActor pActor, qint32 bonus, qint32 y)
{
    oxygine::ResAnim* pStartAnim;
    oxygine::ResAnim* pEndAnim;
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    constexpr float scale = 8.0f;
    qint32 width = 50 * scale;
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
    oxygine::spBox9Sprite pStartBox = MemoryManagement::create<oxygine::Box9Sprite>();
    pStartBox->setResAnim(pStartAnim);
    pStartBox->setSize((static_cast<qint32>(width * divider) + 1), 8 * scale);
    pStartBox->setPosition(5 +  GameMap::getImageSize(), y);
    if (scale > 0)
    {
        pStartBox->setScale(1.0f / scale);
    }
    pActor->addChild(pStartBox);

    oxygine::spBox9Sprite pEndBox = MemoryManagement::create<oxygine::Box9Sprite>();
    pEndBox->setResAnim(pEndAnim);
    pEndBox->setSize((static_cast<qint32>(width * (1.0f - divider)) + 2), 8 * scale);
    pEndBox->setPosition(5 +  GameMap::getImageSize() + pStartBox->getScaledWidth(), y);
    if (scale > 0)
    {
        pEndBox->setScale(1.0f / scale);
    }
    pActor->addChild(pEndBox);
}

void COInfoActor::showLink(QString pageID)
{    
    WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
    auto * entry = pWikiDatabase->getEntry(pageID);
    if (!entry->m_name.isEmpty() &&
        !entry->m_id.isEmpty())
    {
        oxygine::Stage::getStage()->addChild(pWikiDatabase->getPage(entry));
    }
    
}
