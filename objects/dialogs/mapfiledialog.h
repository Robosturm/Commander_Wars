#pragma once

#include "3rd_party/oxygine-framework/oxygine/res/SingleResAnim.h"
#include "objects/dialogs/filedialog.h"
#include "ui_reader/createdgui.h"

class MapFileDialog;
using spMapFileDialog = std::shared_ptr<MapFileDialog>;

class MapFileDialog final : public FileDialog
{
    Q_OBJECT
public:
    explicit MapFileDialog(QString startFolder, const QStringList & wildcards, bool isSaveDialog, QString startFile = "", QString acceptButtonName = tr("Ok"), QColor folderColor = Settings::getInstance()->getMapFolderColor());
    virtual ~MapFileDialog() = default;
    void loadPreviewHook(const QString & file, QImage & img) const override;
public slots:
    void selectedFileChanged(QString file);
private:
    oxygine::spSingleResAnim m_mapResAnim;
};