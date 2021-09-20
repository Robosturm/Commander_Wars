var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.setBackgroundSpeed(sprite.getBackgroundSpeed() + 1);
        BATTLEANIMATION_DESTROYER.loadSprite(sprite, unit, defender, weapon, Qt.point(-116, 0), 1200, 0, 0, 0, 0,
                                             "+cannon", "+flak", Qt.point(20, 20));
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 0;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_DESTROYER.loadSprite(sprite, unit, defender, weapon);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement = Qt.point(0, 0), moveTime = 0, mgStartFrame = 0, mgEndFrame = 0,
                               rocketStartFrame = 0, rocketEndFrame = 0,
                               rocketSprite = "+cannon", mgSprite = "+flak", startPos = Qt.point(-96, 20))
    {
        var player = unit.getOwner();
        // get army name
        sprite.loadMovingSprite("destroyer",  false,
                                BATTLEANIMATION_DESTROYER.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("destroyer+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_DESTROYER.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("destroyer" + rocketSprite + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_DESTROYER.getMaxUnitCount(), Qt.point(startPos.x + 160, startPos.y + 56),
                                  movement, moveTime, false, 1, 1, 0, 0,
                                  false, 200, rocketEndFrame, rocketStartFrame);
        sprite.loadMovingSpriteV2("destroyer" + mgSprite + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_DESTROYER.getMaxUnitCount(), Qt.point(startPos.x + 100, startPos.y + 64),
                                  movement, moveTime, false, 3, 1, 0, 0,
                                  false, 100, mgEndFrame, mgStartFrame);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.restoreBackgroundSpeed();
        var offset = Qt.point(0, 0);
        if (weapon === 0)
        {
            var count = sprite.getUnitCount(5);
            BATTLEANIMATION_DESTROYER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, 0, 0,
                                                 0, count - 1, "+cannon+fire");
            offset = Qt.point(100, 100);
            for (var i = 0; i < count; i++)
            {
                var offset2 = Qt.point(0, 0);
                switch (i)
                {
                case 1:
                    offset2 = Qt.point(-6, -5);
                    break;
                case 2:
                    offset2 = Qt.point(-11, -5);
                    break;
                    //
                case 3:
                    offset2 = Qt.point(6, -32);
                    break;
                case 4:
                    offset2 = Qt.point(-12, -38);
                    break;
                }
                sprite.loadSingleMovingSprite("medium_shot", false,
                                              Qt.point(offset.x + offset2.x,
                                                       offset.y + offset2.y),
                                              Qt.point(0, 0), 0, true,
                                              1, 1.0, 5, 200 * i, false);
                sprite.loadSound("megacannon_weapon_fire.wav", 1, 200 * i);
            }
        }
        else
        {
            BATTLEANIMATION_DESTROYER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, 0, 3,
                                                 0, 0, "+cannon", "+flak+fire");
            offset = Qt.point(29, 109);
            for (var i2 = 0; i2 < 2; ++i2)
            {
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(),
                                  Qt.point(offset.x - i2 * 17, offset.y - i2 * 11),
                                  1, 1, 0, 0, false, true);
            }
            sprite.loadSound("anti_air_gun_fire.wav", 1, 0);
            sprite.loadSound("anti_air_gun_fire.wav", 1, 100);
            sprite.loadSound("anti_air_gun_fire.wav", 1, 200);
            sprite.loadSound("anti_air_gun_fire.wav", 1, 300);
            sprite.loadSound("anti_air_gun_fire.wav", 1, 400);
            sprite.loadSound("anti_air_gun_fire.wav", 1, 500);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            var count = sprite.getUnitCount(5);
            return 600 + 200 * count;
        }
        else
        {
            return 1100;
        }
    };

    this.getFiredDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            var count = sprite.getUnitCount(5);
            return 200 * count;
        }
        else
        {
            return -1;
        }
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        var i = 0;
        if (weapon === 0)
        {
            sprite.loadSprite("cannon_heavy_hit",  false, 5, Qt.point(0, 20),
                              1, 1.0, 0, 0, true);
            sprite.addSpriteScreenshake(8, 0.98, 800, 200);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("impact_explosion.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            sprite.loadSprite("mg_hit",  false, 5, Qt.point(0, 22),
                              1, 1.0, 0, 0);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        if (weapon === 0)
        {
            return 300 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * count;
        }
        else
        {
            return 800 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * count;
        }
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        return true;
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        return 1210;
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        return 2200;
    };

    this.hasDyingAnimation = function()
    {
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_DESTROYER.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 2000);
        sprite.loadSound("ship_dying_move.wav", -2);
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_DESTROYER = new Constructor();
