#include "ingamescriptsupport/events/scripteventvictoryinfo.h"

#include "ingamescriptsupport/scriptdata.h"
#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/base/textbox.h"
#include "objects/base/label.h"

ScriptEventVictoryInfo::ScriptEventVictoryInfo()
    : ScriptEvent (EventType::victoryInfo)
{

}


void ScriptEventVictoryInfo::readEvent(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    QStringList items = line.replace(QString(ScriptData::variables) + ".createVariable(\"victory_info\").writeDataString(\"", "")
                            .replace("\"); // ", ",").split(",");
    if (items.size() >= 1)
    {
        info = items[0];
    }
}

void ScriptEventVictoryInfo::writeEvent(QTextStream& rStream)
{
    rStream <<  "            " << ScriptData::variables << ".createVariable(\"victory_info\").writeDataString(\"" << info << "\"); // "
            << QString::number(getVersion()) << " " << EventVictoryInfo << "\n";
}

QString ScriptEventVictoryInfo::getInfo() const
{
    return info;
}

void ScriptEventVictoryInfo::setInfo(const QString &value)
{
    info = value;
}

void ScriptEventVictoryInfo::showEditEvent(spScriptEditor pScriptEditor)
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
    pText->setHtmlText(tr("Victory Info: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);

    spTextbox pTextBox = spTextbox::create(Settings::getWidth() - width - 40, Settings::getHeight() - 100);
    pTextBox->setCurrentText(info);
    pTextBox->setPosition(width, 30);
    pTextBox->setTooltipText("Additional info text shown in the victory info screen. Replaces the last set text with this one.");
    connect(pTextBox.get(), &Textbox::sigTextChanged, [=](QString value)
    {
        setInfo(value);
    });
    pBox->addItem(pTextBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}
