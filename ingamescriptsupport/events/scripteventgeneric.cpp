#include "ingamescriptsupport/events/scripteventgeneric.h"

#include "ingamescriptsupport/scripteditor.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "coreengine/globalutils.h"

#include "objects/base/spinbox.h"
#include "objects/base/checkbox.h"
#include "objects/base/dropdownmenu.h"
#include "objects/base/dropdownmenusprite.h"
#include "objects/base/textbox.h"
#include "objects/base/label.h"
#include "objects/dialogs/filedialog.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

static constexpr qint32 width = 300;

ScriptEventGeneric::ScriptEventGeneric(GameMap* pMap, EventType type, QString eventIdentifier, const QString & description)
    : ScriptEvent(pMap, type),
      m_eventIdentifier(eventIdentifier),
      m_description(description)
{
    connect(this, &ScriptEventGeneric::sigShowSelectFile, this, &ScriptEventGeneric::showSelectFile, Qt::QueuedConnection);
    connect(this, &ScriptEventGeneric::sigLoad, this, &ScriptEventGeneric::load, Qt::QueuedConnection);
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
        if (m_Items[i].m_writer)
        {
            rStream << m_Items[i].m_writer(m_Items[i]);
        }
        else
        {
            rStream << m_Items[i].item;
        }
        rStream << m_Items[i].afterCode;
    }
    rStream <<  " // " << QString::number(getVersion()) << " " << m_eventIdentifier << "\n";
}

void ScriptEventGeneric::showEditEvent(spScriptEditor pScriptEditor)
{
    spGenericBox pBox = MemoryManagement::create<GenericBox>();
    load(pBox);
    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}

void ScriptEventGeneric::load(spGenericBox pBox)
{
    pBox->clearAllItems();

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 y = 30;
    for (qint32 i = 0; i < m_Items.size(); i++)
    {
        spLabel pText = MemoryManagement::create<Label>(width - 10);
        pText->setStyle(style);
        pText->setHtmlText(m_Items[i].labelText);
        pText->setPosition(30, y);
        pBox->addItem(pText);
        switch (m_Items[i].types)
        {
        case EditTypes::Float:
        {
            addFloatEditor(pBox, i, y);
            break;
        }
        case EditTypes::Integer:
        {
            addIntegerEditor(pBox, i, y);
            break;
        }
        case EditTypes::String:
        {
            addStringEditor(pBox, i, y);
            break;
        }
        case EditTypes::Bool:
        {
            addBoolEditor(pBox, i, y);
            break;
        }
        case EditTypes::Selection:
        {
            addSelectionEditor(pBox, i, y);
            break;
        }
        case EditTypes::File:
        {
            addFileEditor(pBox, i, y);
            break;
        }
        case EditTypes::IconSelection:
        {
            addIconEditor(pBox, i, y);
            break;
        }
        case EditTypes::PointList:
        {
            y += pText->getHeight() + 10;
            addPointListEditor(pBox, i, y);
            break;
        }
        }
        y += pText->getHeight() + 10;
    }
    pBox->setContentSize(800, y + 100);
}

void ScriptEventGeneric::addFloatEditor(spGenericBox & pBox, qint32 i, qint32 & y)
{
    spSpinBox spinBox = MemoryManagement::create<SpinBox>(300, m_Items[i].min, m_Items[i].max, SpinBox::Mode::Float);
    spinBox->setInfinityValue(m_Items[i].infinite);
    spinBox->setTooltipText(m_Items[i].tooltip);
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(m_Items[i].item.toFloat());
    connect(spinBox.get(), &SpinBox::sigValueChanged, this,
    [this, i](qreal value)
    {
        m_Items[i].item = QString::number(value);
    });
    pBox->addItem(spinBox);
}

void ScriptEventGeneric::addIntegerEditor(spGenericBox & pBox, qint32 i, qint32 & y)
{
    spSpinBox spinBox = MemoryManagement::create<SpinBox>(300, m_Items[i].min, m_Items[i].max);
    spinBox->setInfinityValue(m_Items[i].infinite);
    spinBox->setTooltipText(m_Items[i].tooltip);
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(m_Items[i].item.toInt());
    connect(spinBox.get(), &SpinBox::sigValueChanged, this,
            [this, i](qreal value)
            {
                m_Items[i].item = QString::number(value);
            });
    pBox->addItem(spinBox);
}

