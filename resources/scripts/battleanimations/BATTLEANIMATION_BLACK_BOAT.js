var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("black_boat",  false,
                          BATTLEANIMATION_BLACK_BOAT.getMaxUnitCount(), Qt.point(0, 20));
        sprite.loadSprite("black_boat+mask",  true,
                          BATTLEANIMATION_BLACK_BOAT.getMaxUnitCount(), Qt.point(0, 20));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BLACK_BOAT = new Constructor();
