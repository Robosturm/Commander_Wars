var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(2);
        terrain.setTerrainName(WASTE_MOUNTAIN.getName());
    };
    this.getName = function()
    {
        return qsTr("Waste Rock");
    };
    this.getDefense = function()
    {
        return 3;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        var surroundings = terrain.getSurroundings("PLAINS,DESERT,SNOW,WASTE,MOUNTAIN,DESERT_ROCK,SNOW_MOUNTAIN,WASTE_MOUNTAIN,", false, false, GameEnums.Directions_North, false);
        var x = terrain.getX();
        var y = terrain.getY();     
        if (typeof map !== 'undefined')
        {
            if (map.onMap(x, y - 1))
            {
                var building = map.getTerrain(x, y - 1).getBuilding();
				if (building !== null)
				{
					surroundings = "";
				}
            }
        }
		if (surroundings === "")
        {
            terrain.loadBaseSprite("waste_rock+short");
        }
        else
        {
            terrain.loadBaseSprite("waste_rock");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("WASTE_MOUNTAIN", false, false, GameEnums.Directions_East, false);
        surroundings += terrain.getSurroundings("WASTE_MOUNTAIN", false, false, GameEnums.Directions_West, false);
        terrain.loadBaseSprite("waste_rock" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_waste_mountain";
    };
    this.getBonusVision = function(unit)
    {
        if (unit.getUnitType() === GameEnums.UnitType_Infantry)
        {
            return 4;
        }
        else
        {
            return 1;
        }
    };
    this.getDescription = function()
    {
        return qsTr("<r>Clear view. In Fog of War, Infantry unit's gain </r><div c='#00ff00'>vision +4.</div><r> Extremly high movement costs for infantry units. Waste terrain with reduced defense but clear view. In Fog of War, other unit's gain </r><div c='#00ff00'>vision +1.</div>");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["waste_rock.png",
                "waste_rock+short"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_wastemountain";
    };
};
Constructor.prototype = TERRAIN;
var WASTE_MOUNTAIN = new Constructor();
