var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_CHAPERON.getMaxUnitCount());
        var startX = -70;
        sprite.loadMovingSpriteV2("chaperon+mask", GameEnums.Recoloring_Matrix, sprite.getMaxUnitCount(), Qt.point(startX, 5),
                                  Qt.point(65, 0), 600, false,
                                  1, 1);
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(startX - 20, 7),
                                Qt.point(65, 0), 600, false,
                                1, 1);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("tank_move.wav", 5, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_CHAPERON.loadSprite(sprite, unit);
        sprite.loadSprite("vehicle_dust_stop",  false,
                          BATTLEANIMATION_ZCOUNIT_CHAPERON.getMaxUnitCount(), Qt.point(-25, 7), 1);
    };


    this.loadSprite = function(sprite, unit)
    {
        var offset = Qt.point(-5, 5);
        sprite.loadSpriteV2("chaperon+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_ZCOUNIT_CHAPERON.getMaxUnitCount(), offset);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_CHAPERON.loadSprite(sprite, unit);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var offset = Qt.point(0, 0);
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_CHAPERON.getMaxUnitCount());
        BATTLEANIMATION_ZCOUNIT_CHAPERON.loadSprite(sprite, unit);
        // mg
        offset = Qt.point(33, 39);
        sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                          1, 1, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_CHAPERON.getMaxUnitCount());
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                          1, 1.0, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_CHAPERON.getMaxUnitCount());
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
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_CHAPERON.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_CHAPERON = new Constructor();
