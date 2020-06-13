var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("transportplane",  false,
                          BATTLEANIMATION_TRANSPORTPLANE.getMaxUnitCount(), Qt.point(0, 40));
        sprite.loadSpriteV2("transportplane+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_TRANSPORTPLANE.getMaxUnitCount(), Qt.point(0, 40));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_TRANSPORTPLANE = new Constructor();
