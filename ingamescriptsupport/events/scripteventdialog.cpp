#include "scripteventdialog.h"

#include "ingamescriptsupport/scriptdialogdialog.h"

#include "ingamescriptsupport/scripteditor.h"

const QString ScriptEventDialog::ScriptEventDialogItem = "ScriptEventDialogItem";

ScriptEventDialog::ScriptEventDialog()
    : ScriptEvent (EventType::dialog)
{

}

void ScriptEventDialog::addDialog(QString text, QString coid, GameEnums::COMood mood, QColor color)
{
    Dialog dialog;
    dialog.text = text;
    dialog.coid = coid;
    dialog.mood = mood;
    dialog.color = color;
    m_Dialog.append(dialog);
}

void ScriptEventDialog::showEditEvent(spScriptEditor pScriptEditor)
{
    spScriptDialogDialog pScriptDialogDialog = new ScriptDialogDialog(this);
    pScriptEditor->addChild(pScriptDialogDialog);
    connect(pScriptDialogDialog.get(), &ScriptDialogDialog::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}

void ScriptEventDialog::removeDialog(qint32 index)
{
    if (index >= 0 && index < m_Dialog.size())
    {
        m_Dialog.removeAt(index);
    }
}

ScriptEventDialog::Dialog* ScriptEventDialog::getDialog(qint32 index)
{
    if (index >= 0 && index < m_Dialog.size())
    {
        return &m_Dialog[index];
    }
    return nullptr;
}

void ScriptEventDialog::readEvent(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    while (!rStream.atEnd())
    {
        line = rStream.readLine().simplified();
        if (line.endsWith(EventDialog))
        {
            break;
        }
        else if (line.endsWith(ScriptEventDialogItem))
        {
            line = line.simplified();

            QStringList items = line.replace("var dialog" + QString::number(m_Dialog.size()) + " = GameAnimationFactory.createGameAnimationDialog(qsTr(\"", "")
                                    .replace("\"), \"", ",")
                                    .replace("\", GameEnums.COMood_", ",")
                                    .replace(", \"", ",")
                                    .replace("\"); // " + ScriptEventDialogItem, "").split(",");
            if (items.size() == 4)
            {
                Dialog dialog;
                dialog.text = items[0];
                dialog.coid = items[1];
                if (items[2] == "Sad")
                {
                    dialog.mood = GameEnums::COMood_Sad;
                }
                else if (items[2] == "Normal")
                {
                    dialog.mood = GameEnums::COMood_Normal;
                }
                else if (items[2] == "Happy")
                {
                    dialog.mood = GameEnums::COMood_Happy;
                }
                dialog.color = items[3];
                m_Dialog.append(dialog);
            }
        }
    }
}

void ScriptEventDialog::writeEvent(QTextStream& rStream)
{
    rStream << "            // " + EventDialog + "\n";
    for (qint32 i = 0; i < m_Dialog.size(); i++)
    {
        rStream <<  "            var dialog" << QString::number(i) << " = GameAnimationFactory.createGameAnimationDialog(qsTr(\"";
        rStream <<  m_Dialog[i].text;
        rStream << "\"), \"" << m_Dialog[i].coid << "\", GameEnums.COMood_";
        switch (m_Dialog[i].mood)
        {
            case GameEnums::COMood_Sad:
            {
                rStream << "Sad";
                break;
            }
            case GameEnums::COMood_Happy:
            {
                rStream << "Happy";
                break;
            }
            case GameEnums::COMood_Normal:
            {
                rStream << "Normal";
                break;
            }
        }
        rStream << ", \"" << m_Dialog[i].color.name() << "\"); // " << ScriptEventDialogItem << "\n";
    }
    for (qint32 i = 0; i < m_Dialog.size() - 1; i++)
    {
        rStream << "            dialog" << QString::number(i) << ".queueAnimation(dialog" << QString::number(i + 1) << ");\n";
    }
    rStream << "            // " + EventDialog + "\n";
}
