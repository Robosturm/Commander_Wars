#ifndef SCRIPTDIALOGDIALOG_H
#define SCRIPTDIALOGDIALOG_H

#include <QObject>

#include "qvector.h"


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "ingamescriptsupport/events/scripteventdialog.h"

#include "objects/base/panel.h"

#include "objects/base/dropdownmenu.h"
#include "objects/base/dropdownmenucolor.h"
#include "objects/base/textbox.h"

class ScriptDialogDialog;
typedef oxygine::intrusive_ptr<ScriptDialogDialog> spScriptDialogDialog;

class ScriptDialogDialog : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit ScriptDialogDialog(spScriptEventDialog scriptEventDialog);
    virtual ~ScriptDialogDialog() = default;
signals:
    /**
     * @brief sigFinished
     */
    void sigFinished();
    /**
     * @brief sigRemoveLast
     */
    void sigRemoveLast();
    /**
     * @brief sigAddItem
     */
    void sigAddItem();
    /**
     * @brief sigShowChangeBackground
     */
    void sigShowChangeBackground();
public slots:
    /**
     * @brief addItem
     */
    void addItem();
    /**
     * @brief removeLast
     */
    void removeLast();
    /**
     * @brief updateDialog
     */
    void updateDialog();
    /**
     * @brief showChangeBackground
     */
    void showChangeBackground();
    /**
     * @brief setCurrentDialogBackground
     * @param file
     */
    void setCurrentDialogBackground(QString file);
private:
    void addActorItem(qint32 i, qint32 panelWidth);
    qint32 getPanelWidth();
    void loadBackground(QString filename, qint32 index);
private:
    oxygine::spBox9Sprite m_pSpriteBox;
    spScriptEventDialog m_Event;
    qint32 dialogIndex{-1};
    spPanel m_Panel;
    QVector<oxygine::spActor> m_dialogItems;
    QVector<oxygine::spSprite> m_backgrounds;
    QVector<oxygine::spResAnim> m_backgroundAnims;
};

#endif // SCRIPTDIALOGDIALOG_H
