#include "playerselection.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/co.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/gamemanager.h"

#include "objects/dialogs/rules/buildlistdialog.h"
#include "objects/dialogs/rules/coselectiondialog.h"
#include "objects/dialogs/rules/perkselectiondialog.h"

#include "gameinput/humanplayerinput.h"
#include "ai/veryeasyai.h"

#include "multiplayer/networkcommands.h"

#include "coreengine/filesupport.h"
#include "coreengine/userdata.h"

#include "network/localserver.h"

constexpr const char* const CO_ARMY = "CO_ARMY";

PlayerSelection::PlayerSelection(qint32 width, qint32 heigth)
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());


    m_pPlayerSelection = new Panel(true,
                                   QSize(width,
                                         heigth),
                                   QSize(Settings::getWidth() - 70, 100));
    connect(this, &PlayerSelection::sigShowSelectCO, this, &PlayerSelection::showSelectCO, Qt::QueuedConnection);
    connect(this, &PlayerSelection::sigShowSelectCOPerks, this, &PlayerSelection::showSelectCOPerks, Qt::QueuedConnection);
    connect(this, &PlayerSelection::buttonShowPlayerBuildList, this, &PlayerSelection::slotShowPlayerBuildList, Qt::QueuedConnection);
    connect(this, &PlayerSelection::sigAiChanged, this, &PlayerSelection::selectAI, Qt::QueuedConnection);
    connect(this, &PlayerSelection::sigCOsRandom, this, &PlayerSelection::slotCOsRandom, Qt::QueuedConnection);
    connect(this, &PlayerSelection::sigSelectedArmyChanged, this, &PlayerSelection::selectedArmyChanged, Qt::QueuedConnection);

    connect(this, &PlayerSelection::buttonShowAllBuildList, this, &PlayerSelection::slotShowAllBuildList, Qt::QueuedConnection);

    this->addChild(m_pPlayerSelection);
}

void PlayerSelection::attachNetworkInterface(spNetworkInterface pNetworkInterface)
{
    m_pNetworkInterface = pNetworkInterface;
    if (m_pNetworkInterface.get() != nullptr)
    {
        connect(m_pNetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &PlayerSelection::disconnected, Qt::QueuedConnection);
        connect(m_pNetworkInterface.get(), &NetworkInterface::recieveData, this, &PlayerSelection::recieveData, Qt::QueuedConnection);
    }
}

void PlayerSelection::attachCampaign(spCampaign campaign)
{
    m_pCampaign = campaign;
}

bool PlayerSelection::isOpenPlayer(qint32 player)
{
    if (m_pNetworkInterface.get() != nullptr)
    {
        if (m_playerAIs[player]->getCurrentItem() == m_playerAIs[player]->getItemCount() - 1)
        {
            return true;
        }
    }
    return false;
}

bool PlayerSelection::hasOpenPlayer()
{
    for (qint32 i = 0; i < m_playerAIs.size(); i++)
    {
        if (isOpenPlayer(i))
        {
            return true;
        }
    }
    return false;
}

QString PlayerSelection::getPlayerAiName(qint32 player)
{
    return m_playerAIs[player]->getCurrentItemText();
}

void PlayerSelection::setPlayerAiName(qint32 player, QString name)
{
    return m_playerAIs[player]->setCurrentItemText(name);
}

GameEnums::AiTypes PlayerSelection::getPlayerAiType(qint32 player)
{
    if (m_playerAIs[player]->getCurrentItemText() == tr("Closed"))
    {
        return GameEnums::AiTypes_Closed;
    }
    else if (m_pNetworkInterface.get() == nullptr)
    {
        return static_cast<GameEnums::AiTypes>(m_playerAIs[player]->getCurrentItem());
    }
    else
    {
        if (m_playerAIs[player]->getCurrentItem() == m_playerAIs[player]->getItemCount() - 1)
        {
            return GameEnums::AiTypes_Open;
        }
        else if (m_playerAIs[player]->getCurrentItem() < 0)
        {
            return GameEnums::AiTypes_ProxyAi;
        }
        else
        {
            return static_cast<GameEnums::AiTypes>(m_playerAIs[player]->getCurrentItem());
        }
    }
}

void PlayerSelection::resetPlayerSelection()
{
    
    m_pPlayerSelection->setVisible(false);
    m_pPlayerSelection->clearContent();
    m_playerCO1.clear();
    m_playerCO2.clear();
    m_playerColors.clear();
    m_playerIncomes.clear();
    m_playerStartFunds.clear();
    m_playerAIs.clear();
    m_playerTeams.clear();
    m_playerBuildlist.clear();
    m_PlayerSockets.clear();
    m_pReadyBoxes.clear();
    m_playerPerks.clear();
    m_playerArmy.clear();
    
}

void PlayerSelection::showSelectCO(qint32 player, quint8 co)
{
    
    QString coid = "";
    spGameMap pMap = GameMap::getInstance();
    if (pMap->getPlayer(player)->getCO(co) != nullptr)
    {
        coid = pMap->getPlayer(player)->getCO(co)->getCoID();
    }
    QStringList cos;
    if (m_pCampaign.get() != nullptr)
    {
        cos = m_pCampaign->getSelectableCOs(GameMap::getInstance(), player, co);
    }
    else
    {
        cos = pMap->getGameRules()->getCOBannlist();
        if (cos.size() == 0)
        {
            cos= COSpriteManager::getInstance()->getLoadedRessources();
        }
        Userdata* pUserdata = Userdata::getInstance();
        auto items = pUserdata->getItemsList(GameEnums::ShopItemType_CO_Skin, false);
        for (const auto & item : items)
        {
            cos.removeAll(item);
        }
        if (cos.size() == 0)
        {
            cos.push_back("");
        }
    }
    if (cos.size() == 0 ||
        cos[0] != "")
    {
        spCOSelectionDialog dialog = new COSelectionDialog(coid, pMap->getPlayer(player)->getColor(), player, cos);
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
    }
    
}

