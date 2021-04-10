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

    this.activate = function(weather)
    {
        audio.playSound("sun.wav");
    };
}

Constructor.prototype = WEATHER;
var WEATHER_1SUN = new Constructor();

