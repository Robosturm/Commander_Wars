var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("stealthbomber",  false,
                          BATTLEANIMATION_STEALTHBOMBER.getMaxUnitCount(), Qt.point(0, 40));
        sprite.loadSprite("stealthbomber+mask",  true,
                          BATTLEANIMATION_STEALTHBOMBER.getMaxUnitCount(), Qt.point(0, 40));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_STEALTHBOMBER = new Constructor();
