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
        var fogMode = map.getGameRules().getFogMode();
        variable.writeDataInt32(fogMode);
        // only apply fog of war if the fog rules are softer
        if (fogMode < GameEnums.Fog_OfWar)
        {
            map.getGameRules().setFogMode(GameEnums.Fog_OfWar);
        }
        audio.playSound("rain.wav");
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
