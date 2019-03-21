var Constructor = function()
{
    this.getWeatherName = function()
    {

        return qsTr("Snow");
    };

    this.getWeatherTerrainSprite = function()
    {
        return "weather_snow";
    };

    this.getMovementCostModifier = function()
    {

        return 1;
    };
}

Constructor.prototype = WEATHER;
var WEATHER_SNOW = new Constructor();
