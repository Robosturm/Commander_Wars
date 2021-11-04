var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };
    this.armyData = [["ac", "ac"],
                     ["bd", "bd"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["bm", "bm"],
                     ["dm", "dm"],
                     ["ge", "ge"],
                     ["gs", "gs"],
                     ["os", "os"],
                     ["ma", "bh"],
                     ["pf", "pf"],
                     ["ti", "ti"],
                     ["yc", "yc"],];

    this.loadStandingAnimation = function(sprite, unit, defender, weapon, startFrame = 0, endFrame = -1)
    {
        BATTLEANIMATION_MEGATANK.loadSprite(sprite, unit, defender, weapon, 0, 0);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, startFrame = 0, endFrame = -1)
    {
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_MEGATANK.armyData);
        sprite.loadSpriteV2("megatank+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_MEGATANK.getMaxUnitCount(), Qt.point(0, 10));
        sprite.loadSpriteV2("megatank+" + armyName + "+cannon+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_MEGATANK.getMaxUnitCount(), Qt.point(64, 10 + 32),
                            1, 1, 0, 0,
                            false, false, 200, endFrame, startFrame);
        BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        var offset = Qt.point(0, 0);
        if (weapon === 0)
        {
            BATTLEANIMATION_MEGATANK.loadSprite(sprite, unit, defender, weapon, 1, count);
            offset = Qt.point(108, 85);
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
            // 43
            BATTLEANIMATION_MEGATANK.loadSprite(sprite, unit, defender, weapon, 0, 0);
            if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
            {
                offset = Qt.point(68, 94);
                sprite.loadSprite("flak_shot_air",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0, false, true);
            }
            else
            {
                offset = Qt.point(72, 89);
                sprite.loadSprite("flak_shot",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0, false, true);
            }
            sprite.loadSound("mg_weapon_fire.wav", 1, 0);
            sprite.loadSound("mg_weapon_fire.wav", 1, 200);
            sprite.loadSound("mg_weapon_fire.wav", 1, 400);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            var count = sprite.getUnitCount(5);
            return 700 + 200 * count;
        }
        else
        {
            return 800;
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

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_MEGATANK.loadSprite(sprite, unit, defender, weapon, 6, 6);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        var i = 0;
        if (weapon === 0)
        {
            sprite.loadSprite("cannon_heavy_hit",  false, 5, Qt.point(0, 20),
                              1, 1.0, 0, 0, true);
            sprite.addSpriteScreenshake(8, 0.95, 800, 200);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("impact_explosion.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            var xOffset = 0;
            var yOffset = 22;
            if (unit.getUnitType() === GameEnums.UnitType_Air)
            {
                yOffset = 45;
            }
            sprite.loadSprite("flak_hit",  false, 5, Qt.point(xOffset, yOffset),
                              1, 1.0, 0, 0, true);
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
            return 50 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * count;
        }
        else
        {
            return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * count;
        }
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_MEGATANK = new Constructor();
