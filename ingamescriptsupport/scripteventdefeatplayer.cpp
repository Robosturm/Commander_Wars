#include "scripteventdefeatplayer.h"

#include "scripteditor.h"
#include "genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/spinbox.h"

ScriptEventDefeatPlayer::ScriptEventDefeatPlayer()
    : ScriptEvent (ScriptEvent::EventType::defeatPlayer)
{

}

void ScriptEventDefeatPlayer::readEvent(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    QString item = line.replace("map.getPlayer(", "")
                    .replace(").setIsDefeated(true); // " + EventDefeatPlayer, "");
    player = item.toInt();
}

void ScriptEventDefeatPlayer::writeEvent(QTextStream& rStream)
{
    rStream <<  "            map.getPlayer(" << QString::number(player) << ").setIsDefeated(true); // "
            << EventDefeatPlayer << "\n";
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
    spGenericBox pBox = new GenericBox();

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;

    oxygine::spTextField pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setText(tr("Player: ").toStdString().c_str());
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

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}