void PlayerSelection::showPlayerSelection()
{
    
    resetPlayerSelection();
    m_pPlayerSelection->setVisible(true);

    spGameMap pMap = GameMap::getInstance();
    // font style
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;

    // add player labels at top
    QStringList items = {tr("CO's"), tr("Color"), tr("AI Strength"), tr("Startfunds"), tr("Income Modifier"), tr("Team"), tr("Build List")};
    if (m_pNetworkInterface.get() != nullptr)
    {
        items.append(tr("Connected"));
    }
    QVector<qint32> xPositions;
    qint32 labelminStepSize = (m_pPlayerSelection->getWidth() - 100) / items.size();
    if (labelminStepSize < 220)
    {
        labelminStepSize = 220;
    }
    qint32 curPos = 5;

    oxygine::spTextField pLabel;
    for (qint32 i = 0; i < items.size(); i++)
    {
        xPositions.append(curPos);
        pLabel = new oxygine::TextField();
        pLabel->setStyle(style);
        pLabel->setHtmlText(items[i]);
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
    pLabel->setHtmlText(tr("All"));
    pLabel->setPosition(xPositions[0], y);
    m_pPlayerSelection->addItem(pLabel);

    qint32 itemIndex = 0;
    oxygine::spButton pButtonAllCOs = ObjectManager::createButton(tr("All Random"), labelminStepSize - 10);
    pButtonAllCOs->setPosition(xPositions[itemIndex], y);
    m_pPlayerSelection->addItem(pButtonAllCOs);
    pButtonAllCOs->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigCOsRandom(-1);
    });
    itemIndex = 1;
    oxygine::spButton pButtonCOs1 = ObjectManager::createButton(tr("CO 1 Random"), labelminStepSize - 10);
    pButtonCOs1->setPosition(xPositions[itemIndex], y);
    m_pPlayerSelection->addItem(pButtonCOs1);
    pButtonCOs1->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigCOsRandom(0);
    });

    itemIndex = 2;
    oxygine::spButton pButtonCOs2 = ObjectManager::createButton(tr("CO 2 Random"), labelminStepSize - 10);
    pButtonCOs2->setPosition(xPositions[itemIndex], y);
    m_pPlayerSelection->addItem(pButtonCOs2);
    pButtonCOs2->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigCOsRandom(1);
    });
    if (pMap->getGameRules()->getSingleCo())
    {
        pButtonAllCOs->setEnabled(false);
        pButtonCOs2->setEnabled(false);
    }

    if (m_pNetworkInterface.get() != nullptr ||
        m_pCampaign.get() != nullptr)
    {
        pButtonAllCOs->setVisible(false);
        pButtonCOs1->setVisible(false);
        pButtonCOs2->setVisible(false);
    }

    itemIndex = 3;
    spSpinBox allStartFundsSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 100000);
    allStartFundsSpinBox->setTooltipText(tr("Select with how much funds all player start the game."));
    allStartFundsSpinBox->setSpinSpeed(500);
    allStartFundsSpinBox->setPosition(xPositions[itemIndex], y);
    allStartFundsSpinBox->setCurrentValue(0);
    m_pPlayerSelection->addItem(allStartFundsSpinBox);
    connect(allStartFundsSpinBox.get(), &SpinBox::sigValueChanged, this, &PlayerSelection::allPlayerStartFundsChanged, Qt::QueuedConnection);
    if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
        saveGame ||
        m_pCampaign.get() != nullptr)
    {
        allStartFundsSpinBox->setEnabled(false);
    }

    itemIndex = 4;
    spSpinBox allIncomeSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 10, SpinBox::Mode::Float);
    allIncomeSpinBox->setTooltipText(tr("Select the income modifier for all players. The base income for each building is multiplied with this value. For most buildings this means 1.0 equals 1000 funds and 1.1 equals 1100 funds per building."));
    allIncomeSpinBox->setPosition(xPositions[itemIndex], y);
    allIncomeSpinBox->setCurrentValue(1.0f);
    allIncomeSpinBox->setSpinSpeed(0.1f);
    connect(allIncomeSpinBox.get(), &SpinBox::sigValueChanged, this, &PlayerSelection::allPlayerIncomeChanged, Qt::QueuedConnection);
    m_pPlayerSelection->addItem(allIncomeSpinBox);
    if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
        saveGame ||
        m_pCampaign.get() != nullptr)
    {
        allIncomeSpinBox->setEnabled(false);
    }

    itemIndex = 6;
    oxygine::spButton pButtonAllBuildList = ObjectManager::createButton(tr("Build List"), 150);
    pButtonAllBuildList->setPosition(xPositions[itemIndex], y);
    m_pPlayerSelection->addItem(pButtonAllBuildList);
    pButtonAllBuildList->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonShowAllBuildList();
    });
    if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
        saveGame ||
        m_pCampaign.get() != nullptr)
    {
        pButtonAllBuildList->setEnabled(false);
    }

    y += 10 + allIncomeSpinBox->getHeight();
    QVector<QString> teamList;
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        teamList.append(tr("Team") + " " + QString::number(i + 1));
    }
    QVector<QString> defaultAiList = {tr("Human"), tr("Very Easy"), tr("Normal"), tr("Normal Off."), tr("Normal Def."), tr("Closed")}; // tr("Heavy"),  // heavy ai disabled cause it's not finished
    QVector<QString> aiList = defaultAiList;
    if (m_pCampaign.get() != nullptr)
    {
        aiList = {tr("Human")};
        if (m_pNetworkInterface.get() != nullptr)
        {
            aiList.append(tr("Open"));
        }
    }
    else if (m_pNetworkInterface.get() != nullptr)
    {
        if (m_pNetworkInterface->getIsServer())
        {
            aiList.append(tr("Open"));
        }
        else
        {
            aiList = {tr("Human"), tr("Open")};
        }
    }

    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "getDefaultPlayerColors";
    QJSValueList args;
    QJSValue ret = pInterpreter->doFunction("PLAYER", function, args);
    qint32 colorCount = ret.toInt();
    QVector<QColor> playerColors;

    for (qint32 i = 0; i < colorCount; i++)
    {
        QString function = "getDefaultColor";
        QJSValueList args;
        args << i;
        ret = pInterpreter->doFunction("PLAYER", function, args);
        playerColors.append(QColor(ret.toString()));
    }
    bool allPlayer1 = true;
    bool allHuman = true;
    if (m_pCampaign.get() == nullptr)
    {
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = pMap->getPlayer(i);
            if (pPlayer->getTeam() != 0)
            {
                allPlayer1 = false;
            }
            if (pPlayer->getBaseGameInput() != nullptr)
            {
                if (pPlayer->getBaseGameInput()->getAiType() > GameEnums::AiTypes_Human)
                {
                    allHuman = false;
                }
            }
            else
            {
                pPlayer->setBaseGameInput(new HumanPlayerInput());
            }
        }
    }
    else
    {
        allHuman = false;
        allPlayer1 = false;
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
        qint32 ai = 0;
        if (pMap->getPlayer(i)->getBaseGameInput() != nullptr)
        {
            ai = static_cast<qint32>(pMap->getPlayer(i)->getBaseGameInput()->getAiType());
        }

        itemIndex = 0;
        oxygine::spSprite spriteCO1 = new oxygine::Sprite();
        spriteCO1->setPosition(xPositions[itemIndex], y);
        spriteCO1->setSize(32, 12);
        spriteCO1->setScale(2.0f);
        spriteCO1->setDisableColor(QColor(0, 0, 0, 0));
        m_pPlayerSelection->addItem(spriteCO1);
        m_playerCO1.append(spriteCO1);

        spriteCO1->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigShowSelectCO(i, 0);
        });
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            saveGame ||
            (ai > 0 && m_pCampaign.get() != nullptr))
        {
            spriteCO1->setEnabled(false);
        }

        if (m_pNetworkInterface.get() == nullptr ||
            m_pNetworkInterface->getIsServer())
        {
            if (pMap->getPlayer(i)->getCO(0) != nullptr)
            {
                playerCO1Changed(pMap->getPlayer(i)->getCO(0)->getCoID(), i);
            }
            else
            {
                playerCO1Changed("", i);
            }
        }

        oxygine::spSprite spriteCO2 = new oxygine::Sprite();
        spriteCO2->setPosition(xPositions[itemIndex], y + 24);
        spriteCO2->setSize(32, 12);
        spriteCO2->setScale(2.0f);
        spriteCO2->setDisableColor(QColor(0, 0, 0, 0));
        m_pPlayerSelection->addItem(spriteCO2);
        m_playerCO2.append(spriteCO2);
        spriteCO2->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigShowSelectCO(i, 1);
        });
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            saveGame ||
            (ai > 0 && m_pCampaign.get() != nullptr) ||
            pMap->getGameRules()->getSingleCo())
        {
            spriteCO2->setEnabled(false);
        }
        if (pMap->getGameRules()->getSingleCo())
        {
            playerCO2Changed("", i);
        }
        else if (m_pNetworkInterface.get() == nullptr ||
            m_pNetworkInterface->getIsServer())
        {
            if (pMap->getPlayer(i)->getCO(1) != nullptr)
            {
                playerCO2Changed(pMap->getPlayer(i)->getCO(1)->getCoID(), i);
            }
            else
            {
                playerCO2Changed("", i);
            }
        }

        oxygine::spButton pIconButton = ObjectManager::createIconButton("perk");
        pIconButton->setPosition(xPositions[itemIndex] + 70, y + 10);
        m_pPlayerSelection->addItem(pIconButton);
        m_playerPerks.append(pIconButton);
        pIconButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigShowSelectCOPerks(i);
        });
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            saveGame ||
            (ai > 0 && m_pCampaign.get() != nullptr))
        {
            pIconButton->setEnabled(false);
        }
        //
        createArmySelection(ai, xPositions, y, itemIndex, i);



        itemIndex++;
        spDropDownmenuColor playerColor = new DropDownmenuColor(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, playerColors);
        playerColor->setTooltipText(tr("Select the Color for this players army."));
        playerColor->setPosition(xPositions[itemIndex], y);
        playerColor->setCurrentItem(pMap->getPlayer(i)->getColor());
        m_pPlayerSelection->addItem(playerColor);
        connect(playerColor.get(), &DropDownmenuColor::sigItemChanged, this, [=](QColor value)
        {
            playerColorChanged(value, i, playerColor->getCurrentItem());
        }, Qt::QueuedConnection);
        m_playerColors.append(playerColor);
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            saveGame ||
            m_pCampaign.get() != nullptr)
        {
            playerColor->setEnabled(false);
        }

        itemIndex++;
        spDropDownmenu playerAi = new DropDownmenu(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, aiList);
        playerAi->setTooltipText(tr("Select who controls this player."));
        playerAi->setPosition(xPositions[itemIndex], y);
        if (m_pCampaign.get() != nullptr)
        {
            if (ai == 0)
            {
                ai = 0;
                playerAi->setCurrentItem(0);
            }
            else
            {
                playerAi->setCurrentItemText(defaultAiList[ai]);
                playerAi->setEnabled(false);
            }
        }
        else if (m_pNetworkInterface.get() != nullptr)
        {
            if (m_pNetworkInterface->getIsServer())
            {
                if (saveGame)
                {
                    if (ai == GameEnums::AiTypes_ProxyAi)
                    {
                        ai = aiList.size() - 1;
                        playerAi->setCurrentItem(ai);
                    }
                    else
                    {
                        playerAi->setCurrentItem(ai);
                    }
                }
                else if (i > 0)
                {
                    ai = aiList.size() - 1;
                    playerAi->setCurrentItem(ai);
                }
                else
                {
                    ai = 0;
                    playerAi->setCurrentItem(0);
                }
            }
            else
            {
                ai = GameEnums::AiTypes_Normal;
                playerAi->setCurrentItem(ai);
                playerAi->setEnabled(false);
            }
        }
        else if (allHuman)
        {
            if (i > 0)
            {
                ai = GameEnums::AiTypes_Normal;
                playerAi->setCurrentItem(ai);

            }
            else
            {
                ai = 0;
                playerAi->setCurrentItem(0);
            }
        }
        else
        {
            ai = static_cast<qint32>(pMap->getPlayer(i)->getBaseGameInput()->getAiType());
            playerAi->setCurrentItem(ai);
        }
        connect(playerAi.get(), &DropDownmenu::sigItemChanged, this, [=](qint32)
        {
            emit sigAiChanged(i);
        });
        m_playerAIs.append(playerAi);
        m_pPlayerSelection->addItem(playerAi);
        createAi(i, static_cast<GameEnums::AiTypes>(ai));
        m_PlayerSockets.append(0);

        itemIndex++;
        spSpinBox playerStartFundsSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 100000);
        playerStartFundsSpinBox->setTooltipText(tr("Select with how much funds this player starts the game."));
        playerStartFundsSpinBox->setPosition(xPositions[itemIndex], y);
        playerStartFundsSpinBox->setCurrentValue(pMap->getPlayer(i)->getFunds());
        playerStartFundsSpinBox->setSpinSpeed(500);
        m_pPlayerSelection->addItem(playerStartFundsSpinBox);
        m_playerStartFunds.append(playerStartFundsSpinBox);
        connect(playerStartFundsSpinBox.get(), &SpinBox::sigValueChanged, this, [=](float value)
        {
            playerStartFundsChanged(value, i);
        }, Qt::QueuedConnection);
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            saveGame ||
            m_pCampaign.get() != nullptr)
        {
            playerStartFundsSpinBox->setEnabled(false);
        }

        itemIndex++;
        spSpinBox playerIncomeSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 10, SpinBox::Mode::Float);
        playerIncomeSpinBox->setTooltipText(tr("Select the income modifier for this player. The base income for each building is multiplied with this value. For most buildings this means 1.0 equals 1000 funds and 1.1 equals 1100 funds per building."));
        playerIncomeSpinBox->setPosition(xPositions[itemIndex], y);
        playerIncomeSpinBox->setCurrentValue(pMap->getPlayer(i)->getFundsModifier());
        playerIncomeSpinBox->setSpinSpeed(0.1f);
        m_pPlayerSelection->addItem(playerIncomeSpinBox);
        m_playerIncomes.append(playerIncomeSpinBox);
        connect(playerIncomeSpinBox.get(), &SpinBox::sigValueChanged, this, [=](float value)
        {
            playerIncomeChanged(value, i);
        }, Qt::QueuedConnection);
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            saveGame ||
            m_pCampaign.get() != nullptr)
        {
            playerIncomeSpinBox->setEnabled(false);
        }

        itemIndex++;
        spDropDownmenu playerTeam = new DropDownmenu(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, teamList);
        playerTeam->setTooltipText(tr("Select the team of this player. The team can't be changed and players of the same team can't attack each other."));
        playerTeam->setPosition(xPositions[itemIndex], y);
        playerTeam->setCurrentItem(pMap->getPlayer(i)->getTeam());
        m_pPlayerSelection->addItem(playerTeam);
        connect(playerTeam.get(), &DropDownmenu::sigItemChanged, this, [=](qint32 value)
        {
            playerTeamChanged(value, i);
        }, Qt::QueuedConnection);
        m_playerTeams.append(playerTeam);
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            saveGame ||
            m_pCampaign.get() != nullptr)
        {
            playerTeam->setEnabled(false);
        }

        itemIndex++;
        oxygine::spButton pButtonPlayerBuildList = ObjectManager::createButton(tr("Build List"), 150);
        pButtonPlayerBuildList->setPosition(xPositions[itemIndex], y);
        m_pPlayerSelection->addItem(pButtonPlayerBuildList);
        pButtonPlayerBuildList->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
        {
            emit buttonShowPlayerBuildList(i);
        });
        m_playerBuildlist.append(pButtonPlayerBuildList);
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            saveGame ||
            m_pCampaign.get() != nullptr)
        {
            pButtonPlayerBuildList->setEnabled(false);
        }

        itemIndex++;
        if (m_pNetworkInterface.get() != nullptr)
        {
            spCheckbox pCheckbox = new Checkbox();
            pCheckbox->setTooltipText(tr("Shows which player is ready to start the game. All players need to be checked in order to start a game."));
            pCheckbox->setPosition(xPositions[itemIndex] + labelminStepSize / 2 - pCheckbox->getWidth(), y);
            pCheckbox->setEnabled(false);
            m_pReadyBoxes.append(pCheckbox);
            m_pPlayerSelection->addItem(pCheckbox);
        }

        y += 15 + playerIncomeSpinBox->getHeight();
    }
    m_pPlayerSelection->setContentHeigth(y + 50);
    
}

