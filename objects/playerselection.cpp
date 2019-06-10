#include "playerselection.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/co.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/cospritemanager.h"

#include "objects/buildlistdialog.h"
#include "objects/coselectiondialog.h"

#include "gameinput/humanplayerinput.h"
#include "ai/veryeasyai.h"

#include "multiplayer/networkcommands.h"

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
    return true;
}

QString PlayerSelection::getPlayerAiName(qint32 player)
{
    return m_playerAIs[player]->getCurrentItemText();
}

void PlayerSelection::setPlayerAiName(qint32 player, QString name)
{
    return m_playerAIs[player]->setCurrentItemText(name);
}

BaseGameInputIF::AiTypes PlayerSelection::getPlayerAiType(qint32 player)
{
    if (m_pNetworkInterface.get() == nullptr)
    {
        return static_cast<BaseGameInputIF::AiTypes>(m_playerAIs[player]->getCurrentItem());
    }
    else
    {
        if (m_playerAIs[player]->getCurrentItem() == m_playerAIs[player]->getItemCount() - 1)
        {
            return BaseGameInputIF::AiTypes::Open;
        }
        else if (m_playerAIs[player]->getCurrentItem() < 0)
        {
            return BaseGameInputIF::AiTypes::ProxyAi;
        }
        else
        {
            return static_cast<BaseGameInputIF::AiTypes>(m_playerAIs[player]->getCurrentItem());
        }
    }
}


