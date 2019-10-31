#include "scripteventchangeweather.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "objects/spinbox.h"

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
    spGenericBox pBox = new GenericBox();

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;

    oxygine::spTextField pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Days: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spSpinBox spinBox = new SpinBox(150, 1, 9999);
    spinBox->setPosition(width, 30);
    spinBox->setCurrentValue(turns);
    connect(spinBox.get(), &SpinBox::sigValueChanged,
            [=](qreal value)
    {
        setTurns(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);

    pText = new oxygine::TextField();
    pText->setStyle(style);
    pText->setHtmlText(tr("Weather: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);

    QVector<QString> weatherStrings;
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getGameRules()->getWeatherCount(); i++)
    {
        Weather* pWeather = pMap->getGameRules()->getWeather(i);
        weatherStrings.append(pWeather->getWeatherName());
    }

    spDropDownmenu startWeather = new DropDownmenu(200, weatherStrings);
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
