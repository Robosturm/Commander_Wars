#include "scriptdialogdialog.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

#include "resource_management/cospritemanager.h"

#include "coreengine/mainapp.h"

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
    m_pSpriteBox->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    m_pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    m_pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(m_pSpriteBox);
    m_pSpriteBox->setPosition(0, 0);
    m_pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

    QSize size(pApp->getSettings()->getWidth() - 80, pApp->getSettings()->getHeight() - 120);
    m_Panel = new Panel(true, size, size);
    m_Panel->setPosition(30, 30);
    m_pSpriteBox->addChild(m_Panel);

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(pApp->getSettings()->getWidth() - pOkButton->getWidth() - 30, pApp->getSettings()->getHeight() - 30 - pOkButton->getHeight());
    m_pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigFinished();
        this->getParent()->removeChild(this);
    });

    // add Dialog button
    oxygine::spButton pDialogButton = pObjectManager->createButton(tr("add Dialog"), 150);
    pDialogButton->setPosition(30, pApp->getSettings()->getHeight() - 30 - pDialogButton->getHeight());
    m_pSpriteBox->addChild(pDialogButton);
    pDialogButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigAddItem();
    });
    // remove Dialog button
    oxygine::spButton pRemoveButton = pObjectManager->createButton(tr("remove Last"), 150);
    pRemoveButton->setPosition(pApp->getSettings()->getWidth() / 2 - pRemoveButton->getWidth() / 2, pApp->getSettings()->getHeight() - 30 - pRemoveButton->getHeight());
    m_pSpriteBox->addChild(pRemoveButton);
    pRemoveButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigRemoveLast();
    });
    connect(this, &ScriptDialogDialog::sigRemoveLast, this, &ScriptDialogDialog::removeLast, Qt::QueuedConnection);
    connect(this, &ScriptDialogDialog::sigAddItem, this, &ScriptDialogDialog::addItem, Qt::QueuedConnection);
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
    for (qint32 i = 0; i < count; i++)
    {
        ScriptEventDialog::Dialog* pDialog = m_Event->getDialog(i);
        qint32 y = i * 40;
        spTextbox pTextbox = new Textbox(pApp->getSettings()->getWidth() - 600);
        pTextbox->setPosition(0, y);
        pTextbox->setCurrentText(pDialog->text);
        m_Panel->addItem(pTextbox);
        bool up = false;
        if (i >= 5)
        {
            up = true;
        }
        connect(pTextbox.get(), &Textbox::sigTextChanged, [=](QString text)
        {
            pDialog->text = text;
        });

        QVector<QString> moods = {tr("Normal"), tr("Happy"), tr("Sad")};
        spDropDownmenu moodMenu = new DropDownmenu(150, moods, up);
        moodMenu->setPosition(pApp->getSettings()->getWidth() - 600, y);
        moodMenu->setCurrentItem(static_cast<qint32>(pDialog->mood));
        m_Panel->addItem(moodMenu);
        connect(moodMenu.get(), &DropDownmenu::sigItemChanged, [=](qint32 item)
        {
            pDialog->mood = static_cast<GameEnums::COMood>(item);
        });

        QVector<QString> ids = COSpriteManager::getInstance()->getSpriteCOIDs();
        spDropDownmenu coidsMenu = new DropDownmenu(200, ids, up);
        coidsMenu->setPosition(pApp->getSettings()->getWidth() - 450, y);
        coidsMenu->setCurrentItem(pDialog->coid);
        m_Panel->addItem(coidsMenu);
        connect(coidsMenu.get(), &DropDownmenu::sigItemChanged, [=](qint32)
        {
            pDialog->coid = coidsMenu->getCurrentItemText();
        });

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
        spDropDownmenuColor colors = new DropDownmenuColor(110, playerColors, up);
        colors->setPosition(pApp->getSettings()->getWidth() - 250, y);
        colors->setCurrentItem(pDialog->color);
        m_Panel->addItem(colors);
        connect(colors.get(), &DropDownmenuColor::sigItemChanged, [=](QColor color)
        {
            pDialog->color = color;
        });
    }
    m_Panel->setContentHeigth(count * 40 + 20);
    pApp->continueThread();

}

void ScriptDialogDialog::removeLast()
{
    if (m_Event->getDialogSize() > 0)
    {
        m_Event->removeDialog(m_Event->getDialogSize() - 1);
    }
}