void PlayerSelection::resetPlayerSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pPlayerSelection->setVisible(false);
    m_pPlayerSelection->clearContent();
    m_playerCO1.clear();
    m_playerCO2.clear();
    m_playerColors.clear();
    m_playerIncomes.clear();
    m_playerStartFonds.clear();
    m_playerAIs.clear();
    m_playerTeams.clear();
    m_playerBuildlist.clear();
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
    QStringList items = {tr("CO's"), tr("Color"), tr("AI Strength"), tr("Startfonds"), tr("Income Modifier"), tr("Team"), tr("Build List")};
    if (m_pNetworkInterface.get() != nullptr)
    {
        items.append(tr("Connected"));
    }
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

    qint32 itemIndex = 0;
    oxygine::spButton pButtonAllCOs = ObjectManager::createButton(tr("All Random"));
    pButtonAllCOs->setPosition(xPositions[itemIndex] - 40, y);
    m_pPlayerSelection->addItem(pButtonAllCOs);
    pButtonAllCOs->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonAllCOsRandom();
    });
    connect(this, &PlayerSelection::buttonAllCOsRandom, this, &PlayerSelection::slotAllCOsRandom, Qt::QueuedConnection);
    if (m_pNetworkInterface.get() != nullptr ||
        m_pCampaign.get() != nullptr)
    {
        pButtonAllCOs->setVisible(false);
    }

    itemIndex = 3;
    spSpinBox allStartFondsSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 100000);
    allStartFondsSpinBox->setPosition(xPositions[itemIndex], y);
    allStartFondsSpinBox->setCurrentValue(0);
    m_pPlayerSelection->addItem(allStartFondsSpinBox);
    connect(allStartFondsSpinBox.get(), &SpinBox::sigValueChanged, this, &PlayerSelection::allPlayerStartFondsChanged, Qt::QueuedConnection);
    if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
        saveGame ||
        m_pCampaign.get() != nullptr)
    {
        allStartFondsSpinBox->setEnabled(false);
    }

    itemIndex = 4;
    spSpinBox allIncomeSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 10, SpinBox::Mode::Float);
    allIncomeSpinBox->setPosition(xPositions[itemIndex], y);
    allIncomeSpinBox->setCurrentValue(1.0f);
    allIncomeSpinBox->setSpinSpeed(0.1f);
    m_pPlayerSelection->addItem(allIncomeSpinBox);
    connect(allIncomeSpinBox.get(), &SpinBox::sigValueChanged, this, &PlayerSelection::allPlayerIncomeChanged, Qt::QueuedConnection);
    if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
        saveGame ||
        m_pCampaign.get() != nullptr)
    {
        allIncomeSpinBox->setEnabled(false);
    }

    itemIndex = 6;
    oxygine::spButton pButtonAllBuildList = ObjectManager::createButton(tr("Build List"), 120);
    pButtonAllBuildList->setPosition(xPositions[itemIndex], y);
    m_pPlayerSelection->addItem(pButtonAllBuildList);
    pButtonAllBuildList->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit buttonShowAllBuildList();
    });
    connect(this, &PlayerSelection::buttonShowAllBuildList, this, &PlayerSelection::slotShowAllBuildList, Qt::QueuedConnection);
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
    QVector<QString> defaultAiList = {tr("Human"), tr("Very Easy"), tr("Normal")};
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
                if (pPlayer->getBaseGameInput()->getAiType() > BaseGameInputIF::AiTypes::Human)
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
        itemIndex = 0;
        oxygine::spSprite spriteCO1 = new oxygine::Sprite();
        spriteCO1->setPosition(xPositions[itemIndex], y);
        spriteCO1->setSize(32, 12);
        spriteCO1->setScale(2.0f);
        spriteCO1->setDisableColor(oxygine::Color(0, 0, 0, 0));
        m_pPlayerSelection->addItem(spriteCO1);
        m_playerCO1.append(spriteCO1);

        spriteCO1->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigShowSelectCO(i, 0);
        });
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            saveGame)
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
        spriteCO2->setDisableColor(oxygine::Color(0, 0, 0, 0));
        m_pPlayerSelection->addItem(spriteCO2);
        m_playerCO2.append(spriteCO2);
                spriteCO2->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
        {
            emit sigShowSelectCO(i, 1);
        });
        if ((m_pNetworkInterface.get() != nullptr &&
             !m_pNetworkInterface->getIsServer()) ||
            saveGame)
        {
            spriteCO2->setEnabled(false);
        }
        if (m_pNetworkInterface.get() == nullptr ||
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

        bool up = false;
        if ((pMap->getPlayerCount() - i <= 5) &&
            (i > 5))
        {
            up = true;
        }

        itemIndex++;
        spDropDownmenuColor playerColor = new DropDownmenuColor(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, playerColors, up);
        playerColor->setPosition(xPositions[itemIndex], y);
        playerColor->setCurrentItem(pMap->getPlayer(i)->getColor());
        m_pPlayerSelection->addItem(playerColor);
        connect(playerColor.get(), &DropDownmenuColor::sigItemChanged, this, [=](QColor value)
        {
            playerColorChanged(value, i);
        }, Qt::QueuedConnection);
        m_playerColors.append(playerColor);
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            saveGame ||
            m_pCampaign.get() != nullptr)
        {
            playerColor->setEnabled(false);
        }

        itemIndex++;
        spDropDownmenu playerAi = new DropDownmenu(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, aiList, up);
        playerAi->setPosition(xPositions[itemIndex], y);
        if (m_pCampaign.get() != nullptr)
        {
            qint32 ai = 0;
            if (pMap->getPlayer(i)->getBaseGameInput() != nullptr)
            {
                ai = static_cast<qint32>(pMap->getPlayer(i)->getBaseGameInput()->getAiType());
            }
            if (ai == 0)
            {
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
                if (i > 0)
                {
                    playerAi->setCurrentItem(aiList.size() - 1);
                }
                else
                {
                    playerAi->setCurrentItem(0);
                }
            }
            else
            {
                playerAi->setCurrentItem(aiList.size() - 1);
                playerAi->setEnabled(false);
            }
        }
        else if (allHuman)
        {
            if (i > 0)
            {
                playerAi->setCurrentItem(aiList.size() - 1);

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
        createAi(i, static_cast<BaseGameInputIF::AiTypes>(m_playerAIs[i]->getCurrentItem()));
        m_PlayerSockets.append(0);

        itemIndex++;
        spSpinBox playerStartFondsSpinBox = new SpinBox(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 100000);
        playerStartFondsSpinBox->setPosition(xPositions[itemIndex], y);
        playerStartFondsSpinBox->setCurrentValue(pMap->getPlayer(i)->getFonds());
        m_pPlayerSelection->addItem(playerStartFondsSpinBox);
        m_playerStartFonds.append(playerStartFondsSpinBox);
        connect(playerStartFondsSpinBox.get(), &SpinBox::sigValueChanged, this, [=](float value)
        {
            playerStartFondsChanged(value, i);
        }, Qt::QueuedConnection);
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            saveGame ||
            m_pCampaign.get() != nullptr)
        {
            playerStartFondsSpinBox->setEnabled(false);
        }

        itemIndex++;
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
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            saveGame ||
            m_pCampaign.get() != nullptr)
        {
            playerIncomeSpinBox->setEnabled(false);
        }

        itemIndex++;
        spDropDownmenu playerTeam = new DropDownmenu(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, teamList, up);
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
        oxygine::spButton pButtonPlayerBuildList = ObjectManager::createButton(tr("Build List"), 120);
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
            pCheckbox->setPosition(xPositions[itemIndex] + labelminStepSize / 2 - pCheckbox->getWidth(), y);
            pCheckbox->setEnabled(false);
            m_pReadyBoxes.append(pCheckbox);
            m_pPlayerSelection->addItem(pCheckbox);
        }

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
    playerDataChanged();
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
    playerDataChanged();
    pApp->continueThread();
}
void PlayerSelection::playerIncomeChanged(float value, qint32 playerIdx)
{
    GameMap* pMap = GameMap::getInstance();
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    pMap->getPlayer(playerIdx)->setFondsModifier(value);
    playerDataChanged();
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
    playerDataChanged();
}

