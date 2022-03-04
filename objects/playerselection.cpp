#include <QJsonDocument>
#include <QJsonObject>

#include "objects/playerselection.h"

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

#include "network/tcpserver.h"
#include "network/JsonKeys.h"

constexpr const char* const CO_ARMY = "CO_ARMY";

PlayerSelection::PlayerSelection(qint32 width, qint32 heigth)
{
    setObjectName("PlayerSelection");
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());


    m_pPlayerSelection = spPanel::create(true,
                                         QSize(width,
                                               heigth),
                                         QSize(Settings::getWidth() - 70, 100));
    connect(this, &PlayerSelection::sigShowSelectCO, this, &PlayerSelection::showSelectCO, Qt::QueuedConnection);
    connect(this, &PlayerSelection::sigShowSelectCOPerks, this, &PlayerSelection::showSelectCOPerks, Qt::QueuedConnection);
    connect(this, &PlayerSelection::buttonShowPlayerBuildList, this, &PlayerSelection::slotShowPlayerBuildList, Qt::QueuedConnection);
    connect(this, &PlayerSelection::sigAiChanged, this, &PlayerSelection::selectAI, Qt::QueuedConnection);
    connect(this, &PlayerSelection::sigCOsRandom, this, &PlayerSelection::slotCOsRandom, Qt::QueuedConnection);
    connect(this, &PlayerSelection::sigSelectedArmyChanged, this, &PlayerSelection::selectedArmyChanged, Qt::QueuedConnection);
    connect(this, &PlayerSelection::sigChangeAllTeams, this, &PlayerSelection::changeAllTeams, Qt::QueuedConnection);
    connect(this, &PlayerSelection::buttonShowAllBuildList, this, &PlayerSelection::slotShowAllBuildList, Qt::QueuedConnection);

    addChild(m_pPlayerSelection);
}

PlayerSelection::~PlayerSelection()
{
    for (auto & playerColors : m_playerColors)
    {
        playerColors->detach();
    }
    m_playerColors.clear();
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
    if (m_pNetworkInterface.get() != nullptr &&
        player >= 0 &&
        player < m_playerAIs.size())
    {
        if (m_playerAIs[player]->getCurrentItem() == m_playerAIs[player]->getItemCount() - 1)
        {
            return true;
        }
    }
    return false;
}

