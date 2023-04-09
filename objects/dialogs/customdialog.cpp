#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/dialogs/customdialog.h"

#include "resource_management/objectmanager.h"
#include "objects/dialogs/dialogmessagebox.h"

#include "ui_reader/uifactory.h"

CustomDialog::CustomDialog(const QString & jsName, const QString & uiXml, Basemenu* pBaseMenu, const QString & confirmText)
    : m_jsName(jsName),
      m_pBaseMenu(pBaseMenu)
{
#ifdef GRAPHICSUPPORT
    setObjectName("CustomDialog");
#endif
    Interpreter::setCppOwnerShip(this);

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
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
    loadXmlFile(uiXml);
    if (!confirmText.isEmpty())
    {
        oxygine::spButton pOkButton = pObjectManager->createButton(confirmText, 150);
        pOkButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - pOkButton->getScaledWidth() / 2,
                              oxygine::Stage::getStage()->getHeight() - 10 - pOkButton->getScaledHeight());
        addChild(pOkButton);
        pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
        {
            exit();
        });
    }
}

void CustomDialog::loadXmlFile(const QString& uiXml)
{
    UiFactory::getInstance().createUi(uiXml, this);
}

CustomDialog::~CustomDialog()
{
    if (!m_jsName.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        if (pInterpreter != nullptr)
        {
            pInterpreter->deleteObject(m_jsName);
        }
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
