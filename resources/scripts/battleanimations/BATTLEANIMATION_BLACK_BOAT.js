var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadSprite("black_boat",  false,
                          BATTLEANIMATION_BLACK_BOAT.getMaxUnitCount(), Qt.point(0, 20), -1);
        sprite.loadSpriteV2("black_boat+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_BLACK_BOAT.getMaxUnitCount(), Qt.point(0, 20), -1);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BLACK_BOAT = new Constructor();
