var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit)
    {
        sprite.loadSprite("aircraftcarrier",  false,
                          BATTLEANIMATION_AIRCRAFTCARRIER.getMaxUnitCount(), Qt.point(0, 0));
        sprite.loadSprite("aircraftcarrier+mask",  true,
                          BATTLEANIMATION_AIRCRAFTCARRIER.getMaxUnitCount(), Qt.point(0, 0));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_AIRCRAFTCARRIER = new Constructor();
