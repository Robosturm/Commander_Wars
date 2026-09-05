#include "objects/dialogs/mapfiledialog.h"
#include "objects/base/spriteobject.h"
#include "game/gamemap.h"
#include <QString>

namespace
{
    const char* const OBJECT_MAP_LAST_MODIFIED = "OBJECT_MAP_LAST_MODIFIED";
    const char* const OBJECT_MAP_NAME = "OBJECT_MAP_NAME";
    const char* const OBJECT_MAP_AUTHOR = "OBJECT_MAP_AUTHOR";
    const char* const OBJECT_MAP_DESCRIPTION = "OBJECT_MAP_DESCRIPTION";
    const char* const OBJECT_MAP_WIDTH = "OBJECT_MAP_WIDTH";
    const char* const OBJECT_MAP_HEIGHT = "OBJECT_MAP_HEIGHT";
    const char* const OBJECT_MAP_PLAYER_COUNT = "OBJECT_MAP_PLAYER_COUNT";
    const char* const OBJECT_MAP_PREVIEW = "OBJECT_MAP_PREVIEW";
}

MapFileDialog::MapFileDialog(QString startFolder, const QStringList & wildcards, bool isSaveDialog, QString startFile, QString acceptButtonName, QColor folderColor)
    : FileDialog(startFolder, wildcards, isSaveDialog, startFile, true, acceptButtonName, folderColor, true, oxygine::Stage::getStage()->getWidth() * 0.4)
{
#ifdef GRAPHICSUPPORT
    setObjectName("MapFileDialog");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    connect(this, &FileDialog::sigSelectedFileChanged, this, &MapFileDialog::selectedFileChanged, Qt::QueuedConnection);
    showFolder(startFolder);
    loadXml("ui/mapfiledialog.xml");
    pApp->continueRendering();
}

void MapFileDialog::loadPreviewHook(const QString & file, QImage & img) const
{
    QFile mapFile(file);
    if (mapFile.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&mapFile);
        GameMap::MapHeaderInfo headerInfo;
        stream.setVersion(QDataStream::Version::Qt_6_5);
        GameMap::readMapHeader(stream, headerInfo);
        img = headerInfo.m_mapPreview;
    }
}

void MapFileDialog::selectedFileChanged(QString file)
{
    QFileInfo fileInfo(file);
    QFile mapFile(file);
    if (mapFile.open(QIODevice::ReadOnly))
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->pauseRendering();
        QDataStream stream(&mapFile);
        GameMap::MapHeaderInfo headerInfo;
        stream.setVersion(QDataStream::Version::Qt_6_5);
        GameMap::readMapHeader(stream, headerInfo);
        auto * pLabel = getCastedObject<Label>(OBJECT_MAP_LAST_MODIFIED);
        if (pLabel)
        {
            pLabel->setText(fileInfo.lastModified().toString(Qt::DateFormat::TextDate));
        }
        pLabel = getCastedObject<Label>(OBJECT_MAP_NAME);
        if (pLabel)
        {
            pLabel->setText(headerInfo.m_mapName);
        }
        pLabel = getCastedObject<Label>(OBJECT_MAP_AUTHOR);
        if (pLabel)
        {
            pLabel->setText(headerInfo.m_mapAuthor);
        }
        pLabel = getCastedObject<Label>(OBJECT_MAP_DESCRIPTION);
        if (pLabel)
        {
            pLabel->setText(headerInfo.m_mapDescription);
        }
        pLabel = getCastedObject<Label>(OBJECT_MAP_WIDTH);
        if (pLabel)
        {
            pLabel->setText(QString::number(headerInfo.m_width));
        }
        pLabel = getCastedObject<Label>(OBJECT_MAP_HEIGHT);
        if (pLabel)
        {
            pLabel->setText(QString::number(headerInfo.m_height));
        }
        pLabel = getCastedObject<Label>(OBJECT_MAP_PLAYER_COUNT);
        if (pLabel)
        {
            pLabel->setText(QString::number(headerInfo.m_playerCount));
        }
        auto* pSpriteObject = getCastedObject<SpriteObject>(OBJECT_MAP_PREVIEW);
        if (pSpriteObject)
        {
            m_mapResAnim = MemoryManagement::create<oxygine::SingleResAnim>();
            pApp->loadResAnim(m_mapResAnim, headerInfo.m_mapPreview, 1, 1, 1.0f);
            pSpriteObject->setResAnim(m_mapResAnim.get());
        }
        pApp->continueRendering();
    }
}
