#ifndef COBANNLISTDIALOG_H
#define COBANNLISTDIALOG_H

#include <QObject>
#include <QVector>


#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "objects/base/checkbox.h"
#include "objects/base/dropdownmenu.h"

class COBannListDialog;
using spCOBannListDialog = oxygine::intrusive_ptr<COBannListDialog>;

class COBannListDialog final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit COBannListDialog(QStringList cobannlist);
    ~COBannListDialog() = default;
signals:
    void editFinished(QStringList cobannlist);
    void canceled();
    void sigShowSaveBannlist();
    void sigFinished();
public slots:
    void setCOBannlist(qint32 item);
    /**
     * @brief showSaveBannlist
     */
    void showSaveBannlist();
    /**
     * @brief saveBannlist
     */
    void saveBannlist(QString filename);
private slots:
    void remove();
private:
    QStringList getNameList();
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ToggleAll;
    oxygine::spButton m_ExitButton;
    spDropDownmenu m_PredefinedLists;
    QVector<spCheckbox> m_Checkboxes;
    QStringList m_COIDs;
    bool m_toggle{true};
    QStringList m_CurrentCOBannList;
};

#endif // COBANNLISTDIALOG_H
