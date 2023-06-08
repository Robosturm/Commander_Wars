#ifndef FOLDERDIALOG_H
#define FOLDERDIALOG_H

#include <QObject>
#include <QDir>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"

#include "objects/base/textbox.h"
#include "objects/base/panel.h"
#include "objects/base/dropdownmenu.h"

class FolderDialog;
using spFolderDialog = oxygine::intrusive_ptr<FolderDialog>;

/**
 * @brief The FolderDialog class folder dialog for accessing folders.
 * Upon success or cancel the dialog will remove itself from the scene.
 * You have to hold a smart pointer to this object if you need further info from it.
 */
class FolderDialog final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    /**
     * @brief FolderDialog
     * @param startFolder initial folder of the file dialog
     */
    explicit FolderDialog(QString startFolder);
   virtual ~FolderDialog() = default;

signals:
    void sigFolderSelected(QString file);
    void sigCancel();
    void sigShowFolder(QString folder);
    void sigFinished();
public slots:
    void showFolder(QString folder);
    void KeyInput(oxygine::KeyEvent event);
    void deleteItem();
    void remove();
private:
    spTextbox m_CurrentFolder;
    spPanel m_MainPanel;
    oxygine::spButton m_OkButton;
    oxygine::spButton m_CancelButton;
    /**
     * @brief m_Items items directories or folders
     */
    QVector<oxygine::spBox9Sprite> m_Items;
    bool m_focused{true};
};

#endif // FOLDERDIALOG_H