bool PlayerSelection::isClosedPlayer(qint32 player)
{
    bool ret = false;
    if (!getIsCampaign())
    {
        if (m_pNetworkInterface.get() != nullptr &&
            player >= 0 &&
            player < m_playerAIs.size())
        {
            if (m_playerAIs[player]->getCurrentItem() == m_playerAIs[player]->getItemCount() - 2 &&
                m_pNetworkInterface->getIsServer())
            {
                ret = true;
            }
            else
            {
                QString aiName = m_playerAIs[player]->getCurrentItemText();
                ret = (aiName == tr("Closed"));
            }
        }
        else if (m_pNetworkInterface.get() == nullptr &&
                 m_playerAIs[player]->getCurrentItem() == m_playerAIs[player]->getItemCount() - 1 &&
                 m_playerAIs[player]->getItemCount() > 1)
        {
            ret = true;
        }
    }
    return ret;
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

bool PlayerSelection::hasHumanPlayer()
{
    for (qint32 i = 0; i < m_playerAIs.size(); i++)
    {
        if (m_pMap->getPlayer(i)->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human)
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
    if (isClosedPlayer(player))
    {
        return GameEnums::AiTypes_Closed;
    }
    else if (m_pNetworkInterface.get() == nullptr)
    {
        return static_cast<GameEnums::AiTypes>(m_playerAIs[player]->getCurrentItem());
    }
    else
    {
        if (isOpenPlayer(player))
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
    
    if (m_pMap->getPlayer(player)->getCO(co) != nullptr)
    {
        coid = m_pMap->getPlayer(player)->getCO(co)->getCoID();
    }
    QStringList cos;
    if (m_pCampaign.get() != nullptr)
    {
        cos = m_pCampaign->getSelectableCOs(m_pMap, player, co);
    }
    else
    {
        cos = m_pMap->getGameRules()->getCOBannlist();
        if (cos.size() == 0)
        {
            cos= COSpriteManager::getInstance()->getLoadedRessources();
        }
        Userdata* pUserdata = Userdata::getInstance();
        auto items = pUserdata->getShopItemsList(GameEnums::ShopItemType_CO, false);
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
        spCOSelectionDialog dialog = spCOSelectionDialog::create(m_pMap, coid, m_pMap->getPlayer(player)->getColor(), player, cos);
        oxygine::Stage::getStage()->addChild(dialog);
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

bool PlayerSelection::getIsCampaign()
{
    return (m_pCampaign.get() != nullptr && !Console::getDeveloperMode());
}

bool PlayerSelection::getIsArmyCustomizationAllowed()
{    
    return (m_pCampaign.get() == nullptr || m_pCampaign->getAllowArmyCustomization(m_pMap) || Console::getDeveloperMode());}

qint32 PlayerSelection::getDefaultColorCount()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "getDefaultPlayerColors";
    QJSValueList args;
    QJSValue ret = pInterpreter->doFunction("PLAYER", function, args);
    qint32 colorCount = 0;
    if (ret.isNumber())
    {
        colorCount = ret.toInt();
    }
    return colorCount;
}

QColor PlayerSelection::getDefaultColor(qint32 index)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "getDefaultColor";
    QJSValueList args({QJSValue(index)});
    QJSValue ret = pInterpreter->doFunction("PLAYER", function, args);
    QColor color(ret.toString());
    return color;
}

QColor PlayerSelection::getDisplayColor(qint32 index, bool & exists)
{
    exists = false;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function = "getDisplayColor";
    QJSValueList args({QJSValue(index)});
    QColor displayColor;
    QJSValue ret = pInterpreter->doFunction("PLAYER", function, args);
    if (ret.isString())
    {
        QString colorName = ret.toString();
        if (!colorName.isEmpty())
        {
            displayColor = QColor(colorName);
            exists = true;
        }
    }
    return displayColor;
}

QColor PlayerSelection::tableColorToDisplayColor(QColor tableColor)
{
    qint32 colorCount = getDefaultColorCount();
    QColor displayColor = tableColor;
    for (qint32 i = 0; i < colorCount; ++i)
    {
        QColor color = getDefaultColor(i);
        if (color == tableColor)
        {
            bool exists = false;
            displayColor = getDisplayColor(i, exists);
            if (!exists)
            {
                displayColor = tableColor;
            }
            break;
        }
    }
    return displayColor;
}

QColor PlayerSelection::displayColorToTableColor(QColor displayColor)
{
    qint32 colorCount = getDefaultColorCount();
    QColor tableColor = displayColor;
    for (qint32 i = 0; i < colorCount; ++i)
    {
        bool exists = false;
        QColor color = getDisplayColor(i, exists);
        if (color == displayColor && exists)
        {
            tableColor = getDefaultColor(i);
            break;
        }
    }
    return tableColor;
}

void PlayerSelection::showPlayerSelection()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    resetPlayerSelection();
    m_pPlayerSelection->setVisible(true);

    
    // font style
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
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
        pLabel = oxygine::spTextField::create();
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
    pLabel = oxygine::spTextField::create();
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
    if (m_pMap->getGameRules()->getSingleCo())
    {
        pButtonAllCOs->setEnabled(false);
        pButtonCOs2->setEnabled(false);
    }
    bool isCampaign = getIsCampaign();
    bool isArmyCustomizationAllowed = getIsArmyCustomizationAllowed();

    if (m_pNetworkInterface.get() != nullptr ||
        isCampaign)
    {
        pButtonAllCOs->setVisible(false);
        pButtonCOs1->setVisible(false);
        pButtonCOs2->setVisible(false);
    }

    itemIndex = 3;
    spSpinBox allStartFundsSpinBox = spSpinBox::create(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 100000);
    allStartFundsSpinBox->setTooltipText(tr("Select with how much funds all player start the game."));
    allStartFundsSpinBox->setSpinSpeed(500);
    allStartFundsSpinBox->setPosition(xPositions[itemIndex], y);
    allStartFundsSpinBox->setCurrentValue(0);
    m_pPlayerSelection->addItem(allStartFundsSpinBox);
    connect(allStartFundsSpinBox.get(), &SpinBox::sigValueChanged, this, &PlayerSelection::allPlayerStartFundsChanged, Qt::QueuedConnection);
    if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
        m_saveGame ||
        isCampaign)
    {
        allStartFundsSpinBox->setEnabled(false);
    }

    itemIndex = 4;
    spSpinBox allIncomeSpinBox = spSpinBox::create(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 10, SpinBox::Mode::Float);
    allIncomeSpinBox->setTooltipText(tr("Select the income modifier for all players. The base income for each building is multiplied with this value. For most buildings this means 1.0 equals 1000 funds and 1.1 equals 1100 funds per building."));
    allIncomeSpinBox->setPosition(xPositions[itemIndex], y);
    allIncomeSpinBox->setCurrentValue(1.0f);
    allIncomeSpinBox->setSpinSpeed(0.1f);
    connect(allIncomeSpinBox.get(), &SpinBox::sigValueChanged, this, &PlayerSelection::allPlayerIncomeChanged, Qt::QueuedConnection);
    m_pPlayerSelection->addItem(allIncomeSpinBox);
    if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
        m_saveGame ||
        isCampaign)
    {
        allIncomeSpinBox->setEnabled(false);
    }

    itemIndex = 5;
    spSpinBox teamSpinBox = spSpinBox::create(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 2, m_pMap->getPlayerCount(), SpinBox::Mode::Int);
    teamSpinBox->setTooltipText(tr("Automatically changes the teams of all players so the teams are equally distributed, according to the team count."
                                   "Teams are assigned in a way that the first turn advantage is the least relevant."));
    teamSpinBox->setPosition(xPositions[itemIndex], y);
    teamSpinBox->setCurrentValue(m_pMap->getPlayerCount());
    teamSpinBox->setSpinSpeed(1.0f);
    connect(teamSpinBox.get(), &SpinBox::sigValueChanged, this, [=](qreal value)
    {
        emit sigChangeAllTeams(static_cast<qint32>(value));
    });
    m_pPlayerSelection->addItem(teamSpinBox);
    if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
        m_saveGame ||
        isCampaign)
    {
        teamSpinBox->setEnabled(false);
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
        m_saveGame ||
        isCampaign)
    {
        pButtonAllBuildList->setEnabled(false);
    }

    y += 10 + allIncomeSpinBox->getHeight();
    QStringList teamList;
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        teamList.append(tr("Team") + " " + QString::number(i + 1));
    }
    QStringList defaultAiList = {tr("Human"), tr("Very Easy"), tr("Normal"), tr("Normal Off."), tr("Normal Def.")}; // // heavy ai disabled cause it's not finished
#if HEAVY_AI
    Interpreter* pInterpreter = Interpreter::getInstance();
    GameManager* pGameManager = GameManager::getInstance();
    // heavy ai enable code
    for (qint32 i = 0; i < pGameManager->getHeavyAiCount(); ++i)
    {
        QString id = pGameManager->getHeavyAiID(i);
        QJSValue aiName = pInterpreter->doFunction(id, "getName");
        defaultAiList.append(aiName.toString());
    }
