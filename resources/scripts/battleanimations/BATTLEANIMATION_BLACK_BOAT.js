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
        sprite.loadMovingSprite("black_boat",  false,
                                BATTLEANIMATION_BLACK_BOAT.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("black_boat+mask", GameEnums.Recoloring_Table,
                                  BATTLEANIMATION_BLACK_BOAT.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 1200;
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BLACK_BOAT.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 1000);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BLACK_BOAT = new Constructor();
