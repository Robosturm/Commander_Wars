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
        var animation = GameAnimationFactory.createAnimation(0, 0);
        animation.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        animation.setSound("power_colin.wav");
        audio.playSound("sun.wav");
    };
}

Constructor.prototype = WEATHER;
var WEATHER_1SUN = new Constructor();

