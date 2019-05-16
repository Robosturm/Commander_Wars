#include "playerselection.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/co.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/cospritemanager.h"

#include "objects/buildlistdialog.h"
#include "objects/coselectiondialog.h"
#include "objects/dropdownmenucolor.h"

#include "gameinput/humanplayerinput.h"
#include "ai/veryeasyai.h"

PlayerSelection::PlayerSelection(qint32 width, qint32 heigth)
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());


    m_pPlayerSelection = new Panel(true,
                                   QSize(width,
                                         heigth),
                                   QSize(pApp->getSettings()->getWidth() - 70, 100));
    connect(this, &PlayerSelection::sigShowSelectCO, this, &PlayerSelection::showSelectCO, Qt::QueuedConnection);
    connect(this, &PlayerSelection::buttonShowPlayerBuildList, this, &PlayerSelection::slotShowPlayerBuildList, Qt::QueuedConnection);
    connect(this, &PlayerSelection::sigAiChanged, this, &PlayerSelection::selectAI, Qt::QueuedConnection);

    this->addChild(m_pPlayerSelection);
}

void PlayerSelection::resetPlayerSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pPlayerSelection->setVisible(false);
    m_pPlayerSelection->clearContent();
    m_playerCO1.clear();
    m_playerCO2.clear();
    m_playerIncomes.clear();
    m_playerStartFonds.clear();
    m_playerAIs.clear();
    pApp->continueThread();
}

void PlayerSelection::showSelectCO(qint32 player, quint8 co)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    QString coid = "";
    GameMap* pMap = GameMap::getInstance();
    if (pMap->getPlayer(player)->getCO(co) != nullptr)
    {
        coid = pMap->getPlayer(player)->getCO(co)->getCoID();
    }
    spCOSelectionDialog dialog = new COSelectionDialog(coid, pMap->getPlayer(player)->getColor(), player);
    oxygine::getStage()->addChild(dialog);
    m_pPlayerSelection->setVisible(false);
    if (co == 0)
    {
        connect(dialog.get(), &COSelectionDialog::editFinished, this , &PlayerSelection::playerCO1Changed, Qt::QueuedConnection);
    }
    else
    {
        connect(dialog.get(), &COSelectionDialog::editFinished, this , &PlayerSelection::playerCO2Changed, Qt::QueuedConnection);
    }

    connect(dialog.get(), &COSelectionDialog::canceled, this , &PlayerSelection::playerCOCanceled, Qt::QueuedConnection);
    pApp->continueThread();
}