#endif
    defaultAiList.append(tr("Closed"));

    QStringList aiList = defaultAiList;
    if (isCampaign)
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

    qint32 colorCount = getDefaultColorCount();
    QVector<QColor> playerColors;

    for (qint32 i = 0; i < colorCount; i++)
    {
        bool exists = false;
        playerColors.append(getDisplayColor(i, exists));
    }

    bool allPlayer1 = true;
    bool allHuman = true;
    if (!isCampaign)
    {
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
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
                pPlayer->setBaseGameInput(spHumanPlayerInput::create(m_pMap));
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
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            m_pMap->getPlayer(i)->setTeam(i);
        }
    }

    // add player selection information
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        qint32 ai = 0;
        if (m_pMap->getPlayer(i)->getBaseGameInput() != nullptr)
        {
            ai = static_cast<qint32>(m_pMap->getPlayer(i)->getBaseGameInput()->getAiType());
        }

        itemIndex = 0;
        oxygine::spSprite spriteCO1 = oxygine::spSprite::create();
        spriteCO1->setPosition(xPositions[itemIndex], y);
        spriteCO1->setSize(32, 12);
        spriteCO1->setScale(2.0f);
        spriteCO1->setDisableColor(QColor(0, 0, 0, 0));
        m_pPlayerSelection->addItem(spriteCO1);
        m_playerCO1.append(spriteCO1);

        spriteCO1->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
        {
            emit sigShowSelectCO(i, 0);
        });
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            m_saveGame ||
            (ai > 0 && isCampaign))
        {
            spriteCO1->setEnabled(false);
        }

        if (m_pNetworkInterface.get() == nullptr ||
            m_pNetworkInterface->getIsServer())
        {
            if (m_pMap->getPlayer(i)->getCO(0) != nullptr)
            {
                playerCO1Changed(m_pMap->getPlayer(i)->getCO(0)->getCoID(), i);
            }
            else
            {
                playerCO1Changed("", i);
            }
        }

        oxygine::spSprite spriteCO2 = oxygine::spSprite::create();
        spriteCO2->setPosition(xPositions[itemIndex], y + 24);
        spriteCO2->setSize(32, 12);
        spriteCO2->setScale(2.0f);
        spriteCO2->setDisableColor(QColor(0, 0, 0, 0));
        m_pPlayerSelection->addItem(spriteCO2);
        m_playerCO2.append(spriteCO2);
        spriteCO2->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
        {
            emit sigShowSelectCO(i, 1);
        });
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            m_saveGame ||
            (ai > 0 && isCampaign) ||
            m_pMap->getGameRules()->getSingleCo())
        {
            spriteCO2->setEnabled(false);
        }
        if (m_pMap->getGameRules()->getSingleCo())
        {
            playerCO2Changed("", i);
        }
        else if (m_pNetworkInterface.get() == nullptr ||
                 m_pNetworkInterface->getIsServer())
        {
            if (m_pMap->getPlayer(i)->getCO(1) != nullptr)
            {
                playerCO2Changed(m_pMap->getPlayer(i)->getCO(1)->getCoID(), i);
            }
            else
            {
                playerCO2Changed("", i);
            }
        }

        oxygine::spButton pIconButton = ObjectManager::createIconButton("perk", 36);
        pIconButton->setPosition(xPositions[itemIndex] + 67, y + 3);
        m_pPlayerSelection->addItem(pIconButton);
        m_playerPerks.append(pIconButton);
        pIconButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
        {
            emit sigShowSelectCOPerks(i);
        });
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            m_saveGame ||
            (ai > 0 && isCampaign))
        {
            pIconButton->setEnabled(false);
        }
        //
        createArmySelection(ai, xPositions, y, itemIndex, i);

        itemIndex++;
        spDropDownmenuColor playerColor = spDropDownmenuColor::create(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, playerColors);
        playerColor->setTooltipText(tr("Select the Color for this players army."));
        playerColor->setPosition(xPositions[itemIndex], y);
        playerColor->setCurrentItem(tableColorToDisplayColor(m_pMap->getPlayer(i)->getColor()));
        m_pPlayerSelection->addItem(playerColor);
        DropDownmenuColor* pPlayerColor = playerColor.get();
        connect(playerColor.get(), &DropDownmenuColor::sigItemChanged, this, [=](QColor value)
        {
            playerColorChanged(value, i, pPlayerColor->getCurrentItem());
        }, Qt::QueuedConnection);
        m_playerColors.append(playerColor);
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            m_saveGame ||
            !isArmyCustomizationAllowed)
        {
            playerColor->setEnabled(false);
        }

        itemIndex++;
        spDropDownmenu playerAi = spDropDownmenu::create(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, aiList);
        playerAi->setTooltipText(tr("Select who controls this player."));
        playerAi->setPosition(xPositions[itemIndex], y);
        if (isCampaign)
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
                if (m_saveGame)
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
                    ai = GameEnums::AiTypes_Open;
                    playerAi->setCurrentItem(aiList.size() - 1);
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
            auto* input = m_pMap->getPlayer(i)->getBaseGameInput();
            if (input == nullptr)
            {
                ai = GameEnums::AiTypes_Closed;
                playerAi->setCurrentItem(ai);
            }
            else
            {
                ai = static_cast<qint32>(input->getAiType());
                playerAi->setCurrentItem(ai);
            }
        }
        connect(playerAi.get(), &DropDownmenu::sigItemChanged, this, [=](qint32)
        {
            emit sigAiChanged(i);
        });
        m_playerAIs.append(playerAi);
        m_pPlayerSelection->addItem(playerAi);
        m_PlayerSockets.append(0);

        itemIndex++;
        spSpinBox playerStartFundsSpinBox = spSpinBox::create(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 100000);
        playerStartFundsSpinBox->setTooltipText(tr("Select with how much funds this player starts the game."));
        playerStartFundsSpinBox->setPosition(xPositions[itemIndex], y);
        playerStartFundsSpinBox->setCurrentValue(m_pMap->getPlayer(i)->getFunds());
        playerStartFundsSpinBox->setSpinSpeed(500);
        m_pPlayerSelection->addItem(playerStartFundsSpinBox);
        m_playerStartFunds.append(playerStartFundsSpinBox);
        connect(playerStartFundsSpinBox.get(), &SpinBox::sigValueChanged, this, [=](float value)
        {
            playerStartFundsChanged(value, i);
        }, Qt::QueuedConnection);
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            m_saveGame ||
            isCampaign)
        {
            playerStartFundsSpinBox->setEnabled(false);
        }

        itemIndex++;
        spSpinBox playerIncomeSpinBox = spSpinBox::create(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, 0, 10, SpinBox::Mode::Float);
        playerIncomeSpinBox->setTooltipText(tr("Select the income modifier for this player. The base income for each building is multiplied with this value. For most buildings this means 1.0 equals 1000 funds and 1.1 equals 1100 funds per building."));
        playerIncomeSpinBox->setPosition(xPositions[itemIndex], y);
        playerIncomeSpinBox->setCurrentValue(m_pMap->getPlayer(i)->getFundsModifier());
        playerIncomeSpinBox->setSpinSpeed(0.1f);
        m_pPlayerSelection->addItem(playerIncomeSpinBox);
        m_playerIncomes.append(playerIncomeSpinBox);
        connect(playerIncomeSpinBox.get(), &SpinBox::sigValueChanged, this, [=](float value)
        {
            playerIncomeChanged(value, i);
        }, Qt::QueuedConnection);
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            m_saveGame ||
            isCampaign)
        {
            playerIncomeSpinBox->setEnabled(false);
        }

        itemIndex++;
        spDropDownmenu playerTeam = spDropDownmenu::create(xPositions[itemIndex + 1] - xPositions[itemIndex] - 10, teamList);
        playerTeam->setTooltipText(tr("Select the team of this player. The team can't be changed and players of the same team can't attack each other."));
        playerTeam->setPosition(xPositions[itemIndex], y);
        playerTeam->setCurrentItem(m_pMap->getPlayer(i)->getTeam());
        m_pPlayerSelection->addItem(playerTeam);
        connect(playerTeam.get(), &DropDownmenu::sigItemChanged, this, [=](qint32 value)
        {
            playerTeamChanged(value, i);
        }, Qt::QueuedConnection);
        m_playerTeams.append(playerTeam);
        if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
            m_saveGame ||
            isCampaign)
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
            m_saveGame ||
            isCampaign)
        {
            pButtonPlayerBuildList->setEnabled(false);
        }

        itemIndex++;
        if (m_pNetworkInterface.get() != nullptr)
        {
            spCheckbox pCheckbox = spCheckbox::create();
            pCheckbox->setTooltipText(tr("Shows which player is ready to start the game. All players need to be checked in order to start a game."));
            pCheckbox->setPosition(xPositions[itemIndex] + labelminStepSize / 2 - pCheckbox->getWidth(), y);
            pCheckbox->setEnabled(false);
            m_pReadyBoxes.append(pCheckbox);
            m_pPlayerSelection->addItem(pCheckbox);
        }
        if (isCampaign && ai > 0)
        {
            createAi(i, static_cast<GameEnums::AiTypes>(ai), playerAi->getCurrentItemText());
        }
        else if ((m_pNetworkInterface.get() == nullptr ||
                  m_pNetworkInterface->getIsServer() ||
                  m_isServerGame))
        {
            selectPlayerAi(i, static_cast<GameEnums::AiTypes>(ai));
        }
        else
        {
            createAi(i, static_cast<GameEnums::AiTypes>(ai), playerAi->getCurrentItemText());
        }
        y += 15 + playerIncomeSpinBox->getHeight();
    }
    m_pPlayerSelection->setContentHeigth(y + 50);
    pApp->continueRendering();
}

