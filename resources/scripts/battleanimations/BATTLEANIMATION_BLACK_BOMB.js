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
        sprite.loadMovingSpriteV2("black_bomb+mask", GameEnums.Recoloring_Matrix,
                          BATTLEANIMATION_BLACK_BOMB.getMaxUnitCount(), Qt.point(-1, 40), movement, moveTime, false, -1);
        if (alive)
        {
            sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
        }
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 2000;
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BLACK_BOMB.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, -140), 1800, false);
        sprite.loadSound("airunit_dying.wav", 1);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BLACK_BOMB = new Constructor();
