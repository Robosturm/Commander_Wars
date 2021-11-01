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

    this.getWeatherSymbol = function()
    {
        return "weather_symbol_sandstorm";
    };

    this.getOffensiveModifier = function()
    {

        return -15;
    };

    this.getFirerangeModifier = function()
    {

        return -1;
    };

    this.activate = function(weather)
    {
        var animation = GameAnimationFactory.createAnimation(0, 0);
        animation.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        animation.setSound("power_colin.wav");
        audio.playSound("sandstorm.wav");
    };
}

Constructor.prototype = WEATHER;
var WEATHER_SANDSTORM = new Constructor();
