var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };
    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.getMaxUnitCount());
        var startX = -105;
        sprite.loadMovingSprite("royal_guard", false, sprite.getMaxUnitCount(), Qt.point(startX, 5),
                                Qt.point(65, 0), 600, false,
                                1, 1);
        sprite.loadMovingSpriteV2("royal_guard+mask", GameEnums.Recoloring_Table, sprite.getMaxUnitCount(), Qt.point(startX, 5),
                                  Qt.point(65, 0), 600, false,
                                  1, 1);
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(startX - 20, 7),
                                Qt.point(65, 0), 600, false,
                                1, 1);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("heavy_tank_move.wav", 5, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.loadSprite(sprite, unit, defender, weapon, "", 1);
        sprite.loadSprite("vehicle_dust_stop",  false,
                          BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.getMaxUnitCount(), Qt.point(-60, 7), 1);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.loadSprite(sprite, unit, defender, weapon, "", 1);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, count)
    {
        var offset = Qt.point(-40, 5);
        sprite.loadSprite("royal_guard" + ending,  false,
                          BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.getMaxUnitCount(), offset, count);
        sprite.loadSpriteV2("royal_guard" + ending + "+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.getMaxUnitCount(), offset, count);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var offset = Qt.point(0, 0);
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.getMaxUnitCount());
        if (weapon === 0)
        {
            BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.loadSprite(sprite, unit, defender, weapon, "", 1);
            offset = Qt.point(30, 22);
            sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1.0, 0, 120);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", 120 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.loadSprite(sprite, unit, defender, weapon, "", 1);
            // mg
            offset = Qt.point(12, 42);
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", 200 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", 400 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };


    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        // return true if the unit has an implementation for loadMoveInAnimation
        return true;
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 610;
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.getMaxUnitCount();
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 820 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD = new Constructor();
