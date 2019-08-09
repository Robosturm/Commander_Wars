var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("destroyer",  false,
                          BATTLEANIMATION_DESTROYER.getMaxUnitCount(), Qt.point(0, 20));
        sprite.loadSprite("destroyer+mask",  true,
                          BATTLEANIMATION_DESTROYER.getMaxUnitCount(), Qt.point(0, 20));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_DESTROYER = new Constructor();
