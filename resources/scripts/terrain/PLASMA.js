var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Plasma"));
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
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("PLASMA,METEOR", false, false, GameEnums.Directions_Direct);
        terrain.loadBaseSprite("plasma" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_plasma";
    };
};
Constructor.prototype = TERRAIN;
var PLASMA = new Constructor();
