#include "objects/dialogs/editor/mapeditdialog.h"

#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"

#include "objects/dialogs/filedialog.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "ui_reader/uifactory.h"

constexpr const char* const MapEdit = "mapEdit";

MapEditDialog::MapEditDialog(MapEditInfo info)
    : m_info(info)
{
    setObjectName("MapEditDialog");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
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
    pInterpreter->deleteObject(MapEdit);
}

void MapEditDialog::cancel()
{
    emit sigCanceled();
}

void MapEditDialog::finished()
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
    m_info.mapName = name;
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
        }
    }
}

void MapEditDialog::showSelectScript()
{    
    QVector<QString> wildcards;
    wildcards.append("*.js");
    QString path = Settings::getUserPath() + "maps";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards);
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &MapEditDialog::scriptFileChanged, Qt::QueuedConnection);    
}
