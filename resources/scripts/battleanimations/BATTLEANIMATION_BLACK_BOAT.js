var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BLACK_BOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime)
    {
        sprite.loadMovingSpriteV2("black_boat", GameEnums.Recoloring_None,
                                  BATTLEANIMATION_BLACK_BOAT.getMaxUnitCount(), Qt.point(-25, 20), movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("black_boat+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_BLACK_BOAT.getMaxUnitCount(), Qt.point(-25, 20), movement, moveTime, false, -1);
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 2200;
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BLACK_BOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 2000);
        sprite.loadSound("ship_dying_move.wav", -2);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BLACK_BOAT = new Constructor();
