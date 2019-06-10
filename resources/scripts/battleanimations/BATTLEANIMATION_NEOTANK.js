var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit)
    {
        sprite.loadSprite("neotank",  false,
                          BATTLEANIMATION_NEOTANK.getMaxUnitCount(), Qt.point(-5, 5));
        sprite.loadSprite("neotank+mask",  true,
                          BATTLEANIMATION_NEOTANK.getMaxUnitCount(), Qt.point(-5, 5));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_NEOTANK = new Constructor();
