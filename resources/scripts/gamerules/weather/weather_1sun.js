var Constructor = function()
{
    this.getWeatherName = function()
    {

        return qsTr("Sun");
    };
}

Constructor.prototype = WEATHER;
var WEATHER_1SUN = new Constructor();