void PlayerSelection::createArmySelection(qint32 ai, QVector<qint32> & xPositions, qint32 y, qint32 itemIndex, qint32 player)
{
    auto creator = [](QString army)
    {
        GameManager* pGameManager = GameManager::getInstance();
        oxygine::ResAnim* pAnim = pGameManager->getResAnim("icon_" + army.toLower());
        oxygine::spSprite ret = new oxygine::Sprite();
        ret->setResAnim(pAnim);
        return ret;
    };
    QStringList armies = getSelectableArmies();
    spDropDownmenuSprite pArmy = new DropDownmenuSprite(105, armies, creator);
    pArmy->setTooltipText(tr("Selects the army for the player. CO means the army of the first CO is selected."));
    m_pPlayerSelection->addItem(pArmy);
    m_playerArmy.append(pArmy);
    connect(pArmy.get(), &DropDownmenuSprite::sigItemChanged, [=](qint32)
    {
        emit sigSelectedArmyChanged(player, pArmy->getCurrentItemText());
    });
    if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
        saveGame ||
        (ai > 0 && m_pCampaign.get() != nullptr))
    {
        pArmy->setEnabled(false);
    }
    pArmy->setPosition(xPositions[itemIndex] + 105, y);
}

QStringList PlayerSelection::getSelectableArmies()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue erg = pInterpreter->doFunction("PLAYER", "getArmies");
    QStringList ret = erg.toVariant().toStringList();
    ret.push_front(CO_ARMY);
    return ret;
}