void PlayerSelection::createArmySelection(qint32 ai, const QVector<qint32> & xPositions, qint32 y, qint32 itemIndex, qint32 player)
{
    auto creator = [](QString army)
    {
        GameManager* pGameManager = GameManager::getInstance();
        oxygine::ResAnim* pAnim = pGameManager->getResAnim("icon_" + army.toLower());
        oxygine::spSprite ret = oxygine::spSprite::create();
        ret->setResAnim(pAnim);
        return ret;
    };
    QStringList armies = getSelectableArmies();
    spDropDownmenuSprite pArmy = spDropDownmenuSprite::create(105, armies, creator);
    pArmy->setTooltipText(tr("Selects the army for the player. CO means the army of the first CO is selected."));
    m_pPlayerSelection->addItem(pArmy);
    m_playerArmy.append(pArmy);
    DropDownmenuSprite* pPtrArmy = pArmy.get();
    connect(pArmy.get(), &DropDownmenuSprite::sigItemChanged, this, [=](qint32)
    {
        emit sigSelectedArmyChanged(player, pPtrArmy->getCurrentItemText());
    });
    if ((m_pNetworkInterface.get() != nullptr && !m_pNetworkInterface->getIsServer()) ||
        m_saveGame ||
        (ai > 0 && !getIsArmyCustomizationAllowed()))
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
    
    
    Player* pPlayer = m_pMap->getPlayer(player);
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
        QString command = QString(NetworkCommands::PLAYERARMY);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << command;
        sendStream << player;
        sendStream << army;
        emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, true);
    }
    
}

void PlayerSelection::allPlayerIncomeChanged(float value)
{    
    
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        m_pMap->getPlayer(i)->setFundsModifier(value);
        m_playerIncomes[i]->setCurrentValue(value);
    }
    playerDataChanged();
}

void PlayerSelection::allPlayerStartFundsChanged(float value)
{    
    
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        m_pMap->getPlayer(i)->setFunds(static_cast<qint32>(value));
        m_playerStartFunds[i]->setCurrentValue(value);
    }
    playerDataChanged();
}

void PlayerSelection::playerIncomeChanged(float value, qint32 playerIdx)
{

    m_pMap->getPlayer(playerIdx)->setFundsModifier(value);
    playerDataChanged();
}

void PlayerSelection::slotShowAllBuildList()
{
    // use player 0 as default for showing all
    
    spBuildListDialog dialog = spBuildListDialog::create(m_pMap, 0, m_pMap->getPlayer(0)->getBuildList());
    oxygine::Stage::getStage()->addChild(dialog);
    connect(dialog.get(), &BuildListDialog::editFinished, this , &PlayerSelection::slotChangeAllBuildList, Qt::QueuedConnection);
}

void PlayerSelection::slotShowPlayerBuildList(qint32 player)
{    
    
    spBuildListDialog dialog = spBuildListDialog::create(m_pMap, player, m_pMap->getPlayer(player)->getBuildList());
    oxygine::Stage::getStage()->addChild(dialog);
    connect(dialog.get(), &BuildListDialog::editFinished, this , &PlayerSelection::slotChangePlayerBuildList, Qt::QueuedConnection);
}

void PlayerSelection::slotChangeAllBuildList(qint32, QStringList buildList)
{
    
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        m_pMap->getPlayer(i)->setBuildList(buildList);
    }
    playerDataChanged();
}

void PlayerSelection::slotChangePlayerBuildList(qint32 player, QStringList buildList)
{
    
    if (player >= 0 && player < m_pMap->getPlayerCount())
    {
        m_pMap->getPlayer(player)->setBuildList(buildList);
    }
    playerDataChanged();
}

