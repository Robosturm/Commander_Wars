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
        var count = sprite.getUnitCount(BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount());
        var armyName = BATTLEANIMATION_HEAVY_TANK.getArmyName(unit);
        var startX = -85;
        if (armyName === "ma")
        {
            startX = -95;
        }
        sprite.loadMovingSprite("heavy_tank+" + armyName + "+move", false, sprite.getMaxUnitCount(), Qt.point(startX, 5),
                                Qt.point(65, 0), 600, false,
                                1, 1);
        sprite.loadMovingSpriteV2("heavy_tank+" + armyName + "+move+mask", GameEnums.Recoloring_Table, sprite.getMaxUnitCount(), Qt.point(startX, 5),
                                  Qt.point(65, 0), 600, false,
                                  1, 1);
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(startX - 20, 7),
                                Qt.point(65, 0), 600, false,
                                1, 1);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("heavy_tank_move.wav", 5, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
        }
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "+stop", 1);
        var armyName = BATTLEANIMATION_HEAVY_TANK.getArmyName(unit);
        var offset = Qt.point(-40, 7);
        if (armyName === "ma")
        {
            offset = Qt.point(-50, 7);
        }
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

    this.loadSprite = function(sprite, unit, defender, weapon, ending, count)
    {
        var armyName = BATTLEANIMATION_HEAVY_TANK.getArmyName(unit);
        var offset = Qt.point(-20, 5);
        if (armyName === "ma")
        {
            offset = Qt.point(-30, 5);
        }
        sprite.loadSprite("heavy_tank+" + armyName + ending,  false,
                          BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount(), offset, count);
        sprite.loadSpriteV2("heavy_tank+" + armyName + ending + "+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount(), offset, count);
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
            offset = Qt.point(22, 20);
            // gun
            if (armyName === "yc")
            {
                offset = Qt.point(22, 20);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(23, 22);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(21, 21);
                // 45 31
                //-20, 5
                // -7 15
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(23, 20);
            }
            else if (armyName === "ma")
            {
                offset = Qt.point(23, 40);
            }
            sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1.0, 0, 120);
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", 120 + i * BATTLEANIMATION.defaultFrameDelay);
            }
        }
        else
        {
            if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
            {
                BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "+fire_mg_air", 3);
                offset = Qt.point(13, 52);
                if (armyName === "yc")
                {
                    offset = Qt.point(21, 52);
                }
                else if (armyName === "ge")
                {
                    offset = Qt.point(22, 52);
                }
                else if (armyName === "bm")
                {
                    offset = Qt.point(21, 50);
                }
                else if (armyName === "bh")
                {
                    offset = Qt.point(22, 52);
                }
                else if (armyName === "ma")
                {
                    offset = Qt.point(30, 25);
                }
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
            else
            {
                BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "+fire_mg", 3);
                // mg
                offset = Qt.point(13, 46);
                if (armyName === "yc")
                {
                    offset = Qt.point(21, 47);
                }
                else if (armyName === "ge")
                {
                    offset = Qt.point(22, 46);
                }
                else if (armyName === "bm")
                {
                    offset = Qt.point(21, 45);
                }
                else if (armyName === "bh")
                {
                    offset = Qt.point(22, 46);
                }
                else if (armyName === "ma")
                {
                    offset = Qt.point(30, 21);
                }
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
            for (var i = 0; i < count; i++)
            {
                sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", 200 + i * BATTLEANIMATION.defaultFrameDelay);
                sprite.loadSound("mg_weapon_fire.wav", 1, "resources/sounds/", 400 + i * BATTLEANIMATION.defaultFrameDelay);
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
