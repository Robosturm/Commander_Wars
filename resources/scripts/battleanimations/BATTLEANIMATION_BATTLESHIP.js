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
    this.animationData = [["os",[Qt.point(-120, 0), 1200, 20, Qt.point(88, 115), Qt.point(20, -18), Qt.point(18, -23), Qt.point(13, -27), Qt.point(0, 0), Qt.point(-8, -7)]],
                          ["bm",[Qt.point(-120, 0), 1200, 20, Qt.point(88, 115), Qt.point(20, -18), Qt.point(18, -23), Qt.point(13, -27), Qt.point(0, 0), Qt.point(-8, -7)]],
                          ["ge",[Qt.point(-120, 0), 1200, 20, Qt.point(88, 115), Qt.point(20, -18), Qt.point(18, -23), Qt.point(13, -27), Qt.point(0, 0), Qt.point(-8, -7)]],
                          ["yc",[Qt.point(-120, 0), 1200, 20, Qt.point(88, 115), Qt.point(20, -18), Qt.point(18, -23), Qt.point(13, -27), Qt.point(0, 0), Qt.point(-8, -7)]],
                          ["bh",[Qt.point(-120, 0), 1200, 20, Qt.point(88, 115), Qt.point(20, -18), Qt.point(18, -23), Qt.point(13, -27), Qt.point(0, 0), Qt.point(-8, -7)]],
                          ["ma",[Qt.point(-57, 0),    570, 0, Qt.point(0,  71),  Qt.point(20, -18), Qt.point(18, -23), Qt.point(13, -27), Qt.point(0, 0), Qt.point(-8, -7)]],];

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.setBackgroundSpeed(sprite.getBackgroundSpeed() + 1);
        var player = unit.getOwner();        
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BATTLESHIP.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_BATTLESHIP.animationData);
        var movement = data[0];
        var moveTime = data[1];
        var offset = Qt.point(data[2], 20);
        sprite.loadMovingSprite("battleship+" + armyName,  false,
                                BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), offset, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("battleship+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), offset, movement, moveTime, false, -1);
        if (armyName !== "ma")
        {
            var frameTime = 200;
            sprite.loadMovingSpriteV2("battleship+" + armyName + "+fire+mask", GameEnums.Recoloring_Matrix,
                                      BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), Qt.point(offset.x + 160, offset.y + 64),
                                      movement, moveTime, false, 1, 1.0, 0, 0, false, frameTime, 0);
        }
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 0;
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BATTLESHIP.baseStandingAnimation(sprite, unit, defender, weapon, 0, Qt.point(0, 0), 0);
    };

    this.baseStandingAnimation = function(sprite, unit, defender, weapon, fireFrames, movement, moveTime, startFrame = 0)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BATTLESHIP.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_BATTLESHIP.animationData);
        var offset = Qt.point(data[2] + data[0].x, 20);
        sprite.loadMovingSprite("battleship+" + armyName,  false,
                                BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), offset, movement, moveTime, false, -1);
        sprite.loadMovingSpriteV2("battleship+" + armyName + "+mask", GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), offset, movement, moveTime, false, -1);
        if (armyName !== "ma")
        {
            var frameTime = 200;
            sprite.loadMovingSpriteV2("battleship+" + armyName + "+fire+mask", GameEnums.Recoloring_Matrix,
                                      BATTLEANIMATION_BATTLESHIP.getMaxUnitCount(), Qt.point(offset.x + 160, offset.y + 64),
                                      movement, moveTime, false, 1, 1.0, 0, 0, false, frameTime, fireFrames, startFrame);
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.restoreBackgroundSpeed();
        var count = sprite.getUnitCount(5);
        BATTLEANIMATION_BATTLESHIP.baseStandingAnimation(sprite, unit, defender, weapon, count, Qt.point(0, 0), 0, 1);
        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_BATTLESHIP.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_BATTLESHIP.animationData)
        var offset = data[3];
        for (var i = 0; i < count; i++)
        {
            var offset2 = data[4 + i];
            sprite.loadSingleMovingSprite("battleship_shot", false,
                                          Qt.point(offset.x + offset2.x,
                                                   offset.y + offset2.y),
                                          Qt.point(0, 0), 0, true,
                                          1, 1.0, 5, 200 * i);
            sprite.loadSound("cannon_weapon_fire.wav", 1, i * 200);
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
        if (defender.getUnitType()  === GameEnums.UnitType_Naval)
        {
            for (var i = 0; i < count; i++)
            {
                sprite.loadSingleMovingSprite("water_hit",  false, Qt.point(68 - i * 25, 20),
                                              Qt.point(0, 0), 0, false,
                                              1, 1.0, 2, i * BATTLEANIMATION.defaultFrameDelay * 2, true,
                                              100, -1, 0, 0, 180);
            }
        }
        else
        {
            sprite.loadSprite("artillery_heavy_hit",  false, 5, Qt.point(-16, 20),
                              1, 1.0, 0, 0, true);
        }
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("impact_explosion.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
        }
        sprite.addSpriteScreenshake(8, 0.95, 800, 200);

    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        if (defender.getUnitType()  === GameEnums.UnitType_Naval)
        {
            return 500 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * 2 * sprite.getUnitCount(5);
        }
        else
        {
            return 500 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(5);
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

    this.hasDyingAnimation = function()
    {
        return true;
    };

    this.loadDyingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_BATTLESHIP.baseStandingAnimation(sprite, unit, defender, weapon, 0, Qt.point(-140, 0), 2000);
        sprite.loadSound("ship_dying_move.wav", -2);
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        return 1500;
    };

    this.getDyingDurationMS = function(sprite, unit, defender, weapon)
    {
        return 2200;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_BATTLESHIP = new Constructor();
