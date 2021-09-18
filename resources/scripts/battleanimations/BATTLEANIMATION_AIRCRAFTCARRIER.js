var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_AIRCRAFTCARRIER.loadSprite(sprite, unit, defender, weapon, Qt.point(-130, 20), Qt.point(0, 0), 0);
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.setBackgroundSpeed(sprite.getBackgroundSpeed() + 1);
        BATTLEANIMATION_AIRCRAFTCARRIER.loadSprite(sprite, unit, defender, weapon, Qt.point(20, 20), Qt.point(-150, 0), 1500);
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 0;
    };

    this.loadSprite = function(sprite, unit, defender, weapon, startPos, movement, moveTime)
    {
        sprite.loadMovingSprite("aircraftcarrier",  false,
                                BATTLEANIMATION_AIRCRAFTCARRIER.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("aircraftcarrier+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_AIRCRAFTCARRIER.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.restoreBackgroundSpeed();
        BATTLEANIMATION_AIRCRAFTCARRIER.loadStandingAnimation(sprite, unit, defender, weapon);
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSingleMovingSprite("rocket_up", false, Qt.point(40, 75),
                                          Qt.point(128, 64), 500, false,
                                          -1, 1, -1, i * 150);
            sprite.loadSound("rocket_launch.wav", 1, i * 150);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        return 500 + 150 * count;
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        sprite.loadMovingSprite("rocket_hit_air", false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                                Qt.point(-30, 15), 300, false,
                                1, 1.0, 0, 300);
        sprite.loadMovingSprite("rocket_up", false, 5, Qt.point(127, 0),
                                Qt.point(-128, 64), 400, true,
                                -1, 1, 0, 0, true);
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_flying.wav", 1, 0);
            sprite.loadSound("rockets_explode.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(5);
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        return true;
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        return 1510;
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        return 2200;
    };

    this.hasDyingAnimation = function()
    {
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_AIRCRAFTCARRIER.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 2000);
        sprite.loadSound("ship_dying_move.wav", -2);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_AIRCRAFTCARRIER = new Constructor();
