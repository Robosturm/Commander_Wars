#include "coinfodialog.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "resource_management/cospritemanager.h"

#include "resource_management/unitspritemanager.h"

#include "resource_management/gamemanager.h"

#include "objects/panel.h"

#include "game/gamemap.h"

COInfoDialog::COInfoDialog(spCO pCO, spPlayer pPlayer,
                           std::function<void (spCO& m_CurrrentCO, spPlayer& pPlayer, qint32 direction)> stepFunction,
                           bool ingame)
    : QObject(),
      m_CurrentCO(pCO),
      m_pCurrentPlayer(pPlayer),
      m_StepFunction(stepFunction),
      m_Ingame(ingame)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

    // next button
    m_NextButton = pObjectManager->createButton(tr("Next"), 150);
    m_NextButton->setPosition(pApp->getSettings()->getWidth() - m_NextButton->getWidth() - 30, pApp->getSettings()->getHeight() - 30 - m_NextButton->getHeight());
    pSpriteBox->addChild(m_NextButton);
    m_NextButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit next();
    });

    // quit button
    m_QuitButton = pObjectManager->createButton(tr("Quit"), 150);
    m_QuitButton->setPosition(pApp->getSettings()->getWidth() / 2 - m_QuitButton->getWidth() / 2, pApp->getSettings()->getHeight() - 30 - m_QuitButton->getHeight());
    pSpriteBox->addChild(m_QuitButton);
    m_QuitButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit quit();
        detach();
    });

    // back button
    m_BackButton = pObjectManager->createButton(tr("Back"), 150);
    m_BackButton->setPosition(30, pApp->getSettings()->getHeight() - 30 - m_BackButton->getHeight());
    pSpriteBox->addChild(m_BackButton);
    m_BackButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit back();
    });

    connect(this, &COInfoDialog::next, this, &COInfoDialog::slotNext, Qt::QueuedConnection);
    connect(this, &COInfoDialog::back, this, &COInfoDialog::slotBack, Qt::QueuedConnection);

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    // no the fun begins create checkboxes and stuff and a panel down here
    m_pPanel = new Panel(true, QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110),
                         QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110));
    m_pPanel->setPosition(30, 30);
    pSpriteBox->addChild(m_pPanel);

    oxygine::spTextField pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setText(tr("CO Information").toStdString().c_str());
    pLabel->setScale(2.0f);
    pLabel->setPosition(m_pPanel->getWidth() / 2 - pLabel->getTextRect().getWidth(), 10);
    m_pPanel->addItem(pLabel);
    m_pCurrentCO = new oxygine::Sprite();
    m_pCurrentCO->setScale((pApp->getSettings()->getHeight() - 200) / 352.0f);
    m_pCurrentCO->setSize(208, 352);
    m_pCurrentCO->setPosition(pApp->getSettings()->getWidth() - 120 - m_pCurrentCO->getScaledWidth(), 90);
    m_pPanel->addItem(m_pCurrentCO);

    m_COName = new oxygine::TextField();
    m_COName->setStyle(style);
    m_COName->setScale(2.0f);
    m_pPanel->addItem(m_COName);

    style.multiline = true;
    m_COBio = new oxygine::TextField();
    m_COBio->setStyle(style);
    m_COBio->setWidth(m_pCurrentCO->getX() - 50);
    m_COBio->setPosition(10, m_COBio->getY() + 120);
    m_pPanel->addItem(m_COBio);

    m_HitSprite = new oxygine::Box9Sprite();
    m_HitSprite->setResAnim(pCOSpriteManager->getResAnim("hit"));
    m_HitSprite->setSize(100, 16);
    m_HitSprite->setScale(2.5f);
    m_HitSprite->setPosition(10, 200);
    oxygine::spTextField pTextField = new oxygine::TextField();
    pTextField->setPosition(14, 1);
    pTextField->setStyle(style);
    pTextField->setScale(1 / m_HitSprite->getScaleX());
    pTextField->setText(tr("Hit").toStdString().c_str());
    m_HitSprite->setWidth(pTextField->getTextRect().getWidth() / m_HitSprite->getScaleX() + 21);
    m_HitSprite->addChild(pTextField);
    m_pPanel->addItem(m_HitSprite);
    m_HitText = new oxygine::TextField();
    m_HitText->setStyle(style);
    m_HitText->setX(m_HitSprite->getX() + m_HitSprite->getScaledWidth() +10);
    m_pPanel->addItem(m_HitText);

    m_MissSprite = new oxygine::Box9Sprite();
    m_MissSprite->setResAnim(pCOSpriteManager->getResAnim("miss"));
    m_MissSprite->setSize(100, 16);
    m_MissSprite->setScale(2.5f);
    m_MissSprite->setPosition(10, 300);
    pTextField = new oxygine::TextField();
    pTextField->setPosition(14, 1);
    pTextField->setStyle(style);
    pTextField->setScale(1 / m_MissSprite->getScaleX());
    pTextField->setText(tr("Miss").toStdString().c_str());
    m_MissSprite->setWidth(pTextField->getTextRect().getWidth() / m_MissSprite->getScaleX()  + 21);
    m_MissSprite->addChild(pTextField);
    m_pPanel->addItem(m_MissSprite);
    m_MissText = new oxygine::TextField();
    m_MissText->setStyle(style);
    m_MissText->setX(m_MissSprite->getX() + m_MissSprite->getScaledWidth() + 10);
    m_pPanel->addItem(m_MissText);

    m_InfoSprite = new oxygine::Box9Sprite();
    m_InfoSprite->setResAnim(pCOSpriteManager->getResAnim("skill"));
    m_InfoSprite->setSize(100, 16);
    m_InfoSprite->setScale(2.5f);
    m_InfoSprite->setPosition(10, 400);
    pTextField = new oxygine::TextField();
    pTextField->setPosition(3, 2);
    pTextField->setStyle(style);
    pTextField->setScale(1 / m_InfoSprite->getScaleX());
    pTextField->setText(tr("Info").toStdString().c_str());
    m_InfoSprite->setWidth(pTextField->getTextRect().getWidth() / m_InfoSprite->getScaleX() + 12);
    m_InfoSprite->addChild(pTextField);
    m_InfoSprite->setX((pApp->getSettings()->getWidth() - m_pCurrentCO->getScaledWidth()) / 2 - m_InfoSprite->getScaledWidth() / 2);
    m_pPanel->addItem(m_InfoSprite);
    m_InfoText = new oxygine::TextField();
    m_InfoText->setStyle(style);
    m_InfoText->setWidth(m_pCurrentCO->getX() - 50);
    m_pPanel->addItem(m_InfoText);

    m_PowerSprite = new oxygine::Sprite();
    m_PowerSprite->setResAnim(pCOSpriteManager->getResAnim("power"));
    m_PowerSprite->setScale(2.5f);
    m_PowerSprite->setPosition(10, 400);
    m_pPanel->addItem(m_PowerSprite);
    m_Powername = new oxygine::TextField();
    m_Powername->setStyle(style);
    m_Powername->setX(m_PowerSprite->getX() + m_PowerSprite->getScaledWidth() + 10);
    m_pPanel->addItem(m_Powername);
    m_PowerDesc = new oxygine::TextField();
    m_PowerDesc->setStyle(style);
    m_PowerDesc->setWidth(m_pCurrentCO->getX() - 50);
    m_PowerDesc->setX(10);
    m_pPanel->addItem(m_PowerDesc);

    m_SuperPowerSprite = new oxygine::Sprite();
    m_SuperPowerSprite->setResAnim(pCOSpriteManager->getResAnim("superpower"));
    m_SuperPowerSprite->setScale(2.5f);
    m_SuperPowerSprite->setPosition(10, 400);
    m_pPanel->addItem(m_SuperPowerSprite);
    m_SuperPowername = new oxygine::TextField();
    m_SuperPowername->setStyle(style);
    m_SuperPowername->setX(m_SuperPowerSprite->getX() + m_SuperPowerSprite->getScaledWidth() + 10);
    m_pPanel->addItem(m_SuperPowername);
    m_SuperPowerDesc = new oxygine::TextField();
    m_SuperPowerDesc->setStyle(style);
    m_SuperPowerDesc->setWidth(m_pCurrentCO->getX() - 50);
    m_SuperPowerDesc->setX(10);
    m_pPanel->addItem(m_SuperPowerDesc);
    m_SynergyText = new oxygine::TextField();
    m_SynergyText->setStyle(style);
    m_SynergyText->setText(tr("Synergy").toStdString().c_str());
    m_SynergyText->setX(10);
    m_pPanel->addItem(m_SynergyText);

    showCO();
}

