var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };
    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("logi_truck",  false,
                          BATTLEANIMATION_ZCOUNIT_LOGIC_TRUCK.getMaxUnitCount(), Qt.point(0, 10));
        sprite.loadSpriteV2("logi_truck+mask",  GameEnums.Recoloring_Table,
                          BATTLEANIMATION_ZCOUNIT_LOGIC_TRUCK.getMaxUnitCount(), Qt.point(0, 10));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_LOGIC_TRUCK = new Constructor();
