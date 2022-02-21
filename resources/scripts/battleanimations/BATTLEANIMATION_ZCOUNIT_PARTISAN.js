var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.isMountain = function(terrainId)
    {
        if (terrainId === "MOUNTAIN" ||
            terrainId === "SNOW_MOUNTAIN" ||
            terrainId === "DESERT_ROCK" ||
            terrainId === "WASTE_MOUNTAIN")
        {
            return true
        }
        return false;
    };

    this.loadSprite = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_PARTISAN.getMaxUnitCount());
        var offset = Qt.point(-10, 5);
        if (weapon === 1)
        {
            sprite.loadSpriteV2("partisan+alt+mask", GameEnums.Recoloring_Matrix,
                              BATTLEANIMATION_ZCOUNIT_PARTISAN.getMaxUnitCount(), offset, count);
        }
        else
        {
            sprite.loadSpriteV2("partisan+mask", GameEnums.Recoloring_Matrix,
                                BATTLEANIMATION_ZCOUNIT_PARTISAN.getMaxUnitCount(), offset, count);
        }
        BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_PARTISAN.loadSprite(sprite, unit, defender, weapon);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_PARTISAN.getMaxUnitCount());
        BATTLEANIMATION_ZCOUNIT_PARTISAN.loadSprite(sprite, unit, defender, weapon);
        if (weapon === 1)
        {
            var offset = Qt.point(-5, 22);
            sprite.loadMovingSprite("firebomb", false, sprite.getMaxUnitCount(), offset,
                                    Qt.point(127, 40), 600, false,
                                    1, 1, -1);
        }
        else
        {
            var offset = Qt.point(20, 17);
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
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
        if (weapon === 0)
        {
            return 700 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_PARTISAN.getMaxUnitCount());
        }
        else
        {
            return 600 - BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_PARTISAN.getMaxUnitCount());
        }
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_ZCOUNIT_PARTISAN.loadStandingAnimation(sprite, unit, defender, weapon);
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
        }
        else
        {
            sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 2, 0);
        }
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_PARTISAN.getMaxUnitCount());
        var i = 0;
        if (weapon === 1)
        {
            sprite.loadSprite("cannon_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 600, true);
            sprite.addSpriteScreenshake(8, 0.95, 1000, 600);
            sprite.loadMovingSprite("firebomb", false, sprite.getMaxUnitCount(), Qt.point(127, 64),
                                    Qt.point(-127, -40), 600, true,
                                    1, 1, 0, 0, true);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("impact_explosion.wav", 1, 650 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            var yOffset = 22;
            if (unit.getUnitType()  === GameEnums.UnitType_Air)
            {
                yOffset = 40
            }
            sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, yOffset),
                              1, 1.0, 0, 0);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            return 600 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_PARTISAN.getMaxUnitCount());
        }
        else
        {
            return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_PARTISAN.getMaxUnitCount());
        }
    };

    this.getPositionOffset = function(sprite, unit, terrain, unitIdx)
    {
        if (terrain !== null)
        {
            if (BATTLEANIMATION_ZCOUNIT_PARTISAN.isMountain(terrain.getID()))
            {
                if (unitIdx >= 4)
                {
                    return Qt.point(-20 * (6 - unitIdx), 0);
                }
            }
        }
        return Qt.point(0, 0);
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        return false;
    };
    this.getMoveInDurationMS = function()
    {
        return 610;
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_PARTISAN.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_PARTISAN = new Constructor();
