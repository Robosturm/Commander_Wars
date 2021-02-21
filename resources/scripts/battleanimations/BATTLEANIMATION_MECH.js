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
        var terrainId = "PLAINS";
        var terrain = unit.getTerrain();
        if (terrain !== null)
        {
            unit.getTerrain().getTerrainID();
        }
        if (terrainId === "RIVER" ||
            terrainId === "DESERT_TRY_RIVER")
        {
            return "+river";
        }
        return "";
    };

    this.isMountain = function(terrainId)
    {
        if (terrainId === "MOUNTAIN" ||
            terrainId === "SNOW_MOUNTAIN" ||
            terrainId === "DESERT_ROCK")
        {
            return true
        }
        return false;
    };

    this.loadMoveInAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 1 || defender === null)
        {
            var terrainId = unit.getTerrain().getTerrainID();
            if (BATTLEANIMATION_MECH.isMountain(terrainId))
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
            if (BATTLEANIMATION_MECH.isMountain(terrainId))
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
            var offset = Qt.point(11, 19);
            if (armyName === "yc")
            {
                offset = Qt.point(11, 20);
            }
            else if (armyName === "ge")
            {
                weaponRes = "bazooka_ge";
                offset = Qt.point(5, 18);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(6, 20);
            }
            else if (armyName === "bh")
            {
                weaponRes = "bazooka_bh"
                offset = Qt.point(4, 17);
            }
            sprite.loadMovingSprite(weaponRes, false, sprite.getMaxUnitCount(), offset,
                                    Qt.point(127, 0), 400, false,
                                    1, 1, -1);
            offset.x = (offset.x - 20);
            sprite.loadSprite("bazooka_launch_start",  false, BATTLEANIMATION_MECH.getMaxUnitCount(), offset);
            if (armyName !== "ge")
            {
                offset.x = -37;
                offset.y = (offset.y - 12);
                sprite.loadSprite("bazooka_launch",  false, BATTLEANIMATION_MECH.getMaxUnitCount(), offset);
            }
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

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            sprite.loadColorOverlayForLastLoadedFrame("#969696", 1000, 1, 300);
        }
        else
        {
            sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 2, 0);
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


    this.getPositionOffset = function(sprite, unit, terrain, unitIdx)
    {
        if (terrain !== null)
        {
            if (BATTLEANIMATION_MECH.isMountain(terrain.getID()))
            {
                if (unitIdx >= 4)
                {
                    return Qt.point(-20 * (6 - unitIdx), 0);
                }
            }
        }
        return Qt.point(0, 0);
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

    this.getStopDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_MECH.getMaxUnitCount();
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_MECH.getMaxUnitCount();
    };

    this.getImpactDurationMS = function(sprite, unit, defender, weapon)
    {
        if (weapon === 0)
        {
            return 1500 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_MECH.getMaxUnitCount();
        }
        else
        {
            return 1000;
        }
    }
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_MECH = new Constructor();