void PlayerSelection::slotChangePlayerBuildList(qint32 player, QStringList buildList)
{
    GameMap* pMap = GameMap::getInstance();
    if (player >= 0 && player < pMap->getPlayerCount())
    {
        pMap->getPlayer(player)->setBuildList(buildList);
    }
    playerDataChanged();
}

void PlayerSelection::playerStartFondsChanged(float value, qint32 playerIdx)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    pMap->getPlayer(playerIdx)->setFonds(static_cast<qint32>(value));
    playerDataChanged();
    pApp->continueThread();
}
void PlayerSelection::playerTeamChanged(qint32 value, qint32 playerIdx)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    pMap->getPlayer(playerIdx)->setTeam(value);
    playerDataChanged();
    pApp->continueThread();
}

void PlayerSelection::playerDataChanged()
{
    if (m_pNetworkInterface.get() != nullptr &&
        m_pNetworkInterface->getIsServer())
    {
            GameMap* pMap = GameMap::getInstance();
            QByteArray sendData;
            QDataStream sendStream(&sendData, QIODevice::WriteOnly);
            sendStream << NetworkCommands::PLAYERDATA;
            for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
            {
                Player* pPlayer = pMap->getPlayer(i);
                sendStream << pPlayer->getFonds();
                sendStream << pPlayer->getFondsModifier();
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

void PlayerSelection::playerColorChanged(QColor value, qint32 playerIdx)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    pMap->getPlayer(playerIdx)->setColor(value);
    if (m_pNetworkInterface.get() != nullptr)
    {
        GameMap* pMap = GameMap::getInstance();
        Player* pPlayer = pMap->getPlayer(playerIdx);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << NetworkCommands::COLORDATA;
        sendStream << playerIdx;
        sendStream << pPlayer->getColor();
        m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, true);
    }
    pApp->continueThread();
}

void PlayerSelection::playerCO1Changed(QString coid, qint32 playerIdx)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    pMap->getPlayer(playerIdx)->setCO(coid, 0);
    updateCO1Sprite(coid, playerIdx);
    m_pPlayerSelection->setVisible(true);
    updateCOData(playerIdx);
    pApp->continueThread();
}
void PlayerSelection::updateCO1Sprite(QString coid, qint32 playerIdx)
{
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
}
void PlayerSelection::playerCO2Changed(QString coid, qint32 playerIdx)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    pMap->getPlayer(playerIdx)->setCO(coid, 1);
    updateCO2Sprite(coid, playerIdx);
    m_pPlayerSelection->setVisible(true);
    updateCOData(playerIdx);
    pApp->continueThread();
}
void PlayerSelection::updateCO2Sprite(QString coid, qint32 playerIdx)
{
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
}

void PlayerSelection::updateCOData(qint32 playerIdx)
{
    if (m_pNetworkInterface.get() != nullptr)
    {
        GameMap* pMap = GameMap::getInstance();
        Player* pPlayer = pMap->getPlayer(playerIdx);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << NetworkCommands::CODATA;
        sendStream << playerIdx;
        CO* pCO = pPlayer->getCO(0);
        QString coid = "no_co";
        if (pCO != nullptr)
        {
            coid = pCO->getCoID();
        }
        sendStream << coid;
        pCO = pPlayer->getCO(1);
        coid = "no_co";
        if (pCO != nullptr)
        {
            coid = pCO->getCoID();
        }
        sendStream << coid;
        m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, true);
    }
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

