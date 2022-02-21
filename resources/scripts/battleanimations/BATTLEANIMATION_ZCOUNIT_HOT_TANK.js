var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_HOT_TANK.getMaxUnitCount());
        sprite.loadMovingSpriteV2("hot_tank+mask", GameEnums.Recoloring_Matrix, sprite.getMaxUnitCount(), Qt.point(-70, 5),
                                  Qt.point(65, 0), 600, false, -1, 1);
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(-90, 7),
                                Qt.point(65, 0), 600, false, -1, 1);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("recon_move.wav", 5, i * BATTLEANIMATION.defaultFrameDelay);
        }
        BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_HOT_TANK.loadSprite(sprite, unit);
        sprite.loadSprite("vehicle_dust_stop",  false,
                          BATTLEANIMATION_ZCOUNIT_HOT_TANK.getMaxUnitCount(), Qt.point(-40, 7), 1);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_HOT_TANK.loadSprite(sprite, unit);
    };

    this.loadSprite = function(sprite, unit)
    {
        var offset = Qt.point(-5, 5);
        sprite.loadSpriteV2("hot_tank+mask", GameEnums.Recoloring_Matrix,
                          BATTLEANIMATION_ZCOUNIT_HOT_TANK.getMaxUnitCount(), offset);
        BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
    };


    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_HOT_TANK.getMaxUnitCount());
        var offset = Qt.point(40, 27);
        if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
        {
            BATTLEANIMATION_ZCOUNIT_HOT_TANK.loadSprite(sprite, unit, defender, weapon, "+fire_air", 2);
            offset = Qt.point(40, 32);
            sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
        }
        else
        {
            BATTLEANIMATION_ZCOUNIT_HOT_TANK.loadSprite(sprite, unit, defender, weapon, "+fire", 2);
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
        }
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay, 1, true);
            sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay, 1, true);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_HOT_TANK.getMaxUnitCount());
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        return true;
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_HOT_TANK.getMaxUnitCount());
        var yOffset = 22;
        if (unit.getUnitType()  === GameEnums.UnitType_Air)
        {
            yOffset = 40
        }
        sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, yOffset),
                          1, 1.0, 0, 0, true);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_HOT_TANK.getMaxUnitCount());
    };

    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        return 610;
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_HOT_TANK.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_HOT_TANK = new Constructor();
