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
        sprite.loadMovingSpriteV2("royal_guard+mask", GameEnums.Recoloring_Matrix, sprite.getMaxUnitCount(), Qt.point(startX, 5),
                                  Qt.point(65, 0), 600, false,
                                  -1, 1);
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(startX - 20, 7),
                                Qt.point(65, 0), 600, false,
                                -1, 1);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("heavy_tank_move.wav", 5, i * BATTLEANIMATION.defaultFrameDelay);
        }
        BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
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
        sprite.loadSpriteV2("royal_guard" + ending + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.getMaxUnitCount(), offset, count);
        BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
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
                sprite.loadSound("tank_shot.wav", 1, 120 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.loadSprite(sprite, unit, defender, weapon, "", 1);
            // mg
            offset = Qt.point(12, 42);
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
            BATTLEANIMATION.showMgBullets(sprite, offset);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay, 1, true);
                sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay, 1, true);
            }
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.getMaxUnitCount());
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        return true;
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        return 610;
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_ROYAL_GUARD = new Constructor();