void PlayerSelection::selectedArmyChanged(qint32 player, QString army)
{
    spGameMap pMap = GameMap::getInstance();
    
    Player* pPlayer = pMap->getPlayer(player);
    if (army == CO_ARMY)
    {
        pPlayer->setPlayerArmySelected(false);
    }
    else
    {
        pPlayer->setPlayerArmy(army);
        pPlayer->setPlayerArmySelected(true);
    }
    if (m_pNetworkInterface.get() != nullptr)
    {
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << NetworkCommands::PLAYERARMY;
        sendStream << player;
        sendStream << army;
        m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, true);
    }
    
}

void PlayerSelection::allPlayerIncomeChanged(float value)
{
    
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        pMap->getPlayer(i)->setFundsModifier(value);
        m_playerIncomes[i]->setCurrentValue(value);
    }
    playerDataChanged();
    
}
void PlayerSelection::allPlayerStartFundsChanged(float value)
{
    
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        pMap->getPlayer(i)->setFunds(static_cast<qint32>(value));
        m_playerStartFunds[i]->setCurrentValue(value);
    }
    playerDataChanged();
    
}
void PlayerSelection::playerIncomeChanged(float value, qint32 playerIdx)
{
    spGameMap pMap = GameMap::getInstance();
    
    pMap->getPlayer(playerIdx)->setFundsModifier(value);
    playerDataChanged();
    
}

void PlayerSelection::slotShowAllBuildList()
{
    // use player 0 as default for showing all
    
    spGameMap pMap = GameMap::getInstance();
    spBuildListDialog dialog = new BuildListDialog(0, pMap->getPlayer(0)->getBuildList());
    oxygine::getStage()->addChild(dialog);
    connect(dialog.get(), &BuildListDialog::editFinished, this , &PlayerSelection::slotChangeAllBuildList, Qt::QueuedConnection);
    
}

void PlayerSelection::slotShowPlayerBuildList(qint32 player)
{
    
    spGameMap pMap = GameMap::getInstance();
    spBuildListDialog dialog = new BuildListDialog(player, pMap->getPlayer(player)->getBuildList());
    oxygine::getStage()->addChild(dialog);
    connect(dialog.get(), &BuildListDialog::editFinished, this , &PlayerSelection::slotChangePlayerBuildList, Qt::QueuedConnection);
    
}

void PlayerSelection::slotChangeAllBuildList(qint32, QStringList buildList)
{
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        pMap->getPlayer(i)->setBuildList(buildList);
    }
    playerDataChanged();
}

void PlayerSelection::slotChangePlayerBuildList(qint32 player, QStringList buildList)
{
    spGameMap pMap = GameMap::getInstance();
    if (player >= 0 && player < pMap->getPlayerCount())
    {
        pMap->getPlayer(player)->setBuildList(buildList);
    }
    playerDataChanged();
}

void PlayerSelection::playerStartFundsChanged(float value, qint32 playerIdx)
{
    
    spGameMap pMap = GameMap::getInstance();
    pMap->getPlayer(playerIdx)->setFunds(static_cast<qint32>(value));
    playerDataChanged();
    
}
void PlayerSelection::playerTeamChanged(qint32 value, qint32 playerIdx)
{
    
    spGameMap pMap = GameMap::getInstance();
    pMap->getPlayer(playerIdx)->setTeam(value);
    playerDataChanged();
    
}

void PlayerSelection::playerDataChanged()
{
    if (m_pNetworkInterface.get() != nullptr &&
        m_pNetworkInterface->getIsServer())
    {
        spGameMap pMap = GameMap::getInstance();
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << NetworkCommands::PLAYERDATA;
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = pMap->getPlayer(i);
            sendStream << pPlayer->getFunds();
            sendStream << pPlayer->getFundsModifier();
            sendStream << pPlayer->getTeam();
            QStringList buildList = pPlayer->getBuildList();
            sendStream << static_cast<qint32>(buildList.size());
            for (qint32 i2 = 0; i2 < buildList.size(); i2++)
            {
                sendStream << buildList[i2];
            }
        }
        m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
    }
}

