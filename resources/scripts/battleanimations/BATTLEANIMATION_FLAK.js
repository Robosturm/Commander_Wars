var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };
    this.armyData = [["ac", "ac"],
                     ["bd", "bd"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["bm", "bm"],
                     ["ge", "ge"],
                     ["ma", "ma"],
                     ["os", "os"],
                     ["pf", "pf"],
                     ["ti", "ti"],
                     ["yc", "yc"],];
    this.animationData = [["ac", [2, Qt.point(38, 31), Qt.point(38, 20), 1300]],
                          ["bd", [2, Qt.point(17, 43), Qt.point(20, 31), 1300]],
                          ["bh", [2, Qt.point(31, 41), Qt.point(33, 29), 1300]],
                          ["bm", [2, Qt.point(30, 38), Qt.point(31, 24), 1300]],
                          ["ge", [2, Qt.point(31, 43), Qt.point(33, 29), 1300]],
                          ["ma", [2, Qt.point(24, 53), Qt.point(24, 48), 1300]],
                          ["os", [2, Qt.point(35, 40), Qt.point(31, 26), 1300]],
                          ["pf", [1, Qt.point(33, 50), Qt.point(33, 33), 1200]],
                          ["ti", [1, Qt.point(30, 35), Qt.point(33, 32), 1200]],
                          ["yc", [1, Qt.point(34, 46), Qt.point(34, 29), 1200]],];

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_FLAK.getMaxUnitCount());
        var startX = -75;
        BATTLEANIMATION_FLAK.loadSprite(sprite, unit, defender, weapon, "+move", Qt.point(startX, 5), Qt.point(65, 0), 600, -1);
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(startX - 20, 7),
                                Qt.point(65, 0), 600, false,
                                1, 1);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("tank_move.wav", 5, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        return 610;
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 500 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_FLAK.getMaxUnitCount());
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_FLAK.loadSprite(sprite, unit, defender, weapon, "+stop", Qt.point(-10, 5), Qt.point(0, 0), 0, 1);
        sprite.loadSprite("vehicle_dust_stop",  false,
                          BATTLEANIMATION_FLAK.getMaxUnitCount(), Qt.point(-30, 7), 1);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_FLAK.loadSprite(sprite, unit, defender, weapon, "");
    };

    this.loadSprite = function(sprite, unit, defender, weapon, spriteId, startPos = Qt.point(-10, 5), movement = Qt.point(0, 0), moveTime = 0, loops = 1)
    {

        var player = unit.getOwner();
        // get army name
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_FLAK.armyData);
        var airName = "";
        if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
        {
            airName = "+air";
        }
        sprite.loadMovingSpriteV2("flak+" + armyName + spriteId + airName + "+mask",  GameEnums.Recoloring_Matrix,
                                  BATTLEANIMATION_FLAK.getMaxUnitCount(), startPos, movement, moveTime, false, loops);
    }

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_FLAK.getMaxUnitCount());
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_FLAK.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_FLAK.animationData);
        var offset = Qt.point(-10, 5);
        BATTLEANIMATION_FLAK.loadSprite(sprite, unit, defender, weapon, "+fire", Qt.point(-10, 5), Qt.point(0, 0), 0, 3);
        var mgCount = data[0];
        if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
        {
            offset = data[1];
            for (var i2 = 0; i2 < mgCount; ++i2)
            {
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(),
                                  Qt.point(offset.x + i2 * 5, offset.y + i2 * 5),
                                  1, 1, 0, 100 * i2);
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(),
                                  Qt.point(offset.x + i2 * 5, offset.y + i2 * 5),
                                  1, 1, 0, 600 + 100 * i2);
            }
        }
        else
        {
            offset = data[2];
            for (var i2 = 0; i2 < mgCount; ++i2)
            {
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(),
                                  Qt.point(offset.x + i2 * 4, offset.y + i2 * 4),
                                  1, 1, 0, 100 * i2);
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(),
                                  Qt.point(offset.x + i2 * 4, offset.y + i2 * 4),
                                  1, 1, 0, 600 + 100 * i2);
            }
        }
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("vulcan_weapon_fire.wav", 1, i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("vulcan_weapon_fire.wav", 1, 200 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("vulcan_weapon_fire.wav", 1, 400 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("vulcan_weapon_fire.wav", 1, 600 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("vulcan_weapon_fire.wav", 1, 800 + i * BATTLEANIMATION.defaultFrameDelay);
            sprite.loadSound("vulcan_weapon_fire.wav", 1, 1000 + i * BATTLEANIMATION.defaultFrameDelay);
            if (mgCount === 2)
            {
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 100 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 300 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 500 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 700 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 900 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 1100 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_FLAK.getMaxUnitCount());
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_FLAK.armyData);
        var data = Global.getArmyDataFromTable(armyName, BATTLEANIMATION_FLAK.animationData);
        return data[3] + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_FLAK.getMaxUnitCount());
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
        return 800 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_FLAK.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_FLAK = new Constructor();
