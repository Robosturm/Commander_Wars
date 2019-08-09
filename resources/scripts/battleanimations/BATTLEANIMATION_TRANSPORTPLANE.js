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
        sprite.loadSprite("transportplane+mask",  true,
                          BATTLEANIMATION_TRANSPORTPLANE.getMaxUnitCount(), Qt.point(0, 40));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_TRANSPORTPLANE = new Constructor();
