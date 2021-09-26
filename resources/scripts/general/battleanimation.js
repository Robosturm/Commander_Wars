var BATTLEANIMATION =
{
    defaultFrameDelay : 75,
    getMaxUnitCount : function()
    {
        return 5;
    },

    loadStandingAnimation : function(sprite, unit, defender, weapon)
    {

    },

    hasMoveInAnimation : function(sprite, unit, defender, weapon)
    {
        // return true if the unit has an implementation for loadMoveInAnimation
        return false;
    },

    hasDyingAnimation : function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    },

    loadMoveInAnimation : function(sprite, unit, defender, weapon)
    {
        sprite.loadAnimation("loadStandingAnimation", unit, defender, weapon);
    },

    loadStopAnimation : function(sprite, unit, defender, weapon)
    {
        sprite.loadAnimation("loadStandingAnimation", unit, defender, weapon);
    },

    loadDyingAnimation : function(sprite, unit, defender, weapon)
    {
        var spriteId = "unit_explosion";
        var fadeoutTime = 600;
        var offset = Qt.point(-20, 20);
        if (unit.getUnitType()  === GameEnums.UnitType_Air)
        {
            fadeoutTime = 300
            spriteId = "unit_explosion_air";
            offset = Qt.point(-20, 20);
        }
        sprite.loadAnimation("loadStandingFiredAnimation", unit, defender, weapon);
        sprite.loadDyingFadeOutAnimation(fadeoutTime);
        sprite.loadOnlyDyingMovingSprite(spriteId, GameEnums.Recoloring_None, offset)
    },

    getDyingDurationMS : function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        if (unit.getUnitType()  === GameEnums.UnitType_Air)
        {
            return 700;
        }
        else
        {
            return 1300;
        }
    },

    loadStandingFiredAnimation : function(sprite, unit, defender, weapon)
    {
        sprite.loadAnimation("loadStandingAnimation", unit, defender, weapon);
    },

    loadFireAnimation : function(sprite, unit, defender, weapon)
    {
        sprite.loadAnimation("loadStandingAnimation", unit, defender, weapon);
    },

    getPositionOffset : function(sprite, unit, terrain, unitIdx)
    {
        return Qt.point(0, 0);
    },


    loadImpactUnitOverlayAnimation : function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 0);
        }
        else
        {
            sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
        }
    },

    loadImpactAnimation : function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION.getMaxUnitCount());
        var i = 0;
        if (weapon === 0)
        {
            sprite.loadSprite("cannon_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 0, true);
            sprite.addSpriteScreenshake(8, 0.98, 800, 200);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("impact_explosion.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                              1, 1.0, 0, 0, true);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    },

    getImpactDurationMS : function(sprite, unit, defender, weapon)
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine        
        var count = sprite.getUnitCount(BATTLEANIMATION.getMaxUnitCount());
        if (weapon === 0)
        {
            return 300 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * count;
        }
        else
        {
            return 800 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * count;
        }
    },

    getFireDurationMS : function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 100;
    },

    getFiredDurationMS : function(sprite, unit, defender, weapon)
    {
        // must be smaller or equal to the fire duration
        // the time will be scaled with animation speed inside the engine
        return -1;
    },

    getMoveInDurationMS : function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 100;
    },

    getStopDurationMS : function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 100;
    },

    spotterArmyData = [["os", "os"],
                       ["bm", "bm"],
                       ["ge", "ge"],
                       ["yc", "yc"],
                       ["ac", "ac"],
                       ["dm", "dm"],
                       ["ti", "ti"],
                       ["pf", "pf"],
                       ["bh", "bh"],
                       ["bg", "bg"],
                       ["ma", "ma"],],
    loadSpotter : function(sprite, unit)
    {
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION.spotterArmyData);
        if (armyName !== "bg" &&
            armyName !== "bh" &&
            armyName !== "ma")
        {
            sprite.loadSpriteV2("spotter+" + armyName + "+mask", GameEnums.Recoloring_Matrix, 1, Qt.point(90, 5),
                                1, 1, 20);
        }
    },

    getRelativePosition : function(attacker, defender)
    {
        if (defender === null)
        {
            return 0;
        }
        var defHigh = defender.getTerrain().getVisionHigh();
        var atkHigh = attacker.getTerrain().getVisionHigh();
        if (defender.getUnitType() === GameEnums.UnitType_Air &&
            attacker.getUnitType() !== GameEnums.UnitType_Air)
        {
            return 1;
        }
        else if ((defHigh > 1) || (atkHigh > 1))
        {
            if (defHigh > atkHigh)
            {
                return 1;
            }
            else if (defHigh < atkHigh)
            {
                return -1;
            }
        }
        return 0;
    },
};
