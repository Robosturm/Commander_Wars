#include "ingamescriptsupport/events/scripteventchangecobar.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

ScriptEventChangeCOBar::ScriptEventChangeCOBar()
    : ScriptEvent (EventType::changeCOBar)
{

}

qint32 ScriptEventChangeCOBar::getCo() const
{
    return co;
}

void ScriptEventChangeCOBar::setCo(const qint32 &value)
{
    co = value;
}

float ScriptEventChangeCOBar::getStars() const
{
    return stars;
}

void ScriptEventChangeCOBar::setStars(float value)
{
    stars = value;
}


void ScriptEventChangeCOBar::readEvent(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    line = rStream.readLine().simplified();
    QStringList items = line.replace("map.getPlayer(", "")
                            .replace(").getCO(", ",")
                            .replace(").addPowerFilled(", ",")
                            .replace(");", "").split(",");
    if (items.size() >= 3)
    {
        player = items[0].toInt();
        co = items[1].toInt();
        stars = items[2].toFloat();
    }
    line = rStream.readLine().simplified();
}

void ScriptEventChangeCOBar::writeEvent(QTextStream& rStream)
{
    rStream <<  "            if (map.getPlayer(" << QString::number(player) << ").getCO(" << QString::number(co) << ") !== null){ // "
            << QString::number(getVersion()) << " " << EventChangeCOBar << "\n";
    rStream <<  "            map.getPlayer(" << QString::number(player) << ").getCO("  << QString::number(co) << ").addPowerFilled("
            << QString::number(stars) << ");\n";
    rStream <<  "            }\n";
}

qint32 ScriptEventChangeCOBar::getPlayer() const
{
    return player;
}

void ScriptEventChangeCOBar::setPlayer(const qint32 &value)
{
    player = value;
}

void ScriptEventChangeCOBar::showEditEvent(spScriptEditor pScriptEditor)
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
    spinBox->setTooltipText(tr("Player who earns CO-Power Stars."));
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
    pText->setHtmlText(tr("CO: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 1, 2);
    spinBox->setTooltipText(tr("CO who earns CO-Power Stars."));
    spinBox->setPosition(width, 70);
    spinBox->setCurrentValue(co + 1);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setCo(static_cast<qint32>(value) - 1);
    });
    pBox->addItem(spinBox);


    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Stars: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, -100, 100, SpinBox::Mode::Float);
    spinBox->setTooltipText(tr("The amount of Stars that will be changed."));
    spinBox->setPosition(width, 110);
    spinBox->setCurrentValue(stars);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setStars(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}
