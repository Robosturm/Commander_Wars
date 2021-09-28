var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 1;
    };
    this.armyData = [["os", "os"],
                     ["bm", "bm"],
                     ["ge", "ge"],
                     ["yc", "yc"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["ma", "ma"],];

    this.animationData = [["os", [20, Qt.point(-85, 0), 850, Qt.point(57, 79), Qt.point(96, 76), Qt.point(100, 63), Qt.point(0, 0), Qt.point(-5, -5), Qt.point(-10, -10), Qt.point(2, -7), Qt.point(-3, -12)]],
                          ["bm", [20, Qt.point(-85, 0), 850, Qt.point(57, 79), Qt.point(96, 76), Qt.point(100, 63), Qt.point(0, 0), Qt.point(-5, -5), Qt.point(-10, -10), Qt.point(2, -7), Qt.point(-3, -12)]],
                          ["ge", [20, Qt.point(-85, 0), 850, Qt.point(57, 79), Qt.point(96, 76), Qt.point(100, 63), Qt.point(0, 0), Qt.point(-5, -5), Qt.point(-10, -10), Qt.point(2, -7), Qt.point(-3, -12)]],
                          ["yc", [20, Qt.point(-85, 0), 850, Qt.point(57, 79), Qt.point(96, 76), Qt.point(100, 63), Qt.point(0, 0), Qt.point(-5, -5), Qt.point(-10, -10), Qt.point(2, -7), Qt.point(-3, -12)]],
                          ["bh", [20, Qt.point(-85, 0), 850, Qt.point(57, 79), Qt.point(96, 76), Qt.point(100, 63), Qt.point(0, 0), Qt.point(-5, -5), Qt.point(-10, -10), Qt.point(2, -7), Qt.point(-3, -12)]],
                          ["bg", [20, Qt.point(-85, 0), 850, Qt.point(57, 79), Qt.point(96, 76), Qt.point(100, 63), Qt.point(0, 0), Qt.point(-5, -5), Qt.point(-10, -10), Qt.point(2, -7), Qt.point(-3, -12)]],
                          ["ma", [0,  Qt.point(-7, 0),   10, Qt.point(45, 71), Qt.point(93, 57), Qt.point(93,  52), Qt.point(0, 0), Qt.point(-5, -5), Qt.point(0, -6),    Qt.point(2, 10), Qt.point(15, -12)]],];

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.setBackgroundSpeed(sprite.getBackgroundSpeed() + 1);
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_CRUISER.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_CRUISER.animationData);
        BATTLEANIMATION_CRUISER.loadSprite(sprite, unit, defender, weapon, data[1], data[2], 0, 0, 0, 0, Qt.point(data[0], 20));
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 0;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_CRUISER.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_CRUISER.animationData);
        BATTLEANIMATION_CRUISER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, 0, 0, 0, 0, Qt.point(data[0] + data[1].x, 20));
    };

    this.loadSprite = function(sprite, unit, defender, weapon, movement, moveTime, mgStartFrame, mgEndFrame, rocketStartFrame, rocketEndFrame, startPos,
                               rocketSprite = "+missile", mgSprite = "+cannon+fire+air")
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_CRUISER.armyData);
        sprite.loadMovingSprite("cruiser+" + armyName,  false,
                                BATTLEANIMATION_CRUISER.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("cruiser+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_CRUISER.getMaxUnitCount(), startPos, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("cruiser+" + armyName + rocketSprite + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_CRUISER.getMaxUnitCount(), Qt.point(startPos.x + 80, startPos.y + 32),
                                  movement, moveTime, false, 1, 1, 0, 0,
                                  false, 100, rocketEndFrame, rocketStartFrame);
        sprite.loadMovingSpriteV2("cruiser+" + armyName + mgSprite + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_CRUISER.getMaxUnitCount(), Qt.point(startPos.x + 80 + 64, startPos.y + 32),
                                  movement, moveTime, false, 3, 1, 0, 0,
                                  false, 100, mgEndFrame, mgStartFrame);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.restoreBackgroundSpeed();
        var count = sprite.getUnitCount(5);
        var player = unit.getOwner();
        var rocketSprite = "+missile";
        var position = BATTLEANIMATION.getRelativePosition(unit, defender);
        var mgSprite = "+cannon+fire+air";
        var mgStartFrame = 0;
        var mgEndFrame = 0;
        var rocketEndFrame = 0;
        if (weapon === 0)
        {
            rocketSprite = "+missile+fire";
            rocketEndFrame = count - 1;
        }
        else
        {
            if (position <= 0)
            {
                mgSprite = "+cannon+fire+ground";
                mgStartFrame = 1;
                mgEndFrame = 2;
            }
            else
            {
                mgEndFrame = 1;
            }
        }
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_CRUISER.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_CRUISER.animationData);
        BATTLEANIMATION_CRUISER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, 
                                           mgStartFrame, mgEndFrame, 0, rocketEndFrame, Qt.point(data[0] + data[1].x, 20),
                                           rocketSprite, mgSprite);
        var offset = Qt.point(0, 0);
        if (weapon === 0)
        {
            offset = data[3];
            for (var i = 0; i < count; i++)
            {
                var offset2 = data[6 + i];
                sprite.loadSingleMovingSprite("rocket_up", false,
                                              Qt.point(offset.x + offset2.x,
                                                       offset.y + offset2.y),
                                              Qt.point(80, 40), 400, true,
                                              -1, 1.0, 5, 100 * i, false);
                sprite.loadSound("rocket_launch.wav", 1, 100 * i);
            }
        }
        else
        {
            if (position > 0)
            {
                offset = data[4];
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0, false, true);
            }
            else
            {
                offset = data[5];
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0, false, true);
            }

            sprite.loadSound("anti_air_gun_fire.wav", 1, 0);
            sprite.loadSound("anti_air_gun_fire.wav", 1, 200);
            sprite.loadSound("anti_air_gun_fire.wav", 1, 400);
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            return sprite.getUnitCount(5) * 100 + 400;
        }
        else
        {
            return 700;
        }
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        var rocketEndFrame = 0;
        if (weapon === 0)
        {
            rocketEndFrame = sprite.getUnitCount(5);
        }
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_CRUISER.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_CRUISER.animationData);
        BATTLEANIMATION_CRUISER.loadSprite(sprite, unit, defender, weapon, Qt.point(0, 0), 0, 0, 0, rocketEndFrame, rocketEndFrame, Qt.point(data[0] + data[1].x, 20));
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_CRUISER.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_CRUISER.animationData);
        var rocketEndFrame = 0;
        if (weapon === 0)
        {
            rocketEndFrame = sprite.getFireUnitCount(5);
        }
        BATTLEANIMATION_CRUISER.loadSprite(sprite, unit, defender, weapon, Qt.point(-140, 0), 2000, 0, 0, rocketEndFrame, rocketEndFrame, Qt.point(data[0] + data[1].x, 20));
        sprite.loadSound("ship_dying_move.wav", -2);
    };


    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(5);
        var i = 0;
        if (weapon === 0)
        {
            sprite.loadSprite("rocket_hit",  false, 5, Qt.point(0, 20),
                              1, 1.0, 0, 400, true);
            sprite.addSpriteScreenshake(8, 0.95, 800, 500);
            sprite.loadMovingSprite("rocket_down", false, 5, Qt.point(127, 90),
                                    Qt.point(-127, -60), 400, true,
                                    -1, 1, 0, 0, true);
            for (i = 0; i < count; i++)
            {
                sprite.loadSound("rocket_flying.wav", 1, 0);
                sprite.loadSound("impact_explosion.wav", 1, 300 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            var yOffset = 22;
            if (unit.getUnitType()  === GameEnums.UnitType_Air)
            {
                yOffset = 40
            }
            sprite.loadSprite("mg_hit",  false, 5, Qt.point(0, yOffset),
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
            return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(5);
        }
        else
        {
            return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(5);
        }
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        return true;
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_CRUISER.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_CRUISER.animationData);
        return data[2];
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 2200;
    };

    this.hasDyingAnimation = function()
    {
        // return true if the unit has an implementation for loadDyingAnimation
        return true;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_CRUISER = new Constructor();
