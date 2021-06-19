#include "ingamescriptsupport/events/scripteventgeneric.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"

#include "objects/base/spinbox.h"
#include "objects/base/checkbox.h"
#include "objects/base/dropdownmenu.h"
#include "objects/base/textbox.h"
#include "objects/base/label.h"
#include "objects/dialogs/filedialog.h"

ScriptEventGeneric::ScriptEventGeneric(EventType type, QString eventIdentifier)
    : ScriptEvent (type),
      m_eventIdentifier(eventIdentifier)
{
    connect(this, &ScriptEventGeneric::sigShowSelectFile, this, &ScriptEventGeneric::showSelectFile, Qt::QueuedConnection);
}

void ScriptEventGeneric::readEvent(QTextStream& rStream, QString line)
{
    line = line.simplified();
    removeCustomStart(line);
    for (auto & item : m_Items)
    {
        line.replace(item.beforeCode, "");
        line.replace(item.afterCode, "@");
    }
    QStringList items = line.split("@");
    if (items.size() >= m_Items.size())
    {
        for (qint32 i = 0; i < m_Items.size(); i++)
        {
            m_Items[i].item = items[i];
        }
    }
}

void ScriptEventGeneric::writeEvent(QTextStream& rStream)
{
    rStream <<  "            ";
    writeCustomStart(rStream);
    for (qint32 i = 0; i < m_Items.size(); i++)
    {
        rStream << m_Items[i].beforeCode;
        rStream << m_Items[i].item;
        rStream << m_Items[i].afterCode;
    }
    rStream <<  " // " << QString::number(getVersion()) << " " << m_eventIdentifier << "\n";
}

void ScriptEventGeneric::showEditEvent(spScriptEditor pScriptEditor)
{
    spGenericBox pBox = spGenericBox::create();

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;
    qint32 y = 30;
    for (qint32 i = 0; i < m_Items.size(); i++)
    {
        spLabel pText = spLabel::create(width - 10);
        pText->setStyle(style);
        pText->setHtmlText(m_Items[i].labelText);
        pText->setPosition(30, y);
        pBox->addItem(pText);
        switch (m_Items[i].types)
        {
            case EditTypes::Float:
            {
                spSpinBox spinBox = spSpinBox::create(300, m_Items[i].min, m_Items[i].max, SpinBox::Mode::Float);
                spinBox->setInfinityValue(m_Items[i].infinite);
                spinBox->setTooltipText(m_Items[i].tooltip);
                spinBox->setPosition(width, y);
                spinBox->setCurrentValue(m_Items[i].item.toFloat());
                connect(spinBox.get(), &SpinBox::sigValueChanged,
                        [=](qreal value)
                {
                    m_Items[i].item = QString::number(value);
                });
                pBox->addItem(spinBox);
                break;
            }
            case EditTypes::Integer:
            {

                spSpinBox spinBox = spSpinBox::create(300, m_Items[i].min, m_Items[i].max);
                spinBox->setInfinityValue(m_Items[i].infinite);
                spinBox->setTooltipText(m_Items[i].tooltip);
                spinBox->setPosition(width, y);
                spinBox->setCurrentValue(m_Items[i].item.toInt());
                connect(spinBox.get(), &SpinBox::sigValueChanged,
                        [=](qreal value)
                {
                    m_Items[i].item = QString::number(value);
                });
                pBox->addItem(spinBox);
                break;
            }
            case EditTypes::String:
            {
                spTextbox textBox = spTextbox::create(300);
                textBox->setTooltipText(m_Items[i].tooltip);
                textBox->setPosition(width, y);
                textBox->setCurrentText(m_Items[i].item);
                connect(textBox.get(), &Textbox::sigTextChanged,
                        [=](QString value)
                {
                    m_Items[i].item = value;
                });
                pBox->addItem(textBox);
                break;
            }
            case EditTypes::Bool:
            {
                spCheckbox checkBox = spCheckbox::create();
                checkBox->setTooltipText(m_Items[i].tooltip);
                checkBox->setPosition(width, y);
                QVariant var = m_Items[i].item;
                checkBox->setChecked(var.toBool());
                connect(checkBox.get(), &Checkbox::checkChanged,
                        [=](bool value)
                {
                    QVariant var;
                    var.setValue(value);
                    m_Items[i].item = var.toString();
                });
                pBox->addItem(checkBox);
                break;
            }
            case EditTypes::Selection:
            {
                spDropDownmenu dropDown = spDropDownmenu::create(300, m_Items[i].items);
                dropDown->setTooltipText(m_Items[i].tooltip);
                dropDown->setPosition(width, y);
                for (qint32 i2  = 0; i2 < m_Items[i].data.size(); i2++)
                {
                    if (m_Items[i].data[i2] == m_Items[i].item)
                    {
                        dropDown->setCurrentItem(i2);
                    }
                }
                connect(dropDown.get(), &DropDownmenu::sigItemChanged, [=](qint32 item)
                {
                    m_Items[i].item = m_Items[i].data[item];
                });
                pBox->addItem(dropDown);
                break;
            }
            case EditTypes::File:
            {
                spTextbox pTextbox = spTextbox::create(Settings::getWidth() - 220 - width);
                pTextbox->setTooltipText(m_Items[i].tooltip);
                pTextbox->setCurrentText(m_Items[i].item);
                pTextbox->setPosition(width, y);
                connect(pTextbox.get(), &Textbox::sigTextChanged, [=](QString item)
                {
                    m_Items[i].item = item;
                });
                pBox->addItem(pTextbox);
                ObjectManager* pObjectManager = ObjectManager::getInstance();
                oxygine::spButton pButtonSelect = pObjectManager->createButton(tr("Select"), 150);
                pButtonSelect->setPosition(10 + pTextbox->getX() + pTextbox->getWidth(), y);
                pBox->addChild(pButtonSelect);
                pButtonSelect->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
                {
                     emit sigShowSelectFile (m_Items[i].filter, m_Items[i].startFolder, m_Items[i].item, pTextbox);
                });
                break;
            }
        }
        y += 40;
    }
    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}

void ScriptEventGeneric::showSelectFile (QString filter, QString startFolder, QString currentFile, spTextbox pTextbox)
{
    
    QVector<QString> wildcards;
    wildcards.append(filter);
    QString path = QCoreApplication::applicationDirPath() + startFolder;
    spFileDialog fileDialog = spFileDialog::create(path, wildcards, currentFile);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, [=](QString id)
    {
        QString file = id.replace(QCoreApplication::applicationDirPath() + "/", "");
        pTextbox->setCurrentText(file);
        emit pTextbox->sigTextChanged(file);
    });
    oxygine::getStage()->addChild(fileDialog);
    
}
