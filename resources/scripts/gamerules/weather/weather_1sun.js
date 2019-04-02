var Constructor = function()
{
    this.getWeatherName = function()
    {

        return qsTr("Sun");
    };

    this.getWeatherSymbol = function()
    {
        return "weather_symbol_sun";
    };
}

Constructor.prototype = WEATHER;
var WEATHER_1SUN = new Constructor();

