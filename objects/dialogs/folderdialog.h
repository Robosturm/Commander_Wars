#ifndef FOLDERDIALOG_H
#define FOLDERDIALOG_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "objects/base/textbox.h"
#include "objects/base/panel.h"
#include "objects/base/dropdownmenu.h"


class FolderDialog;
typedef oxygine::intrusive_ptr<FolderDialog> spFolderDialog;

/**
 * @brief The FolderDialog class folder dialog for accessing folders.
 * Upon success or cancel the dialog will remove itself from the scene.
 * You have to hold a smart pointer to this object if you need further info from it.
 */
class FolderDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    /**
     * @brief FolderDialog
     * @param startFolder initial folder of the file dialog
     */
    explicit FolderDialog(QString startFolder);
    virtual ~FolderDialog();

signals:
    void sigFolderSelected(QString file);
    void sigCancel();
    void sigShowFolder(QString folder);
public slots:
    void showFolder(QString folder);
    void KeyInput(oxygine::KeyEvent event);
    void deleteItem();

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
