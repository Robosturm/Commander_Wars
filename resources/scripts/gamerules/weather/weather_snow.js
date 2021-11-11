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
        var animationCount = GameAnimationFactory.getAnimationCount();
        var queueAnimation = null;
        if (animationCount > 0)
        {
            queueAnimation = GameAnimationFactory.getAnimation(animationCount - 1);
        }
        var animation = GameAnimationFactory.createAnimation(0, 0);
        animation.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        animation.setSound("snow.wav");
        animation.setStartOfAnimationCall("ANIMATION", "preOnAnimationChangedAnimation");
        if (queueAnimation !== null)
        {
            queueAnimation.queueAnimation(animation);
        }
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

    this.getDefaultWeatherChance = function()
    {
        return 10;
    };
}

Constructor.prototype = WEATHER;
var WEATHER_SNOW = new Constructor();
