#include "ingamescriptsupport/events/scripteventmodifyunit.h"

#include "resource_management/fontmanager.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

ScriptEventModifyUnit::ScriptEventModifyUnit()
    : ScriptEvent (ScriptEvent::EventType::modifyUnit)
{

}

void ScriptEventModifyUnit::writeEvent(QTextStream& rStream)
{
    rStream <<  "            if(map.getTerrain("
            << QString::number(m_x) << ", "
            << QString::number(m_y) << ").getUnit() !== null){map.getTerrain("
            << QString::number(m_x) << ", "
            << QString::number(m_y) << ").getUnit().modifyUnit("
            << QString::number(m_hpChange) << ", "
            << QString::number(m_ammo1Change) << ", "
            << QString::number(m_ammo2Change) << ", "
            << QString::number(m_fuelChange) << ");} // "
            << QString::number(getVersion()) << " " << EventModifyUnit << "\n";
}

void ScriptEventModifyUnit::readEvent(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    QStringList items = line.replace("if(map.getTerrain(", "")
                            .replace(").getUnit() !== null){map.getTerrain(", ",")
                            .replace(").getUnit().modifyUnit(", ",")
                            .replace(", ", ",")
                            .replace(");} // " , ",").split(",");
    if (items.size() >= 8)
    {
        m_x = items[0].toInt();
        m_y = items[1].toInt();
        m_hpChange = items[3].toInt();
        m_ammo1Change = items[4].toInt();
        m_ammo2Change = items[5].toInt();
        m_fuelChange = items[6].toInt();
    }
}

void ScriptEventModifyUnit::showEditEvent(spScriptEditor pScriptEditor)
{
    spGenericBox pBox = spGenericBox::create();

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;
    spLabel pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("X: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(300, 0, 9999);
    spinBox->setTooltipText(tr("X Location at which the unit gets modified."));
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(m_x);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptEventModifyUnit::setX, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Y: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 9999);
    spinBox->setTooltipText(tr("Y Location at which the unit gets modified."));
    spinBox->setPosition(width, 70);
    spinBox->setCurrentValue(m_y);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptEventModifyUnit::setY, Qt::QueuedConnection);
    pBox->addItem(spinBox);


    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Modify HP: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, -10, 10);
    spinBox->setInfinityValue(-200000);
    spinBox->setTooltipText(tr("HP change for the unit."));
    spinBox->setPosition(width, 110);
    spinBox->setCurrentValue(m_hpChange);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptEventModifyUnit::setHpChange, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Modify Ammo 1: "));
    pText->setPosition(30, 150);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, -100, 100);
    spinBox->setInfinityValue(-200000);
    spinBox->setTooltipText(tr("Ammo 1 change for the unit."));
    spinBox->setPosition(width, 150);
    spinBox->setCurrentValue(m_ammo1Change);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptEventModifyUnit::setAmmo1Change, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Modify Ammo 2: "));
    pText->setPosition(30, 190);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, -100, 100);
    spinBox->setInfinityValue(-200000);
    spinBox->setTooltipText(tr("Ammo 2 change for the unit."));
    spinBox->setPosition(width, 190);
    spinBox->setCurrentValue(m_ammo2Change);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptEventModifyUnit::setAmmo2Change, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Modify Fuel: "));
    pText->setPosition(30, 230);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, -100, 100);
    spinBox->setInfinityValue(-200000);
    spinBox->setTooltipText(tr("Fuel change for the unit."));
    spinBox->setPosition(width, 230);
    spinBox->setCurrentValue(m_fuelChange);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this, &ScriptEventModifyUnit::setFuelChange, Qt::QueuedConnection);
    pBox->addItem(spinBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}

qint32 ScriptEventModifyUnit::getX() const
{
    return m_x;
}

void ScriptEventModifyUnit::setX(const qint32 &x)
{
    m_x = x;
}

qint32 ScriptEventModifyUnit::getY() const
{
    return m_y;
}

void ScriptEventModifyUnit::setY(const qint32 &y)
{
    m_y = y;
}

qint32 ScriptEventModifyUnit::getHpChange() const
{
    return m_hpChange;
}

void ScriptEventModifyUnit::setHpChange(const qint32 &hpChange)
{
    m_hpChange = hpChange;
}

qint32 ScriptEventModifyUnit::getAmmo1Change() const
{
    return m_ammo1Change;
}

void ScriptEventModifyUnit::setAmmo1Change(const qint32 &ammo1Change)
{
    m_ammo1Change = ammo1Change;
}

qint32 ScriptEventModifyUnit::getAmmo2Change() const
{
    return m_ammo2Change;
}

void ScriptEventModifyUnit::setAmmo2Change(const qint32 &ammo2Change)
{
    m_ammo2Change = ammo2Change;
}

qint32 ScriptEventModifyUnit::getFuelChange() const
{
    return m_fuelChange;
}

void ScriptEventModifyUnit::setFuelChange(const qint32 &fuelChange)
{
    m_fuelChange = fuelChange;
}
