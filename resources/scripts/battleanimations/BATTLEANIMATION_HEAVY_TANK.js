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
                     ["gs", "gs"],
                     ["ma", "ma"],
                     ["os", "os"],
                     ["pf", "pf"],
                     ["ti", "ti"],
                     ["yc", "yc"],];

    this.animationData = [["ac", [Qt.point(16, 28), Qt.point(17, 51), Qt.point(17, 51)]],
                          ["bd", [Qt.point(23, 18), Qt.point(19, 46), Qt.point(19, 41)]],
                          ["bh", [Qt.point(15, 26), Qt.point(17, 52), Qt.point(17, 46)]],
                          ["bm", [Qt.point(16, 21), Qt.point(16, 52), Qt.point(16, 45)]],
                          ["dm", [Qt.point(17, 21), Qt.point(16, 47), Qt.point(17, 42)]],
                          ["ge", [Qt.point(15, 27), Qt.point(17, 52), Qt.point(17, 46)]],
                          ["gs", [Qt.point(21, 28), Qt.point(22, 54), Qt.point(21, 47)]],
                          ["ma", [Qt.point(8, 29),  Qt.point(36, 29), Qt.point(36, 22)]],
                          ["os", [Qt.point(15, 22), Qt.point(9, 52),  Qt.point(8, 46)]],
                          ["pf", [Qt.point(16, 25), Qt.point(21, 52), Qt.point(21, 47)]],
                          ["ti", [Qt.point(21, 27), Qt.point(23, 54), Qt.point(23, 49)]],
                          ["yc", [Qt.point(15, 26), Qt.point(17, 52), Qt.point(17, 46)]],];

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount());
        var startX = -90;
        BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "+move", -1, Qt.point(startX, 5), Qt.point(65, 0), 600)
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(startX - 20, 7),
                                Qt.point(65, 0), 600, false,
                                1, 1);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("heavy_tank_move.wav", 5, i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "+stop", 1);
        var armyName = BATTLEANIMATION_HEAVY_TANK.getArmyName(unit);
        var offset = Qt.point(-45, 7);
        sprite.loadSprite("vehicle_dust_stop",  false,
                          BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount(), offset, 1);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "", 1);
    };

    this.getArmyName = function(unit)
    {
        var player = unit.getOwner();
        // get army name
        return Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_HEAVY_TANK.armyData);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, loops = -1, startPos = Qt.point(-25, 5), movement = Qt.point(0, 0), moveTime = 0)
    {
        var armyName = BATTLEANIMATION_HEAVY_TANK.getArmyName(unit);
        var weaponId = "";
        if (weapon === 1)
        {
            weaponId = "+mg";
        }
        sprite.loadMovingSpriteV2("heavy_tank+" + armyName + weaponId + ending + "+mask", GameEnums.Recoloring_Matrix,
                            BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount(), startPos, movement, moveTime, false, loops);
        BATTLEANIMATION.loadSpotterOrCoMini(sprite, unit, false);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        // get army name
        var armyName = BATTLEANIMATION_HEAVY_TANK.getArmyName(unit);
        var offset = Qt.point(0, 0);
        var count = sprite.getUnitCount(BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount());
        var data = Global.getDataFromTable(armyName, BATTLEANIMATION_HEAVY_TANK.animationData);
        if (weapon === 0)
        {
            BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "+fire", 1);
            offset = data[0];
            sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1.0, 0, 120);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("tank_shot.wav", 1, 120 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
            {
                BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "+fire+air", 3);
                offset = data[1];
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
            else
            {
                BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "+fire", 3);
                offset = data[2];
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
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
        return 600 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount());
    };

    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        return true;
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        return 610;
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        return 500 + BATTLEANIMATION.defaultFrameDelay * sprite.getUnitCount(BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount());
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HEAVY_TANK = new Constructor();
