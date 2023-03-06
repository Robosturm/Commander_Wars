var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 0;
    };
    this.getDefaultPalette = function()
    {
        return "palette_clear";
    };
    this.init = function (terrain)
    {
        terrain.setPalette(ROUGH_SEA.getDefaultPalette());
        terrain.setVisionHigh(1);
        terrain.setTerrainName(ROUGH_SEA.getName());
    };
    this.getName = function()
    {
        return qsTr("Rough Sea");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID, map, currentPalette)
    {
        if (currentTerrainID === "LAKE")
        {
            terrain.loadBaseTerrain("LAKE", currentPalette);
        }
        else if (currentTerrainID === "SEA")
        {
            terrain.loadBaseTerrain("SEA", currentPalette);
        }
        else
        {
            terrain.loadBaseTerrain("SEA");
        }
    };
    this.loadBaseSprite = function(terrain, map)
    {
        terrain.loadBaseSprite("rough_sea+N+E+S+W+mask");
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
        var baseId = terrain.getBaseTerrainID();
        if (baseId === "SEA")
        {
            SEA.loadOverlaySprite(terrain, map);
        }
        else
        {
            LAKE.loadOverlaySprite(terrain, map);
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_rough_sea";
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
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
    this.getEditorPlacementSound = function()
    {
        return "placeReaf.wav";
    };
};
Constructor.prototype = TERRAIN;
var ROUGH_SEA = new Constructor();
