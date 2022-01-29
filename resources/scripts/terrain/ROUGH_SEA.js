var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 0;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(ROUGH_SEA.getName());
    };
    this.getName = function()
    {
        return qsTr("Rough Sea");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID, map)
    {
        if (currentTerrainID === "LAKE")
        {
            terrain.loadBaseTerrain("LAKE");
        }
        else
        {
            terrain.loadBaseTerrain("SEA");
        }
    };
    this.loadBaseSprite = function(terrain, map)
    {
        terrain.loadBaseSprite("rough_sea+N+E+S+W");
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.canBePlaced = function(x, y, map)
    {
        var terrain = map.getTerrain(x, y);
        var baseId = terrain.getBaseTerrainID();
        if (baseId === "SEA" ||
            baseId === "LAKE")
        {
            // it's a sea nice
            var surroundings = terrain.getSurroundings("SEA,LAKE", true, false, GameEnums.Directions_Direct);
            // we need sea all around us :)
            if (surroundings === "+N+E+S+W")
            {
                return true;
            }
        }
        return false;
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        SEA.loadOverlaySprite(terrain);
        LAKE.loadOverlaySprite(terrain);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_rough_sea";
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain, dfender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        return "back_" + weatherModifier +"sea";
    };
    this.getTerrainAnimationMoveSpeed = function()
    {
        return 1;
    };
    this.getDescription = function()
    {
        return qsTr("Movement of naval units is impeded but air units are not affected.");
    };
};
Constructor.prototype = TERRAIN;
var ROUGH_SEA = new Constructor();
