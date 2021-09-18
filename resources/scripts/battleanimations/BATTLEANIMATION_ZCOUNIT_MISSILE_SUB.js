var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_MISSILE_SUB.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime)
    {
        sprite.loadMovingSprite("missile_sub",  false,
                                BATTLEANIMATION_ZCOUNIT_MISSILE_SUB.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("missile_sub+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_ZCOUNIT_MISSILE_SUB.getMaxUnitCount(), Qt.point(0, 20), movement, moveTime, false, -1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_MISSILE_SUB.loadStandingAnimation(sprite, unit, defender, weapon);
        var count = sprite.getUnitCount(5);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSingleMovingSprite("rocket_up", false, Qt.point(50, 75),
                                          Qt.point(128, 64), 400, false,
                                          -1, 1, -1, i * 150);
            sprite.loadSound("rocket_launch.wav", 1, i * 150);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 500 + 150 * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_MISSILE_SUB.getMaxUnitCount());
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        sprite.loadSprite("rocket_hit",  false, 5, Qt.point(0, 20),
                          1, 1.0, 0, 300);
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        sprite.loadMovingSprite("rocket_down", false, 5, Qt.point(127, 80),
                                Qt.point(-128, -64), 400, true,
                                -1, 1, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_flying.wav", 1, 0);
            sprite.loadSound("rockets_explode.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 1100 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_MISSILE_SUB.getMaxUnitCount());
    };

    this.hasDyingAnimation = function()
    {
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_MISSILE_SUB.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 2000);
        sprite.loadSound("ship_dying_move.wav", -2);
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        return 2200;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_MISSILE_SUB = new Constructor();
