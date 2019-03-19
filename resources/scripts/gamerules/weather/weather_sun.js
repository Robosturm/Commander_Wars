var Constructor = function()
{
    this.getWeatherName = function()
    {

        return qsTr("Sun");
    };
}

Constructor.prototype = WEATHER;
var WEATHER_SUN = new Constructor();

