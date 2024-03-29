var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_TRANSPORTPLANE.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime, alive = true)
    {
        sprite.loadMovingSpriteV2("transportplane+mask", GameEnums.Recoloring_Matrix,
                          BATTLEANIMATION_TRANSPORTPLANE.getMaxUnitCount(), Qt.point(0, 40), movement, moveTime);
        if (alive)
        {
            sprite.addMoveTweenToLastLoadedSprites(0, -3, 1200);
        }
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_TRANSPORTPLANE.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, -140), 1800, false);
        sprite.loadSound("airunit_dying.wav", 1);
    };

    this.getDyingDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 2000;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_TRANSPORTPLANE = new Constructor();
