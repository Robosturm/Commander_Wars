#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "ui_reader/createdgui.h"
#include "ui_reader/uifactory.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"


#include "objects/base/moveinbutton.h"
#include "objects/dialogs/filedialog.h"
#include "objects/dialogs/folderdialog.h"
#include "objects/dialogs/dialogtextinput.h"

CreatedGui::CreatedGui()
{
    connect(this, &CreatedGui::sigFinished, this, &CreatedGui::remove, Qt::QueuedConnection);
}

CreatedGui::~CreatedGui()
{
    for (auto & pItem : m_factoryUiItem)
    {
        pItem->detach();
    }
    m_factoryUiItem.clear();
}

void CreatedGui::addFactoryUiItem(oxygine::spActor pItem)
{
    pItem->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_factoryUiItem.append(pItem);
    addChild(pItem);
}

void CreatedGui::setEnabled(bool value)
{
    for (auto & item : m_factoryUiItem)
    {
        spMoveInButton pMoveInButton = oxygine::dynamic_pointer_cast<MoveInButton>(item);
        if (pMoveInButton.get() == nullptr)
        {
            item->setEnabled(value);
        }
    }
}

void CreatedGui::resetUi()
{
    for (auto & item : m_factoryUiItem)
    {
        item->detach();
    }
    m_factoryUiItem.clear();
}

void CreatedGui::loadXml(QString xmlFile)
{
    UiFactory::getInstance().createUi(xmlFile, this);
}

void CreatedGui::setObjectEnabled(const QString id, bool value)
{
    for (auto & item : m_factoryUiItem)
    {
        auto* pObject = dynamic_cast<QObject*>(item.get());
        if (pObject != nullptr &&
            pObject->objectName() == id)
        {
            item->setEnabled(value);
        }
    }
}

void CreatedGui::showFileDialog(const QStringList & wildcards, const QString & startFolder, bool isSaveDialog, const QString & jsObject, const QString & jsCallback, QString startFile, bool preview, QString acceptButtonName)
{
    spFileDialog fileDialog = spFileDialog::create(startFolder, wildcards, isSaveDialog, startFile, preview, acceptButtonName);
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, [this, jsObject, jsCallback](QString filename)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args = {pInterpreter->newQObject(this),
                             filename};
        pInterpreter->doFunction(jsObject, jsCallback, args);
    }, Qt::QueuedConnection);
}

void CreatedGui::showFolderDialog(const QString & startFolder, const QString & jsObject, const QString & jsCallback)
{
    spFolderDialog folderDialog = spFolderDialog::create(startFolder);
    addChild(folderDialog);
    connect(folderDialog.get(),  &FolderDialog::sigFolderSelected, this, [this, jsObject, jsCallback](QString foldername)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args = {pInterpreter->newQObject(this),
                             foldername};
        pInterpreter->doFunction(jsObject, jsCallback, args);
    }, Qt::QueuedConnection);
}

void CreatedGui::showTextInputDialog(const QString & text, bool showCancel, const QString & startInput, const QString & jsObject, const QString & jsCallback)
{
    spDialogTextInput pDialogTextInput = spDialogTextInput::create(text, showCancel, startInput);
    addChild(pDialogTextInput);
    connect(pDialogTextInput.get(),  &DialogTextInput::sigTextChanged, this, [this, jsObject, jsCallback](QString foldername)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args = {pInterpreter->newQObject(this),
                             foldername};
        pInterpreter->doFunction(jsObject, jsCallback, args);
    }, Qt::QueuedConnection);
}

QObject* CreatedGui::getObject(const QString & id)
{
    for (auto & item : m_factoryUiItem)
    {
        QObject* pObj = dynamic_cast<QObject*>(item.get());
        if (pObj!= nullptr &&
            pObj->objectName() == id)
        {
            return pObj;
        }
    }
    return nullptr;
}

qint32 CreatedGui::getUiWidth() const
{
    return getWidth();
}

qint32 CreatedGui::getUiHeight() const
{
    return getHeight();
}

void CreatedGui::createSubUi(const QString & uiXml, CreatedGui* pBaseUi)
{
    spCreatedGui pUi = spCreatedGui::create();
    pUi->m_pBaseUi = pBaseUi;
    pUi->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));
    UiFactory::getInstance().createUi(uiXml, pUi.get());
    oxygine::Stage::getStage()->addChild(pUi);
}

void CreatedGui::restart()
{
    CONSOLE_PRINT("Forcing restart to reload required data changed in the options.", GameConsole::eDEBUG);
    removeChildren();
    detach();
    emit Mainapp::getInstance()->sigQuit(1);
}

CreatedGui* CreatedGui::getUiParent()
{
    return m_pBaseUi;
}

void CreatedGui::exit()
{
    emit sigFinished();
}

void CreatedGui::remove()
{
    detach();
}
