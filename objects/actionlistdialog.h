#ifndef ACTIONLISTDIALOG_H
#define ACTIONLISTDIALOG_H

#include <QObject>
#include <QVector>

#include "oxygine-framework.h"

#include "objects/checkbox.h"
#include "objects/dropdownmenu.h"

class ActionListDialog;
typedef oxygine::intrusive_ptr<ActionListDialog> spActionListDialog;

class ActionListDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit ActionListDialog(QStringList bannlist);
    virtual ~ActionListDialog() = default;
signals:
    void editFinished(QStringList actionList);
    void canceled();
    void sigShowSaveBannlist();
public slots:
    /**
     * @brief setBuildlist
     * @param item
     */
    void setBuildlist(qint32 item);
    /**
     * @brief showSaveBannlist
     */
    void showSaveBannlist();
    /**
     * @brief saveBannlist
     */
    void saveBannlist(QString filename);
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ToggleAll;
    oxygine::spButton m_ExitButton;
    spDropDownmenu m_PredefinedLists;
    QVector<spCheckbox> m_Checkboxes;
    bool toggle{true};
    QStringList m_CurrentActionList;
};

#endif // ACTIONLISTDIALOG_H
