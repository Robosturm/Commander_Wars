var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("iron_shield_generator",  false,
                          BATTLEANIMATION_APC.getMaxUnitCount(), Qt.point(10, 10));
        sprite.loadSprite("iron_shield_generator+mask",  true,
                          BATTLEANIMATION_APC.getMaxUnitCount(), Qt.point(10, 10));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_IRON_SHIELD_GENERATOR = new Constructor();