void PlayerSelection::showPlayerSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    resetPlayerSelection();
    m_pPlayerSelection->setVisible(true);

    GameMap* pMap = GameMap::getInstance();
    // font style
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    // add player labels at top
    QStringList items = {tr("Username"), tr("CO's"), tr("Color"), tr("AI Strength"), tr("Startfonds"), tr("Income Modifier"), tr("Team"), tr("Build List")};
    QVector<qint32> xPositions;
    qint32 labelminStepSize = (m_pPlayerSelection->getWidth() - 100) / items.size();
    if (labelminStepSize < 150)
    {
        labelminStepSize = 150;
    }
    qint32 curPos = 5;

    oxygine::spTextField pLabel;
    for (qint32 i = 0; i < items.size(); i++)
    {
        xPositions.append(curPos);
        pLabel = new oxygine::TextField();
        pLabel->setStyle(style);
        pLabel->setText(items[i].toStdString().c_str());
        qint32 width = pLabel->getTextRect().getWidth() + 10;
        pLabel->setPosition(curPos, 5);
        m_pPlayerSelection->addItem(pLabel);
        if (width < labelminStepSize)
        {
            width = labelminStepSize;
        }
        curPos += width;
    }
    xPositions.append(curPos);
    m_pPlayerSelection->setContentWidth(curPos + 50);
    qint32 y = pLabel->getTextRect().getHeight() + 10 + 25;
    // all player
    pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setText(tr("All").toStdString().c_str());
    pLabel->setPosition(xPositions[0], y);
    m_pPlayerSelection->addItem(pLabel);

    qint32 itemIndex = 1;
    oxygine::spButton pButtonAllCOs = ObjectManager::createButton(tr("All Random"));
    pButtonAllCOs->setPosition(xPositions[itemIndex] - 40, y);
    m_pPlayerSelection->addItem(pButtonAllCOs);
    pButtonAllCOs->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonAllCOsRandom();
    });
    connect(this, &PlayerSelection::buttonAllCOsRandom, this, &PlayerSelection::slotAllCOsRandom, Qt::QueuedConnection);

    itemIndex = 4;
    spSpinBox allStartFondsSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 100000);
    allStartFondsSpinBox->setPosition(xPositions[itemIndex], y);
    m_pPlayerSelection->addItem(allStartFondsSpinBox);
    connect(allStartFondsSpinBox.get(), &SpinBox::sigValueChanged, this, &PlayerSelection::allPlayerStartFondsChanged, Qt::QueuedConnection);

    itemIndex = 5;
    spSpinBox allIncomeSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 10, SpinBox::Mode::Float);
    allIncomeSpinBox->setPosition(xPositions[itemIndex], y);
    allIncomeSpinBox->setCurrentValue(1.0f);
    allIncomeSpinBox->setSpinSpeed(0.1f);
    m_pPlayerSelection->addItem(allIncomeSpinBox);
    connect(allIncomeSpinBox.get(), &SpinBox::sigValueChanged, this, &PlayerSelection::allPlayerIncomeChanged, Qt::QueuedConnection);

    itemIndex = 7;
    oxygine::spButton pButtonAllBuildList = ObjectManager::createButton(tr("Build List"));
    pButtonAllBuildList->setPosition(xPositions[itemIndex], y);
    m_pPlayerSelection->addItem(pButtonAllBuildList);
    pButtonAllBuildList->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonShowAllBuildList();
    });
    connect(this, &PlayerSelection::buttonShowAllBuildList, this, &PlayerSelection::slotShowAllBuildList, Qt::QueuedConnection);

    y += 10 + allIncomeSpinBox->getHeight();
    QVector<QString> teamList;
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        teamList.append(tr("Team") + " " + QString::number(i + 1));
    }

    QVector<QString> aiList = {tr("Human"), tr("Very Easy")};

    QString function = "getDefaultPlayerColors";
    QJSValueList args;
    QJSValue ret = pApp->getInterpreter()->doFunction("PLAYER", function, args);
    qint32 colorCount = ret.toInt();
    QVector<QColor> playerColors;

    for (qint32 i = 0; i < colorCount; i++)
    {
        Mainapp* pApp = Mainapp::getInstance();
        QString function = "getDefaultColor";
        QJSValueList args;
        args << i;
        ret = pApp->getInterpreter()->doFunction("PLAYER", function, args);
        playerColors.append(QColor(ret.toString()));
    }
    bool allPlayer1 = true;
    bool allHuman = true;
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = pMap->getPlayer(i);
        if (pPlayer->getTeam() != 0)
        {
            allPlayer1 = false;
        }
        if (pPlayer->getBaseGameInput() != nullptr)
        {
            if (pPlayer->getBaseGameInput()->getAiType() != BaseGameInputIF::AiTypes::Human)
            {
                allHuman = false;
            }
        }
        else
        {
            pPlayer->setBaseGameInput(new HumanPlayerInput());
        }
    }
    // assume players had no real team assigned
    // reassign each a unique team
    if (allPlayer1)
    {
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
           pMap->getPlayer(i)->setTeam(i);
        }
    }

    // add player selection information
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        itemIndex = 1;
        oxygine::spSprite spriteCO1 = new oxygine::Sprite();
        spriteCO1->setPosition(xPositions[itemIndex], y);
        spriteCO1->setSize(32, 12);
        spriteCO1->setScale(2.0f);
        m_pPlayerSelection->addItem(spriteCO1);
        m_playerCO1.append(spriteCO1);
        if (pMap->getPlayer(i)->getCO(0) != nullptr)
        {
            playerCO1Changed(pMap->getPlayer(i)->getCO(0)->getCoID(), i);
        }
        else
        {
            playerCO1Changed("", i);
        }
        spriteCO1->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigShowSelectCO(i, 0);
        });

        oxygine::spSprite spriteCO2 = new oxygine::Sprite();
        spriteCO2->setPosition(xPositions[itemIndex], y + 24);
        spriteCO2->setSize(32, 12);
        spriteCO2->setScale(2.0f);
        m_pPlayerSelection->addItem(spriteCO2);
        m_playerCO2.append(spriteCO2);
        if (pMap->getPlayer(i)->getCO(1) != nullptr)
        {
            playerCO2Changed(pMap->getPlayer(i)->getCO(1)->getCoID(), i);
        }
        else
        {
            playerCO2Changed("", i);
        }
        spriteCO2->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigShowSelectCO(i, 1);
        });

        bool up = false;
        if ((pMap->getPlayerCount() - i <= 5) &&
            (i > 5))
        {
            up = true;
        }

        itemIndex = 2;
        spDropDownmenuColor playerColor = new DropDownmenuColor(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, playerColors, up);
        playerColor->setPosition(xPositions[itemIndex], y);
        playerColor->setCurrentItem(pMap->getPlayer(i)->getColor());
        m_pPlayerSelection->addItem(playerColor);
        connect(playerColor.get(), &DropDownmenuColor::sigItemChanged, this, [=](QColor value)
        {
            playerColorChanged(value, i);
        }, Qt::QueuedConnection);

        itemIndex = 3;
        spDropDownmenu playerAi = new DropDownmenu(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, aiList, up);
        playerAi->setPosition(xPositions[itemIndex], y);
        if (allHuman)
        {
            if (i > 0)
            {
                playerAi->setCurrentItem(1);
                Player* pPlayer = pMap->getPlayer(i);
                pPlayer->setBaseGameInput(new VeryEasyAI());
            }
            else
            {
                playerAi->setCurrentItem(0);
            }
        }
        else
        {
            playerAi->setCurrentItem(static_cast<qint32>(pMap->getPlayer(i)->getBaseGameInput()->getAiType()));
        }
        connect(playerAi.get(), &DropDownmenu::sigItemChanged, this, [=](qint32)
        {
            emit sigAiChanged(i);
        });
        m_playerAIs.append(playerAi);
        m_pPlayerSelection->addItem(playerAi);

        itemIndex = 4;
        spSpinBox playerStartFondsSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 100000);
        playerStartFondsSpinBox->setPosition(xPositions[itemIndex], y);
        playerStartFondsSpinBox->setCurrentValue(pMap->getPlayer(i)->getFonds());
        m_pPlayerSelection->addItem(playerStartFondsSpinBox);
        m_playerStartFonds.append(playerStartFondsSpinBox);
        connect(playerStartFondsSpinBox.get(), &SpinBox::sigValueChanged, this, [=](float value)
        {
            playerStartFondsChanged(value, i);
        }, Qt::QueuedConnection);

        itemIndex = 5;
        spSpinBox playerIncomeSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 10, SpinBox::Mode::Float);
        playerIncomeSpinBox->setPosition(xPositions[itemIndex], y);
        playerIncomeSpinBox->setCurrentValue(pMap->getPlayer(i)->getFondsModifier());
        playerIncomeSpinBox->setSpinSpeed(0.1f);
        m_pPlayerSelection->addItem(playerIncomeSpinBox);
        m_playerIncomes.append(playerIncomeSpinBox);
        connect(playerIncomeSpinBox.get(), &SpinBox::sigValueChanged, this, [=](float value)
        {
            playerIncomeChanged(value, i);
        }, Qt::QueuedConnection);

        itemIndex = 6;

        spDropDownmenu playerTeam = new DropDownmenu(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, teamList, up);
        playerTeam->setPosition(xPositions[itemIndex], y);
        playerTeam->setCurrentItem(pMap->getPlayer(i)->getTeam());
        m_pPlayerSelection->addItem(playerTeam);
        connect(playerTeam.get(), &DropDownmenu::sigItemChanged, this, [=](qint32 value)
        {
            playerTeamChanged(value, i);
        }, Qt::QueuedConnection);

        itemIndex = 7;
        oxygine::spButton pButtonPlayerBuildList = ObjectManager::createButton(tr("Build List"));
        pButtonPlayerBuildList->setPosition(xPositions[itemIndex], y);
        m_pPlayerSelection->addItem(pButtonPlayerBuildList);
        pButtonPlayerBuildList->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
        {
            emit buttonShowPlayerBuildList(i);
        });
        y += 15 + playerIncomeSpinBox->getHeight();
    }
    m_pPlayerSelection->setContentHeigth(y);
    pApp->continueThread();
}

