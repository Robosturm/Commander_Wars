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
        var maxCount = sprite.getMaxUnitCount();
        var startCount = sprite.getUnitCount(maxCount, sprite.getDyingStartHp());
        var endCount = sprite.getUnitCount(maxCount, sprite.getDyingEndHp());
        var musicSound = "explosion+land.wav";
        if (unit.getUnitType()  === GameEnums.UnitType_Air)
        {
            fadeoutTime = 300
            spriteId = "unit_explosion_air";
            offset = Qt.point(-30, 20);
            var unitId = unit.getUnitID();
            if (unitId === "K_HELI" ||
                unitId === "T_HELI")
            {
                musicSound = "explosion+copter.wav";
            }
            else
            {
                musicSound = "explosion+air.wav";
            }
        }
        if (sprite.getHasFired())
        {
            sprite.loadAnimation("loadStandingFiredAnimation", unit, defender, weapon);
        }
        else
        {
            sprite.loadAnimation("loadStandingAnimation", unit, defender, weapon);
        }
        sprite.loadDyingFadeOutAnimation(fadeoutTime);
        sprite.loadOnlyDyingMovingSprite(spriteId, GameEnums.Recoloring_None, offset,255);
        for (var i = maxCount; i >= maxCount - startCount + 1; --i)
        {
            if (i < (maxCount - endCount + 1))
            {
                sprite.loadSound(musicSound, 1, (i - (maxCount - startCount + 1)) * BATTLEANIMATION.defaultFrameDelay);
            }
        }
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
            sprite.loadSprite("bullet_tank",  false, sprite.getMaxUnitCount(), Qt.point(10, 30),
                              1, 1.0, 0, 0, true, true, 50);
            sprite.loadSprite("cannon_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 100, true);
            sprite.addSpriteScreenshake(8, 0.98, 800, 300);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("tank_hit.wav", 1, 50 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("impact_explosion.wav", 1, 100 + i * BATTLEANIMATION.defaultFrameDelay);
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
                              1, 1.0, 0, 0, true);
            BATTLEANIMATION.playMgImpactSound(sprite, unit, defender, weapon, count);
        }
    },

    playMgImpactSound : function(sprite, unit, defender, weapon, count)
    {
        for (i = 0; i < count; i++)
        {
            sprite.loadSound("mg_impact.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
    },

    showMgBullets : function(sprite, offset)
    {
        var mgFrames = 3;
        for (i = 0; i < mgFrames; ++i)
        {
            sprite.loadSprite("bullet_casing", false, sprite.getMaxUnitCount(), Qt.point(offset.x - 15, offset.y + 2),
                              1, 1, 0, mgFrames * BATTLEANIMATION.defaultFrameDelay, false, true);
        }
    },

    getImpactDurationMS : function(sprite, unit, defender, weapon)
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine        
        var count = sprite.getUnitCount(BATTLEANIMATION.getMaxUnitCount());
        if (weapon === 0)
        {
            return 150 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * count;
        }
        else
        {
            return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * count;
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

    spotterArmyData = [["ac", "ac"],
                       ["bm", "bm"],
                       ["dm", "dm"],
                       ["ge", "ge"],
                       ["os", "os"],
                       ["pf", "pf"],
                       ["ti", "ti"],
                       ["yc", "yc"],
                       // empty
                       ["bh", ""],
                       ["bg", ""],
                       ["ma", ""],],
    loadSpotterOrCoMini : function(sprite, unit, spotterNeeded)
    {
        var coMiniLoaded = false;
        if (settings.getUseCoMinis())
        {
            var owner = unit.getOwner();
            var co = owner.getCO(0);
            var coid = "";
            if (co !== null && co.getCOUnit() === unit)
            {
                coid = co.getCoID();
            }
            else
            {
                co = owner.getCO(1);
                if (co !== null && co.getCOUnit() === unit)
                {
                    coid = co.getCoID();
                }
            }
            if (coid !== "" &&
                sprite.existResAnim(coid + "+mini"))
            {
                sprite.loadCoMini(coid + "+mini", GameEnums.Recoloring_None, Qt.point(90, 5),
                                  Qt.point(0, 0), 1, false, 1, 1, 20);
                coMiniLoaded = true;
            }
        }
        if (!coMiniLoaded && spotterNeeded)
        {
            BATTLEANIMATION.loadSpotter(sprite, unit);
        }
    },
    loadSpotter : function(sprite, unit)
    {
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION.spotterArmyData);
        if (armyName !== "")
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
