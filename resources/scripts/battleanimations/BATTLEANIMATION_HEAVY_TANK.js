var Constructor = function()
{
    this.getMaxUnitCount = function()
    {
        return 5;
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        var armyName = BATTLEANIMATION_HEAVY_TANK.getArmyName(unit);
        sprite.loadMovingSprite("heavy_tank+" + armyName + "+move", false, sprite.getMaxUnitCount(), Qt.point(-85, 5),
                                Qt.point(65, 0), 400, false,
                                1, 1);
        sprite.loadMovingSpriteV2("heavy_tank+" + armyName + "+move+mask", GameEnums.Recoloring_Table, sprite.getMaxUnitCount(), Qt.point(-80, 5),
                                  Qt.point(65, 0), 400, false,
                                  1, 1);
        sprite.loadMovingSprite("vehicle_dust", false, sprite.getMaxUnitCount(), Qt.point(-105, 7),
                                Qt.point(65, 0), 400, false,
                                1, 1);
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "+stop");
        sprite.loadSprite("vehicle_dust_stop",  false,
                          BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount(), Qt.point(-40, 7), 1);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "");
    };

    this.getArmyName = function(unit)
    {
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "yc") &&
                (armyName !== "ge") &&
                (armyName !== "bm") &&
                (armyName !== "bh") &&
                (armyName !== "ma"))
        {
            armyName = "os";
        }
        return armyName;
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending)
    {
        var armyName = BATTLEANIMATION_HEAVY_TANK.getArmyName(unit);
        var offset = Qt.point(-20, 5);
        if (armyName === "ma")
        {
            offset = Qt.point(-30, 5);
        }
        sprite.loadSprite("heavy_tank+" + armyName + ending,  false,
                          BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount(), offset, 1);
        sprite.loadSpriteV2("heavy_tank+" + armyName + ending + "+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_HEAVY_TANK.getMaxUnitCount(), offset, 1);
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        // get army name
        var armyName = BATTLEANIMATION_HEAVY_TANK.getArmyName(unit);
        var offset = Qt.point(0, 0);
        if (weapon === 0)
        {
            BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "+fire");
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
                offset = Qt.point(23, 16);
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
            sprite.loadSound("tank_shot.wav", 1, "resources/sounds/", 120);
        }
        else
        {
            if (BATTLEANIMATION.getRelativePosition(unit, defender) > 0)
            {
                BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "mg_fire_air");
                offset = Qt.point(24, 47);
                if (armyName === "yc")
                {
                    offset = Qt.point(32, 48);
                }
                else if (armyName === "ge")
                {
                    offset = Qt.point(33, 47);
                }
                else if (armyName === "bm")
                {
                    offset = Qt.point(32, 46);
                }
                else if (armyName === "bh")
                {
                    offset = Qt.point(33, 47);
                }
                else if (armyName === "ma")
                {
                    offset = Qt.point(41, 20);
                }
                sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                                  1, 1, 0, 0);
            }
            else
            {
                BATTLEANIMATION_HEAVY_TANK.loadSprite(sprite, unit, defender, weapon, "mg_fire");
                // mg
                offset = Qt.point(24, 47);
                if (armyName === "yc")
                {
                    offset = Qt.point(32, 48);
                }
                else if (armyName === "ge")
                {
                    offset = Qt.point(33, 47);
                }
                else if (armyName === "bm")
                {
                    offset = Qt.point(32, 46);
                }
                else if (armyName === "bh")
                {
                    offset = Qt.point(33, 47);
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

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 620;
    };

    this.hasMoveInAnimation = function()
    {
        // return true if the unit has an implementation for loadMoveInAnimation
        return true;
    };
    this.getMoveInDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 410;
    };

    this.getStopDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 300;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_HEAVY_TANK = new Constructor();
