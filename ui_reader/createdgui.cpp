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
    Interpreter::setCppOwnerShip(this);
    connect(this, &CreatedGui::sigFinished, this, &CreatedGui::remove, Qt::QueuedConnection);
}

CreatedGui::~CreatedGui()
{
    for (auto & pItem : m_factoryUiItem)
    {
        pItem->detachAndRemove();
    }
    m_factoryUiItem.clear();
}

void CreatedGui::addFactoryUiItem(oxygine::spActor pItem)
{
    pItem->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_factoryUiItem.append(pItem);
}

void CreatedGui::setEnabled(bool value)
{
    for (auto & item : m_factoryUiItem)
    {
        spMoveInButton pMoveInButton = std::dynamic_pointer_cast<MoveInButton>(item);
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
        item->detachAndRemove();
    }
    m_factoryUiItem.clear();
}

void CreatedGui::loadXml(QString xmlFile)
{
    Mainapp::getInstance()->pauseRendering();
    UiFactory::getInstance().createUi(xmlFile, this);
    Mainapp::getInstance()->continueRendering();
}

void CreatedGui::reloadUi(QString xmlFile)
{
    resetUi();
    loadXml(xmlFile);
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

void CreatedGui::showFileDialog(const QStringList & wildcards, const QString & startFolder, bool isSaveDialog, const QString & jsObject, const QString & jsCallback, const QString & startFile, bool preview, const QString & acceptButtonName)
{
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(startFolder, wildcards, isSaveDialog, startFile, preview, acceptButtonName);
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
    spFolderDialog folderDialog = MemoryManagement::create<FolderDialog>(startFolder);
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
    spDialogTextInput pDialogTextInput = MemoryManagement::create<DialogTextInput>(text, showCancel, startInput);
    addChild(pDialogTextInput);
    connect(pDialogTextInput.get(),  &DialogTextInput::sigTextChanged, this, [this, jsObject, jsCallback](QString foldername)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args = {pInterpreter->newQObject(this),
                             foldername};
        pInterpreter->doFunction(jsObject, jsCallback, args);
    }, Qt::QueuedConnection);
}

QObject* CreatedGui::getObject(const QString id)
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
    spCreatedGui pUi = MemoryManagement::create<CreatedGui>();
    pUi->m_pBaseUi = pBaseUi;
    pUi->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));
    UiFactory::getInstance().createUi(uiXml, pUi.get());
    oxygine::Stage::getStage()->addChild(pUi);
}

void CreatedGui::restart()
{
    CONSOLE_PRINT("Forcing restart to reload required data changed in the options.", GameConsole::eDEBUG);
    removeChildren();
    detachAndRemove();
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
    detachAndRemove();
}

void CreatedGui::deleteObject(const QString & id)
{
    auto iter = m_factoryUiItem.begin();
    while (iter != m_factoryUiItem.end())
    {
        QObject* pObj = dynamic_cast<QObject*>(iter->get());
        if (pObj!= nullptr &&
            pObj->objectName() == id)
        {
            iter->get()->detach();
            iter = m_factoryUiItem.erase(iter);
            break;
        }
    }
}

void CreatedGui::changeBackground(QString background)
{
    if (m_backgroundSprite.get() == nullptr)
    {
        m_backgroundSprite= MemoryManagement::create<oxygine::Sprite>();
        m_backgroundSprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
    }
    addChild(m_backgroundSprite);

    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim(background);
    if (Interpreter::exists())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args({pInterpreter->newQObject(this),
                           background});
        QJSValue erg;
        if (pInterpreter->exists("BACKGROUNDSELECTOR", "getBackgroundSprites" + objectName()))
        {
            erg = pInterpreter->doFunction("BACKGROUNDSELECTOR", "getBackgroundSprites" + objectName(), args);
        }
        else
        {
            erg = pInterpreter->doFunction("BACKGROUNDSELECTOR", "getBackgroundSprites", args);
        }
        if (erg.isString())
        {
            background = erg.toString();
            pBackground = pBackgroundManager->getResAnim(background);
        }
        else if (erg.isArray())
        {
            float targetRatio = static_cast<float>(oxygine::Stage::getStage()->getWidth()) / static_cast<float>(oxygine::Stage::getStage()->getHeight());
            float minDiff = std::numeric_limits<float>::max();
            auto sprites = erg.toVariant().toStringList();
            for (const auto & sprite : std::as_const(sprites))
            {
                auto* pAnim = pBackgroundManager->getResAnim(sprite);
                if (pAnim != nullptr && pAnim->getHeight() > 0)
                {
                    float ratio = static_cast<float>(pAnim->getWidth()) / static_cast<float>(pAnim->getHeight());
                    auto diff = qAbs(ratio - targetRatio);
                    if (diff < minDiff)
                    {
                        minDiff = diff;
                        pBackground = pAnim;
                    }
                }
            }
        }
    }
    if (pBackground != nullptr &&
        pBackground->getWidth() > 0 &&
        pBackground->getHeight() > 0)
    {
        m_backgroundSprite->setResAnim(pBackground);
        // background should be last to draw
        m_backgroundSprite->setScaleX(static_cast<float>(oxygine::Stage::getStage()->getWidth()) / static_cast<float>(pBackground->getWidth()));
        m_backgroundSprite->setScaleY(static_cast<float>(oxygine::Stage::getStage()->getHeight()) / static_cast<float>(pBackground->getHeight()));
    }
}