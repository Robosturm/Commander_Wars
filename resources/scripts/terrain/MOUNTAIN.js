var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(2);
        terrain.setTerrainName(MOUNTAIN.getName());
    };
    this.getName = function()
    {
        return qsTr("Mountain");
    };
    this.getDefense = function()
    {
        return 4;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
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
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("MOUNTAIN", false, false, GameEnums.Directions_North, false);
        surroundings += terrain.getSurroundings("MOUNTAIN", false, false, GameEnums.Directions_South, false);
        if (surroundings !== "")
        {
            terrain.loadBaseSprite("mountain" + surroundings);
        }
        else
        {
            surroundings = terrain.getSurroundings("MOUNTAIN", false, false, GameEnums.Directions_East, false);
            surroundings += terrain.getSurroundings("MOUNTAIN", false, false, GameEnums.Directions_West, false);
            terrain.loadBaseSprite("mountain" + surroundings);
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_mountain";
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
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var rand = globals.randInt(0, 1);
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "back_" + weatherModifier + "mountain+" + rand.toString();
    };
    this.getDescription = function()
    {
        return qsTr("<r>Clear view. In Fog of War, Infantry unit's gain </r><div c='#00ff00'>vision +3.</div>");
    };
    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["mountain.png",
                "mountain+E",
                "mountain+E+W",
                "mountain+W"];
    };
};
Constructor.prototype = TERRAIN;
var MOUNTAIN = new Constructor();
