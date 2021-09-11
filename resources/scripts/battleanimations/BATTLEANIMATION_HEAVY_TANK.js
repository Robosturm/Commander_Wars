var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };
    this.armyData = [["os", "os"],
                     ["bm", "bm"],
                     ["ge", "ge"],
                     ["yc", "yc"],
                     ["bh", "bh"],
                     ["bg", "bh"],
                     ["ma", "ma"],];
    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
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
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        // get army name
        var armyName = BATTLEANIMATION_HEAVY_TANK.getArmyName(unit);
        var offset = Qt.point(0, 0);
        var count = sprite.getUnitCount(BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount());
        if (weapon === 0)
        {
            BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "+fire", 1);
            offset = Qt.point(15, 22);
            if (armyName === "yc")
            {
                offset = Qt.point(15, 26);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(15, 27);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(16, 21);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(15, 26);
            }
            else if (armyName === "ma")
            {
                offset = Qt.point(28, 40);
            }
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
                offset = Qt.point(9, 52);
                if (armyName === "yc")
                {
                    offset = Qt.point(17, 52);
                }
                else if (armyName === "ge")
                {
                    offset = Qt.point(17, 52);
                }
                else if (armyName === "bm")
                {
                    offset = Qt.point(16, 52);
                }
                else if (armyName === "bh")
                {
                    offset = Qt.point(17, 52);
                }
                else if (armyName === "ma")
                {
                    offset = Qt.point(35, 25);
                }
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
            else
            {
                BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "+fire", 3);
                offset = Qt.point(8, 46);
                if (armyName === "yc")
                {
                    offset = Qt.point(17, 46);
                }
                else if (armyName === "ge")
                {
                    offset = Qt.point(17, 46);
                }
                else if (armyName === "bm")
                {
                    offset = Qt.point(16, 45);
                }
                else if (armyName === "bh")
                {
                    offset = Qt.point(17, 46);
                }
                else if (armyName === "ma")
                {
                    offset = Qt.point(35, 21);
                }
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


    this.hasMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        // return true if the unit has an implementation for loadMoveInAnimation
        return true;
    };
    this.getMoveInDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 610;
    };

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount();
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 820 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HEAVY_TANK = new Constructor();