void PlayerSelection::selectAI(qint32 player)
{
    if (m_pNetworkInterface.get() != nullptr)
    {
        BaseGameInputIF::AiTypes type = static_cast<BaseGameInputIF::AiTypes>(m_playerAIs[player]->getCurrentItem());
        if (isOpenPlayer(player))
        {
            type = BaseGameInputIF::AiTypes::Open;
        }
        if (m_pNetworkInterface->getIsServer())
        {
            m_PlayerSockets[player] = 0;
            GameMap* pMap = GameMap::getInstance();
            createAi(player, static_cast<BaseGameInputIF::AiTypes>(m_playerAIs[player]->getCurrentItem()));
            QByteArray data;
            QDataStream stream(&data, QIODevice::WriteOnly);
            stream << NetworkCommands::PLAYERCHANGED;
            if (type == BaseGameInputIF::AiTypes::Human)
            {
                stream << Settings::getUsername();
            }
            else
            {
                stream << m_playerAIs[player]->getCurrentItemText();
            }
            stream << player;
            if (type == BaseGameInputIF::AiTypes::Open)
            {
                stream << static_cast<qint32>(BaseGameInputIF::AiTypes::Open);
            }
            else
            {
                stream << static_cast<qint32>(BaseGameInputIF::AiTypes::ProxyAi);
            }
            pMap->getPlayer(player)->serializeObject(stream);
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
        createAi(player, static_cast<BaseGameInputIF::AiTypes>(m_playerAIs[player]->getCurrentItem()));
    }
}

void PlayerSelection::createAi(qint32 player, BaseGameInputIF::AiTypes type)
{
    Player* pPlayer = GameMap::getInstance()->getPlayer(player);
    pPlayer->setBaseGameInput(BaseGameInputIF::createAi(type));
}

void PlayerSelection::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    // data for us?
    if (service == NetworkInterface::NetworkSerives::Multiplayer)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        if (messageType == NetworkCommands::REQUESTPLAYER)
        {
            requestPlayer(socketID, stream);
        }
        else if (messageType == NetworkCommands::PLAYERCHANGED)
        {
            changePlayer(socketID, stream);
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
        else if (messageType == NetworkCommands::CLIENTREADY)
        {
            recievePlayerReady(socketID, stream);
        }
        else if (messageType == NetworkCommands::SERVERREADY)
        {
            recievePlayerServerReady(socketID, stream);
        }
    }
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

void PlayerSelection::sendPlayerReady(quint64 socketID, QVector<qint32> player, bool value)
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
        dynamic_cast<TCPServer*>(m_pNetworkInterface.get())->sigForwardData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer);
    }
}