void PlayerSelection::playerColorChanged(QColor value, qint32 playerIdx, qint32 item)
{
    
    spGameMap pMap = GameMap::getInstance();
    pMap->getPlayer(playerIdx)->setColor(value, item);
    if (m_pNetworkInterface.get() != nullptr)
    {
        spGameMap pMap = GameMap::getInstance();
        Player* pPlayer = pMap->getPlayer(playerIdx);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << NetworkCommands::COLORDATA;
        sendStream << playerIdx;
        sendStream << pPlayer->getColor();
        m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, true);
    }
    
}

void PlayerSelection::playerCO1Changed(QString coid, qint32 playerIdx)
{
    
    if (!saveGame)
    {
        spGameMap pMap = GameMap::getInstance();
        CO* pCO = pMap->getPlayer(playerIdx)->getCO(1);
        if (coid == "" ||
            coid == "CO_RANDOM" ||
            pCO == nullptr ||
            pCO->getCoID() != coid)
        {
            QStringList perks;
            CO* pCurrentCO = pMap->getPlayer(playerIdx)->getCO(0);
            if (pCurrentCO != nullptr)
            {
                perks = pCurrentCO->getPerkList();
            }
            pMap->getPlayer(playerIdx)->setCO(coid, 0);
            pCurrentCO = pMap->getPlayer(playerIdx)->getCO(0);
            if (pCurrentCO != nullptr)
            {
                pCurrentCO->setPerkList(perks);
                pCurrentCO->setCoStyleFromUserdata();
            }
            updateCOData(playerIdx);
        }
    }
    updateCO1Sprite(coid, playerIdx);
    m_pPlayerSelection->setVisible(true);
    
}
void PlayerSelection::updateCO1Sprite(QString coid, qint32 playerIdx)
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get())
    {
        CO* pCurrentCO = pMap->getPlayer(playerIdx)->getCO(0);
        oxygine::ResAnim* pAnim = nullptr;
        if (coid.isEmpty() || pCurrentCO == nullptr)
        {
            pAnim = COSpriteManager::getInstance()->getResAnim("no_co+info");
        }
        else
        {
            pAnim = pCurrentCO->getResAnim((coid + "+info"));
        }
        m_playerCO1[playerIdx]->setResAnim(pAnim);
    }
}
void PlayerSelection::playerCO2Changed(QString coid, qint32 playerIdx)
{    
    if (!saveGame)
    {
        spGameMap pMap = GameMap::getInstance();
        CO* pCO = pMap->getPlayer(playerIdx)->getCO(0);
        if (coid == "" ||
            coid == "CO_RANDOM" ||
            pCO == nullptr ||
            pCO->getCoID() != coid)
        {
            QStringList perks;
            CO* pCurrentCO = pMap->getPlayer(playerIdx)->getCO(1);
            if (pCurrentCO != nullptr)
            {
                perks = pCurrentCO->getPerkList();
            }
            pMap->getPlayer(playerIdx)->setCO(coid, 1);
            pCurrentCO = pMap->getPlayer(playerIdx)->getCO(1);
            if (pCurrentCO != nullptr)
            {
                pCurrentCO->setPerkList(perks);
                pCurrentCO->setCoStyleFromUserdata();
            }
            updateCOData(playerIdx);
        }
    }
    updateCO2Sprite(coid, playerIdx);
    m_pPlayerSelection->setVisible(true);
    
}
void PlayerSelection::updateCO2Sprite(QString coid, qint32 playerIdx)
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get())
    {
        CO* pCurrentCO = pMap->getPlayer(playerIdx)->getCO(1);
        oxygine::ResAnim* pAnim = nullptr;
        if (coid.isEmpty() || pCurrentCO == nullptr)
        {
            pAnim = COSpriteManager::getInstance()->getResAnim("no_co+info");
        }
        else
        {
            pAnim = pCurrentCO->getResAnim((coid + "+info"));
        }
        m_playerCO2[playerIdx]->setResAnim(pAnim);
    }
}

void PlayerSelection::updateCOData(qint32 playerIdx)
{
    if (m_pNetworkInterface.get() != nullptr)
    {
        spGameMap pMap = GameMap::getInstance();
        Player* pPlayer = pMap->getPlayer(playerIdx);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << NetworkCommands::CODATA;
        sendStream << playerIdx;
        CO* pCO = pPlayer->getCO(0);
        QString coid = "";
        QStringList perks;
        if (pCO != nullptr)
        {
            coid = pCO->getCoID();
            perks = pCO->getPerkList();
        }
        sendStream << coid;
        Filesupport::writeVectorList(sendStream, perks);
        if (pCO != nullptr)
        {
            pCO->writeCoStyleToStream(sendStream);
        }
        pCO = pPlayer->getCO(1);
        coid = "";
        perks.clear();
        if (pCO != nullptr)
        {
            coid = pCO->getCoID();
            perks = pCO->getPerkList();
        }
        sendStream << coid;
        Filesupport::writeVectorList(sendStream, perks);
        if (pCO != nullptr)
        {
            pCO->writeCoStyleToStream(sendStream);
        }
        m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, true);
    }
}

void PlayerSelection::playerCOCanceled()
{
    m_pPlayerSelection->setVisible(true);
}

void PlayerSelection::slotCOsRandom(qint32 mode)
{
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        if ((mode == 0) || mode < 0)
        {
            playerCO1Changed("CO_RANDOM", i);
        }
        if ((mode == 1) || mode < 0)
        {
            playerCO2Changed("CO_RANDOM", i);
        }
    }    
}

void PlayerSelection::showSelectCOPerks(qint32 player)
{
    spGameMap pMap = GameMap::getInstance();
    Player* pPlayer = pMap->getPlayer(player);
    if (pPlayer->getCO(0) != nullptr || pPlayer->getCO(1) != nullptr)
    {
        Userdata* pUserdata = Userdata::getInstance();
        auto hiddenList = pUserdata->getItemsList(GameEnums::ShopItemType_Perk, false);
        spPerkSelectionDialog pPerkSelectionDialog = new PerkSelectionDialog(pPlayer, pMap->getGameRules()->getMaxPerkCount(), false, hiddenList);
        oxygine::getStage()->addChild(pPerkSelectionDialog);
        connect(pPerkSelectionDialog.get(), &PerkSelectionDialog::sigFinished, [=]()
        {
            updateCOData(player);
        });
    }    
}

void PlayerSelection::selectPlayerAi(qint32 player, GameEnums::AiTypes eAiType)
{
    if (player >= 0 && player < m_playerAIs.size())
    {
        m_playerAIs[player]->setCurrentItem(static_cast<qint32>(eAiType));
        selectAI(player);
    }
}

