#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "objects/base/textbox.h"
#include "objects/base/panel.h"
#include "objects/base/dropdownmenu.h"


class FileDialog;
typedef oxygine::intrusive_ptr<FileDialog> spFileDialog;

/**
 * @brief The FileDialog class file dialog for accessing folder and files.
 * Upon success or cancel the dialog will remove itself from the scene.
 * You have to hold a smart pointer to this object if you need further info from it.
 */
class FileDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    /**
     * @brief FileDialog
     * @param startFolder initial folder of the file dialog
     * @param wildcards wildcard items will be split at ;
     */
    explicit FileDialog(QString startFolder, QVector<QString> wildcards, QString startFile = "", bool preview = false);
    virtual ~FileDialog();

    bool getPreview() const;
    void setPreview(bool preview);

signals:
    void sigFileSelected(QString file);
    void sigCancel();
    void sigShowFolder(QString folder);
public slots:
    void showFolder(QString folder);
    void filterChanged(qint32);
    void KeyInput(oxygine::KeyEvent event);
    void deleteItem();
private:
    spTextbox m_CurrentFolder;
    spTextbox m_CurrentFile;
    spPanel m_MainPanel;
    oxygine::spButton m_OkButton;
    spDropDownmenu m_DropDownmenu;
    oxygine::spButton m_CancelButton;
    /**
     * @brief m_Items items directories or folders
     */
    QVector<oxygine::spBox9Sprite> m_Items;
    bool m_preview{false};
    bool m_focused{true};
    QVector<oxygine::spResAnim> m_ResAnims;
};

#endif // FILEDIALOG_H
