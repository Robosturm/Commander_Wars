var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };
    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSpriteV2("intel_truck+mask",  GameEnums.Recoloring_Matrix,
                          BATTLEANIMATION_APC.getMaxUnitCount(), Qt.point(10, 10));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_INTEL_TRUCK = new Constructor();
