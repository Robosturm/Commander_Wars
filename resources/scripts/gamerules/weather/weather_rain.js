var Constructor = function()
{
    this.getWeatherName = function()
    {

        return qsTr("Rain");
    };

    this.getWeatherTerrainSprite = function()
    {

        return "weather_rain";
    };

    this.getWeatherSymbol = function()
    {
        return "weather_symbol_rain";
    };

    this.getVisionrangeModifier = function()
    {

        return -1;
    };

    this.activate = function(weather)
    {
        var variable = weather.getVariables().createVariable("FOGMODE");
        variable.writeDataInt32(map.getGameRules().getFogMode());
        map.getGameRules().setFogMode(GameEnums.Fog_OfWar);
    };

    this.deactivate = function(weather)
    {
        var variable = weather.getVariables().getVariable("FOGMODE");
        var mode = variable.readDataInt32();
        map.getGameRules().setFogMode(mode);
    };
}

Constructor.prototype = WEATHER;
var WEATHER_RAIN = new Constructor();
