#ifndef SCRIPTEVENTDIALOG_H
#define SCRIPTEVENTDIALOG_H

#include "QVector"

#include "QColor"

#include "ingamescriptsupport/scriptevent.h"

#include "game/GameEnums.h"

class ScriptEventDialog : public ScriptEvent
{
    Q_OBJECT
public:
    static const QString ScriptEventDialogItem;

    struct Dialog
    {
        QString text;
        QString coid;
        GameEnums::COMood mood{GameEnums::COMood_Normal};
        QColor color;
    };

    ScriptEventDialog();
    /**
     * @brief readEvent
     * @param rStream
     */
    virtual void readEvent(QTextStream& rStream) override;
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
    Dialog* getDialog(qint32 index);
private:
    QVector<Dialog> m_Dialog;
};

#endif // SCRIPTEVENTDIALOG_H
