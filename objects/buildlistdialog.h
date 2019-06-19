#ifndef BUILDLISTDIALOG_H
#define BUILDLISTDIALOG_H

#include <QObject>
#include <QVector>

#include "oxygine-framework.h"

#include "objects/checkbox.h"
#include "objects/dropdownmenu.h"

class BuildListDialog;
typedef oxygine::intrusive_ptr<BuildListDialog> spBuildListDialog;

class BuildListDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit BuildListDialog(qint32 player, QStringList buildList);
    virtual ~BuildListDialog() = default;
signals:
    void editFinished(qint32 player, QStringList color);
    void canceled();
public slots:
    void setBuildlist(qint32 item);
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ToggleAll;
    oxygine::spButton m_ExitButton;
    spDropDownmenu m_PredefinedLists;
    QVector<spCheckbox> m_Checkboxes;
    bool toggle{true};
    QStringList m_CurrentBuildList;
    qint32 m_Player;
};

#endif // BUILDLISTDIALOG_H
