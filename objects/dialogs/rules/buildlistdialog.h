#ifndef BUILDLISTDIALOG_H
#define BUILDLISTDIALOG_H

#include <QObject>
#include <QVector>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/checkbox.h"
#include "objects/base/dropdownmenu.h"

class BuildListDialog;
typedef oxygine::intrusive_ptr<BuildListDialog> spBuildListDialog;

class BuildListDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
    struct UnitGroup
    {
        QString name;
        QStringList units;
    };
public:
    explicit BuildListDialog(qint32 player, QStringList buildList);
    virtual ~BuildListDialog() = default;
signals:
    void editFinished(qint32 player, QStringList buildList);
    void canceled();
    void sigShowSaveBannlist();
    void sigFinished();
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
private slots:
    void remove();
private:
    QVector<QString> getNameList();
    QVector<UnitGroup> getUnitGroups();
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ToggleAll;
    oxygine::spButton m_ExitButton;
    spDropDownmenu m_PredefinedLists;
    QVector<spCheckbox> m_Checkboxes;
    QStringList m_UnitList;
    bool m_toggle{true};
    QStringList m_CurrentBuildList;
    qint32 m_Player;
};

#endif // BUILDLISTDIALOG_H
