#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"

#include "objects/dialogs/filedialog.h"
#include "objects/dialogs/editor/dialogextendmap.h"
#include "objects/base/textbox.h"

#include "coreengine/interpreter.h"
#include "coreengine/globalutils.h"

#include "resource_management/objectmanager.h"

#include "ui_reader/uifactory.h"

DialogExtendMap::DialogExtendMap()
    : CreatedGui()
{
#ifdef GRAPHICSUPPORT
    setObjectName("MapEditDialog");
#endif
    Interpreter::setCppOwnerShip(this);

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("filedialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));
    UiFactory::getInstance().createUi("ui/editor/mapExtendDialog.xml", this);
    connect(this, &DialogExtendMap::sigRemove, this, &DialogExtendMap::remove, Qt::QueuedConnection);
}

void DialogExtendMap::onOk()
{
    emit sigOk(m_mapFile, m_direction);
    emit sigRemove();
}

QString DialogExtendMap::getMapFile() const
{
    return m_mapFile;
}

void DialogExtendMap::setMapFile(const QString newMapFile)
{
    m_mapFile = newMapFile;
}

GameEnums::Directions DialogExtendMap::getDirection() const
{
    return m_direction;
}

void DialogExtendMap::setDirection(const GameEnums::Directions newDirection)
{
    m_direction = newDirection;
}

void DialogExtendMap::cancel()
{
    emit sigCanceled();
    emit sigRemove();
}

void DialogExtendMap::remove()
{
    detach();
}

void DialogExtendMap::mapFileChanged(QString file)
{
    file = GlobalUtils::makePathRelative(file);
    for (auto & item : m_factoryUiItem)
    {
        spTextbox pText = std::dynamic_pointer_cast<Textbox>(item);
        if (pText.get() != nullptr &&
            pText->objectName() == "MapTextbox")
        {
            pText->setCurrentText(file);
            m_mapFile = file;
        }
    }
}

void DialogExtendMap::showSelectMap()
{
    QStringList wildcards;
    wildcards.append("*.map");
    QString path = Settings::userPath() + "maps";
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(path, wildcards, false, "", false, tr("Load"));
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &DialogExtendMap::mapFileChanged, Qt::QueuedConnection);
}
