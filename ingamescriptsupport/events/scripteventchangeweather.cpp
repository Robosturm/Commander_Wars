#include "ingamescriptsupport/events/scripteventchangeweather.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/base/spinbox.h"
#include "objects/base/label.h"

#include "game/gamemap.h"
#include "game/weather.h"

ScriptEventChangeWeather::ScriptEventChangeWeather()
    : ScriptEvent (EventType::changeWeather)
{

}

qint32 ScriptEventChangeWeather::getTurns() const
{
    return turns;
}

void ScriptEventChangeWeather::setTurns(const qint32 &value)
{
    turns = value;
}

qint32 ScriptEventChangeWeather::getWeatherID() const
{
    return weatherID;
}

void ScriptEventChangeWeather::setWeatherID(const qint32 &value)
{
    weatherID = value;
}

void ScriptEventChangeWeather::readEvent(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    QStringList items = line.replace("map.getGameRules().changeWeather(", "")
                            .replace(", map.getPlayerCount() * ", ",")
                            .replace("); // ", ",").split(",");
    if (items.size() >= 2)
    {
        weatherID = items[0].toInt();
        turns = items[1].toInt();
    }
}

void ScriptEventChangeWeather::writeEvent(QTextStream& rStream)
{
    rStream <<  "            map.getGameRules().changeWeather(" << QString::number(weatherID) << ", map.getPlayerCount() * " + QString::number(turns) + "); // "
            << QString::number(getVersion()) << " " << EventChangeWeather << "\n";
}

void ScriptEventChangeWeather::showEditEvent(spScriptEditor pScriptEditor)
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
    pText->setHtmlText(tr("Days: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(300, 1, 9999);
    spinBox->setTooltipText(tr("The amount of days the new weather appears."));
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(turns);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setTurns(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Weather: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);

    QVector<QString> weatherStrings;
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getGameRules()->getWeatherCount(); i++)
    {
        Weather* pWeather = pMap->getGameRules()->getWeather(i);
        weatherStrings.append(pWeather->getWeatherName());
    }

    spDropDownmenu startWeather = spDropDownmenu::create(200, weatherStrings);
    startWeather->setTooltipText(tr("The new weather that appears."));
    startWeather->setPosition(150, 70);
    startWeather->setCurrentItem(weatherID);
    connect(startWeather.get(), &DropDownmenu::sigItemChanged, [=](qint32 item)
    {
        weatherID = item;
    });
    pBox->addItem(startWeather);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}
