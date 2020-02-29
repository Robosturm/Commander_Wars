var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("repair_tank",  false,
                          BATTLEANIMATION_APC.getMaxUnitCount(), Qt.point(10, 10));
        sprite.loadSprite("repair_tank+mask",  true,
                          BATTLEANIMATION_APC.getMaxUnitCount(), Qt.point(10, 10));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_REPAIR_TANK = new Constructor();
