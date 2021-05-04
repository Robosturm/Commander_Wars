#include "ingamescriptsupport/events/scripteventdefeatplayer.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

ScriptEventDefeatPlayer::ScriptEventDefeatPlayer()
    : ScriptEvent (ScriptEvent::EventType::defeatPlayer)
{

}

void ScriptEventDefeatPlayer::readEvent(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    QStringList item = line.replace("map.getPlayer(", "")
                    .replace(").defeatPlayer(null); // ", ",").split(",");
    player = item[0].toInt();
}

void ScriptEventDefeatPlayer::writeEvent(QTextStream& rStream)
{
    rStream <<  "            map.getPlayer(" << QString::number(player) << ").defeatPlayer(null); // "
            << QString::number(getVersion()) << " " << EventDefeatPlayer << "\n";
}

qint32 ScriptEventDefeatPlayer::getPlayer() const
{
    return player;
}

void ScriptEventDefeatPlayer::setPlayer(const qint32 &value)
{
    player = value;
}

void ScriptEventDefeatPlayer::showEditEvent(spScriptEditor pScriptEditor)
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
    spinBox->setTooltipText(tr("The player that gets defeated."));
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(player + 1);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setPlayer(static_cast<qint32>(value) - 1);
    });
    pBox->addItem(spinBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}
