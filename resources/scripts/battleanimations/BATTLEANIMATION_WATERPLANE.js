var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit)
    {
        sprite.loadSprite("waterplane",  false,
                          BATTLEANIMATION_WATERPLANE.getMaxUnitCount(), Qt.point(-50, 20));
        sprite.loadSprite("waterplane+mask",  true,
                          BATTLEANIMATION_WATERPLANE.getMaxUnitCount(), Qt.point(-50, 20));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_WATERPLANE = new Constructor();
