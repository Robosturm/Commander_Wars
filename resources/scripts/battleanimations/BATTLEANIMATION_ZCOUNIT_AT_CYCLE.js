var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        // get army name
        sprite.loadSpriteV2("at_cycle+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_ZCOUNIT_AT_CYCLE.getMaxUnitCount(), Qt.point(-15, 5));
        BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_AT_CYCLE.getMaxUnitCount());
        BATTLEANIMATION_ZCOUNIT_AT_CYCLE.loadStandingAnimation(sprite, unit, defender, weapon);
        var offset = Qt.point(32, 14);
        sprite.loadMovingSprite("bazooka_yc", false, sprite.getMaxUnitCount(), offset,
                                Qt.point(160, 0), 500, false,
                                1, 1, -1);
        sprite.loadSprite("rocket_trailing_smoke",  false, BATTLEANIMATION_MECH.getMaxUnitCount(), offset);
        offset = Qt.point(offset.x - 76, offset.y - 5);
        sprite.loadSprite("bazooka_launch",  false, BATTLEANIMATION_MECH.getMaxUnitCount(), offset);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("baazoka_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
        BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
            return 700 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_AT_CYCLE.getMaxUnitCount());
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_AT_CYCLE.getMaxUnitCount());
        sprite.loadSprite("cannon_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 400, true);
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        sprite.loadMovingSprite("bazooka_yc", false, sprite.getMaxUnitCount(), Qt.point(127, 24),
                                Qt.point(-127, 0), 400, true,
                                1, 1, 0, 0, true);
        for (i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_flying.wav", 1, 0);
            sprite.loadSound("impact_explosion.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_AT_CYCLE.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_AT_CYCLE = new Constructor();