void PlayerSelection::playerStartFundsChanged(float value, qint32 playerIdx)
{    
    
    m_pMap->getPlayer(playerIdx)->setFunds(static_cast<qint32>(value));
    playerDataChanged();
}

void PlayerSelection::playerTeamChanged(qint32 value, qint32 playerIdx)
{    
    
    m_pMap->getPlayer(playerIdx)->setTeam(value);
    playerDataChanged();
}

void PlayerSelection::playerDataChanged()
{
    if (m_pNetworkInterface.get() != nullptr &&
        m_pNetworkInterface->getIsServer())
    {
        QString command = QString(NetworkCommands::PLAYERDATA);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << command;
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
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
        emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
    }
}

void PlayerSelection::playerColorChanged(QColor displayColor, qint32 playerIdx, qint32 item)
{
    QColor tableColor = displayColorToTableColor(displayColor);
    m_pMap->getPlayer(playerIdx)->setColor(tableColor, item);
    if (m_pNetworkInterface.get() != nullptr)
    {
        QString command = QString(NetworkCommands::COLORDATA);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << command;
        sendStream << playerIdx;
        sendStream << displayColor;
        emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, true);
    }
}

void PlayerSelection::playerCO1Changed(QString coid, qint32 playerIdx)
{
    
    if (!m_saveGame)
    {
        CO* pCO = m_pMap->getPlayer(playerIdx)->getCO(1);
        if (coid == "" ||
            coid == CO::CO_RANDOM ||
            pCO == nullptr ||
            pCO->getCoID() != coid)
        {
            QStringList perks;
            CO* pCurrentCO = m_pMap->getPlayer(playerIdx)->getCO(0);
            if (pCurrentCO != nullptr)
            {
                perks = pCurrentCO->getPerkList();
            }
            m_pMap->getPlayer(playerIdx)->setCO(coid, 0);
            pCurrentCO = m_pMap->getPlayer(playerIdx)->getCO(0);
            if (pCurrentCO != nullptr)
            {
                pCurrentCO->setPerkList(perks);
                pCurrentCO->setCoStyleFromUserdata();
            }
            updateCOData(playerIdx);
        }
    }
    updateCO1Sprite(coid, playerIdx);
    if (getIsCampaign() && m_pCampaign->getAutoSelectPlayerColors(m_pMap))
    {
        autoSelectPlayerColors();
    }
    m_pPlayerSelection->setVisible(true);
}

void PlayerSelection::autoSelectPlayerColors()
{
    QVector<qint32> usedColors;
    QVector<qint32> openPlayers;
    
    qint32 playerCount = m_pMap->getPlayerCount();
    if (m_playerColors.size() == playerCount)
    {
        for (qint32 i = 0; i < playerCount; ++i)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
            CO* pCO = pPlayer->getCO(0);
            if (pCO != nullptr)
            {
                Interpreter* pInterpreter = Interpreter::getInstance();
                QJSValueList args({pCO->getCOArmy().toLower()});
                QJSValue erg = pInterpreter->doFunction("PLAYER", "getDisplayColorFromArmy", args);
                if (erg.isNumber())
                {
                    qint32 index = erg.toInt();
                    if (index >= 0 && !usedColors.contains(index))
                    {
                        usedColors.append(index);
                        m_playerColors[i]->setCurrentItem(index);
                        playerColorChanged(m_playerColors[i]->getCurrentItemColor(), i, index);
                    }
                    else
                    {
                        openPlayers.append(i);
                    }
                }
            }
        }
        qint32 colors = m_playerColors[0]->getItemCount();
        for (const auto & openPlayer : qAsConst(openPlayers))
        {
            for (qint32 i = 0; i < colors; ++i)
            {
                if (!usedColors.contains(i))
                {
                    usedColors.append(i);
                    m_playerColors[openPlayer]->setCurrentItem(i);
                    playerColorChanged(m_playerColors[openPlayer]->getCurrentItemColor(), openPlayer, i);
                    break;
                }
            }
        }
    }
}

GameMap *PlayerSelection::getMap() const
{
    return m_pMap;
}

void PlayerSelection::setMap(GameMap *newPMap)
{
    m_pMap = newPMap;
}

void PlayerSelection::updateCO1Sprite(QString coid, qint32 playerIdx)
{
    
    if (m_pMap)
    {
        CO* pCurrentCO = m_pMap->getPlayer(playerIdx)->getCO(0);
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
        if (pAnim != nullptr)
        {
            m_playerCO1[playerIdx]->setScale(2.0f * pAnim->getWidth() / 32.0f);
        }
        else
        {
            m_playerCO1[playerIdx]->setScale(2.0f);
        }
    }
}

void PlayerSelection::playerCO2Changed(QString coid, qint32 playerIdx)
{    
    if (!m_saveGame)
    {
        
        CO* pCO = m_pMap->getPlayer(playerIdx)->getCO(0);
        if (coid == "" ||
            coid == CO::CO_RANDOM ||
            pCO == nullptr ||
            pCO->getCoID() != coid)
        {
            QStringList perks;
            CO* pCurrentCO = m_pMap->getPlayer(playerIdx)->getCO(1);
            if (pCurrentCO != nullptr)
            {
                perks = pCurrentCO->getPerkList();
            }
            m_pMap->getPlayer(playerIdx)->setCO(coid, 1);
            pCurrentCO = m_pMap->getPlayer(playerIdx)->getCO(1);
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
    
    if (m_pMap)
    {
        CO* pCurrentCO = m_pMap->getPlayer(playerIdx)->getCO(1);
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
        if (pAnim != nullptr)
        {
            m_playerCO2[playerIdx]->setScale(2.0f * pAnim->getWidth() / 32.0f);
        }
        else
        {
            m_playerCO2[playerIdx]->setScale(2.0f);
        }
    }
}

void PlayerSelection::updateCOData(qint32 playerIdx)
{
    if (m_pNetworkInterface.get() != nullptr)
    {
        QString command = QString(NetworkCommands::CODATA);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        
        Player* pPlayer = m_pMap->getPlayer(playerIdx);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << command;
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
        emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, true);
    }
}

void PlayerSelection::playerCOCanceled()
{
    m_pPlayerSelection->setVisible(true);
}

void PlayerSelection::slotCOsRandom(qint32 mode)
{
    
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        if ((mode == 0) || mode < 0)
        {
            playerCO1Changed(CO::CO_RANDOM, i);
        }
        if ((mode == 1) || mode < 0)
        {
            playerCO2Changed(CO::CO_RANDOM, i);
        }
    }
}

