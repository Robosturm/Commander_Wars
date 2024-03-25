#ifndef ACTIONLISTDIALOG_H
#define ACTIONLISTDIALOG_H

#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "objects/base/checkbox.h"
#include "objects/base/dropdownmenu.h"

class GameMap;
class ActionListDialog;
using spActionListDialog = std::shared_ptr<ActionListDialog>;

class ActionListDialog final : public QObject, public oxygine::Actor
{
    Q_OBJECT
    static const char* const FILEPATH;
public:
    explicit ActionListDialog(QStringList bannlist, GameMap* pMap);
    virtual ~ActionListDialog() = default;
signals:
    void editFinished(QStringList actionList);
    void canceled();
    void sigShowSaveBannlist();
    void sigShowDeleteBannlist();
    void sigDoSaveBannlist(QString filename);
    void sigDeleteBannlist(const QString file);
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
    void doSaveBannlist(QString filename);
    void showDeleteBannlist();
    void deleteBannlist(const QString file);
private:
    /**
     * @brief getNameList
     */
    QStringList getNameList();
    void updatePredefinedList();
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_ToggleAll;
    oxygine::spButton m_ExitButton;
    oxygine::spBox9Sprite m_pSpriteBox;
    spDropDownmenu m_PredefinedLists;
    QVector<spCheckbox> m_Checkboxes;
    QStringList m_actionList;
    bool m_toggle{true};
    QStringList m_CurrentActionList;
    GameMap* m_pMap{nullptr};
};

#endif // ACTIONLISTDIALOG_H
