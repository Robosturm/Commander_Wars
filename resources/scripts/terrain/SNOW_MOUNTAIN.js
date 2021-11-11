var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(2);
        terrain.setTerrainName(SNOW_MOUNTAIN.getName());
    };
    this.getName = function()
    {
        return qsTr("Snowy Mountain");
    };
    this.getDefense = function()
    {
        return 4;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS");
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE");
        }
        else
        {
            terrain.loadBaseTerrain("SNOW");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("MOUNTAIN,DESERT_ROCK,SNOW_MOUNTAIN,WASTE_MOUNTAIN", false, false, GameEnums.Directions_Direct, false);
        var itemCount = surroundings.split("+").length - 1;
        if (itemCount === 4)
        {
            terrain.loadBaseSprite("snow_mountain");
        }
        else
        {
            terrain.loadBaseSprite("snow_mountain+short");
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_snow_mountain";
    };
    this.getBonusVision = function(unit)
    {
        if (unit.getUnitType() === GameEnums.UnitType_Infantry)
        {
            return 3;
        }
        else
        {
            return 0;
        }
    };
    this.getDescription = function()
    {
        return qsTr("<r>Clear view. In Fog of War, Infantry unit's gain </r><div c='#00ff00'>vision +3.</div><r> Extremly high movement costs for infantry units.</r>");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["snow_mountain.png",
                "snow_mountain+short"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var variables = terrain.getVariables();
        var variable = variables.getVariable("BACKGROUND_ID");
        var rand = 0;
        if (variable === null)
        {
            rand = globals.randInt(0, 1);
            variable = variables.createVariable("BACKGROUND_ID");
            variable.writeDataInt32(rand);
        }
        else
        {
            rand = variable.readDataInt32();
        }
        return "back_snowmountain+" + rand.toString();
    };
};
Constructor.prototype = TERRAIN;
var SNOW_MOUNTAIN = new Constructor();
