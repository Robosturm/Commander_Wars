#ifndef ACTIONLISTDIALOG_H
#define ACTIONLISTDIALOG_H

#include <QObject>
#include <QVector>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/checkbox.h"
#include "objects/base/dropdownmenu.h"

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
    /**
     * @brief getNameList
     */
    QVector<QString> getNameList();
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
