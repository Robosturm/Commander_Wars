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

    this.getRiverString = function(unit)
    {
        var terrainId = unit.getTerrain().getTerrainID();
        if (terrainId === "RIVER" ||
            terrainId === "DESERT_TRY_RIVER")
        {
            return "+river";
        }
        return "";
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1 || defender === null)
        {
            var terrainId = unit.getTerrain().getTerrainID();
            if (terrainId === "MOUNTAIN")
            {
                BATTLEANIMATION_MECH.loadStandingAnimation(sprite, unit, defender, weapon);
            }
            else
            {
                var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_MECH.armyData);
                var riverName = BATTLEANIMATION_MECH.getRiverString(unit);
                sprite.loadMovingSprite("mech+" + armyName + riverName + "+walk", false, sprite.getMaxUnitCount(), Qt.point(-75, 5),
                                        Qt.point(65, 0), 600, false,
                                        1, 1);
                sprite.loadMovingSpriteV2("mech+" + armyName + riverName + "+walk+mask", GameEnums.Recoloring_Table, sprite.getMaxUnitCount(), Qt.point(-75, 5),
                                          Qt.point(65, 0), 600, false,
                                          1, 1);
            }
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadMoveInAnimation(sprite, unit, defender, weapon);
        }
    };

    this.loadStopAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1 || defender === null)
        {
            var terrainId = unit.getTerrain().getTerrainID();
            if (terrainId === "MOUNTAIN")
            {
                BATTLEANIMATION_MECH.loadStandingAnimation(sprite, unit, defender, weapon);
            }
            else
            {
                BATTLEANIMATION_MECH.loadSprite(sprite, unit, defender, weapon, "+stop", 1);
            }
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadStopAnimation(sprite, unit, defender, weapon);
        }
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, count)
    {
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_MECH.armyData);
        var riverName = BATTLEANIMATION_MECH.getRiverString(unit);
        var offset = Qt.point(-10, 5);
        sprite.loadSprite("mech+" + armyName + riverName + ending,  false,
                          BATTLEANIMATION_MECH.getMaxUnitCount(), offset, count);
        sprite.loadSpriteV2("mech+" + armyName + riverName + ending + "+mask", GameEnums.Recoloring_Table,
                          BATTLEANIMATION_MECH.getMaxUnitCount(), offset, count);
    };

    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1 || defender === null)
        {
            BATTLEANIMATION_MECH.loadSprite(sprite, unit, defender, weapon, "", 1);
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadStandingAnimation(sprite, unit, defender, weapon);
        }
    };

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1)
        {
            BATTLEANIMATION_MECH.loadSprite(sprite, unit, defender, weapon, "+fire", 1);
            var weaponRes = "bazooka_os";
            var player = unit.getOwner();
            // get army name
            var armyName = Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_MECH.armyData);
            var offset = Qt.point(22, 23);
            if (armyName === "yc")
            {
                offset = Qt.point(21, 17);
            }
            else if (armyName === "ge")
            {
                weaponRes = "bazooka_ge";
                offset = Qt.point(23, 15);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(22, 17);
            }
            else if (armyName === "bh")
            {
                weaponRes = "bazooka_bh"
                offset = Qt.point(20, 14);
            }
            sprite.loadMovingSprite(weaponRes, false, sprite.getMaxUnitCount(), offset,
                                    Qt.point(127, 0), 400, false,
                                    1, 1, -1);
            sprite.loadSound("rocket_launch.wav", 1, "resources/sounds/", 0);
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadFireAnimation(sprite, unit, defender, weapon);
        }
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1)
        {
            BATTLEANIMATION_MECH.loadSprite(sprite, unit, defender, weapon, "+fired", 1);
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadStandingAnimation(sprite, unit, defender, weapon);
        }
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1)
        {
            sprite.loadSprite("unit_explosion",  false, sprite.getMaxUnitCount(), Qt.point(0, 20),
                              1, 1.0, 0, 300);
            sprite.loadMovingSprite("bazooka_os", false, sprite.getMaxUnitCount(), Qt.point(127, 24),
                                    Qt.point(-127, 0), 400, true,
                                    1, 1, 0, 0, true);
            sprite.loadSound("rocket_flying.wav", 1, "resources/sounds/", 0);
            sprite.loadSound("impact_explosion.wav", 1, "resources/sounds/", 200);
        }
        else
        {
            sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                              1, 1.0, 0, 0);
            sprite.loadSound("mg_impact.wav", 1, "resources/sounds/", 0);
        }
    };

    this.getImpactDurationMS = function()
    {
        // should be a second or longer.
        // the time will be scaled with animation speed inside the engine
        return 1500;
    }

    this.getPositionOffset = function(sprite, unit, terrain, unitIdx)
    {
        if (terrain !== null)
        {
            if (terrain.getID() === "MOUNTAIN")
            {
                if (unitIdx >= 4)
                {
                    return Qt.point(-20 * (6 - unitIdx), 0);
                }
            }
        }
        return Qt.point(0, 0);
    };

    this.getFireDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 500;
    };

    this.hasMoveInAnimation = function()
    {
        // return true if the unit has an implementation for loadMoveInAnimation
        return true;
    };
    this.getMoveInDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 610;
    };

    this.getStopDurationMS = function()
    {
        // the time will be scaled with animation speed inside the engine
        return 300;
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_MECH = new Constructor();
