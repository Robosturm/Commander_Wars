#ifndef COBANNLISTDIALOG_H
#define COBANNLISTDIALOG_H

#include <QObject>
#include <QVector>

#include "oxygine-framework.h"

#include "objects/checkbox.h"
#include "objects/dropdownmenu.h"

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
public slots:
    void setCOBannlist(qint32 item);
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ToggleAll;
    oxygine::spButton m_ExitButton;
    spDropDownmenu m_PredefinedLists;
    QVector<spCheckbox> m_Checkboxes;
    bool toggle{true};
    QStringList m_CurrentCOBannList;
};

#endif // COBANNLISTDIALOG_H
