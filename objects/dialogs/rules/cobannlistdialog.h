#ifndef COBANNLISTDIALOG_H
#define COBANNLISTDIALOG_H

#include <QObject>
#include <QVector>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/checkbox.h"
#include "objects/base/dropdownmenu.h"

class COBannListDialog;
typedef oxygine::intrusive_ptr<COBannListDialog> spCOBannListDialog;

class COBannListDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit COBannListDialog(QStringList cobannlist);
    virtual ~COBannListDialog() = default;
signals:
    void editFinished(QStringList cobannlist);
    void canceled();
    void sigShowSaveBannlist();
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
private:
    QVector<QString> getNameList();
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ToggleAll;
    oxygine::spButton m_ExitButton;
    spDropDownmenu m_PredefinedLists;
    QVector<spCheckbox> m_Checkboxes;
    QStringList m_COIDs;
    bool toggle{true};
    QStringList m_CurrentCOBannList;
};

#endif // COBANNLISTDIALOG_H
