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
            terrainId === "DESERT_ROCK")
        {
            return true
        }
        return false;
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_MECH.loadMoveInAnimation(sprite, unit, defender, weapon);
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_MECH.loadStopAnimation(sprite, unit, defender, weapon);
    };

    this.loadSprite = function(sprite, unit, defender)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_COMMANDO.getMaxUnitCount());
        var offset = Qt.point(-10, 5);
        sprite.loadSpriteV2("commando+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_ZCOUNIT_COMMANDO.getMaxUnitCount(), offset, count);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1)
        {
            BATTLEANIMATION_MECH.loadSprite(sprite, unit, defender, weapon, "", 1);
        }
        else
        {
            BATTLEANIMATION_ZCOUNIT_COMMANDO.loadSprite(sprite, unit, defender);
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_COMMANDO.getMaxUnitCount());
        if (weapon === 1)
        {
            BATTLEANIMATION_MECH.loadFireAnimation(sprite, unit, defender, weapon);
        }
        else
        {
            BATTLEANIMATION_ZCOUNIT_COMMANDO.loadSprite(sprite, unit, defender);
            var offset = Qt.point(18, 15);
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("mg_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            return 500 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_COMMANDO.getMaxUnitCount());
        }
        else
        {
            return 600 - BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_COMMANDO.getMaxUnitCount());
        }
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1)
        {
            BATTLEANIMATION_MECH.loadSprite(sprite, unit, defender, weapon, "+fired", 1);
        }
        else
        {
            BATTLEANIMATION_ZCOUNIT_COMMANDO.loadStandingAnimation(sprite, unit, defender, weapon);
        }
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

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            return 600 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_COMMANDO.getMaxUnitCount());
        }
        else
        {
            return 800 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_COMMANDO.getMaxUnitCount());
        }
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_ZCOUNIT_COMMANDO.getMaxUnitCount());
        var i = 0;
        if (weapon === 1)
        {
            sprite.loadSprite("cannon_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 300);
            sprite.addSpriteScreenshake(8, 0.95, 800, 500);
            sprite.loadMovingSprite("bazooka_os", false, sprite.getMaxUnitCount(), Qt.point(127, 24),
                                    Qt.point(-127, 0), 400, true,
                                    1, 1, 0, 0, true);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("rocket_flying.wav", 1, 0);
                sprite.loadSound("impact_explosion.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                              1, 1.0, 0, 0);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.getPositionOffset = function(sprite, unit, terrain, unitIdx)
    {
        if (terrain !== null)
        {
            if (BATTLEANIMATION_ZCOUNIT_COMMANDO.isMountain(terrain.getID()))
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
        var terrainId = unit.getTerrain().getTerrainID();
        if (BATTLEANIMATION_ZCOUNIT_COMMANDO.isMountain(terrainId) || weapon === 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    };
    this.getMoveInDurationMS = function()
    {
        return 610;
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_ZCOUNIT_COMMANDO.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_ZCOUNIT_COMMANDO = new Constructor();