void ScriptEventGeneric::addStringEditor(spGenericBox & pBox, qint32 i, qint32 & y)
{
    spTextbox textBox = MemoryManagement::create<Textbox>(300);
    textBox->setTooltipText(m_Items[i].tooltip);
    textBox->setPosition(width, y);
    textBox->setCurrentText(m_Items[i].item);
    connect(textBox.get(), &Textbox::sigTextChanged, this,
            [this, i](QString value)
            {
                m_Items[i].item = value;
            });
    pBox->addItem(textBox);
}

void ScriptEventGeneric::addBoolEditor(spGenericBox & pBox, qint32 i, qint32 & y)
{
    spCheckbox checkBox = MemoryManagement::create<Checkbox>();
    checkBox->setTooltipText(m_Items[i].tooltip);
    checkBox->setPosition(width, y);
    QVariant var = m_Items[i].item;
    checkBox->setChecked(var.toBool());
    connect(checkBox.get(), &Checkbox::checkChanged, this,
    [this, i](bool value)
    {
        QVariant var;
        var.setValue(value);
        m_Items[i].item = var.toString();
    });
    pBox->addItem(checkBox);
}

void ScriptEventGeneric::addSelectionEditor(spGenericBox & pBox, qint32 i, qint32 & y)
{
    spDropDownmenu dropDown = MemoryManagement::create<DropDownmenu>(300, m_Items[i].items);
    dropDown->setTooltipText(m_Items[i].tooltip);
    dropDown->setPosition(width, y);
    for (qint32 i2  = 0; i2 < m_Items[i].data.size(); i2++)
    {
        if (m_Items[i].data[i2] == m_Items[i].item)
        {
            dropDown->setCurrentItem(i2);
        }
    }
    connect(dropDown.get(), &DropDownmenu::sigItemChanged, this, [this, i](qint32 item)
            {
                m_Items[i].item = m_Items[i].data[item];
            });
    pBox->addItem(dropDown);
}

void ScriptEventGeneric::addFileEditor(spGenericBox & pBox, qint32 i, qint32 & y)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    spTextbox pTextbox = MemoryManagement::create<Textbox>(oxygine::Stage::getStage()->getWidth() - 220 - width);
    pTextbox->setTooltipText(m_Items[i].tooltip);
    pTextbox->setCurrentText(m_Items[i].item);
    pTextbox->setPosition(width, y);
    connect(pTextbox.get(), &Textbox::sigTextChanged, this, [this, i](QString item)
    {
        m_Items[i].item = item;
    });
    pBox->addItem(pTextbox);
    oxygine::spButton pButtonSelect = pObjectManager->createButton(tr("Select"), 150);
    pButtonSelect->setPosition(10 + pTextbox->getX() + pTextbox->getScaledWidth(), y);
    pBox->addChild(pButtonSelect);
    pButtonSelect->addEventListener(oxygine::TouchEvent::CLICK, [this, i, pTextbox](oxygine::Event*)
    {
        emit sigShowSelectFile (m_Items[i].filter, m_Items[i].startFolder, m_Items[i].item, pTextbox);
    });
}

void ScriptEventGeneric::addIconEditor(spGenericBox & pBox, qint32 i, qint32 & y)
{
    spDropDownmenuSprite dropDown = MemoryManagement::create<DropDownmenuSprite>(105, m_Items[i].items, m_Items[i].creator, -1, false);
    dropDown->setTooltipText(m_Items[i].tooltip);
    dropDown->setPosition(width, y);
    for (qint32 i2  = 0; i2 < m_Items[i].data.size(); i2++)
    {
        if (m_Items[i].data[i2] == m_Items[i].item)
        {
            dropDown->setCurrentItem(i2);
        }
    }
    connect(dropDown.get(), &DropDownmenuSprite::sigItemChanged, this, [this, i](qint32 item)
    {
        m_Items[i].item = m_Items[i].data[item];
    });
    pBox->addItem(dropDown);
}