void PlayerSelection::selectAI(qint32 player)
{
    if (m_pNetworkInterface.get() != nullptr)
    {
        GameEnums::AiTypes type = static_cast<GameEnums::AiTypes>(m_playerAIs[player]->getCurrentItem());
        if (isOpenPlayer(player))
        {
            type = GameEnums::AiTypes_Open;
        }
        if (m_pNetworkInterface->getIsServer())
        {
            Console::print("AI " + QString::number(type) + " selected for player " + QString::number(player) + " sending data.", Console::eDEBUG);
            quint64 socket = m_pNetworkInterface->getSocketID();
            m_PlayerSockets[player] = socket;
            spGameMap pMap = GameMap::getInstance();
            createAi(player, static_cast<GameEnums::AiTypes>(m_playerAIs[player]->getCurrentItem()));
            QString name;
            if (type == GameEnums::AiTypes_Human)
            {
                name = Settings::getUsername();
            }
            else
            {
                name = m_playerAIs[player]->getCurrentItemText();
            }
            qint32 ai = type;
            if (type == GameEnums::AiTypes_Open)
            {
                ai = static_cast<qint32>(GameEnums::AiTypes_Open);
            }
            else if (m_isServerGame &&
                     Mainapp::getSlave() &&
                     type != GameEnums::AiTypes_Human)
            {
                ai = type;
                createAi(player, GameEnums::AiTypes_ProxyAi);
            }
            else if (Mainapp::getSlave())
            {
                ai = static_cast<qint32>(GameEnums::AiTypes_ProxyAi);
            }
            QByteArray data;
            createPlayerChangedData(data, socket, name, ai, player);
            // update data for all clients
            m_pNetworkInterface->sig_sendData(0, data, NetworkInterface::NetworkSerives::Multiplayer, false);
            updatePlayerData(player);
        }
        else
        {
            sendPlayerRequest(0, player, type);
        }
    }
    else
    {
        createAi(player, static_cast<GameEnums::AiTypes>(m_playerAIs[player]->getCurrentItem()));
    }
}

void PlayerSelection::createAi(qint32 player, GameEnums::AiTypes type)
{
    GameMap* pMap = GameMap::getInstance();
    Player* pPlayer = pMap->getPlayer(player);
    pPlayer->setBaseGameInput(BaseGameInputIF::createAi(type));
    if (pPlayer->getBaseGameInput() != nullptr)
    {
        pPlayer->getBaseGameInput()->setEnableNeutralTerrainAttack(pMap->getGameRules()->getAiAttackTerrain());
    }
}

void PlayerSelection::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    // data for us?
    if (service == NetworkInterface::NetworkSerives::Multiplayer)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        Console::print("Network Command received: " + messageType + " for socket " + QString::number(socketID), Console::eDEBUG);
        if (messageType == NetworkCommands::REQUESTPLAYER)
        {
            requestPlayer(socketID, stream);
        }
        else if (messageType == NetworkCommands::PLAYERCHANGED)
        {
            changePlayer(socketID, stream);
            sendOpenPlayerCount();
        }
        else if (messageType == NetworkCommands::PLAYERDATA)
        {
            recievedPlayerData(socketID, stream);
        }
        else if (messageType == NetworkCommands::CODATA)
        {
            recievedCOData(socketID, stream);
        }
        else if (messageType == NetworkCommands::COLORDATA)
        {
            recievedColorData(socketID, stream);
        }
        else if (messageType == NetworkCommands::PLAYERARMY)
        {
            recievePlayerArmy(socketID, stream);
        }
        else
        {
            Console::print("Command not handled in playerselection", Console::eDEBUG);
        }
    }
    else if (service == NetworkInterface::NetworkSerives::ServerHosting)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        Console::print("Server Command received: " + messageType + " for socket " + QString::number(socketID), Console::eDEBUG);
        if (messageType == NetworkCommands::SERVERREQUESTOPENPLAYERCOUNT)
        {
            sendOpenPlayerCount();
        }
        else
        {
            Console::print("Command not handled in playerselection", Console::eDEBUG);
        }
    }
}

void PlayerSelection::sendOpenPlayerCount()
{
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream << NetworkCommands::SERVEROPENPLAYERCOUNT;
    qint32 openPlayerCount = 0;
    for (const auto & playerAI : m_playerAIs)
    {
        if (playerAI->getCurrentItem() == playerAI->getItemCount() - 1)
        {
            openPlayerCount++;
        }
    }
    sendStream << openPlayerCount;
    emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::ServerHosting, false);
}

bool PlayerSelection::getIsServerGame() const
{
    return m_isServerGame;
}

void PlayerSelection::setIsServerGame(bool isServerGame)
{
    m_isServerGame = isServerGame;
}

void PlayerSelection::recievePlayerReady(quint64 socketID, QDataStream& stream)
{
    if (m_pNetworkInterface->getIsServer())
    {
        bool value = false;;
        stream >> value;
        QVector<qint32> player;
        for  (qint32 i = 0; i < m_PlayerSockets.size(); i++)
        {
            if (m_PlayerSockets[i] == socketID)
            {
                m_pReadyBoxes[i]->setChecked(value);
                player.append(i);
            }
        }
        sendPlayerReady(socketID, player, value);
    }
}

void PlayerSelection::sendPlayerReady(quint64 socketID, const QVector<qint32> & player, bool value)
{
    if (m_pNetworkInterface->getIsServer())
    {
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << NetworkCommands::SERVERREADY;
        sendStream << value;
        sendStream << static_cast<qint32>(player.size());
        for  (qint32 i = 0; i < player.size(); i++)
        {
            sendStream << player[i];
        }
        m_pNetworkInterface.get()->sigForwardData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer);
    }
}

void PlayerSelection::recievePlayerServerReady(quint64, QDataStream& stream)
{
    if (!m_pNetworkInterface->getIsServer() ||
        m_isServerGame)
    {
        bool value = false;
        stream >> value;
        qint32 size = 0;
        stream >> size;
        for  (qint32 i = 0; i < size; i++)
        {
            qint32 player = 0;
            stream >> player;
            m_pReadyBoxes[player]->setChecked(value);
        }
    }
}

bool PlayerSelection::getSaveGame() const
{
    return saveGame;
}

void PlayerSelection::setSaveGame(bool value)
{
    saveGame = value;
}

void PlayerSelection::sendPlayerRequest(quint64 socketID, qint32 player, GameEnums::AiTypes aiType)
{
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream << NetworkCommands::REQUESTPLAYER;
    // request player smaller 0 for any (the first avaible on the server :)
    sendStream << static_cast<qint32>(player);
    sendStream << Settings::getUsername();
    sendStream << static_cast<qint32>(aiType);
    emit m_pNetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
}

