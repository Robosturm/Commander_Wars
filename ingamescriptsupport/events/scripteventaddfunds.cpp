#include "ingamescriptsupport/events/scripteventaddfunds.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

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


void ScriptEventAddFunds::readEvent(QTextStream& rStream, QString line)
{
    line = line.simplified();
    QStringList items = line.replace("map.getPlayer(", "")
                            .replace(").addFunds(", ",")
                            .replace("); // ", ",").split(",");
    if (items.size() >= 2)
    {
        player = items[0].toInt();
        funds = items[1].toInt();
    }
}

void ScriptEventAddFunds::writeEvent(QTextStream& rStream)
{
    rStream <<  "            map.getPlayer(" << QString::number(player) << ").addFunds(" + QString::number(funds) + "); // "
            << QString::number(getVersion()) << " " << EventAddFunds << "\n";
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
    spGenericBox pBox = spGenericBox::create();

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;

    spLabel pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Player: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(300, 1, 9999);
    spinBox->setTooltipText(tr("Player that earns the given amount of funds."));
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(player + 1);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setPlayer(static_cast<qint32>(value) - 1);
    });
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Funds: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 999999);
    spinBox->setTooltipText(tr("The funds the given player will earn."));
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
