#include "objects/dialogs/mapfiledialog.h"
#include "game/gamemap.h"

MapFileDialog::MapFileDialog(QString startFolder, const QStringList & wildcards, bool isSaveDialog, QString startFile, QString acceptButtonName, QColor folderColor)
    : FileDialog(startFolder, wildcards, isSaveDialog, startFile, true, acceptButtonName, folderColor, true)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    showFolder(startFolder);
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