void PlayerSelection::requestPlayer(quint64 socketID, QDataStream& stream)
{
    if (m_pNetworkInterface->getIsServer() ||
        m_isServerGame)
    {
        spGameMap pMap = GameMap::getInstance();
        qint32 player = 0;
        QString username;
        stream >> player;
        stream >> username;
        qint32 aiType;
        stream >> aiType;
        GameEnums::AiTypes eAiType = static_cast<GameEnums::AiTypes>(aiType);
        // the client wants any player?
        if (player < 0)
        {
            for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
            {
                if (isOpenPlayer(i))
                {
                    player = i;
                    break;
                }
            }
        }
        // opening a player is always valid changing to an human with an open player is also valid
        if (isOpenPlayer(player) || eAiType == GameEnums::AiTypes_Open)
        {
            // valid request
            // change data locally and send remote update
            Player* pPlayer = pMap->getPlayer(player);
            // we need to handle opening a player slightly different here...
            if (eAiType == GameEnums::AiTypes_Open)
            {
                pPlayer->setBaseGameInput(nullptr);
                m_playerAIs[player]->setCurrentItem(m_playerAIs[player]->getItemCount() - 1);
                m_PlayerSockets[player] = 0;
            }
            else
            {
                pPlayer->setBaseGameInput(BaseGameInputIF::createAi(GameEnums::AiTypes_ProxyAi));
                pPlayer->setSocketId(socketID);
                m_playerAIs[player]->setCurrentItemText(username);
                m_PlayerSockets[player] = socketID;
            }
            updatePlayerData(player);

            qint32 aiType = 0;
            if (eAiType == GameEnums::AiTypes_Open)
            {
                aiType = static_cast<qint32>(GameEnums::AiTypes_Open);
            }
            else
            {
                aiType = static_cast<qint32>(GameEnums::AiTypes_Human);
            }
            QByteArray sendDataRequester;
            createPlayerChangedData(sendDataRequester, socketID, username, aiType, player);
            // create data block for other clients
            if (eAiType == GameEnums::AiTypes_Open)
            {
                aiType = static_cast<qint32>(GameEnums::AiTypes_Open);
            }
            else
            {
                aiType = static_cast<qint32>(GameEnums::AiTypes_ProxyAi);
            }            
            QByteArray sendDataOtherClients;
            createPlayerChangedData(sendDataOtherClients, socketID, username, aiType, player);
            // send player update
            m_pNetworkInterface->sig_sendData(socketID, sendDataRequester, NetworkInterface::NetworkSerives::Multiplayer, false);
            emit m_pNetworkInterface.get()->sigForwardData(socketID, sendDataOtherClients, NetworkInterface::NetworkSerives::Multiplayer);
        }
        else
        {
            Console::print("Player change rejected. Cause player isn't open anymore", Console::eDEBUG);
        }

    }
}

void PlayerSelection::changePlayer(quint64 socketId, QDataStream& stream)
{
    if (!m_pNetworkInterface->getIsServer() ||
        m_isServerGame)
    {
        spGameMap pMap = GameMap::getInstance();
        quint64 socket;
        QString name;
        qint32 aiType;
        qint32 player;
        stream >> socket;
        stream >> name;
        stream >> player;
        stream >> aiType;
        Console::print("Remote change of Player " + QString::number(player) + " to " + name + " for socket " + QString::number(socket) + " and ai " + QString::number(aiType), Console::eDEBUG);
        if (socket != m_pNetworkInterface->getSocketID() ||
            aiType != GameEnums::AiTypes::AiTypes_ProxyAi)
        {
            m_PlayerSockets[player] = socket;
            if (m_isServerGame)
            {
                if (aiType != GameEnums::AiTypes::AiTypes_Open &&
                    aiType != GameEnums::AiTypes::AiTypes_Human)
                {
                    aiType = GameEnums::AiTypes::AiTypes_ProxyAi;
                }
                Console::print("Change of Player " + QString::number(player) + " to " + name + " for socket " + QString::number(socket) + " and ai " + QString::number(aiType) + " after validation.", Console::eDEBUG);
            }
            GameEnums::AiTypes eAiType = static_cast<GameEnums::AiTypes>(aiType);
            setPlayerAi(player, eAiType, name);
            pMap->getPlayer(player)->deserializeObject(stream);
            pMap->getPlayer(player)->setBaseGameInput(BaseGameInputIF::createAi(eAiType));

            bool humanFound = false;
            for (qint32 i = 0; i < m_playerAIs.size(); i++)
            {
                if (static_cast<GameEnums::AiTypes>(m_playerAIs[i]->getCurrentItem()) == GameEnums::AiTypes_Human)
                {
                    humanFound = true;
                    break;
                }
            }
            updatePlayerData(player);
            if (!humanFound)
            {
                emit sigDisconnect();
            }
            if (m_isServerGame)
            {
                if (aiType != GameEnums::AiTypes::AiTypes_Open)
                {
                    aiType = GameEnums::AiTypes::AiTypes_ProxyAi;
                }
                QByteArray data;
                createPlayerChangedData(data, socket, name, aiType, player);
                m_pNetworkInterface->sigForwardData(socketId, data, NetworkInterface::NetworkSerives::Multiplayer);
            }
        }
        else
        {
            Console::print("Update Ignored", Console::eDEBUG);
        }
    }
}

void PlayerSelection::createPlayerChangedData(QByteArray & data, quint64 socketId, QString name, qint32 aiType, qint32 player)
{
    QDataStream sendStream(&data, QIODevice::WriteOnly);
    spGameMap pMap = GameMap::getInstance();
    sendStream << NetworkCommands::PLAYERCHANGED;
    sendStream << socketId;
    sendStream << name;
    sendStream << player;
    sendStream << aiType;
    pMap->getPlayer(player)->serializeObject(sendStream);
}

void PlayerSelection::setPlayerAi(qint32 player, GameEnums::AiTypes eAiType, QString name)
{
    if (eAiType != GameEnums::AiTypes_Human)
    {
        if (eAiType == GameEnums::AiTypes_Open)
        {
            m_playerAIs[player]->setCurrentItem(m_playerAIs[player]->getItemCount() - 1);
        }
        else
        {
            setPlayerAiName(player, name);
        }
    }
    else
    {
        m_playerAIs[player]->setCurrentItem(0);
    }
}

void PlayerSelection::recievedPlayerData(quint64, QDataStream& stream)
{
    if (!m_pNetworkInterface->getIsServer() ||
        m_isServerGame)
    {
        spGameMap pMap = GameMap::getInstance();
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = pMap->getPlayer(i);
            qint32 funds = 0;
            float fundsModifier = 0.0f;
            qint32 team = 0;
            stream >> funds;
            stream >> fundsModifier;
            stream >> team;
            pPlayer->setFunds(funds);
            pPlayer->setFundsModifier(fundsModifier);
            pPlayer->setTeam(team);
            QStringList buildList;
            qint32 size = 0;
            stream >> size;
            for (qint32 i2 = 0; i2 < size; i2++)
            {
                QString item;
                stream >> item;
                buildList.append(item);
            }
            pPlayer->setBuildList(buildList);
            updatePlayerData(i);
        }
    }
}

void PlayerSelection::recievedCOData(quint64, QDataStream& stream)
{
    spGameMap pMap = GameMap::getInstance();
    qint32 playerIdx;
    QString coid;
    stream >> playerIdx;
    stream >> coid;
    QStringList perks = Filesupport::readVectorList<QString, QList>(stream);
    pMap->getPlayer(playerIdx)->setCO(coid, 0);
    CO* pCO = pMap->getPlayer(playerIdx)->getCO(0);
    if (pCO != nullptr)
    {
        pCO->setPerkList(perks);
        pCO->readCoStyleFromStream(stream);
    }
    updateCO1Sprite(coid, playerIdx);
    stream >> coid;
    perks = Filesupport::readVectorList<QString, QList>(stream);
    pMap->getPlayer(playerIdx)->setCO(coid, 1);
    pCO = pMap->getPlayer(playerIdx)->getCO(1);
    if (pCO != nullptr)
    {
        pCO->setPerkList(perks);
        pCO->readCoStyleFromStream(stream);
    }
    updateCO2Sprite(coid, playerIdx);
    
}

