var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("cannonboat",  false,
                          BATTLEANIMATION_CANNONBOAT.getMaxUnitCount(), Qt.point(0, 20));
        sprite.loadSprite("cannonboat+mask",  true,
                          BATTLEANIMATION_CANNONBOAT.getMaxUnitCount(), Qt.point(0, 20));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_CANNONBOAT = new Constructor();
