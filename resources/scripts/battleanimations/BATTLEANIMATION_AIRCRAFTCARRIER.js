var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("aircraftcarrier",  false,
                          BATTLEANIMATION_AIRCRAFTCARRIER.getMaxUnitCount(), Qt.point(0, 20));
        sprite.loadSprite("aircraftcarrier+mask",  true,
                          BATTLEANIMATION_AIRCRAFTCARRIER.getMaxUnitCount(), Qt.point(0, 20));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_AIRCRAFTCARRIER = new Constructor();