void ScriptEventGeneric::addPointListEditor(spGenericBox pBox, qint32 i, qint32 & y)
{
    if (!m_Items[i].item.isEmpty())
    {
        m_Items[i].item = m_Items[i].item.replace("Qt.point(", "");
        m_Items[i].item = m_Items[i].item.replace(")", "");
        m_Items[i].item = m_Items[i].item.replace("[", "");
        m_Items[i].item = m_Items[i].item.replace("]", "");
        auto items = m_Items[i].item.split(",");
        for (qint32 i2 = 0; i2 < items.size(); i2 += 2)
        {
            m_Items[i].pointData.append(QPoint(items[i2].trimmed().toInt(), items[i2].trimmed().toInt()));
        }
        m_Items[i].item = "";
    }
    if (!m_Items[i].m_writer)
    {
        m_Items[i].m_writer = [](const Data & data)
        {
            QString ret = "[";
            for (qint32 i2 = 0; i2 < data.pointData.size(); ++i2)
            {
                QPoint point = data.pointData[i2].toPoint();
                if (i2 == data.pointData.size() - 1)
                {
                    ret += "Qt.point(" + QString::number(point.x()) + "," + QString::number(point.y()) +")";
                }
                else
                {
                    ret += "Qt.point(" + QString::number(point.x()) + "," + QString::number(point.y()) +"),";
                }
            }
            ret += "]";
            return ret;
        };
    }
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    for (qint32 i2 = 0; i2 < m_Items[i].pointData.size(); ++i2)
    {
        spSpinBox spinBox = MemoryManagement::create<SpinBox>(300, m_Items[i].min, m_Items[i].max);
        spinBox->setInfinityValue(m_Items[i].infinite);
        spinBox->setTooltipText(m_Items[i].tooltip);
        spinBox->setPosition(10, y);
        spinBox->setCurrentValue(m_Items[i].pointData[i2].toPoint().x());
        connect(spinBox.get(), &SpinBox::sigValueChanged, this, [this, i, i2](qreal value)
        {
            QPoint point = m_Items[i].pointData[i2].toPoint();
            point.setX(value);
            m_Items[i].pointData[i2] = QVariant(point);
        });
        pBox->addItem(spinBox);
        spinBox = MemoryManagement::create<SpinBox>(300, m_Items[i].min, m_Items[i].max);
        spinBox->setInfinityValue(m_Items[i].infinite);
        spinBox->setTooltipText(m_Items[i].tooltip);
        spinBox->setPosition(20 + spinBox->getWidth(), y);
        spinBox->setCurrentValue(m_Items[i].pointData[i2].toPoint().y());
        connect(spinBox.get(), &SpinBox::sigValueChanged, this, [this, i, i2](qreal value)
        {
            QPoint point = m_Items[i].pointData[i2].toPoint();
            point.setY(value);
            m_Items[i].pointData[i2] = QVariant(point);
        });
        pBox->addItem(spinBox);

        y += spinBox->getScaledHeight() + 10;
    }
    y += 10;
    oxygine::spButton pButtonSelect = pObjectManager->createButton(tr("Add target"), 250);
    pButtonSelect->setPosition(10, y );
    pBox->addItem(pButtonSelect);
    pButtonSelect->addEventListener(oxygine::TouchEvent::CLICK, [this, i, pBox](oxygine::Event*)
    {
        m_Items[i].pointData.append(QVariant(QPoint()));
        emit sigLoad(pBox);
    });
}

void ScriptEventGeneric::showSelectFile(QString filter, QString startFolder, QString currentFile, spTextbox pTextbox)
{    
    QStringList wildcards;
    wildcards.append(filter);
    QString path = Settings::userPath() + startFolder;
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(path, wildcards, false, currentFile, false, tr("Select"));
    Textbox* pBox = pTextbox.get();
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, [=](QString id)
    {
        QString file = GlobalUtils::makePathRelative(id);
        pBox->setCurrentText(file);
        emit pBox->sigTextChanged(file);
    });
    oxygine::Stage::getStage()->addChild(fileDialog);
    
}
