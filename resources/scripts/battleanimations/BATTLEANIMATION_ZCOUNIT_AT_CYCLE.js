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
        sprite.loadSprite("at_cycle",  false,
                          BATTLEANIMATION_ZCOUNIT_AT_CYCLE.getMaxUnitCount(), Qt.point(-15, 5));
        sprite.loadSpriteV2("at_cycle+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_ZCOUNIT_AT_CYCLE.getMaxUnitCount(), Qt.point(-15, 5));
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_AT_CYCLE.getMaxUnitCount());
        BATTLEANIMATION_ZCOUNIT_AT_CYCLE.loadStandingAnimation(sprite, unit, defender, weapon);
        var offset = Qt.point(28, 20);
        sprite.loadMovingSprite("bazooka_os", false, sprite.getMaxUnitCount(), offset,
                                Qt.point(127, 0), 400, false,
                                1, 1, -1);
        offset.x = (offset.x - 20);
        sprite.loadSprite("bazooka_launch_start",  false, BATTLEANIMATION_MECH.getMaxUnitCount(), offset);

        // 9   21
        // -30 15
        offset = Qt.point(-36, 11);
        sprite.loadSprite("bazooka_launch",  false, BATTLEANIMATION_MECH.getMaxUnitCount(), offset);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("baazoka_fire.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_AT_CYCLE.getMaxUnitCount());
        sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                          1, 1.0, 0, 300);
        sprite.addSpriteScreenshake(8, 0.95, 800, 500);
        sprite.loadMovingSprite("bazooka_os", false, sprite.getMaxUnitCount(), Qt.point(127, 24),
                                Qt.point(-127, 0), 400, true,
                                1, 1, 0, 0, true);
        for (i = 0; i < count; i++)
        {
            sprite.loadSound("rocket_flying.wav", 1, "resources/sounds/", 0);
            sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 200 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_AT_CYCLE.getMaxUnitCount();
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 1500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_AT_CYCLE.getMaxUnitCount();
    }
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_AT_CYCLE = new Constructor();
