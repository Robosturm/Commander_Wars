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

    this.getArmyName = function(unit)
    {
        var player = unit.getOwner();
        // get army name
        return Global.getArmyNameFromPlayerTable(player, BATTLEANIMATION_LIGHT_TANK.armyData);
    };

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
        var terrainId = unit.getTerrain().getTerrainID();
        if (BATTLEANIMATION_INFANTRY.isMountain(terrainId))
        {
            BATTLEANIMATION_INFANTRY.loadStandingAnimation(sprite, unit, defender, weapon);
        }
        else
        {
            var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_INFANTRY.armyData);
            var riverName = BATTLEANIMATION_INFANTRY.getRiverString(unit);
            sprite.loadMovingSprite("infantry+" + armyName + riverName + "+walk", false, sprite.getMaxUnitCount(), Qt.point(-75, 5),
                                    Qt.point(65, 0), 600, false,
                                    1, 1);
            sprite.loadMovingSpriteV2("infantry+" + armyName + riverName + "+walk+mask", GameEnums.Recoloring_Table, sprite.getMaxUnitCount(), Qt.point(-75, 5),
                                      Qt.point(65, 0), 600, false,
                                      1, 1);
        }
    };

    this.loadSprite = function(sprite, unit, defender, weapon, ending, count)
    {
        var armyName = Global.getArmyNameFromPlayerTable(unit.getOwner(), BATTLEANIMATION_INFANTRY.armyData);
        var riverName = BATTLEANIMATION_INFANTRY.getRiverString(unit);
        var offset = Qt.point(-10, 5);
        sprite.loadSprite("infantry+" + armyName + riverName + ending,  false,
                          BATTLEANIMATION_INFANTRY.getMaxUnitCount(), offset, count);
        sprite.loadSpriteV2("infantry+" + armyName + riverName + ending + "+mask", GameEnums.Recoloring_Table,
                            BATTLEANIMATION_INFANTRY.getMaxUnitCount(), offset, count);
    };


    this.loadStandingAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_INFANTRY.loadSprite(sprite, unit, defender, weapon, "", 1);
    };

    this.loadStandingFiredAnimation = function(sprite, unit, defender, weapon)
    {
        BATTLEANIMATION_INFANTRY.loadSprite(sprite, unit, defender, weapon, "", 1);
    };

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

    this.loadFireAnimation = function(sprite, unit, defender, weapon)
    {
        var armyName = BATTLEANIMATION_INFANTRY.getArmyName(unit);
        var offset = Qt.point(0, 0);
        var position = BATTLEANIMATION.getRelativePosition(unit, defender);
        if (position > 0)
        {
            BATTLEANIMATION_INFANTRY.loadSprite(sprite, unit, defender, weapon, "+fire+up", 3);
            offset = Qt.point(14, 20);
            if (armyName === "yc")
            {
                offset = Qt.point(21, 24);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(14, 22);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(15, 25);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(18, 22);
            }
            else if (armyName === "ma")
            {
                offset = Qt.point(16, 19);
            }
            sprite.loadSprite("mg_shot_air",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
        }
        else if (position < 0)
        {
            BATTLEANIMATION_INFANTRY.loadSprite(sprite, unit, defender, weapon, "+fire+down", 3);
            offset = Qt.point(14, 15);
            if (armyName === "yc")
            {
                offset = Qt.point(21, 20);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(14, 17);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(15, 20);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(18, 17);
            }
            else if (armyName === "ma")
            {
                offset = Qt.point(16, 19);
            }
            offset.y = offset.y - 9;
            offset.x = offset.x - 1;
            sprite.loadSprite("mg_shot_down",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
        }
        else
        {
            BATTLEANIMATION_INFANTRY.loadSprite(sprite, unit, defender, weapon, "+fire", 3);
            offset = Qt.point(14, 17);
            if (armyName === "yc")
            {
                offset = Qt.point(21, 22);
            }
            else if (armyName === "ge")
            {
                offset = Qt.point(14, 19);
            }
            else if (armyName === "bm")
            {
                offset = Qt.point(15, 23);
            }
            else if (armyName === "bh")
            {
                offset = Qt.point(18, 20);
            }
            else if (armyName === "ma")
            {
                offset = Qt.point(16, 19);
            }
            offset.y = offset.y - 5;
            sprite.loadSprite("mg_shot",  false, sprite.getMaxUnitCount(), offset,
                              1, 1, 0, 0);
        }
    };

    this.loadImpactUnitOverlayAnimation = function(sprite, unit, defender, weapon)
    {
        sprite.loadColorOverlayForLastLoadedFrame("#969696", 300, 3, 0);
    };

    this.loadImpactAnimation = function(sprite, unit, defender, weapon)
    {
        var count = sprite.getUnitCount(BATTLEANIMATION_INFANTRY.getMaxUnitCount());
        sprite.loadSprite("mg_hit",  false, sprite.getMaxUnitCount(), Qt.point(0, 22),
                          1, 1.0, 0, 0);
        for (var i = 0; i < count; i++)
        {
            sprite.loadSound("mg_impact.wav", 1, "resources/sounds/", i * BATTLEANIMATION.defaultFrameDelay);
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
        return 300 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_INFANTRY.getMaxUnitCount();
    };

    this.getFireDurationMS = function(sprite, unit, defender, weapon)
    {
        // the time will be scaled with animation speed inside the engine
        return 610 + BATTLEANIMATION.defaultFrameDelay * BATTLEANIMATION_INFANTRY.getMaxUnitCount();
    };
};

Constructor.prototype = BATTLEANIMATION;
var BATTLEANIMATION_INFANTRY = new Constructor();
