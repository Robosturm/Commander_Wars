#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/dialogs/editor/mapeditdialog.h"

#include "coreengine/interpreter.h"
#include "coreengine/globalutils.h"

#include "objects/dialogs/filedialog.h"
#include "objects/dialogs/dialogmessagebox.h"

#include "resource_management/objectmanager.h"

#include "ui_reader/uifactory.h"

constexpr const char* const MapEdit = "mapEdit";

MapEditDialog::MapEditDialog(MapEditInfo info, const QString & confirmMessage)
    : m_info(info),
      m_confirmMessage(confirmMessage)
{
#ifdef GRAPHICSUPPORT
    setObjectName("MapEditDialog");
#endif
    Interpreter::setCppOwnerShip(this);

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    connect(this, &MapEditDialog::sigCanceled, this, &MapEditDialog::remove, Qt::QueuedConnection);
    connect(this, &MapEditDialog::sigFinished, this, &MapEditDialog::remove, Qt::QueuedConnection);
    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->setGlobal(MapEdit, pInterpreter->newQObject(this));
    UiFactory::getInstance().createUi("ui/mapEditDialog.xml", this);
}

MapEditDialog::~MapEditDialog()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (pInterpreter != nullptr)
    {
        pInterpreter->deleteObject(MapEdit);
    }
}

void MapEditDialog::cancel()
{
    emit sigCanceled();
}

void MapEditDialog::finished()
{
    if (!m_confirmMessage.isEmpty())
    {
        spDialogMessageBox pDialogConfirm = spDialogMessageBox::create(m_confirmMessage, true);
        connect(pDialogConfirm.get(), &DialogMessageBox::sigOk, this, &MapEditDialog::onConfirm, Qt::QueuedConnection);
        addChild(pDialogConfirm);
    }
    else
    {
        onConfirm();
    }
}

void MapEditDialog::onConfirm()
{
    emit editFinished(m_info);
    emit sigFinished();
}

void MapEditDialog::setMapName(QString name)
{
    m_info.mapName = name;
}

QString MapEditDialog::getMapName() const
{
    return m_info.mapName;
}

void MapEditDialog::setMapAuthor(QString name)
{
    m_info.author = name;
}

QString MapEditDialog::getMapAuthor() const
{
    return m_info.author;
}

void MapEditDialog::setMapDescription(QString name)
{
    m_info.description = name;
}

QString MapEditDialog::getMapDescription() const
{
    return m_info.description;
}

void MapEditDialog::setMapScript(QString name)
{
    m_info.scriptFile = name;
}

QString MapEditDialog::getMapScript() const
{
    return m_info.scriptFile;
}

void MapEditDialog::setMapWidth(qint32 value)
{
    m_info.mapWidth = value;
}

qint32 MapEditDialog::getMapWidth() const
{
    return m_info.mapWidth;
}

void MapEditDialog::setMapHeight(qint32 value)
{
    m_info.mapHeigth = value;
}

qint32 MapEditDialog::getMapHeight() const
{
    return m_info.mapHeigth;
}

void MapEditDialog::setMapPlayerCount(qint32 value)
{
    m_info.playerCount = value;
}

qint32 MapEditDialog::getMapPlayerCount() const
{
    return m_info.playerCount;
}

void MapEditDialog::setMapTurnLimit(qint32 value)
{
    m_info.turnLimit = value;
}

qint32 MapEditDialog::getMapTurnLimit() const
{
    return m_info.turnLimit;
}

void MapEditDialog::setMapDeployLimit(qint32 value)
{
    m_info.deployLimit = value;
}

qint32 MapEditDialog::getMapDeployLimit() const
{
    return m_info.deployLimit;
}

void MapEditDialog::setMapFlag(GameEnums::MapFilterFlags flag, bool value)
{
    if (value)
    {
        m_info.mapFlags = static_cast<GameEnums::MapFilterFlags>(m_info.mapFlags | flag);
    }
    else
    {
        m_info.mapFlags = static_cast<GameEnums::MapFilterFlags>(m_info.mapFlags & !flag);
    }
}

bool MapEditDialog::getMapFlag(GameEnums::MapFilterFlags flag) const
{
    return (m_info.mapFlags & flag) > 0;
}

void MapEditDialog::remove()
{
    detach();
}

void MapEditDialog::scriptFileChanged(QString file)
{
    file = GlobalUtils::makePathRelative(file);
    for (auto & item : m_factoryUiItem)
    {
        spTextbox pText = oxygine::dynamic_pointer_cast<Textbox>(item);
        if (pText.get() != nullptr &&
            pText->objectName() == "ScriptTextbox")
        {
            pText->setCurrentText(file);
            m_info.scriptFile = file;
        }
    }
}

void MapEditDialog::showSelectScript()
{    
    QStringList wildcards;
    wildcards.append("*.js");
    QString path = Settings::getUserPath() + "maps";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards, false, "", false, tr("Load"));
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &MapEditDialog::scriptFileChanged, Qt::QueuedConnection);    
}
