#include "scripteventaddfunds.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/spinbox.h"

ScriptEventAddFunds::ScriptEventAddFunds()
    : ScriptEvent (EventType::addFunds)
{

}

qint32 ScriptEventAddFunds::getFunds() const
{
    return funds;
}

void ScriptEventAddFunds::setFunds(const qint32 &value)
{
    funds = value;
}


void ScriptEventAddFunds::readEvent(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    QStringList items = line.replace("map.getPlayer(", "")
                            .replace(").addFunds(", ",")
                            .replace("); // " + EventAddFunds, "").split(",");
    if (items.size() == 2)
    {
        player = items[0].toInt();
        funds = items[1].toInt();
    }
}

void ScriptEventAddFunds::writeEvent(QTextStream& rStream)
{
    rStream <<  "            map.getPlayer(" << QString::number(player) << ").addFunds(" + QString::number(funds) + "); // "
            << EventAddFunds << "\n";
}

qint32 ScriptEventAddFunds::getPlayer() const
{
    return player;
}

void ScriptEventAddFunds::setPlayer(const qint32 &value)
{
    player = value;
}

void ScriptEventAddFunds::showEditEvent(spScriptEditor pScriptEditor)
{
    spGenericBox pBox = new GenericBox();

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;

    oxygine::spTextField pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Player: ").toStdString().c_str());
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = new SpinBox(150, 1, 9999);
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(player + 1);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setPlayer(static_cast<qint32>(value) - 1);
    });
    pBox->addItem(spinBox);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Funds: ").toStdString().c_str());
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = new SpinBox(150, 0, 999999);
    spinBox->setPosition(width, 70);
    spinBox->setCurrentValue(funds);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setFunds(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}
