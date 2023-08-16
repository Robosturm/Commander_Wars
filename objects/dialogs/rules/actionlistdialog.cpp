#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/dialogs//rules/actionlistdialog.h"

#include <QDirIterator>

#include "coreengine/filesupport.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/gamemanager.h"

#include "game/gamemap.h"

#include "objects/base/panel.h"

#include "objects/dialogs/dialogmessagebox.h"
#include "objects/dialogs/dialogtextinput.h"
#include "objects/base/label.h"

const char* const ActionListDialog::FILEPATH = "data/actionbannlist/";

ActionListDialog::ActionListDialog(QStringList bannlist, GameMap* pMap)
    : m_CurrentActionList(bannlist),
      m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("ActionListDialog");
#endif
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_pSpriteBox = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(m_pSpriteBox);
    m_pSpriteBox->setPosition(0, 0);
    m_pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    // no the fun begins create checkboxes and stuff and a panel down here
    spPanel pPanel = MemoryManagement::create<Panel>(true, QSize(oxygine::Stage::getStage()->getWidth() - 60, oxygine::Stage::getStage()->getHeight() - 150),
                               QSize(oxygine::Stage::getStage()->getWidth() - 60, oxygine::Stage::getStage()->getHeight() - 150));
    pPanel->setPosition(30, 30);
    m_pSpriteBox->addChild(pPanel);

    oxygine::TextStyle headerStyle = oxygine::TextStyle(FontManager::getMainFont48());
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = false;
    spLabel pLabel = MemoryManagement::create<Label>(pPanel->getScaledWidth() - 60);
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText(tr("Action List"));
    pLabel->setPosition(pPanel->getScaledWidth() / 2 - pLabel->getTextRect().width() / 2, 10);
    pPanel->addItem(pLabel);

    GameManager* pGameManager = GameManager::getInstance();

    QStringList actionList = pGameManager->getLoadedRessources();

    qint32 y = pLabel->getY() + pLabel->getTextRect().height() + 10;
    qint32 x = 10;
    
    m_CurrentActionList = m_pMap->getGameRules()->getAllowedActions();

    for (qint32 i = 0; i < actionList.size(); i++)
    {
        QString actionId = actionList[i];
        QString icon = pGameManager->getActionIcon(actionId);
        if (!icon.isEmpty())
        {
            pLabel = MemoryManagement::create<Label>(300);
            pLabel->setStyle(style);
            pLabel->setHtmlText(pGameManager->getName(i));
            pLabel->setPosition(x + 90, y);
            QString tooltip = pGameManager->getDescription(i);

            spTooltip pTooltip = MemoryManagement::create<Tooltip>();

            oxygine::spSprite pSprite = pGameManager->getIcon(pMap, icon);
            pSprite->setScale(1.25f * pSprite->getScaleX());
            pTooltip->addChild(pSprite);
            pTooltip->setPosition(x + 45, y);
            pTooltip->setTooltipText(tooltip);

            spCheckbox pCheckbox = MemoryManagement::create<Checkbox>();
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
            connect(pCheckbox.get(), &Checkbox::checkChanged, this, [this, actionId](bool checked)
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

            x += 400;
            if (x + 400 > pPanel->getContentWidth() - 60)
            {
                y += pLabel->getHeight() + 20;
                x = 10;
            }
        }
    }
    pPanel->setContentHeigth(y + 50);
    connect(this, &ActionListDialog::canceled, this, &ActionListDialog::remove, Qt::QueuedConnection);
    connect(this, &ActionListDialog::sigFinished, this, &ActionListDialog::remove, Qt::QueuedConnection);

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(oxygine::Stage::getStage()->getWidth() - m_OkButton->getScaledWidth() - 30,
                            oxygine::Stage::getStage()->getHeight() - 30 - m_OkButton->getScaledHeight());
    m_pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit editFinished(m_CurrentActionList);
        emit sigFinished();
    });

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30, oxygine::Stage::getStage()->getHeight() - 30 - m_ExitButton->getScaledHeight());
    m_pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit canceled();
    });

    oxygine::spButton pSave = pObjectManager->createButton(tr("Save"), 150);
    pSave->setPosition(oxygine::Stage::getStage()->getWidth() / 2 + 10,
                       oxygine::Stage::getStage()->getHeight() - 30 - m_ExitButton->getScaledHeight());
    pSave->addClickListener([this](oxygine::Event*)
    {
        emit sigShowSaveBannlist();
    });
    m_pSpriteBox->addChild(pSave);
    connect(this, &ActionListDialog::sigShowSaveBannlist, this, &ActionListDialog::showSaveBannlist, Qt::QueuedConnection);
    connect(this, &ActionListDialog::sigDoSaveBannlist, this, &ActionListDialog::doSaveBannlist, Qt::QueuedConnection);

    oxygine::spButton pDelete = pObjectManager->createButton(tr("Delete"), 150);
    pDelete->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - pDelete->getScaledWidth() - 10,
                       oxygine::Stage::getStage()->getHeight() - 30 - m_ExitButton->getScaledHeight());
    pDelete->addClickListener([this](oxygine::Event*)
    {
        emit sigShowDeleteBannlist();
    });
    m_pSpriteBox->addChild(pDelete);
    connect(this, &ActionListDialog::sigShowDeleteBannlist, this, &ActionListDialog::showDeleteBannlist, Qt::QueuedConnection);
    connect(this, &ActionListDialog::sigDeleteBannlist, this, &ActionListDialog::deleteBannlist, Qt::QueuedConnection);

    m_ToggleAll = pObjectManager->createButton(tr("Un/Select All"), 180);
    m_ToggleAll->setPosition(oxygine::Stage::getStage()->getWidth() / 2 + 10,
                             oxygine::Stage::getStage()->getHeight() - 75 - m_ToggleAll->getScaledHeight());
    m_pSpriteBox->addChild(m_ToggleAll);
    m_ToggleAll->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        m_toggle = !m_toggle;
        for (qint32 i = 0; i < m_Checkboxes.size(); i++)
        {
            m_Checkboxes[i]->setChecked(m_toggle);
            emit m_Checkboxes[i]->checkChanged(m_toggle);
        }
    });
    updatePredefinedList();
}