void PlayerSelection::showSelectCOPerks(qint32 player)
{
    
    Player* pPlayer = m_pMap->getPlayer(player);
    if (pPlayer->getCO(0) != nullptr || pPlayer->getCO(1) != nullptr)
    {
        Userdata* pUserdata = Userdata::getInstance();
        auto hiddenList = pUserdata->getShopItemsList(GameEnums::ShopItemType_Perk, false);
        spPerkSelectionDialog pPerkSelectionDialog = spPerkSelectionDialog::create(m_pMap, pPlayer, m_pMap->getGameRules()->getMaxPerkCount(), false, hiddenList);
        oxygine::Stage::getStage()->addChild(pPerkSelectionDialog);
        connect(pPerkSelectionDialog.get(), &PerkSelectionDialog::sigFinished, this, [=]()
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
    GameEnums::AiTypes type = static_cast<GameEnums::AiTypes>(m_playerAIs[player]->getCurrentItem());
    if (isOpenPlayer(player))
    {
        type = GameEnums::AiTypes_Open;
    }
    else if (isClosedPlayer(player))
    {
        type = GameEnums::AiTypes_Closed;
    }
    QString name = "";
    if (type == GameEnums::AiTypes_Human &&
        !Mainapp::getSlave())
    {
        name = Settings::getUsername();
    }
    else
    {
        name = m_playerAIs[player]->getCurrentItemText();
    }
    CONSOLE_PRINT("Selecting ai type " + QString::number(type) + " with name " + name + " for payer " + QString::number(player), Console::eDEBUG);
    if (m_pNetworkInterface.get() != nullptr)
    {

        if (m_pNetworkInterface->getIsServer())
        {
            CONSOLE_PRINT(name + " AI " + QString::number(type) + " selected for player " + QString::number(player) + " sending data.", Console::eDEBUG);
            quint64 socket = m_pNetworkInterface->getSocketID();
            m_PlayerSockets[player] = socket;

            createAi(player, type, name);

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
                createAi(player, GameEnums::AiTypes_ProxyAi, name);
            }
            else if (Mainapp::getSlave())
            {
                ai = static_cast<qint32>(GameEnums::AiTypes_ProxyAi);
            }
            QByteArray data;
            createPlayerChangedData(data, socket, name, ai, player, false);
            // update data for all clients
            emit m_pNetworkInterface->sig_sendData(0, data, NetworkInterface::NetworkSerives::Multiplayer, false);
            updatePlayerData(player);
        }
        else
        {
            sendPlayerRequest(0, player, type);
        }
    }
    else
    {
        if (isClosedPlayer(player))
        {
            createAi(player, GameEnums::AiTypes_Closed, name);
        }
        else
        {
            createAi(player, static_cast<GameEnums::AiTypes>(m_playerAIs[player]->getCurrentItem()), name);
        }
    }
}

void PlayerSelection::createAi(qint32 player, GameEnums::AiTypes type, QString displayName)
{    
    if(m_pMap != nullptr)
    {
        Player* pPlayer = m_pMap->getPlayer(player);
        pPlayer->setBaseGameInput(BaseGameInputIF::createAi(m_pMap, type));
        pPlayer->setDisplayName(displayName);
        if (pPlayer->getBaseGameInput() != nullptr)
        {
            pPlayer->getBaseGameInput()->setEnableNeutralTerrainAttack(m_pMap->getGameRules()->getAiAttackTerrain());
        }
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
        CONSOLE_PRINT("Network Command received: " + messageType + " for socket " + QString::number(socketID), Console::eDEBUG);
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
        else if (messageType == NetworkCommands::PLAYERACCESSDENIED)
        {
            playerAccessDenied();
        }
        else if (messageType == NetworkCommands::JOINASOBSERVER)
        {
            joinObserver(socketID);
        }
        else if (messageType == NetworkCommands::SERVERREADY)
        {
            recievePlayerServerReady(socketID, stream);
        }
        else
        {
            CONSOLE_PRINT("Command not handled in playerselection", Console::eDEBUG);
        }
    }
    else if (service == NetworkInterface::NetworkSerives::ServerHostingJson)
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject objData = doc.object();
        QString messageType = objData.value(JsonKeys::JSONKEY_COMMAND).toString();
        CONSOLE_PRINT("Server Command received: " + messageType + " for socket " + QString::number(socketID), Console::eDEBUG);
        if (messageType == NetworkCommands::SERVERREQUESTOPENPLAYERCOUNT)
        {
            sendOpenPlayerCount();
        }
        else
        {
            CONSOLE_PRINT("Command not handled in playerselection", Console::eDEBUG);
        }
    }
}

void PlayerSelection::joinObserver(quint64 socketID)
{
    if (m_pNetworkInterface->getIsServer())
    {
        auto* gameRules = m_pMap->getGameRules();
        auto & observer = gameRules->getObserverList();
        if (observer.size() < gameRules->getMultiplayerObserver())
        {
            observer.append(socketID);
            auto server = oxygine::dynamic_pointer_cast<TCPServer>(m_pNetworkInterface);
            if (server.get())
            {
                auto client = server->getClient(socketID);
                client->setIsObserver(true);
            }
        }
        else
        {
            emit m_pNetworkInterface.get()->sigDisconnectClient(socketID);
        }
    }
}

void PlayerSelection::playerAccessDenied()
{
    if (!m_pNetworkInterface->getIsServer() &&
        !m_pNetworkInterface->getIsObserver() &&
        !hasHumanPlayer())
    {
        emit m_pNetworkInterface->sigDisconnected(-1);
    }
}

