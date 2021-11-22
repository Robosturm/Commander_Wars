var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Hovercraft");
    };
    this.movementpointsTable = [["PLAINS", 1],
                                ["PLAINS_DESTROYED", 1],
                                ["PLAINS_PLASMA", 1],
                                ["BEACH", 1],
                                ["BRIDGE", 1],
                                ["BRIDGE1", 1],
                                ["DESTROYEDWELD", 1],
                                ["RUIN", 1],
                                ["STREET", 1],
                                ["SNOW_STREET", 1],
                                ["STREET1", 1],
                                ["AIRPORT", 1],
                                ["FACTORY", 1],
                                ["HARBOUR", 1],
                                ["HQ", 1],
                                ["PIPESTATION", 1],
                                ["RADAR", 1],
                                ["TOWER", 1],
                                ["TOWN", 1],
                                ["SILO", 1],
                                ["SILO_ROCKET", 1],
                                ["LABOR", 1],
                                ["TEMPORARY_AIRPORT", 1],
                                ["TEMPORARY_HARBOUR", 1],
                                ["OILRIG", 1],
                                ["RIVER", 1],
                                ["DESERT", 1],
                                ["DESERT_DESTROYEDWELD", 1],
                                ["DESERT_PATH", 1],
                                ["DESERT_PATH1", 1],
                                ["DESERT_TRY_RIVER", 1],
                                ["DESERT_RUIN", 1],
                                ["SNOW", 1],
                                ["SNOW_DESTROYEDWELD", 1],
                                ["FOREST", 3],
                                ["FOREST1", 3],
                                ["FOREST2", 3],
                                ["FOREST3", 3],
                                ["WASTELAND", 1],
                                ["DESERT_FOREST", 3],
                                ["DESERT_FOREST1", 3],
                                ["DESERT_WASTELAND", 1],
                                ["SNOW_FOREST", 3],
                                ["SNOW_FOREST1", 3],
                                ["SNOW_FOREST2", 3],
                                ["SNOW_WASTELAND", 1],
                                ["SNOW_RUIN", 1],
                                ["SEA", 2],
                                ["LAKE", 1],
                                ["FOG", 2],
                                ["ROUGH_SEA", 4],
                                ["REAF", 3],
                                ["WASTE",  1],
                                ["WASTE_PATH", 1],
                                ["WASTE_FOREST", 1],
                                ["WASTE_WASTELAND", 1],
                                ["WASTE_RUIN", 1],
                                ["WASTE_DESTROYEDWELD", 1],
                                ["CREEPER", 1],
                                ["TELEPORTTILE", 0]];

    this.getMovementpoints = function(terrain, unit, currentTerrain, trapChecking = false)
    {
        var id = terrain.getID();
        var baseId = terrain.getBaseTerrainID();
        var currentId = currentTerrain.getID();
        if ((id === "ZGATE_E_W" || id === "ZGATE_N_S") &&
            (unit !== null) &&
            (unit.getOwner().isAlly(terrain.getBuilding().getOwner())))
        {
            return 1;
        }
        var costs = MOVEMENTTABLE.getMovementpointsFromTable(terrain, MOVE_HOVERCRAFT.movementpointsTable);
        if (baseId === "LAKE")
        {
            return costs;
        }
        var currentGroup = currentTerrain.getTerrainGroup();
        var targetGroup = terrain.getTerrainGroup();
        // sea tile near land?
        if (currentGroup === 0)
        {
            var valid = false;
            var fields = globals.getCircle(1, 2);
            // check all fields we can attack
            for (var i = 0; i < fields.size(); i++)
            {
                var x = fields.at(i).x + terrain.getX();
                var y = fields.at(i).y + terrain.getY();
                // check with which weapon we can attack and if we could deal damage with this weapon
                if (map.onMap(x, y))
                {
                    if (map.getTerrain(x, y).getTerrainGroup() > 0)
                    {
                        // not a sea tile. -> land tile
                        valid = true;
                        break;
                    }
                }
            }
            fields.remove();
            if (!valid)
            {
                return -1;
            }
        }
        if (currentGroup === targetGroup)
        {
            return costs;
        }
        else
        {
            // from sea to land or vice versa
            if (currentGroup === 0 || targetGroup === 0)
            {
                // fields we can move from land to sea
                var crossable = ["HARBOUR", "BEACH", "TEMPORARY_HARBOUR", "RIVER", "TELEPORTTILE"];
                for (var i = 0; i < crossable.length; i++)
                {
                    if (crossable[i] === id ||
                        crossable[i] === currentId)
                    {
                        return costs;
                    }
                }
            }
            else
            {
                return costs;
            }
        }
        return -1;
    };
};
Constructor.prototype = MOVEMENTTABLE;
var MOVE_HOVERCRAFT = new Constructor();