void PlayerSelection::allPlayerIncomeChanged(float value)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        pMap->getPlayer(i)->setFondsModifier(value);
        m_playerIncomes[i]->setCurrentValue(value);
    }
    pApp->continueThread();
}
void PlayerSelection::allPlayerStartFondsChanged(float value)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        pMap->getPlayer(i)->setFonds(static_cast<qint32>(value));
        m_playerStartFonds[i]->setCurrentValue(value);
    }
    pApp->continueThread();
}
void PlayerSelection::playerIncomeChanged(float value, qint32 playerIdx)
{
    GameMap* pMap = GameMap::getInstance();
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    pMap->getPlayer(playerIdx)->setFondsModifier(value);
    pApp->continueThread();
}

void PlayerSelection::slotShowAllBuildList()
{
    // use player 0 as default for showing all
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    spBuildListDialog dialog = new BuildListDialog(0, pMap->getPlayer(0)->getBuildList());
    oxygine::getStage()->addChild(dialog);
    connect(dialog.get(), &BuildListDialog::editFinished, this , &PlayerSelection::slotChangeAllBuildList, Qt::QueuedConnection);
    pApp->continueThread();
}

void PlayerSelection::slotShowPlayerBuildList(qint32 player)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    spBuildListDialog dialog = new BuildListDialog(player, pMap->getPlayer(player)->getBuildList());
    oxygine::getStage()->addChild(dialog);
    connect(dialog.get(), &BuildListDialog::editFinished, this , &PlayerSelection::slotChangePlayerBuildList, Qt::QueuedConnection);
    pApp->continueThread();
}

