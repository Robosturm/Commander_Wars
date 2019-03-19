var Constructor = function()
{
    this.getWeatherName = function()
    {

        return qsTr("Sandstorm");
    };

    this.getWeatherTerrainSprite = function()
    {

        return "weather_sandstorm";
    };

    this.getOffensiveModifier = function()
    {

        return -15;
    };

    this.getFirerangeModifier = function()
    {

        return -1;
    };
}

Constructor.prototype = WEATHER;
var WEATHER_SANDSTORM = new Constructor();
