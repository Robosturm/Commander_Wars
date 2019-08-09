var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("antitankcannon",  false,
                          BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount(), Qt.point(-5, 5));
        sprite.loadSprite("antitankcannon+mask",  true,
                          BATTLEANIMATION_ANTITANKCANNON.getMaxUnitCount(), Qt.point(-5, 5));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ANTITANKCANNON = new Constructor();
