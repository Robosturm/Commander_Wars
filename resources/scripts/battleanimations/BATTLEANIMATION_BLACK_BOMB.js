var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BLACK_BOMB.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime, alive = true)
    {
        sprite.loadMovingSprite("black_bomb",  false,
                          BATTLEANIMATION_BLACK_BOMB.getMaxUnitCount(), Qt.point(0, 40), movement, moveTime, false, -1);
        if (alive)
        {
            sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
        }
        sprite.loadMovingSpriteV2("black_bomb+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_BLACK_BOMB.getMaxUnitCount(), Qt.point(0, 40), movement, moveTime, false, -1);
        if (alive)
        {
            sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
        }
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 1000;
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BLACK_BOMB.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, -140), 600, false);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BLACK_BOMB = new Constructor();
