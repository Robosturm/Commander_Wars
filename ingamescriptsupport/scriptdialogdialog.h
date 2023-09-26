#ifndef SCRIPTDIALOGDIALOG_H
#define SCRIPTDIALOGDIALOG_H

#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

#include "ingamescriptsupport/events/scripteventdialog.h"

#include "objects/base/panel.h"

class ScriptDialogDialog;
using spScriptDialogDialog = std::shared_ptr<ScriptDialogDialog>;

class ScriptDialogDialog final : public QObject, public oxygine::Actor
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
private slots:
    void remove();
private:
    void addActorItem(qint32 i, qint32 panelWidth);
    qint32 getPanelWidth();
    void loadBackground(QString filename, qint32 index);
private:
    oxygine::spBox9Sprite m_pSpriteBox;
    spScriptEventDialog m_Event;
    qint32 dialogIndex{-1};
    spPanel m_Panel;
    QVector<QVector<oxygine::spActor>> m_dialogItems;
    QVector<oxygine::spSprite> m_backgrounds;
    QVector<oxygine::spResAnim> m_backgroundAnims;
};

#endif // SCRIPTDIALOGDIALOG_H
