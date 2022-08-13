#include "objects/dialogs/mapSelection/mapselectionfilterdialog.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "ui_reader/uifactory.h"

constexpr const char* const MapSelectionFilter = "mapSelectionFilter";

MapSelectionFilterDialog::MapSelectionFilterDialog(MapFilter* filter)
    : m_mapFilter(filter)
{
    setObjectName("MapSelectionFilterDialog");
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

    connect(this, &MapSelectionFilterDialog::sigFinished, this, &MapSelectionFilterDialog::remove, Qt::QueuedConnection);
    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->setGlobal(MapSelectionFilter, pInterpreter->newQObject(this));
    UiFactory::getInstance().createUi("ui/mapSelectionFilterDialog.xml", this);
}

MapSelectionFilterDialog::~MapSelectionFilterDialog()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->deleteObject(MapSelectionFilter);
}

void MapSelectionFilterDialog::exit()
{
    emit sigFinished();
}

void MapSelectionFilterDialog::remove()
{
    detach();
}

bool MapSelectionFilterDialog::getFlagActive(GameEnums::MapFilterFlags flag) const
{
    return m_mapFilter->getFlagActive(flag);
}

void MapSelectionFilterDialog::setFlagActive(GameEnums::MapFilterFlags flag, bool isActive)
{
    m_mapFilter->setFlagActive(flag, isActive);
}

bool MapSelectionFilterDialog::getFlagOptional(GameEnums::MapFilterFlags flag) const
{
    return m_mapFilter->getFlagOptional(flag);
}

void MapSelectionFilterDialog::addToFilter(GameEnums::MapFilterFlags flag, bool active, bool isOptional)
{
    m_mapFilter->addToFilter(flag, active, isOptional);
}

void MapSelectionFilterDialog::removeFromFilter(GameEnums::MapFilterFlags flag)
{
    m_mapFilter->removeFromFilter(flag);
}

void MapSelectionFilterDialog::setFlagOptional(GameEnums::MapFilterFlags flag, bool isOptional)
{
    m_mapFilter->setFlagOptional(flag, isOptional);
}

void MapSelectionFilterDialog::setMinHeight(qint32 value)
{
    m_mapFilter->setMinHeight(value);
}

qint32 MapSelectionFilterDialog::getMinHeight() const
{
    return m_mapFilter->getMinHeight();
}

void MapSelectionFilterDialog::setMaxHeight(qint32 value)
{
    m_mapFilter->setMaxHeight(value);
}

qint32 MapSelectionFilterDialog::getMaxHeight() const
{
    return m_mapFilter->getMaxHeight();
}

void MapSelectionFilterDialog::setMinWidth(qint32 value)
{
    m_mapFilter->setMinWidth(value);
}

qint32 MapSelectionFilterDialog::getMinWidth() const
{
    return m_mapFilter->getMinWidth();
}

void MapSelectionFilterDialog::setMaxWidth(qint32 value)
{
    m_mapFilter->setMaxWidth(value);
}

qint32 MapSelectionFilterDialog::getMaxWidth() const
{
    return m_mapFilter->getMaxWidth();
}
void MapSelectionFilterDialog::setMinPlayer(qint32 value)
{
    m_mapFilter->setMinPlayer(value);
}

qint32 MapSelectionFilterDialog::getMinPlayer() const
{
    return m_mapFilter->getMinPlayer();
}

void MapSelectionFilterDialog::setMaxPlayer(qint32 value)
{
    m_mapFilter->setMaxPlayer(value);
}

qint32 MapSelectionFilterDialog::getMaxPlayer() const
{
    return m_mapFilter->getMaxPlayer();
}

QString MapSelectionFilterDialog::getMapAuthor() const
{
    return m_mapFilter->getMapAuthor();
}

void MapSelectionFilterDialog::setMapAuthor(const QString &newMapAuthor)
{
    m_mapFilter->setMapAuthor(newMapAuthor);
}

QString MapSelectionFilterDialog::getMapName() const
{
    return m_mapFilter->getMapName();
}

void MapSelectionFilterDialog::setMapName(const QString &newMapName)
{
    m_mapFilter->setMapName(newMapName);
}
