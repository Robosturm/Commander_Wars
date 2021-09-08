var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSpriteV2("repair_tank+mask", GameEnums.Recoloring_Matrix,
                          BATTLEANIMATION_APC.getMaxUnitCount(), Qt.point(10, 10));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_REPAIR_TANK = new Constructor();
