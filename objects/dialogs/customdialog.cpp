#include "objects/dialogs/customdialog.h"

#include "coreengine/mainapp.h"
#include "resource_management/objectmanager.h"
#include "objects/dialogs/dialogmessagebox.h"

#include "ui_reader/uifactory.h"

CustomDialog::CustomDialog(const QString & jsName, const QString & uiXml, Basemenu* pBaseMenu, const QString & confirmText)
    : m_jsName(jsName),
      m_pBaseMenu(pBaseMenu)
{
    setObjectName("CustomDialog");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));
    connect(this, &CustomDialog::sigFinished, this, &CustomDialog::remove, Qt::QueuedConnection);

    if (!jsName.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        pInterpreter->setGlobal(jsName, pInterpreter->newQObject(this));
    }
    UiFactory::getInstance().createUi(uiXml, this);
    if (!confirmText.isEmpty())
    {
        oxygine::spButton pOkButton = pObjectManager->createButton(confirmText, 150);
        pOkButton->setPosition(Settings::getWidth() / 2 - pOkButton->getWidth() / 2,
                              Settings::getHeight() - 10 - pOkButton->getHeight());
        addChild(pOkButton);
        pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
        {
            exit();
        });
    }
}

CustomDialog::~CustomDialog()
{
    if (!m_jsName.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        pInterpreter->deleteObject(m_jsName);
    }
}

Basemenu* CustomDialog::getBaseMenu()
{
    return m_pBaseMenu;
}

void CustomDialog::exit()
{
    emit sigFinished();
}

void CustomDialog::remove()
{
    detach();
}

void CustomDialog::createDialog(const QString & jsName, const QString & uiXml, Basemenu* pBaseMenu)
{
    spCustomDialog pDialog = spCustomDialog::create(jsName, uiXml, pBaseMenu);
    pBaseMenu->addChild(pDialog);
}

void CustomDialog::showMessageBox(QString text, bool withCancel, QString confirmText, QString cancelText)
{
    spDialogMessageBox pDialog = spDialogMessageBox::create(text, withCancel, confirmText, cancelText);
    m_pBaseMenu->addChild(pDialog);
}
