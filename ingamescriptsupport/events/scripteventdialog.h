#ifndef SCRIPTEVENTDIALOG_H
#define SCRIPTEVENTDIALOG_H

#include "QVector"
#include "QColor"

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "ingamescriptsupport/events/scriptevent.h"

#include "game/GameEnums.h"

class DialogEntry;
typedef oxygine::intrusive_ptr<DialogEntry> spDialogEntry;

class DialogEntry : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    QString text;
    QString coid;
    GameEnums::COMood mood{GameEnums::COMood_Normal};
    QColor color;
    QString background;
};

class ScriptEventDialog;
typedef oxygine::intrusive_ptr<ScriptEventDialog> spScriptEventDialog;

class ScriptEventDialog : public ScriptEvent
{
    Q_OBJECT
public:
    static const QString ScriptEventDialogItem;
    static const QString m_CurrentPlayerCO0;
    static const QString m_CurrentPlayerCO1;

    ScriptEventDialog();
    /**
     * @brief readEvent
     * @param rStream
     */
    virtual void readEvent(QTextStream& rStream, QString line) override;
    /**
     * @brief writeEvent
     * @param rStream
     */
    virtual void writeEvent(QTextStream& rStream) override;
    /**
     * @brief getDialogSize
     * @return
     */
    qint32 getDialogSize()
    {
        return m_Dialog.size();
    }
    /**
     * @brief getDialog
     * @param index
     * @return
     */
    void addDialog(QString text, QString coid, GameEnums::COMood mood, QColor color);
    /**
     * @brief getDialog
     * @param index
     * @return
     */
    spDialogEntry getDialog(qint32 index);
    /**
     * @brief getDialog
     * @param index
     * @return
     */
    void removeDialog(qint32 index);
    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() override
    {
        return tr("Dialog: ") + QString::number(m_Dialog.size());
    }
    /**
     * @brief showEditEvent
     */
    virtual void showEditEvent(spScriptEditor pScriptEditor) override;
    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 1;
    }
private:
    QVector<spDialogEntry> m_Dialog;
};

#endif // SCRIPTEVENTDIALOG_H