void PlayerSelection::sendOpenPlayerCount()
{
    if (Mainapp::getSlaveClient().get() != nullptr)
    {
        QString command = QString(NetworkCommands::SERVEROPENPLAYERCOUNT);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        data.insert(JsonKeys::JSONKEY_SLAVENAME, Settings::getSlaveServerName());
        qint32 openPlayerCount = 0;
        for (const auto & playerAI : qAsConst(m_playerAIs))
        {
            if (playerAI->getCurrentItem() == playerAI->getItemCount() - 1)
            {
                openPlayerCount++;
            }
        }
        data.insert(JsonKeys::JSONKEY_OPENPLAYERCOUNT, openPlayerCount);
        QJsonDocument doc(data);
        emit Mainapp::getSlaveClient()->sig_sendData(0, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
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
        QString command = QString(NetworkCommands::SERVERREADY);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << command;
        sendStream << value;
        sendStream << static_cast<qint32>(player.size());
        for  (qint32 i = 0; i < player.size(); i++)
        {
            sendStream << player[i];
        }
        emit m_pNetworkInterface.get()->sigForwardData(socketID, sendData, NetworkInterface::NetworkSerives::Multiplayer);
    }
}

void PlayerSelection::recievePlayerServerReady(quint64, QDataStream& stream)
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

bool PlayerSelection::getSaveGame() const
{
    return m_saveGame;
}

void PlayerSelection::setSaveGame(bool value)
{
    m_saveGame = value;
}

void PlayerSelection::sendPlayerRequest(quint64 socketID, qint32 player, GameEnums::AiTypes aiType)
{
    QString command = QString(NetworkCommands::REQUESTPLAYER);
    CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream << command;
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
            for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
            {
                if (isOpenPlayer(i))
                {
                    player = i;
                    break;
                }
            }
        }
        // opening a player is always valid changing to an human with an open player is also valid
        if (isOpenPlayer(player) ||
            eAiType == GameEnums::AiTypes_Open)
        {
            CONSOLE_PRINT("Player " + username + " change for " + QString::number(player) + " changed locally to ai-type " + QString::number(eAiType), Console::eDEBUG);
            // valid request
            // change data locally and send remote update
            Player* pPlayer = m_pMap->getPlayer(player);
            // we need to handle opening a player slightly different here...
            if (eAiType == GameEnums::AiTypes_Open)
            {
                pPlayer->setBaseGameInput(spBaseGameInputIF());
                m_playerAIs[player]->setCurrentItem(m_playerAIs[player]->getItemCount() - 1);
                m_PlayerSockets[player] = 0;
            }
            else
            {
                pPlayer->setBaseGameInput(BaseGameInputIF::createAi(m_pMap, GameEnums::AiTypes_ProxyAi));
                pPlayer->setDisplayName(username);
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
            CONSOLE_PRINT("Player change " + QString::number(player) + " changing remote to ai-type " + QString::number(aiType), Console::eDEBUG);
            QByteArray sendDataRequester;
            createPlayerChangedData(sendDataRequester, socketID, username, aiType, player, true);
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
            createPlayerChangedData(sendDataOtherClients, socketID, username, aiType, player, false);
            // send player update
            emit m_pNetworkInterface->sig_sendData(socketID, sendDataRequester, NetworkInterface::NetworkSerives::Multiplayer, false);
            emit m_pNetworkInterface.get()->sigForwardData(socketID, sendDataOtherClients, NetworkInterface::NetworkSerives::Multiplayer);
        }
        else
        {
            CONSOLE_PRINT("Player change rejected. Cause player isn't open anymore", Console::eDEBUG);
            QString command = NetworkCommands::PLAYERACCESSDENIED;
            QByteArray accessDenied;
            QDataStream sendStream(&accessDenied, QIODevice::WriteOnly);
            sendStream << command;
            emit m_pNetworkInterface->sig_sendData(socketID, accessDenied, NetworkInterface::NetworkSerives::Multiplayer, false);
        }
    }
}

void PlayerSelection::changePlayer(quint64 socketId, QDataStream& stream)
{
    if (!m_pNetworkInterface->getIsServer() ||
        m_isServerGame)
    {        
        quint64 socket;
        QString name;
        qint32 aiType;
        qint32 player;
        bool clientRequest = false;;
        stream >> clientRequest;
        stream >> socket;
        stream >> name;
        stream >> player;
        stream >> aiType;
        CONSOLE_PRINT("Remote change of Player " + QString::number(player) + " with name " + name + " for socket " + QString::number(socket) + " and ai " + QString::number(aiType), Console::eDEBUG);
        if (socket != m_pNetworkInterface->getSocketID() ||
            aiType != GameEnums::AiTypes::AiTypes_ProxyAi)
        {
            m_PlayerSockets[player] = socket;
            if (Mainapp::getSlave())
            {
                if (aiType != GameEnums::AiTypes::AiTypes_Open &&
                    aiType != GameEnums::AiTypes::AiTypes_Closed)
                {
                    aiType = GameEnums::AiTypes::AiTypes_ProxyAi;
                }
                CONSOLE_PRINT("Slave remapped change of Player " + QString::number(player) + " with name " + name + " for socket " + QString::number(socket) + " and ai " + QString::number(aiType) + " after validation.", Console::eDEBUG);
            }
            else if (m_isServerGame)
            {
                if (aiType != GameEnums::AiTypes::AiTypes_Open &&
                    aiType != GameEnums::AiTypes::AiTypes_Human)
                {
                    aiType = GameEnums::AiTypes::AiTypes_ProxyAi;
                }
                CONSOLE_PRINT("Server remapped change of Player " + QString::number(player) + " with name " + name + " for socket " + QString::number(socket) + " and ai " + QString::number(aiType) + " after validation.", Console::eDEBUG);
            }
            else if (!clientRequest)
            {
                if (aiType != GameEnums::AiTypes::AiTypes_Open &&
                    aiType != GameEnums::AiTypes::AiTypes_Closed)
                {
                    aiType = GameEnums::AiTypes::AiTypes_ProxyAi;
                }
                CONSOLE_PRINT("Remapped change of Player " + QString::number(player) + " with name " + name + " for socket " + QString::number(socket) + " and ai " + QString::number(aiType) + " after validation.", Console::eDEBUG);
            }
            GameEnums::AiTypes eAiType = static_cast<GameEnums::AiTypes>(aiType);
            setPlayerAi(player, eAiType, name);
            m_pMap->getPlayer(player)->deserializeObject(stream);
            m_pMap->getPlayer(player)->setBaseGameInput(BaseGameInputIF::createAi(m_pMap, eAiType));
            m_pMap->getPlayer(player)->setDisplayName(name);

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
            if (!humanFound && !m_pNetworkInterface->getIsObserver())
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
                createPlayerChangedData(data, socket, name, aiType, player, false);
                emit m_pNetworkInterface->sigForwardData(socketId, data, NetworkInterface::NetworkSerives::Multiplayer);
            }
        }
        else
        {
            CONSOLE_PRINT("Update ignored", Console::eDEBUG);
        }
    }
}

