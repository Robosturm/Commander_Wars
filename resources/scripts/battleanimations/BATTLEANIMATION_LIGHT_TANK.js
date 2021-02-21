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
        var armyName = BATTLEANIMATION_HEAVY_TANK.getArmyName(unit);
        sprite.loadMovingSprite("light_tank+" + armyName + "+move", false, sprite.getMaxUnitCount(), Qt.point(-70, 5),
                                Qt.point(65, 0), 600, false,
                                1, 1);
        sprite.loadMovingSpriteV2("light_tank+" + armyName + "+move+mask", GameEnums.Recoloring_Table, sprite.getMaxUnitCount(), Qt.point(-70, 5),
                                  Qt.point(65, 0), 600, false,
                                  1, 1);
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(-90, 7),
                                Qt.point(65, 0), 600, false,
                                1, 1);
    };

    this.getArmyName = function(unit)
    {
        var player = unit.getOwner();
        // get army name
        return Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_LIGHT_TANK.armyData);
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, count)
    {
        var armyName = BATTLEANIMATION_LIGHT_TANK.getArmyName(unit);
        var offset = Qt.point(-5, 5);
        if (armyName === "ma")
        {
            offset = Qt.point(-20, 5);
        }
        sprite.loadSprite("light_tank+" + armyName + ending,  false,
                          BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount(), offset, count);
        sprite.loadSpriteV2("light_tank+" + armyName + ending + "+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount(), offset, count);
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_LIGHT_TANK.loadSprite(sprite, unit, defender, weapon, "+stop", 1);
        sprite.loadSprite("vehicle_dust_stop",  false,
                          BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount(), Qt.point(-25, 7), 1);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_LIGHT_TANK.loadSprite(sprite, unit, defender, weapon, "", 1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        // get army name
        var armyName = BATTLEANIMATION_LIGHT_TANK.getArmyName(unit);
        var offset = Qt.point(0, 0);
        if (weapon === 0)
        {
            // gun
            BATTLEANIMATION_LIGHT_TANK.loadSprite(sprite, unit, defender, weapon, "+fire", 1);
            offset = Qt.point(21, 21);
            if (armyName === "yc")
            {
                offset = Qt.point(29, 16);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(32, 21);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(32, 16);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(31, 21);
            }
            else if (armyName === "ma")
            {
                offset = Qt.point(17, 37);
            }
            sprite.loadSprite("medium_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 0.5, 0, 0);
            sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", 0);
        }
        else
        {
            BATTLEANIMATION_LIGHT_TANK.loadSprite(sprite, unit, defender, weapon, "+fire_mg", 3);
            if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
            {
                // mg
                offset = Qt.point(24, 43);
                if (armyName === "yc")
                {
                    offset = Qt.point(35, 40);
                }
                else if (armyName === "ge")
                {
                    offset = Qt.point(36, 41);
                }
                else if (armyName === "bm")
                {
                    offset = Qt.point(35, 39);
                }
                else if (armyName === "bh")
                {
                    offset = Qt.point(32, 40);
                }
                else if (armyName === "ma")
                {
                    offset = Qt.point(41, 24);
                }
                sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
            else
            {
                // mg
                offset = Qt.point(24, 38);
                if (armyName === "yc")
                {
                    offset = Qt.point(35, 36);
                }
                else if (armyName === "ge")
                {
                    offset = Qt.point(36, 37);
                }
                else if (armyName === "bm")
                {
                    offset = Qt.point(35, 35);
                }
                else if (armyName === "bh")
                {
                    offset = Qt.point(32, 36);
                }
                else if (armyName === "ma")
                {
                    offset = Qt.point(41, 20);
                }
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
        }
    };

    this.hasMoveInAnimation = function()
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
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount();
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 820 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_LIGHT_TANK.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_LIGHT_TANK = new Constructor();
