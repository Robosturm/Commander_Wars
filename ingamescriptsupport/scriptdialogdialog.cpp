#include "scriptdialogdialog.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "resource_management/cospritemanager.h"

#include "coreengine/mainapp.h"

#include "objects/filedialog.h"
#include "objects/dropdownmenusprite.h"
#include "qfileinfo.h"

ScriptDialogDialog::ScriptDialogDialog(spScriptEventDialog scriptEventDialog)
    : QObject(),
      m_Event(scriptEventDialog)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("semidialog");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    m_pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    m_pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(m_pSpriteBox);
    m_pSpriteBox->setPosition(0, 0);
    m_pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

    QSize size(Settings::getWidth() - 80, Settings::getHeight() - 120);
    m_Panel = new Panel(true, size, size);
    m_Panel->setPosition(30, 30);
    m_pSpriteBox->addChild(m_Panel);

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(Settings::getWidth() - pOkButton->getWidth() - 30, Settings::getHeight() - 30 - pOkButton->getHeight());
    m_pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigFinished();
        detach();
    });

    // add Dialog button
    oxygine::spButton pDialogButton = pObjectManager->createButton(tr("add Dialog"), 150);
    pDialogButton->setPosition(30, Settings::getHeight() - 30 - pDialogButton->getHeight());
    m_pSpriteBox->addChild(pDialogButton);
    pDialogButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigAddItem();
    });
    // remove Dialog button
    oxygine::spButton pRemoveButton = pObjectManager->createButton(tr("remove Last"), 150);
    pRemoveButton->setPosition(Settings::getWidth() / 2 - pRemoveButton->getWidth() / 2, Settings::getHeight() - 30 - pRemoveButton->getHeight());
    m_pSpriteBox->addChild(pRemoveButton);
    pRemoveButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigRemoveLast();
    });
    connect(this, &ScriptDialogDialog::sigRemoveLast, this, &ScriptDialogDialog::removeLast, Qt::QueuedConnection);
    connect(this, &ScriptDialogDialog::sigAddItem, this, &ScriptDialogDialog::addItem, Qt::QueuedConnection);
    connect(this, &ScriptDialogDialog::sigShowChangeBackground, this, &ScriptDialogDialog::showChangeBackground, Qt::QueuedConnection);
    updateDialog();
}

void ScriptDialogDialog::addItem()
{
    m_Event->addDialog("", "CO_ANDY", GameEnums::COMood_Normal, Qt::red);
    updateDialog();
}

