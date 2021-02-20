var RANDOMMAPGENERATOR =
        {
    // functions or entries marked mandatory need to be implemented in any random map generator
    // new random map generators are not implemented like mods
    // they need to be a complete copy of this file and the variable filled has to have the name RANDOMMAPGENERATOR
    // during generation the generator is overwritten by the current selected one.
    // always need to be here
    getTerrainBaseChances : function()
    {
        return [50, 17, 16, 0, 0, 17];
    },
    getTerrainBases : function()
    {
        // the entry Buildings is mandatory
        return ["PLAINS", "FOREST", "MOUNTAIN", "RIVER", "SEA", "Buildings"];
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
        if (baseID === "SEA")
        {
            return ["SEA", "BRIDGE"];
        }
		else if (terrainId === "RIVER")
		{
			return ["", "BRIDGE"];
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
    getFORESTTopTerrainIDs : function()
    {
        return [];
    },
    getFORESTTopTerrainChances : function()
    {
        return [];
    },
    getFORESTDistribution : function()
    {
        return Qt.point(5, 10);
    },
	getFORESTPlaceable : function(x, y)
    {
        return true;
    },
    getFORESTCreateType : function()
    {
        return GameEnums.RandomMapTerrainType_Group;
    },	
	getPLAINSTopTerrainIDs : function()
    {
        return [];
    },
    getPLAINSTopTerrainChances : function()
    {
        return [];
    },
    getPLAINSDistribution : function()
    {
        return Qt.point(1, 1);
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
    getMOUNTAINTopTerrainIDs : function()
    {
        return [];
    },
    getMOUNTAINTopTerrainChances : function()
    {
        return [];
    },
    getMOUNTAINDistribution : function()
    {
        return Qt.point(2, 6);
    },
	getMOUNTAINPlaceable : function(x, y)
    {
        return true;
    },
    getMOUNTAINCreateType : function()
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
        var id = map.getTerrain(x, y).getTerrainID();
        if (id === "PLAINS" ||
                id === "FOREST" ||
                id === "MOUNTAIN")
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
