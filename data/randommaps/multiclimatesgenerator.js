var RANDOMMAPGENERATOR =
        {
    // functions or entries marked mandatory need to be implemented in any random map generator
    // new random map generators are not implemented like mods
    // they need to be a complete copy of this file and the variable filled has to have the name RANDOMMAPGENERATOR
    // during generation the generator is overwritten by the current selected one.
    // always need to be here
    getTerrainBaseChances : function()
    {
        return [30, 20, 20, 10, 0, 20];
    },
    getTerrainBases : function()
    {
        // the entry Buildings is mandatory
        return ["PLAINS", "DESERT", "SNOW", "RIVER", "SEA", "Buildings"];
    },
    getBuildingBaseChances : function()
    {
        return [20, 10, 10, 60, 0, 0];
    },
    getBuildingBases : function()
    {
        return ["FACTORY", "AIRPORT", "HARBOUR", "TOWN", "MINE", "OILRIG"];
    },
    getUnitBaseChances : function()
    {
        return [50, 10, 10, 20, 10];
    },
    getUnitBases : function()
    {
        return ["INFANTRY", "MECH", "ARTILLERY", "LIGHT_TANK", "FLAK"];
    },
    getRoadCreation : function(terrain)
    {
        // describes how roads from hq to hq are created on different tiles without a building on it
        var baseID = terrain.getBaseTerrainID();
        var terrainId = terrain.getTerrainID()
		// first item is the base terrain or empty for use the current
		// second item is the road placed on top of the tile
        if (baseID === "SEA")
        {
            return ["SEA", "BRIDGE"];
        }
		else if (terrainId === "RIVER")
		{
			return ["", "BRIDGE"];
		}
		else if (baseID === "SNOW")
        {
            return ["SNOW", "STREET"];
        }
		else if (baseID === "DESERT")
        {
            return ["DESERT", "DESERT_PATH"];
        }
        return ["PLAINS", "STREET"];
    },
	getBaseTerrainID : function()
	{
		// map is filled with this terrain at the start
		return "PLAINS";
	},
	// for each terrain we need
	// get + TerrainID + TopTerrainIDs and get + TerrainID + TopTerrainChances
    // each function returns the terrains that can be placed on top of the base terrain mainly for sea tiles + the chance it's gonna be placed
    // get + TerrainID + Distribution describes the chance of all terrains beeing connected to one mass
    // get + TerrainID + CreateType how the terrain is placed in random blob or more in a line like a river or road
	// get + terrainID + Placeable if a terrain can be placed at the given location
    getPLAINSTopTerrainIDs : function()
    {
        return ["FOREST", "MOUNTAIN"];
    },
    getPLAINSTopTerrainChances : function()
    {
        return [0.1, 0.1];
    },
    getPLAINSDistribution : function()
    {
        return Qt.point(1, 4);
    },
	getPLAINSPlaceable : function(x, y)
    {
        return true;
    },
    getPLAINSCreateType : function()
    {
        return GameEnums.RandomMapTerrainType_Group;
    },
    getRIVERTopTerrainIDs : function()
    {
        return ["BRIDGE"];
    },
    getRIVERTopTerrainChances : function()
    {
        return [0.05];
    },
    getRIVERDistribution : function()
    {
        return Qt.point(1, 3);
    },
	getRIVERPlaceable : function(x, y)
    {
        return true;
    },
    getRIVERCreateType : function()
    {
        return GameEnums.RandomMapTerrainType_Line;
    },
    getSNOWTopTerrainIDs : function()
    {
        return ["SNOW_FOREST", "SNOW_MOUNTAIN"];
    },
    getSNOWTopTerrainChances : function()
    {
        return [0.1, 0.1];
    },
    getSNOWDistribution : function()
    {
        return Qt.point(1, 4);
    },
	getSNOWPlaceable : function(x, y)
    {
        return true;
    },
    getSNOWCreateType : function()
    {
        return GameEnums.RandomMapTerrainType_Group;
    },
	getDESERTTopTerrainIDs : function()
    {
        return ["DESERT_FOREST", "DESERT_ROCK"];
    },
    getDESERTTopTerrainChances : function()
    {
        return [0.1, 0.1];
    },
    getDESERTDistribution : function()
    {
        return Qt.point(1, 4);
    },
	getDESERTPlaceable : function(x, y)
    {
        return true;
    },
    getDESERTCreateType : function()
    {
        return GameEnums.RandomMapTerrainType_Group;
    },
    getSEATopTerrainIDs : function()
    {
        return ["REAF", "ROUGH_SEA", "FOG", "BEACH"];
    },
    getSEATopTerrainChances : function()
    {
        return [0.05, 0.05, 0.05, 0.1];
    },
    getSEADistribution : function()
    {
        return Qt.point(1, 4);
    },
	getSEAPlaceable : function(x, y)
    {
        return true;
    },
    getSEACreateType : function()
    {
        return GameEnums.RandomMapTerrainType_Group;
    },
    // get + Building + Placeable -> returns if a building can be placed at the given location
    // get + Building + BaseTerrainID -> terrain that replaces the current one before the building is placed
    getFACTORYPlaceable : function(x, y)
    {
        //  mandatory
        return RANDOMMAPGENERATOR.getBuildingBaseTerrain(x, y);
    },
    getFACTORYBaseTerrainID : function()
    {
        //  mandatory
        return "PLAINS";
    },
    getAIRPORTPlaceable : function(x, y)
    {
        return RANDOMMAPGENERATOR.getBuildingBaseTerrain(x, y);
    },
    getAIRPORTBaseTerrainID : function()
    {
        return "PLAINS";
    },
    getHARBOURPlaceable : function(x, y)
    {
        for (var i = 0; i < 4; i++)
        {
            var x1 = x;
            var y1 = y;
            if (i === 0)
            {
                x1 = x1 + 1;
            }
            else if (i === 1)
            {
                x1 = x1 - 1;
            }
            else if (i === 2)
            {
                y1 = y1 + 1;
            }
            else if (i === 3)
            {
                y1 = y1 - 1;
            }
            if (map.onMap(x1, y1))
            {
                var id = map.getTerrain(x1, y1).getTerrainID();
                if (id === "SEA")
                {
                    return true;
                }
            }
        }
        return false;
    },
    getHARBOURBaseTerrainID : function()
    {
        return "PLAINS";
    },
    getTOWNPlaceable : function(x, y)
    {
        return RANDOMMAPGENERATOR.getBuildingBaseTerrain(x, y);
    },
    getTOWNBaseTerrainID : function()
    {
        return "PLAINS";
    },
    getMINEPlaceable : function(x, y)
    {
        return RANDOMMAPGENERATOR.getBuildingBaseTerrain(x, y);
    },
    getMINEBaseTerrainID : function()
    {
        return "PLAINS";
    },

    getHQPlaceable : function(x, y)
    {
        // mandatory
        return RANDOMMAPGENERATOR.getBuildingBaseTerrain(x, y);
    },
    getHQBaseTerrainID : function()
    {
        //  mandatory
        return "PLAINS";
    },
    getOILRIGPlaceable : function(x, y)
    {
        var id = map.getTerrain(x, y).getTerrainID();
        if (id === "SEA")
        {
            return true;
        }
        return false;
    },
    getOILRIGBaseTerrainID : function()
    {
        return "SEA";
    },
    getBuildingBaseTerrain : function(x, y)
    {
        var id = map.getTerrain(x, y).getBaseTerrainID();
        if (id === "PLAINS" ||
            id === "SNOW" ||
            id === "DESERT")
        {
            return true;
        }
        return false;

    },
	customStep : function()
	{
		// do what ever you like here
	},
};
