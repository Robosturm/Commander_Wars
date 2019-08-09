var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("flare",  false,
                          BATTLEANIMATION_FLARE.getMaxUnitCount(), Qt.point(-45, 5));
        sprite.loadSprite("flare+mask",  true,
                          BATTLEANIMATION_FLARE.getMaxUnitCount(), Qt.point(-45, 5));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_FLARE = new Constructor();
