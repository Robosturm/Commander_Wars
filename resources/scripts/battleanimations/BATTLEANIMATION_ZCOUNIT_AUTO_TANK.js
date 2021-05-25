var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_AUTO_TANK.getMaxUnitCount());
        var startX = -70;
        sprite.loadMovingSprite("auto_tank", false, sprite.getMaxUnitCount(), Qt.point(startX, 5),
                                Qt.point(65, 0), 600, false,
                                1, 1);
        sprite.loadMovingSpriteV2("auto_tank+mask", GameEnums.Recoloring_Table, sprite.getMaxUnitCount(), Qt.point(startX, 5),
                                  Qt.point(65, 0), 600, false,
                                  1, 1);
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(startX - 20, 7),
                                Qt.point(65, 0), 600, false,
                                1, 1);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("tank_move.wav", 5, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadSprite = function(sprite, unit)
    {
        var offset = Qt.point(-5, 5);
        sprite.loadSprite("auto_tank",  false,
                          BATTLEANIMATION_ZCOUNIT_AUTO_TANK.getMaxUnitCount(), offset);
        sprite.loadSpriteV2("auto_tank+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_ZCOUNIT_AUTO_TANK.getMaxUnitCount(), offset);
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_AUTO_TANK.loadSprite(sprite, unit);
        sprite.loadSprite("vehicle_dust_stop",  false,
                          BATTLEANIMATION_ZCOUNIT_AUTO_TANK.getMaxUnitCount(), Qt.point(-25, 7), 1);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_AUTO_TANK.loadSprite(sprite, unit);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var offset = Qt.point(0, 0);
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_AUTO_TANK.getMaxUnitCount());
        if (weapon === 0)
        {
            // gun
            BATTLEANIMATION_ZCOUNIT_AUTO_TANK.loadSprite(sprite, unit);
            offset = Qt.point(33, 21);
            // 41 24
            // -5  5
            // -3 -7
            sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 0.5, 0, 0);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            BATTLEANIMATION_ZCOUNIT_AUTO_TANK.loadSprite(sprite, unit);
            if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
            {
                // mg
                offset = Qt.point(27, 34);
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
            else
            {
                // mg
                offset = Qt.point(27, 29);
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
            // 32 29
            // -5  5
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
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_AUTO_TANK.getMaxUnitCount();
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 820 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_AUTO_TANK.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_AUTO_TANK = new Constructor();
