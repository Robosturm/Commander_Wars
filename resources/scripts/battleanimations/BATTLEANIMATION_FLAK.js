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
                     ["dm", "dm"],
                     ["ge", "ge"],
                     ["ma", "ma"],
                     ["os", "os"],
                     ["pf", "pf"],
                     ["ti", "ti"],
                     ["yc", "yc"],];
    this.animationData = [["ac", [2, 900, 1,  ""]],
                          ["bd", [4, 900, 2, "+single"]],
                          ["bh", [2, 900, 1,  ""]],
                          ["bm", [2, 900, 1,  ""]],
                          ["dm", [2, 900, 1,  ""]],
                          ["ge", [2, 900, 1,  ""]],
                          ["ma", [2, 900, 1,  ""]],
                          ["os", [2, 900, 1,  ""]],
                          ["pf", [2, 900, 1,  ""]],
                          ["ti", [1, 800, 1,  ""]],
                          ["yc", [1, 800, 1,  ""]],];

    this.animationMgDataAir = [["ac", [0, Qt.point(39, 32), 0, Qt.point(41, 34)]],
                               ["bd", [0, Qt.point(17, 44), 0, Qt.point(17, 35), 0, Qt.point(22, 47), -1, Qt.point(23, 41)]],
                               ["bh", [0, Qt.point(31, 43), 0, Qt.point(35, 47)]],
                               ["bm", [0, Qt.point(30, 38), 0, Qt.point(34, 42)]],
                               ["dm", [0, Qt.point(35, 35), 0, Qt.point(44, 45)]],
                               ["ge", [0, Qt.point(32, 43), 0, Qt.point(35, 47)]],
                               ["ma", [0, Qt.point(25, 50), 0, Qt.point(30, 54)]],
                               ["os", [0, Qt.point(31, 41), 0, Qt.point(35, 45)]],
                               ["pf", [0, Qt.point(33, 50), 0, Qt.point(31, 46)]],
                               ["ti", [0, Qt.point(30, 46)]],
                               ["yc", [0, Qt.point(34, 46)]]];

    this.animationMgData    = [["ac", [0, Qt.point(39, 20), 0, Qt.point(41, 22)]],
                               ["bd", [0, Qt.point(19, 32), -1, Qt.point(26, 36), 0, Qt.point(21, 24), -1, Qt.point(26, 28)]],
                               ["bh", [0, Qt.point(33, 29), 0, Qt.point(36, 32)]],
                               ["bm", [0, Qt.point(31, 24), 0, Qt.point(34, 27)]],
                               ["dm", [0, Qt.point(37, 20), 0, Qt.point(43, 28)]],
                               ["ge", [0, Qt.point(33, 29), 0, Qt.point(36, 32)]],
                               ["ma", [0, Qt.point(25, 45), 0, Qt.point(30, 49)]],
                               ["os", [0, Qt.point(31, 26), 0, Qt.point(34, 29)]],
                               ["pf", [0, Qt.point(29, 31), 0, Qt.point(33, 34)]],
                               ["ti", [0, Qt.point(33, 33)]],
                               ["yc", [0, Qt.point(34, 30)]],];


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
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_FLAK.animationData);
        var offset = Qt.point(-10, 5);
        BATTLEANIMATION_FLAK.loadSprite(sprite, unit, defender, weapon, "+fire", Qt.point(-10, 5), Qt.point(0, 0), 0, 2);
        var mgCount = data[0];
        if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
        {
            var mgData = Global.getDataFromTable(armyName, BATTLEANIMATION_FLAK.animationMgDataAir);
            for (var i2 = 0; i2 < mgCount; ++i2)
            {
                sprite.loadSprite("mg_shot_air+flak" + data[3],  false, sprite.getMaxUnitCount(),
                                        mgData[1 + i2 * 2],
                                  data[2], 1, mgData[0 + i2 * 2], 100 * i2);
            }
        }
        else
        {
            var mgData = Global.getDataFromTable(armyName, BATTLEANIMATION_FLAK.animationMgData);
            for (var i2 = 0; i2 < mgCount; ++i2)
            {
                sprite.loadSprite("mg_shot+flak" + data[3],  false, sprite.getMaxUnitCount(),
                                        mgData[1 + i2 * 2],
                                  data[2], 1, mgData[0 + i2 * 2], 100 * i2);
            }
        }
        for (var i = 0; i < count; i++)
        {
            var soundCount = 1;
            if (mgCount > 1)
            {
                soundCount = 2;
            }
            for (var i2 = 0; i2 < soundCount; ++i2)
            {
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 100 * i2 +       i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 100 * i2 + 200 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 100 * i2 + 400 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("vulcan_weapon_fire.wav", 1, 100 * i2 + 600 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_FLAK.getMaxUnitCount());
        var player = unit.getOwner();
        var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_FLAK.armyData);
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_FLAK.animationData);
        return data[1] + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_FLAK.getMaxUnitCount());
    };

    this.getFiredDurationMS = function(sprite, unit, defender, weapon)
    {
        return 800 - BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_FLAK.getMaxUnitCount());
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
        return 400 - BATTLEANIMATION.defaultFrameDelay + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_FLAK.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_FLAK = new Constructor();
