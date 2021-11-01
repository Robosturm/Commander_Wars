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
        var animationCount = GameAnimationFactory.getAnimationCount();
        var queueAnimation = null;
        if (animationCount > 0)
        {
            queueAnimation = GameAnimationFactory.getAnimation(animationCount - 1);
        }
        var animation = GameAnimationFactory.createAnimation(0, 0);
        animation.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        animation.setSound("sun.wav");
        if (queueAnimation !== null)
        {
            queueAnimation.queueAnimation(animation);
        }
    };
}

Constructor.prototype = WEATHER;
var WEATHER_1SUN = new Constructor();