void PlayerSelection::createPlayerChangedData(QByteArray & data, quint64 socketId, QString name, qint32 aiType, qint32 player, bool clientRequest)
{
    QString command = QString(NetworkCommands::PLAYERCHANGED);
    CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
    QDataStream sendStream(&data, QIODevice::WriteOnly);
    
    sendStream << command;
    sendStream << clientRequest;
    sendStream << socketId;
    sendStream << name;
    sendStream << player;
    sendStream << aiType;
    m_pMap->getPlayer(player)->serializeObject(sendStream);
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
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = m_pMap->getPlayer(i);
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
    qint32 playerIdx;
    QString coid;
    stream >> playerIdx;
    stream >> coid;
    QStringList perks = Filesupport::readVectorList<QString, QList>(stream);
    m_pMap->getPlayer(playerIdx)->setCO(coid, 0);
    CO* pCO = m_pMap->getPlayer(playerIdx)->getCO(0);
    if (pCO != nullptr)
    {
        pCO->setPerkList(perks);
        pCO->readCoStyleFromStream(stream);
    }
    updateCO1Sprite(coid, playerIdx);
    stream >> coid;
    perks = Filesupport::readVectorList<QString, QList>(stream);
    m_pMap->getPlayer(playerIdx)->setCO(coid, 1);
    pCO = m_pMap->getPlayer(playerIdx)->getCO(1);
    if (pCO != nullptr)
    {
        pCO->setPerkList(perks);
        pCO->readCoStyleFromStream(stream);
    }
    updateCO2Sprite(coid, playerIdx);
}

void PlayerSelection::recievedColorData(quint64, QDataStream& stream)
{    
    qint32 playerIdx = 0;
    QColor displayColor;
    stream >> playerIdx;
    stream >> displayColor;
    Player* pPlayer = m_pMap->getPlayer(playerIdx);
    QColor tableColor = displayColorToTableColor(displayColor);
    pPlayer->setColor(tableColor);
    m_playerColors[playerIdx]->setCurrentItem(displayColor);
}

void PlayerSelection::recievePlayerArmy(quint64, QDataStream& stream)
{    
    qint32 playerIdx = 0;
    QString army;
    stream >> playerIdx;
    stream >> army;
    Player* pPlayer = m_pMap->getPlayer(playerIdx);
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
        CONSOLE_PRINT("Reopening players for socket " + QString::number(socketID) + " after disconnecting", Console::eLogLevels::eDEBUG);
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
        CONSOLE_PRINT("Removing socket " + QString::number(socketID) + " from observer list", Console::eLogLevels::eDEBUG);
        auto* gameRules = m_pMap->getGameRules();
        auto & observer = gameRules->getObserverList();
        observer.removeAll(socketID);
    }
}

void PlayerSelection::updatePlayerData(qint32 player)
{    
    if (player < 0)
    {
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            updatePlayerData(i);
        }
    }
    else
    {
        Player* pPlayer = m_pMap->getPlayer(player);
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
        if (m_pNetworkInterface->getIsObserver())
        {
            m_playerAIs[player]->setEnabled(false);
            m_playerColors[player]->setEnabled(false);
            m_playerCO1[player]->setEnabled(false);
            m_playerCO2[player]->setEnabled(false);
            m_playerPerks[player]->setEnabled(false);
            m_playerArmy[player]->setEnabled(false);
        }
        else if (pPlayer->getBaseGameInput() == nullptr ||
                 pPlayer->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human ||
                 notServerChangeAblePlayer)
        {
            if (((pPlayer->getBaseGameInput() != nullptr &&
                  pPlayer->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human) ||
                 notServerChangeAblePlayer) &&
                !m_saveGame)
            {
                m_playerAIs[player]->setEnabled(true);
                m_playerColors[player]->setEnabled(true);
                m_playerCO1[player]->setEnabled(true);
                m_playerCO2[player]->setEnabled(true);
                m_playerPerks[player]->setEnabled(true);
                m_playerArmy[player]->setEnabled(true);
            }
            else
            {
                if (m_pNetworkInterface->getIsServer() || m_isServerGame ||
                    isOpenPlayer(player))
                {
                    m_playerAIs[player]->setEnabled(true);
                }
                else
                {
                    m_playerAIs[player]->setEnabled(false);
                }
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
                !m_saveGame)
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
        if (getIsCampaign())
        {
            auto* input = pPlayer->getBaseGameInput();
            if (input != nullptr &&
                input->getAiType() != GameEnums::AiTypes_Human)
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

void PlayerSelection::changeAllTeams(qint32 value)
{    
    qint32 playersPerTeam = m_pMap->getPlayerCount() / value;
    qint32 freePlayer = m_pMap->getPlayerCount() - value * playersPerTeam;
    qint32 upCountPlayers = playersPerTeam / 2;
    qint32 player = 0;
    for (qint32 team = 0; team < value; ++team)
    {
        for (qint32 i = 0; i < playersPerTeam - upCountPlayers; ++i)
        {
            m_playerTeams[player]->setCurrentItem(team);
            playerTeamChanged(team, player);
            ++player;
        }
    }
    for (qint32 team = value - 1; team >= 0; --team)
    {
        for (qint32 i = 0; i < upCountPlayers; ++i)
        {
            m_playerTeams[player]->setCurrentItem(team);
            playerTeamChanged(team, player);
            ++player;
        }
        if (freePlayer > 0)
        {
            --freePlayer;
            m_playerTeams[player]->setCurrentItem(team);
            playerTeamChanged(team, player);
            ++player;
        }
    }
}
