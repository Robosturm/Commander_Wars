var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("duster",  false,
                          BATTLEANIMATION_DUSTER.getMaxUnitCount(), Qt.point(-40, 20));
        sprite.loadSprite("duster+mask",  true,
                          BATTLEANIMATION_DUSTER.getMaxUnitCount(), Qt.point(-40, 20));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_DUSTER = new Constructor();
