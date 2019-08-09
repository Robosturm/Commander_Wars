var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("hoellium",  false,
                          BATTLEANIMATION_HOELLIUM.getMaxUnitCount(), Qt.point(10, 10));
        sprite.loadSprite("hoellium+mask",  true,
                          BATTLEANIMATION_HOELLIUM.getMaxUnitCount(), Qt.point(10, 10));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HOELLIUM = new Constructor();