void COInfoDialog::slotNext()
{
    m_StepFunction(m_CurrentCO, m_pCurrentPlayer, 1);
    showCO();
}

void COInfoDialog::slotBack()
{
    m_StepFunction(m_CurrentCO, m_pCurrentPlayer, -1);
    showCO();
}

void COInfoDialog::showCO()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = nullptr;
    QString coid = "";
    if (m_CurrentCO.get() != nullptr)
    {
        coid = m_CurrentCO->getCoID();
    }
    if (!coid.isEmpty())
    {
        pAnim = pCOSpriteManager->getResAnim((coid + "+nrm").toStdString().c_str());
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
    if (m_CurrentCO.get() != nullptr)
    {
        corange = m_CurrentCO->getCORange();
    }

    Interpreter* pInterpreter = pApp->getInterpreter();
    QJSValue value;
    if (m_CurrentCO.get() != nullptr)
    {
        QJSValue value = pInterpreter->doFunction(coid, "getName");
        if (value.isString())
        {
            coName = value.toString();
        }
    }
    m_COName->setText(coName.toStdString().c_str());
    m_COName->setPosition((pApp->getSettings()->getWidth() - m_pCurrentCO->getScaledWidth()) / 2 - m_COName->getTextRect().getWidth(), 60);
    if (m_CurrentCO.get() != nullptr)
    {
        value = pInterpreter->doFunction(coid, "getBio");
        if (value.isString())
        {
            coBio = value.toString();
        }
    }
    m_COBio->setText(coBio.toStdString().c_str());

    if (m_CurrentCO.get() != nullptr)
    {
        value = pInterpreter->doFunction(coid, "getHits");
        if (value.isString())
        {
            coHits = value.toString();
        }
    }
    m_HitSprite->setY(m_COBio->getY() + m_COBio->getTextRect().getHeight() + 10);
    m_HitText->setText(coHits.toStdString().c_str());
    m_HitText->setY(m_HitSprite->getY());

    if (m_CurrentCO.get() != nullptr)
    {
        value = pInterpreter->doFunction(coid, "getMiss");
        if (value.isString())
        {
            coMiss = value.toString();
        }
    }
    m_MissSprite->setY(m_HitSprite->getY() + 50);
    m_MissText->setText(coMiss.toStdString().c_str());
    m_MissText->setY(m_MissSprite->getY());

    if (m_CurrentCO.get() != nullptr)
    {
        value = pInterpreter->doFunction(coid, "getCODescription");
        if (value.isString())
        {
            coDesc = value.toString();
        }
    }
    m_InfoSprite->setY(m_MissSprite->getY() + 50);
    m_InfoText->setText((coDesc + "\n\n" + tr("CO-Zone-Range: ") + QString::number(corange)).toStdString().c_str());
    m_InfoText->setY(m_InfoSprite->getY() + 50);

    if (m_CurrentCO.get() != nullptr)
    {
        value = pInterpreter->doFunction(coid, "getPowerDescription");
        if (value.isString())
        {
            coPowerDesc = value.toString();
        }
    }
    if (m_CurrentCO.get() != nullptr)
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
    m_Powername->setText(coPower.toStdString().c_str());
    m_PowerDesc->setText(coPowerDesc.toStdString().c_str());

    if (m_CurrentCO.get() != nullptr)
    {
        value = pInterpreter->doFunction(coid, "getSuperPowerDescription");
        if (value.isString())
        {
            coSuperpowerDesc = value.toString();
        }
    }
    if (m_CurrentCO.get() != nullptr)
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
    m_SuperPowername->setText(coSuperpower.toStdString().c_str());
    m_SuperPowerDesc->setText(coSuperpowerDesc.toStdString().c_str());

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 y = m_SuperPowerDesc->getY() + m_SuperPowerDesc->getTextRect().getHeight() + 20;
    if (m_CurrentCO.get() != nullptr)
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
            QJSValue obj1 = pApp->getInterpreter()->newQObject(m_CurrentCO.get());
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
                    pText->setText(coSynergyName.toStdString().c_str());
                    pText->setX(10);
                    pText->setY(y);
                    m_pPanel->addItem(pText);
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
                    m_pPanel->addItem(pActor);
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
        m_pPanel->addItem(m_UnitDataActors[i]);
        spUnit pUnit = new Unit(unitID, m_pCurrentPlayer.get(), false);
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
        if (m_CurrentCO.get() != nullptr)
        {
            offBonus = m_CurrentCO->getOffensiveBonus(pUnit.get(), QPoint(-1, -1), nullptr, QPoint(-1, -1), false);
            defBonus = m_CurrentCO->getDeffensiveBonus(nullptr, QPoint(-1, -1), pUnit.get(), QPoint(-1, -1), false);
            firerangeBonus = m_CurrentCO->getFirerangeModifier(pUnit.get(), QPoint(-1, -1));
            movementBonus = m_CurrentCO->getMovementpointModifier(pUnit.get(), QPoint(-1, -1));
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
                pText->setText(("+" + QString::number(firerangeBonus)).toStdString().c_str());
            }
            else
            {
                pText->setText((QString::number(firerangeBonus)).toStdString().c_str());
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
                pText->setText(("+" + QString::number(movementBonus)).toStdString().c_str());
            }
            else
            {
                pText->setText((QString::number(movementBonus)).toStdString().c_str());
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
    m_pPanel->setContentHeigth(y + 100);
    pApp->continueThread();
}

void COInfoDialog::createStrengthBar(oxygine::spActor pActor, qint32 bonus, qint32 y)
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

