var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("black_bomb",  false,
                          BATTLEANIMATION_BLACK_BOMB.getMaxUnitCount(), Qt.point(0, 40));
        sprite.loadSprite("black_bomb+mask",  true,
                          BATTLEANIMATION_BLACK_BOMB.getMaxUnitCount(), Qt.point(0, 40));
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BLACK_BOMB = new Constructor();
