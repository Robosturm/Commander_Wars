var Constructor = function()
{
    this.getWeatherName = function()
    {

        return qsTr("Sandstorm");
    };

    this.getDescription = function()
    {
        return qsTr("Sandy weather which reduces the offence and indirect fire range of units.");
    };

    this.getWeatherTerrainSprite = function()
    {

        return "weather_sandstorm";
    };

    this.getWeatherSymbol = function()
    {
        return "weather_symbol_sandstorm_alt";
    };

    this.getOffensiveModifier = function()
    {

        return -15;
    };

    this.getFirerangeModifier = function()
    {

        return -1;
    };

    this.activate = function(weather, map)
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
        animation.setSound("sandstorm.wav");
        animation.setStartOfAnimationCall("ANIMATION", "preOnAnimationChangedAnimation");
        if (queueAnimation !== null)
        {
            queueAnimation.queueAnimation(animation);
        }
    };

    this.getDefaultWeatherChance = function()
    {
        return 10;
    };
}

Constructor.prototype = WEATHER;
var WEATHER_SANDSTORM = new Constructor();