void ActionListDialog::remove()
{
    m_Checkboxes.clear();
    detach();
}

QStringList ActionListDialog::getNameList()
{
    QStringList items;
    QStringList filters;
    filters << QString("*") + Filesupport::LIST_FILENAME_ENDING;
    QDirIterator dirIter(FILEPATH, filters, QDir::Files, QDirIterator::IteratorFlag::NoIteratorFlags);
    while (dirIter.hasNext())
    {
        dirIter.next();
        QString file = dirIter.fileInfo().canonicalFilePath();
        auto data = Filesupport::readList(file);
        items.append(data.name);
    }
    return items;
}

void ActionListDialog::setBuildlist(qint32)
{
    QStringList data;
    QString file = m_PredefinedLists->getCurrentItemText();
    auto fileData = Filesupport::readList(file + Filesupport::LIST_FILENAME_ENDING, FILEPATH);
    data = fileData.items;
    m_CurrentActionList = data;
}

void ActionListDialog::showSaveBannlist()
{    
    spDialogTextInput pSaveInput = MemoryManagement::create<DialogTextInput>(tr("Bannlist Name"), true, "");
    connect(pSaveInput.get(), &DialogTextInput::sigTextChanged, this, &ActionListDialog::saveBannlist, Qt::QueuedConnection);
    addChild(pSaveInput);    
}

void ActionListDialog::showDeleteBannlist()
{
    if (QFile::exists(FILEPATH + m_PredefinedLists->getCurrentItemText() + Filesupport::LIST_FILENAME_ENDING))
    {
        QString file = FILEPATH + m_PredefinedLists->getCurrentItemText() + Filesupport::LIST_FILENAME_ENDING;
        spDialogMessageBox pDialogOverwrite = MemoryManagement::create<DialogMessageBox>(tr("Do you want to delete the action bannlist: ") + file + "?", true);
        connect(pDialogOverwrite.get(), &DialogMessageBox::sigOk, this, [this, file]
        {
            emit sigDeleteBannlist(file);
        }, Qt::QueuedConnection);
        addChild(pDialogOverwrite);
    }
}

void ActionListDialog::deleteBannlist(const QString file)
{
    QFile::remove(file);
    updatePredefinedList();
}

void ActionListDialog::saveBannlist(QString filename)
{    
    if (QFile::exists(FILEPATH + filename + Filesupport::LIST_FILENAME_ENDING))
    {
        spDialogMessageBox pDialogOverwrite = MemoryManagement::create<DialogMessageBox>(tr("Do you want to overwrite the action bannlist: ") + FILEPATH + filename + Filesupport::LIST_FILENAME_ENDING + "?", true);
        connect(pDialogOverwrite.get(), &DialogMessageBox::sigOk, this, [this, filename]
        {
            emit sigDoSaveBannlist(filename);
        }, Qt::QueuedConnection);
        addChild(pDialogOverwrite);
    }
    else
    {
        doSaveBannlist(filename);
    }
}

void ActionListDialog::doSaveBannlist(QString filename)
{
    Filesupport::storeList(filename, m_CurrentActionList, FILEPATH);
    updatePredefinedList();
}

void ActionListDialog::updatePredefinedList()
{
    if (m_PredefinedLists.get() != nullptr)
    {
        m_PredefinedLists->detach();
    }
    auto items = getNameList();
    m_PredefinedLists = MemoryManagement::create<DropDownmenu>(300, items);
    m_PredefinedLists->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_PredefinedLists->getScaledWidth() - 10,
                                   oxygine::Stage::getStage()->getHeight() - 75 - m_ToggleAll->getScaledHeight());
    m_pSpriteBox->addChild(m_PredefinedLists);
    connect(m_PredefinedLists.get(), &DropDownmenu::sigItemChanged, this, &ActionListDialog::setBuildlist, Qt::QueuedConnection);
}