void PlayerSelection::recievePlayerServerReady(quint64, QDataStream& stream)
{
    if (!m_pNetworkInterface->getIsServer())
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

void PlayerSelection::sendPlayerRequest(quint64 socketID, qint32 player, BaseGameInputIF::AiTypes aiType)
{
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream << NetworkCommands::REQUESTPLAYER;
    // request player smaller 0 for any (the first avaible on the server :)
    sendStream << static_cast<qint32>(player);
    sendStream << Settings::getUsername();
    sendStream << static_cast<qint32>(aiType);
    m_pNetworkInterface->sig_sendData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
}

void PlayerSelection::requestPlayer(quint64 socketID, QDataStream& stream)
{
    if (m_pNetworkInterface->getIsServer())
    {
        GameMap* pMap = GameMap::getInstance();
        qint32 player = 0;
        QString username;
        stream >> player;
        stream >> username;
        qint32 aiType;
        stream >> aiType;
        BaseGameInputIF::AiTypes eAiType = static_cast<BaseGameInputIF::AiTypes>(aiType);
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
        if (isOpenPlayer(player) || eAiType == BaseGameInputIF::AiTypes::Open)
        {
            // valid request
            // change data locally and send remote update
            Player* pPlayer = pMap->getPlayer(player);
            // we need to handle opening a player slightly different here...
            if (eAiType == BaseGameInputIF::AiTypes::Open)
            {
                pPlayer->setBaseGameInput(nullptr);
                m_playerAIs[player]->setCurrentItem(m_playerAIs[player]->getItemCount() - 1);
                m_PlayerSockets[player] = 0;
            }
            else
            {
                pPlayer->setBaseGameInput(BaseGameInputIF::createAi(BaseGameInputIF::AiTypes::ProxyAi));
                m_playerAIs[player]->setCurrentItemText(username);
                m_PlayerSockets[player] = socketID;
            }
            updatePlayerData(player);

            QByteArray sendDataRequester;
            QDataStream sendStreamRequester(&sendDataRequester, QIODevice::WriteOnly);
            QByteArray sendDataOtherClients;
            QDataStream sendStreamOtherClients(&sendDataOtherClients, QIODevice::WriteOnly);
            // create data block for requester
            sendStreamRequester << NetworkCommands::PLAYERCHANGED;
            sendStreamRequester << username;
            sendStreamRequester << player;
            if (eAiType == BaseGameInputIF::AiTypes::Open)
            {
                sendStreamRequester << static_cast<qint32>(BaseGameInputIF::AiTypes::Open);
            }
            else
            {
                sendStreamRequester << static_cast<qint32>(BaseGameInputIF::AiTypes::Human);
            }
            pMap->getPlayer(player)->serializeObject(sendStreamRequester);
            // create data block for other clients
            sendStreamOtherClients << NetworkCommands::PLAYERCHANGED;
            sendStreamOtherClients << username;
            sendStreamOtherClients << player;
            if (eAiType == BaseGameInputIF::AiTypes::Open)
            {
                sendStreamOtherClients << static_cast<qint32>(BaseGameInputIF::AiTypes::Open);
            }
            else
            {
                sendStreamOtherClients << static_cast<qint32>(BaseGameInputIF::AiTypes::ProxyAi);
            }
            pMap->getPlayer(player)->serializeObject(sendStreamOtherClients);
            // send player update
            m_pNetworkInterface->sig_sendData(socketID, sendDataRequester, NetworkInterface::NetworkSerives::Multiplayer, false);
            emit dynamic_cast<TCPServer*>(m_pNetworkInterface.get())->sigForwardData(socketID, sendDataOtherClients, NetworkInterface::NetworkSerives::Multiplayer);
        }
        else
        {
            Player* pPlayer = pMap->getPlayer(player);
            QByteArray sendData;
            QDataStream sendStream(&sendData, QIODevice::WriteOnly);
            sendStream << NetworkCommands::PLAYERCHANGED;
            if (pPlayer->getBaseGameInput()->getAiType() == BaseGameInputIF::AiTypes::Human)
            {
                sendStream << Settings::getUsername();
            }
            else
            {
                sendStream << m_playerAIs[player]->getCurrentItemText();
            }
            sendStream << player;
            sendStream << static_cast<qint32>(BaseGameInputIF::AiTypes::ProxyAi);
            pPlayer->serializeObject(sendStream);
        }
    }
}

void PlayerSelection::changePlayer(quint64, QDataStream& stream)
{
    if (!m_pNetworkInterface->getIsServer())
    {
        GameMap* pMap = GameMap::getInstance();
        QString name;
        qint32 aiType;
        qint32 player;
        stream >> name;
        stream >> player;
        stream >> aiType;
        BaseGameInputIF::AiTypes eAiType = static_cast<BaseGameInputIF::AiTypes>(aiType);
        if (eAiType != BaseGameInputIF::AiTypes::Human)
        {
            if (eAiType == BaseGameInputIF::AiTypes::Open)
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
        pMap->getPlayer(player)->deserializeObject(stream);
        pMap->getPlayer(player)->setBaseGameInput(BaseGameInputIF::createAi(eAiType));

        bool humanFound = false;
        for (qint32 i = 0; i < m_playerAIs.size(); i++)
        {
            if (static_cast<BaseGameInputIF::AiTypes>(m_playerAIs[i]->getCurrentItem()) == BaseGameInputIF::AiTypes::Human)
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
    }
}

void PlayerSelection::recievedPlayerData(quint64, QDataStream& stream)
{
    if (!m_pNetworkInterface->getIsServer())
    {
        GameMap* pMap = GameMap::getInstance();
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = pMap->getPlayer(i);
            qint32 fonds = 0;
            float fondsModifier = 0.0f;
            qint32 team = 0;
            stream >> fonds;
            stream >> fondsModifier;
            stream >> team;
            pPlayer->setFonds(fonds);
            pPlayer->setFondsModifier(fondsModifier);
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
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    qint32 playerIdx;
    QString coid;
    stream >> playerIdx;
    stream >> coid;
    pMap->getPlayer(playerIdx)->setCO(coid, 0);
    updateCO1Sprite(coid, playerIdx);
    stream >> coid;
    pMap->getPlayer(playerIdx)->setCO(coid, 1);
    updateCO2Sprite(coid, playerIdx);
    pApp->continueThread();
}

void PlayerSelection::recievedColorData(quint64, QDataStream& stream)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    qint32 playerIdx = 0;
    QColor color;
    stream >> playerIdx;
    stream >> color;
    Player* pPlayer = pMap->getPlayer(playerIdx);
    pPlayer->setColor(color);
    m_playerColors[playerIdx]->setCurrentItem(color);
    pApp->continueThread();
}

void PlayerSelection::disconnected(quint64 socketID)
{
    if (m_pNetworkInterface->getIsServer())
    {
        // handle disconnect of clients here
        for (qint32 i = 0; i < m_PlayerSockets.size(); i++)
        {
            // this player has disconnect reopen him
            if (m_PlayerSockets[i] == socketID)
            {
                // reopen all players
                m_playerAIs[i]->setCurrentItem(m_playerAIs[i]->getItemCount() - 1);
                selectAI(i);
            }
        }
    }
}

void PlayerSelection::updatePlayerData(qint32 player)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
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
        m_playerStartFonds[player]->setCurrentValue(pPlayer->getFonds());
        m_playerIncomes[player]->setCurrentValue(pPlayer->getFondsModifier());
        m_playerTeams[player]->setCurrentItem(pPlayer->getTeam());
        // check for open player
        if (pPlayer->getBaseGameInput() == nullptr ||
            pPlayer->getBaseGameInput()->getAiType() == BaseGameInputIF::AiTypes::Human)
        {
            m_playerAIs[player]->setEnabled(true);
            if (((pPlayer->getBaseGameInput() != nullptr &&
                pPlayer->getBaseGameInput()->getAiType() == BaseGameInputIF::AiTypes::Human) ||
                m_pNetworkInterface->getIsServer()) &&
                !saveGame)
            {
                m_playerColors[player]->setEnabled(true);
                m_playerCO1[player]->setEnabled(true);
                m_playerCO2[player]->setEnabled(true);

            }
            else
            {
                m_playerColors[player]->setEnabled(false);
                m_playerCO1[player]->setEnabled(false);
                m_playerCO2[player]->setEnabled(false);
            }

            if (pPlayer->getBaseGameInput() != nullptr &&
                pPlayer->getBaseGameInput()->getAiType() == BaseGameInputIF::AiTypes::Human)
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
            if (m_pNetworkInterface->getIsServer() && !saveGame)
            {
                m_playerAIs[player]->setEnabled(true);
                if (m_playerAIs[player]->getCurrentItem() >= 0)
                {
                    m_playerColors[player]->setEnabled(true);
                    m_playerCO1[player]->setEnabled(true);
                    m_playerCO2[player]->setEnabled(true);
                }
                else
                {
                    m_playerColors[player]->setEnabled(false);
                    m_playerCO1[player]->setEnabled(false);
                    m_playerCO2[player]->setEnabled(false);
                }
            }
            else
            {
                m_playerAIs[player]->setEnabled(false);
                m_playerColors[player]->setEnabled(false);
                m_playerCO1[player]->setEnabled(false);
                m_playerCO2[player]->setEnabled(false);
            }
        }
        if (m_pCampaign.get() != nullptr)
        {
            if (pPlayer->getBaseGameInput()->getAiType() != BaseGameInputIF::AiTypes::Human)
            {
                m_playerAIs[player]->setEnabled(false);
                m_playerCO1[player]->setEnabled(false);
                m_playerCO2[player]->setEnabled(false);
            }
            m_playerColors[player]->setEnabled(false);
        }
    }
    pApp->continueThread();
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
        if (static_cast<BaseGameInputIF::AiTypes>(m_playerAIs[i]->getCurrentItem()) >= BaseGameInputIF::AiTypes::Human &&
            static_cast<BaseGameInputIF::AiTypes>(m_playerAIs[i]->getCurrentItem()) != BaseGameInputIF::AiTypes::Open)
        {
            m_pReadyBoxes[i]->setChecked(value);
        }
    }
}

bool PlayerSelection::getPlayerReady()
{
    return m_PlayerReady;
}
