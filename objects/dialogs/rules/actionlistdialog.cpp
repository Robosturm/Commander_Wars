#include "actionlistdialog.h"

#include "qdiriterator.h"

#include "coreengine/filesupport.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "resource_management/gamemanager.h"

#include "game/gamemap.h"

#include "objects/base/panel.h"

#include "objects/dialogs/dialogtextinput.h"
#include "objects/base/label.h"

ActionListDialog::ActionListDialog(QStringList bannlist)
    : QObject(),
      m_CurrentActionList(bannlist)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() - m_OkButton->getWidth() - 30, Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit editFinished(m_CurrentActionList);
        detach();
    });

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30, Settings::getHeight() - 30 - m_ExitButton->getHeight());
    pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit canceled();
        detach();
    });

    m_ToggleAll = pObjectManager->createButton(tr("Un/Select All"), 180);
    m_ToggleAll->setPosition(Settings::getWidth() / 2 + 60 , Settings::getHeight() - 30 - m_ToggleAll->getHeight());
    pSpriteBox->addChild(m_ToggleAll);
    m_ToggleAll->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        toggle = !toggle;
        for (qint32 i = 0; i < m_Checkboxes.size(); i++)
        {
            m_Checkboxes[i]->setChecked(toggle);
            emit m_Checkboxes[i]->checkChanged(toggle);
        }
    });
    auto items = getNameList();
    m_PredefinedLists = new DropDownmenu(300, items);

    m_PredefinedLists->setPosition(Settings::getWidth() / 2 + 40 - m_PredefinedLists->getWidth(), Settings::getHeight() - 30 - m_ToggleAll->getHeight());
    pSpriteBox->addChild(m_PredefinedLists);
    connect(m_PredefinedLists.get(), &DropDownmenu::sigItemChanged, this, &ActionListDialog::setBuildlist, Qt::QueuedConnection);

    oxygine::spButton pSave = pObjectManager->createButton(tr("Save"), 150);
    pSave->setPosition(Settings::getWidth() / 2 - pSave->getWidth() / 2, Settings::getHeight() - 75 - m_ToggleAll->getHeight());
    pSave->addClickListener([=](oxygine::Event*)
    {
        emit sigShowSaveBannlist();
    });
    pSpriteBox->addChild(pSave);
    connect(this, &ActionListDialog::sigShowSaveBannlist, this, &ActionListDialog::showSaveBannlist, Qt::QueuedConnection);


    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    // no the fun begins create checkboxes and stuff and a panel down here
    spPanel pPanel = new Panel(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 150),
                               QSize(Settings::getWidth() - 60, Settings::getHeight() - 150));
    pPanel->setPosition(30, 30);
    pSpriteBox->addChild(pPanel);

    oxygine::TextStyle headerStyle = FontManager::getMainFont48();
    headerStyle.color = FontManager::getFontColor();
    headerStyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;
    spLabel pLabel = new Label(pPanel->getWidth() - 60);
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText(tr("Action List"));
    pLabel->setPosition(pPanel->getWidth() / 2 - pLabel->getTextRect().getWidth() / 2, 10);
    pPanel->addItem(pLabel);

    GameManager* pGameManager = GameManager::getInstance();

    QStringList actionList = pGameManager->getLoadedRessources();

    qint32 y = 30 + pLabel->getTextRect().getHeight() * 2;
    qint32 x = 10;
    spGameMap pMap = GameMap::getInstance();
    m_CurrentActionList = pMap->getGameRules()->getAllowedActions();

    for (qint32 i = 0; i < actionList.size(); i++)
    {
        QString actionId = actionList[i];
        QString icon = pGameManager->getActionIcon(actionId);
        if (!icon.isEmpty())
        {
            pLabel = new Label(250);
            pLabel->setStyle(style);
            pLabel->setHtmlText(pGameManager->getName(i));
            pLabel->setPosition(x + 80, y);
            QString tooltip = pGameManager->getDescription(i);

            spTooltip pTooltip = new Tooltip();

            oxygine::spSprite pSprite = pGameManager->getIcon(icon);
            pSprite->setScale(1.25f * pSprite->getScaleX());
            pTooltip->addChild(pSprite);
            pTooltip->setPosition(x + 45, y);
            pTooltip->setTooltipText(tooltip);

            spCheckbox pCheckbox = new Checkbox();
            pCheckbox->setPosition(x, y);
            pCheckbox->setTooltipText(tooltip);
            m_Checkboxes.append(pCheckbox);

            if (m_CurrentActionList.contains(actionId))
            {
                pCheckbox->setChecked(true);
            }
            else
            {
                pCheckbox->setChecked(false);
            }
            connect(pCheckbox.get(), &Checkbox::checkChanged, [=](bool checked)
            {
                if (checked)
                {
                    m_CurrentActionList.append(actionId);
                }
                else
                {
                    m_CurrentActionList.removeAll(actionId);
                }
            });

            pPanel->addItem(pCheckbox);
            pPanel->addItem(pLabel);
            pPanel->addItem(pTooltip);

            x += 340;
            if (x + 340 > pPanel->getContentWidth())
            {
                y += 40;
                x = 10;
            }
        }
    }
    pPanel->setContentHeigth(y + 50);
}

QVector<QString> ActionListDialog::getNameList()
{
    QVector<QString> items;
    QStringList filters;
    filters << "*.bl";
    QDirIterator dirIter("data/actionbannlist/", filters, QDir::Files, QDirIterator::IteratorFlag::NoIteratorFlags);
    while (dirIter.hasNext())
    {
        dirIter.next();
        QString file = dirIter.fileInfo().absoluteFilePath();
        std::tuple<QString, QStringList> data = Filesupport::readList(file);
        items.append(std::get<0>(data));
    }
    return items;
}

void ActionListDialog::setBuildlist(qint32)
{
    QStringList data;
    QString file = m_PredefinedLists->getCurrentItemText();
    auto fileData = Filesupport::readList(file + ".bl", "data/actionbannlist/");
    data = std::get<1>(fileData);
    m_CurrentActionList = data;
}

void ActionListDialog::showSaveBannlist()
{    
    spDialogTextInput pSaveInput = new DialogTextInput(tr("Bannlist Name"), true, "");
    connect(pSaveInput.get(), &DialogTextInput::sigTextChanged, this, &ActionListDialog::saveBannlist, Qt::QueuedConnection);
    addChild(pSaveInput);    
}

void ActionListDialog::saveBannlist(QString filename)
{    
    Filesupport::storeList(filename, m_CurrentActionList, "data/actionbannlist/");
    auto items = getNameList();
    m_PredefinedLists->changeList(items);
}
