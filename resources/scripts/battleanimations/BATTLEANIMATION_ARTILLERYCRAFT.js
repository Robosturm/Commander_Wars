var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit)
    {
        sprite.loadSprite("artillerycraft",  false,
                          BATTLEANIMATION_ARTILLERYCRAFT.getMaxUnitCount(), Qt.point(-55, 5));
        sprite.loadSprite("artillerycraft+mask",  true,
                          BATTLEANIMATION_ARTILLERYCRAFT.getMaxUnitCount(), Qt.point(-55, 5));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ARTILLERYCRAFT = new Constructor();