void ScriptDialogDialog::updateDialog()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Panel->clearContent();
    qint32 count = m_Event->getDialogSize();
    qint32 panelWidth = Settings::getWidth();
    if (panelWidth < 1000)
    {
        panelWidth = 1000;
    }

    for (qint32 i = 0; i < count; i++)
    {
        ScriptEventDialog::Dialog* pDialog = m_Event->getDialog(i);
        qint32 y = i * 40;
        qint32 boxWidth = panelWidth - 800;
        qint32 posX = panelWidth - 800;
        spTextbox pTextbox = new Textbox(boxWidth);
        pTextbox->setTooltipText(tr("The text the CO should talk."));
        pTextbox->setPosition(0, y);
        pTextbox->setCurrentText(pDialog->text);
        m_Panel->addItem(pTextbox);
        connect(pTextbox.get(), &Textbox::sigTextChanged, [=](QString text)
        {
            pDialog->text = text;
        });
        QVector<QString> moods = {tr("Normal"), tr("Happy"), tr("Sad")};
        spDropDownmenu moodMenu = new DropDownmenu(150, moods);
        moodMenu->setTooltipText(tr("The CO Mood/Icon that will be used for the dialog."));
        moodMenu->setPosition(posX, y);
        moodMenu->setCurrentItem(static_cast<qint32>(pDialog->mood));
        m_Panel->addItem(moodMenu);
        connect(moodMenu.get(), &DropDownmenu::sigItemChanged, [=](qint32 item)
        {
            pDialog->mood = static_cast<GameEnums::COMood>(item);
        });

        QVector<QString> ids;
        COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
        ids.append(ScriptEventDialog::m_CurrentPlayerCO0);
        ids.append(ScriptEventDialog::m_CurrentPlayerCO1);
        for (qint32 i = 0; i < pCOSpriteManager->getCount(); i++)
        {
            ids.append(pCOSpriteManager->getID(i));
        }
        ids.append("co_beast");
        ids.append("co_davis");
        ids.append("co_officier_os");
        ids.append("co_officier_bm");
        ids.append("co_officier_ge");
        ids.append("co_officier_yc");
        ids.append("co_officier_bh");

        auto creator = [=](QString id)
        {
            oxygine::ResAnim* pAnim = nullptr;
            if (id == ScriptEventDialog::m_CurrentPlayerCO0)
            {
                pAnim = COSpriteManager::getInstance()->getResAnim("co_0+info");
            }
            else if (id == ScriptEventDialog::m_CurrentPlayerCO1)
            {
                pAnim = COSpriteManager::getInstance()->getResAnim("co_1+info");
            }
            else
            {
                pAnim = COSpriteManager::getInstance()->getResAnim(id + "+info");
            }
            oxygine::spSprite pSprite = new oxygine::Sprite();
            pSprite->setResAnim(pAnim);
            pSprite->setSize(pAnim->getSize());
            return pSprite;
        };

        spDropDownmenuSprite coidsMenu = new DropDownmenuSprite(150, ids, creator);
        coidsMenu->setTooltipText(tr("The ID of the CO that should talk.\nNote: CO 1 and CO 2 represent the CO of the current Player."));
        coidsMenu->setPosition(posX + 150, y);
        coidsMenu->setCurrentItem(pDialog->coid);
        m_Panel->addItem(coidsMenu);
        connect(coidsMenu.get(), &DropDownmenuSprite::sigItemChanged, [=](qint32)
        {
            pDialog->coid = coidsMenu->getCurrentItemText();
        });

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
        spDropDownmenuColor colors = new DropDownmenuColor(110, playerColors);
        colors->setTooltipText(tr("The background color of the dialog."));
        colors->setPosition(posX + 350, y);
        colors->setCurrentItem(pDialog->color);
        m_Panel->addItem(colors);
        connect(colors.get(), &DropDownmenuColor::sigItemChanged, [=](QColor color)
        {
            pDialog->color = color;
        });

        oxygine::spButton pBackgroundTextbox = ObjectManager::createButton(tr("Background"), 200);
        pBackgroundTextbox->setPosition(posX + 465, y);
        m_Panel->addItem(pBackgroundTextbox);
        pBackgroundTextbox->addClickListener([=](oxygine::Event*)
        {
            dialogIndex = i;
            emit sigShowChangeBackground();
        });
    }
    m_Panel->setContentHeigth(count * 40 + 20);
    m_Panel->setContentWidth(panelWidth);
    pApp->continueThread();

}

void ScriptDialogDialog::removeLast()
{
    if (m_Event->getDialogSize() > 0)
    {
        m_Event->removeDialog(m_Event->getDialogSize() - 1);
        updateDialog();
    }
}

void ScriptDialogDialog::showChangeBackground()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    ScriptEventDialog::Dialog* pDialog = m_Event->getDialog(dialogIndex);
    QFileInfo file(pDialog->background);
    QString folder = "maps/";
    QString fileName = "";
    if (file.exists() && file.isFile())
    {
        folder = file.path();
        fileName = file.fileName();
    }
    spFileDialog pFileDialog = new FileDialog(folder, QVector<QString>(1, "*.png"), fileName);
    addChild(pFileDialog);
    connect(pFileDialog.get(), &FileDialog::sigFileSelected, this, &ScriptDialogDialog::setCurrentDialogBackground, Qt::QueuedConnection);
    pApp->continueThread();
}

void ScriptDialogDialog::setCurrentDialogBackground(QString file)
{
    QFileInfo fileInfo(file);
    ScriptEventDialog::Dialog* pDialog = m_Event->getDialog(dialogIndex);
    if (fileInfo.exists() && fileInfo.isFile())
    {
        pDialog->background = file.replace(QCoreApplication::applicationDirPath() + "/", "");
    }
    else
    {
        pDialog->background = "";
    }
    dialogIndex = -1;
}
