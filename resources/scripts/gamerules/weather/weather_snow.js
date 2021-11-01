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

    this.getWeatherSymbol = function()
    {
        return "weather_symbol_snow";
    };

    this.activate = function(weather)
    {
        var animation = GameAnimationFactory.createAnimation(0, 0);
        animation.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        animation.setSound("power_colin.wav");
        audio.playSound("snow.wav");
    };

    this.getMovementCostModifier = function(weather, unit, terrain)
    {
        var id = terrain.getID();
        if ((unit.getUnitType() === GameEnums.UnitType_Air) ||
            (id !== "STREET" &&
             id !== "BRIDGE" &&
            id !== "DESERT_PATH" &&
            id !== "TELEPORTTILE" &&
            terrain.getBuilding() === null))
        {
            return 1;
        }
        return 0;
    };
}

Constructor.prototype = WEATHER;
var WEATHER_SNOW = new Constructor();
