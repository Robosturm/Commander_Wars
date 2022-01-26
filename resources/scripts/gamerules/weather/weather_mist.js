var Constructor = function()
{
    this.getWeatherName = function()
    {

        return qsTr("Mist");
    };

    this.getWeatherSymbol = function()
    {
        return "weather_symbol_mist";
    };

    this.activate = function(weather)
    {
        var animationCount = GameAnimationFactory.getAnimationCount();
        var queueAnimation = null;
        if (animationCount > 0)
        {
            queueAnimation = GameAnimationFactory.getAnimation(animationCount - 1);
        }
        var animation = GameAnimationFactory.createAnimation(map, 0, 0);
        animation.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        animation.setStartOfAnimationCall("ANIMATION", "preOnAnimationChangedAnimation");
        animation.setSound("rain.wav");
        var variable = weather.getVariables().createVariable("FOGMODE");
        var fogMode = map.getGameRules().getFogMode();
        variable.writeDataInt32(fogMode);
        // only apply fog of war if the fog rules are softer
        if (fogMode === GameEnums.Fog_Off)
        {
            map.getGameRules().setFogMode(GameEnums.Fog_OfMist);
        }
        if (queueAnimation !== null)
        {
            queueAnimation.queueAnimation(animation);
        }
    };

    this.deactivate = function(weather)
    {
        var variable = weather.getVariables().getVariable("FOGMODE");
        var mode = variable.readDataInt32();
        map.getGameRules().setFogMode(mode);
    };

    this.getDefaultWeatherChance = function()
    {
        return 0;
    };
}

Constructor.prototype = WEATHER;
var WEATHER_MIST= new Constructor();