void PlayerSelection::recievedColorData(quint64, QDataStream& stream)
{
    spGameMap pMap = GameMap::getInstance();
    qint32 playerIdx = 0;
    QColor color;
    stream >> playerIdx;
    stream >> color;
    Player* pPlayer = pMap->getPlayer(playerIdx);
    pPlayer->setColor(color);
    m_playerColors[playerIdx]->setCurrentItem(color);
    
}

void PlayerSelection::recievePlayerArmy(quint64, QDataStream& stream)
{
    spGameMap pMap = GameMap::getInstance();
    qint32 playerIdx = 0;
    QString army;
    stream >> playerIdx;
    stream >> army;
    Player* pPlayer = pMap->getPlayer(playerIdx);
    if (army == CO_ARMY)
    {
        pPlayer->setPlayerArmySelected(false);
    }
    else
    {
        pPlayer->setPlayerArmy(army);
        pPlayer->setPlayerArmySelected(true);
    }
    m_playerArmy[playerIdx]->setCurrentItem(army);
    
}

void PlayerSelection::disconnected(quint64 socketID)
{
    if (m_pNetworkInterface->getIsServer())
    {
        Console::print("Reopening players for socket " + QString::number(socketID) + " after disconnecting", Console::eLogLevels::eDEBUG);
        // handle disconnect of clients here
        for (qint32 i = 0; i < m_PlayerSockets.size(); i++)
        {
            // this player has disconnect reopen him
            if (m_PlayerSockets[i] == socketID &&
                m_playerAIs[i]->getCurrentItem() != m_playerAIs[i]->getItemCount() - 1)
            {
                // reopen all players
                m_playerAIs[i]->setCurrentItem(m_playerAIs[i]->getItemCount() - 1);
                selectAI(i);

            }
        }
        if (Mainapp::getSlave())
        {
            dynamic_cast<LocalServer*>(m_pNetworkInterface.get())->removeSocket(socketID);
        }
    }
}

void PlayerSelection::updatePlayerData(qint32 player)
{
    spGameMap pMap = GameMap::getInstance();
    if (player < 0)
    {
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            updatePlayerData(i);
        }
    }
    else
    {
        Player* pPlayer = pMap->getPlayer(player);
        CO* pCO = pPlayer->getCO(0);
        QString coid = "no_co";
        if (pCO != nullptr)
        {
            coid = pCO->getCoID();
        }
        updateCO1Sprite(coid, player);
        pCO = pPlayer->getCO(1);
        coid = "no_co";
        if (pCO != nullptr)
        {
            coid = pCO->getCoID();
        }
        updateCO2Sprite(coid, player);
        m_playerColors[player]->setCurrentItem(pPlayer->getColor());
        m_playerStartFunds[player]->setCurrentValue(pPlayer->getFunds());
        m_playerIncomes[player]->setCurrentValue(pPlayer->getFundsModifier());
        m_playerTeams[player]->setCurrentItem(pPlayer->getTeam());
        // check for open player
        bool notServerChangeAblePlayer = false;
        if (pPlayer->getBaseGameInput() != nullptr)
        {
            notServerChangeAblePlayer = (m_pNetworkInterface->getIsServer() && pPlayer->getBaseGameInput()->getAiType() != GameEnums::AiTypes_ProxyAi);
        }
        if (pPlayer->getBaseGameInput() == nullptr ||
            pPlayer->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human ||
            notServerChangeAblePlayer)
        {
            m_playerAIs[player]->setEnabled(true);
            if (((pPlayer->getBaseGameInput() != nullptr &&
                  pPlayer->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human) ||
                 notServerChangeAblePlayer) &&
                !saveGame)
            {
                m_playerColors[player]->setEnabled(true);
                m_playerCO1[player]->setEnabled(true);
                m_playerCO2[player]->setEnabled(true);
                m_playerPerks[player]->setEnabled(true);
                m_playerArmy[player]->setEnabled(true);
            }
            else
            {
                m_playerColors[player]->setEnabled(false);
                m_playerCO1[player]->setEnabled(false);
                m_playerCO2[player]->setEnabled(false);
                m_playerPerks[player]->setEnabled(false);
                m_playerArmy[player]->setEnabled(false);
            }

            if (pPlayer->getBaseGameInput() != nullptr &&
                pPlayer->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human)
            {
                m_pReadyBoxes[player]->setChecked(m_PlayerReady);
            }
            else
            {
                m_pReadyBoxes[player]->setChecked(false);
            }
        }
        else
        {
            if ((m_pNetworkInterface->getIsServer() || m_isServerGame) &&
                !saveGame)
            {
                m_playerAIs[player]->setEnabled(true);
                if (m_playerAIs[player]->getCurrentItem() >= 0)
                {
                    m_playerColors[player]->setEnabled(true);
                    m_playerCO1[player]->setEnabled(true);
                    m_playerCO2[player]->setEnabled(true);
                    m_playerPerks[player]->setEnabled(true);
                    m_playerArmy[player]->setEnabled(true);
                }
                else
                {
                    m_playerColors[player]->setEnabled(false);
                    m_playerCO1[player]->setEnabled(false);
                    m_playerCO2[player]->setEnabled(false);
                    m_playerPerks[player]->setEnabled(false);
                    m_playerArmy[player]->setEnabled(false);
                }
            }
            else
            {
                m_playerAIs[player]->setEnabled(false);
                m_playerColors[player]->setEnabled(false);
                m_playerCO1[player]->setEnabled(false);
                m_playerCO2[player]->setEnabled(false);
                m_playerPerks[player]->setEnabled(false);
                m_playerArmy[player]->setEnabled(false);
            }
        }
        if (m_pCampaign.get() != nullptr)
        {
            if (pPlayer->getBaseGameInput()->getAiType() != GameEnums::AiTypes_Human)
            {
                m_playerAIs[player]->setEnabled(false);
                m_playerCO1[player]->setEnabled(false);
                m_playerCO2[player]->setEnabled(false);
                m_playerPerks[player]->setEnabled(false);
                m_playerArmy[player]->setEnabled(false);
            }
            m_playerColors[player]->setEnabled(false);
        }
    }
    
}

bool PlayerSelection::getReady(qint32 playerIdx)
{
    if (playerIdx >= 0 && playerIdx < m_pReadyBoxes.size())
    {
        return m_pReadyBoxes[playerIdx]->getChecked();
    }
    return false;
}

void PlayerSelection::setPlayerReady(bool value)
{
    m_PlayerReady = value;
    for (qint32 i = 0; i < m_playerAIs.size(); i++)
    {
        if (static_cast<GameEnums::AiTypes>(m_playerAIs[i]->getCurrentItem()) >= GameEnums::AiTypes_Human &&
            static_cast<GameEnums::AiTypes>(m_playerAIs[i]->getCurrentItem()) != GameEnums::AiTypes_Open)
        {
            m_pReadyBoxes[i]->setChecked(value);
        }
    }
}

bool PlayerSelection::getPlayerReady()
{
    return m_PlayerReady;
}