void PlayerSelection::slotChangeAllBuildList(qint32, QStringList buildList)
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        pMap->getPlayer(i)->setBuildList(buildList);
    }
}

void PlayerSelection::slotChangePlayerBuildList(qint32 player, QStringList buildList)
{
    GameMap* pMap = GameMap::getInstance();
    if (player >= 0 && player < pMap->getPlayerCount())
    {
        pMap->getPlayer(player)->setBuildList(buildList);
    }
}

void PlayerSelection::playerStartFondsChanged(float value, qint32 playerIdx)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    pMap->getPlayer(playerIdx)->setFonds(static_cast<qint32>(value));
    pApp->continueThread();
}
void PlayerSelection::playerTeamChanged(qint32 value, qint32 playerIdx)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    pMap->getPlayer(playerIdx)->setTeam(value);
    pApp->continueThread();
}
void PlayerSelection::playerColorChanged(QColor value, qint32 playerIdx)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    pMap->getPlayer(playerIdx)->setColor(value);
    pApp->continueThread();
}
void PlayerSelection::playerCO1Changed(QString coid, qint32 playerIdx)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    pMap->getPlayer(playerIdx)->setCO(coid, 0);
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = nullptr;
    if (coid.isEmpty())
    {
        pAnim = pCOSpriteManager->getResAnim("no_co+info");
    }
    else
    {
        pAnim = pCOSpriteManager->getResAnim((coid + "+info").toStdString().c_str());
    }
    m_playerCO1[playerIdx]->setResAnim(pAnim);
    m_pPlayerSelection->setVisible(true);
    pApp->continueThread();
}

void PlayerSelection::playerCO2Changed(QString coid, qint32 playerIdx)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    pMap->getPlayer(playerIdx)->setCO(coid, 1);
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = nullptr;
    if (coid.isEmpty())
    {
        pAnim = pCOSpriteManager->getResAnim("no_co+info");
    }
    else
    {
        pAnim = pCOSpriteManager->getResAnim((coid + "+info").toStdString().c_str());
    }
    m_playerCO2[playerIdx]->setResAnim(pAnim);
    m_pPlayerSelection->setVisible(true);
    pApp->continueThread();
}

void PlayerSelection::playerCOCanceled()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pPlayerSelection->setVisible(true);
    pApp->continueThread();
}

void PlayerSelection::slotAllCOsRandom()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        playerCO1Changed("CO_RANDOM", i);
        playerCO2Changed("CO_RANDOM", i);
    }
    pApp->continueThread();
}

qint32 PlayerSelection::getPlayerAI(qint32 player)
{
    return m_playerAIs[player]->getCurrentItem();
}

void PlayerSelection::selectAI(qint32 player)
{
    Player* pPlayer = GameMap::getInstance()->getPlayer(player);
    switch (m_playerAIs[player]->getCurrentItem())
    {
        case 0:
        {
            pPlayer->setBaseGameInput(new HumanPlayerInput());
            break;
        }
        case 1:
        {
            pPlayer->setBaseGameInput(new VeryEasyAI());
            break;
        }
        default:
        {
            pPlayer->setBaseGameInput(new HumanPlayerInput());
            break;
        }
    }
}
